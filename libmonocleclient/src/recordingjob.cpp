// recordingjob.cpp
//

///// Includes /////

#include "monocleclient/recordingjob.h"

#include <monocleprotocol/recordingjobstate_generated.h>
#include <network/uri.hpp>

#include "monocleclient/device.h"
#include "monocleclient/receiver.h"
#include "monocleclient/recordingjobsource.h"

///// Namespaces /////

namespace client
{

///// Methods /////

RecordingJob::RecordingJob(const boost::shared_ptr<Device>& device, const uint64_t token, const QString& name, const bool enabled, const uint64_t priority) :
  device_(device),
  token_(token),
  name_(name),
  enabled_(enabled),
  priority_(priority)
{

}

RecordingJob::~RecordingJob()
{

}

void RecordingJob::Set(const QString& name, const bool enabled, const uint64_t priority)
{
  name_ = name;
  enabled_ = enabled;
  priority_ = priority;
}

void RecordingJob::AddSource(const QSharedPointer<client::RecordingJobSource>& source)
{
  sources_.push_back(source);

}

QSharedPointer<RecordingJobSource> RecordingJob::GetSource(const uint64_t sourcetoken) const
{
  std::vector< QSharedPointer<RecordingJobSource> >::const_iterator source = std::find_if(sources_.cbegin(), sources_.cend(), [sourcetoken](const QSharedPointer<client::RecordingJobSource>& source) { return (source->GetToken() == sourcetoken); });
  if (source == sources_.cend())
  {

    return nullptr;
  }
  return (*source);
}

std::pair< QSharedPointer<RecordingJobSource>, QSharedPointer<client::RecordingJobSourceTrack> > RecordingJob::GetSourceTrack(const uint64_t sourcetracktoken) const
{
  for (const QSharedPointer<client::RecordingJobSource>& source : sources_)
  {
    const QSharedPointer<client::RecordingJobSourceTrack> sourcetrack = source->GetTrack(sourcetracktoken);
    if (sourcetrack)
    {

      return std::make_pair(source, sourcetrack);
    }
  }
  return std::make_pair(nullptr, nullptr);
}

void RecordingJob::RemoveSource(const uint64_t token)
{
  std::vector< QSharedPointer<RecordingJobSource> >::const_iterator source = std::find_if(sources_.cbegin(), sources_.cend(), [token](const QSharedPointer<client::RecordingJobSource>& source) { return (source->GetToken() == token); });
  if (source == sources_.cend())
  {

    return;
  }
  sources_.erase(source);
}

monocle::RecordingJobState RecordingJob::GetState(const QSharedPointer<client::RecordingTrack>& track) const
{
  for (const QSharedPointer<client::RecordingJobSource>& source : sources_)
  {
    const monocle::RecordingJobState state = source->GetState(track);
    if (state != monocle::RecordingJobState::Idle)
    {

      return state;
    }
  }
  return monocle::RecordingJobState::Idle;
}

std::vector<uint64_t> RecordingJob::GetReceivers(const QSharedPointer<client::RecordingTrack>& track) const
{
  std::vector<uint64_t> receivers;
  for (const QSharedPointer<client::RecordingJobSource>& source : sources_)
  {
    for (const uint64_t receivertoken : source->GetReceivers(track))
    {
      receivers.push_back(receivertoken);

    }
  }
  return receivers;
}

std::vector<QString> RecordingJob::GetActiveProfileTokens(const QSharedPointer<client::RecordingTrack>& track) const
{
  std::vector<QString> result;
  for (const QSharedPointer<client::RecordingJobSource>& source : sources_)
  {
    const std::vector<QString> activeprofiletokens = source->GetActiveProfileTokens(track);
    result.insert(result.end(), activeprofiletokens.begin(), activeprofiletokens.end());
  }
  return result;
}

std::vector<ROTATION> RecordingJob::GetActiveRotations(const QSharedPointer<client::RecordingTrack>& track) const
{
  std::vector<ROTATION> result;
  for (const QSharedPointer<client::RecordingJobSource>& source : sources_)
  {
    const std::vector<ROTATION> activerotations = source->GetActiveRotations(track);
    result.insert(result.end(), activerotations.begin(), activerotations.end());
  }
  return result;
}

size_t RecordingJob::GetNumObjectDetectors() const
{
  size_t total = 0;
  for (const QSharedPointer<RecordingJobSource>& source : sources_)
  {
    //TODO this should now look at the track it points too, and whether it is of TrackType::ObjectDetector
    const QSharedPointer<Receiver> receiver = device_->GetReceiver(source->GetReceiverToken());
    if (receiver)
    {
      try
      {
        const network::uri uri(receiver->GetMediaUri().toStdString());
        if (uri.has_scheme() && (uri.scheme().to_string() == "objectdetector"))
        {
          total += source->GetTracks().size();

        }
      }
      catch (...)
      {

      }
    }
  }
  return total;
}

std::vector< QSharedPointer<RecordingJobSource> > RecordingJob::GetObjectDetectors(const uint32_t trackid) const
{
  std::vector< QSharedPointer<RecordingJobSource> > results;
  for (const QSharedPointer<RecordingJobSource>& source : sources_)
  {
    // Check to see whether this source is an object detector pointing to the original track
    QSharedPointer<Receiver> receiver = device_->GetReceiver(source->GetReceiverToken());
    if (!receiver)
    {

      continue;
    }

    try
    {
      network::uri uri(receiver->GetMediaUri().toStdString());
      if (!uri.has_scheme() || !uri.has_path())
      {

        continue;
      }

      if (uri.scheme().compare("objectdetector"))
      {

        continue;
      }

      if (uri.path().compare(std::to_string(trackid)))//TODO not sure this works, be careful?
      {

        continue;
      }

      if (source->GetTracks(monocle::TrackType::ObjectDetector).empty())
      {

        continue;
      }

      results.push_back(source);
    }
    catch (...)
    {

      continue;
    }
  }
  return results;
}

}
