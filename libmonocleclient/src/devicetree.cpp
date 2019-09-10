// devicetree.cpp
//

///// Includes /////

#include "monocleclient/devicetree.h"

#include <QAction>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>

#include "monocleclient/devicemgr.h"
#include "monocleclient/devicetreedeviceitem.h"
#include "monocleclient/devicetreeitem.h"
#include "monocleclient/devicetreemediaitem.h"
#include "monocleclient/devicetreerecordingitem.h"
#include "monocleclient/devicetreerecordingtrackitem.h"
#include "monocleclient/editdevicewindow.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/recording.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTree::DeviceTree(QWidget* parent) :
  QTreeWidget(parent),
  latencygreen_(":/latencygreen.png"),
  latencyyellow_(":/latencyyellow.png"),
  latencyred_(":/latencyred.png"),
  latencynone_(":/latencynone.png"),
  recordingicon_(":/recording.png"),
  mapicon_(":/map.png"),
  showfilter_(new QAction("Show Filter", this)),
  adddevice_(new QAction("Add Device", this)),
  importfile_(new QAction("Import File", this))
{
  showfilter_->setCheckable(true);
  showfilter_->setChecked(true);

  connect(this, &DeviceTree::itemExpanded, this, &DeviceTree::ItemExpanded);
  connect(this, &DeviceTree::itemCollapsed, this, &DeviceTree::ItemCollapsed);
  connect(this, &DeviceTree::itemDoubleClicked, this, &DeviceTree::DoubleClicked);
  connect(showfilter_, &QAction::triggered, this, &DeviceTree::ShowFilter);
  connect(adddevice_, &QAction::triggered, this, &DeviceTree::AddDevice);
  connect(importfile_, &QAction::triggered, this, &DeviceTree::ImportFile);
  connect(&MainWindow::Instance()->GetDeviceMgr(), &DeviceMgr::DeviceAdded, this, &DeviceTree::DeviceAdded);
  connect(&MainWindow::Instance()->GetDeviceMgr(), &DeviceMgr::DeviceRemoved, this, &DeviceTree::DeviceRemoved);
  connect(&MainWindow::Instance()->GetMediaMgr(), &MediaMgr::MediaAdded, this, &DeviceTree::MediaAdded);
  connect(&MainWindow::Instance()->GetMediaMgr(), &MediaMgr::MediaRemoved, this, &DeviceTree::MediaRemoved);

  setExpandsOnDoubleClick(false);

  for (auto& device : MainWindow::Instance()->GetDeviceMgr().GetDevices())
  {
    DeviceAdded(device);

  }
}

DeviceTree::~DeviceTree()
{

}

void DeviceTree::SetFilter(const QString& filter)
{
  for (int i = 0; i < topLevelItemCount(); ++i)
  {
    QTreeWidgetItem* item = topLevelItem(i);
    if (typeid(*item) == typeid(DeviceTreeDeviceItem))
    {
      static_cast<DeviceTreeDeviceItem*>(topLevelItem(i))->SetFilter(filter);

    }
    else if (typeid(*item) == typeid(DeviceTreeMediaItem))
    {
      static_cast<DeviceTreeMediaItem*>(topLevelItem(i))->SetFilter(filter);

    }
  }
}

QMimeData* DeviceTree::mimeData(const QList<QTreeWidgetItem*> items) const
{
  if (items.isEmpty())
  {

    return nullptr;
  }
  
  for (QTreeWidgetItem* item : items)
  {
    if (typeid(*item) == typeid(DeviceTreeRecordingItem))
    {
      DeviceTreeRecordingItem* devicetreerecordingitem = static_cast<DeviceTreeRecordingItem*>(item);
      QMimeData* mimedata = new QMimeData();

      QByteArray bytearray;
      const uint64_t deviceidentifier = devicetreerecordingitem->GetDevice()->GetIdentifier();
      bytearray.append(reinterpret_cast<const char*>(&deviceidentifier), sizeof(deviceidentifier));
      mimedata->setData(MIME_DEVICE_TREE_DEVICE_IDENTIFIER, bytearray);

      bytearray.clear();
      const uint64_t recordingtoken = devicetreerecordingitem->GetRecording()->GetToken();
      bytearray.append(reinterpret_cast<const char*>(&recordingtoken), sizeof(recordingtoken));
      mimedata->setData(MIME_DEVICE_TREE_RECORDING_TOKEN, bytearray);
      return mimedata;
    }
    else if (typeid(*item) == typeid(DeviceTreeRecordingTrackItem))
    {
      DeviceTreeRecordingTrackItem* devicetreerecordingtrackitem = static_cast<DeviceTreeRecordingTrackItem*>(item);
      if (devicetreerecordingtrackitem->GetTrack()->GetTrackType() != monocle::TrackType::Video)
      {

        return nullptr; // Can only drag and drop video tracks
      }

      QMimeData* mimedata = new QMimeData();
      QByteArray bytearray;
      const uint64_t deviceidentifier = devicetreerecordingtrackitem->GetDevice()->GetIdentifier();
      bytearray.append(reinterpret_cast<const char*>(&deviceidentifier), sizeof(deviceidentifier));
      mimedata->setData(MIME_DEVICE_TREE_DEVICE_IDENTIFIER, bytearray);

      bytearray.clear();
      const uint64_t recordingtoken = devicetreerecordingtrackitem->GetRecording()->GetToken();
      bytearray.append(reinterpret_cast<const char*>(&recordingtoken), sizeof(recordingtoken));
      mimedata->setData(MIME_DEVICE_TREE_RECORDING_TOKEN, bytearray);

      bytearray.clear();
      const uint32_t recordingtrackid = devicetreerecordingtrackitem->GetTrack()->GetId();
      bytearray.append(reinterpret_cast<const char*>(&recordingtrackid), sizeof(recordingtrackid));
      mimedata->setData(MIME_DEVICE_TREE_RECORDING_TRACK_ID, bytearray);
      return mimedata;
    }
  }

  return nullptr;
}

void DeviceTree::contextMenuEvent(QContextMenuEvent* event)
{
  QTreeWidgetItem* item = itemAt(event->pos());
  if (item)
  {
    static_cast<DeviceTreeItem*>(item)->ContextMenuEvent(event->globalPos());

  }
  else
  {
    QMenu* menu = new QMenu(this);
    showfilter_->setChecked(!MainWindow::Instance()->IsDeviceTreeFilterHidden());
    menu->addAction(showfilter_);
    menu->addAction(adddevice_);
    menu->addAction(importfile_);
    menu->exec(event->globalPos());
  }
}

void DeviceTree::ItemExpanded(QTreeWidgetItem* item)
{
  static_cast<DeviceTreeItem*>(item)->Expanded();

}

void DeviceTree::ItemCollapsed(QTreeWidgetItem* item)
{
  static_cast<DeviceTreeItem*>(item)->Collapsed();

}

void DeviceTree::DoubleClicked(QTreeWidgetItem* item, int)
{
  static_cast<DeviceTreeItem*>(item)->DoubleClicked();

}

void DeviceTree::ShowFilter(bool checked)
{
  MainWindow::Instance()->ShowDeviceTreeFilter(checked);

}

void DeviceTree::AddDevice(bool)
{
  EditDeviceWindow(static_cast<QWidget*>(parent()), nullptr).exec();

}

void DeviceTree::ImportFile(bool)
{
  const QString filename = QFileDialog::getOpenFileName(this, tr("Import File"), QString(), tr("Monocle File (*.mon);;"));
  if (filename.isEmpty())
  {

    return;
  }
  MainWindow::Instance()->GetMediaMgr().AddMedia(filename);
}

void DeviceTree::DeviceAdded(const boost::shared_ptr<Device>& device)
{
  if (!device)
  {

    return;
  }
  addTopLevelItem(new DeviceTreeDeviceItem(this, device, latencygreen_, latencyyellow_, latencyred_, latencynone_, recordingicon_, mapicon_));
}

void DeviceTree::DeviceRemoved(const boost::shared_ptr<Device>& device)
{
  if (!device)
  {

    return;
  }

  for (int i = (topLevelItemCount() - 1); i >= 0; --i)
  {
    if (std::string(DeviceTreeDeviceItem::staticMetaObject.className()) != static_cast<DeviceTreeItem*>(topLevelItem(i))->metaObject()->className())
    {

      continue;
    }

    if ((static_cast<DeviceTreeDeviceItem*>(topLevelItem(i)))->GetDevice() == device)
    {
      delete takeTopLevelItem(i);

    }
  }
}

void DeviceTree::MediaAdded(QSharedPointer<Media>& media)
{
  if (media.isNull())
  {

    return;
  }
  addTopLevelItem(new DeviceTreeMediaItem(this, media));
}

void DeviceTree::MediaRemoved(QSharedPointer<Media>& media)
{
  if (media.isNull())
  {

    return;
  }

  for (int i = (topLevelItemCount() - 1); i >= 0; --i)
  {
    QTreeWidgetItem* item = topLevelItem(i);
    if (typeid(*item) != typeid(DeviceTreeMediaItem))
    {

      continue;
    }

    if ((static_cast<DeviceTreeMediaItem*>(topLevelItem(i)))->GetMedia() == media)
    {
      delete takeTopLevelItem(i);

    }
  }
}

}
