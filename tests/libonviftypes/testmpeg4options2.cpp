// testmpeg4options2.cpp
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

TEST(TestMpeg4Options2, Compare)
{
  Compare(Mpeg4Options2({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5), { MPEG4PROFILE_ASP }, IntRange(5, 5)));
}

}

}
