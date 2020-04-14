// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_RECORDINGTRACKLOGMESSAGE_MONOCLE_H_
#define FLATBUFFERS_GENERATED_RECORDINGTRACKLOGMESSAGE_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

#include "logmessage_generated.h"
#include "severity_generated.h"

namespace monocle {

struct RecordingTrackLogMessage;
struct RecordingTrackLogMessageBuilder;

struct RecordingTrackLogMessage FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef RecordingTrackLogMessageBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RECORDINGTOKEN = 4,
    VT_ID = 6,
    VT_MESSAGE = 8
  };
  uint64_t recordingtoken() const {
    return GetField<uint64_t>(VT_RECORDINGTOKEN, 0);
  }
  uint32_t id() const {
    return GetField<uint32_t>(VT_ID, 0);
  }
  const monocle::LogMessage *message() const {
    return GetPointer<const monocle::LogMessage *>(VT_MESSAGE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_RECORDINGTOKEN) &&
           VerifyField<uint32_t>(verifier, VT_ID) &&
           VerifyOffset(verifier, VT_MESSAGE) &&
           verifier.VerifyTable(message()) &&
           verifier.EndTable();
  }
};

struct RecordingTrackLogMessageBuilder {
  typedef RecordingTrackLogMessage Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_recordingtoken(uint64_t recordingtoken) {
    fbb_.AddElement<uint64_t>(RecordingTrackLogMessage::VT_RECORDINGTOKEN, recordingtoken, 0);
  }
  void add_id(uint32_t id) {
    fbb_.AddElement<uint32_t>(RecordingTrackLogMessage::VT_ID, id, 0);
  }
  void add_message(flatbuffers::Offset<monocle::LogMessage> message) {
    fbb_.AddOffset(RecordingTrackLogMessage::VT_MESSAGE, message);
  }
  explicit RecordingTrackLogMessageBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<RecordingTrackLogMessage> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<RecordingTrackLogMessage>(end);
    return o;
  }
};

inline flatbuffers::Offset<RecordingTrackLogMessage> CreateRecordingTrackLogMessage(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t recordingtoken = 0,
    uint32_t id = 0,
    flatbuffers::Offset<monocle::LogMessage> message = 0) {
  RecordingTrackLogMessageBuilder builder_(_fbb);
  builder_.add_recordingtoken(recordingtoken);
  builder_.add_message(message);
  builder_.add_id(id);
  return builder_.Finish();
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_RECORDINGTRACKLOGMESSAGE_MONOCLE_H_
