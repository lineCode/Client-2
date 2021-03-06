// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ERRORCODE_MONOCLE_H_
#define FLATBUFFERS_GENERATED_ERRORCODE_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

enum class ErrorCode : uint16_t {
  Success = 0,
  BadAuth = 1,
  Disconnected = 2,
  InternalError = 3,
  InvalidMessage = 4,
  InvalidParameter = 5,
  InvalidToken = 6,
  MaxRecordings = 7,
  MissingMessage = 8,
  MissingParameter = 9,
  OperationCancelled = 10,
  OperationTimeout = 11,
  Permission = 12,
  UnrecognisedMessageID = 13,
  InvalidLicense = 14,
  MIN = Success,
  MAX = InvalidLicense
};

inline const ErrorCode (&EnumValuesErrorCode())[15] {
  static const ErrorCode values[] = {
    ErrorCode::Success,
    ErrorCode::BadAuth,
    ErrorCode::Disconnected,
    ErrorCode::InternalError,
    ErrorCode::InvalidMessage,
    ErrorCode::InvalidParameter,
    ErrorCode::InvalidToken,
    ErrorCode::MaxRecordings,
    ErrorCode::MissingMessage,
    ErrorCode::MissingParameter,
    ErrorCode::OperationCancelled,
    ErrorCode::OperationTimeout,
    ErrorCode::Permission,
    ErrorCode::UnrecognisedMessageID,
    ErrorCode::InvalidLicense
  };
  return values;
}

inline const char * const *EnumNamesErrorCode() {
  static const char * const names[16] = {
    "Success",
    "BadAuth",
    "Disconnected",
    "InternalError",
    "InvalidMessage",
    "InvalidParameter",
    "InvalidToken",
    "MaxRecordings",
    "MissingMessage",
    "MissingParameter",
    "OperationCancelled",
    "OperationTimeout",
    "Permission",
    "UnrecognisedMessageID",
    "InvalidLicense",
    nullptr
  };
  return names;
}

inline const char *EnumNameErrorCode(ErrorCode e) {
  if (flatbuffers::IsOutRange(e, ErrorCode::Success, ErrorCode::InvalidLicense)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesErrorCode()[index];
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_ERRORCODE_MONOCLE_H_
