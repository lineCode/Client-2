// capabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace search
{

///// Classes /////

Capabilities::Capabilities()
{

}

Capabilities::Capabilities(const boost::optional<bool>& metadatasearch, const boost::optional<bool>& generalstartevents) :
  metadatasearch_(metadatasearch),
  generalstartevents_(generalstartevents)
{

}

Capabilities::Capabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "MetadataSearch")
    {
      metadatasearch_ = attribute.as_bool();
      
    }
    else if (name == "GeneralStartEvents")
    {
      generalstartevents_ = attribute.as_bool();

    }
  }
}

std::string Capabilities::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("MetadataSearch", metadatasearch_) + ToXmlAttribute("GeneralStartEvents", generalstartevents_) + "></"+name+">");
}

Element Capabilities::ToElement(const std::string& name) const
{
  std::vector< std::pair<std::string, std::string> > attributes;
  if (metadatasearch_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("MetadataSearch"), onvif::ToString(*metadatasearch_)));

  }

  if (generalstartevents_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("GeneralStartEvents"), onvif::ToString(*generalstartevents_)));

  }

  return Element(name, attributes, std::string(), {});
}

bool Capabilities::operator==(const Capabilities& rhs) const
{
  return ((metadatasearch_ == rhs.metadatasearch_) && (generalstartevents_ == rhs.generalstartevents_));
}

}

}
