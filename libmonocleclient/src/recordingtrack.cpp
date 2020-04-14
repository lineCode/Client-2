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

RecordingTrack::RecordingTrack(const uint32_t id, const QString& token, const monocle::TrackType tracktype, const QString& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector< std::pair<uint64_t, uint64_t> >& indices, const std::vector<monocle::CODECINDEX>& codecindices, const std::vector<DATASNAPSHOT>& totaltrackdata) :
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
  codecindices_(codecindices),
  totaltrackdata_(24),
  datarate_(0)
{
  for (const DATASNAPSHOT& totaltrackdata : totaltrackdata)
  {
    totaltrackdata_.push_back(totaltrackdata);

  }

  if (totaltrackdata_.size() >= 2)
  {
    const DATASNAPSHOT first = totaltrackdata[totaltrackdata_.size() - 1];
    const DATASNAPSHOT second = totaltrackdata[totaltrackdata_.size() - 2];
    datarate_ = static_cast<uint64_t>(static_cast<double>(first.totaldata_ - second.totaldata_) / (static_cast<double>(first.time_ - second.time_) / 1000.0));
  }
}

RecordingTrack::~RecordingTrack()
{

}

void RecordingTrack::ChangeTrack(const QString& token, const monocle::TrackType tracktype, const QString& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<monocle::CODECINDEX>& codecindices, const std::vector<DATASNAPSHOT>& totaltrackdata)
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
  for (const DATASNAPSHOT& totaltrackdata : totaltrackdata)
  {
    totaltrackdata_.push_back(totaltrackdata);

  }
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

void RecordingTrack::AddTotalTrackData(const DATASNAPSHOT& totaltrackdata)
{
  if (totaltrackdata_.size() && (totaltrackdata.time_ > totaltrackdata_.back().time_) && (totaltrackdata.totaldata_ > totaltrackdata_.back().totaldata_))
  {
    const double den = static_cast<double>(totaltrackdata.time_ - totaltrackdata_.back().time_) / 1000.0;
    if (den == 0.0)
    {
      datarate_ = 0;

    }
    else
    {
      datarate_ = static_cast<uint64_t>((static_cast<double>(totaltrackdata.totaldata_ - totaltrackdata_.back().totaldata_)) / den);

    }
  }
  else
  {
    datarate_ = 0;

  }
  totaltrackdata_.push_back(totaltrackdata);
  emit DataRate(datarate_);
}

void RecordingTrack::SetTotalTrackData(const std::vector<DATASNAPSHOT>& totaltrackdata)
{
  totaltrackdata_.clear();
  for (const DATASNAPSHOT& t : totaltrackdata)
  {
    totaltrackdata_.push_back(t);

  }
  
  if (totaltrackdata_.size() >= 2)
  {
    const DATASNAPSHOT first = totaltrackdata[totaltrackdata_.size() - 1];
    const DATASNAPSHOT second = totaltrackdata[totaltrackdata_.size() - 2];
    datarate_ = static_cast<uint64_t>(static_cast<double>(first.totaldata_ - second.totaldata_) / (static_cast<double>(first.time_ - second.time_) / 1000.0));
    emit DataRate(datarate_);
  }
}

void RecordingTrack::AddCodec(const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp)
{
  codecindices_.push_back(monocle::CODECINDEX(id, codec, parameters, timestamp));

}

void RecordingTrack::RemoveCodec(const uint64_t id)
{
  codecindices_.erase(std::remove_if(codecindices_.begin(), codecindices_.end(), [id](const monocle::CODECINDEX& codecindex) { return (codecindex.id_ == id); }), codecindices_.end());

}

bool RecordingTrack::HasFile(const uint64_t file) const
{
  return (std::find(files_.cbegin(), files_.cend(), file) != files_.cend());
}

std::vector<monocle::CODECINDEX> RecordingTrack::GetCodecIndices(const monocle::Codec codec) const
{
  std::vector<monocle::CODECINDEX> codecindices;
  for (const monocle::CODECINDEX& codecindex : codecindices_)
  {
    if (codecindex.codec_ == codec)
    {
      codecindices.push_back(codecindex);

    }
  }
  return codecindices;
}

}
