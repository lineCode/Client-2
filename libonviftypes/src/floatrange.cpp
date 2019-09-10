// floatrange.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

FloatRange::FloatRange()
{

}

FloatRange::FloatRange(const boost::optional<float>& min, const boost::optional<float>& max) :
  min_(min),
  max_(max)
{

}

FloatRange::FloatRange(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Min")
    {
      min_ = element.text().as_float();

    }
    else if (name == "Max")
    {
      max_ = element.text().as_float();

    }
  }
}

std::string FloatRange::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Min", min_) + onvif::ToXml("tt:Max", max_) + "</"+name+">");
}

bool FloatRange::operator==(const FloatRange& rhs) const
{
  return ((min_ == rhs.min_) && (max_ == rhs.max_));
}

}
