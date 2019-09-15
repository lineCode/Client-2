// networkcapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

NetworkCapabilities::NetworkCapabilities()
{
  
}

NetworkCapabilities::NetworkCapabilities(const boost::optional<bool>& ipfilter, const boost::optional<bool>& zeroconfiguration, const boost::optional<bool>& ipversion6, const boost::optional<bool>& dynamicdns, const boost::optional<bool>& dot11configuration) :
  ipfilter_(ipfilter),
  zeroconfiguration_(zeroconfiguration),
  ipversion6_(ipversion6),
  dynamicdns_(dynamicdns),
  dot11configuration_(dot11configuration)
{
  
}

NetworkCapabilities::NetworkCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "IPFilter")
    {
      ipfilter_ = element.text().as_bool();

    }
    else if (name == "ZeroConfiguration")
    {
      zeroconfiguration_ = element.text().as_bool();

    }
    else if (name == "IPVersion6")
    {
      ipversion6_ = element.text().as_bool();

    }
    else if (name == "DynDNS")
    {
      dynamicdns_ = element.text().as_bool();

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "Dot11Configuration")
        {
          dot11configuration_ = extension.text().as_bool();

        }
      }
    }
  }
}

std::string NetworkCapabilities::ToXml(const std::string& name) const
{
  std::string extension;
  if (dot11configuration_.is_initialized())
  {
    extension = "<tt:Extension>" + onvif::ToXml("tt:Dot11Configuration", dot11configuration_) + "</tt:Extension>";

  }

  return ("<"+name+">" + onvif::ToXml("tt:IPFilter", ipfilter_) + onvif::ToXml("tt:ZeroConfiguration", zeroconfiguration_) + onvif::ToXml("tt:IPVersion6", ipversion6_) + onvif::ToXml("tt:DynDNS", dynamicdns_) + extension + "</"+name+">");
}

bool NetworkCapabilities::operator==(const NetworkCapabilities& rhs) const
{
  return ((ipfilter_ == rhs.ipfilter_) && (zeroconfiguration_ == rhs.zeroconfiguration_) && (ipversion6_ == rhs.ipversion6_) && (dynamicdns_ == rhs.dynamicdns_) && (dot11configuration_ == rhs.dot11configuration_));
}

}
