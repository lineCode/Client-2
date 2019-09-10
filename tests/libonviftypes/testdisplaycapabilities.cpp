// testdisplaycapabilities.cpp
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

TEST(TestDisplayCapabilities, Compare)
{
  Compare(DisplayCapabilities(std::string("xaddr"), true));
}

}

}
