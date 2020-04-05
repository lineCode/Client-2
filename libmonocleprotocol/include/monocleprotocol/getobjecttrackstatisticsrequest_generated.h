// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_GETOBJECTTRACKSTATISTICSREQUEST_MONOCLE_H_
#define FLATBUFFERS_GENERATED_GETOBJECTTRACKSTATISTICSREQUEST_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

struct GetObjectTrackStatisticsRequest;

struct GetObjectTrackStatisticsRequest FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RECORDINGTOKEN = 4,
    VT_TRACKID = 6,
    VT_STARTTIME = 8,
    VT_ENDTIME = 10,
    VT_INTERVAL = 12
  };
  uint64_t recordingtoken() const {
    return GetField<uint64_t>(VT_RECORDINGTOKEN, 0);
  }
  uint32_t trackid() const {
    return GetField<uint32_t>(VT_TRACKID, 0);
  }
  uint64_t starttime() const {
    return GetField<uint64_t>(VT_STARTTIME, 0);
  }
  uint64_t endtime() const {
    return GetField<uint64_t>(VT_ENDTIME, 0);
  }
  uint64_t interval() const {
    return GetField<uint64_t>(VT_INTERVAL, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_RECORDINGTOKEN) &&
           VerifyField<uint32_t>(verifier, VT_TRACKID) &&
           VerifyField<uint64_t>(verifier, VT_STARTTIME) &&
           VerifyField<uint64_t>(verifier, VT_ENDTIME) &&
           VerifyField<uint64_t>(verifier, VT_INTERVAL) &&
           verifier.EndTable();
  }
};

struct GetObjectTrackStatisticsRequestBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_recordingtoken(uint64_t recordingtoken) {
    fbb_.AddElement<uint64_t>(GetObjectTrackStatisticsRequest::VT_RECORDINGTOKEN, recordingtoken, 0);
  }
  void add_trackid(uint32_t trackid) {
    fbb_.AddElement<uint32_t>(GetObjectTrackStatisticsRequest::VT_TRACKID, trackid, 0);
  }
  void add_starttime(uint64_t starttime) {
    fbb_.AddElement<uint64_t>(GetObjectTrackStatisticsRequest::VT_STARTTIME, starttime, 0);
  }
  void add_endtime(uint64_t endtime) {
    fbb_.AddElement<uint64_t>(GetObjectTrackStatisticsRequest::VT_ENDTIME, endtime, 0);
  }
  void add_interval(uint64_t interval) {
    fbb_.AddElement<uint64_t>(GetObjectTrackStatisticsRequest::VT_INTERVAL, interval, 0);
  }
  explicit GetObjectTrackStatisticsRequestBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  GetObjectTrackStatisticsRequestBuilder &operator=(const GetObjectTrackStatisticsRequestBuilder &);
  flatbuffers::Offset<GetObjectTrackStatisticsRequest> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<GetObjectTrackStatisticsRequest>(end);
    return o;
  }
};

inline flatbuffers::Offset<GetObjectTrackStatisticsRequest> CreateGetObjectTrackStatisticsRequest(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t recordingtoken = 0,
    uint32_t trackid = 0,
    uint64_t starttime = 0,
    uint64_t endtime = 0,
    uint64_t interval = 0) {
  GetObjectTrackStatisticsRequestBuilder builder_(_fbb);
  builder_.add_interval(interval);
  builder_.add_endtime(endtime);
  builder_.add_starttime(starttime);
  builder_.add_recordingtoken(recordingtoken);
  builder_.add_trackid(trackid);
  return builder_.Finish();
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_GETOBJECTTRACKSTATISTICSREQUEST_MONOCLE_H_
