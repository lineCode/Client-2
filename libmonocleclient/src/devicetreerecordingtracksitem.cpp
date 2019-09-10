// devicetreerecordingtracksitem.cpp
//

///// Includes /////

#include "monocleclient/devicetreerecordingtracksitem.h"

#include <QAction>
#include <QMenu>

#include "monocleclient/devicetreerecordingtrackitem.h"
#include "monocleclient/managerecordingtrackswindow.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingtrack.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTreeRecordingTracksItem::DeviceTreeRecordingTracksItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording) :
  DeviceTreeItem(parent, "Tracks"),
  device_(device),
  recording_(recording),
  manage_(new QAction("Manage", this))
{
  connect(manage_, &QAction::triggered, this, &DeviceTreeRecordingTracksItem::Manage);
  connect(recording.get(), &Recording::TrackAdded, this, &DeviceTreeRecordingTracksItem::TrackAdded);
  connect(recording.get(), &Recording::TrackRemoved, this, &DeviceTreeRecordingTracksItem::TrackRemoved);
}

DeviceTreeRecordingTracksItem::~DeviceTreeRecordingTracksItem()
{

}

void DeviceTreeRecordingTracksItem::ContextMenuEvent(const QPoint& pos)
{
  QMenu* menu = new QMenu(treeWidget());
  menu->addAction(manage_);
  menu->exec(pos);
}

void DeviceTreeRecordingTracksItem::Expanded()
{

}

void DeviceTreeRecordingTracksItem::Collapsed()
{

}

void DeviceTreeRecordingTracksItem::DoubleClicked()
{
  DeviceTreeItem::DoubleClicked();

}

void DeviceTreeRecordingTracksItem::AddTrack(const QSharedPointer<client::RecordingTrack>& track)
{
  addChild(new DeviceTreeRecordingTrackItem(this, device_, recording_, track));

}

void DeviceTreeRecordingTracksItem::Manage(bool)
{
  ManageRecordingTracksWindow(treeWidget(), device_, recording_).exec();

}

void DeviceTreeRecordingTracksItem::TrackAdded(const QSharedPointer<client::RecordingTrack>& track)
{
  AddTrack(track);

}

void DeviceTreeRecordingTracksItem::TrackRemoved(const uint64_t id)
{
  for (int i = (childCount() - 1); i >= 0; --i)
  {
    if (static_cast<DeviceTreeRecordingTrackItem*>(child(i))->GetTrack()->GetId() == id)
    {
      removeChild(child(i));

    }
  }
}

}
