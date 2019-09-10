// h264options.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

H264Options::H264Options()
{
  
}

H264Options::H264Options(const std::vector<VideoResolution>& resolutionsavailable, const boost::optional<IntRange>& govlengthrange, const boost::optional<IntRange>& frameraterange, const boost::optional<IntRange>& encodingintervalrange, const std::vector<H264PROFILE>& h264profilessupported) :
  resolutionsavailable_(resolutionsavailable),
  govlengthrange_(govlengthrange),
  frameraterange_(frameraterange),
  encodingintervalrange_(encodingintervalrange),
  h264profilessupported_(h264profilessupported)
{
  
}

H264Options::H264Options(const pugi::xml_node& node)
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
      frameraterange_ = IntRange(element);

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
  }
}

std::string H264Options::ToXml(const std::string& name) const
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

  return ("<"+name+">" + resolutions + ToXmlClass("GovLengthRange", govlengthrange_) + ToXmlClass("FrameRateRange", frameraterange_) + ToXmlClass("EncodingIntervalRange", encodingintervalrange_) + h264profilessupported + "</"+name+">");
}

bool H264Options::operator==(const H264Options& rhs) const
{
  return (std::is_permutation(resolutionsavailable_.begin(), resolutionsavailable_.end(), rhs.resolutionsavailable_.begin(), rhs.resolutionsavailable_.end()) && (govlengthrange_ == rhs.govlengthrange_) && (frameraterange_ == rhs.frameraterange_) && (encodingintervalrange_ == rhs.encodingintervalrange_) && (std::is_permutation(h264profilessupported_.begin(), h264profilessupported_.end(), rhs.h264profilessupported_.begin(), rhs.h264profilessupported_.end())));
}

}
