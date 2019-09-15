// searchcapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

SearchCapabilities::SearchCapabilities()
{

}

SearchCapabilities::SearchCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& metadatasearch) :
  xaddr_(xaddr),
  metadatasearch_(metadatasearch)
{

}

SearchCapabilities::SearchCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "XAddr")
    {
      xaddr_ = element.text().get();

    }
    else if (name == "MetadataSearch")
    {
      metadatasearch_ = element.text().as_bool();

    }
  }
}

std::string SearchCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + onvif::ToXml("tt:MetadataSearch", metadatasearch_) + "</"+name+">");
}

bool SearchCapabilities::operator==(const SearchCapabilities& rhs) const
{
  return ((xaddr_ == rhs.xaddr_) && (metadatasearch_ == rhs.metadatasearch_));
}

}
