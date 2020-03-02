// devicetreerecordingitem.cpp
//

///// Includes /////

#include "monocleclient/devicetreerecordingitem.h"

#include <QMenu>
#include <QMessageBox>
#include <QStringList>

#include "monocleclient/devicetreerecordingtrackitem.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/managerecordingjobswindow.h"
#include "monocleclient/managerecordingwindow.h"
#include "monocleclient/managerecordingtrackswindow.h"
#include "monocleclient/managetrackwindow.h"
#include "monocleclient/devicetreerecordingjobsitem.h"
#include "monocleclient/receiver.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"
#include "monocleclient/recordinglogwindow.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTreeRecordingItem::DeviceTreeRecordingItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QIcon& recordingicon, const QIcon& cameraicon) :
  DeviceTreeItem(parent, recording->GetName()),
  device_(device),
  recording_(recording),
  cameraicon_(cameraicon),
  edit_(new QAction("Edit", this)),
  addvideotrack_(new QAction("Add Video Track", this)),
  remove_(new QAction("Remove", this)),
  viewlog_(new QAction("View Log", this))
{
  connect(recording_.data(), &Recording::TrackAdded, this, &DeviceTreeRecordingItem::TrackAdded);
  connect(recording_.data(), &Recording::TrackRemoved, this, &DeviceTreeRecordingItem::TrackRemoved);
  connect(recording_.data(), &Recording::JobSourceTrackAdded, this, &DeviceTreeRecordingItem::JobSourceTrackAdded);
  connect(recording_.data(), &Recording::JobSourceTrackRemoved, this, &DeviceTreeRecordingItem::JobSourceTrackRemoved);
  connect(recording_.data(), &Recording::ActiveJobChanged, this, &DeviceTreeRecordingItem::ActiveJobChanged);
  connect(recording_.data(), &Recording::JobSourceAdded, this, &DeviceTreeRecordingItem::RecordingJobSourceAdded);
  connect(recording_.data(), &Recording::JobSourceRemoved, this, &DeviceTreeRecordingItem::RecordingJobSourceRemoved);
  connect(recording_.data(), &Recording::JobSourceTrackStateChanged, this, &DeviceTreeRecordingItem::RecordingJobSourceTrackStateChanged);

  connect(edit_, &QAction::triggered, this, &DeviceTreeRecordingItem::Edit);
  connect(addvideotrack_, &QAction::triggered, this, &DeviceTreeRecordingItem::AddVideoTrack);
  connect(remove_, &QAction::triggered, this, &DeviceTreeRecordingItem::Remove);
  connect(viewlog_, &QAction::triggered, this, &DeviceTreeRecordingItem::ViewLog);

  UpdateChildren();
  UpdateToolTip();

  setIcon(0, recordingicon);
}

DeviceTreeRecordingItem::~DeviceTreeRecordingItem()
{
  removerecordingconnection_.Close();

}

void DeviceTreeRecordingItem::ContextMenuEvent(const QPoint& pos)
{
  QMenu* menu = new QMenu(treeWidget());
  menu->addAction(edit_);
  if (recording_->GetNumVideoTracks() < 5)
  {
    menu->addAction(addvideotrack_);

  }
  menu->addAction(remove_);
  menu->addAction(viewlog_);
  menu->exec(pos);
}

void DeviceTreeRecordingItem::Expanded()
{

}

void DeviceTreeRecordingItem::Collapsed()
{

}

void DeviceTreeRecordingItem::DoubleClicked()
{
  std::vector< QSharedPointer<client::RecordingTrack> > tracks = recording_->GetVideoTracks();
  if (tracks.empty())
  {
    Expanded();
    return;
  }

  MainWindow::Instance()->GetVideoWidgetsMgr().CreateVideoView(device_, recording_, tracks.front());
}

void DeviceTreeRecordingItem::SetFilter(const QString& filter)
{
  if (filter.isEmpty())
  {
    setHidden(false);

  }
  else
  {
    if ((recording_->GetName().indexOf(filter, 0, Qt::CaseInsensitive) == -1) && (recording_->GetLocation().indexOf(filter, 0, Qt::CaseInsensitive) == -1))
    {
      setHidden(true);

    }
    else
    {
      setHidden(false);

    }
  }
}

void DeviceTreeRecordingItem::UpdateToolTip()
{
  if (recording_->GetActiveJob())
  {
    QStringList tooltips;
    bool error = false;
    for (const QSharedPointer<client::RecordingJobSource>& source : recording_->GetActiveJob()->GetSources())
    {
      const QSharedPointer<client::Receiver> receiver = recording_->GetDevice()->GetReceiver(source->GetReceiverToken());
      if (!receiver)
      {
        LOG_GUI_WARNING_SOURCE(device_, QString("Unable to find receiver: ") + QString::number(source->GetReceiverToken()));
        continue;
      }

      for (const QSharedPointer<client::RecordingJobSourceTrack>& track : source->GetTracks())
      {
        if (!track->GetTrack())
        {

          continue;
        }

        if (track->GetTrack()->GetTrackType() == monocle::TrackType::ObjectDetector)
        {

          continue;
        }

        monocle::TrackType tracktype = monocle::TrackType::Video;
        if (track->GetTrack() && (track->GetTrack()->GetTrackType() == monocle::TrackType::Metadata))
        {
          tracktype = monocle::TrackType::Metadata;

        }

        if (track->GetState() == monocle::RecordingJobState::Idle)
        {
          if ((tracktype != monocle::TrackType::Metadata) && !receiver->GetMediaUri().isEmpty()) // Metadata might not send frames for long durations and may become idle. Empty URI is ok to be idle as well
          {
            error = true;

          }
          tooltips.append(Tooltip(receiver->GetMediaUri(), "Idle"));
        }
        else if (track->GetState() == monocle::RecordingJobState::Error)
        {
          error = true;
          tooltips.append(Tooltip(receiver->GetMediaUri(), "Error " + track->GetError()));
        }
        else // if (track->GetState() == monocle::RecordingJobState::Active)
        {
          tooltips.append(Tooltip(receiver->GetMediaUri(), "Active"));

        }
      }
    }
    if (error)
    {
      setBackground(0, Qt::red);

    }
    else
    {
      setData(0, Qt::BackgroundRole, QVariant());

    }
    setToolTip(0, tooltips.join("\r\n"));
  }
  else
  {
    setData(0, Qt::BackgroundRole, QVariant());
    setData(0, Qt::ToolTipRole, QVariant());
  }
}

void DeviceTreeRecordingItem::UpdateChildren()
{
  // Add any children
  for (const QSharedPointer<RecordingJob>& job : recording_->GetJobs())
  {
    for (const QSharedPointer<RecordingJobSource>& source : job->GetSources())
    {
      for (const QSharedPointer<RecordingJobSourceTrack>& sourcetrack : source->GetTracks())
      {
        const QSharedPointer<RecordingTrack>& track = sourcetrack->GetTrack();
        if (track->GetTrackType() != monocle::TrackType::Video)
        {

          continue;
        }

        if (GetChild(sourcetrack, track)) // If we already have this child, ignore this
        {

          continue;
        }

        addChild(new DeviceTreeRecordingTrackItem(this, device_, recording_, job, source, sourcetrack, track, cameraicon_));
      }
    }
  }

  // Clear up any that may have disappeared
  for (int i = (childCount() - 1); i >= 0; --i)
  {
    DeviceTreeRecordingTrackItem* item = static_cast<DeviceTreeRecordingTrackItem*>(child(i));
    if (!Exists(item->GetRecordingJobSourceTrack(), item->GetTrack()))
    {
      removeChild(item);

    }
  }
}

DeviceTreeRecordingTrackItem* DeviceTreeRecordingItem::GetChild(const QSharedPointer<RecordingJobSourceTrack>& recordingjobsourcetrack, const QSharedPointer<RecordingTrack>& track) const
{
  for (int i = 0; i < childCount(); ++i)
  {
    DeviceTreeRecordingTrackItem* item = static_cast<DeviceTreeRecordingTrackItem*>(child(i));
    if ((item->GetRecordingJobSourceTrack() == recordingjobsourcetrack) && (item->GetTrack() == track))
    {

      return item;
    }
  }
  return nullptr;
}

bool DeviceTreeRecordingItem::Exists(const QSharedPointer<RecordingJobSourceTrack>& recordingjobsourcetrack, const QSharedPointer<RecordingTrack>& track) const
{
  for (const QSharedPointer<RecordingJob>& job : recording_->GetJobs())
  {
    for (const QSharedPointer<RecordingJobSource>& source : job->GetSources())
    {
      for (const QSharedPointer<RecordingJobSourceTrack>& sourcetrack : source->GetTracks())
      {
        if ((recordingjobsourcetrack == sourcetrack) && (sourcetrack->GetTrack() == track))
        {

          return true;
        }
      }
    }
  }
  return false;
}

QString DeviceTreeRecordingItem::Tooltip(const QString& mediauri, const QString& status) const
{
  if (mediauri.isEmpty())
  {

    return status;
  }
  else
  {

    return (mediauri + ": " + status);
  }
}

void DeviceTreeRecordingItem::TrackAdded(const QSharedPointer<client::RecordingTrack>& track)
{
  UpdateChildren();
  UpdateToolTip();
}

void DeviceTreeRecordingItem::TrackRemoved(const uint32_t id)
{
  UpdateChildren();
  UpdateToolTip();
}

void DeviceTreeRecordingItem::JobSourceTrackAdded(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack)
{
  UpdateChildren();
  UpdateToolTip();
}

void DeviceTreeRecordingItem::JobSourceTrackRemoved(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const uint64_t token)
{
  UpdateChildren();
  UpdateToolTip();
}

void DeviceTreeRecordingItem::ActiveJobChanged(const QSharedPointer<client::RecordingJob>& activejob)
{
  UpdateToolTip();

}

void DeviceTreeRecordingItem::RecordingJobSourceAdded(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource)
{
  UpdateChildren();
  UpdateToolTip();
}

void DeviceTreeRecordingItem::RecordingJobSourceRemoved(const QSharedPointer<client::RecordingJob>& recordingjob, const uint64_t token)
{
  UpdateChildren();
  UpdateToolTip();
}

void DeviceTreeRecordingItem::RecordingJobSourceTrackStateChanged(const QSharedPointer<client::RecordingJob>& job, const QSharedPointer<client::RecordingJobSource>& source, const QSharedPointer<client::RecordingJobSourceTrack>& track, const uint64_t time, const monocle::RecordingJobState state, const QString& error, const monocle::RecordingJobState prevstate)
{
  UpdateChildren();
  UpdateToolTip();
}

void DeviceTreeRecordingItem::Edit(bool)
{
  ManageRecordingWindow(treeWidget(), device_, recording_->GetToken()).exec();

}

void DeviceTreeRecordingItem::AddVideoTrack(bool)
{
  ManageTrackWindow(treeWidget(), device_, recording_, nullptr, nullptr, nullptr, nullptr).exec();

}

void DeviceTreeRecordingItem::ManageTracks(bool)
{
  ManageRecordingTracksWindow(treeWidget(), device_, recording_).exec();

}

void DeviceTreeRecordingItem::ManageJobs(bool)
{
  ManageRecordingJobsWindow(treeWidget(), device_, recording_).exec();

}

void DeviceTreeRecordingItem::Remove(bool)
{
  if (QMessageBox::question(treeWidget(), tr("Remove"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    removerecordingconnection_ = device_->RemoveRecording(recording_->GetToken(), [this](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVERECORDINGRESPONSE& removerecordingresponse)
    {
      if (removerecordingresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removerecordingresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
    });
  }
}

void DeviceTreeRecordingItem::ViewLog(bool)
{
  RecordingLogWindow(treeWidget(), device_, { recording_ }, std::vector< QSharedPointer<client::RecordingTrack> >(), std::vector< QSharedPointer<client::RecordingJob> >(), std::vector< QSharedPointer<client::RecordingJobSourceTrack> >()).exec();

}

}
