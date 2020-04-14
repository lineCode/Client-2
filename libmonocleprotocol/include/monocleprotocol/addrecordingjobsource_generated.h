// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ADDRECORDINGJOBSOURCE_MONOCLE_H_
#define FLATBUFFERS_GENERATED_ADDRECORDINGJOBSOURCE_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

#include "receivermode_generated.h"

namespace monocle {

struct AddRecordingJobSource;
struct AddRecordingJobSourceBuilder;

struct AddRecordingJobSource FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef AddRecordingJobSourceBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RECORDINGTRACKID = 4,
    VT_RECEIVERMODE = 6,
    VT_SOURCEPARAMETERS = 8,
    VT_MEDIAURI = 10,
    VT_USERNAME = 12,
    VT_PASSWORD = 14,
    VT_RECORDINGJOBSOURCETRACKPARAMETERS = 16
  };
  uint32_t recordingtrackid() const {
    return GetField<uint32_t>(VT_RECORDINGTRACKID, 0);
  }
  monocle::ReceiverMode receivermode() const {
    return static_cast<monocle::ReceiverMode>(GetField<int8_t>(VT_RECEIVERMODE, 0));
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *sourceparameters() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_SOURCEPARAMETERS);
  }
  const flatbuffers::String *mediauri() const {
    return GetPointer<const flatbuffers::String *>(VT_MEDIAURI);
  }
  const flatbuffers::String *username() const {
    return GetPointer<const flatbuffers::String *>(VT_USERNAME);
  }
  const flatbuffers::String *password() const {
    return GetPointer<const flatbuffers::String *>(VT_PASSWORD);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *recordingjobsourcetrackparameters() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_RECORDINGJOBSOURCETRACKPARAMETERS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_RECORDINGTRACKID) &&
           VerifyField<int8_t>(verifier, VT_RECEIVERMODE) &&
           VerifyOffset(verifier, VT_SOURCEPARAMETERS) &&
           verifier.VerifyVector(sourceparameters()) &&
           verifier.VerifyVectorOfStrings(sourceparameters()) &&
           VerifyOffset(verifier, VT_MEDIAURI) &&
           verifier.VerifyString(mediauri()) &&
           VerifyOffset(verifier, VT_USERNAME) &&
           verifier.VerifyString(username()) &&
           VerifyOffset(verifier, VT_PASSWORD) &&
           verifier.VerifyString(password()) &&
           VerifyOffset(verifier, VT_RECORDINGJOBSOURCETRACKPARAMETERS) &&
           verifier.VerifyVector(recordingjobsourcetrackparameters()) &&
           verifier.VerifyVectorOfStrings(recordingjobsourcetrackparameters()) &&
           verifier.EndTable();
  }
};

struct AddRecordingJobSourceBuilder {
  typedef AddRecordingJobSource Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_recordingtrackid(uint32_t recordingtrackid) {
    fbb_.AddElement<uint32_t>(AddRecordingJobSource::VT_RECORDINGTRACKID, recordingtrackid, 0);
  }
  void add_receivermode(monocle::ReceiverMode receivermode) {
    fbb_.AddElement<int8_t>(AddRecordingJobSource::VT_RECEIVERMODE, static_cast<int8_t>(receivermode), 0);
  }
  void add_sourceparameters(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> sourceparameters) {
    fbb_.AddOffset(AddRecordingJobSource::VT_SOURCEPARAMETERS, sourceparameters);
  }
  void add_mediauri(flatbuffers::Offset<flatbuffers::String> mediauri) {
    fbb_.AddOffset(AddRecordingJobSource::VT_MEDIAURI, mediauri);
  }
  void add_username(flatbuffers::Offset<flatbuffers::String> username) {
    fbb_.AddOffset(AddRecordingJobSource::VT_USERNAME, username);
  }
  void add_password(flatbuffers::Offset<flatbuffers::String> password) {
    fbb_.AddOffset(AddRecordingJobSource::VT_PASSWORD, password);
  }
  void add_recordingjobsourcetrackparameters(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> recordingjobsourcetrackparameters) {
    fbb_.AddOffset(AddRecordingJobSource::VT_RECORDINGJOBSOURCETRACKPARAMETERS, recordingjobsourcetrackparameters);
  }
  explicit AddRecordingJobSourceBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<AddRecordingJobSource> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<AddRecordingJobSource>(end);
    return o;
  }
};

inline flatbuffers::Offset<AddRecordingJobSource> CreateAddRecordingJobSource(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t recordingtrackid = 0,
    monocle::ReceiverMode receivermode = monocle::ReceiverMode::AutoConnect,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> sourceparameters = 0,
    flatbuffers::Offset<flatbuffers::String> mediauri = 0,
    flatbuffers::Offset<flatbuffers::String> username = 0,
    flatbuffers::Offset<flatbuffers::String> password = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> recordingjobsourcetrackparameters = 0) {
  AddRecordingJobSourceBuilder builder_(_fbb);
  builder_.add_recordingjobsourcetrackparameters(recordingjobsourcetrackparameters);
  builder_.add_password(password);
  builder_.add_username(username);
  builder_.add_mediauri(mediauri);
  builder_.add_sourceparameters(sourceparameters);
  builder_.add_recordingtrackid(recordingtrackid);
  builder_.add_receivermode(receivermode);
  return builder_.Finish();
}

inline flatbuffers::Offset<AddRecordingJobSource> CreateAddRecordingJobSourceDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t recordingtrackid = 0,
    monocle::ReceiverMode receivermode = monocle::ReceiverMode::AutoConnect,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *sourceparameters = nullptr,
    const char *mediauri = nullptr,
    const char *username = nullptr,
    const char *password = nullptr,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *recordingjobsourcetrackparameters = nullptr) {
  auto sourceparameters__ = sourceparameters ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*sourceparameters) : 0;
  auto mediauri__ = mediauri ? _fbb.CreateString(mediauri) : 0;
  auto username__ = username ? _fbb.CreateString(username) : 0;
  auto password__ = password ? _fbb.CreateString(password) : 0;
  auto recordingjobsourcetrackparameters__ = recordingjobsourcetrackparameters ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*recordingjobsourcetrackparameters) : 0;
  return monocle::CreateAddRecordingJobSource(
      _fbb,
      recordingtrackid,
      receivermode,
      sourceparameters__,
      mediauri__,
      username__,
      password__,
      recordingjobsourcetrackparameters__);
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_ADDRECORDINGJOBSOURCE_MONOCLE_H_
