// focusstatus20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

FocusStatus20::FocusStatus20()
{

}

FocusStatus20::FocusStatus20(const boost::optional<float>& xaddr, const boost::optional<MOVESTATUS>& movestatus, const boost::optional<std::string>& error) :
  xaddr_(xaddr),
  movestatus_(movestatus),
  error_(error)
{

}

FocusStatus20::FocusStatus20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Position")
    {
      xaddr_ = element.text().as_float();

    }
    else if (name == "MoveStatus")
    {
      movestatus_ = GetMoveStatus(element.text().get());

    }
    else if (name == "Error")
    {
      error_ = element.text().get();

    }
  }
}

std::string FocusStatus20::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Position", xaddr_) + onvif::ToXml("tt:MoveStatus", xaddr_) + onvif::ToXml("tt:Error", error_) + "</"+name+">");
}

bool FocusStatus20::operator==(const FocusStatus20& rhs) const
{
  return ((xaddr_ == rhs.xaddr_) && (xaddr_ == rhs.xaddr_) && (xaddr_ == rhs.xaddr_));
}

}
