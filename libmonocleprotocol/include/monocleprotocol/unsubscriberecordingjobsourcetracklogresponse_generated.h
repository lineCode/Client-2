// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE_MONOCLE_H_
#define FLATBUFFERS_GENERATED_UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

struct UnsubscribeRecordingJobSourceTrackLogResponse;
struct UnsubscribeRecordingJobSourceTrackLogResponseBuilder;

struct UnsubscribeRecordingJobSourceTrackLogResponse FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef UnsubscribeRecordingJobSourceTrackLogResponseBuilder Builder;
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           verifier.EndTable();
  }
};

struct UnsubscribeRecordingJobSourceTrackLogResponseBuilder {
  typedef UnsubscribeRecordingJobSourceTrackLogResponse Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  explicit UnsubscribeRecordingJobSourceTrackLogResponseBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<UnsubscribeRecordingJobSourceTrackLogResponse> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<UnsubscribeRecordingJobSourceTrackLogResponse>(end);
    return o;
  }
};

inline flatbuffers::Offset<UnsubscribeRecordingJobSourceTrackLogResponse> CreateUnsubscribeRecordingJobSourceTrackLogResponse(
    flatbuffers::FlatBufferBuilder &_fbb) {
  UnsubscribeRecordingJobSourceTrackLogResponseBuilder builder_(_fbb);
  return builder_.Finish();
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE_MONOCLE_H_
