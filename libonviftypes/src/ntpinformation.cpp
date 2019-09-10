// ntpinformation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

NTPInformation::NTPInformation()
{

}

NTPInformation::NTPInformation(const boost::optional<bool>& fromdhcp, const std::vector<NetworkHost>& ntpfromdhcp, const std::vector<NetworkHost>& ntpmanual) :
  fromdhcp_(fromdhcp),
  ntpfromdhcp_(ntpfromdhcp),
  ntpmanual_(ntpmanual)
{

}

NTPInformation::NTPInformation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "FromDHCP")
    {
      fromdhcp_ = element.text().as_bool();

    }
    else if (name == "NTPFromDHCP")
    {
      ntpfromdhcp_.push_back(NetworkHost(element));

    }
    else if (name == "NTPManual")
    {
      ntpmanual_.push_back(NetworkHost(element));

    }
  }
}

std::string NTPInformation::ToXml(const std::string& name) const
{
  std::string ntpfromdhcps;
  for (const auto& ntpfromdhcp : ntpfromdhcp_)
  {
    ntpfromdhcps += ntpfromdhcp.ToXml("tt:NTPFromDHCP");

  }
  
  std::string ntpmanuals;
  for (const auto& ntpmanual : ntpmanual_)
  {
    ntpmanuals += ntpmanual.ToXml("tt:NTPManual");

  }
  
  return ("<"+name+">" + onvif::ToXml("tt:FromDHCP", fromdhcp_) + ntpfromdhcps + ntpmanuals + "</"+name+">");
}

bool NTPInformation::operator==(const NTPInformation& rhs) const
{
  return ((fromdhcp_ == rhs.fromdhcp_) && std::is_permutation(ntpfromdhcp_.begin(), ntpfromdhcp_.end(), rhs.ntpfromdhcp_.begin(), rhs.ntpfromdhcp_.end()) && std::is_permutation(ntpmanual_.begin(), ntpmanual_.end(), rhs.ntpmanual_.begin(), rhs.ntpmanual_.end()));
}

}
