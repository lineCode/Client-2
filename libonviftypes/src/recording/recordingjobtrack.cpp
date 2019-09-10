// recordingjobtrack.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

RecordingJobTrack::RecordingJobTrack()
{

}

RecordingJobTrack::RecordingJobTrack(const boost::optional<std::string>& sourcetag, const boost::optional<std::string>& destination) :
  sourcetag_(sourcetag),
  destination_(destination)
{

}

RecordingJobTrack::RecordingJobTrack(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SourceTag")
    {
      sourcetag_ = element.text().get();

    }
    else if (name == "Destination")
    {
      destination_ = element.text().get();

    }
  }
}

std::string RecordingJobTrack::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:SourceTag", sourcetag_) + onvif::ToXml("tt:Destination", destination_) + "</"+name+">");
}

bool RecordingJobTrack::operator==(const RecordingJobTrack& rhs) const
{
  return ((sourcetag_ == rhs.sourcetag_) && (destination_ == rhs.destination_));
}

}

}
