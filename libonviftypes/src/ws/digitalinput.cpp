// digitalinput.cpp
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

DigitalInput::DigitalInput()
{

}

DigitalInput::DigitalInput(const MessageDescription& messagedescription) :
  messagedescription_(messagedescription)
{

}

DigitalInput::DigitalInput(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "MessageDescription")
    {
      messagedescription_ = MessageDescription(element);

    }
  }
}

std::string DigitalInput::ToXml(const std::string& name) const
{
  return std::string("<" + name + " wstop:topic=\"true\">" + messagedescription_.ToXml("tt:MessageDescription") + "</" + name + ">");
}

bool DigitalInput::operator==(const DigitalInput& rhs) const
{
  return (messagedescription_ == rhs.messagedescription_);
}

}

}
