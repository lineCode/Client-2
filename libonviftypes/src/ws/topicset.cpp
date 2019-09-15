// topicset.cpp
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

TopicSet::TopicSet()
{

}

TopicSet::TopicSet(const Device& device) :
  device_(device)
{

}

TopicSet::TopicSet(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Device")
    {
      device_ = Device(element);

    }
  }
}

std::string TopicSet::ToXml(const std::string& name) const
{
  return std::string("<" + name + ">" + device_.ToXml(std::string("tns1:Device")) + "</" + name + ">");
}

bool TopicSet::operator==(const TopicSet& rhs) const
{
  return (device_ == rhs.device_);
}

}

}
