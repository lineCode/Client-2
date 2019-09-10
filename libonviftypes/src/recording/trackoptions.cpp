// trackoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

TrackOptions::TrackOptions()
{

}

TrackOptions::TrackOptions(const boost::optional<int>& sparetotal, const boost::optional<int>& sparevideo, const boost::optional<int>& spareaudio, const boost::optional<int>& sparemetadata) :
  sparetotal_(sparetotal),
  sparevideo_(sparevideo),
  spareaudio_(spareaudio),
  sparemetadata_(sparemetadata)
{

}

TrackOptions::TrackOptions(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    std::string name = TrimNamespace(attribute.name());
    if (name == "SpareTotal")
    {
      sparetotal_ = attribute.as_int();

    }
    else if (name == "SpareVideo")
    {
      sparevideo_ = attribute.as_int();

    }
    else if (name == "SpareAudio")
    {
      spareaudio_ = attribute.as_int();

    }
    else if (name == "SpareMetadata")
    {
      sparemetadata_ = attribute.as_int();

    }
  }
}

std::string TrackOptions::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("SpareTotal", sparetotal_) + ToXmlAttribute("SpareVideo", sparevideo_) + ToXmlAttribute("SpareAudio", spareaudio_) + ToXmlAttribute("SpareMetadata", sparemetadata_) + "></"+name+">");
}

bool TrackOptions::operator==(const TrackOptions& rhs) const
{
  return ((sparetotal_ == rhs.sparetotal_) && (sparevideo_ == rhs.sparevideo_) && (spareaudio_ == rhs.spareaudio_) && (sparemetadata_ == rhs.sparemetadata_));
}

}

}
