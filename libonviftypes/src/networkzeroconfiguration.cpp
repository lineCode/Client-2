// networkzeroconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

NetworkZeroConfiguration::NetworkZeroConfiguration()
{

}

NetworkZeroConfiguration::NetworkZeroConfiguration(const boost::optional<std::string>& interfacetoken, const boost::optional<bool>& enabled, const std::vector<std::string>& addresses, const std::vector<NetworkZeroConfiguration>& additional) :
  interfacetoken_(interfacetoken),
  enabled_(enabled),
  addresses_(addresses),
  additional_(additional)
{

}

NetworkZeroConfiguration::NetworkZeroConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "InterfaceToken")
    {
      interfacetoken_ = element.text().get();

    }
    else if (name == "Enabled")
    {
      enabled_ = element.text().as_bool();

    }
    else if (name == "Addresses")
    {
      addresses_.push_back(element.text().get());

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "Additional")
        {
          additional_.push_back(NetworkZeroConfiguration(extension));

        }
      }
    }
  }
}

std::string NetworkZeroConfiguration::ToXml(const std::string& name) const
{
  std::string addresses;
  for (const auto& address : addresses_)
  {
    addresses += onvif::ToXml("tt:Addresses", boost::optional<std::string>(address));

  }
  
  std::string additionals;
  for (const auto& additional : additional_)
  {
    additionals += additional.ToXml("tt:Additional");

  }
  
  return ("<"+name+">" + onvif::ToXml("tt:InterfaceToken", interfacetoken_) + onvif::ToXml("tt:Enabled", enabled_) + addresses + std::string("<tt:Extension>") + additionals + "</tt:Extension></"+name+">");
}

boost::optional<NetworkZeroConfiguration> NetworkZeroConfiguration::GetNetworkZeroConfiguration(const std::string& interfacetoken) const
{
  if (interfacetoken_ == interfacetoken)
  {

    return *this;
  }

  for (const auto& additional : additional_)
  {
    const boost::optional<NetworkZeroConfiguration> networkzeroconfiguration = GetNetworkZeroConfiguration(interfacetoken);
    if (networkzeroconfiguration.is_initialized())
    {

      return networkzeroconfiguration;
    }
  }

  return boost::none;
}

bool NetworkZeroConfiguration::operator==(const NetworkZeroConfiguration& rhs) const
{
  return ((interfacetoken_ == rhs.interfacetoken_) && (enabled_ == rhs.enabled_) && std::is_permutation(addresses_.begin(), addresses_.end(), rhs.addresses_.begin(), rhs.addresses_.end()) && std::is_permutation(additional_.begin(), additional_.end(), rhs.additional_.begin(), rhs.additional_.end()));
}

}
