// testh264options2.cpp
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

TEST(TestH264Options2, Compare)
{
  Compare(H264Options2({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5), { H264PROFILE_HIGH, H264PROFILE_MAIN }, IntRange(5, 5)));
}

}

}
