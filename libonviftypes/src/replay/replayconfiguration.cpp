// replayconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace replay
{

///// Classes /////

ReplayConfiguration::ReplayConfiguration()
{

}

ReplayConfiguration::ReplayConfiguration(const boost::optional<Duration> sessiontimeout) :
  sessiontimeout_(sessiontimeout)
{

}

ReplayConfiguration::ReplayConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SessionTimeout")
    {
      sessiontimeout_ = GetDuration(element.text().get());

    }
  }
}

std::string ReplayConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:SessionTimeout", sessiontimeout_) + "</"+name+">");
}

bool ReplayConfiguration::operator==(const ReplayConfiguration& rhs) const
{
  return (sessiontimeout_ == rhs.sessiontimeout_);
}

}

}
