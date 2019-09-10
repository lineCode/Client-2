// capabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Classes /////

Capabilities::Capabilities()
{

}

Capabilities::Capabilities(const boost::optional<ProfileCapabilities>& profilecapabilities, const boost::optional<StreamingCapabilities>& streamingcapabilities, const boost::optional<bool>& snapshoturi, const boost::optional<bool>& rotation, const boost::optional<bool>& videosourcemode, const boost::optional<bool>& osd, const boost::optional<bool>& exicompression) :
  profilecapabilities_(profilecapabilities),
  streamingcapabilities_(streamingcapabilities),
  snapshoturi_(snapshoturi),
  rotation_(rotation),
  videosourcemode_(videosourcemode),
  osd_(osd),
  exicompression_(exicompression)
{

}

Capabilities::Capabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "SnapshotUri")
    {
      snapshoturi_ = attribute.as_bool();
      
    }
    else if (name == "Rotation")
    {
      rotation_ = attribute.as_bool();

    }
    else if (name == "VideoSourceMode")
    {
      videosourcemode_ = attribute.as_bool();

    }
    else if (name == "OSD")
    {
      osd_ = attribute.as_bool();

    }
    else if (name == "EXICompression")
    {
      exicompression_ = attribute.as_bool();

    }
  }

  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "ProfileCapabilities")
    {
      profilecapabilities_ = ProfileCapabilities(element);

    }
    else if (name == "StreamingCapabilities")
    {
      streamingcapabilities_ = StreamingCapabilities(element);

    }
  }
}

std::string Capabilities::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("SnapshotUri", snapshoturi_) + ToXmlAttribute("Rotation", rotation_) + ToXmlAttribute("VideoSourceMode", videosourcemode_) + ToXmlAttribute("OSD", osd_) + ToXmlAttribute("EXICompression", exicompression_) + ">" + ToXmlClass("tt:ProfileCapabilities", profilecapabilities_) + ToXmlClass("tt:StreamingCapabilities", streamingcapabilities_) + onvif::ToXml("tt:SnapshotUri", snapshoturi_) + onvif::ToXml("tt:Rotation", rotation_) + onvif::ToXml("tt:VideoSourceMode", videosourcemode_) + onvif::ToXml("tt:OSD", osd_) + onvif::ToXml("tt:EXICompression", exicompression_) + "</"+name+">");
}

bool Capabilities::operator==(const Capabilities& rhs) const
{
  return ((profilecapabilities_ == rhs.profilecapabilities_) && (streamingcapabilities_ == rhs.streamingcapabilities_) && (snapshoturi_ == rhs.snapshoturi_) && (rotation_ == rhs.rotation_) && (videosourcemode_ == rhs.videosourcemode_) && (osd_ == rhs.osd_) && (exicompression_ == rhs.exicompression_));
}

}

}

