// capabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Classes /////

Capabilities::Capabilities()
{

}

Capabilities::Capabilities(const boost::optional<int>& videosources, const boost::optional<int>& videooutputs, const boost::optional<int>& audiosources, const boost::optional<int>& audiooutputs, const boost::optional<int>& relayoutputs, const boost::optional<int>& serialports, const boost::optional<int>& digitalinputs, const boost::optional<bool>& digitalinputoptions) :
  videosources_(videosources),
  videooutputs_(videooutputs),
  audiosources_(audiosources),
  audiooutputs_(audiooutputs),
  relayoutputs_(relayoutputs),
  serialports_(serialports),
  digitalinputs_(digitalinputs),
  digitalinputoptions_(digitalinputoptions)
{
  
}

Capabilities::Capabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    std::string name = TrimNamespace(attribute.name());
    if (name == "VideoSources")
    {
      videosources_ = attribute.as_int();

    }
    else if (name == "VideoOutputs")
    {
      videooutputs_ = attribute.as_int();

    }
    else if (name == "AudioSources")
    {
      audiosources_ = attribute.as_int();
      
    }
    else if (name == "AudioOutputs")
    {
      audiooutputs_ = attribute.as_int();

    }
    else if (name == "RelayOutputs")
    {
      relayoutputs_ = attribute.as_int();

    }
    else if (name == "SerialPorts")
    {
      serialports_ = attribute.as_int();

    }
    else if (name == "DigitalInputs")
    {
      digitalinputs_ = attribute.as_int();

    }
    else if (name == "DigitalInputOptions")
    {
      digitalinputoptions_ = attribute.as_bool();

    }
  }
}

std::string Capabilities::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("VideoSources", videosources_) + ToXmlAttribute("VideoOutputs", videooutputs_) + ToXmlAttribute("AudioSources", audiosources_) + ToXmlAttribute("AudioOutputs", audiooutputs_) + ToXmlAttribute("RelayOutputs", relayoutputs_) + ToXmlAttribute("SerialPorts", serialports_) + ToXmlAttribute("DigitalInputs", digitalinputs_) + ToXmlAttribute("DigitalInputOptions", digitalinputoptions_) + "></"+name+">");
}

Element Capabilities::ToElement(const std::string& name) const
{
  std::vector< std::pair<std::string, std::string> > attributes;
  if (videosources_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("VideoSources"), onvif::ToString(*videosources_)));

  }

  if (videooutputs_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("VideoOutputs"), onvif::ToString(*videooutputs_)));

  }

  if (audiosources_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("AudioSources"), onvif::ToString(*audiosources_)));

  }

  if (audiooutputs_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("AudioOutputs"), onvif::ToString(*audiooutputs_)));

  }

  if (relayoutputs_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("RelayOutputs"), onvif::ToString(*relayoutputs_)));

  }

  if (serialports_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("SerialPorts"), onvif::ToString(*serialports_)));

  }

  if (digitalinputs_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("DigitalInputs"), onvif::ToString(*digitalinputs_)));

  }

  if (digitalinputoptions_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("DigitalInputOptions"), onvif::ToString(*digitalinputoptions_)));

  }

  return Element(name, attributes, std::string(), {});
}

bool Capabilities::operator==(const Capabilities& rhs) const
{
  return ((videosources_ == rhs.videosources_) && (videooutputs_ == rhs.videooutputs_) && (audiosources_ == rhs.audiosources_) && (audiooutputs_ == rhs.audiooutputs_) && (relayoutputs_ == rhs.relayoutputs_) && (serialports_ == rhs.serialports_) && (digitalinputs_ == rhs.digitalinputs_) && (digitalinputoptions_ == rhs.digitalinputoptions_));
}

}

}
