// imagingcapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

ImagingCapabilities::ImagingCapabilities()
{

}

ImagingCapabilities::ImagingCapabilities(const boost::optional<std::string>& xaddr) :
  xaddr_(xaddr)
{

}

ImagingCapabilities::ImagingCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "XAddr")
    {
      xaddr_ = element.text().get();

    }
  }
}

std::string ImagingCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + "</"+name+">");
}

bool ImagingCapabilities::operator==(const ImagingCapabilities& rhs) const
{
  return (xaddr_ == rhs.xaddr_);
}

}
