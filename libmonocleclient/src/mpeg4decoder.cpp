// mpeg4decoder.cpp
//

///// Includes /////

#include "monocleclient/mpeg4decoder.h"

#include <boost/algorithm/string.hpp>
#include <utility/utility.hpp>

///// Namespaces /////

namespace client
{

///// Methods /////

MPEG4Decoder::MPEG4Decoder(const uint64_t id, utility::PublicKey& publickey) :
  Decoder(id, publickey, nullptr)
{
  
}

MPEG4Decoder::~MPEG4Decoder()
{
  Destroy();

}

void MPEG4Decoder::Destroy()
{
  Decoder::Destroy();

}

DECODERERROR MPEG4Decoder::Init(const std::vector<std::string>& parameters)
{
  codec_ = avcodec_find_decoder(AV_CODEC_ID_MPEG4);
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

  context_->flags2 |= AV_CODEC_FLAG2_CHUNKS;
  context_->thread_count = 1;

  av_init_packet(&packet_);
  packet_.dts = AV_NOPTS_VALUE;
  packet_.pts = AV_NOPTS_VALUE;

  // profile-levl-id
  std::string profilelevelid = "1";
  std::vector<std::string>::const_iterator i = std::find_if(parameters.cbegin(), parameters.cend(), [](const std::string& parameter) { return boost::istarts_with(parameter, "profile-level-id="); });
  if (i != parameters.cend())
  {
    if (!std::all_of(i->begin() + 17, i->cend(), [](char c) { return std::isdigit(c); }))
    {

      return DECODERERROR_INVALIDPROFILELEVELID;
    }
  }

  try
  {
    context_->profile = std::stoi(profilelevelid.c_str(), nullptr, 16);

  }
  catch (...)
  {

    return DECODERERROR_INVALIDPROFILELEVELID;
  }

  // config
  i = std::find_if(parameters.cbegin(), parameters.cend(), [](const std::string& parameter) { return boost::istarts_with(parameter, "config="); });
  if (i != parameters.cend())
  {
    if (!std::all_of(i->begin() + 7, i->cend(), [](char c) { return std::isxdigit(c); }))
    {

      return DECODERERROR_INVALIDCONFIG;
    }

    std::vector<unsigned char> extradata = utility::HexToData(i->substr(7));
    extradata.insert(extradata.begin(), { 0, 0, 1 });
    context_->extradata_size = static_cast<int>(extradata.size());
    context_->extradata = new uint8_t[extradata.size()];
    memcpy(context_->extradata, extradata.data(), extradata.size());
  }

  AVDictionary* dictionary = nullptr;
  if (avcodec_open2(context_, codec_, &dictionary) < 0)
  {

    return DECODERERROR_OPENCODEC;
  }
  
  return DECODERERROR_SUCCESS;
}

ImageBuffer MPEG4Decoder::Decode(const uint64_t playrequestindex, const uint64_t time, const int64_t sequencenum, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint8_t* data, const int size, FreeImageBuffers* freeimagebuffers)
{
  return Decoder::Decode(playrequestindex, true, time, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, data, size, freeimagebuffers);
}

}
