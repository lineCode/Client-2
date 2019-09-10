// testdatetimerange.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestDateTimeRange, Compare)
{
  Compare(DateTimeRange(ws::DateTime(std::string("2002-05-30T09:00:00")), ws::DateTime(std::string("2002-05-30T09:00:00"))));
}

}

}
