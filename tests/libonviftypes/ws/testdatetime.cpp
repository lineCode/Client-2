// testdatetime.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestWsDateTime, Compare)
{
  Compare(ws::DateTime(std::string("2002-05-30T09:00:00")));
}

TEST(TestWsDateTime, GetPTime)
{
  const unsigned int year = 1950;
  const unsigned int month = 1;
  const unsigned int day = 1;
  const unsigned int hours = 1;
  const unsigned int minutes = 1;
  const unsigned int seconds = 1;
  const unsigned int milliseconds = 0;

  ASSERT_EQ(ws::DateTime(year, month, day, hours, minutes, seconds, milliseconds).GetPTime(), boost::optional<boost::posix_time::ptime>(boost::posix_time::ptime(boost::gregorian::date(year, month, day), boost::posix_time::time_duration(hours, minutes, seconds, milliseconds))));
}

}

}
