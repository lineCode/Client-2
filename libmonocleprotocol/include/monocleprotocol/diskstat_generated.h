// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_DISKSTAT_MONOCLE_H_
#define FLATBUFFERS_GENERATED_DISKSTAT_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

struct DiskStat;
struct DiskStatBuilder;

struct DiskStat FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef DiskStatBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_DEVICE = 4,
    VT_UTILITY = 6,
    VT_READSPEED = 8,
    VT_WRITESPEED = 10
  };
  const flatbuffers::String *device() const {
    return GetPointer<const flatbuffers::String *>(VT_DEVICE);
  }
  double utility() const {
    return GetField<double>(VT_UTILITY, 0.0);
  }
  double readspeed() const {
    return GetField<double>(VT_READSPEED, 0.0);
  }
  double writespeed() const {
    return GetField<double>(VT_WRITESPEED, 0.0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_DEVICE) &&
           verifier.VerifyString(device()) &&
           VerifyField<double>(verifier, VT_UTILITY) &&
           VerifyField<double>(verifier, VT_READSPEED) &&
           VerifyField<double>(verifier, VT_WRITESPEED) &&
           verifier.EndTable();
  }
};

struct DiskStatBuilder {
  typedef DiskStat Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_device(flatbuffers::Offset<flatbuffers::String> device) {
    fbb_.AddOffset(DiskStat::VT_DEVICE, device);
  }
  void add_utility(double utility) {
    fbb_.AddElement<double>(DiskStat::VT_UTILITY, utility, 0.0);
  }
  void add_readspeed(double readspeed) {
    fbb_.AddElement<double>(DiskStat::VT_READSPEED, readspeed, 0.0);
  }
  void add_writespeed(double writespeed) {
    fbb_.AddElement<double>(DiskStat::VT_WRITESPEED, writespeed, 0.0);
  }
  explicit DiskStatBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<DiskStat> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<DiskStat>(end);
    return o;
  }
};

inline flatbuffers::Offset<DiskStat> CreateDiskStat(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> device = 0,
    double utility = 0.0,
    double readspeed = 0.0,
    double writespeed = 0.0) {
  DiskStatBuilder builder_(_fbb);
  builder_.add_writespeed(writespeed);
  builder_.add_readspeed(readspeed);
  builder_.add_utility(utility);
  builder_.add_device(device);
  return builder_.Finish();
}

inline flatbuffers::Offset<DiskStat> CreateDiskStatDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *device = nullptr,
    double utility = 0.0,
    double readspeed = 0.0,
    double writespeed = 0.0) {
  auto device__ = device ? _fbb.CreateString(device) : 0;
  return monocle::CreateDiskStat(
      _fbb,
      device__,
      utility,
      readspeed,
      writespeed);
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_DISKSTAT_MONOCLE_H_
