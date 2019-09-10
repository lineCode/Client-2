// audioencoderconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

AudioEncoderConfiguration::AudioEncoderConfiguration()
{

}

AudioEncoderConfiguration::AudioEncoderConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<AUDIOENCODING>& encoding, const boost::optional<int>& bitrate, const boost::optional<int>& samplerate, const boost::optional<MulticastConfiguration>& multicast, const boost::optional<Duration>& sessiontimeout) :
  name_(name),
  usecount_(usecount),
  token_(token),
  encoding_(encoding),
  bitrate_(bitrate),
  samplerate_(samplerate),
  multicast_(multicast),
  sessiontimeout_(sessiontimeout)
{

}

AudioEncoderConfiguration::AudioEncoderConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Name")
    {
      name_ = element.text().get();

    }
    else if (name == "UseCount")
    {
      usecount_ = element.text().as_int();

    }
    else if (name == "Encoding")
    {
      encoding_ = GetAudioEncoding(element.text().get());

    }
    else if (name == "Bitrate")
    {
      bitrate_ = element.text().as_int();

    }
    else if (name == "SampleRate")
    {
      samplerate_ = element.text().as_int();

    }
    else if (name == "Multicast")
    {
      multicast_ = MulticastConfiguration(element);

    }
    else if (name == "SessionTimeout")
    {
      sessiontimeout_ = GetDuration(element.text().get());

    }
  }

  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "token")
    {
      token_ = attribute.value();

    }
  }
}

std::string AudioEncoderConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+ onvif::ToXmlAttribute("token", token_) + ">" + onvif::ToXml("tt:Name", name_) + onvif::ToXml("tt:UseCount", usecount_) + onvif::ToXmlEnum("tt:Encoding", encoding_) + onvif::ToXml("tt:Bitrate", bitrate_) + onvif::ToXml("tt:SampleRate", samplerate_) + ToXmlClass("tt:Multicast", multicast_) + onvif::ToXml("tt:SessionTimeout", sessiontimeout_) + "</"+name+">");
}

bool AudioEncoderConfiguration::operator==(const AudioEncoderConfiguration& rhs) const
{
  return ((name_ == rhs.name_) && (usecount_ == rhs.usecount_) && (encoding_ == rhs.encoding_) && (bitrate_ == rhs.bitrate_) && (samplerate_ == rhs.samplerate_) && (multicast_ == rhs.multicast_) && (sessiontimeout_ == rhs.sessiontimeout_) && (token_ == rhs.token_));
}

}
