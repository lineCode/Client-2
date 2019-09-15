// endpointreferencetype.cpp
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

EndpointReferenceType::EndpointReferenceType()
{

}

EndpointReferenceType::EndpointReferenceType(const std::string& address, const Element& referenceparameters) :
  address_(address),
  referenceparameters_(referenceparameters)
{

}

EndpointReferenceType::EndpointReferenceType(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Address")
    {
      address_ = element.text().get();

    }
    else if (name == "ReferenceParameters")
    {
      referenceparameters_ = Element(element);

    }
  }
}

std::string EndpointReferenceType::ToXml(const std::string& name) const
{
  return std::string("<" + name + ">" + onvif::ToXml("wsa:Address", boost::optional<std::string>(address_)) + referenceparameters_.ToXml() + "</" + name + ">");
}

bool EndpointReferenceType::operator==(const EndpointReferenceType& rhs) const
{
  return ((address_ == rhs.address_) && (referenceparameters_ == rhs.referenceparameters_));
}

}

}
