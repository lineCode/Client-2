// devicecapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

DeviceCapabilities::DeviceCapabilities()
{
  
}

DeviceCapabilities::DeviceCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<NetworkCapabilities>& network, const boost::optional<SystemCapabilities>& system, const boost::optional<IOCapabilities>& io, const boost::optional<SecurityCapabilities>& security) :
  xaddr_(xaddr),
  network_(network),
  system_(system),
  io_(io),
  security_(security)
{
  
}

DeviceCapabilities::DeviceCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "XAddr")
    {
      xaddr_ = element.text().get();

    }
    else if (name == "Network")
    {
      network_ = NetworkCapabilities(element);

    }
    else if (name == "System")
    {
      system_ = SystemCapabilities(element);

    }
    else if (name == "IO")
    {
      io_ = IOCapabilities(element);

    }
    else if (name == "Security")
    {
      security_ = SecurityCapabilities(element);

    }
  }
}

std::string DeviceCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + ToXmlClass("tt:Network", network_) + ToXmlClass("tt:System", system_) + ToXmlClass("tt:IO", io_) + ToXmlClass("tt:Security", security_) + "</"+name+">");
}

bool DeviceCapabilities::operator==(const DeviceCapabilities& rhs) const
{
  return ((xaddr_ == rhs.xaddr_) && (network_ == rhs.network_) && (system_ == rhs.system_) && (io_ == rhs.io_) && (security_ == rhs.security_));
}

}
