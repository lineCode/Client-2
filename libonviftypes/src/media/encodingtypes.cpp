// encodingtypes.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Classes /////

EncodingTypes::EncodingTypes()
{

}

EncodingTypes::EncodingTypes(const std::vector<VIDEOENCODING>& videoencodings) :
  videoencodings_(videoencodings)
{

}

EncodingTypes::EncodingTypes(const std::string& encodings)
{
  std::vector<std::string> items;
  boost::algorithm::split(items, encodings, boost::is_any_of("\t\n "),boost::token_compress_on); 
  
  for (const auto& item : items)
  {
    const boost::optional<VIDEOENCODING> videoencoding = GetVideoEncoding(item);
    if (videoencoding.is_initialized())
    {
      videoencodings_.push_back(*videoencoding);
      
    }
  }
}

std::string EncodingTypes::ToString() const
{
  std::vector<std::string> encodings;
  for (const VIDEOENCODING videoencoding : videoencodings_)
  {
    encodings.push_back(onvif::ToString(videoencoding));

  }
  return boost::join(encodings, std::string(" "));
}

bool EncodingTypes::operator==(const EncodingTypes& rhs) const
{
  return std::is_permutation(videoencodings_.begin(), videoencodings_.end(), rhs.videoencodings_.begin(), rhs.videoencodings_.end());
}

}

}
