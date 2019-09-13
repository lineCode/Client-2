// devicetreerecordingjobitem.cpp
//

///// Includes /////

#include "monocleclient/devicetreerecordingjobitem.h"

#include <QMenu>
#include <QMessageBox>

#include "monocleclient/devicetreerecordingjobsourceitem.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/managerecordingjobwindow.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordinglogwindow.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"
#include "monocleclient/recordingtrack.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTreeRecordingJobItem::DeviceTreeRecordingJobItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob) :
  DeviceTreeItem(parent, recordingjob->GetName()),
  device_(device),
  recording_(recording),
  recordingjob_(recordingjob),
  edit_(new QAction("Edit", this)),
  remove_(new QAction("Remove", this)),
  viewlog_(new QAction("View Log", this))
{
  connect(edit_, &QAction::triggered, this, &DeviceTreeRecordingJobItem::Edit);
  connect(remove_, &QAction::triggered, this, &DeviceTreeRecordingJobItem::Remove);
  connect(viewlog_, &QAction::triggered, this, &DeviceTreeRecordingJobItem::ViewLog);
  connect(recording.get(), &Recording::JobChanged, this, &DeviceTreeRecordingJobItem::JobChanged);
  connect(recording.get(), &Recording::ActiveJobChanged, this, &DeviceTreeRecordingJobItem::ActiveJobChanged);
  connect(recordingjob.get(), &RecordingJob::SourceTrackAdded, this, &DeviceTreeRecordingJobItem::JobSourceTrackAdded);
  connect(recordingjob.get(), &RecordingJob::SourceTrackRemoved, this, &DeviceTreeRecordingJobItem::JobSourceTrackRemoved);

  SetToolTip();
}

DeviceTreeRecordingJobItem::~DeviceTreeRecordingJobItem()
{
  removerecordingjobconnection_.Close();

}

void DeviceTreeRecordingJobItem::ContextMenuEvent(const QPoint& pos)
{
  QMenu* menu = new QMenu(treeWidget());
  menu->addAction(edit_);
  menu->addAction(remove_);
  menu->addAction(viewlog_);
  menu->exec(pos);
}

void DeviceTreeRecordingJobItem::Expanded()
{

}

void DeviceTreeRecordingJobItem::Collapsed()
{

}

void DeviceTreeRecordingJobItem::DoubleClicked()
{
  DeviceTreeItem::DoubleClicked();

}

void DeviceTreeRecordingJobItem::Edit(bool)
{
  ManageRecordingJobWindow(treeWidget(), device_, recording_, recordingjob_, nullptr).exec();

}

void DeviceTreeRecordingJobItem::ViewLog(bool)
{
  RecordingLogWindow(treeWidget(), device_, { recording_ }, std::vector< QSharedPointer<client::RecordingTrack> >(), { recordingjob_ }, { }).exec();

}

void DeviceTreeRecordingJobItem::Remove(bool)
{
  if (QMessageBox::question(treeWidget(), tr("Remove"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    removerecordingjobconnection_ = device_->RemoveRecordingJob(recording_->GetToken(), recordingjob_->GetToken(), [](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVERECORDINGJOBRESPONSE& removerecordingjobresponse)
    {
      if (removerecordingjobresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removerecordingjobresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
    });
  }
}

void DeviceTreeRecordingJobItem::JobChanged(const QSharedPointer<client::RecordingJob>& recordingjob)
{
  if (recordingjob_ == recordingjob)
  {
    setText(0, recordingjob->GetName());

  }
}

void DeviceTreeRecordingJobItem::ActiveJobChanged(const QSharedPointer<client::RecordingJob>& recordingjob)
{
  SetToolTip();

}

void DeviceTreeRecordingJobItem::SetToolTip()
{
  if (recording_->GetActiveJob() == recordingjob_)
  {
    setToolTip(0, QString("State: Active"));

  }
  else
  {
    setToolTip(0, QString("State: Idle"));

  }
}

void DeviceTreeRecordingJobItem::JobSourceTrackAdded(const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack)
{
  addChild(new DeviceTreeRecordingJobSourceItem(this, device_, recording_, recordingjob_, recordingjobsource, recordingjobsourcetrack));

}

void DeviceTreeRecordingJobItem::JobSourceTrackRemoved(const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const uint64_t token)
{
  for (int i = (childCount() - 1); i >= 0; --i)
  {
    DeviceTreeRecordingJobSourceItem* recordingjobsourcetrack = static_cast<DeviceTreeRecordingJobSourceItem*>(child(i));
    if ((recordingjobsourcetrack->GetRecordingJobSource() == recordingjobsource) && (recordingjobsourcetrack->GetRecordingJobSourceTrack()->GetToken() == token))
    {
      removeChild(child(i));

    }
  }
}

}
