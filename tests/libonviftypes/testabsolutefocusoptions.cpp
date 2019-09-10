// testabsolutefocusoptions.cpp
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

TEST(TestAbsoluteFocusOptions, Compare)
{
  Compare(AbsoluteFocusOptions(FloatRange(5.0f, 6.0f), FloatRange(7.0f, 8.0f)));
}

}

}
