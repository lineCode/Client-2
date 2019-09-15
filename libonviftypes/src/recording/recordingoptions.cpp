// recordingoptions.cpp
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

RecordingOptions::RecordingOptions()
{

}

RecordingOptions::RecordingOptions(const boost::optional<JobOptions>& job, const boost::optional<TrackOptions>& track) :
  job_(job),
  track_(track)
{

}

RecordingOptions::RecordingOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Job")
    {
      job_ = JobOptions(element);

    }
    else if (name == "Track")
    {
      track_ = TrackOptions(element);

    }
  }
}

std::string RecordingOptions::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlClass("trc:Job", job_) + onvif::ToXmlClass("trc:Track", track_) + "</"+name+">");
}

bool RecordingOptions::operator==(const RecordingOptions& rhs) const
{
  return ((job_ == rhs.job_) && (track_ == rhs.track_));
}

}

}
