// datetime
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace ws
{

///// Classes /////

DateTime::DateTime()
{

}

DateTime::DateTime(const boost::posix_time::ptime& ptime) :
  datetime_(boost::posix_time::to_iso_extended_string(ptime) + "Z")
{

}

DateTime::DateTime(const std::string& datetime) :
  datetime_(datetime)
{

}

DateTime::DateTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hours, unsigned int minutes, unsigned int seconds, unsigned int milliseconds)
{
  std::ostringstream ss;
  ss << std::setfill('0') << std::setw(4) << year << std::string("-") << std::setw(2) << month << std::string("-") << std::setw(2) << day << std::string("T") << std::setw(2) << hours << std::string(":") << std::setw(2) << minutes << std::string(":") << std::setw(2) << seconds << std::string(".") << std::setw(3) << milliseconds;
  datetime_ = ss.str();
}

DateTime::DateTime(const pugi::xml_node& node)
{
  datetime_ = node.text().get();

}

std::string DateTime::ToXml(const std::string& name) const
{
  return std::string("<"+name+">" + datetime_ + "</"+name+">");
}

boost::optional<boost::posix_time::ptime> DateTime::GetPTime() const
{
  std::istringstream is(datetime_);
  is.imbue(std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%Y-%m-%dT%H:%M:%S%fZ")));
  boost::posix_time::ptime ptime;
  is >> ptime;
  if (is.fail())
  {
    std::istringstream is(datetime_);
    is.imbue(std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%Y-%m-%dT%H:%M:%SZ")));
    is >> ptime;
    if (is.fail())
    {

      return boost::none;
    }
  }

  return ptime;
}

bool DateTime::operator==(const DateTime& rhs) const
{
  return (datetime_ == rhs.datetime_);
}

}

}
