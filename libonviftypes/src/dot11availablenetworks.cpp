// dot11availablenetworks.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Dot11AvailableNetworks::Dot11AvailableNetworks()
{

}

Dot11AvailableNetworks::Dot11AvailableNetworks(const boost::optional<std::string>& ssid, const boost::optional<std::string>& bssid, const std::vector<DOT11AUTHANDMANAGEMENTSUITE>& authandmanagementsuite, const boost::optional<DOT11CIPHER>& paircipher, const boost::optional<DOT11CIPHER>& groupcipher, const boost::optional<DOT11SIGNALSTRENGTH>& signalstrength) :
  ssid_(ssid),
  bssid_(bssid),
  authandmanagementsuite_(authandmanagementsuite),
  paircipher_(paircipher),
  groupcipher_(groupcipher),
  signalstrength_(signalstrength)
{

}

Dot11AvailableNetworks::Dot11AvailableNetworks(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SSID")
    {
      ssid_ = element.text().get();

    }
    else if (name == "BSSID")
    {
      bssid_ = element.text().get();

    }
    else if (name == "AuthAndMangementSuite")
    {
      const boost::optional<DOT11AUTHANDMANAGEMENTSUITE> dot11authandmanagementsuite = GetDot11AuthAndManagementSuite(element.text().get());
      if (!dot11authandmanagementsuite.is_initialized())
      {

        continue;
      }

      authandmanagementsuite_.push_back(*dot11authandmanagementsuite);

    }
    else if (name == "PairCipher")
    {
      paircipher_ = GetDot11Cipher(element.text().get());

    }
    else if (name == "GroupCipher")
    {
      groupcipher_ = GetDot11Cipher(element.text().get());

    }
    else if (name == "SignalStrength")
    {
      signalstrength_ = GetDot11SignalStrength(element.text().get());

    }
  }
}

std::string Dot11AvailableNetworks::ToXml(const std::string& name) const
{
  std::string authandmanagementsuites;
  for (const auto& authandmanagementsuite : authandmanagementsuite_)
  {
    authandmanagementsuites += onvif::ToXmlEnum("tt:AuthAndMangementSuite", authandmanagementsuite);

  }
  
  return ("<"+name+">" + onvif::ToXml("tt:SSID", ssid_) + onvif::ToXml("tt:BSSID", bssid_) + authandmanagementsuites + onvif::ToXmlEnum("tt:PairCipher", paircipher_) + onvif::ToXmlEnum("tt:GroupCipher", groupcipher_) + onvif::ToXmlEnum("tt:SignalStrength", signalstrength_) + "</"+name+">");
}

bool Dot11AvailableNetworks::operator==(const Dot11AvailableNetworks& rhs) const
{
  return ((ssid_ == rhs.ssid_) && (bssid_ == rhs.bssid_) && std::is_permutation(authandmanagementsuite_.begin(), authandmanagementsuite_.end(), rhs.authandmanagementsuite_.begin(), rhs.authandmanagementsuite_.end()) && (paircipher_ == rhs.paircipher_) && (groupcipher_ == rhs.groupcipher_) && (signalstrength_ == rhs.signalstrength_));
}

}
