// space2ddescription.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Space2dDescription::Space2dDescription()
{
  
}

Space2dDescription::Space2dDescription(const boost::optional<std::string>& uri, const boost::optional<FloatRange>& xrange, const boost::optional<FloatRange>& yrange) :
  uri_(uri),
  xrange_(xrange),
  yrange_(yrange)
{
  
}

Space2dDescription::Space2dDescription(const pugi::xml_node& node)
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
    else if (name == "YRange")
    {
      yrange_ = FloatRange(element);

    }
  }
}

std::string Space2dDescription::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:URI", uri_) + ToXmlClass("tt:XRange", xrange_) + ToXmlClass("tt:YRange", yrange_) + "</"+name+">");
}

bool Space2dDescription::operator==(const Space2dDescription& rhs) const
{
  return ((uri_ == rhs.uri_) && (xrange_ == rhs.xrange_) && (yrange_ == rhs.yrange_));
}

}
