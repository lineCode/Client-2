// focusmove.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

FocusMove::FocusMove()
{

}

FocusMove::FocusMove(const boost::optional<AbsoluteFocus>& absolute, const boost::optional<RelativeFocus>& relative, const boost::optional<ContinuousFocus>& continuous) :
  absolute_(absolute),
  relative_(relative),
  continuous_(continuous)
{

}

FocusMove::FocusMove(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Absolute")
    {
      absolute_ = AbsoluteFocus(element);

    }
    else if (name == "Relative")
    {
      relative_ = RelativeFocus(element);

    }
    else if (name == "Continuous")
    {
      continuous_ = ContinuousFocus(element);

    }
  }
}

std::string FocusMove::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:Absolute", absolute_) + ToXmlClass("tt:Relative", relative_) + ToXmlClass("tt:Continuous", continuous_) + "</"+name+">");
}

bool FocusMove::operator==(const FocusMove& rhs) const
{
  return ((absolute_ == rhs.absolute_) && (relative_ == rhs.relative_) && (continuous_ == rhs.continuous_));
}

}
