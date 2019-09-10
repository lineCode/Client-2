// testaudiooutputconfigurationoptions.cpp
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

TEST(TestAudioOutputConfigurationOptions, Compare)
{
  Compare(AudioOutputConfigurationOptions({ std::string("token1"), std::string("token2") }, { std::string("primacy1"), std::string("primacy2") }, IntRange(5, 6)));
}

}

}
