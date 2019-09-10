// jpegoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

JpegOptions::JpegOptions()
{
  
}

JpegOptions::JpegOptions(const std::vector<VideoResolution>& resolutionsavailable, const boost::optional<IntRange>& frameraterange, const boost::optional<IntRange>& encodingintervalrange) :
  resolutionsavailable_(resolutionsavailable),
  frameraterange_(frameraterange),
  encodingintervalrange_(encodingintervalrange)
{
  
}

JpegOptions::JpegOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "ResolutionsAvailable")
    {
      resolutionsavailable_.push_back(VideoResolution(element));

    }
    else if (name == "FrameRateRange")
    {
      frameraterange_ = IntRange(element);

    }
    else if (name == "EncodingIntervalRange")
    {
      encodingintervalrange_ = IntRange(element);

    }
  }
}

std::string JpegOptions::ToXml(const std::string& name) const
{
  std::string resolutions;
  for (const auto& resolution : resolutionsavailable_)
  {
    resolutions += resolution.ToXml("tt:ResolutionsAvailable");

  }
  
  return ("<"+name+">" + resolutions + ToXmlClass("FrameRateRange", frameraterange_) + ToXmlClass("EncodingIntervalRange", encodingintervalrange_) + "</"+name+">");
}

bool JpegOptions::operator==(const JpegOptions& rhs) const
{
  return (std::is_permutation(resolutionsavailable_.begin(), resolutionsavailable_.end(), rhs.resolutionsavailable_.begin(), rhs.resolutionsavailable_.end()) && (frameraterange_ == rhs.frameraterange_) && (encodingintervalrange_ == rhs.encodingintervalrange_));
}

}
