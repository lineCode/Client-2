// testaudiosourceconfiguration.cpp
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

TEST(TestAudioSourceConfiguration, Compare)
{
  Compare(AudioSourceConfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken")));
}

}

}
