// recordingsummary.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

RecordingSummary::RecordingSummary()
{

}

RecordingSummary::RecordingSummary(const boost::optional<ws::DateTime>& datafrom, const boost::optional<ws::DateTime>& datauntil, const boost::optional<int>& numberrecordings) :
  datafrom_(datafrom),
  datauntil_(datauntil),
  numberrecordings_(numberrecordings)
{

}

RecordingSummary::RecordingSummary(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "DataFrom")
    {
      datafrom_ = ws::DateTime(element);

    }
    else if (name == "DataUntil")
    {
      datauntil_ = ws::DateTime(element);

    }
    else if (name == "NumberRecordings")
    {
      numberrecordings_ = element.text().as_int();

    }
  }
}

std::string RecordingSummary::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlClass("tt:DataFrom", datafrom_) + onvif::ToXmlClass("tt:DataUntil", datauntil_) + onvif::ToXml("tt:NumberRecordings", numberrecordings_) + "</"+name+">");
}

bool RecordingSummary::operator==(const RecordingSummary& rhs) const
{
  return ((datafrom_ == rhs.datafrom_) && (datauntil_ == rhs.datauntil_) && (numberrecordings_ == rhs.numberrecordings_));
}

}
