// service.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Classes /////

Service::Service()
{

}

Service::Service(const std::string& Namespace, const std::string& xaddr, const boost::optional<Element>& capabilities, const OnvifVersion& version) :
  Namespace_(Namespace),
  xaddr_(xaddr),
  capabilities_(capabilities.is_initialized() ? boost::optional<Element>(Element("tds:Capabilities", { }, std::string(), { *capabilities })) : boost::none),
  version_(version)
{

}

Service::Service(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Namespace")
    {
      Namespace_ = element.text().get();

    }
    else if (name == "XAddr")
    {
      xaddr_ = element.text().get();

    }
    else if (name == "Capabilities")
    {
      capabilities_ = Element(element);

    }
    else if (name == "Version")
    {
      version_ = OnvifVersion(element);

    }
  }
}

std::string Service::ToXml(const std::string& name) const
{
  std::string capabilities;
  if (capabilities_.is_initialized())
  {
    capabilities = capabilities_->ToXml();

  }

  return ("<"+name+">" + onvif::ToXml("tds:Namespace", Namespace_) + (onvif::ToXml("tds:XAddr", xaddr_)) + capabilities + ToXmlClass("tds:Version", version_) + "</"+name+">");
}

bool Service::operator==(const Service& rhs) const
{
  return ((Namespace_ == rhs.Namespace_) && (xaddr_ == rhs.xaddr_) && (capabilities_ == rhs.capabilities_) && (version_ == rhs.version_));
}

}

}
