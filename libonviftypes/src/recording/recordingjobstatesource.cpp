// recordingjobstatesource.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

RecordingJobStateSource::RecordingJobStateSource()
{

}

RecordingJobStateSource::RecordingJobStateSource(const boost::optional<SourceReference>& sourcetoken, const boost::optional<RECORDINGJOBSTATE>& state, const boost::optional<RecordingJobStateTracks>& tracks) :
  sourcetoken_(sourcetoken),
  state_(state),
  tracks_(tracks)
{

}

RecordingJobStateSource::RecordingJobStateSource(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SourceToken")
    {
      sourcetoken_ = SourceReference(element);

    }
    else if (name == "State")
    {
      state_ = GetRecordingJobState(element.text().get());

    }
    else if (name == "Tracks")
    {
      tracks_ = RecordingJobStateTracks(element);

    }
  }
}

std::string RecordingJobStateSource::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlClass("tt:SourceToken", sourcetoken_) + onvif::ToXmlEnum("tt:State", state_) + onvif::ToXmlClass("tt:Tracks", tracks_) + "</"+name+">");
}

bool RecordingJobStateSource::operator==(const RecordingJobStateSource& rhs) const
{
  return ((sourcetoken_ == rhs.sourcetoken_) && (state_ == rhs.state_) && (tracks_ == rhs.tracks_));
}

}

}
