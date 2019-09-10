// testmpeg4options.cpp
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

TEST(TestMpeg4Options, Compare)
{
  Compare(Mpeg4Options({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5), { MPEG4PROFILE_ASP }));
}

}

}
