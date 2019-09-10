// receiverconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Classes /////

ReceiverConfiguration::ReceiverConfiguration()
{

}

ReceiverConfiguration::ReceiverConfiguration(const boost::optional<RECEIVERMODE>& mode, const boost::optional<std::string>& mediauri, const boost::optional<StreamSetup>& streamsetup) :
  mode_(mode),
  mediauri_(mediauri),
  streamsetup_(streamsetup)
{

}

ReceiverConfiguration::ReceiverConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Mode")
    {
      mode_ = GetReceiverMode(element.text().get());

    }
    else if (name == "MediaUri")
    {
      mediauri_ = element.text().get();

    }
    else if (name == "StreamSetup")
    {
      streamsetup_ = StreamSetup(element);

    }
  }
}

std::string ReceiverConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Mode", mode_) + onvif::ToXml("tt:MediaUri", mediauri_) + onvif::ToXmlClass("tt:StreamSetup", streamsetup_) + "</"+name+">");
}

bool ReceiverConfiguration::operator==(const ReceiverConfiguration& rhs) const
{
  return ((mode_ == rhs.mode_) && (mediauri_ == rhs.mediauri_) && (streamsetup_ == rhs.streamsetup_));
}

}

}
