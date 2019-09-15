// durationrange.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

DurationRange::DurationRange()
{

}

DurationRange::DurationRange(const boost::optional<Duration>& min, const boost::optional<Duration>& max) :
  min_(min),
  max_(max)
{

}

DurationRange::DurationRange(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Min")
    {
      min_ = GetDuration(element.text().get());

    }
    else if (name == "Max")
    {
      max_ = GetDuration(element.text().get());

    }
  }
}

std::string DurationRange::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Min", min_) + onvif::ToXml("tt:Max", max_) + "</"+name+">");
}

bool DurationRange::operator==(const DurationRange& rhs) const
{
  return ((min_ == rhs.min_) && (max_ == rhs.max_));
}

}
