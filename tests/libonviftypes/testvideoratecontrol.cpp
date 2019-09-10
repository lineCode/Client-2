// testvideoratecontrol.cpp
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

TEST(TestVideoRateControl, Compare)
{
  Compare(VideoRateControl(5, 5, 5));
}

}

}
