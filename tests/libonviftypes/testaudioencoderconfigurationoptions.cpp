// testaudioencoderconfigurationoptions.cpp
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

TEST(TestAudioEncoderConfigurationOptions, Compare)
{
  Compare(AudioEncoderConfigurationOptions({ AudioEncoderConfigurationOption(AUDIOENCODING_G726, IntList({ 5, 6, 7 }), IntList({ 6, 7, 8 })), AudioEncoderConfigurationOption(AUDIOENCODING_G711, IntList({ 1, 2, 3 }), IntList({ 4, 5, 6 })) }));
}

}

}
