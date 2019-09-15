// h264options2.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

H264Options2::H264Options2()
{
  
}

H264Options2::H264Options2(const std::vector<VideoResolution>& resolutionsavailable, const boost::optional<IntRange>& govlengthrange, const boost::optional<IntRange>& framerateaverage, const boost::optional<IntRange>& encodingintervalrange, const std::vector<H264PROFILE>& h264profilessupported, const boost::optional<IntRange>& bitraterange) :
  resolutionsavailable_(resolutionsavailable),
  govlengthrange_(govlengthrange),
  framerateaverage_(framerateaverage),
  encodingintervalrange_(encodingintervalrange),
  h264profilessupported_(h264profilessupported),
  bitraterange_(bitraterange)
{
  
}

H264Options2::H264Options2(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "ResolutionsAvailable")
    {
      resolutionsavailable_.push_back(VideoResolution(element));

    }
    else if (name == "GovLengthRange")
    {
      govlengthrange_ = IntRange(element);

    }
    else if (name == "FrameRateRange")
    {
      framerateaverage_ = IntRange(element);

    }
    else if (name == "EncodingIntervalRange")
    {
      encodingintervalrange_ = IntRange(element);

    }
    else if (name == "H264ProfilesSupported")
    {
      boost::optional<H264PROFILE> h264profile = GetH264Profile(element.text().get());
      if (h264profile.is_initialized())
      {
        h264profilessupported_.push_back(*h264profile);

      }
    }
    else if (name == "BitrateRange")
    {
      bitraterange_ = IntRange(element);

    }
  }
}

std::string H264Options2::ToXml(const std::string& name) const
{
  std::string resolutions;
  for (const auto& resolution : resolutionsavailable_)
  {
    resolutions += resolution.ToXml("tt:ResolutionsAvailable");

  }
  
  std::string h264profilessupported;
  for (const auto& h264profilesupported : h264profilessupported_)
  {
    h264profilessupported += onvif::ToXmlEnum("H264ProfilesSupported", h264profilesupported);

  }
  
  return ("<"+name+">" + resolutions + ToXmlClass("GovLengthRange", govlengthrange_) + ToXmlClass("FrameRateRange", framerateaverage_) + ToXmlClass("EncodingIntervalRange", encodingintervalrange_) + h264profilessupported + ToXmlClass("BitrateRange", bitraterange_) + "</"+name+">");
}

bool H264Options2::operator==(const H264Options2& rhs) const
{
  return (std::is_permutation(resolutionsavailable_.begin(), resolutionsavailable_.end(), rhs.resolutionsavailable_.begin(), rhs.resolutionsavailable_.end()) && (govlengthrange_ == rhs.govlengthrange_) && (framerateaverage_ == rhs.framerateaverage_) && (encodingintervalrange_ == rhs.encodingintervalrange_) && (std::is_permutation(h264profilessupported_.begin(), h264profilessupported_.end(), rhs.h264profilessupported_.begin(), rhs.h264profilessupported_.end())) && (bitraterange_ == rhs.bitraterange_));
}

}
