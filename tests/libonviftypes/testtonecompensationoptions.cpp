// testtonecompensationoptions.cpp
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

TEST(TestToneCompensationOptions, Compare)
{
  Compare(ToneCompensationOptions({ TONECOMPENSATIONMODE_ON, TONECOMPENSATIONMODE_AUTO }, true));
}

}

}
