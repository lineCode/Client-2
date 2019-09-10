// datetimerange.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

DateTimeRange::DateTimeRange()
{

}

DateTimeRange::DateTimeRange(const boost::optional<ws::DateTime>& from, const boost::optional<ws::DateTime>& until) :
  from_(from),
  until_(until)
{

}

DateTimeRange::DateTimeRange(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "From")
    {
      from_ = ws::DateTime(element);

    }
    else if (name == "Until")
    {
      until_ = ws::DateTime(element);

    }
  }
}

std::string DateTimeRange::ToXml(const std::string& name) const
{
  return std::string("<"+name+">" + ToXmlClass("tt:From", from_) + ToXmlClass("tt:Until", until_) + "</"+name+">");
}

bool DateTimeRange::operator==(const DateTimeRange& rhs) const
{
  return ((from_ == rhs.from_) && (until_ == rhs.until_));
}

}
