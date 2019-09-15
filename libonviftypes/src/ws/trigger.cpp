// trigger.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace ws
{

///// Classes /////

Trigger::Trigger()
{

}

Trigger::Trigger(const DigitalInput& digitalinput) :
  digitalinput_(digitalinput)
{

}

Trigger::Trigger(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "DigitalInput")
    {
      digitalinput_ = DigitalInput(element);

    }
  }
}

std::string Trigger::ToXml(const std::string& name) const
{
  return std::string("<" + name + " wstop:topic=\"true\">" + digitalinput_.ToXml("DigitalInput") + "</" + name + ">");
}

bool Trigger::operator==(const Trigger& rhs) const
{
  return (digitalinput_ == rhs.digitalinput_);
}

}

}
