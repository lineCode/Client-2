// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_GPUSTAT_MONOCLE_H_
#define FLATBUFFERS_GENERATED_GPUSTAT_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

struct GPUStat;
struct GPUStatBuilder;

struct GPUStat FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef GPUStatBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_UUID = 4,
    VT_NAME = 6,
    VT_GPUUSAGE = 8,
    VT_MEMORYUSAGE = 10,
    VT_TOTALMEMORY = 12,
    VT_USEDMEMORY = 14
  };
  const flatbuffers::String *uuid() const {
    return GetPointer<const flatbuffers::String *>(VT_UUID);
  }
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  uint32_t gpuusage() const {
    return GetField<uint32_t>(VT_GPUUSAGE, 0);
  }
  uint32_t memoryusage() const {
    return GetField<uint32_t>(VT_MEMORYUSAGE, 0);
  }
  uint64_t totalmemory() const {
    return GetField<uint64_t>(VT_TOTALMEMORY, 0);
  }
  uint64_t usedmemory() const {
    return GetField<uint64_t>(VT_USEDMEMORY, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_UUID) &&
           verifier.VerifyString(uuid()) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<uint32_t>(verifier, VT_GPUUSAGE) &&
           VerifyField<uint32_t>(verifier, VT_MEMORYUSAGE) &&
           VerifyField<uint64_t>(verifier, VT_TOTALMEMORY) &&
           VerifyField<uint64_t>(verifier, VT_USEDMEMORY) &&
           verifier.EndTable();
  }
};

struct GPUStatBuilder {
  typedef GPUStat Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_uuid(flatbuffers::Offset<flatbuffers::String> uuid) {
    fbb_.AddOffset(GPUStat::VT_UUID, uuid);
  }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(GPUStat::VT_NAME, name);
  }
  void add_gpuusage(uint32_t gpuusage) {
    fbb_.AddElement<uint32_t>(GPUStat::VT_GPUUSAGE, gpuusage, 0);
  }
  void add_memoryusage(uint32_t memoryusage) {
    fbb_.AddElement<uint32_t>(GPUStat::VT_MEMORYUSAGE, memoryusage, 0);
  }
  void add_totalmemory(uint64_t totalmemory) {
    fbb_.AddElement<uint64_t>(GPUStat::VT_TOTALMEMORY, totalmemory, 0);
  }
  void add_usedmemory(uint64_t usedmemory) {
    fbb_.AddElement<uint64_t>(GPUStat::VT_USEDMEMORY, usedmemory, 0);
  }
  explicit GPUStatBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<GPUStat> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<GPUStat>(end);
    return o;
  }
};

inline flatbuffers::Offset<GPUStat> CreateGPUStat(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> uuid = 0,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    uint32_t gpuusage = 0,
    uint32_t memoryusage = 0,
    uint64_t totalmemory = 0,
    uint64_t usedmemory = 0) {
  GPUStatBuilder builder_(_fbb);
  builder_.add_usedmemory(usedmemory);
  builder_.add_totalmemory(totalmemory);
  builder_.add_memoryusage(memoryusage);
  builder_.add_gpuusage(gpuusage);
  builder_.add_name(name);
  builder_.add_uuid(uuid);
  return builder_.Finish();
}

inline flatbuffers::Offset<GPUStat> CreateGPUStatDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *uuid = nullptr,
    const char *name = nullptr,
    uint32_t gpuusage = 0,
    uint32_t memoryusage = 0,
    uint64_t totalmemory = 0,
    uint64_t usedmemory = 0) {
  auto uuid__ = uuid ? _fbb.CreateString(uuid) : 0;
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return monocle::CreateGPUStat(
      _fbb,
      uuid__,
      name__,
      gpuusage,
      memoryusage,
      totalmemory,
      usedmemory);
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_GPUSTAT_MONOCLE_H_
