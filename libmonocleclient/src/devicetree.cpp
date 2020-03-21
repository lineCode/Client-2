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
#include "monocleclient/devicetreemapitem.h"
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
  setguiorderconnection_.Close();

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
    if (item->type() == DEVICE_TREE_ITEM_TYPE::DEVICE_MAP)
    {
      DeviceTreeMapItem* devicetreemapitem = static_cast<DeviceTreeMapItem*>(item);
      QMimeData* mimedata = new QMimeData();
      QByteArray bytearray;

      const uint64_t deviceidentifier = devicetreemapitem->GetDevice()->GetIdentifier();
      bytearray.append(reinterpret_cast<const char*>(&deviceidentifier), sizeof(deviceidentifier));
      mimedata->setData(MIME_DEVICE_TREE_DEVICE_IDENTIFIER, bytearray);

      bytearray.clear();
      const uint64_t maptoken = devicetreemapitem->GetMap()->GetToken();
      bytearray.append(reinterpret_cast<const char*>(&maptoken), sizeof(maptoken));
      mimedata->setData(MIME_DEVICE_TREE_MAP_TOKEN, bytearray);
      return mimedata;
    }
    else if (item->type() == DEVICE_TREE_ITEM_TYPE::DEVICE_RECORDING)
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
    else if (item->type() == DEVICE_TREE_ITEM_TYPE::DEVICE_RECORDING_TRACK)
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
    if (toplevelitem->type() == static_cast<int>(DEVICE_TREE_ITEM_TYPE::DEVICE_RECORDING)) // If a user drags onto a DEVICE_RECORDING_TRACK item, we want to take the corresponding recording item instead
    {
      destitem = toplevelitem;

    }
  }

  if (toplevelitem->type() != static_cast<int>(DEVICE_TREE_ITEM_TYPE::DEVICE))
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

  const QByteArray maptokendata = mimedata->data(MIME_DEVICE_TREE_MAP_TOKEN);
  const QByteArray recordingtokendata = mimedata->data(MIME_DEVICE_TREE_RECORDING_TOKEN);
  DeviceTreeItem* item = nullptr;
  if (!maptokendata.isEmpty())
  {
    if (maptokendata.isEmpty())
    {
      event->ignore();
      return;
    }
    if (maptokendata.size() != sizeof(uint64_t))
    {
      event->ignore();
      return;
    }
    const uint64_t maptoken = *reinterpret_cast<const uint64_t*>(maptokendata.data());

    item = deviceitem->GetMapItem(maptoken);
    if (!item)
    {
      event->ignore();
      return;
    }
  }
  else if (!recordingtokendata.isEmpty())
  {
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

    item = deviceitem->GetRecordingItem(recordingtoken);
    if (!item)
    {
      event->ignore();
      return;
    }
  }
  else
  {
    event->ignore();
    return;
  }

  const int itemindex = deviceitem->indexOfChild(item);
  if (itemindex == -1)
  {
    event->ignore();
    return;
  }

  item = static_cast<DeviceTreeItem*>(deviceitem->takeChild(itemindex));
  if (!item) // Shouldn't happen but ok...
  {
    event->ignore();
    return;
  }

  int destindex = deviceitem->indexOfChild(destitem);
  if (destindex == -1) // This can happen when the user drops the item on the device, so put it at the top. If it is an error for another reason, no harm in just placing it back anyway
  {
    deviceitem->insertChild(0, item);
    event->accept();
    return;
  }

  if ((destindex + 1) != itemindex) // If we are moving one up, then this effectively "swaps" them
  {
    ++destindex;

  }

  deviceitem->insertChild(destindex, item);

  // Save the result to the server
  std::vector< std::pair<uint64_t, uint64_t> > recordings;
  std::vector< std::pair<uint64_t, uint64_t> > maps;
  for (int i = 0; i < deviceitem->childCount(); ++i)
  {
    QTreeWidgetItem* item = deviceitem->child(i);
    if (item->type() == DEVICE_TREE_ITEM_TYPE::DEVICE_RECORDING)
    {
      const DeviceTreeRecordingItem* recordingitem = static_cast<const DeviceTreeRecordingItem*>(item);
      recordings.push_back(std::make_pair(recordingitem->GetRecording()->GetToken(), i));
    }
    else if (item->type() == DEVICE_TREE_ITEM_TYPE::DEVICE_MAP)
    {
      const DeviceTreeMapItem* mapitem = static_cast<const DeviceTreeMapItem*>(item);
      maps.push_back(std::make_pair(mapitem->GetMap()->GetToken(), i));
    }
  }
  
  setguiorderconnection_ = device->SetGuiOrder(recordings, maps, [device](const std::chrono::steady_clock::duration latency, const monocle::client::SETGUIORDERRESPONSE& setguiorderresponse)
  {
    if (setguiorderresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      LOG_GUI_THREAD_WARNING_SOURCE(device, "SetGuiOrder failed");
      return;
    }
  });
}

void DeviceTree::DragEvent(QDragMoveEvent* event)
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

  const QByteArray maptokendata = mimedata->data(MIME_DEVICE_TREE_MAP_TOKEN);
  const QByteArray recordingtokendata = mimedata->data(MIME_DEVICE_TREE_RECORDING_TOKEN);
  if (!maptokendata.isEmpty())
  {
    if (maptokendata.size() != sizeof(uint64_t))
    {
      event->ignore();
      return;
    }
  }
  else if (!recordingtokendata.isEmpty())
  {
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
  }
  else
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

  if (item->type() != static_cast<int>(DEVICE_TREE_ITEM_TYPE::DEVICE))
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
