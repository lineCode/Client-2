// testaudiooutputconfiguration.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestAudioOutputConfiguration, Compare)
{
  Compare(AudioOutputConfiguration(std::string("name"), 5, std::string("token"), std::string("outputtoken"), std::string("sendprimacy"), 5));
}

}

}
