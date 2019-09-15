// dot11capabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Dot11Capabilities::Dot11Capabilities()
{

}

Dot11Capabilities::Dot11Capabilities(const boost::optional<bool>& tkip, const boost::optional<bool>& scanavailablenetworks, const boost::optional<bool>& multipleconfiguration, const boost::optional<bool>& adhocstationmode, const boost::optional<bool>& wep) :
  tkip_(tkip),
  scanavailablenetworks_(scanavailablenetworks),
  multipleconfiguration_(multipleconfiguration), 
  adhocstationmode_(adhocstationmode),
  wep_(wep)
{

}

Dot11Capabilities::Dot11Capabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "TKIP")
    {
      tkip_ = element.text().as_bool();

    }
    else if (name == "ScanAvailableNetworks")
    {
      scanavailablenetworks_ = element.text().as_bool();

    }
    else if (name == "MultipleConfiguration")
    {
      multipleconfiguration_ = element.text().as_bool();

    }
    else if (name == "AdHocStationMode")
    {
      adhocstationmode_ = element.text().as_bool();

    }
    else if (name == "WEP")
    {
      wep_ = element.text().as_bool();

    }
  }
}

std::string Dot11Capabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:TKIP", tkip_) + onvif::ToXml("tt:ScanAvailableNetworks", scanavailablenetworks_) + onvif::ToXml("tt:MultipleConfiguration", multipleconfiguration_) + onvif::ToXml("tt:AdHocStationMode", adhocstationmode_) + onvif::ToXml("tt:WEP", wep_) + "</"+name+">");
}

bool Dot11Capabilities::operator==(const Dot11Capabilities& rhs) const
{
  return ((tkip_ == rhs.tkip_) && (scanavailablenetworks_ == rhs.scanavailablenetworks_) && (multipleconfiguration_ == rhs.multipleconfiguration_) && (adhocstationmode_ == rhs.adhocstationmode_) && (wep_ == rhs.wep_));
}

}
