// mpeg4options.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Mpeg4Options::Mpeg4Options()
{
  
}

Mpeg4Options::Mpeg4Options(const std::vector<VideoResolution>& resolutionsavailable, const boost::optional<IntRange>& govlengthrange, const boost::optional<IntRange>& frameraterange, const boost::optional<IntRange>& encodingintervalrange, const std::vector<MPEG4PROFILE>& mpeg4profilessupported) :
  resolutionsavailable_(resolutionsavailable),
  govlengthrange_(govlengthrange),
  frameraterange_(frameraterange),
  encodingintervalrange_(encodingintervalrange),
  mpeg4profilessupported_(mpeg4profilessupported)
{
  
}

Mpeg4Options::Mpeg4Options(const pugi::xml_node& node)
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
    else if (name == "Mpeg4ProfilesSupported")
    {
      boost::optional<MPEG4PROFILE> mpeg4profile = GetMpeg4Profile(element.text().get());
      if (mpeg4profile.is_initialized())
      {
        mpeg4profilessupported_.push_back(*mpeg4profile);

      }
    }
  }
}

std::string Mpeg4Options::ToXml(const std::string& name) const
{
  std::string resolutions;
  for (const auto& resolution : resolutionsavailable_)
  {
    resolutions += resolution.ToXml("tt:ResolutionsAvailable");

  }
  
  std::string mpeg4profilessupported;
  for (const auto& mpeg4profilesupported : mpeg4profilessupported_)
  {
    mpeg4profilessupported += onvif::ToXmlEnum("Mpeg4ProfilesSupported", mpeg4profilesupported);

  }
  
  return ("<"+name+">" + resolutions + ToXmlClass("GovLengthRange", govlengthrange_) + ToXmlClass("FrameRateRange", frameraterange_) + ToXmlClass("EncodingIntervalRange", encodingintervalrange_) + mpeg4profilessupported + "</"+name+">");
}

bool Mpeg4Options::operator==(const Mpeg4Options& rhs) const
{
  return (std::is_permutation(resolutionsavailable_.begin(), resolutionsavailable_.end(), rhs.resolutionsavailable_.begin(), rhs.resolutionsavailable_.end()) && (govlengthrange_ == rhs.govlengthrange_) && (frameraterange_ == rhs.frameraterange_) && (encodingintervalrange_ == rhs.encodingintervalrange_) && (std::is_permutation(mpeg4profilessupported_.begin(), mpeg4profilessupported_.end(), rhs.mpeg4profilessupported_.begin(), rhs.mpeg4profilessupported_.end())));
}

}
