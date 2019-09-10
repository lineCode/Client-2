// testbacklightcompensation.cpp
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

TEST(TestBacklightCompensation, Compare)
{
  Compare(BacklightCompensation(BACKLIGHTCOMPENSATIONMODE_OFF, 5.0f));
}

}

}
