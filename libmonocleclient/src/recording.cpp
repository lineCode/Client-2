// recording.cpp
//

///// Includes /////

#include "monocleclient/recording.h"

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"

///// Namespaces /////

namespace client
{

///// Methods /////

Recording::Recording(const boost::shared_ptr<Device>& device, const uint64_t token, const QString& sourceid, const QString& name, const QString& location, const QString& description, const QString& address, const QString& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob) :
  device_(device),
  token_(token),
  sourceid_(sourceid),
  name_(name),
  location_(location),
  description_(description),
  address_(address),
  content_(content),
  retentiontime_(retentiontime)
{

}

Recording::~Recording()
{

}

QSharedPointer<client::RecordingJob> Recording::GetJob(const uint64_t jobtoken) const
{
  std::vector< QSharedPointer<client::RecordingJob> >::const_iterator job = std::find_if(jobs_.cbegin(), jobs_.cend(), [jobtoken](const QSharedPointer<client::RecordingJob>& job) { return (job->GetToken() == jobtoken); });
  if (job == jobs_.cend())
  {

    return nullptr;
  }
  return (*job);
}

QSharedPointer<client::RecordingTrack> Recording::GetTrack(const uint32_t trackid) const
{
  auto track = std::find_if(tracks_.begin(), tracks_.end(), [trackid](const QSharedPointer<client::RecordingTrack>& track) { return (track->GetId() == trackid); });
  if (track == tracks_.end())
  {

    return nullptr;
  }
  else
  {

    return *track;
  }
}

monocle::RecordingJobState Recording::GetState(const QSharedPointer<client::RecordingTrack>& track) const
{
  if (activejob_ == nullptr)
  {

    return monocle::RecordingJobState::Idle;
  }
  return activejob_->GetState(track);
}

QSharedPointer<client::RecordingTrack> Recording::AddTrack(const monocle::RECORDINGTRACK& track)
{
  std::vector< std::pair<uint64_t, uint64_t> > indices;
  indices.reserve(track.indices_.size());
  for (const monocle::INDEX& index : track.indices_)
  {
    indices.push_back(std::make_pair(index.starttime(), index.endtime()));

  }

  tracks_.push_back(QSharedPointer<client::RecordingTrack>::create(track.id_, QString::fromStdString(track.token_), track.tracktype_, QString::fromStdString(track.description_), track.fixedfiles_, track.digitalsignature_, track.encrypt_, track.flushfrequency_, track.files_, indices, track.codecindices_));
  emit TrackAdded(tracks_.back());
  return tracks_.back();
}

QSharedPointer<client::RecordingTrack> Recording::ChangeTrack(const uint32_t id, const QString& token, const monocle::TrackType tracktype, const QString& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<monocle::CODECINDEX>& codecindices)
{
  auto track = std::find_if(tracks_.begin(), tracks_.end(), [id](const QSharedPointer<client::RecordingTrack>& track) { return (track->GetId() == id); });
  if (track == tracks_.end())
  {

    return nullptr;
  }

  (*track)->ChangeTrack(token, tracktype, description, fixedfiles, digitalsigning, encrypt, flushfrequency, files, codecindices);
  emit TrackChanged(*track);
  return *track;
}

void Recording::AddJob(const QSharedPointer<client::RecordingJob>& job)
{
  jobs_.push_back(job);
  emit JobAdded(job);
}

QSharedPointer<client::RecordingJob> Recording::ChangeJob(const uint64_t token, const QString& name, const bool enabled, const uint64_t priority)
{
  auto job = std::find_if(jobs_.begin(), jobs_.end(), [token](const QSharedPointer<client::RecordingJob>& job) { return (job->GetToken() == token); });
  if (job == jobs_.end())
  {

    return nullptr;
  }
  (*job)->Set(name, enabled, priority);
  emit JobChanged(*job);
  return *job;
}

void Recording::RemoveJob(const uint64_t token)
{
  auto job = std::find_if(jobs_.begin(), jobs_.end(), [token](const QSharedPointer<client::RecordingJob>& job) { return (job->GetToken() == token); });
  if (job == jobs_.end())
  {

    return;
  }
  jobs_.erase(job);
  emit JobRemoved(token);
}

void Recording::RemoveTrack(const uint32_t id)
{
  auto track = std::find_if(tracks_.begin(), tracks_.end(), [id](const QSharedPointer<client::RecordingTrack>& track) { return (track->GetId() == id); });
  if (track == tracks_.end())
  {

    return;
  }
  tracks_.erase(track);
  emit TrackRemoved(id);
}

std::vector< QSharedPointer<client::RecordingTrack> > Recording::GetTracks(const monocle::TrackType tracktype) const
{
  std::vector< QSharedPointer<client::RecordingTrack> > tracks;
  for (const QSharedPointer<client::RecordingTrack>& track : tracks_)
  {
    if (track->GetTrackType() == tracktype)
    {
      tracks.push_back(track);

    }
  }
  return tracks;
}

unsigned int Recording::GetNumVideoTracks() const
{
  unsigned int numvideotracks = 0;
  for (const QSharedPointer<client::RecordingTrack>& track : tracks_)
  {
    if (track->GetTrackType() == monocle::TrackType::Video)
    {
      ++numvideotracks;

    }
  }
  return numvideotracks;
}

std::vector< QSharedPointer<client::RecordingTrack> > Recording::GetVideoTracks() const
{

  return GetTracks(monocle::TrackType::Video);
}

std::vector< QSharedPointer<client::RecordingTrack> > Recording::GetAudioTracks() const
{

  return GetTracks(monocle::TrackType::Audio);
}

std::vector< QSharedPointer<client::RecordingTrack> > Recording::GetMetadataTracks() const
{

  return GetTracks(monocle::TrackType::Metadata);
}

std::vector< QSharedPointer<client::RecordingTrack> > Recording::GetObjectDetectorTracks() const
{

  return GetTracks(monocle::TrackType::ObjectDetector);
}

bool Recording::HasObjectDetectorTracks() const
{
  for (const QSharedPointer<client::RecordingTrack>& track : tracks_)
  {
    if (track->GetTrackType() == monocle::TrackType::ObjectDetector)
    {

      return true;
    }
  }
  return false;
}

std::vector< QSharedPointer<client::Receiver> > Recording::GetActiveReceivers(const QSharedPointer<client::RecordingTrack>& track) const
{
  if (activejob_ == nullptr)
  {

    return std::vector< QSharedPointer<client::Receiver> >();
  }

  std::vector< QSharedPointer<client::Receiver> > activereceivers;
  for (const uint64_t receivertoken : activejob_->GetReceivers(track))
  {
    QSharedPointer<client::Receiver> receiver = device_->GetReceiver(receivertoken);
    if (!receiver)
    {
      LOG_GUI_WARNING_SOURCE(device_, QString("Unable to find receiver: ") + QString::number(receivertoken));
      continue;
    }
    if (std::find(activereceivers.cbegin(), activereceivers.cend(), receiver) != activereceivers.cend()) // Don't want duplicates
    {

      continue;
    }
    activereceivers.push_back(receiver);
  }
  return activereceivers;
}

std::vector<QString> Recording::GetActiveProfileTokens(const QSharedPointer<client::RecordingTrack>& track) const
{
  if (activejob_ == nullptr)
  {

    return std::vector<QString>();
  }
  return activejob_->GetActiveProfileTokens(track);
}

std::vector<ROTATION> Recording::GetActiveRotations(const QSharedPointer<client::RecordingTrack>& track) const
{
  if (activejob_ == nullptr)
  {

    return std::vector<ROTATION>();
  }
  return activejob_->GetActiveRotations(track);
}

size_t Recording::GetNumObjectDetectors() const
{
  size_t total = 0;
  for (const QSharedPointer<RecordingJob>& job : jobs_)
  {
    total += job->GetNumObjectDetectors();

  }
  return total;
}

void Recording::SetActiveJob(const boost::optional<uint64_t>& activejob)
{
  if ((activejob_ == nullptr) && !activejob.is_initialized())
  {

    return;
  }

  if (activejob_ && activejob.is_initialized() && (activejob_->GetToken() == *activejob))
  {

    return;
  }

  if (activejob.is_initialized())
  {
    auto job = std::find_if(jobs_.begin(), jobs_.end(), [&activejob](const QSharedPointer<client::RecordingJob>& job) { return (job->GetToken() == *activejob); });
    if (job == jobs_.end())
    {
      LOG_GUI_WARNING_SOURCE(device_, QString("Unable to find activejob: ") + QString::number(*activejob));
      activejob_.reset();
    }
    else
    {
      activejob_ = *job;

    }
  }
  else
  {
    activejob_.reset();

  }
  emit ActiveJobChanged(activejob_);
}

void Recording::Set(const QString& sourceid, const QString& name, const QString& location, const QString& description, const QString& address, const QString& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob)
{
  sourceid_ = sourceid;
  name_ = name;
  location_ = location;
  description_ = description;
  address_ = address;
  content_ = content;
  retentiontime_ = retentiontime;
  SetActiveJob(activejob);
}

}
