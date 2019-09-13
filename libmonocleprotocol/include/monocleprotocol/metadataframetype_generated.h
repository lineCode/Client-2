// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_METADATAFRAMETYPE_MONOCLE_H_
#define FLATBUFFERS_GENERATED_METADATAFRAMETYPE_MONOCLE_H_

#include "flatbuffers/flatbuffers.h"

namespace monocle {

enum class MetadataFrameType : uint16_t {
  ONVIF_XML = 1,
  OBJECT_DETECTION = 2,
  MIN = ONVIF_XML,
  MAX = OBJECT_DETECTION
};

inline const MetadataFrameType (&EnumValuesMetadataFrameType())[2] {
  static const MetadataFrameType values[] = {
    MetadataFrameType::ONVIF_XML,
    MetadataFrameType::OBJECT_DETECTION
  };
  return values;
}

inline const char * const *EnumNamesMetadataFrameType() {
  static const char * const names[3] = {
    "ONVIF_XML",
    "OBJECT_DETECTION",
    nullptr
  };
  return names;
}

inline const char *EnumNameMetadataFrameType(MetadataFrameType e) {
  if (e < MetadataFrameType::ONVIF_XML || e > MetadataFrameType::OBJECT_DETECTION) return "";
  const size_t index = static_cast<size_t>(e) - static_cast<size_t>(MetadataFrameType::ONVIF_XML);
  return EnumNamesMetadataFrameType()[index];
}

}  // namespace monocle

#endif  // FLATBUFFERS_GENERATED_METADATAFRAMETYPE_MONOCLE_H_
