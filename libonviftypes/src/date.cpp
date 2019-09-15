// date.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Date::Date()
{

}

Date::Date(const boost::optional<int>& year, const boost::optional<int>& month, const boost::optional<int>& day) :
  year_(year),
  month_(month),
  day_(day)
{

}

Date::Date(const boost::gregorian::date& date) :
  year_(date.year()), month_(date.month()), day_(date.day())
{

}

Date::Date(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Year")
    {
      year_ = element.text().as_int();

    }
    else if (name == "Month")
    {
      month_ = element.text().as_int();

    }
    else if (name == "Day")
    {
      day_ = element.text().as_int();

    }
  }
}

std::string Date::ToXml(const std::string& name) const
{
  return std::string("<"+name+">" + onvif::ToXml("tt:Year", year_) + onvif::ToXml("tt:Month", month_) + onvif::ToXml("tt:Day", day_) + "</"+name+">");
}

boost::optional<boost::gregorian::date> Date::GetDate() const
{
  if (!year_ || !month_ || !day_)
  {

    return boost::none;
  }
  
  try
  {
  
    return boost::gregorian::date(*year_, *month_, *day_);
  }
  catch (...)
  {

    return boost::none;
  }
}

bool Date::operator==(const Date& rhs) const
{
  return ((year_ == rhs.year_) && (month_ == rhs.month_) && (day_ == rhs.day_));
}

}
