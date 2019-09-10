// testmpeg4configuration.cpp
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

TEST(TestMpeg4Configuration, Compare)
{
  Compare(Mpeg4Configuration(5, MPEG4PROFILE_ASP));
}

}

}
