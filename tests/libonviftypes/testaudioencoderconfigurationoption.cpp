// testaudioencoderconfigurationoption.cpp
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

TEST(TestAudioEncoderConfigurationOption, Compare)
{
  Compare(AudioEncoderConfigurationOption(AUDIOENCODING_G711, IntList({ 5, 6, 7 }), IntList({ 6, 7, 8 })));
}

}

}
