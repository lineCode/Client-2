// recordingjobconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

RecordingJobConfiguration::RecordingJobConfiguration()
{

}

RecordingJobConfiguration::RecordingJobConfiguration(const boost::optional<std::string>& recordingtoken, const boost::optional<RECORDINGJOBMODE>& mode, const boost::optional<uint64_t>& priority, const std::vector<RecordingJobSource>& source) :
  recordingtoken_(recordingtoken),
  mode_(mode),
  priority_(priority),
  source_(source)
{

}

RecordingJobConfiguration::RecordingJobConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "RecordingToken")
    {
      recordingtoken_ = element.text().get();

    }
    else if (name == "Mode")
    {
      mode_ = GetRecordingJobMode(element.text().get());

    }
    else if (name == "Priority")
    {
      priority_ = element.text().as_ullong();

    }
    else if (name == "Source")
    {
      source_.push_back(RecordingJobSource(element));

    }
  }
}

std::string RecordingJobConfiguration::ToXml(const std::string& name) const
{
  std::string sources;
  for (const auto source : source_)
  {
    sources += source.ToXml("tt:Source");

  }

  return ("<"+name+">" + onvif::ToXml("tt:RecordingToken", recordingtoken_) + onvif::ToXmlEnum("tt:Mode", mode_) + onvif::ToXml("tt:Priority", priority_) + sources + "</"+name+">");
}

bool RecordingJobConfiguration::operator==(const RecordingJobConfiguration& rhs) const
{
  return ((recordingtoken_ == rhs.recordingtoken_) && (mode_ == rhs.mode_) && (priority_ == rhs.priority_) && std::is_permutation(source_.begin(), source_.end(), rhs.source_.begin(), rhs.source_.end()));
}

}

}
