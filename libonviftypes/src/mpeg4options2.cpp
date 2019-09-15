// mpeg4options2.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Mpeg4Options2::Mpeg4Options2()
{
  
}

Mpeg4Options2::Mpeg4Options2(const std::vector<VideoResolution>& resolutionsavailable, const boost::optional<IntRange>& govlengthrange, const boost::optional<IntRange>& framerateaverage, const boost::optional<IntRange>& encodingintervalrange, const std::vector<MPEG4PROFILE>& mpeg4profilessupported, const boost::optional<IntRange>& bitraterange) :
  resolutionsavailable_(resolutionsavailable),
  govlengthrange_(govlengthrange),
  framerateaverage_(framerateaverage),
  encodingintervalrange_(encodingintervalrange),
  mpeg4profilessupported_(mpeg4profilessupported),
  bitraterange_(bitraterange)
{
  
}

Mpeg4Options2::Mpeg4Options2(const pugi::xml_node& node)
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
    else if (name == "Mpeg4ProfilesSupported")
    {
      boost::optional<MPEG4PROFILE> mpeg4profile = GetMpeg4Profile(element.text().get());
      if (mpeg4profile.is_initialized())
      {
        mpeg4profilessupported_.push_back(*mpeg4profile);

      }
    }
    else if (name == "BitrateRange")
    {
      bitraterange_ = IntRange(element);

    }
  }
}

std::string Mpeg4Options2::ToXml(const std::string& name) const
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
  
  return ("<"+name+">" + resolutions + ToXmlClass("GovLengthRange", govlengthrange_) + ToXmlClass("FrameRateRange", framerateaverage_) + ToXmlClass("EncodingIntervalRange", encodingintervalrange_) + mpeg4profilessupported + ToXmlClass("BitrateRange", bitraterange_) + "</"+name+">");
}

bool Mpeg4Options2::operator==(const Mpeg4Options2& rhs) const
{
  return (std::is_permutation(resolutionsavailable_.begin(), resolutionsavailable_.end(), rhs.resolutionsavailable_.begin(), rhs.resolutionsavailable_.end()) && (govlengthrange_ == rhs.govlengthrange_) && (framerateaverage_ == rhs.framerateaverage_) && (encodingintervalrange_ == rhs.encodingintervalrange_) && (std::is_permutation(mpeg4profilessupported_.begin(), mpeg4profilessupported_.end(), rhs.mpeg4profilessupported_.begin(), rhs.mpeg4profilessupported_.end())) && (bitraterange_ == rhs.bitraterange_));
}

}
