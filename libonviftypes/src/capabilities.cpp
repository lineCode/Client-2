// capabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Capabilities::Capabilities()
{
  
}

Capabilities::Capabilities(const boost::optional<AnalyticsCapabilities>& analytics, const boost::optional<DeviceCapabilities>& device, const boost::optional<EventCapabilities>& events, const boost::optional<ImagingCapabilities>& imaging, const boost::optional<MediaCapabilities>& media, const boost::optional<PTZCapabilities>& ptz, const boost::optional<DeviceIOCapabilities>& deviceio, const boost::optional<DisplayCapabilities>& display, const boost::optional<RecordingCapabilities>& recording, const boost::optional<SearchCapabilities>& search, const boost::optional<ReplayCapabilities>& replay, const boost::optional<ReceiverCapabilities>& receiver, const boost::optional<AnalyticsDeviceCapabilities>& analyticsdevice) :
  analytics_(analytics),
  device_(device),
  events_(events),
  imaging_(imaging),
  media_(media),
  ptz_(ptz),
  deviceio_(deviceio),
  display_(display),
  recording_(recording),
  search_(search),
  replay_(replay),
  receiver_(receiver),
  analyticsdevice_(analyticsdevice)
{
  
}

Capabilities::Capabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Analytics")
    {
      analytics_ = AnalyticsCapabilities(element);

    }
    else if (name == "Device")
    {
      device_ = DeviceCapabilities(element);

    }
    else if (name == "Events")
    {
      events_ = EventCapabilities(element);

    }
    else if (name == "Imaging")
    {
      imaging_ = ImagingCapabilities(element);

    }
    else if (name == "Media")
    {
      media_ = MediaCapabilities(element);

    }
    else if (name == "PTZ")
    {
      ptz_ = PTZCapabilities(element);

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "DeviceIO")
        {
          deviceio_ = DeviceIOCapabilities(extension);

        }
        else if (name == "Display")
        {
          display_ = DisplayCapabilities(extension);

        }
        else if (name == "Receiver")
        {
          receiver_ = ReceiverCapabilities(extension);

        }
        else if (name == "Recording")
        {
          recording_ = RecordingCapabilities(extension);

        }
        else if (name == "Search")
        {
          search_ = SearchCapabilities(extension);

        }
        else if (name == "Replay")
        {
          replay_ = ReplayCapabilities(extension);

        }
        else if (name == "AnalyticsDevice")
        {
          analyticsdevice_ = AnalyticsDeviceCapabilities(extension);

        }
      }
    }
  }
}

utility::Version Capabilities::GetMaxSupportedVersion() const
{
  if (!device_.is_initialized() || !device_->system_.is_initialized())
  {

    return utility::Version(0, 0, 0);
  }

  utility::Version maxsupportedversion(0, 0, 0);
  for (const auto& onvifversion : device_->system_->supportedversions_)
  {
    if (onvifversion.GetVersion() > maxsupportedversion)
    {
      maxsupportedversion = onvifversion.GetVersion();

    }
  }
  
  return maxsupportedversion;
}

std::string Capabilities::ToXml(const std::string& name) const
{
  std::string extension;
  if (deviceio_.is_initialized() || display_.is_initialized() || recording_.is_initialized() || search_.is_initialized() || replay_.is_initialized() || receiver_.is_initialized() || analyticsdevice_.is_initialized())
  {
    extension = "<tt:Extension>" + ToXmlClass("tt:DeviceIO", deviceio_) + ToXmlClass("tt:Display", display_) + ToXmlClass("tt:Recording", recording_) + ToXmlClass("tt:Search", search_) + ToXmlClass("tt:Replay", replay_) + ToXmlClass("tt:Receiver", receiver_) + ToXmlClass("tt:AnalyticsDevice", analyticsdevice_) + "</tt:Extension>";

  }

  return ("<"+name+">" + ToXmlClass("tt:Analytics", analytics_) + ToXmlClass("tt:Device", device_) + ToXmlClass("tt:Events", events_) + ToXmlClass("tt:Imaging", imaging_) + ToXmlClass("tt:Media", media_) + ToXmlClass("tt:PTZ", ptz_) + extension + "</"+name+">");
}

bool Capabilities::operator==(const Capabilities& rhs) const
{
  return ((analytics_ == rhs.analytics_) && (device_ == rhs.device_) && (events_ == rhs.events_) && (imaging_ == rhs.imaging_) && (media_ == rhs.media_) && (ptz_ == rhs.ptz_) && (deviceio_ == rhs.deviceio_) && (display_ == rhs.display_) && (recording_ == rhs.recording_) && (search_ == rhs.search_) && (replay_ == rhs.replay_) && (receiver_ == rhs.receiver_) && (analyticsdevice_ == rhs.analyticsdevice_));
}

}
