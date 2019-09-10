// recordingblock.cpp
//

///// Includes /////

#include "monocleclient/recordingblock.h"

#include <array>

///// Namespaces /////

namespace client
{

///// Methods /////

RecordingBlock::RecordingBlock(const bool metadata, const uint64_t starttime, const uint64_t endtime) :
  metadata_(metadata),
  starttime_(starttime),
  endtime_(endtime)
{

}

RecordingBlock::~RecordingBlock()
{

}

void RecordingBlock::Update(const float top, const float bottom, const float minwidth, const uint64_t starttime, const uint64_t endtime, std::vector<float>& recordingblockverticesdata)
{
  const float start = ((static_cast<float>(static_cast<int64_t>(starttime_) - static_cast<int64_t>(starttime)) / static_cast<float>(static_cast<int64_t>(endtime) - static_cast<int64_t>(starttime))) * 2.0f) - 1.0f;
  float end = ((static_cast<float>(static_cast<int64_t>(endtime_) - static_cast<int64_t>(starttime)) / static_cast<float>(static_cast<int64_t>(endtime) - static_cast<int64_t>(starttime))) * 2.0f) - 1.0f;
  if ((end - start) < minwidth)
  {
    end = start + minwidth;

  }
  recordingblockverticesdata.push_back(end);
  recordingblockverticesdata.push_back(bottom);
  recordingblockverticesdata.push_back(end);
  recordingblockverticesdata.push_back(top);
  recordingblockverticesdata.push_back(start);
  recordingblockverticesdata.push_back(bottom);

  recordingblockverticesdata.push_back(end);
  recordingblockverticesdata.push_back(top);
  recordingblockverticesdata.push_back(start);
  recordingblockverticesdata.push_back(top);
  recordingblockverticesdata.push_back(start);
  recordingblockverticesdata.push_back(bottom);
}

}
