// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_CONTROLSTREAMPLAYREQUEST_MONOCLE_H_
#define FLATBUFFERS_GENERATED_CONTROLSTREAMPLAYREQUEST_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

struct ControlStreamPlayRequest;
struct ControlStreamPlayRequestBuilder;

struct ControlStreamPlayRequest FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ControlStreamPlayRequestBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TOKEN = 4,
    VT_PLAYREQUESTINDEX = 6,
    VT_FETCHMARKER = 8,
    VT_RATECONTROL = 10,
    VT_FORWARDS = 12,
    VT_STARTTIME = 14,
    VT_ENDTIME = 16,
    VT_NUMFRAMES = 18,
    VT_IFRAMES = 20
  };
  uint64_t token() const {
    return GetField<uint64_t>(VT_TOKEN, 0);
  }
  uint64_t playrequestindex() const {
    return GetField<uint64_t>(VT_PLAYREQUESTINDEX, 0);
  }
  bool fetchmarker() const {
    return GetField<uint8_t>(VT_FETCHMARKER, 0) != 0;
  }
  bool ratecontrol() const {
    return GetField<uint8_t>(VT_RATECONTROL, 0) != 0;
  }
  bool forwards() const {
    return GetField<uint8_t>(VT_FORWARDS, 0) != 0;
  }
  uint64_t starttime() const {
    return GetField<uint64_t>(VT_STARTTIME, 0);
  }
  uint64_t endtime() const {
    return GetField<uint64_t>(VT_ENDTIME, 0);
  }
  uint64_t numframes() const {
    return GetField<uint64_t>(VT_NUMFRAMES, 0);
  }
  bool iframes() const {
    return GetField<uint8_t>(VT_IFRAMES, 0) != 0;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_TOKEN) &&
           VerifyField<uint64_t>(verifier, VT_PLAYREQUESTINDEX) &&
           VerifyField<uint8_t>(verifier, VT_FETCHMARKER) &&
           VerifyField<uint8_t>(verifier, VT_RATECONTROL) &&
           VerifyField<uint8_t>(verifier, VT_FORWARDS) &&
           VerifyField<uint64_t>(verifier, VT_STARTTIME) &&
           VerifyField<uint64_t>(verifier, VT_ENDTIME) &&
           VerifyField<uint64_t>(verifier, VT_NUMFRAMES) &&
           VerifyField<uint8_t>(verifier, VT_IFRAMES) &&
           verifier.EndTable();
  }
};

struct ControlStreamPlayRequestBuilder {
  typedef ControlStreamPlayRequest Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_token(uint64_t token) {
    fbb_.AddElement<uint64_t>(ControlStreamPlayRequest::VT_TOKEN, token, 0);
  }
  void add_playrequestindex(uint64_t playrequestindex) {
    fbb_.AddElement<uint64_t>(ControlStreamPlayRequest::VT_PLAYREQUESTINDEX, playrequestindex, 0);
  }
  void add_fetchmarker(bool fetchmarker) {
    fbb_.AddElement<uint8_t>(ControlStreamPlayRequest::VT_FETCHMARKER, static_cast<uint8_t>(fetchmarker), 0);
  }
  void add_ratecontrol(bool ratecontrol) {
    fbb_.AddElement<uint8_t>(ControlStreamPlayRequest::VT_RATECONTROL, static_cast<uint8_t>(ratecontrol), 0);
  }
  void add_forwards(bool forwards) {
    fbb_.AddElement<uint8_t>(ControlStreamPlayRequest::VT_FORWARDS, static_cast<uint8_t>(forwards), 0);
  }
  void add_starttime(uint64_t starttime) {
    fbb_.AddElement<uint64_t>(ControlStreamPlayRequest::VT_STARTTIME, starttime, 0);
  }
  void add_endtime(uint64_t endtime) {
    fbb_.AddElement<uint64_t>(ControlStreamPlayRequest::VT_ENDTIME, endtime, 0);
  }
  void add_numframes(uint64_t numframes) {
    fbb_.AddElement<uint64_t>(ControlStreamPlayRequest::VT_NUMFRAMES, numframes, 0);
  }
  void add_iframes(bool iframes) {
    fbb_.AddElement<uint8_t>(ControlStreamPlayRequest::VT_IFRAMES, static_cast<uint8_t>(iframes), 0);
  }
  explicit ControlStreamPlayRequestBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ControlStreamPlayRequest> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ControlStreamPlayRequest>(end);
    return o;
  }
};

inline flatbuffers::Offset<ControlStreamPlayRequest> CreateControlStreamPlayRequest(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t token = 0,
    uint64_t playrequestindex = 0,
    bool fetchmarker = false,
    bool ratecontrol = false,
    bool forwards = false,
    uint64_t starttime = 0,
    uint64_t endtime = 0,
    uint64_t numframes = 0,
    bool iframes = false) {
  ControlStreamPlayRequestBuilder builder_(_fbb);
  builder_.add_numframes(numframes);
  builder_.add_endtime(endtime);
  builder_.add_starttime(starttime);
  builder_.add_playrequestindex(playrequestindex);
  builder_.add_token(token);
  builder_.add_iframes(iframes);
  builder_.add_forwards(forwards);
  builder_.add_ratecontrol(ratecontrol);
  builder_.add_fetchmarker(fetchmarker);
  return builder_.Finish();
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_CONTROLSTREAMPLAYREQUEST_MONOCLE_H_
