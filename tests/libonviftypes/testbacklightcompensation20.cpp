// testbacklightcompensation20.cpp
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

TEST(TestBacklightCompensation20, Compare)
{
  Compare(BacklightCompensation20(BACKLIGHTCOMPENSATIONMODE_OFF, 5.0f));
}

}

}
