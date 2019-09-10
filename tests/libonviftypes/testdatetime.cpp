// testdatetime.cpp
//

///// Includes /////

#include <boost/date_time.hpp>
#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestDateTime, Compare)
{
  Compare(DateTime(Time(5, 5, 5), Date(5, 5, 5)));
  Compare(DateTime(boost::posix_time::second_clock::universal_time()));
}

TEST(TestDateTime, GetDuration)
{
  ASSERT_EQ(GetDuration(std::string("P5Y")), boost::optional<onvif::Duration>(onvif::Duration(5, 0, 0, 0, 0, 0, 0)));
  ASSERT_EQ(GetDuration(std::string("P5Y2M10D")), boost::optional<onvif::Duration>(onvif::Duration(5, 2, 10, 0, 0, 0, 0)));
  ASSERT_EQ(GetDuration(std::string("P5Y2M10DT15H")), boost::optional<onvif::Duration>(onvif::Duration(5, 2, 10, 15, 0, 0, 0)));
  ASSERT_EQ(GetDuration(std::string("PT15H")), boost::optional<onvif::Duration>(onvif::Duration(0, 0, 0, 15, 0, 0, 0)));
}

TEST(TestDateTime, GetOffset)
{
  const auto now = boost::posix_time::second_clock::universal_time();
  const auto date = now.date();
  const auto time = now.time_of_day();

  ASSERT_LE(DateTime(Time(time.hours(), time.minutes(), time.seconds()), Date(static_cast<int>(date.year()), date.month().as_number(), date.day().as_number())).GetOffset(), 1000);
}

}

}
