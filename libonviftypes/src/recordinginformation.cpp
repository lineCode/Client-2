// recordinginformation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

RecordingInformation::RecordingInformation()
{

}

RecordingInformation::RecordingInformation(const boost::optional<std::string>& recordingtoken, const boost::optional<RecordingSourceInformation>& source, const boost::optional<ws::DateTime>& earliestrecording, const boost::optional<ws::DateTime>& latestrecording, const boost::optional<std::string>& content, const std::vector<TrackInformation>& track, const boost::optional<RECORDINGSTATUS>& recordingstatus) :
  recordingtoken_(recordingtoken),
  source_(source),
  earliestrecording_(earliestrecording),
  latestrecording_(latestrecording),
  content_(content),
  track_(track),
  recordingstatus_(recordingstatus)
{

}

RecordingInformation::RecordingInformation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "RecordingToken")
    {
      recordingtoken_ = element.text().get();

    }
    else if (name == "Source")
    {
      source_ = RecordingSourceInformation(element);

    }
    else if (name == "EarliestRecording")
    {
      earliestrecording_ = ws::DateTime(element);

    }
    else if (name == "LatestRecording")
    {
      latestrecording_ = ws::DateTime(element);

    }
    else if (name == "Content")
    {
      content_ = element.text().get();

    }
    else if (name == "Track")
    {
      track_.push_back(TrackInformation(element));

    }
    else if (name == "RecordingStatus")
    {
      recordingstatus_ = GetRecordingStatus(element.text().get());

    }
  }
}

std::string RecordingInformation::ToXml(const std::string& name) const
{
  return ToXml(name, true);
}

std::string RecordingInformation::ToXml(const std::string& name, const bool namespaces) const
{
  const std::string namespacett = namespaces ? "tt:" : "";

  std::string tracks;
  for (const TrackInformation& track : track_)
  {
    tracks += track.ToXml(namespacett + "Track", namespaces);

  }
  return ("<"+name+">" + onvif::ToXml(namespacett + "RecordingToken", recordingtoken_) + onvif::ToXmlClass(namespacett + "Source", source_, namespaces) + onvif::ToXmlClass(namespacett + "EarliestRecording", earliestrecording_) + onvif::ToXmlClass(namespacett + "LatestRecording", latestrecording_) + onvif::ToXml(namespacett + "Content", content_) + tracks + onvif::ToXmlEnum(namespacett + "RecordingStatus", recordingstatus_) + "</"+name+">");
}

bool RecordingInformation::operator==(const RecordingInformation& rhs) const
{
  return ((recordingtoken_ == rhs.recordingtoken_) && (source_ == rhs.source_) && (earliestrecording_ == rhs.earliestrecording_) && (latestrecording_ == rhs.latestrecording_) && (content_ == rhs.content_) && (track_ == rhs.track_) && (recordingstatus_ == rhs.recordingstatus_));
}

}
