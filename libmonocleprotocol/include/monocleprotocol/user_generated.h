// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_USER_MONOCLE_H_
#define FLATBUFFERS_GENERATED_USER_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

struct User;
struct UserBuilder;

struct User FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef UserBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TOKEN = 4,
    VT_USERNAME = 6,
    VT_GROUPTOKEN = 8
  };
  uint64_t token() const {
    return GetField<uint64_t>(VT_TOKEN, 0);
  }
  const flatbuffers::String *username() const {
    return GetPointer<const flatbuffers::String *>(VT_USERNAME);
  }
  uint64_t grouptoken() const {
    return GetField<uint64_t>(VT_GROUPTOKEN, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_TOKEN) &&
           VerifyOffset(verifier, VT_USERNAME) &&
           verifier.VerifyString(username()) &&
           VerifyField<uint64_t>(verifier, VT_GROUPTOKEN) &&
           verifier.EndTable();
  }
};

struct UserBuilder {
  typedef User Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_token(uint64_t token) {
    fbb_.AddElement<uint64_t>(User::VT_TOKEN, token, 0);
  }
  void add_username(flatbuffers::Offset<flatbuffers::String> username) {
    fbb_.AddOffset(User::VT_USERNAME, username);
  }
  void add_grouptoken(uint64_t grouptoken) {
    fbb_.AddElement<uint64_t>(User::VT_GROUPTOKEN, grouptoken, 0);
  }
  explicit UserBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<User> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<User>(end);
    return o;
  }
};

inline flatbuffers::Offset<User> CreateUser(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t token = 0,
    flatbuffers::Offset<flatbuffers::String> username = 0,
    uint64_t grouptoken = 0) {
  UserBuilder builder_(_fbb);
  builder_.add_grouptoken(grouptoken);
  builder_.add_token(token);
  builder_.add_username(username);
  return builder_.Finish();
}

inline flatbuffers::Offset<User> CreateUserDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t token = 0,
    const char *username = nullptr,
    uint64_t grouptoken = 0) {
  auto username__ = username ? _fbb.CreateString(username) : 0;
  return monocle::CreateUser(
      _fbb,
      token,
      username__,
      grouptoken);
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_USER_MONOCLE_H_
