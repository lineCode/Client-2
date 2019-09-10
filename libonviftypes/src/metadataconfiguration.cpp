// mediacapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

MetadataConfiguration::MetadataConfiguration()
{

}

MetadataConfiguration::MetadataConfiguration(const boost::optional<std::string>& token, const boost::optional<METADATACOMPRESSIONTYPE>& compressiontype, const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<PTZFilter>& ptzstatus, const boost::optional<EventSubscription>& events, const boost::optional<bool>& analytics, const boost::optional<MulticastConfiguration>& multicast, const boost::optional<Duration>& sessiontimeout, const boost::optional<AnalyticsEngineConfiguration>& analyticsengineconfiguration) :
  token_(token),
  compressiontype_(compressiontype),
  name_(name),
  usecount_(usecount),
  ptzstatus_(ptzstatus),
  events_(events),
  analytics_(analytics),
  multicast_(multicast),
  sessiontimeout_(sessiontimeout),
  analyticsengineconfiguration_(analyticsengineconfiguration)
{

}

MetadataConfiguration::MetadataConfiguration(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "token")
    {
      token_ = attribute.value();

    }
    else if (name == "CompressionType")
    {
      compressiontype_ = GetMetadataCompressionType(attribute.value());

    }
  }

  for (const auto& element : node)
  {
    std::string name = TrimNamespace(element.name());
    if (name == "Name")
    {
      name_ = element.text().get();

    }
    else if (name == "UseCount")
    {
      usecount_ = element.text().as_int();

    }
    else if (name == "PTZStatus")
    {
      ptzstatus_ = PTZFilter(element);

    }
    else if (name == "Events")
    {
      events_ = EventSubscription(element);

    }
    else if (name == "Analytics")
    {
      analytics_ = element.text().as_bool();

    }
    else if (name == "Multicast")
    {
      multicast_ = MulticastConfiguration(element);

    }
    else if (name == "SessionTimeout")
    {
      sessiontimeout_ = GetDuration(element.text().get());

    }
    else if (name == "AnalyticsEngineConfiguration")
    {
      analyticsengineconfiguration_ = AnalyticsEngineConfiguration(element);

    }
  }
}

std::string MetadataConfiguration::ToXml(const std::string& name) const
{
  return ("<" + name + ToXmlAttribute("token", token_) + ToXmlAttributeEnum("CompressionType", compressiontype_) + ">" + onvif::ToXml("tt:Name", name_) + onvif::ToXml("tt:UseCount", usecount_) + onvif::ToXmlClass("tt:PTZStatus", ptzstatus_) + onvif::ToXmlClass("tt:Events", events_) + onvif::ToXml("tt:Analytics", analytics_) + onvif::ToXmlClass("tt:Multicast", multicast_) + onvif::ToXml("tt:SessionTimeout", sessiontimeout_) + onvif::ToXmlClass("tt:AnalyticsEngineConfiguration", analyticsengineconfiguration_) + "</"+name+">");
}

bool MetadataConfiguration::operator==(const MetadataConfiguration& rhs) const
{
  return ((token_ == rhs.token_) && (compressiontype_ == rhs.compressiontype_) && (name_ == rhs.name_) && (usecount_ == rhs.usecount_) && (ptzstatus_ == rhs.ptzstatus_) && (events_ == rhs.events_) && (analytics_ == rhs.analytics_) && (multicast_ == rhs.multicast_) && (sessiontimeout_ == rhs.sessiontimeout_) && (analyticsengineconfiguration_ == rhs.analyticsengineconfiguration_));
}

}
