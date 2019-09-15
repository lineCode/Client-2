// moveoptions20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

MoveOptions20::MoveOptions20()
{

}

MoveOptions20::MoveOptions20(const boost::optional<AbsoluteFocusOptions>& absolute, const boost::optional<RelativeFocusOptions20>& relative, const boost::optional<ContinuousFocusOptions>& continuous) :
  absolute_(absolute),
  relative_(relative),
  continuous_(continuous)
{

}

MoveOptions20::MoveOptions20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Absolute")
    {
      absolute_ = AbsoluteFocusOptions(element);

    }
    else if (name == "Relative")
    {
      relative_ = RelativeFocusOptions20(element);

    }
    else if (name == "Continuous")
    {
      continuous_ = ContinuousFocusOptions(element);

    }
  }
}

std::string MoveOptions20::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:Absolute", absolute_) + ToXmlClass("tt:Relative", relative_) + ToXmlClass("tt:Continuous", continuous_) + "</"+name+">");
}

bool MoveOptions20::operator==(const MoveOptions20& rhs) const
{
  return ((absolute_ == rhs.absolute_) && (relative_ == rhs.relative_) && (continuous_ == rhs.continuous_));
}

}
