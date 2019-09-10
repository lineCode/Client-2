// mpeg4decoder.h
//

#ifndef ID7D4G1TBZ98JGSZ22UYXNSJQK64NSKBMP
#define ID7D4G1TBZ98JGSZ22UYXNSJQK64NSKBMP

///// Includes /////

#include "decoder.h"

#include <boost/circular_buffer.hpp>
#include <string>
#include <vector>

///// Namespaces /////

namespace client
{

///// Classes /////

class MPEG4Decoder : public Decoder
{
 public:

  MPEG4Decoder(const uint64_t id, utility::PublicKey& publickey);
  ~MPEG4Decoder();

  virtual void Destroy() override;

  virtual monocle::Codec GetCodec() const { return monocle::Codec::MPEG4; };
  virtual AVCodecID GetCodecId() const override { return AV_CODEC_ID_MPEG4; }

  DECODERERROR Init(const std::vector<std::string>& parameters);

  ImageBuffer Decode(const uint64_t playrequestindex, const uint64_t time, const int64_t sequencenum, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint8_t* data, const int size, FreeImageBuffers* freeimagebuffers);

 private:

};

}

#endif
