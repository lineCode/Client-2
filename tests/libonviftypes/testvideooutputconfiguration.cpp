// testvideooutputconfiguration.cpp
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

TEST(TestVideoOutputConfiguration, Compare)
{
  Compare(VideoOutputConfiguration(std::string("name"), 5, std::string("token"), std::string("outputtoken")));
}

}

}
