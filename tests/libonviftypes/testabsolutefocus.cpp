// testabsolutefocus.cpp
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

TEST(TestAbsoluteFocus, Compare)
{
  Compare(AbsoluteFocus(5.0f, 6.0f));
}

}

}
