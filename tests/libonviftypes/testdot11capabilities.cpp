// testdot11capabilities.cpp
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

TEST(TestDot11Capabilities, Compare)
{
  Compare(Dot11Capabilities(true, true, true, true, true));
}

}

}
