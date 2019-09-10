// h265decoder.h
//

#ifndef ID6GO0DXQOZO7J54545455555PJAPZIDFV
#define ID6GO0DXQOZO7J54545455555PJAPZIDFV

///// Includes /////

#include "decoder.h"

#include <string>
#include <vector>

///// Namespaces /////

namespace client
{

///// Classes /////

class H265Decoder : public Decoder
{
 public:

  H265Decoder(const uint64_t id, const utility::PublicKey& publickey);
  virtual ~H265Decoder();

  virtual void Destroy() override;

  virtual monocle::Codec GetCodec() const { return monocle::Codec::H265; };
  virtual AVCodecID GetCodecId() const override { return AV_CODEC_ID_H265; }

  DECODERERROR Init(const std::vector<std::string>& parameters);

  ImageBuffer Decode(const uint64_t playrequestindex, const bool marker, const uint64_t time, const int64_t sequencenum, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint8_t* data, const int size, FreeImageBuffers* freeimagebuffers);

 private:

  DECODERERROR Init();

  std::string spropsps_;
  std::string sproppps_;
  std::string spropvps_;
  std::string spropsei_;


};

}

#endif
