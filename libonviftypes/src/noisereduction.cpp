// noisereduction.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

NoiseReduction::NoiseReduction()
{

}

NoiseReduction::NoiseReduction(const boost::optional<float>& level) :
  level_(level)
{

}

NoiseReduction::NoiseReduction(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Level")
    {
      level_ = element.text().as_float();

    }
  }
}

std::string NoiseReduction::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Level", level_) + "</"+name+">");
}

bool NoiseReduction::operator==(const NoiseReduction& rhs) const
{
  return (level_ == rhs.level_);
}

}
