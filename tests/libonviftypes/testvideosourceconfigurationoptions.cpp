// testvideosourceconfigurationoptions.cpp
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

TEST(TestVideoSourceConfigurationOptions, Compare)
{
  Compare(VideoSourceConfigurationOptions(IntRectangleRange(IntRange(5, 6), IntRange(7, 8), IntRange(9, 10), IntRange(boost::none, 6)), { "token1", "token2" }, RotateOptions({ ROTATE_ON, ROTATE_AUTO }, IntList({ 90, 180 }))));
}

}

}
