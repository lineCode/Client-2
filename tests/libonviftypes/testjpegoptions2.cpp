// testjpegoptions2.cpp
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

TEST(TestJpegOptions2, Compare)
{
  Compare(JpegOptions2({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5)));
}

}

}
