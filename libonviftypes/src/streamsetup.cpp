// streamsetup.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

StreamSetup::StreamSetup()
{

}

StreamSetup::StreamSetup(const boost::optional<STREAMTYPE>& stream, const boost::optional<Transport>& transport) :
  stream_(stream),
  transport_(transport)
{

}

StreamSetup::StreamSetup(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Stream")
    {
      stream_ = GetStreamType(element.text().get());

    }
    else if (name == "Transport")
    {
      transport_ = Transport(element);

    }
  }
}

std::string StreamSetup::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Stream", stream_) + ToXmlClass("tt:Transport", transport_) + "</"+name+">");
}

bool StreamSetup::operator==(const StreamSetup& rhs) const
{
  return ((stream_ == rhs.stream_) && (transport_ == rhs.transport_));
}

}
