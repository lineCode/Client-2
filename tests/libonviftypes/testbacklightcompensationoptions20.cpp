// testbacklightcompensationoptions20.cpp
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

TEST(TestBacklightCompensationOptions20, Compare)
{
  Compare(BacklightCompensationOptions20({ BACKLIGHTCOMPENSATIONMODE_ON, BACKLIGHTCOMPENSATIONMODE_OFF }, FloatRange(5.0f, 6.0f)));
}

}

}
