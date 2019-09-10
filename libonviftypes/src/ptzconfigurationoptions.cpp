// ptzconfigurationoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

PTZConfigurationOptions::PTZConfigurationOptions()
{

}

PTZConfigurationOptions::PTZConfigurationOptions(const boost::optional<IntAttrList>& ptzramps, const boost::optional<PTZSpaces>& spaces) :
  ptzramps_(ptzramps),
  spaces_(spaces)
{

}

PTZConfigurationOptions::PTZConfigurationOptions(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "PTZRamps")
    {
      ptzramps_ = IntAttrList(attribute.value());

    }
  }

  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Spaces")
    {
      spaces_ = PTZSpaces(element);

    }
  }
}

std::string PTZConfigurationOptions::ToXml(const std::string& name) const
{
  std::string ptzramps;
  if (ptzramps_.is_initialized())
  {
    ptzramps = std::string(" PTZRamps=\"") + ptzramps_->ToString() + "\"";
  
  }

  return ("<"+name + ptzramps + ">" + ToXmlClass("tt:Spaces", spaces_) + "</"+name+">");
}

bool PTZConfigurationOptions::operator==(const PTZConfigurationOptions& rhs) const
{
  return ((ptzramps_ == rhs.ptzramps_) && (spaces_ == rhs.spaces_));
}

}
