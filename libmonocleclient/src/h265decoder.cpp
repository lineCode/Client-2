// h265decoder.cpp
//

///// Includes /////

#include "monocleclient/h265decoder.h"

#include <boost/algorithm/string.hpp>
#include <utility/utility.hpp>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Globals /////

static const uint8_t STARTSEQUENCE[] = { 0, 0, 0, 1 };

///// Methods /////

H265Decoder::H265Decoder(const uint64_t id, const utility::PublicKey& publickey) :
  Decoder(id, publickey, nullptr)
{
  
}

H265Decoder::~H265Decoder()
{
  Destroy();

}

void H265Decoder::Destroy()
{
  Decoder::Destroy();
  spropsps_.clear();
  sproppps_.clear();
  spropvps_.clear();
  spropsei_.clear();
}

DECODERERROR H265Decoder::Init(const std::vector<std::string>& parameters)
{
  Destroy();

  const std::string SPROPSPS = "sprop-sps=";
  std::vector<std::string>::const_iterator spropsps = std::find_if(parameters.cbegin(), parameters.cend(), [&SPROPSPS](const std::string& parameter) { return (boost::istarts_with(parameter, SPROPSPS)); });
  if (spropsps != parameters.cend())
  {
    spropsps_ = spropsps->substr(SPROPSPS.size(), std::string::npos);

  }

  const std::string SPROPPPS = "sprop-pps=";
  std::vector<std::string>::const_iterator sproppps = std::find_if(parameters.cbegin(), parameters.cend(), [&SPROPPPS](const std::string& parameter) { return (boost::istarts_with(parameter, SPROPPPS)); });
  if (sproppps != parameters.cend())
  {
    sproppps_ = sproppps->substr(SPROPPPS.size(), std::string::npos);

  }

  const std::string SPROPVPS = "sprop-vps=";
  std::vector<std::string>::const_iterator spropvps = std::find_if(parameters.cbegin(), parameters.cend(), [&SPROPVPS](const std::string& parameter) { return (boost::istarts_with(parameter, SPROPVPS)); });
  if (spropvps != parameters.cend())
  {
    spropvps_ = spropvps->substr(SPROPVPS.size(), std::string::npos);

  }

  const std::string SPROPSEI = "sprop-sei=";
  std::vector<std::string>::const_iterator spropsei = std::find_if(parameters.cbegin(), parameters.cend(), [&SPROPSEI](const std::string& parameter) { return (boost::istarts_with(parameter, SPROPSEI)); });
  if (spropsei != parameters.cend())
  {
    spropsei_ = spropsei->substr(SPROPSEI.size(), std::string::npos);

  }
  
  return Init();
}

ImageBuffer H265Decoder::Decode(const uint64_t playrequestindex, const bool marker, const uint64_t time, const int64_t sequencenum, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint8_t* data, const int size, FreeImageBuffers* freeimagebuffers)
{
  return Decoder::Decode(playrequestindex, marker, time, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, data, size, freeimagebuffers);
}

DECODERERROR H265Decoder::Init()
{
  codec_ = avcodec_find_decoder(AV_CODEC_ID_HEVC);
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
  context_->thread_count = 1;

  context_->ticks_per_frame = 1;

  context_->time_base.num = 1;
  context_->time_base.den = 90000.0;

  if (codec_->capabilities & AV_CODEC_CAP_TRUNCATED)
  {
    context_->flags |= AV_CODEC_FLAG_TRUNCATED;

  }

  // Extradata
  std::vector<unsigned char> spropvpsdata;
  if (spropvps_.size())
  {
    spropvpsdata = utility::Base64Decode(spropvps_);

  }

  std::vector<unsigned char> spropspsdata;
  if (spropsps_.size())
  {
    spropspsdata = utility::Base64Decode(spropsps_);

  }

  std::vector<unsigned char> spropppsdata;
  if (sproppps_.size())
  {
    spropppsdata = utility::Base64Decode(sproppps_);

  }

  std::vector<unsigned char> spropseidata;
  if (spropsei_.size())
  {
    spropseidata = utility::Base64Decode(spropsei_);

  }

  const size_t PADDING_SIZE = 32;
  std::vector<uint8_t> extradata;
  extradata.reserve(spropvpsdata.size() + spropspsdata.size() + spropppsdata.size() + spropseidata.size() + PADDING_SIZE);
  if (spropvpsdata.size())
  {
    extradata.insert(extradata.end(), STARTSEQUENCE, STARTSEQUENCE + sizeof(STARTSEQUENCE));
    extradata.insert(extradata.end(), spropvpsdata.cbegin(), spropvpsdata.cend());
  }

  if (spropspsdata.size())
  {
    extradata.insert(extradata.end(), STARTSEQUENCE, STARTSEQUENCE + sizeof(STARTSEQUENCE));
    extradata.insert(extradata.end(), spropspsdata.cbegin(), spropspsdata.cend());
  }

  if (spropppsdata.size())
  {
    extradata.insert(extradata.end(), STARTSEQUENCE, STARTSEQUENCE + sizeof(STARTSEQUENCE));
    extradata.insert(extradata.end(), spropppsdata.cbegin(), spropppsdata.cend());
  }

  if (spropseidata.size())
  {
    extradata.insert(extradata.end(), STARTSEQUENCE, STARTSEQUENCE + sizeof(STARTSEQUENCE));
    extradata.insert(extradata.end(), spropseidata.cbegin(), spropseidata.cend());
  }

  // Add padding, fFmpeg does this, so we will too...
  std::array<uint8_t, PADDING_SIZE> padding;
  padding.fill(0);
  extradata.insert(extradata.end(), padding.cbegin(), padding.cend());

  context_->extradata_size = static_cast<int>(extradata.size() - PADDING_SIZE);
  if (context_->extradata_size > 0)
  {
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

}
