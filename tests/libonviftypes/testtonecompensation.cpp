// testtonecompensation.cpp
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

TEST(TestToneCompensation, Compare)
{
  Compare(ToneCompensation(TONECOMPENSATIONMODE_ON, 4.0f));
}

}

}
