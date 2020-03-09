// managetrackfileswindow.cpp
//

///// Includes /////

#include "monocleclient/managetrackfileswindow.h"

#include "monocleclient/device.h"
#include "monocleclient/file.h"
#include "monocleclient/mainwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageTrackFilesWindow::ManageTrackFilesWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const std::vector< QSharedPointer<File> >& files) :
  QDialog(parent),
  device_(device)
{
  ui_.setupUi(this);
  
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);
  connect(device_.get(), &Connection::SignalFileAdded, this, &ManageTrackFilesWindow::FileAdded, Qt::QueuedConnection);
  connect(device_.get(), &Connection::SignalFileRemoved, this, &ManageTrackFilesWindow::FileRemoved, Qt::QueuedConnection);

  for (const QSharedPointer<File>& file : device->GetFiles())
  {
    QListWidgetItem* item = new QListWidgetItem(file->GetPath());
    item->setData(Qt::UserRole, file->GetToken());
    if (std::find(files.cbegin(), files.cend(), file) == files.cend())
    {
      item->setCheckState(Qt::CheckState::Unchecked);

    }
    else
    {
      item->setCheckState(Qt::CheckState::Checked);

    }
    ui_.listfiles->addItem({ item });
  }
}

ManageTrackFilesWindow::~ManageTrackFilesWindow()
{

}

void ManageTrackFilesWindow::FileAdded(const uint64_t filetoken, const std::string& path, const std::string& mountpoint, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate)
{
  QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(path));
  item->setData(Qt::UserRole, filetoken);
  item->setCheckState(Qt::CheckState::Unchecked);
  ui_.listfiles->addItem({ item });
}

void ManageTrackFilesWindow::FileRemoved(const uint64_t token)
{
  for (int i = 0; i < ui_.listfiles->count(); ++i)
  {
    QListWidgetItem* item = ui_.listfiles->item(i);
    if (item->data(Qt::UserRole).toULongLong() == token)
    {
      ui_.listfiles->removeItemWidget(item);
      return;
    }
  }
}

void ManageTrackFilesWindow::on_buttonok_clicked()
{
  for (int i = 0; i < ui_.listfiles->count(); ++i)
  {
    QListWidgetItem* item = ui_.listfiles->item(i);
    if (item->checkState() == Qt::CheckState::Checked)
    {
      const uint64_t token = item->data(Qt::UserRole).toULongLong();
      QSharedPointer<File> file = device_->GetFile(token);
      if (!file)
      {
        LOG_GUI_WARNING_SOURCE(device_, "Unable to find file: " + QString::number(token));
        continue;
      }
      files_.push_back(file);
    }
  }

  accept();
}

}

QT_END_NAMESPACE
