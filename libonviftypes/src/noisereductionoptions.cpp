// noisereductionoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

NoiseReductionOptions::NoiseReductionOptions()
{

}

NoiseReductionOptions::NoiseReductionOptions(const boost::optional<bool>& level) :
  level_(level)
{

}

NoiseReductionOptions::NoiseReductionOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Level")
    {
      level_ = element.text().as_bool();

    }
  }
}

std::string NoiseReductionOptions::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Level", level_) + "</"+name+">");
}

bool NoiseReductionOptions::operator==(const NoiseReductionOptions& rhs) const
{
  return (level_ == rhs.level_);
}

}
