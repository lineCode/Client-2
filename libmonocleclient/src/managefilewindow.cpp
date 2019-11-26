// managefilewindow.cpp
//

///// Includes /////

#include "monocleclient/managefilewindow.h"

#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <QMessageBox>
#include <QValidator>
#include <utility/utility.hpp>

#include "monocleclient/browsefileswindow.h"
#include "monocleclient/device.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageFileWindow::ManageFileWindow(QWidget* parent, boost::shared_ptr<Device>& device) :
  QDialog(parent),
  device_(device),
  mountmodel_(new QStringListModel(this)),
  pathmodel_(new QStringListModel(this)),
  mountcompleter_(new QCompleter(this)),
  pathcompleter_(new QCompleter(this))
{
  ui_.setupUi(this);

  connect(device_.get(), &Connection::SignalMountPointAdded, this, &ManageFileWindow::SlotMountPointAdded, Qt::QueuedConnection);
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  ui_.editsize->setValidator(new QIntValidator(1, 536870912, this));

  if (!device_->SupportsGetChildFoldersFilter())
  {
    ui_.buttonbrowsemountpoint->setVisible(false);
    ui_.buttonbrowsepath->setVisible(false);
    setFixedHeight(minimumSizeHint().height());
  }

  if (device_->IsWindows()) // Windows doesn't care about mount points
  {
    ui_.labelmountpoint->setVisible(false);
    ui_.editmountpoint->setVisible(false);
    ui_.buttonbrowsemountpoint->setVisible(false);
    setFixedHeight(minimumSizeHint().height());
  }

  mountcompleter_->setModel(mountmodel_);
  mountcompleter_->setCompletionMode(QCompleter::PopupCompletion);
  mountcompleter_->setMaxVisibleItems(16);
  ui_.editmountpoint->setCompleter(mountcompleter_);

  pathcompleter_->setModel(pathmodel_);
  pathcompleter_->setCompletionMode(QCompleter::PopupCompletion);
  pathcompleter_->setMaxVisibleItems(16);
  ui_.editpath->setCompleter(pathcompleter_);

  const std::vector<MOUNTPOINT> mountpoints = device_->GetMountPoints();
  QStringList strings;
  strings.reserve(static_cast<int>(mountpoints.size()));
  for (const MOUNTPOINT& mountpoint : mountpoints)
  {
    strings.push_back(mountpoint.path_);

  }
  mountmodel_->setStringList(strings);
  pathmodel_->setStringList(strings);
}

ManageFileWindow::~ManageFileWindow()
{
  addfileconnection_.Close();

  for (monocle::client::Connection& getchildfoldersconnection : getchildfoldersconnections_)
  {
    getchildfoldersconnection.Close();

  }
}

void ManageFileWindow::SetEnabled(const bool enabled)
{
  ui_.editmountpoint->setEnabled(enabled);
  ui_.editpath->setEnabled(enabled);
  on_checkfilldisk_toggled(enabled);
  ui_.checkfilldisk->setEnabled(enabled);
  ui_.combochunksize->setEnabled(enabled);
  ui_.checkautomount->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageFileWindow::SlotMountPointAdded(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const QString& path, const QString& type, const QString& source)
{
  QStringList strings = mountmodel_->stringList();
  strings.push_back(path);
  mountmodel_->setStringList(strings);

  strings = pathmodel_->stringList();
  strings.push_back(path);
  pathmodel_->setStringList(strings);
}

void ManageFileWindow::on_buttonbrowsemountpoint_clicked()
{
  BrowseFilesWindow browsefileswindow(this, device_, ui_.editmountpoint->text());
  if (browsefileswindow.exec() == QDialog::Accepted)
  {
    ui_.editmountpoint->setText(browsefileswindow.GetLocation());

  }
}

void ManageFileWindow::on_buttonbrowsepath_clicked()
{
  BrowseFilesWindow browsefileswindow(this, device_, ui_.editpath->text());
  if (browsefileswindow.exec() == QDialog::Accepted)
  {
    ui_.editpath->setText(browsefileswindow.GetLocation());

  }
}

void ManageFileWindow::on_editpath_textEdited(const QString& text)
{
  if (!device_->SupportsGetChildFolders())
  {

    return;
  }

  const boost::filesystem::path path(text.toStdString());
  if (!path.has_parent_path())
  {

    return;
  }
  const std::string parentpath = path.parent_path().string();
  std::vector<std::string>::const_iterator i = std::find_if(requestpaths_.cbegin(), requestpaths_.cend(), [&parentpath](const std::string& path) { return (path == parentpath); });
  if (i == requestpaths_.cend())
  {
    requestpaths_.push_back(parentpath);
    getchildfoldersconnections_.push_back(device_->GetChildFolders(parentpath, true, [this](const std::chrono::steady_clock::duration latency, const monocle::client::GETCHILDFOLDERSRESPONSE& getchildfoldersresponse)
    {
      if (getchildfoldersresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        // Just ignore this
        return;
      }

      QStringList strings = pathmodel_->stringList();
      for (const std::string& childfolder : getchildfoldersresponse.childfolders_)
      {
        strings.push_back(QString::fromStdString(childfolder));

      }
      pathmodel_->setStringList(strings);
      pathcompleter_->complete();
    }));
  }
}

void ManageFileWindow::on_checkfilldisk_toggled(const bool)
{
  if (ui_.checkfilldisk->isChecked())
  {
    ui_.editsize->setEnabled(false);
    
  }
  else
  {
    ui_.editsize->setEnabled(true);

  }
}

void ManageFileWindow::on_buttonok_clicked()
{
  std::string mountpoint;
  const std::string path = ui_.editpath->text().toStdString();
  if (device_->IsLinux())
  {
    mountpoint = ui_.editmountpoint->text().toStdString();

  }

  const uint64_t size = ui_.editsize->text().toULongLong() * 1024llu * 1024llu * 1024llu;
  if (size < 1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Size must be at least 1GB"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  uint64_t chunksize = 0;
  try
  {
    std::string chunksizetext = ui_.combochunksize->currentText().toStdString();
    chunksizetext.erase(std::remove_if(chunksizetext.begin(), chunksizetext.end(), [](const char c) { return !std::isdigit(c); }), chunksizetext.end()); // Remove the "MB" and anything else that might get in the way
    chunksize = boost::lexical_cast<uint64_t>(chunksizetext) * 1024ull * 1024ull;
  }
  catch (...)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to parse number of chunks"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  SetEnabled(false);
  addfileconnection_ = device_->AddFile(mountpoint, path, ui_.checkfilldisk->isChecked(), size / chunksize, chunksize, ui_.checkautomount->isChecked(), [this](const std::chrono::steady_clock::duration latency, const monocle::client::ADDFILERESPONSE& addfilesresponse)
  {
    SetEnabled(true);
    if (addfilesresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), QString::fromStdString(addfilesresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
    accept();
  });
}

}

QT_END_NAMESPACE
