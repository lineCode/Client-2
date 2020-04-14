// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_RECORDINGJOBSOURCEADDED_MONOCLE_H_
#define FLATBUFFERS_GENERATED_RECORDINGJOBSOURCEADDED_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

struct RecordingJobSourceAdded;
struct RecordingJobSourceAddedBuilder;

struct RecordingJobSourceAdded FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef RecordingJobSourceAddedBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RECORDINGTOKEN = 4,
    VT_RECORDINGJOBTOKEN = 6,
    VT_RECORDINGJOBSOURCETOKEN = 8,
    VT_RECEIVERTOKEN = 10
  };
  uint64_t recordingtoken() const {
    return GetField<uint64_t>(VT_RECORDINGTOKEN, 0);
  }
  uint64_t recordingjobtoken() const {
    return GetField<uint64_t>(VT_RECORDINGJOBTOKEN, 0);
  }
  uint64_t recordingjobsourcetoken() const {
    return GetField<uint64_t>(VT_RECORDINGJOBSOURCETOKEN, 0);
  }
  uint64_t receivertoken() const {
    return GetField<uint64_t>(VT_RECEIVERTOKEN, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_RECORDINGTOKEN) &&
           VerifyField<uint64_t>(verifier, VT_RECORDINGJOBTOKEN) &&
           VerifyField<uint64_t>(verifier, VT_RECORDINGJOBSOURCETOKEN) &&
           VerifyField<uint64_t>(verifier, VT_RECEIVERTOKEN) &&
           verifier.EndTable();
  }
};

struct RecordingJobSourceAddedBuilder {
  typedef RecordingJobSourceAdded Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_recordingtoken(uint64_t recordingtoken) {
    fbb_.AddElement<uint64_t>(RecordingJobSourceAdded::VT_RECORDINGTOKEN, recordingtoken, 0);
  }
  void add_recordingjobtoken(uint64_t recordingjobtoken) {
    fbb_.AddElement<uint64_t>(RecordingJobSourceAdded::VT_RECORDINGJOBTOKEN, recordingjobtoken, 0);
  }
  void add_recordingjobsourcetoken(uint64_t recordingjobsourcetoken) {
    fbb_.AddElement<uint64_t>(RecordingJobSourceAdded::VT_RECORDINGJOBSOURCETOKEN, recordingjobsourcetoken, 0);
  }
  void add_receivertoken(uint64_t receivertoken) {
    fbb_.AddElement<uint64_t>(RecordingJobSourceAdded::VT_RECEIVERTOKEN, receivertoken, 0);
  }
  explicit RecordingJobSourceAddedBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<RecordingJobSourceAdded> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<RecordingJobSourceAdded>(end);
    return o;
  }
};

inline flatbuffers::Offset<RecordingJobSourceAdded> CreateRecordingJobSourceAdded(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t recordingtoken = 0,
    uint64_t recordingjobtoken = 0,
    uint64_t recordingjobsourcetoken = 0,
    uint64_t receivertoken = 0) {
  RecordingJobSourceAddedBuilder builder_(_fbb);
  builder_.add_receivertoken(receivertoken);
  builder_.add_recordingjobsourcetoken(recordingjobsourcetoken);
  builder_.add_recordingjobtoken(recordingjobtoken);
  builder_.add_recordingtoken(recordingtoken);
  return builder_.Finish();
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_RECORDINGJOBSOURCEADDED_MONOCLE_H_
