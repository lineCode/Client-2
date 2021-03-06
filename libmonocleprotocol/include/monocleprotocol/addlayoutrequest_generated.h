// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ADDLAYOUTREQUEST_MONOCLE_H_
#define FLATBUFFERS_GENERATED_ADDLAYOUTREQUEST_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

#include "layout_generated.h"

namespace monocle {

struct AddLayoutRequest;
struct AddLayoutRequestBuilder;

struct AddLayoutRequest FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef AddLayoutRequestBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_LAYOUT = 4
  };
  const monocle::Layout *layout() const {
    return GetPointer<const monocle::Layout *>(VT_LAYOUT);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_LAYOUT) &&
           verifier.VerifyTable(layout()) &&
           verifier.EndTable();
  }
};

struct AddLayoutRequestBuilder {
  typedef AddLayoutRequest Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_layout(flatbuffers::Offset<monocle::Layout> layout) {
    fbb_.AddOffset(AddLayoutRequest::VT_LAYOUT, layout);
  }
  explicit AddLayoutRequestBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<AddLayoutRequest> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<AddLayoutRequest>(end);
    return o;
  }
};

inline flatbuffers::Offset<AddLayoutRequest> CreateAddLayoutRequest(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<monocle::Layout> layout = 0) {
  AddLayoutRequestBuilder builder_(_fbb);
  builder_.add_layout(layout);
  return builder_.Finish();
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_ADDLAYOUTREQUEST_MONOCLE_H_
