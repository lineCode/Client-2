// recordingtrack.cpp
//

///// Includes /////

#include "monocleclient/recordingtrack.h"

#include <monocleprotocol/monocleprotocol.hpp>

#include "monocleclient/file.h"

///// Namespaces /////

namespace client
{

///// Methods /////

RecordingTrack::RecordingTrack(const uint32_t id, const QString& token, const monocle::TrackType tracktype, const QString& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector< std::pair<uint64_t, uint64_t> >& indices, const std::vector<monocle::CODECINDEX>& codecindices) :
  id_(id),
  token_(token),
  tracktype_(tracktype),
  description_(description),
  fixedfiles_(fixedfiles),
  digitalsigning_(digitalsigning),
  encrypt_(encrypt),
  flushfrequency_(flushfrequency),
  files_(files),
  indices_(indices),
  codecindices_(codecindices)
{

}

RecordingTrack::~RecordingTrack()
{

}

void RecordingTrack::ChangeTrack(const QString& token, const monocle::TrackType tracktype, const QString& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<monocle::CODECINDEX>& codecindices)
{
  token_ = token;
  tracktype_ = tracktype;
  description_ = description;
  fixedfiles_ = fixedfiles;
  digitalsigning_ = digitalsigning;
  encrypt_ = encrypt;
  flushfrequency_ = flushfrequency;
  files_ = files;
  codecindices_ = codecindices;
}

void RecordingTrack::SetData(const std::vector<monocle::INDEX>& indices)
{
  indices_.clear();
  indices_.reserve(indices.size());
  for (const monocle::INDEX& index : indices)
  {
    indices_.push_back(std::make_pair(index.starttime(), index.endtime()));

  }
}

void RecordingTrack::DeleteData(const monocle::RecordingJobState state, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end)
{
  if (!start.is_initialized() && !end.is_initialized())
  {
    indices_.clear();
    return;
  }

  const uint64_t s = start.is_initialized() ? *start : 0;
  const uint64_t e = end.is_initialized() ? *end : std::numeric_limits<uint64_t>::max();
  indices_.erase(std::remove_if(indices_.begin(), indices_.end(), [s, e](const std::pair<uint64_t, uint64_t>& index) { return ((index.first >= s) && (index.second <= e)); }), indices_.end()); // Remove any indices that are completely out of scope

  // If we are cutting an index from both sides, we need to split it. This can only happen if both start and end are both not chopping indefinitely
  if (start.is_initialized() && end.is_initialized())
  {
    std::vector< std::pair<uint64_t, uint64_t> > splitindices;
    for (std::vector< std::pair<uint64_t, uint64_t> >::iterator index = indices_.begin(); index != indices_.end();)
    {
      if (index->first < *start && index->second > *end)
      {
        splitindices.push_back(std::make_pair(index->first, *start));
        splitindices.push_back(std::make_pair(*end, index->second));
        index = indices_.erase(index);
      }
      else
      {
        ++index;

      }
    }
    indices_.insert(indices_.end(), splitindices.cbegin(), splitindices.cend());
  }

  // If we are cutting it from one side
  for (std::pair<uint64_t, uint64_t>& index : indices_)
  {
    if ((s <= index.first) && (index.first < e) && (index.second >= e))
    {
      index.first = e;

    }
    else if ((e > index.second) && (index.first <= s) && (index.second > s))
    {
      index.second = s;

    }
  }

  if (end.is_initialized() && indices_.empty() && (state == monocle::RecordingJobState::Active)) // If there is an attempt to remove all the blocks, yet we are active, create one at the end
  {
    indices_.push_back(std::make_pair(*end, *end));

  }
}

void RecordingTrack::SetState(const uint64_t time, const monocle::RecordingJobState state, const monocle::RecordingJobState prevstate)
{
  if ((prevstate != monocle::RecordingJobState::Active) && (state == monocle::RecordingJobState::Active))
  {
    if (indices_.empty())
    {
      indices_.push_back(std::make_pair(time, time));

    }
    else
    {
      if ((time > indices_.back().second) && (time - indices_.back().second) < 5000)
      {
        indices_.back().second = time;

      }
      else
      {
        indices_.push_back(std::make_pair(time, time));

      }
    }
  }
  else if ((prevstate == monocle::RecordingJobState::Active) && (state != monocle::RecordingJobState::Active))
  {
    if (indices_.size()) // Terminate oldest index
    {
      indices_.back().second = time;

    }
  }
}

bool RecordingTrack::HasFile(const uint64_t file) const
{
  return (std::find(files_.cbegin(), files_.cend(), file) != files_.cend());
}

}
