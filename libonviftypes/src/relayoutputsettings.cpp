// relayoutputsettings.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

RelayOutputSettings::RelayOutputSettings()
{

}

RelayOutputSettings::RelayOutputSettings(const boost::optional<RELAYMODE>& mode, const boost::optional<Duration>& delaytime, const boost::optional<RELAYIDLESTATE>& idlestate) :
  mode_(mode),
  delaytime_(delaytime),
  idlestate_(idlestate)
{

}

RelayOutputSettings::RelayOutputSettings(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      mode_ = GetRelayMode(element.text().get());

    }
    else if (name == "DelayTime")
    {
      delaytime_ = GetDuration(element.text().get());

    }
    else if (name == "IdleState")
    {
      idlestate_ = GetRelayIdleState(element.text().get());

    }
  }
}

std::string RelayOutputSettings::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("Mode", mode_) + onvif::ToXml("DelayTime", delaytime_) + onvif::ToXmlEnum("IdleState", idlestate_) + "</"+name+">");
}

bool RelayOutputSettings::operator==(const RelayOutputSettings& rhs) const
{
  return ((mode_ == rhs.mode_) && (delaytime_ == rhs.delaytime_) && (idlestate_ == rhs.idlestate_));
}

}
