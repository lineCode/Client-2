// gettracksresponselist.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

GetTracksResponseList::GetTracksResponseList()
{

}

GetTracksResponseList::GetTracksResponseList(const std::vector<GetTracksResponseItem>& track) :
  track_(track)
{

}

GetTracksResponseList::GetTracksResponseList(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Track")
    {
      track_.push_back(GetTracksResponseItem(element));

    }
  }
}

std::string GetTracksResponseList::ToXml(const std::string& name) const
{
  std::string tracks;
  for (const auto track : track_)
  {
    tracks += track.ToXml("tt:Track");

  }

  return ("<"+name+">" + tracks + "</"+name+">");
}

bool GetTracksResponseList::operator==(const GetTracksResponseList& rhs) const
{
  return std::is_permutation(track_.begin(), track_.end(), rhs.track_.begin(), rhs.track_.end());
}

}

}
