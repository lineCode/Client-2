// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_UNSUBSCRIBERECORDINGJOBLOGRESPONSE_MONOCLE_H_
#define FLATBUFFERS_GENERATED_UNSUBSCRIBERECORDINGJOBLOGRESPONSE_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

struct UnsubscribeRecordingJobLogResponse;
struct UnsubscribeRecordingJobLogResponseBuilder;

struct UnsubscribeRecordingJobLogResponse FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef UnsubscribeRecordingJobLogResponseBuilder Builder;
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           verifier.EndTable();
  }
};

struct UnsubscribeRecordingJobLogResponseBuilder {
  typedef UnsubscribeRecordingJobLogResponse Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  explicit UnsubscribeRecordingJobLogResponseBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<UnsubscribeRecordingJobLogResponse> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<UnsubscribeRecordingJobLogResponse>(end);
    return o;
  }
};

inline flatbuffers::Offset<UnsubscribeRecordingJobLogResponse> CreateUnsubscribeRecordingJobLogResponse(
    flatbuffers::FlatBufferBuilder &_fbb) {
  UnsubscribeRecordingJobLogResponseBuilder builder_(_fbb);
  return builder_.Finish();
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_UNSUBSCRIBERECORDINGJOBLOGRESPONSE_MONOCLE_H_
