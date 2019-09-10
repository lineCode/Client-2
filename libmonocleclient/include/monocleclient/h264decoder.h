// h264decoder.h
//

#ifndef ID6GO0DXQOZO7J5RHQ6FDKVI9PJAPZIDFV
#define ID6GO0DXQOZO7J5RHQ6FDKVI9PJAPZIDFV

///// Includes /////

#include "decoder.h"

#include <boost/optional.hpp>
#include <string>
#include <vector>

///// Namespaces /////

namespace client
{

///// Classes /////

class H264Decoder : public Decoder
{
 public:

  H264Decoder(const uint64_t id, const utility::PublicKey& publickey);
  virtual ~H264Decoder();

  virtual void Destroy() override;

  virtual monocle::Codec GetCodec() const { return monocle::Codec::H264; };
  virtual AVCodecID GetCodecId() const override { return AV_CODEC_ID_H264; }

  DECODERERROR Init(const std::vector<std::string>& parameters);

  inline const boost::optional<int>& GetHardwareDevice() const { return hardwaredevice_; }
  inline const std::string& GetProfileLeveLID() const { return profilelevelid_; }
  inline const std::string& GetSPropParmaeterSets() const { return spropparametersets_; }

  ImageBuffer Decode(const uint64_t playrequestindex, const bool marker, const uint64_t time, const int64_t sequencenum, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint8_t* data, const int size, FreeImageBuffers* freeimagebuffers);

 private:

  DECODERERROR Init();

  boost::optional<int> hardwaredevice_;
  std::string profilelevelid_;
  std::string spropparametersets_;

};

}

#endif
