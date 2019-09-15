// zoomlimits.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

ZoomLimits::ZoomLimits()
{

}

ZoomLimits::ZoomLimits(const boost::optional<Space1dDescription>& range) :
  range_(range)
{

}

ZoomLimits::ZoomLimits(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Range")
    {
      range_ = Space1dDescription(element);

    }
  }
}

std::string ZoomLimits::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:Range", range_) + "</"+name+">");
}

bool ZoomLimits::operator==(const ZoomLimits& rhs) const
{
  return (range_ == rhs.range_);
}

}
