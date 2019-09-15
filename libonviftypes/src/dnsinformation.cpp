// dnsinformation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

DNSInformation::DNSInformation()
{

}

DNSInformation::DNSInformation(const boost::optional<bool>& fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<IPAddress>& dnsfromdhcp, const std::vector<IPAddress>& dnsmanual) :
  fromdhcp_(fromdhcp),
  searchdomain_(searchdomain),
  dnsfromdhcp_(dnsfromdhcp),
  dnsmanual_(dnsmanual)
{

}

DNSInformation::DNSInformation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "FromDHCP")
    {
      fromdhcp_ = element.text().as_bool();

    }
    else if (name == "SearchDomain")
    {
      searchdomain_.push_back(element.text().get());

    }
    else if (name == "DNSFromDHCP")
    {
      dnsfromdhcp_.push_back(IPAddress(element));

    }
    else if (name == "DNSManual")
    {
      dnsmanual_.push_back(IPAddress(element));

    }
  }
}

std::string DNSInformation::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:FromDHCP", fromdhcp_) + onvif::ToXml("tt:SearchDomain", searchdomain_) + onvif::ToXml("tt:DNSFromDHCP", dnsfromdhcp_) + onvif::ToXml("tt:DNSManual", dnsmanual_) + "</"+name+">");
}

bool DNSInformation::operator==(const DNSInformation& rhs) const
{
  return ((fromdhcp_ == rhs.fromdhcp_) && (searchdomain_ == rhs.searchdomain_) && (dnsfromdhcp_ == rhs.dnsfromdhcp_) && (dnsmanual_ == rhs.dnsmanual_));
}

}
