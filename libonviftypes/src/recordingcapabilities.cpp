// recordingcapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

RecordingCapabilities::RecordingCapabilities()
{
  
}

RecordingCapabilities::RecordingCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& receiversource, const boost::optional<bool>& mediaprofilesource, const boost::optional<bool>& dynamicrecordings, const boost::optional<bool>& dynamictracks, const boost::optional<int>& maxstringlength) :
  xaddr_(xaddr),
  receiversource_(receiversource),
  mediaprofilesource_(mediaprofilesource),
  dynamicrecordings_(dynamicrecordings),
  dynamictracks_(dynamictracks),
  maxstringlength_(maxstringlength)
{
  
}

RecordingCapabilities::RecordingCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "XAddr")
    {
      xaddr_ = element.text().get();

    }
    else if (name == "ReceiverSource")
    {
      receiversource_ = element.text().as_bool();

    }
    else if (name == "MediaProfileSource")
    {
      mediaprofilesource_ = element.text().as_bool();

    }
    else if (name == "DynamicRecordings")
    {
      dynamicrecordings_ = element.text().as_bool();

    }
    else if (name == "DynamicTracks")
    {
      dynamictracks_ = element.text().as_bool();

    }
    else if (name == "MaxStringLength")
    {
      maxstringlength_ = element.text().as_int();

    }
  }
}

std::string RecordingCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + onvif::ToXml("tt:ReceiverSource", receiversource_) + onvif::ToXml("tt:MediaProfileSource", mediaprofilesource_) + onvif::ToXml("tt:DynamicRecordings", dynamicrecordings_) + onvif::ToXml("tt:DynamicTracks", dynamictracks_) + onvif::ToXml("tt:MaxStringLength", maxstringlength_) + "</"+name+">");
}

bool RecordingCapabilities::operator==(const RecordingCapabilities& rhs) const
{
  return ((xaddr_ == rhs.xaddr_) && (receiversource_ == rhs.receiversource_) && (mediaprofilesource_ == rhs.mediaprofilesource_) && (dynamicrecordings_ == rhs.dynamicrecordings_) && (dynamictracks_ == rhs.dynamictracks_) && (maxstringlength_ == rhs.maxstringlength_));
}

}
