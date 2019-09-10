// testptzpresettoursupported.cpp
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

TEST(TestPTZPresetTourSupported, Compare)
{
  Compare(PTZPresetTourSupported(5, { PTZPRESETTOUROPERATION_STOP, PTZPRESETTOUROPERATION_PAUSE }));
}

}

}
