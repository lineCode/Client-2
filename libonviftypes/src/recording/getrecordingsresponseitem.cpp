// getrecordingsresponseitem.cpp
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

GetRecordingsResponseItem::GetRecordingsResponseItem()
{

}

GetRecordingsResponseItem::GetRecordingsResponseItem(const boost::optional<std::string>& recordingtoken, const boost::optional<RecordingConfiguration>& configuration, const boost::optional<GetTracksResponseList>& tracks) :
  recordingtoken_(recordingtoken),
  configuration_(configuration),
  tracks_(tracks)
{

}

GetRecordingsResponseItem::GetRecordingsResponseItem(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "RecordingToken")
    {
      recordingtoken_ = element.text().get();

    }
    else if (name == "Configuration")
    {
      configuration_ = RecordingConfiguration(element);

    }
    else if (name == "Tracks")
    {
      tracks_ = GetTracksResponseList(element);

    }
  }
}

std::string GetRecordingsResponseItem::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:RecordingToken", recordingtoken_) + onvif::ToXmlClass("tt:Configuration", configuration_) + onvif::ToXmlClass("tt:Tracks", tracks_) + "</"+name+">");
}

bool GetRecordingsResponseItem::operator==(const GetRecordingsResponseItem& rhs) const
{
  return ((recordingtoken_ == rhs.recordingtoken_) && (configuration_ == rhs.configuration_) && (tracks_ == rhs.tracks_));
}

}

}
