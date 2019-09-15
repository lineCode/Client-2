// dot11pskset.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Dot11PSKSet::Dot11PSKSet()
{

}

Dot11PSKSet::Dot11PSKSet(const boost::optional<std::string>& key, const boost::optional<std::string>& passphrase) :
  key_(key),
  passphrase_(passphrase)
{

}

Dot11PSKSet::Dot11PSKSet(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "According")
    {
      key_ = element.text().get();

    }
    else if (name == "Passphrase")
    {
      passphrase_ = element.text().get();

    }
  }
}

std::string Dot11PSKSet::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:According", key_) + onvif::ToXml("tt:Passphrase", passphrase_) + "</"+name+">");
}

bool Dot11PSKSet::operator==(const Dot11PSKSet& rhs) const
{
  return ((key_ == rhs.key_) && (passphrase_ == rhs.passphrase_));
}

}
