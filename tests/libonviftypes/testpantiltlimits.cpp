// testpantiltlimits.cpp
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

TEST(TestPanTiltLimits, Compare)
{
  Compare(PanTiltLimits(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f))));
}

}

}
