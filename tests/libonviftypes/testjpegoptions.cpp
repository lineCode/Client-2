// testjpegoptions.cpp
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

TEST(TestJpegOptions, Compare)
{
  Compare(JpegOptions({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5)));
}

}

}
