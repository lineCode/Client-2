// misccapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Classes /////

MiscCapabilities::MiscCapabilities()
{
  
}

MiscCapabilities::MiscCapabilities(const boost::optional<StringAttrList>& auxiliarycommands) :
  auxiliarycommands_(auxiliarycommands)
{
  
}

MiscCapabilities::MiscCapabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "AuxiliaryCommands")
    {
      auxiliarycommands_ = StringAttrList(attribute.value());

    }
  }
}

std::string MiscCapabilities::ToXml(const std::string& name) const
{
  std::string auxiliarycommands;
  if (auxiliarycommands_.is_initialized())
  {
    auxiliarycommands = std::string(" tds:AuxiliaryCommands=\"") + auxiliarycommands_->ToString() + "\"";

  }

  return ("<"+name + auxiliarycommands + "/>");
}

Element MiscCapabilities::ToElement(const std::string& name) const
{
  std::vector< std::pair<std::string, std::string> > attributes;
  if (auxiliarycommands_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("tds:AuxiliaryCommands"), auxiliarycommands_->ToString()));

  }

  return Element(name, attributes, std::string(), {});
}

bool MiscCapabilities::operator==(const MiscCapabilities& rhs) const
{
  return (auxiliarycommands_ == rhs.auxiliarycommands_);
}

}

}
