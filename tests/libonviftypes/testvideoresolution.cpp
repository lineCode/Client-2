// testvideoresolution.cpp
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

TEST(TestVideoResolution, Compare)
{
  Compare(VideoResolution(5, 5));
}

}

}
