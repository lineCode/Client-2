// testaudiosourceconfigurationoptions.cpp
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

TEST(TestAudioSourceConfigurationOptions, Compare)
{
  Compare(AudioSourceConfigurationOptions({ std::string("token1"), std::string("token2") }));
}

}

}
