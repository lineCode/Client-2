// widedynamicrangeoptions20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

WideDynamicRangeOptions20::WideDynamicRangeOptions20()
{

}

WideDynamicRangeOptions20::WideDynamicRangeOptions20(const std::vector<WIDEDYNAMICMODE>& mode, const boost::optional<FloatRange>& level) :
  mode_(mode),
  level_(level)
{

}

WideDynamicRangeOptions20::WideDynamicRangeOptions20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      boost::optional<WIDEDYNAMICMODE> mode = GetWideDynamicMode(element.text().get());
      if (mode.is_initialized())
      {
        mode_.push_back(*mode);

      }
    }
    else if (name == "Level")
    {
      level_ = FloatRange(element);

    }
  }
}

std::string WideDynamicRangeOptions20::ToXml(const std::string& name) const
{
  std::string modes;
  for (const auto& mode : mode_)
  {
    modes += onvif::ToXmlEnum("tt:Mode", mode);

  }
  
  return ("<"+name+">" + modes + onvif::ToXmlClass("tt:Level", level_) + "</"+name+">");
}

bool WideDynamicRangeOptions20::operator==(const WideDynamicRangeOptions20& rhs) const
{
  return (std::is_permutation(mode_.begin(), mode_.end(), rhs.mode_.begin(), rhs.mode_.end()) && (level_ == rhs.level_));
}

}
