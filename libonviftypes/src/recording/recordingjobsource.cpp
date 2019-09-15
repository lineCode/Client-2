// recordingjobsource.cpp
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

RecordingJobSource::RecordingJobSource()
{

}

RecordingJobSource::RecordingJobSource(const boost::optional<SourceReference>& sourcetoken, const boost::optional<bool>& autocreatereceiver, const std::vector<RecordingJobTrack>& tracks) :
  sourcetoken_(sourcetoken),
  autocreatereceiver_(autocreatereceiver),
  tracks_(tracks)
{

}

RecordingJobSource::RecordingJobSource(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SourceToken")
    {
      sourcetoken_ = SourceReference(element);

    }
    else if (name == "AutoCreateReceiver")
    {
      autocreatereceiver_ = element.text().as_bool();

    }
    else if (name == "Tracks")
    {
      tracks_.push_back(RecordingJobTrack(element));

    }
  }
}

std::string RecordingJobSource::ToXml(const std::string& name) const
{
  std::string tracks;
  for (const auto track : tracks_)
  {
    tracks += track.ToXml("tt:Tracks");

  }

  return ("<"+name+">" + onvif::ToXmlClass("tt:SourceToken", sourcetoken_) + onvif::ToXml("tt:AutoCreateReceiver", autocreatereceiver_) + tracks + "</"+name+">");
}

bool RecordingJobSource::operator==(const RecordingJobSource& rhs) const
{
  return ((sourcetoken_ == rhs.sourcetoken_) && (autocreatereceiver_ == rhs.autocreatereceiver_) && std::is_permutation(tracks_.begin(), tracks_.end(), rhs.tracks_.begin(), rhs.tracks_.end()));
}

}

}
