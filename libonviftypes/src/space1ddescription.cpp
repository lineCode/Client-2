// space1ddescription.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

Space1dDescription::Space1dDescription()
{

}

Space1dDescription::Space1dDescription(const boost::optional<std::string>& uri, const boost::optional<FloatRange>& xrange) :
  uri_(uri),
  xrange_(xrange)
{

}

Space1dDescription::Space1dDescription(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "URI")
    {
      uri_ = element.text().get();

    }
    else if (name == "XRange")
    {
      xrange_ = FloatRange(element);

    }
  }
}

std::string Space1dDescription::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:URI", uri_) + ToXmlClass("tt:XRange", xrange_) + "</"+name+">");
}

bool Space1dDescription::operator==(const Space1dDescription& rhs) const
{
  return ((uri_ == rhs.uri_) && (xrange_ == rhs.xrange_));
}

}
