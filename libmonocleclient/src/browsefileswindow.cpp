// browsefileswindow.cpp
//

///// Includes /////

#include "monocleclient/browsefileswindow.h"

#include <boost/filesystem.hpp>
#include <QDir>

#include "monocleclient/browsefilestreeitem.h"
#include "monocleclient/device.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

BrowseFilesWindow::BrowseFilesWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QString& location) :
  QDialog(parent),
  device_(device),
  location_(location)
{
  ui_.setupUi(this);

  connect(ui_.treebrowsefiles, &BrowseFilesTree::itemClicked, this, &BrowseFilesWindow::ItemClicked);
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &BrowseFilesWindow::reject);

  ui_.editlocation->setText(location);

  getchildfoldersconnection_ = device_->GetChildFolders(std::string("/"), false, [this](const std::chrono::steady_clock::duration latency, const monocle::client::GETCHILDFOLDERSRESPONSE& getchildfoldersresponse)
  {
    if (getchildfoldersresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {

      return;
    }

    for (const std::string& childfolder : getchildfoldersresponse.childfolders_)
    {
      BrowseFilesTreeItem* item = new BrowseFilesTreeItem(childfolder, device_);
      item->setText(0, childfolder.c_str());
      item->setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy::ShowIndicator);
      ui_.treebrowsefiles->addTopLevelItem(item);
    }
  });
}

BrowseFilesWindow::~BrowseFilesWindow()
{
  getchildfoldersconnection_.Close();

}

void BrowseFilesWindow::ItemClicked(QTreeWidgetItem* item, int column)
{
  BrowseFilesTreeItem* browsefilestreeitem = static_cast<BrowseFilesTreeItem*>(item);
  std::string folder = browsefilestreeitem->GetFolder();
  if (folder.size() && ((folder.back() != '/') && (folder.back() != '\\')))
  {
    folder += QDir::separator().toLatin1();

  }

  std::string currentfilename = boost::filesystem::path(ui_.editlocation->text().toStdString()).filename().string();
  if (currentfilename.empty())
  {
    currentfilename = "file.dat";

  }
  ui_.editlocation->setText(QString::fromStdString(folder) + QString::fromStdString(currentfilename));
}

void BrowseFilesWindow::on_buttonok_clicked()
{
  location_ = ui_.editlocation->text();
  accept();
}

}

QT_END_NAMESPACE
