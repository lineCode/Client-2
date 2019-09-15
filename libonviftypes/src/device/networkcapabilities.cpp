// networkcapabilities.cpp
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

NetworkCapabilities::NetworkCapabilities()
{

}

NetworkCapabilities::NetworkCapabilities(const boost::optional<bool>& ipfilter, const boost::optional<bool>& zeroconfiguration, const boost::optional<bool>& ipversion6, const boost::optional<bool>& dynamicdns, const boost::optional<bool>& dot11configuration, const boost::optional<int>& dot1xconfigurations, const boost::optional<bool>& hostnamefromdhcp, const boost::optional<int>& ntp, const boost::optional<bool>& dhcpv6) :
  ipfilter_(ipfilter),
  zeroconfiguration_(zeroconfiguration),
  ipversion6_(ipversion6),
  dynamicdns_(dynamicdns),
  dot11configuration_(dot11configuration),
  dot1xconfigurations_(dot1xconfigurations),
  hostnamefromdhcp_(hostnamefromdhcp),
  ntp_(ntp),
  dhcpv6_(dhcpv6)
{

}

NetworkCapabilities::NetworkCapabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "IPFilter")
    {
      ipfilter_ = attribute.as_bool();

    }
    else if (name == "ZeroConfiguration")
    {
      zeroconfiguration_ = attribute.as_bool();

    }
    else if (name == "IPVersion6")
    {
      ipversion6_ = attribute.as_bool();

    }
    else if (name == "DynDNS")
    {
      dynamicdns_ = attribute.as_bool();

    }
    else if (name == "Dot11Configuration")
    {
      dot11configuration_ = attribute.as_bool();

    }
    else if (name == "Dot1XConfigurations")
    {
      dot1xconfigurations_ = attribute.as_int();

    }
    else if (name == "HostnameFromDHCP")
    {
      hostnamefromdhcp_ = attribute.as_bool();

    }
    else if (name == "NTP")
    {
      ntp_ = attribute.as_int();

    }
    else if (name == "DHCPv6")
    {
      dhcpv6_ = attribute.as_bool();

    }
  }
}

std::string NetworkCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+ onvif::ToXmlAttribute("IPFilter", ipfilter_) + onvif::ToXmlAttribute("ZeroConfiguration", zeroconfiguration_) + onvif::ToXmlAttribute("IPVersion6", ipversion6_) + onvif::ToXmlAttribute("DynDNS", dynamicdns_) + onvif::ToXmlAttribute("Dot11Configuration", dot11configuration_) + onvif::ToXmlAttribute("Dot1XConfigurations", dot1xconfigurations_) + onvif::ToXmlAttribute("HostnameFromDHCP", hostnamefromdhcp_) + onvif::ToXmlAttribute("NTP", ntp_) + onvif::ToXmlAttribute("DHCPv6", dhcpv6_) + "/>");
}

Element NetworkCapabilities::ToElement(const std::string& name) const
{
  std::vector< std::pair<std::string, std::string> > attributes;
  if (ipfilter_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("IPFilter"), ToString(*ipfilter_)));

  }

  if (zeroconfiguration_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("ZeroConfiguration"), ToString(*zeroconfiguration_)));

  }

  if (ipversion6_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("IPVersion6"), ToString(*ipversion6_)));

  }

  if (dynamicdns_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("DynDNS"), ToString(*dynamicdns_)));

  }

  if (dot11configuration_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("Dot11Configuration"), ToString(*dot11configuration_)));

  }

  if (dot1xconfigurations_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("Dot1XConfigurations"), std::to_string(*dot1xconfigurations_)));

  }

  if (hostnamefromdhcp_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("HostnameFromDHCP"), ToString(*hostnamefromdhcp_)));

  }

  if (ntp_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("NTP"), std::to_string(*ntp_)));

  }

  if (dhcpv6_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("DHCPv6"), ToString(*dhcpv6_)));

  }

  return Element(name, attributes, std::string(), { });
}

bool NetworkCapabilities::operator==(const NetworkCapabilities& rhs) const
{
  return ((ipfilter_ == rhs.ipfilter_) && (zeroconfiguration_ == rhs.zeroconfiguration_) && (ipversion6_ == rhs.ipversion6_) && (dynamicdns_ == rhs.dynamicdns_) && (dot11configuration_ == rhs.dot11configuration_) && (dot1xconfigurations_ == rhs.dot1xconfigurations_) && (hostnamefromdhcp_ == rhs.hostnamefromdhcp_) && (ntp_ == rhs.ntp_) && (dhcpv6_ == rhs.dhcpv6_));
}

}

}
