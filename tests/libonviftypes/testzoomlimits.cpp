// testfloatrange.cpp
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

TEST(TestZoomLimits, Compare)
{
  Compare(ZoomLimits(Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f))));
}

}

}
