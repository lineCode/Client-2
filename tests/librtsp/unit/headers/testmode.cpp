// testmode.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/mode.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestMode, ToString)
{
  ASSERT_EQ(std::string(), ToString(MODE_INVALID));
  ASSERT_EQ(std::string("PLAY"), ToString(MODE_PLAY));
  ASSERT_EQ(std::string("RECORD"), ToString(MODE_RECORD));
}

TEST(TestMode, FromString)
{
  ASSERT_EQ(MODE_INVALID, ModeTypeFromString("test"));
  ASSERT_EQ(MODE_PLAY, ModeTypeFromString("PLAY"));
  ASSERT_EQ(MODE_RECORD, ModeTypeFromString("RECORD"));
}

}

}

}
