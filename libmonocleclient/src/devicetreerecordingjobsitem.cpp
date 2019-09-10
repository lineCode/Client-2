// devicetreerecordingjobsitem.cpp
//

///// Includes /////

#include "monocleclient/devicetreerecordingjobsitem.h"

#include <QAction>
#include <QMenu>

#include "monocleclient/devicetreerecordingjobitem.h"
#include "monocleclient/managerecordingjobswindow.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTreeRecordingJobsItem::DeviceTreeRecordingJobsItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording) :
  DeviceTreeItem(parent, "Jobs"),
  device_(device),
  recording_(recording),
  manage_(new QAction("Manage", this))
{
  connect(manage_, &QAction::triggered, this, &DeviceTreeRecordingJobsItem::Manage);
  connect(recording.get(), &Recording::JobAdded, this, &DeviceTreeRecordingJobsItem::RecordingJobAdded);
  connect(recording.get(), &Recording::JobRemoved, this, &DeviceTreeRecordingJobsItem::RecordingJobRemoved);
}

DeviceTreeRecordingJobsItem::~DeviceTreeRecordingJobsItem()
{

}

void DeviceTreeRecordingJobsItem::ContextMenuEvent(const QPoint& pos)
{
  QMenu* menu = new QMenu(treeWidget());
  menu->addAction(manage_);
  menu->exec(pos);
}

void DeviceTreeRecordingJobsItem::Expanded()
{

}

void DeviceTreeRecordingJobsItem::Collapsed()
{

}

void DeviceTreeRecordingJobsItem::DoubleClicked()
{
  DeviceTreeItem::DoubleClicked();

}

void DeviceTreeRecordingJobsItem::AddRecordingJob(const QSharedPointer<client::RecordingJob>& recordingjob)
{
  addChild(new DeviceTreeRecordingJobItem(this, device_, recording_, recordingjob));

}

void DeviceTreeRecordingJobsItem::Manage(bool)
{
  ManageRecordingJobsWindow(treeWidget(), device_, recording_).exec();

}

void DeviceTreeRecordingJobsItem::RecordingJobAdded(const QSharedPointer<client::RecordingJob>& recordingjob)
{
  addChild(new DeviceTreeRecordingJobItem(this, device_, recording_, recordingjob));

}

void DeviceTreeRecordingJobsItem::RecordingJobRemoved(const uint64_t token)
{
  for (int i = (childCount() - 1); i >= 0; --i)
  {
    if (static_cast<DeviceTreeRecordingJobItem*>(child(i))->GetRecordingJob()->GetToken() == token)
    {
      removeChild(child(i));

    }
  }
}

}
