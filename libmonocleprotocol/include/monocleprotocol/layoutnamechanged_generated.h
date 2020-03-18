// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_LAYOUTNAMECHANGED_MONOCLE_H_
#define FLATBUFFERS_GENERATED_LAYOUTNAMECHANGED_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

struct LayoutNameChanged;

struct LayoutNameChanged FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TOKEN = 4,
    VT_NAME = 6
  };
  uint64_t token() const {
    return GetField<uint64_t>(VT_TOKEN, 0);
  }
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_TOKEN) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           verifier.EndTable();
  }
};

struct LayoutNameChangedBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_token(uint64_t token) {
    fbb_.AddElement<uint64_t>(LayoutNameChanged::VT_TOKEN, token, 0);
  }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(LayoutNameChanged::VT_NAME, name);
  }
  explicit LayoutNameChangedBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  LayoutNameChangedBuilder &operator=(const LayoutNameChangedBuilder &);
  flatbuffers::Offset<LayoutNameChanged> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<LayoutNameChanged>(end);
    return o;
  }
};

inline flatbuffers::Offset<LayoutNameChanged> CreateLayoutNameChanged(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t token = 0,
    flatbuffers::Offset<flatbuffers::String> name = 0) {
  LayoutNameChangedBuilder builder_(_fbb);
  builder_.add_token(token);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<LayoutNameChanged> CreateLayoutNameChangedDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t token = 0,
    const char *name = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return monocle::CreateLayoutNameChanged(
      _fbb,
      token,
      name__);
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_LAYOUTNAMECHANGED_MONOCLE_H_
