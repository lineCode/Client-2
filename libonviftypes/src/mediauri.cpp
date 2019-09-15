// mediauri.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

MediaUri::MediaUri()
{

}

MediaUri::MediaUri(const boost::optional<std::string>& uri, const boost::optional<bool>& invalidateafterconnect, const boost::optional<bool>& invalidateafterreboot, const boost::optional<Duration>& timeout) :
  uri_(uri),
  invalidateafterconnect_(invalidateafterconnect),
  invalidateafterreboot_(invalidateafterreboot),
  timeout_(timeout)
{

}

MediaUri::MediaUri(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Uri")
    {
      uri_ = element.text().get();

    }
    else if (name == "InvalidAfterConnect")
    {
      invalidateafterconnect_ = element.text().as_bool();

    }
    else if (name == "InvalidAfterReboot")
    {
      invalidateafterreboot_ = element.text().as_bool();

    }
    else if (name == "Timeout")
    {
      timeout_ = GetDuration(element.text().get());

    }
  }
}

std::string MediaUri::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Uri", uri_) + onvif::ToXml("tt:InvalidAfterConnect", invalidateafterconnect_) + onvif::ToXml("tt:InvalidAfterReboot", invalidateafterreboot_) + onvif::ToXml("tt:Timeout", timeout_) + "</"+name+">");
}

bool MediaUri::operator==(const MediaUri& rhs) const
{
  return ((uri_ == rhs.uri_) && (invalidateafterconnect_ == rhs.invalidateafterconnect_) && (invalidateafterreboot_ == rhs.invalidateafterreboot_) && (timeout_ == rhs.timeout_));
}

}
