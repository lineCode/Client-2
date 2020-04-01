// recordingblocks.cpp
//

///// Includes /////

#include <boost/asio.hpp>

#include "monocleclient/recordingblocks.h"

#include "monocleclient/device.h"
#include "monocleclient/media.h"
#include "monocleclient/mediaview.h"
#include "monocleclient/playbackwidget.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingblock.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"
#include "monocleclient/videoview.h"

///// Namespaces /////

namespace client
{

///// Methods /////

RecordingBlocks::RecordingBlocks(PlaybackWidget* playbackwidget, const QSharedPointer<View>& view) :
  QObject(nullptr),
  playbackwidget_(playbackwidget),
  view_(view),
  top_(0.0f),
  bottom_(0.0f),
  metadatatop_(0.0f),
  metadatabottom_(0.0f),
  minwidth_(0.0f)
{
  if (view_->GetViewType() == VIEWTYPE_MONOCLE)
  {
    QSharedPointer<VideoView> videoview = qSharedPointerCast<VideoView>(view_);
    connect(videoview->GetRecording().data(), &Recording::TrackAdded, this, &RecordingBlocks::TrackAdded);
    connect(videoview->GetRecording().data(), &Recording::TrackRemoved, this, &RecordingBlocks::TrackRemoved);
    connect(videoview->GetRecording().data(), &Recording::JobSourceTrackStateChanged, this, &RecordingBlocks::JobSourceTrackStateChanged);
    connect(videoview->GetRecording().data(), &Recording::TrackSetData, this, &RecordingBlocks::TrackSetData);
    connect(videoview->GetRecording().data(), &Recording::TrackDeleteData, this, &RecordingBlocks::TrackDeleteData);
  }

  Init();
}

RecordingBlocks::~RecordingBlocks()
{
  Destroy();

}

void RecordingBlocks::Init()
{
  Destroy();

  // Recording blocks
  if (view_->GetViewType() == VIEWTYPE_MEDIA)
  {
    QSharedPointer<MediaView> mediaview = qSharedPointerCast<MediaView>(view_);
    std::vector< std::unique_ptr<RecordingBlock> > recordingblocks;
    for (const std::pair<uint64_t, uint64_t>& index : mediaview->GetVideoTrack().GetIndices())
    {
      std::unique_ptr<RecordingBlock> recordingblock = std::make_unique<RecordingBlock>(false, index.first, index.second);
      recordingblocks.emplace_back(std::move(recordingblock));
    }
    recordingtracks_.emplace(mediaview->GetVideoTrack().index_, std::move(recordingblocks));
    recordingblocks.clear();
    
    for (const file::TRACK& metadatatrack : mediaview->GetMetadataTracks())
    {
      for (const std::pair<uint64_t, uint64_t>& index : metadatatrack.GetIndices())
      {
        std::unique_ptr<RecordingBlock> recordingblock = std::make_unique<RecordingBlock>(true, index.first, index.second);
        recordingblocks.emplace_back(std::move(recordingblock));
      }
      recordingtracks_.emplace(metadatatrack.index_, std::move(recordingblocks));
      recordingblocks.clear();
    }
  }
  else if (view_->GetViewType() == VIEWTYPE_MONOCLE)
  {
    const QSharedPointer<VideoView> videoview = qSharedPointerCast<VideoView>(view_);
    recordingtracks_.emplace(videoview->GetTrack()->GetId(), InitRecordingBlocks(videoview->GetTrack()));
    for (const QSharedPointer<RecordingTrack>& metadatatrack : videoview->GetRecording()->GetMetadataTracks())
    {
      recordingtracks_.emplace(metadatatrack->GetId(), InitRecordingBlocks(metadatatrack));

    }
    for (const QSharedPointer<RecordingTrack>& objectdetectortrack : videoview->GetRecording()->GetObjectDetectorTracks())
    {
      recordingtracks_.emplace(objectdetectortrack->GetId(), InitRecordingBlocks(objectdetectortrack));

    }
  }
  endtime_ = std::chrono::steady_clock::now();

  recordingblockvertices_.create();
  recordingblockvertices_.setUsagePattern(QOpenGLBuffer::DynamicDraw);

  metadatarecordingblockvertices_.create();
  metadatarecordingblockvertices_.setUsagePattern(QOpenGLBuffer::DynamicDraw);

  // Play marker
  playmarkervertices_.create();
  playmarkervertices_.setUsagePattern(QOpenGLBuffer::DynamicDraw);
}

void RecordingBlocks::Destroy()
{
  playmarkervertices_.destroy();
  recordingblockverticesdata_.clear();
  recordingblockvertices_.destroy();
  metadatarecordingblockverticesdata_.clear();
  metadatarecordingblockvertices_.destroy();
  recordingtracks_.clear();
}

void RecordingBlocks::Update(const float top, const float bottom, const float metadatatop, const float metadatabottom, const float minwidth, const uint64_t starttime, const uint64_t endtime, const uint64_t globalendtime)
{
  top_ = top;
  bottom_ = bottom;
  metadatatop_ = metadatatop;
  metadatabottom_ = metadatabottom;
  minwidth_ = minwidth;

  // If a track is active, update the final recording block with the new time
  if (view_->GetViewType() == VIEWTYPE_MONOCLE)
  {
    QSharedPointer<VideoView> videoview = qSharedPointerCast<VideoView>(view_);
    for (const std::pair< const uint64_t, std::vector< std::unique_ptr<RecordingBlock> > >& recordingtrack : recordingtracks_)
    {
      QSharedPointer<client::RecordingTrack> track = videoview->GetRecording()->GetTrack(recordingtrack.first);
      if (track == nullptr)
      {
        // Shouldn't ever happen... Or if it does just ignore it
        continue;
      }

      if (recordingtrack.second.empty() || (videoview->GetRecording()->GetState(track) != monocle::RecordingJobState::Active))
      {

        continue;
      }

      recordingtrack.second.back()->SetEndTime(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + videoview->GetDevice()->GetTimeOffset());
    }
  }

  recordingblockverticesdata_.clear();
  metadatarecordingblockverticesdata_.clear();
  for (std::pair< const uint64_t, std::vector< std::unique_ptr<RecordingBlock> > >& recordingtrack : recordingtracks_)
  {
    for (std::unique_ptr<RecordingBlock>& recordingblock : recordingtrack.second)
    {
      if (recordingblock->IsMetadata())
      {
        recordingblock->Update(metadatatop, metadatabottom, minwidth, starttime, endtime, metadatarecordingblockverticesdata_);
        
      }
      else
      {
        recordingblock->Update(top, bottom, minwidth, starttime, endtime, recordingblockverticesdata_);

      }
    }
  }
  recordingblockvertices_.bind();
  recordingblockvertices_.allocate(recordingblockverticesdata_.data(), static_cast<int>(recordingblockverticesdata_.size() * sizeof(float)));
  recordingblockvertices_.release();

  metadatarecordingblockvertices_.bind();
  metadatarecordingblockvertices_.allocate(metadatarecordingblockverticesdata_.data(), static_cast<int>(metadatarecordingblockverticesdata_.size() * sizeof(float)));
  metadatarecordingblockvertices_.release();

  // Marker
  if (view_->GetFrameTime() != std::chrono::steady_clock::time_point())
  {
    const uint64_t time = GetPlayMarkerTime();
    const float position = std::max(-1.0, std::min(1.0, ((static_cast<double>(time) - static_cast<double>(starttime)) / (static_cast<double>(endtime) - static_cast<double>(starttime)) * 2.0) - 1.0));
    const std::array<float, 4> vertices =
    {
      position, bottom,
      position, top
    };
    playmarkervertices_.bind();
    playmarkervertices_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(float)));
    playmarkervertices_.release();
  }
}

uint64_t RecordingBlocks::GetPlayMarkerTime() const
{
  uint64_t time = view_->GetPlayMarkerTime() - view_->GetTimeOffset();
  if (!view_->IsPaused())
  {
    time += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - view_->GetFrameTime()).count();

  }
  return time;
}

boost::optional<uint64_t> RecordingBlocks::GetStartTime() const
{
  boost::optional<uint64_t> time;
  for (const std::pair< const uint64_t, std::vector< std::unique_ptr<RecordingBlock> > >& recordingtrack : recordingtracks_)
  {
    if (recordingtrack.second.empty())
    {

      continue;
    }

    for (const std::unique_ptr<RecordingBlock>& recordingblock : recordingtrack.second)
    {
      if (recordingblock->IsMetadata())
      {

        continue;
      }

      if (time.is_initialized())
      {
        if (recordingblock->GetStartTime() < *time)
        {
          time = recordingblock->GetStartTime();

        }
      }
      else
      {
        time = recordingblock->GetStartTime();

      }
    }
  }
  return time;
}

boost::optional<uint64_t> RecordingBlocks::GetEndTime() const
{
  boost::optional<uint64_t> time;
  for (const std::pair< const uint64_t, std::vector< std::unique_ptr<RecordingBlock> > >& recordingtrack : recordingtracks_)
  {
    if (recordingtrack.second.empty())
    {

      continue;
    }

    for (const std::unique_ptr<RecordingBlock>& recordingblock : recordingtrack.second)
    {
      if (recordingblock->IsMetadata())
      {

        continue;
      }

      if (time.is_initialized())
      {
        if (recordingblock->GetStartTime() > *time)
        {
          time = recordingblock->GetEndTime();

        }
      }
      else
      {
        time = recordingblock->GetEndTime();

      }
    }
  }
  return time;
}

void RecordingBlocks::TrackAdded(const QSharedPointer<client::RecordingTrack>& track)
{
  playbackwidget_->makeCurrent();
  recordingtracks_.emplace(track->GetId(), InitRecordingBlocks(track));
  playbackwidget_->doneCurrent();
}

void RecordingBlocks::TrackRemoved(const uint32_t id)
{
  for (auto recordingtrack = recordingtracks_.begin(); recordingtrack != recordingtracks_.end();)
  {
    if (recordingtrack->first == id)
    {
      recordingtracks_.erase(recordingtrack++);

    }
    else
    {
      ++recordingtrack;
    }
  }
}

void RecordingBlocks::JobSourceTrackStateChanged(const QSharedPointer<client::RecordingJob>& job, const QSharedPointer<client::RecordingJobSource>& source, const QSharedPointer<client::RecordingJobSourceTrack>& track, uint64_t time, const monocle::RecordingJobState state, const QString& error, const monocle::RecordingJobState prevstate)
{
  time -= view_->GetTimeOffset();

  std::map< const uint64_t, std::vector< std::unique_ptr<RecordingBlock> > >::iterator recordingtrack = recordingtracks_.find(track->GetTrack()->GetId());
  if (recordingtrack == recordingtracks_.end())
  {

    return;
  }

  playbackwidget_->makeCurrent();
  if (state == monocle::RecordingJobState::Active) // Create or extend the RecordingBlock
  {
    const bool metadata = (track->GetTrack()->GetTrackType() == monocle::TrackType::Metadata) || (track->GetTrack()->GetTrackType() == monocle::TrackType::ObjectDetector);
    if (recordingtrack->second.empty())
    {
      std::unique_ptr<RecordingBlock> rb = std::make_unique<RecordingBlock>(metadata, time, playbackwidget_->GetGlobalEndTime());
      recordingtrack->second.emplace_back(std::move(rb));
    }
    else
    {
      if ((time > recordingtrack->second.back()->GetEndTime()) && ((time - recordingtrack->second.back()->GetEndTime()) < 5000)) // Extend the oldest RecordingBlock or create a new one
      {
        recordingtrack->second.back()->SetEndTime(time);

      }
      else
      {
        std::unique_ptr<RecordingBlock> rb = std::make_unique<RecordingBlock>(metadata, time, playbackwidget_->GetGlobalEndTime());
        recordingtrack->second.emplace_back(std::move(rb));
      }
    }
    Update(top_, bottom_, metadatatop_, metadatabottom_, minwidth_, playbackwidget_->GetGlobalStartTime(), playbackwidget_->GetGlobalEndTime(), playbackwidget_->GetGlobalEndTime());
    playbackwidget_->update();
  }
  else if ((prevstate == monocle::RecordingJobState::Active) && ((state == monocle::RecordingJobState::Idle) || (state == monocle::RecordingJobState::Error))) // Close the oldest RecordingBlock
  {
    if (!recordingtrack->second.empty())
    {
      recordingtrack->second.back()->SetEndTime(time);
      Update(top_, bottom_, metadatatop_, metadatabottom_, minwidth_, playbackwidget_->GetGlobalStartTime(), playbackwidget_->GetGlobalEndTime(), playbackwidget_->GetGlobalEndTime());
      playbackwidget_->update();
    }
  }
  playbackwidget_->doneCurrent();
}

void RecordingBlocks::TrackSetData(const QSharedPointer<client::RecordingTrack>& track, const std::vector<monocle::INDEX>& indices)
{
  std::map< const uint64_t, std::vector< std::unique_ptr<RecordingBlock> > >::iterator recordingtrack = recordingtracks_.find(track->GetId());
  if (recordingtrack == recordingtracks_.end())
  {

    return;
  }

  playbackwidget_->makeCurrent();
  recordingtrack->second = InitRecordingBlocks(track);
  playbackwidget_->UpdateRecordingsBlocks();
  playbackwidget_->doneCurrent();
  playbackwidget_->update();
}

void RecordingBlocks::TrackDeleteData(const QSharedPointer<client::RecordingTrack>& track, const monocle::RecordingJobState state, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end)
{
  std::map< const uint64_t, std::vector< std::unique_ptr<RecordingBlock> > >::iterator recordingtrack = recordingtracks_.find(track->GetId());
  if (recordingtrack == recordingtracks_.end())
  {

    return;
  }

  playbackwidget_->makeCurrent();
  if (!start.is_initialized() && !end.is_initialized()) // Delete the whole track
  {
    recordingtracks_.erase(recordingtrack);
    return;
  }

  const int64_t offset = qSharedPointerCast<VideoView>(view_)->GetDevice()->GetTimeOffset();
  const uint64_t s = start.is_initialized() ? (*start + offset) : 0;
  const uint64_t e = end.is_initialized() ? (*end + offset) : std::numeric_limits<uint64_t>::max();
  recordingtrack->second.erase(std::remove_if(recordingtrack->second.begin(), recordingtrack->second.end(), [s, e](const std::unique_ptr<RecordingBlock>& index) { return ((index->GetStartTime() >= s) && (index->GetEndTime() <= e)); }), recordingtrack->second.end()); // Remove any indices that are completely out of scope

  // If we are cutting an index from both sides, we need to split it. This can only happen if both start and end are both not chopping indefinitely
  const bool metadata = (track->GetTrackType() == monocle::TrackType::Metadata) || (track->GetTrackType() == monocle::TrackType::ObjectDetector);
  if (start.is_initialized() && end.is_initialized())
  {
    std::vector< std::pair<uint64_t, uint64_t> > splitindices;
    for (std::vector< std::unique_ptr<RecordingBlock> >::iterator recordingblock = recordingtrack->second.begin(); recordingblock != recordingtrack->second.end();)
    {
      if ((*recordingblock)->GetStartTime() < *start && (*recordingblock)->GetEndTime() > *end)
      {
        splitindices.push_back(std::make_pair((*recordingblock)->GetStartTime(), *start));
        splitindices.push_back(std::make_pair(*end, (*recordingblock)->GetEndTime()));
        recordingblock = recordingtrack->second.erase(recordingblock);
      }
      else
      {
        ++recordingblock;

      }
    }

    std::vector< std::unique_ptr<RecordingBlock> > recordingblocks;
    for (const std::pair<uint64_t, uint64_t>& splitindex : splitindices)
    {
      std::unique_ptr<RecordingBlock> recordingblock = std::make_unique<RecordingBlock>(metadata, splitindex.first - offset, splitindex.second - offset);
      recordingblocks.emplace_back(std::move(recordingblock));
    }
    recordingtracks_.emplace(track->GetId(), std::move(recordingblocks));
  }

  // If we are cutting it from one side
  for (std::unique_ptr<RecordingBlock>& recordingblock : recordingtrack->second)
  {
    if ((s <= recordingblock->GetStartTime()) && (recordingblock->GetStartTime() < e) && (recordingblock->GetEndTime() >= e))
    {
      recordingblock->SetStartTime(e);

    }
    else if ((e > recordingblock->GetEndTime()) && (recordingblock->GetStartTime() <= s) && (recordingblock->GetEndTime() > s))
    {
      recordingblock->SetEndTime(s);

    }
  }

  if (end.is_initialized() && recordingtrack->second.empty() && (state == monocle::RecordingJobState::Active)) // If there is an attempt to remove all the blocks, yet we are active, create one at the end
  {
    std::unique_ptr<RecordingBlock> recordingblock = std::make_unique<RecordingBlock>(metadata, *end + offset, *end + offset);
    recordingtrack->second.emplace_back(std::move(recordingblock));
  }

  playbackwidget_->UpdateRecordingsBlocks();
  playbackwidget_->doneCurrent();
}

std::vector< std::unique_ptr<RecordingBlock> > RecordingBlocks::InitRecordingBlocks(const QSharedPointer<client::RecordingTrack> track) const
{
  const bool metadata = (track->GetTrackType() == monocle::TrackType::Metadata) || (track->GetTrackType() == monocle::TrackType::ObjectDetector);
  const int64_t offset = qSharedPointerCast<VideoView>(view_)->GetDevice()->GetTimeOffset();
  std::vector< std::unique_ptr<RecordingBlock> > recordingblocks;
  for (const std::pair<uint64_t, uint64_t>& index : track->GetIndices())
  {
    std::unique_ptr<RecordingBlock> recordingblock = std::make_unique<RecordingBlock>(metadata, index.first - offset, index.second - offset);
    recordingblocks.emplace_back(std::move(recordingblock));
  }
  return recordingblocks;
}

}
