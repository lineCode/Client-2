// stream.h
//

#ifndef IDFMKGWSDFDSFSDFDFEFEWQCDC366BC609
#define IDFMKGWSDFDSFSDFDFEFEWQCDC366BC609

///// Includes /////

#include <stdint.h>
#include <string>
#include <vector>

///// Declarations /////

namespace monocle
{

enum class Codec : int8_t;
enum class ErrorCode : uint16_t;
enum class MetadataFrameType : uint16_t;

}

///// Namespaces /////

namespace client
{

///// Declarations /////

typedef void(*CONTROLSTREAMEND)(const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error, void* callbackdata);
typedef void(*H265CALLBACK)(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata);
typedef void(*H264CALLBACK)(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata);
typedef void(*JPEGCALLBACK)(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* framedata, const size_t size, void* callbackdata);
typedef void(*METADATACALLBACK)(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata);
typedef void(*MPEG4CALLBACK)(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata);
typedef void(*NEWCODECINDEX)(const uint64_t streamtoken, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp, void* callbackdata);

///// Prototypes /////

int BuildH265Frame(const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size, std::vector<unsigned char>& buffer);
int BuildH264Frame(const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size, std::vector<unsigned char>& buffer);
void JPEGHeader(const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, std::vector<unsigned char>& buffer);

///// Classes /////

class Stream
{
 public:

  Stream(const uint64_t token, const CONTROLSTREAMEND controlstreamendcallback, const H265CALLBACK h265callback, const H264CALLBACK h264callback, const METADATACALLBACK metadatacallback, const JPEGCALLBACK jpegcallback, const MPEG4CALLBACK mpeg4callback, const NEWCODECINDEX newcodecindexcallback, void* callbackdata);
  Stream(const Stream& rhs);
  ~Stream();

  void ControlStreamEnd(const uint64_t playrequestindex, const monocle::ErrorCode error);
  void H265Frame(const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size) const;
  void H264Frame(const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size) const;
  void MetadataFrame(const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* data, const size_t size) const;
  void JPEGFrame(const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* data, size_t size) const;
  void MPEG4Frame(const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* data, const size_t size) const;
  
  void NewCodecIndex(const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp);

  inline uint64_t GetToken() const { return token_; }

  Stream& operator=(const Stream& rhs);

 private:

  uint64_t token_;
  CONTROLSTREAMEND controlstreamendcallback_;
  H265CALLBACK h265callback_;
  H264CALLBACK h264callback_;
  METADATACALLBACK metadatacallback_;
  JPEGCALLBACK jpegcallback_;
  MPEG4CALLBACK mpeg4callback_;
  NEWCODECINDEX newcodecindexcallback_;

  void* callbackdata_;

  mutable std::vector<unsigned char> buffer_;

};

}

#endif
