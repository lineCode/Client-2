// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_INDEX_MONOCLE_H_
#define FLATBUFFERS_GENERATED_INDEX_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

struct INDEX;

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(8) INDEX FLATBUFFERS_FINAL_CLASS {
 private:
  uint64_t starttime_;
  uint64_t endtime_;

 public:
  INDEX() {
    memset(static_cast<void *>(this), 0, sizeof(INDEX));
  }
  INDEX(uint64_t _starttime, uint64_t _endtime)
      : starttime_(flatbuffers::EndianScalar(_starttime)),
        endtime_(flatbuffers::EndianScalar(_endtime)) {
  }
  uint64_t starttime() const {
    return flatbuffers::EndianScalar(starttime_);
  }
  uint64_t endtime() const {
    return flatbuffers::EndianScalar(endtime_);
  }
};
FLATBUFFERS_STRUCT_END(INDEX, 16);

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_INDEX_MONOCLE_H_
