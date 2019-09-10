// testsystemdatetime.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestSystemDateTime, Compare)
{
  Compare(SystemDateTime(DATETIME_MANUAL, true, TimeZone(std::string("timezone")), DateTime(Time(5, 5, 5), Date(5, 5, 5)), DateTime(Time(5, 5, 5), Date(5, 5, 5))));
}

}

}
