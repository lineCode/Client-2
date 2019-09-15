// device.cpp
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

Device::Device()
{

}

Device::Device(const Trigger& trigger) :
  trigger_(trigger)
{

}

Device::Device(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Trigger")
    {
      trigger_ = Trigger(element);

    }
  }
}

std::string Device::ToXml(const std::string& name) const
{
  return std::string("<" + name + " wstop:topic=\"true\">" + trigger_.ToXml("Trigger") + "</" + name + ">");
}

bool Device::operator==(const Device& rhs) const
{
  return (trigger_ == rhs.trigger_);
}

}

}
