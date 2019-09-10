// capabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace imaging
{

///// Classes /////

Capabilities::Capabilities()
{

}

Capabilities::Capabilities(const boost::optional<bool>& imagestabilization) :
  imagestabilization_(imagestabilization)
{
  
}

Capabilities::Capabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    std::string name = TrimNamespace(attribute.name());
    if (name == "ImageStabilization")
    {
      imagestabilization_ = attribute.as_bool();

    }
  }
}

std::string Capabilities::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("ImageStabilization", imagestabilization_) + "></"+name+">");
}

bool Capabilities::operator==(const Capabilities& rhs) const
{
  return (imagestabilization_ == rhs.imagestabilization_);
}

}

}
