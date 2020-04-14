// mjpegdecoder.cpp
//

///// Includes /////

#include "monocleclient/mjpegdecoder.h"

#include <utility/utility.hpp>

///// Namespaces /////

namespace client
{

///// Methods /////

MJpegDecoder::MJpegDecoder(const uint32_t trackid, const uint64_t id, const utility::PublicKey& publickey) :
  Decoder(trackid, id, publickey, nullptr)
{
  
}

MJpegDecoder::~MJpegDecoder()
{
  Destroy();

}

void MJpegDecoder::Destroy()
{
  Decoder::Destroy();

}

DECODERERROR MJpegDecoder::Init()
{
  codec_ = avcodec_find_decoder(AV_CODEC_ID_MJPEG);
  if (codec_ == nullptr)
  {

    return DECODERERROR_INITCODEC;
  }

  context_ = avcodec_alloc_context3(codec_);
  if (context_ == nullptr)
  {

    return DECODERERROR_INITCONTEXT;
  }

  if (avcodec_get_context_defaults3(context_, codec_) < 0)
  {

    return DECODERERROR_RESETCODEC;
  }

  context_->width = 0;
  context_->height = 0;

  context_->time_base.num = 1;
  context_->time_base.den = 90000;

  context_->thread_count = 1;

  av_init_packet(&packet_);
  packet_.dts = AV_NOPTS_VALUE;
  packet_.pts = AV_NOPTS_VALUE;

  AVDictionary* dictionary = nullptr;
  if (avcodec_open2(context_, codec_, &dictionary) < 0)
  {

    return DECODERERROR_OPENCODEC;
  }

  return DECODERERROR_SUCCESS;
}

ImageBuffer MJpegDecoder::Decode(const uint64_t playrequestindex, const uint64_t time, const int64_t sequencenum, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint8_t* payload, unsigned int payloadsize, FreeImageBuffers* freeimagebuffers)
{
  return Decoder::Decode(playrequestindex, true, time, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, payload, payloadsize, freeimagebuffers);
}

}
