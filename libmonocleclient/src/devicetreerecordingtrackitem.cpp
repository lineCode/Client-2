// devicetreerecordingtrackitem.cpp
//

///// Includes /////

#include "monocleclient/devicetreerecordingtrackitem.h"

#include <QMenu>
#include <QMessageBox>

#include "monocleclient/mainwindow.h"
#include "monocleclient/managetrackwindow.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordinglogwindow.h"
#include "monocleclient/recordingtrack.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTreeRecordingTrackItem::DeviceTreeRecordingTrackItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack, const QSharedPointer<client::RecordingTrack>& track) :
  DeviceTreeItem(parent, GetName(track)),
  device_(device),
  recording_(recording),
  recordingjobsourcetrack_(recordingjobsourcetrack),
  track_(track),
  edit_(new QAction("Edit", this)),
  remove_(new QAction("Remove", this)),
  viewlog_(new QAction("View Log", this))
{
  connect(edit_, &QAction::triggered, this, &DeviceTreeRecordingTrackItem::Edit);
  connect(remove_, &QAction::triggered, this, &DeviceTreeRecordingTrackItem::Remove);
  connect(viewlog_, &QAction::triggered, this, &DeviceTreeRecordingTrackItem::ViewLog);
  connect(recording.get(), &Recording::TrackChanged, this, &DeviceTreeRecordingTrackItem::TrackChanged);
}

DeviceTreeRecordingTrackItem::~DeviceTreeRecordingTrackItem()
{
  removetrackconnection_.Close();

}

void DeviceTreeRecordingTrackItem::ContextMenuEvent(const QPoint& pos)
{
  QMenu* menu = new QMenu(treeWidget());
  menu->addAction(edit_);
  menu->addAction(remove_);
  menu->addAction(viewlog_);
  menu->exec(pos);
}

void DeviceTreeRecordingTrackItem::Expanded()
{

}

void DeviceTreeRecordingTrackItem::Collapsed()
{

}

void DeviceTreeRecordingTrackItem::DoubleClicked()
{
  if (track_->GetTrackType() != monocle::TrackType::Video)
  {

    return;
  }
  MainWindow::Instance()->GetVideoWidgetsMgr().CreateVideoView(device_, recording_, track_);
}

QString DeviceTreeRecordingTrackItem::GetName(const QSharedPointer<client::RecordingTrack>& track) const
{
  return (track->GetDescription() + "(" + monocle::EnumNameTrackType(track->GetTrackType()) + ")");//TODO no longer need this(unless it is empty... then we can just put the track type?)... or "empty"?
}

void DeviceTreeRecordingTrackItem::Edit(bool)
{
  ManageTrackWindow(treeWidget(), device_, recording_, recordingjobsourcetrack_, track_).exec();

}

void DeviceTreeRecordingTrackItem::Remove(bool)
{
  //TODO The tracks get removed... but the RecordingJobSource does NOT get removed
  if (QMessageBox::question(treeWidget(), tr("Remove"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    removetrackconnection_ = device_->RemoveTrack(recording_->GetToken(), track_->GetId(), [](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVETRACKRESPONSE& removetrackresponse)
    {
      if (removetrackresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removetrackresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
    });
  }
}

void DeviceTreeRecordingTrackItem::ViewLog(bool)
{
  RecordingLogWindow(treeWidget(), device_, { recording_ }, { track_ }, std::vector< QSharedPointer<client::RecordingJob> >(), std::vector< QSharedPointer<client::RecordingJobSourceTrack> >()).exec();

}

void DeviceTreeRecordingTrackItem::TrackChanged(const QSharedPointer<client::RecordingTrack>& track)
{
  if (track_ != track)
  {

    return;
  }
  setText(0, GetName(track));
}

}
