// capabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace ptz
{

///// Classes /////

Capabilities::Capabilities()
{

}

Capabilities::Capabilities(const boost::optional<bool>& eflip, const boost::optional<bool>& reverse, const boost::optional<bool>& getcompatibleconfigurations, const boost::optional<bool>& movestatus, const boost::optional<bool>& statusposition) :
  eflip_(eflip),
  reverse_(reverse),
  getcompatibleconfigurations_(getcompatibleconfigurations),
  movestatus_(movestatus),
  statusposition_(statusposition)
{

}

Capabilities::Capabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "EFlip")
    {
      eflip_ = attribute.as_bool();
      
    }
    else if (name == "Reverse")
    {
      reverse_ = attribute.as_bool();

    }
    else if (name == "GetCompatibleConfigurations")
    {
      getcompatibleconfigurations_ = attribute.as_bool();

    }
    else if (name == "MoveStatus")
    {
      movestatus_ = attribute.as_bool();

    }
    else if (name == "StatusPosition")
    {
      statusposition_ = attribute.as_bool();

    }
  }
}

std::string Capabilities::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("EFlip", eflip_) + ToXmlAttribute("Reverse", reverse_) + ToXmlAttribute("GetCompatibleConfigurations", getcompatibleconfigurations_) + ToXmlAttribute("MoveStatus", movestatus_) + ToXmlAttribute("StatusPosition", statusposition_) + "></"+name+">");
}

bool Capabilities::operator==(const Capabilities& rhs) const
{
  return ((eflip_ == rhs.eflip_) && (reverse_ == rhs.reverse_) && (getcompatibleconfigurations_ == rhs.getcompatibleconfigurations_) && (movestatus_ == rhs.movestatus_) && (statusposition_ == rhs.statusposition_));
}

}

}
