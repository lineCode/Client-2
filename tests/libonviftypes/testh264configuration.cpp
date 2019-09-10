// h264configuration.cpp
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

TEST(TestH264Configuration, Compare)
{
  Compare(H264Configuration(5, H264PROFILE_BASELINE));
}

}

}
