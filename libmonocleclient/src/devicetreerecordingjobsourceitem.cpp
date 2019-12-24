// devicetreerecordingjobsourceitem.cpp
//

///// Includes /////

#include "monocleclient/devicetreerecordingjobsourceitem.h"

#include <QMenu>
#include <QMessageBox>

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/manageonvifwindow.h"
#include "monocleclient/managerecordingjobwindow.h"
#include "monocleclient/receiver.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"
#include "monocleclient/recordinglogwindow.h"
#include "monocleclient/recordingtrack.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTreeRecordingJobSourceItem::DeviceTreeRecordingJobSourceItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack) :
  DeviceTreeItem(parent, GetName(device, recordingjobsource, recordingjobsourcetrack)),
  device_(device),
  recording_(recording),
  recordingjob_(recordingjob),
  recordingjobsource_(recordingjobsource),
  recordingjobsourcetrack_(recordingjobsourcetrack),
  manage_(new QAction("Manage", this)),
  edit_(new QAction("Edit", this)),
  remove_(new QAction("Remove", this)),
  viewlog_(new QAction("View Log", this))
{
  connect(manage_, &QAction::triggered, this, &DeviceTreeRecordingJobSourceItem::Manage);
  connect(edit_, &QAction::triggered, this, &DeviceTreeRecordingJobSourceItem::Edit);
  connect(remove_, &QAction::triggered, this, &DeviceTreeRecordingJobSourceItem::Remove);
  connect(viewlog_, &QAction::triggered, this, &DeviceTreeRecordingJobSourceItem::ViewLog);
  connect(recording_.data(), &Recording::ActiveJobChanged, this, &DeviceTreeRecordingJobSourceItem::ActiveJobChanged);
  connect(recordingjobsourcetrack_.data(), &RecordingJobSourceTrack::StateChanged, this, &DeviceTreeRecordingJobSourceItem::SourceTrackStateChanged);
  
  UpdateToolTip();
}

DeviceTreeRecordingJobSourceItem::~DeviceTreeRecordingJobSourceItem()
{
  removerecordingjobsourceconnection_.Close();

}

void DeviceTreeRecordingJobSourceItem::ContextMenuEvent(const QPoint& pos)
{
  QMenu* menu = new QMenu(treeWidget());

  const QSharedPointer<Receiver> receiver = device_->GetReceiver(recordingjobsource_->GetReceiverToken());
  if (receiver && receiver->IsONVIF())
  {
    menu->addAction(manage_);

  }
  menu->addAction(edit_);
  menu->addAction(remove_);
  menu->addAction(viewlog_);
  menu->exec(pos);
}

void DeviceTreeRecordingJobSourceItem::Expanded()
{

}

void DeviceTreeRecordingJobSourceItem::Collapsed()
{

}

void DeviceTreeRecordingJobSourceItem::Manage(bool)
{
  const QSharedPointer<Receiver> receiver = device_->GetReceiver(recordingjobsource_->GetReceiverToken());
  if (!receiver)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find receiver: ") + QString::number(recordingjobsource_->GetReceiverToken()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  ManageONVIFWindow(treeWidget(), sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), receiver->GetMediaUri(), receiver->GetUsername(), receiver->GetPassword()).exec();
}

void DeviceTreeRecordingJobSourceItem::Edit(bool)
{
  // This can be improved to open the RTSP or ONVIF window directly and then set it here, however that is a lot of work!
  ManageRecordingJobWindow(treeWidget(), device_, recording_, recordingjob_, recordingjobsourcetrack_).exec();
}

void DeviceTreeRecordingJobSourceItem::ViewLog(bool)
{
  RecordingLogWindow(treeWidget(), device_, { recording_ }, std::vector< QSharedPointer<client::RecordingTrack> >(), { }, { recordingjobsourcetrack_ }).exec();

}

void DeviceTreeRecordingJobSourceItem::Remove(bool)
{
  if (QMessageBox::question(treeWidget(), tr("Remove"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    removerecordingjobsourceconnection_ = device_->RemoveRecordingJobSource(recording_->GetToken(), recordingjob_->GetToken(), recordingjobsource_->GetToken(), [](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVERECORDINGJOBSOURCERESPONSE& removerecordingjobsourceresponse)
    {
      if (removerecordingjobsourceresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removerecordingjobsourceresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
    });
  }
}

void DeviceTreeRecordingJobSourceItem::DoubleClicked()
{
  DeviceTreeItem::DoubleClicked();

}

QString DeviceTreeRecordingJobSourceItem::GetName(const boost::shared_ptr<Device>& device, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack) const
{
  QString name;
  const QSharedPointer<client::Receiver> receiver = device->GetReceiver(recordingjobsource->GetReceiverToken());
  if (!receiver)
  {
    name = "<None>";

  }
  else
  {
    name = receiver->GetMediaUri();

  }

  const QSharedPointer<client::RecordingTrack> track = recordingjobsourcetrack->GetTrack();
  if (track)
  {
    name += " (" + QString(monocle::EnumNameTrackType(track->GetTrackType())) + ")";

  }

  return name;
}

void DeviceTreeRecordingJobSourceItem::UpdateToolTip()
{
  if (recording_->GetActiveJob() == recordingjob_)
  {
    QString tooltip;
    const QSharedPointer<client::Receiver> receiver = device_->GetReceiver(recordingjobsource_->GetReceiverToken());
    if (!receiver)
    {
      LOG_GUI_WARNING_SOURCE(device_, QString("Unable to find receiver: ") + QString::number(recordingjobsource_->GetReceiverToken()));
      return;
    }

    monocle::TrackType tracktype = monocle::TrackType::Video;
    if (recordingjobsourcetrack_->GetTrack() && ((recordingjobsourcetrack_->GetTrack()->GetTrackType() == monocle::TrackType::Metadata) || (recordingjobsourcetrack_->GetTrack()->GetTrackType() == monocle::TrackType::ObjectDetector)))
    {
      tracktype = monocle::TrackType::Metadata;

    }

    bool error = false;
    if (recordingjobsourcetrack_->GetState() == monocle::RecordingJobState::Idle)
    {
      if (tracktype != monocle::TrackType::Metadata) // Metadata might not send frames for long durations and may become idle
      {
        error = true;

      }
      tooltip = receiver->GetMediaUri() + ": Idle";
    }
    else if (recordingjobsourcetrack_->GetState() == monocle::RecordingJobState::Error)
    {
      error = true;
      tooltip = receiver->GetMediaUri() + ": Error " + recordingjobsourcetrack_->GetError();
    }
    else // if (recordingjobsourcetrack_->GetState() == monocle::RecordingJobState::Active)
    {
      tooltip = receiver->GetMediaUri() + ": Active";

    }

    if (error)
    {
      setBackground(0, Qt::red);

    }
    else
    {
      setData(0, Qt::BackgroundRole, QVariant());

    }
    setToolTip(0, tooltip);
  }
  else
  {
    setData(0, Qt::BackgroundRole, QVariant());
    setData(0, Qt::ToolTipRole, QVariant());
  }
}

void DeviceTreeRecordingJobSourceItem::ActiveJobChanged(const QSharedPointer<client::RecordingJob>& activejob)
{
  UpdateToolTip();

}

void DeviceTreeRecordingJobSourceItem::SourceTrackStateChanged(const monocle::RecordingJobState state, const QString& error, const monocle::RecordingJobState prevstate)
{
  UpdateToolTip();

}

}
