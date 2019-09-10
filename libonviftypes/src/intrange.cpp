// intrange.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

IntRange::IntRange()
{

}

IntRange::IntRange(const boost::optional<int>& min, const boost::optional<int>& max) :
  min_(min),
  max_(max)
{

}

IntRange::IntRange(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Min")
    {
      min_ = element.text().as_int();

    }
    else if (name == "Max")
    {
      max_ = element.text().as_int();

    }
  }
}

std::string IntRange::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Min", min_) + onvif::ToXml("tt:Max", max_) + "</"+name+">");
}

bool IntRange::operator==(const IntRange& rhs) const
{
  return ((min_ == rhs.min_) && (max_ == rhs.max_));
}

}
