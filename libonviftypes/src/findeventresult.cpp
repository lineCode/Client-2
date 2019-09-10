// findeventresult.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

FindEventResult::FindEventResult()
{

}

FindEventResult::FindEventResult(const boost::optional<std::string>& recordingtoken, const boost::optional<std::string>& tracktoken, const boost::optional<ws::DateTime>& time, const boost::optional<Element>& event, const boost::optional<bool>& startstateevent) :
  recordingtoken_(recordingtoken),
  tracktoken_(tracktoken),
  time_(time),
  event_(event),
  startstateevent_(startstateevent)
{

}

FindEventResult::FindEventResult(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "RecordingToken")
    {
      recordingtoken_ = element.text().get();

    }
    else if (name == "TrackToken")
    {
      tracktoken_ = element.text().get();

    }
    else if (name == "Time")
    {
      time_ = ws::DateTime(element);

    }
    else if (name == "Event")
    {
      event_ = Element(element);

    }
    else if (name == "StartStateEvent")
    {
      startstateevent_ = element.text().as_bool();

    }
  }
}

std::string FindEventResult::ToXml(const std::string& name) const
{
  std::string event;
  if (event_.is_initialized())
  {
    event = event_->ToXml();

  }

  return ("<"+name+">" + onvif::ToXml("tt:RecordingToken", recordingtoken_) + onvif::ToXml("tt:TrackToken", tracktoken_) + onvif::ToXmlClass("tt:Time", time_) + event + onvif::ToXml("tt:StartStateEvent", startstateevent_) + "</"+name+">");
}

bool FindEventResult::operator==(const FindEventResult& rhs) const
{
  return ((recordingtoken_ == rhs.recordingtoken_) && (tracktoken_ == rhs.tracktoken_) && (time_ == rhs.time_) && (event_ == rhs.event_) && (startstateevent_ == rhs.startstateevent_));
}

}
