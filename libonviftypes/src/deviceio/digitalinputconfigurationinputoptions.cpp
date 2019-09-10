// digitalinputconfigurationinputoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Classes /////

DigitalInputConfigurationInputOptions::DigitalInputConfigurationInputOptions()
{

}

DigitalInputConfigurationInputOptions::DigitalInputConfigurationInputOptions(const std::vector<DIGITALIDLESTATE>& idlestate) :
  idlestate_(idlestate)
{

}

DigitalInputConfigurationInputOptions::DigitalInputConfigurationInputOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "IdleState")
    {
      const boost::optional<DIGITALIDLESTATE> idlestate = GetDigitalIdleState(element.text().get());
      if (idlestate.is_initialized())
      {
        idlestate_.push_back(*idlestate);

      }
    }
  }
}

std::string DigitalInputConfigurationInputOptions::ToXml(const std::string& name) const
{
  std::string idlestates;
  for (const auto& idlestate : idlestate_)
  {
    idlestates += onvif::ToXmlEnum("tt:IdleState", idlestate);

  }

  return ("<" + name + ">" + idlestates + "</" + name + ">");
}

bool DigitalInputConfigurationInputOptions::operator==(const DigitalInputConfigurationInputOptions& rhs) const
{
  return (idlestate_ == rhs.idlestate_);
}

}

}
