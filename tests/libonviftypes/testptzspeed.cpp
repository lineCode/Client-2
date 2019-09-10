// testptzspeed.cpp
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

TEST(TestPTZSpeed, Compare)
{
  Compare(PTZSpeed(Vector2d(5, 5, std::string("space")), Vector1d(5, std::string("space"))));
}

}

}
