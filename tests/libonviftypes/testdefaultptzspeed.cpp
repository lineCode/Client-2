// testdefaultptzspeed.cpp
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

TEST(TestDefaultPTZSpeed, Compare)
{
  Compare(DefaultPTZSpeed(Vector2d(5, 5, std::string("space")), 5));
}

}

}
