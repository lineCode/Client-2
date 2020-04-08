// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_GROUPREMOVED_MONOCLE_H_
#define FLATBUFFERS_GENERATED_GROUPREMOVED_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

struct GroupRemoved;
struct GroupRemovedBuilder;

struct GroupRemoved FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef GroupRemovedBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TOKEN = 4
  };
  uint64_t token() const {
    return GetField<uint64_t>(VT_TOKEN, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_TOKEN) &&
           verifier.EndTable();
  }
};

struct GroupRemovedBuilder {
  typedef GroupRemoved Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_token(uint64_t token) {
    fbb_.AddElement<uint64_t>(GroupRemoved::VT_TOKEN, token, 0);
  }
  explicit GroupRemovedBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<GroupRemoved> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<GroupRemoved>(end);
    return o;
  }
};

inline flatbuffers::Offset<GroupRemoved> CreateGroupRemoved(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t token = 0) {
  GroupRemovedBuilder builder_(_fbb);
  builder_.add_token(token);
  return builder_.Finish();
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_GROUPREMOVED_MONOCLE_H_
