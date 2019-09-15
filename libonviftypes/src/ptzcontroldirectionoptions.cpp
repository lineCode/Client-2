// ptzcontroldirectionoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

PTZControlDirectionOptions::PTZControlDirectionOptions()
{

}

PTZControlDirectionOptions::PTZControlDirectionOptions(const boost::optional<EFLIPMODE>& eflip, const boost::optional<REVERSEMODE>& reverse) :
  eflip_(eflip),
  reverse_(reverse)
{

}

PTZControlDirectionOptions::PTZControlDirectionOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "EFlip")
    {
      eflip_ = GetEFlipMode(element.text().get());

    }
    else if (name == "Reverse")
    {
      reverse_ = GetReverseMode(element.text().get());

    }
  }
}

std::string PTZControlDirectionOptions::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:EFlip", eflip_) + onvif::ToXmlEnum("tt:Reverse", reverse_) + "</"+name+">");
}

bool PTZControlDirectionOptions::operator==(const PTZControlDirectionOptions& rhs) const
{
  return ((eflip_ == rhs.eflip_) && (reverse_ == rhs.reverse_));
}

}
