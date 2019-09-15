// ptzfilter.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

PTZFilter::PTZFilter()
{

}

PTZFilter::PTZFilter(const boost::optional<bool>& status, const boost::optional<bool>& position) :
  status_(status),
  position_(position)
{

}

PTZFilter::PTZFilter(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Status")
    {
      status_ = element.text().as_bool();

    }
    else if (name == "Position")
    {
      position_ = element.text().as_bool();

    }
  }
}

std::string PTZFilter::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Status", status_) + onvif::ToXml("tt:Position", position_) + "</"+name+">");
}

bool PTZFilter::operator==(const PTZFilter& rhs) const
{
  return ((status_ == rhs.status_) && (position_ == rhs.position_));
}

}
