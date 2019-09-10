// recordingjobstatetrack.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

RecordingJobStateTrack::RecordingJobStateTrack()
{

}

RecordingJobStateTrack::RecordingJobStateTrack(const boost::optional<std::string>& sourcetag, const boost::optional<std::string>& destination, const boost::optional<std::string>& error, const boost::optional<RECORDINGJOBSTATE>& state) :
  sourcetag_(sourcetag),
  destination_(destination),
  error_(error),
  state_(state)
{

}

RecordingJobStateTrack::RecordingJobStateTrack(const pugi::xml_node& node)
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
    else if (name == "Error")
    {
      error_ = element.text().get();

    }
    else if (name == "State")
    {
      state_ = GetRecordingJobState(element.text().get());

    }
  }
}

std::string RecordingJobStateTrack::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:SourceTag", sourcetag_) + onvif::ToXml("tt:Destination", destination_) + onvif::ToXml("tt:Error", error_) + onvif::ToXmlEnum("tt:State", state_) + "</"+name+">");
}

bool RecordingJobStateTrack::operator==(const RecordingJobStateTrack& rhs) const
{
  return ((sourcetag_ == rhs.sourcetag_) && (destination_ == rhs.destination_) && (error_ == rhs.error_) && (state_ == rhs.state_));
}

}

}
