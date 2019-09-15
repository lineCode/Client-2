// recordingjobstatetracks.cpp
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

RecordingJobStateTracks::RecordingJobStateTracks()
{

}

RecordingJobStateTracks::RecordingJobStateTracks(const std::vector<RecordingJobStateTrack>& track) :
  track_(track)
{

}

RecordingJobStateTracks::RecordingJobStateTracks(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Track")
    {
      track_.push_back(RecordingJobStateTrack(element));

    }
  }
}

std::string RecordingJobStateTracks::ToXml(const std::string& name) const
{
  std::string tracks;
  for (const auto track : track_)
  {
    tracks += track.ToXml("tt:Track");

  }

  return ("<"+name+">" + tracks + "</"+name+">");
}

bool RecordingJobStateTracks::operator==(const RecordingJobStateTracks& rhs) const
{
  return std::is_permutation(track_.begin(), track_.end(), rhs.track_.begin(), rhs.track_.end());
}

}

}
