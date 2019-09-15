// jpegoptions2.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

JpegOptions2::JpegOptions2()
{
  
}

JpegOptions2::JpegOptions2(const std::vector<VideoResolution>& resolutionsavailable, const boost::optional<IntRange>& framerateaverage, const boost::optional<IntRange>& encodingintervalrange, const boost::optional<IntRange>& bitraterange) :
  resolutionsavailable_(resolutionsavailable),
  framerateaverage_(framerateaverage),
  encodingintervalrange_(encodingintervalrange),
  bitraterange_(bitraterange)
{
  
}

JpegOptions2::JpegOptions2(const pugi::xml_node& node)
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
      framerateaverage_ = IntRange(element);

    }
    else if (name == "EncodingIntervalRange")
    {
      encodingintervalrange_ = IntRange(element);

    }
    else if (name == "BitrateRange")
    {
      bitraterange_ = IntRange(element);

    }
  }
}

std::string JpegOptions2::ToXml(const std::string& name) const
{
  std::string resolutions;
  for (const auto& resolution : resolutionsavailable_)
  {
    resolutions += resolution.ToXml("tt:ResolutionsAvailable");

  }
  
  return ("<"+name+">" + resolutions + ToXmlClass("FrameRateRange", framerateaverage_) + ToXmlClass("EncodingIntervalRange", encodingintervalrange_) + ToXmlClass("BitrateRange", bitraterange_) + "</"+name+">");
}

bool JpegOptions2::operator==(const JpegOptions2& rhs) const
{
  return (std::is_permutation(resolutionsavailable_.begin(), resolutionsavailable_.end(), rhs.resolutionsavailable_.begin(), rhs.resolutionsavailable_.end()) && (framerateaverage_ == rhs.framerateaverage_) && (encodingintervalrange_ == rhs.encodingintervalrange_) && (bitraterange_ == rhs.bitraterange_));
}

}
