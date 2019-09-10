// testduration.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestDuration, Compare)
{
  ASSERT_EQ(*GetDuration("P3Y4M5DT6H7M8S"), Duration(3, 4, 5, 6, 7, 8, 0));
  ASSERT_EQ(*GetDuration("PT2H"), Duration(0, 0, 0, 2, 0, 0, 0));
  ASSERT_EQ(*GetDuration("P1347Y"), Duration(1347, 0, 0, 0, 0, 0, 0));
  ASSERT_EQ(*GetDuration("P1347M"), Duration(0, 1347, 0, 0, 0, 0, 0));
  ASSERT_EQ(*GetDuration("PT0S"), Duration(0, 0, 0, 0, 0, 0, 0));
  ASSERT_EQ(*GetDuration("P2MT30M"), Duration(0, 2, 0, 0, 30, 0, 0));
  ASSERT_EQ(*GetDuration("PT6H"), Duration(0, 0, 0, 6, 0, 0, 0));
  ASSERT_EQ(*GetDuration("P1Y"), Duration(1, 0, 0, 0, 0, 0, 0));
  ASSERT_EQ(*GetDuration("PT0.5S"), Duration(0, 0, 0, 0, 0, 0, 500));
  ASSERT_EQ(*GetDuration("PT1.5S"), Duration(0, 0, 0, 0, 0, 1, 500));
  ASSERT_FALSE(GetDuration("abc").is_initialized());
  ASSERT_FALSE(GetDuration("P").is_initialized());
  ASSERT_FALSE(GetDuration("T").is_initialized());
  ASSERT_FALSE(GetDuration("").is_initialized());
  ASSERT_EQ(Duration(), Duration(0, 0, 0, 0, 0, 0, 0));

  ASSERT_EQ(Duration(0), Duration(0, 0, 0, 0, 0, 0, 0));
  ASSERT_EQ(Duration(1), Duration(0, 0, 0, 0, 0, 1, 0));

  ASSERT_EQ(Duration(59), Duration(0, 0, 0, 0, 0, 59, 0));
  ASSERT_EQ(Duration(60), Duration(0, 0, 0, 0, 1, 0, 0));
  ASSERT_EQ(Duration(61), Duration(0, 0, 0, 0, 1, 1, 0));

  ASSERT_EQ(Duration(3599), Duration(0, 0, 0, 0, 59, 59, 0));
  ASSERT_EQ(Duration(3600), Duration(0, 0, 0, 1, 0, 0, 0));
  ASSERT_EQ(Duration(3601), Duration(0, 0, 0, 1, 0, 1, 0));

  ASSERT_EQ(Duration(86399), Duration(0, 0, 0, 23, 59, 59, 0));
  ASSERT_EQ(Duration(86400), Duration(0, 0, 1, 0, 0, 0, 0));
  ASSERT_EQ(Duration(86401), Duration(0, 0, 1, 0, 0, 1, 0));
}

TEST(TestDuration, ToString)
{
  ASSERT_EQ(*GetDuration(Duration(3, 4, 5, 6, 7, 8, 0).ToString()), Duration(3, 4, 5, 6, 7, 8, 0));
  ASSERT_EQ(*GetDuration(Duration(0, 0, 0, 2, 0, 0, 0).ToString()), Duration(0, 0, 0, 2, 0, 0, 0));
  ASSERT_EQ(*GetDuration(Duration(1347, 0, 0, 0, 0, 0, 0).ToString()), Duration(1347, 0, 0, 0, 0, 0, 0));
  ASSERT_EQ(*GetDuration(Duration(0, 1347, 0, 0, 0, 0, 0).ToString()), Duration(0, 1347, 0, 0, 0, 0, 0));
  ASSERT_EQ(*GetDuration(Duration(0, 0, 0, 0, 0, 0, 0).ToString()), Duration(0, 0, 0, 0, 0, 0, 0));
  ASSERT_EQ(*GetDuration(Duration(0, 0, 0, 0, 0, 0, 0).ToString()), Duration(0, 0, 0, 0, 0, 0, 0));
  ASSERT_EQ(*GetDuration(Duration(0, 0, 0, 6, 0, 0, 0).ToString()), Duration(0, 0, 0, 6, 0, 0, 0));
  ASSERT_EQ(*GetDuration(Duration(1, 0, 0, 0, 0, 0, 0).ToString()), Duration(1, 0, 0, 0, 0, 0, 0));
  ASSERT_EQ(*GetDuration(Duration(0, 0, 0, 0, 0, 0, 500).ToString()), Duration(0, 0, 0, 0, 0, 0, 500));
  ASSERT_EQ(*GetDuration(Duration(0, 0, 0, 0, 0, 0, 1500).ToString()), Duration(0, 0, 0, 0, 0, 1, 500));
}

}

}
