// testaudiodecoderconfigurationoptions.cpp
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

TEST(TestAudioDecoderConfigurationOptions, Compare)
{
  Compare(AudioDecoderConfigurationOptions(AacDecOptions(IntList({ 1, 2, 3 }), IntList({ 4, 5, 6 })), G711DecOptions(IntList({ 7, 8, 9 }), IntList({ 10, 11, 12 })), G726DecOptions(IntList({ 13, 14, 15 }), IntList({ 16, 17, 18 }))));
}

}

}
