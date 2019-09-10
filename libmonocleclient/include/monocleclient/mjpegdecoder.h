// mjpegdecoder.h
//

#ifndef ID7JLRVE7EATQ18Z5R9QGPEKCFSESNU4M5
#define ID7JLRVE7EATQ18Z5R9QGPEKCFSESNU4M5

///// Includes /////

#include "decoder.h"

#include <monocleprotocol/codec_generated.h>

///// Namespaces /////

namespace client
{

///// Classes /////

class MJpegDecoder : public Decoder
{
 public:

  MJpegDecoder(const uint64_t id, const utility::PublicKey& publickey);
  virtual ~MJpegDecoder();
  virtual void Destroy() override;

  virtual monocle::Codec GetCodec() const { return monocle::Codec::MJPEG; };
  virtual AVCodecID GetCodecId() const override { return AV_CODEC_ID_MJPEG; }

  DECODERERROR Init();

  ImageBuffer Decode(const uint64_t playrequestindex, const uint64_t time, const int64_t sequencenum, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint8_t* payload, unsigned int payloadsize, FreeImageBuffers* freeimagebuffers);

 private:

};

}

#endif
