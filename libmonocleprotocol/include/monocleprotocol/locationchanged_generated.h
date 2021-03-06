// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_LOCATIONCHANGED_MONOCLE_H_
#define FLATBUFFERS_GENERATED_LOCATIONCHANGED_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

struct LocationChanged;
struct LocationChangedBuilder;

struct LocationChanged FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef LocationChangedBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_LATITUDE = 4,
    VT_LONGITUDE = 6
  };
  const flatbuffers::String *latitude() const {
    return GetPointer<const flatbuffers::String *>(VT_LATITUDE);
  }
  const flatbuffers::String *longitude() const {
    return GetPointer<const flatbuffers::String *>(VT_LONGITUDE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_LATITUDE) &&
           verifier.VerifyString(latitude()) &&
           VerifyOffset(verifier, VT_LONGITUDE) &&
           verifier.VerifyString(longitude()) &&
           verifier.EndTable();
  }
};

struct LocationChangedBuilder {
  typedef LocationChanged Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_latitude(flatbuffers::Offset<flatbuffers::String> latitude) {
    fbb_.AddOffset(LocationChanged::VT_LATITUDE, latitude);
  }
  void add_longitude(flatbuffers::Offset<flatbuffers::String> longitude) {
    fbb_.AddOffset(LocationChanged::VT_LONGITUDE, longitude);
  }
  explicit LocationChangedBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<LocationChanged> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<LocationChanged>(end);
    return o;
  }
};

inline flatbuffers::Offset<LocationChanged> CreateLocationChanged(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> latitude = 0,
    flatbuffers::Offset<flatbuffers::String> longitude = 0) {
  LocationChangedBuilder builder_(_fbb);
  builder_.add_longitude(longitude);
  builder_.add_latitude(latitude);
  return builder_.Finish();
}

inline flatbuffers::Offset<LocationChanged> CreateLocationChangedDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *latitude = nullptr,
    const char *longitude = nullptr) {
  auto latitude__ = latitude ? _fbb.CreateString(latitude) : 0;
  auto longitude__ = longitude ? _fbb.CreateString(longitude) : 0;
  return monocle::CreateLocationChanged(
      _fbb,
      latitude__,
      longitude__);
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_LOCATIONCHANGED_MONOCLE_H_
