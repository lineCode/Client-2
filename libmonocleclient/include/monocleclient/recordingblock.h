// recordingblock.h
//

#ifndef IDG4K6923408SADDSAASDADSGGGCRYVBT4
#define IDG4K6923408SADDSAASDADSGGGCRYVBT4

///// Includes /////

#include <QSharedPointer>
#include <stdint.h>

///// Declarations /////

namespace monocle { enum class TrackType : int8_t; }

///// Namespaces /////

namespace client
{

///// Classes /////

class RecordingBlock
{
 public:

  RecordingBlock(const bool metadata, const uint64_t starttime, const uint64_t endtime);
  virtual ~RecordingBlock();

  void Update(const float top, const float bottom, const float minwidth, const uint64_t starttime, const uint64_t endtime, std::vector<float>& recordingblockverticesdata);

  inline bool IsMetadata() const { return metadata_; }
  inline void SetStartTime(const uint64_t starttime) { starttime_ = starttime; }
  inline uint64_t GetStartTime() const { return starttime_; }
  inline void SetEndTime(const uint64_t endtime) { endtime_ = endtime; }
  inline uint64_t GetEndTime() const { return endtime_; }

 private:

  bool metadata_;
  uint64_t starttime_;
  uint64_t endtime_;

};

}

#endif
