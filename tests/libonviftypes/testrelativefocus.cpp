// testarelativefocus.cpp
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

TEST(TestRelativeFocus, Compare)
{
  Compare(RelativeFocus(5.0f, 6.0f));
}

}

}
