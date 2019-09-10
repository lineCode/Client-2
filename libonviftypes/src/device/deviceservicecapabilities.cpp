// deviceservicecapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Classes /////

DeviceServiceCapabilities::DeviceServiceCapabilities()
{
  
}

DeviceServiceCapabilities::DeviceServiceCapabilities(const boost::optional<NetworkCapabilities>& network, const boost::optional<SecurityCapabilities>& security, const boost::optional<SystemCapabilities>& system, const boost::optional<MiscCapabilities>& misc) :
  network_(network),
  security_(security),
  system_(system),
  misc_(misc)
{
  
}

DeviceServiceCapabilities::DeviceServiceCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Network")
    {
      network_ = NetworkCapabilities(element);

    }
    else if (name == "Security")
    {
      security_ = SecurityCapabilities(element);

    }
    else if (name == "System")
    {
      system_ = SystemCapabilities(element);

    }
    else if (name == "Misc")
    {
      misc_ = MiscCapabilities(element);

    }
  }
}

std::string DeviceServiceCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlClass("tds:Network", network_) + onvif::ToXmlClass("tds:Security", security_) + onvif::ToXmlClass("tds:System", system_) + onvif::ToXmlClass("tds:Misc", misc_) + "</"+name+">");
}

Element DeviceServiceCapabilities::ToElement(const std::string& name) const
{
  std::vector<Element> elements;
  if (network_.is_initialized())
  {
    elements.push_back(network_->ToElement("tds:Network"));

  }

  if (security_.is_initialized())
  {
    elements.push_back(security_->ToElement("tds:Security"));

  }

  if (system_.is_initialized())
  {
    elements.push_back(system_->ToElement("tds:System"));

  }

  if (misc_.is_initialized())
  {
    elements.push_back(misc_->ToElement("tds:Misc"));

  }

  return Element(name, { }, std::string(), elements);
}

bool DeviceServiceCapabilities::operator==(const DeviceServiceCapabilities& rhs) const
{
  return ((network_ == rhs.network_) && (security_ == rhs.security_) && (system_ == rhs.system_) && (misc_ == rhs.misc_));
}

}

}
