// recordingconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

RecordingConfiguration::RecordingConfiguration()
{

}

RecordingConfiguration::RecordingConfiguration(const boost::optional<RecordingSourceInformation>& source, const boost::optional<std::string>& content, const boost::optional<Duration>& maximumretentiontime) :
  source_(source),
  content_(content),
  maximumretentiontime_(maximumretentiontime)
{

}

RecordingConfiguration::RecordingConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Source")
    {
      source_ = RecordingSourceInformation(element);

    }
    else if (name == "Content")
    {
      content_ = element.text().get();

    }
    else if (name == "MaximumRetentionTime")
    {
      maximumretentiontime_ = GetDuration(element.text().get());

    }
  }
}

std::string RecordingConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlClass("tt:Source", source_, true) + onvif::ToXml("tt:Content", content_) + onvif::ToXml("tt:MaximumRetentionTime", maximumretentiontime_) + "</"+name+">");
}

bool RecordingConfiguration::operator==(const RecordingConfiguration& rhs) const
{
  return ((source_ == rhs.source_) && (content_ == rhs.content_) && (maximumretentiontime_ == rhs.maximumretentiontime_));
}

}

}
