// recordingjobsource.cpp
//

///// Includes /////

#include "monocleclient/recordingjobsource.h"

#include <monocleprotocol/recordingjobstate_generated.h>

#include "monocleclient/recordingjobsourcetrack.h"
#include "monocleclient/recordingtrack.h"

///// Namespaces /////

namespace client
{

///// Methods /////

RecordingJobSource::RecordingJobSource(const uint64_t token, const uint64_t receivertoken) :
  token_(token),
  receivertoken_(receivertoken)
{

}

RecordingJobSource::~RecordingJobSource()
{

}

void RecordingJobSource::AddTrack(const QSharedPointer<client::RecordingJobSourceTrack>& track)
{
  tracks_.push_back(track);

}

void RecordingJobSource::RemoveTrack(const uint64_t token)
{
  std::vector< QSharedPointer<client::RecordingJobSourceTrack> >::const_iterator track = std::find_if(tracks_.cbegin(), tracks_.cend(), [token](const QSharedPointer<client::RecordingJobSourceTrack>& track) { return (track->GetToken() == token); });
  if (track == tracks_.cend())
  {
    // Could provide a warning here, but we don't have the device handy
    return;
  }
  tracks_.erase(track);
}

std::vector < QSharedPointer<client::RecordingJobSourceTrack> > RecordingJobSource::GetTracks(const monocle::TrackType tracktype)
{
  std::vector< QSharedPointer<client::RecordingJobSourceTrack> > tracks;
  for (const QSharedPointer<client::RecordingJobSourceTrack>& track : tracks_)
  {
    if (!track->GetTrack())
    {

      continue;
    }

    if (track->GetTrack()->GetTrackType() == tracktype)
    {
      tracks.push_back(track);

    }
  }
  return tracks;
}

QSharedPointer<client::RecordingJobSourceTrack> RecordingJobSource::GetTrack(const uint64_t tracktoken) const
{
  std::vector< QSharedPointer<client::RecordingJobSourceTrack> >::const_iterator track = std::find_if(tracks_.cbegin(), tracks_.cend(), [tracktoken](const QSharedPointer<client::RecordingJobSourceTrack>& track) { return (track->GetToken() == tracktoken); });
  if (track == tracks_.cend())
  {

    return nullptr;
  }
  return (*track);
}

monocle::RecordingJobState RecordingJobSource::GetState(const QSharedPointer<client::RecordingTrack>& track) const
{
  for (const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack : tracks_)
  {
    if ((recordingjobsourcetrack->GetTrack() == track) && (recordingjobsourcetrack->GetState() != monocle::RecordingJobState::Idle))
    {

      return recordingjobsourcetrack->GetState();
    }
  }
  return monocle::RecordingJobState::Idle;
}

std::vector<uint64_t> RecordingJobSource::GetReceivers(const QSharedPointer<client::RecordingTrack>& track) const
{
  std::vector<uint64_t> receivers;
  for (const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack : tracks_)
  {
    if (recordingjobsourcetrack->GetTrack() != track)
    {

      continue;
    }
    receivers.push_back(receivertoken_);
  }
  return receivers;
}

std::vector<QString> RecordingJobSource::GetActiveProfileTokens(const QSharedPointer<client::RecordingTrack>& track) const
{
  std::vector<QString> profiletokens;
  for (const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack : tracks_)
  {
    if (recordingjobsourcetrack->GetTrack() == track)
    {
      const boost::optional<QString> profiletoken = recordingjobsourcetrack->GetActiveProfileToken();
      if (profiletoken.is_initialized())
      {
        profiletokens.push_back(*profiletoken);

      }
    }
  }
  return profiletokens;
}

std::vector<ROTATION> RecordingJobSource::GetActiveRotations(const QSharedPointer<client::RecordingTrack>& track) const
{
  std::vector<ROTATION> rotations;
  for (const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack : tracks_)
  {
    if (recordingjobsourcetrack->GetTrack() == track)
    {
      const boost::optional<ROTATION> rotation = recordingjobsourcetrack->GetRotation();
      if (rotation.is_initialized())
      {
        rotations.push_back(*rotation);

      }
    }
  }
  return rotations;
}

}
