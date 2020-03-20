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

void DeviceTree::dragEnterEvent(QDragEnterEvent* event)
{
  DragEvent(event);

}

void DeviceTree::dragMoveEvent(QDragMoveEvent* event)
{
  DragEvent(event);

}

void DeviceTree::dragLeaveEvent(QDragLeaveEvent* event)
{
  event->accept();

}

void DeviceTree::dropEvent(QDropEvent* event)
{
  const QMimeData* mimedata = event->mimeData();
  if (mimedata == nullptr)
  {
    event->ignore();
    return;
  }

  const QByteArray deviceidentifierdata = mimedata->data(MIME_DEVICE_TREE_DEVICE_IDENTIFIER);
  if (deviceidentifierdata.isEmpty())
  {
    event->ignore();
    return;
  }
  if (deviceidentifierdata.size() != sizeof(uint64_t))
  {
    event->ignore();
    return;
  }
  const uint64_t deviceidentifier = *reinterpret_cast<const uint64_t*>(deviceidentifierdata.data());

  const QByteArray recordingtokendata = mimedata->data(MIME_DEVICE_TREE_RECORDING_TOKEN);
  if (recordingtokendata.isEmpty())
  {
    event->ignore();
    return;
  }
  if (recordingtokendata.size() != sizeof(uint64_t))
  {
    event->ignore();
    return;
  }
  const uint64_t recordingtoken = *reinterpret_cast<const uint64_t*>(recordingtokendata.data());

  if (!mimedata->data(MIME_DEVICE_TREE_RECORDING_TRACK_ID).isEmpty())
  {
    event->ignore();
    return;
  }

  // Retrieve the corresponding device we are hovering over is the same one, because we can't drag recordings out onto another device
  const QPoint pos = event->pos();
  QTreeWidgetItem* destitem = itemAt(pos.x(), pos.y());
  if (!destitem)
  {
    event->ignore();
    return;
  }

  QTreeWidgetItem* toplevelitem = destitem;
  while (toplevelitem->parent())
  {
    toplevelitem = toplevelitem->parent();
  }

  if (toplevelitem->type() != static_cast<int>(DEVICE_TREE_TOP_LEVEL_ITEM_TYPE::DEVICE))
  {
    event->ignore();
    return;
  }

  DeviceTreeDeviceItem* deviceitem = static_cast<DeviceTreeDeviceItem*>(toplevelitem);
  if (deviceitem->GetDevice()->GetIdentifier() != deviceidentifier)
  {
    event->ignore();
    return;
  }

  const boost::shared_ptr<Device> device = MainWindow::Instance()->GetDeviceMgr().GetDevice(deviceidentifier);
  if (device == nullptr)
  {
    event->ignore();
    return;
  }

  DeviceTreeRecordingItem* recordingitem = deviceitem->GetRecordingItem(recordingtoken);
  if (!recordingitem)
  {
    event->ignore();
    return;
  }

  const int deviceitemindex = deviceitem->indexOfChild(recordingitem);
  if (deviceitemindex == -1)
  {
    event->ignore();
    return;
  }

  recordingitem = static_cast<DeviceTreeRecordingItem*>(deviceitem->takeChild(deviceitemindex));
  if (!recordingitem) // Shouldn't happen but ok...
  {
    event->ignore();
    return;
  }

  const int destindex = deviceitem->indexOfChild(destitem);
  if (destindex == -1) // This can happen when the user drops the item on the device, so put it at the top. If it is an error for another reason, no harm in just placing it back anyway
  {
    deviceitem->insertChild(0, recordingitem);
    event->accept();
    return;
  }

  deviceitem->insertChild(destindex, recordingitem);//TODO is this satisfactory?

  //TODO in order to put it below anything else, we have to drag it below onto others, so what we need to do it deviceitem->count() index I think
    //TODO so half this shit goes away
      //TODO so that means tidying up this and the other method

  //TODO now put it in the correct location



  int i = 0;//TODO
  //TODO how do we GET the recording item?
    //TODO Do we get the recording, and then work backwards to get the recordingitem?

  //TODO take the recording item, then insert it somewhere else

  //TODO we need to then ask the device to re-order the "order" values
    //TODO we probably want this to be per user I think...
    //TODO make sure to update other clients with event that this has happenned
      //TODO I think the client just "does" it and then errors back if it failed to do so... but still maintains it locally

}

void DeviceTree::DragEvent(QDragMoveEvent* event)
{
  const QMimeData* mimedata = event->mimeData();
  if (mimedata == nullptr)
  {
    event->ignore();
    return;
  }

  const QByteArray devicetoken = mimedata->data(MIME_DEVICE_TREE_DEVICE_IDENTIFIER);
  if (devicetoken.isEmpty())
  {
    event->ignore();
    return;
  }
  if (devicetoken.size() != sizeof(uint64_t))
  {
    event->ignore();
    return;
  }
  const uint64_t deviceidentifier = *reinterpret_cast<const uint64_t*>(devicetoken.data());

  const QByteArray recordingtokendata = mimedata->data(MIME_DEVICE_TREE_RECORDING_TOKEN);
  if (recordingtokendata.isEmpty())
  {
    event->ignore();
    return;
  }
  if (recordingtokendata.size() != sizeof(uint64_t))
  {
    event->ignore();
    return;
  }

  if (!mimedata->data(MIME_DEVICE_TREE_RECORDING_TRACK_ID).isEmpty())
  {
    event->ignore();
    return;
  }

  // Retrieve the corresponding device we are hovering over is the same one, because we can't drag recordings out onto another device
  const QPoint pos = event->pos();
  QTreeWidgetItem* item = itemAt(pos.x(), pos.y());
  if (!item)
  {
    event->ignore();
    return;
  }

  while (item->parent())
  {
    item = item->parent();
  }

  if (item->type() != static_cast<int>(DEVICE_TREE_TOP_LEVEL_ITEM_TYPE::DEVICE))
  {
    event->ignore();
    return;
  }

  DeviceTreeDeviceItem* deviceitem = static_cast<DeviceTreeDeviceItem*>(item);
  if (deviceitem->GetDevice()->GetIdentifier() != deviceidentifier)
  {
    event->ignore();
    return;
  }

  const boost::shared_ptr<Device> device = MainWindow::Instance()->GetDeviceMgr().GetDevice(deviceidentifier);
  if (device == nullptr)
  {
    event->ignore();
    return;
  }

  event->accept();
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
