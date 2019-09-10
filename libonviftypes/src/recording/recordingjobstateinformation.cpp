// recordingjobstateinformation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

RecordingJobStateInformation::RecordingJobStateInformation()
{

}

RecordingJobStateInformation::RecordingJobStateInformation(const boost::optional<std::string>& recordingtoken, const boost::optional<RECORDINGJOBSTATE>& state, const std::vector<RecordingJobStateSource>& sources) :
  recordingtoken_(recordingtoken),
  state_(state),
  sources_(sources)
{

}

RecordingJobStateInformation::RecordingJobStateInformation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "RecordingToken")
    {
      recordingtoken_ = element.text().get();

    }
    else if (name == "State")
    {
      state_ = GetRecordingJobState(element.text().get());

    }
    else if (name == "Sources")
    {
      sources_.push_back(RecordingJobStateSource(element));

    }
  }
}

std::string RecordingJobStateInformation::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:RecordingToken", recordingtoken_) + onvif::ToXmlEnum("tt:State", state_) + onvif::ToXml("tt:Sources", sources_) + "</"+name+">");
}

bool RecordingJobStateInformation::operator==(const RecordingJobStateInformation& rhs) const
{
  return ((recordingtoken_ == rhs.recordingtoken_) && (state_ == rhs.state_) && std::is_permutation(sources_.begin(), sources_.end(), rhs.sources_.begin(), rhs.sources_.end()));
}

}

}
