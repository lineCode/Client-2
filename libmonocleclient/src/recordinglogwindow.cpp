// recordinglogwindow.cpp
//

///// Includes /////

#include "monocleclient/recordinglogwindow.h"

#include <QClipboard>
#include <QMessageBox>
#include <QTreeWidgetItem>

#include "monocleclient/device.h"
#include "monocleclient/receiver.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"

///// Namespaces /////

namespace client
{

///// Globals /////

const int ITEM_RECORDING = 0;
const int ITEM_RECORDING_TRACKS = 1;
const int ITEM_RECORDING_TRACK = 2;
const int ITEM_RECORDING_JOBS = 3;
const int ITEM_RECORDING_JOB = 4;
const int ITEM_RECORDING_JOB_SOURCE = 5;

const int ROLE_ITEM = Qt::UserRole;
const int ROLE_TRACK_TOKEN = Qt::UserRole + 1;
const int ROLE_JOB_TOKEN = Qt::UserRole + 2;
const int ROLE_JOB_SOURCE_TOKEN = Qt::UserRole + 3;
const int ROLE_JOB_SOURCE_TRACK_TOKEN = Qt::UserRole + 4;

///// Methods /////

RecordingLogWindow::RecordingLogWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const std::vector< QSharedPointer<client::RecordingTrack> >& filtertracks, const std::vector< QSharedPointer<client::RecordingJob> >& filterjobs, const std::vector< QSharedPointer<client::RecordingJobSourceTrack> >& filterjobsourcetracks) :
  QDialog(parent),
  device_(device),
  recording_(recording)
{
  ui_.setupUi(this);

  recordingtracksitem_ = new QTreeWidgetItem({ "Tracks" });
  recordingtracksitem_->setData(0, ROLE_ITEM, ITEM_RECORDING_TRACKS);
  recordingjobsitem_ = new QTreeWidgetItem({ "Jobs" });
  recordingjobsitem_->setData(0, ROLE_ITEM, ITEM_RECORDING_JOBS);

  connect(recording.get(), &Recording::TrackAdded, this, &RecordingLogWindow::RecordingTrackAdded, Qt::QueuedConnection);
  connect(recording.get(), &Recording::TrackChanged, this, &RecordingLogWindow::RecordingTrackChanged, Qt::QueuedConnection);
  connect(recording.get(), &Recording::TrackRemoved, this, &RecordingLogWindow::RecordingTrackRemoved, Qt::QueuedConnection);

  connect(recording.get(), &Recording::JobAdded, this, &RecordingLogWindow::RecordingJobAdded, Qt::QueuedConnection);
  connect(recording.get(), &Recording::JobChanged, this, &RecordingLogWindow::RecordingJobChanged, Qt::QueuedConnection);
  connect(recording.get(), &Recording::JobRemoved, this, &RecordingLogWindow::RecordingJobRemoved, Qt::QueuedConnection);

  connect(recording.get(), &Recording::JobSourceTrackAdded, this, &RecordingLogWindow::RecordingJobSourceTrackAdded, Qt::QueuedConnection);
  connect(recording.get(), &Recording::JobSourceTrackChanged, this, &RecordingLogWindow::RecordingJobSourceTrackChanged, Qt::QueuedConnection);
  connect(recording.get(), &Recording::JobSourceTrackRemoved, this, &RecordingLogWindow::RecordingJobSourceTrackRemoved, Qt::QueuedConnection);

  connect(recording.get(), &Recording::TrackLogMessage, this, &RecordingLogWindow::RecordingTrackLogMessage, Qt::QueuedConnection);
  connect(recording.get(), &Recording::JobLogMessage, this, &RecordingLogWindow::RecordingJobLogMessage, Qt::QueuedConnection);
  connect(recording.get(), &Recording::JobSourceTrackLogMessage, this, &RecordingLogWindow::RecordingJobSourceTrackLogMessage, Qt::QueuedConnection);
  connect(recording.get(), &Recording::LogMessage, this, &RecordingLogWindow::RecordingLogMessage, Qt::QueuedConnection);

  // Build the tree as we currently know it
  bool filter = false;
  if (filtertracks.size() || filterjobs.size() || filterjobsourcetracks.size())
  {
    filter = true;

  }

  ui_.treefilter->setRootIsDecorated(false);
  ui_.treefilter->header()->hide();

  QTreeWidgetItem* recordingitem = new QTreeWidgetItem({ recording_->GetName() });
  recordingitem->setData(0, ROLE_ITEM, ITEM_RECORDING);
  recordingitem->setCheckState(0, filter ? Qt::CheckState::Unchecked : Qt::CheckState::Checked);
  ui_.treefilter->addTopLevelItem(recordingitem);

  recordingitem->addChild(recordingtracksitem_);
  for (const QSharedPointer<client::RecordingTrack>& track : recording_->GetTracks())
  {
    bool checked = true;
    if (filter && (std::find(filtertracks.cbegin(), filtertracks.cend(), track) == filtertracks.cend()))
    {
      checked = false;

    }

    AddRecordingTrack(track, checked);
  }
  recordingtracksitem_->setExpanded(true);

  recordingitem->addChild(recordingjobsitem_);
  for (const QSharedPointer<client::RecordingJob>& recordingjob : recording_->GetRecordingJobs())
  {
    bool checked = true;
    if (filter && (std::find(filterjobs.cbegin(), filterjobs.cend(), recordingjob) == filterjobs.cend()))
    {
      checked = false;

    }

    AddRecordingJob(recordingjob, checked);

    for (const QSharedPointer<client::RecordingJobSource>& recordingjobsource : recordingjob->GetSources())
    {
      for (const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack : recordingjobsource->GetTracks())
      {
        bool checked = true;
        if (filter && (std::find(filterjobsourcetracks.cbegin(), filterjobsourcetracks.cend(), recordingjobsourcetrack) == filterjobsourcetracks.cend()))
        {
          checked = false;

        }

        AddRecordingJobSourceTrack(recordingjob, recordingjobsource, recordingjobsourcetrack, checked);
      }
    }
  }
  recordingjobsitem_->setExpanded(true);

  recordingitem->setExpanded(true);

  // Subscribe to all the logs we want to listen to
  boost::shared_ptr< std::vector<std::string> > messages = boost::make_shared< std::vector<std::string> >(); // messages will collate all the possible failures into a single QMessageBox if there is one failure or more(empty strings represent success)
  const size_t totalrequests = 4;
  subscriberecordingconnection_ = recording_->GetDevice()->SubscribeRecordingLog(recording_->GetToken(), [this, recording, messages, totalrequests](const std::chrono::steady_clock::duration latency, const monocle::client::SUBSCRIBERECORDINGLOGRESPONSE& subscriberecordinglogresponse)
  {
    if (subscriberecordinglogresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      messages->push_back(subscriberecordinglogresponse.GetErrorText());

    }
    else
    {
      messages->push_back(std::string());
      recordingmessages_ = subscriberecordinglogresponse.messages_;
      Update();
    }

    Message(messages, totalrequests);
  });

  subscriberecordingtrackconnection_ = recording_->GetDevice()->SubscribeRecordingTrackLog(recording_->GetToken(), [this, recording, messages, totalrequests](const std::chrono::steady_clock::duration latency, const monocle::client::SUBSCRIBERECORDINGTRACKLOGRESPONSE& subscriberecordingtracklogresponse)
  {
    if (subscriberecordingtracklogresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      messages->push_back(subscriberecordingtracklogresponse.GetErrorText());

    }
    else
    {
      messages->push_back(std::string());
      for (const std::pair< uint32_t, std::vector<monocle::LOGMESSAGE> >& recordingtracklogmessage : subscriberecordingtracklogresponse.recordingtracklogmessages_)
      {
        recordingtracks_[recordingtracklogmessage.first] = recordingtracklogmessage.second;

      }
      Update();
    }

    Message(messages, totalrequests);
  });

  subscriberecordingjobconnection_ = recording_->GetDevice()->SubscribeRecordingJobLog(recording_->GetToken(), [this, recording, messages, totalrequests](const std::chrono::steady_clock::duration latency, const monocle::client::SUBSCRIBERECORDINGJOBLOGRESPONSE& subscriberecordingjoblogresponse)
  {
    if (subscriberecordingjoblogresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      messages->push_back(subscriberecordingjoblogresponse.GetErrorText());

    }
    else
    {
      messages->push_back(std::string());
      for (const std::pair< uint64_t, std::vector<monocle::LOGMESSAGE> >& recordingjobloglogmessages : subscriberecordingjoblogresponse.recordingjoblogmessages_)
      {
        recordingjobs_[recordingjobloglogmessages.first] = recordingjobloglogmessages.second;

      }
      Update();
    }

    Message(messages, totalrequests);
  });

  subscriberecordingjobsourcetrackconnection_ = recording_->GetDevice()->SubscribeRecordingJobSourceTrackLog(recording_->GetToken(), [this, recording, messages, totalrequests](const std::chrono::steady_clock::duration latency, const monocle::client::SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE& subscriberecordingjobsourcetracklogresponse)
  {
    if (subscriberecordingjobsourcetracklogresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      messages->push_back(subscriberecordingjobsourcetracklogresponse.GetErrorText());

    }
    else
    {
      messages->push_back(std::string());
      for (const std::pair< monocle::client::SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE::RECORDINGJOBSOURCETRACKTOKEN, std::vector<monocle::LOGMESSAGE> >& recordingtracklogmessage : subscriberecordingjobsourcetracklogresponse.recordingjobsourcetracklogmessages_)
      {
        recordingjobsourcetracks_[std::make_tuple(recordingtracklogmessage.first.recordingjobtoken_, recordingtracklogmessage.first.recordingjobsourcetoken_, recordingtracklogmessage.first.recordingjobsourcetracktoken_)] = recordingtracklogmessage.second;

      }
      Update();
    }

    Message(messages, totalrequests);
  });
}

RecordingLogWindow::~RecordingLogWindow()
{
  subscriberecordingconnection_.Close();
  subscriberecordingtrackconnection_.Close();
  subscriberecordingjobconnection_.Close();
  subscriberecordingjobsourcetrackconnection_.Close();

  recording_->GetDevice()->UnsubscribeRecordingJobLog(recording_->GetToken());
  recording_->GetDevice()->UnsubscribeRecordingTrackLog(recording_->GetToken());
  recording_->GetDevice()->UnsubscribeRecordingJobSourceTrackLog(recording_->GetToken());
  recording_->GetDevice()->UnsubscribeRecordingLog(recording_->GetToken());
}

void RecordingLogWindow::Update()
{
  std::vector<monocle::LOGMESSAGE> messages;

  if (ui_.treefilter->topLevelItem(0)->checkState(0) == Qt::Checked)
  {
    messages = recordingmessages_;

  }

  for (const std::pair< uint32_t, std::vector<monocle::LOGMESSAGE> >& recordingtrack : recordingtracks_)
  {
    if (FilterRecordingTrack(recordingtrack.first))
    {
      messages.insert(messages.end(), recordingtrack.second.begin(), recordingtrack.second.end());

    }
  }

  for (const std::pair< uint64_t, std::vector<monocle::LOGMESSAGE> >& recordingjob : recordingjobs_)
  {
    if (FilterRecordingJob(recordingjob.first))
    {
      messages.insert(messages.end(), recordingjob.second.begin(), recordingjob.second.end());

    }
  }

  for (const std::pair< std::tuple<uint64_t, uint64_t, uint64_t>, std::vector<monocle::LOGMESSAGE> >& recordingjobsourcetrack : recordingjobsourcetracks_)
  {
    if (FilterRecordingJobSource(std::get<0>(recordingjobsourcetrack.first), std::get<1>(recordingjobsourcetrack.first), std::get<2>(recordingjobsourcetrack.first)))
    {
      messages.insert(messages.end(), recordingjobsourcetrack.second.begin(), recordingjobsourcetrack.second.end());

    }
  }
  std::sort(messages.begin(), messages.end(), [](const monocle::LOGMESSAGE& lhs, const monocle::LOGMESSAGE& rhs) { return (lhs.time_ < rhs.time_); });
  
  // Create the text
  QString text;
  for (const monocle::LOGMESSAGE& logmessage : messages)
  {
    if (Filter(logmessage.severity_))
    {
      text.append(HTML(logmessage.severity_, QString::fromStdString(logmessage.message_)));

    }
  }
  ui_.textlog->setHtml(text);
}

void RecordingLogWindow::RecordingTrackAdded(const QSharedPointer<client::RecordingTrack>& track)
{
  AddRecordingTrack(track, true);
  
}

void RecordingLogWindow::RecordingTrackChanged(const QSharedPointer<client::RecordingTrack>& track)
{
  QTreeWidgetItem* item = FindTrackItem(track->GetId());
  if (!item)
  {

    return;
  }
  item->setText(0, track->GetDescription());
}

void RecordingLogWindow::RecordingTrackRemoved(const uint32_t trackid)
{
  QTreeWidgetItem* item = FindTrackItem(trackid);
  if (!item)
  {

    return;
  }
  QTreeWidgetItem* parent = item->parent();
  parent->removeChild(item);
}

void RecordingLogWindow::RecordingJobAdded(const QSharedPointer<client::RecordingJob>& recordingjob)
{
  AddRecordingJob(recordingjob, true);

}

void RecordingLogWindow::RecordingJobChanged(const QSharedPointer<client::RecordingJob>& recordingjob)
{
  QTreeWidgetItem* item = FindJobItem(recordingjob->GetToken());
  if (!item)
  {

    return;
  }
  item->setText(0, recordingjob->GetName());
}

void RecordingLogWindow::RecordingJobRemoved(const uint64_t recordingjobtoken)
{
  QTreeWidgetItem* item = FindJobItem(recordingjobtoken);
  if (!item)
  {

    return;
  }
  QTreeWidgetItem* parent = item->parent();
  parent->removeChild(item);
}

void RecordingLogWindow::RecordingJobSourceTrackAdded(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack)
{
  AddRecordingJobSourceTrack(recordingjob, recordingjobsource, recordingjobsourcetrack, true);

}

void RecordingLogWindow::RecordingJobSourceTrackChanged(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack)
{
  QTreeWidgetItem* item = FindJobSourceTrackItem(recordingjob->GetToken(), recordingjobsource->GetToken(), recordingjobsourcetrack->GetToken());
  if (!item)
  {

    return;
  }

  const QSharedPointer<Receiver> receiver = device_->GetReceiver(recordingjobsource->GetReceiverToken());
  QString name = "<None>";
  if (receiver)
  {
    name = receiver->GetMediaUri();

  }
  item->setText(0, name);
}

void RecordingLogWindow::RecordingJobSourceTrackRemoved(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const uint64_t token)
{
  QTreeWidgetItem* item = FindJobSourceTrackItem(recordingjob->GetToken(), recordingjobsource->GetToken(), token);
  if (!item)
  {

    return;
  }
  QTreeWidgetItem* parent = item->parent();
  parent->removeChild(item);
}

void RecordingLogWindow::RecordingTrackLogMessage(const QSharedPointer<client::RecordingTrack>& recordingtrack, const uint64_t time, const monocle::Severity severity, const QString& message)
{
  auto rt = recordingtracks_.find(recordingtrack->GetId());
  if (rt == recordingtracks_.end())
  {

    return;
  }

  if (std::find(rt->second.cbegin(), rt->second.cend(), monocle::LOGMESSAGE(time, severity, message.toStdString())) != rt->second.cend()) // Ignore duplicate messages which are possible
  {

    return;
  }

  rt->second.push_back(monocle::LOGMESSAGE(time, severity, message.toStdString()));

  if (!FilterRecordingTrack(recordingtrack->GetId()))
  {

    return;
  }

  if (!Filter(severity))
  {

    return;
  }
  ui_.textlog->moveCursor(QTextCursor::End);
  ui_.textlog->insertHtml(HTML(severity, message));
}

void RecordingLogWindow::RecordingJobLogMessage(const QSharedPointer<client::RecordingJob>& recordingjob, const uint64_t time, const monocle::Severity severity, const QString& message)
{
  auto rj = recordingjobs_.find(recordingjob->GetToken());
  if (rj == recordingjobs_.end())
  {

    return;
  }

  if (std::find(rj->second.cbegin(), rj->second.cend(), monocle::LOGMESSAGE(time, severity, message.toStdString())) != rj->second.cend()) // Ignore duplicate messages which are possible
  {

    return;
  }

  rj->second.push_back(monocle::LOGMESSAGE(time, severity, message.toStdString()));

  if (!FilterRecordingJob(recordingjob->GetToken()))
  {

    return;
  }

  if (!Filter(severity))
  {

    return;
  }
  ui_.textlog->moveCursor(QTextCursor::End);
  ui_.textlog->insertHtml(HTML(severity, message));
}

void RecordingLogWindow::RecordingJobSourceTrackLogMessage(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack, const uint64_t time, const monocle::Severity severity, const QString& message)
{
  auto rjst = recordingjobsourcetracks_.find(std::make_tuple(recordingjob->GetToken(), recordingjobsource->GetToken(), recordingjobsourcetrack->GetToken()));
  if (rjst == recordingjobsourcetracks_.end())
  {

    return;
  }

  if (std::find(rjst->second.cbegin(), rjst->second.cend(), monocle::LOGMESSAGE(time, severity, message.toStdString())) != rjst->second.cend()) // Ignore duplicate messages which are possible
  {

    return;
  }

  rjst->second.push_back(monocle::LOGMESSAGE(time, severity, message.toStdString()));

  if (!FilterRecordingJobSource(recordingjob->GetToken(), recordingjobsource->GetToken(), recordingjobsourcetrack->GetToken()))
  {

    return;
  }

  if (!Filter(severity))
  {

    return;
  }
  ui_.textlog->moveCursor(QTextCursor::End);
  ui_.textlog->insertHtml(HTML(severity, message));
}

void RecordingLogWindow::RecordingLogMessage(const uint64_t time, const monocle::Severity severity, const QString& message)
{
  if (std::find(recordingmessages_.cbegin(), recordingmessages_.cend(), monocle::LOGMESSAGE(time, severity, message.toStdString())) != recordingmessages_.cend()) // Ignore duplicate messages which are possible
  {

    return;
  }

  recordingmessages_.push_back(monocle::LOGMESSAGE(time, severity, message.toStdString()));

  if (!Filter(severity))
  {

    return;
  }
  ui_.textlog->moveCursor(QTextCursor::End);
  ui_.textlog->insertHtml(HTML(severity, message));
}

bool RecordingLogWindow::Filter(const monocle::Severity severity) const
{
  switch (severity)
  {
    case monocle::Severity::Trace:
    {

      return ui_.checktrace->isChecked();
    }
    case monocle::Severity::Debug:
    {

      return ui_.checkdebug->isChecked();
    }
    case monocle::Severity::Info:
    {

      return ui_.checkinfo->isChecked();
    }
    case monocle::Severity::Warn:
    {

      return ui_.checkwarning->isChecked();
    }
    case monocle::Severity::Err:
    {

      return ui_.checkerror->isChecked();
    }
    case monocle::Severity::Critical:
    {

      return ui_.checkcritical->isChecked();
    }
    default:
    {

      return false;
    }
  }
}

bool RecordingLogWindow::FilterRecordingTrack(const uint32_t recordingtrackid) const
{
  QTreeWidgetItem* item = FindTrackItem(recordingtrackid);
  if (!item)
  {

    return false;
  }
  else
  {
    if (item->checkState(0) == Qt::Checked)
    {

      return true;
    }
    else
    {

      return false;
    }
  }

  return false;
}

bool RecordingLogWindow::FilterRecordingJob(const uint64_t recordingjobtoken) const
{
  QTreeWidgetItem* item = FindJobItem(recordingjobtoken);
  if (!item)
  {

    return false;
  }
  else
  {
    if (item->checkState(0) == Qt::Checked)
    {

      return true;
    }
    else
    {

      return false;
    }
  }
  return false;
}

bool RecordingLogWindow::FilterRecordingJobSource(const uint64_t jobtoken, const uint64_t jobsourcetoken, const uint64_t jobsourcetracktoken) const
{
  QTreeWidgetItem* item = FindJobSourceTrackItem(jobtoken, jobsourcetoken, jobsourcetracktoken);
  if (!item)
  {

    return false;
  }
  else
  {
    if (item->checkState(0) == Qt::Checked)
    {

      return true;
    }
    else
    {

      return false;
    }
  }
  return false;
}

QString RecordingLogWindow::HTML(const monocle::Severity severity, const QString& message) const
{
  QString html = "<font color=\"";
  switch (severity)
  {
    case monocle::Severity::Trace:
    case monocle::Severity::Debug:
    case monocle::Severity::Info:
    {
      html += "Green\">";
      break;
    }
    case monocle::Severity::Warn:
    {
      html += "Orange\">";
      break;
    }
    case monocle::Severity::Err:
    case monocle::Severity::Critical:
    {
      html += "Red\">";
      break;
    }
    default:
    {

      return QString();
    }
  }
  html += message + "</font><br>";
  return html;
}

void RecordingLogWindow::Message(const boost::shared_ptr< std::vector<std::string> >& messages, const size_t totalrequests) const
{
  QString text;
  if (messages->size() == totalrequests)
  {
    for (const std::string& message : *messages)
    {
      if (message.size())
      {
        text += QString::fromStdString(message) + "\r\n";

      }
    }

    if (text.size())
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), text, QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();

    }
  }
}

void RecordingLogWindow::AddRecordingTrack(const QSharedPointer<client::RecordingTrack>& track, const bool checked)
{
  QTreeWidgetItem* recordingtrackitem = new QTreeWidgetItem({ track->GetDescription() });
  recordingtrackitem->setData(0, ROLE_ITEM, ITEM_RECORDING_TRACK);
  recordingtrackitem->setData(0, ROLE_TRACK_TOKEN, track->GetId());
  recordingtrackitem->setCheckState(0, checked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  recordingtracksitem_->addChild(recordingtrackitem);
  recordingtrackitem->setExpanded(true);
}

void RecordingLogWindow::AddRecordingJob(const QSharedPointer<client::RecordingJob>& recordingjob, const bool checked)
{
  QTreeWidgetItem* recordingjobitem = new QTreeWidgetItem({ recordingjob->GetName() });
  recordingjobitem->setData(0, ROLE_ITEM, ITEM_RECORDING_JOB);
  recordingjobitem->setData(0, ROLE_JOB_TOKEN, static_cast<qulonglong>(recordingjob->GetToken()));
  recordingjobitem->setCheckState(0, checked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  recordingjobsitem_->addChild(recordingjobitem);
  recordingjobitem->setExpanded(true);
}

void RecordingLogWindow::AddRecordingJobSourceTrack(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack, const bool checked)
{
  QTreeWidgetItem* recordingjobitem = FindJobItem(recordingjob->GetToken());
  if (!recordingjobitem)
  {
    // Just ignore this for the moment
    return;
  }

  const QSharedPointer<Receiver> receiver = device_->GetReceiver(recordingjobsource->GetReceiverToken());
  QString name = "<None>";
  if (receiver)
  {
    name = receiver->GetMediaUri();

  }

  QTreeWidgetItem* recordingjobsourceitem = new QTreeWidgetItem({ name });
  recordingjobsourceitem->setData(0, ROLE_ITEM, ITEM_RECORDING_JOB_SOURCE);
  recordingjobsourceitem->setData(0, ROLE_JOB_TOKEN, static_cast<qulonglong>(recordingjob->GetToken()));
  recordingjobsourceitem->setData(0, ROLE_JOB_SOURCE_TOKEN, static_cast<qulonglong>(recordingjobsource->GetToken()));
  recordingjobsourceitem->setData(0, ROLE_JOB_SOURCE_TRACK_TOKEN, static_cast<qulonglong>(recordingjobsourcetrack->GetToken()));
  recordingjobsourceitem->setCheckState(0, checked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  recordingjobitem->addChild(recordingjobsourceitem);
  recordingjobsourceitem->setExpanded(true);
}

QTreeWidgetItem* RecordingLogWindow::FindTrackItem(const uint64_t token) const
{
  QTreeWidgetItemIterator it(ui_.treefilter);
  while (*it)
  {
    auto i = *it;
    ++it;
    if (i->data(0, ROLE_ITEM) != ITEM_RECORDING_TRACK)
    {

      continue;
    }

    if (i->data(0, ROLE_TRACK_TOKEN).toULongLong() != token)
    {

      continue;
    }
    return i;
  }
  return nullptr;
}

QTreeWidgetItem* RecordingLogWindow::FindJobItem(const uint64_t token) const
{
  QTreeWidgetItemIterator it(ui_.treefilter);
  while (*it)
  {
    auto i = *it;
    ++it;
    if (i->data(0, ROLE_ITEM) != ITEM_RECORDING_JOB)
    {

      continue;
    }

    if (i->data(0, ROLE_JOB_TOKEN).toULongLong() != token)
    {

      continue;
    }
    return i;
  }
  return nullptr;
}

QTreeWidgetItem* RecordingLogWindow::FindJobSourceTrackItem(const uint64_t jobtoken, const uint64_t jobsourcetoken, const uint64_t jobsourcetracktoken) const
{
  QTreeWidgetItemIterator it(ui_.treefilter);
  while (*it)
  {
    auto i = *it;
    ++it;
    if (i->data(0, ROLE_ITEM) != ITEM_RECORDING_JOB_SOURCE)
    {

      continue;
    }

    if (i->data(0, ROLE_JOB_TOKEN).toULongLong() != jobtoken)
    {

      continue;
    }

    if (i->data(0, ROLE_JOB_SOURCE_TOKEN).toULongLong() != jobsourcetoken)
    {

      continue;
    }

    if (i->data(0, ROLE_JOB_SOURCE_TRACK_TOKEN).toULongLong() != jobsourcetracktoken)
    {

      continue;
    }
    return i;
  }
  return nullptr;
}

void RecordingLogWindow::on_checktrace_clicked()
{
  Update();

}

void RecordingLogWindow::on_checkdebug_clicked()
{
  Update();

}

void RecordingLogWindow::on_checkinfo_clicked()
{
  Update();

}

void RecordingLogWindow::on_checkwarning_clicked()
{
  Update();

}

void RecordingLogWindow::on_checkerror_clicked()
{
  Update();

}

void RecordingLogWindow::on_checkcritical_clicked()
{
  Update();

}

void RecordingLogWindow::on_treefilter_itemChanged(QTreeWidgetItem* item, int column)
{
  // We could potentially do something in here so I'll leave it available
  const int itemtype = item->data(0, ROLE_ITEM).toInt();
  switch (itemtype)
  {
    case ITEM_RECORDING:
    {

      break;
    }
    case ITEM_RECORDING_TRACKS:
    {

      break;
    }
    case ITEM_RECORDING_TRACK:
    {

      break;
    }
    case ITEM_RECORDING_JOBS:
    {

      break;
    }
    case ITEM_RECORDING_JOB:
    {

      break;
    }
  }

  Update();
}

void RecordingLogWindow::on_buttoncopy_clicked()
{
  QApplication::clipboard()->setText(ui_.textlog->toPlainText());

}

void RecordingLogWindow::on_buttonok_clicked()
{
  accept();
  
}

}
