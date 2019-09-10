// testaudioencoderconfiguration.cpp
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

TEST(TestAudioEncoderConfiguration, Compare)
{
  Compare(AudioEncoderConfiguration(std::string("name"), 5, std::string("token"), AUDIOENCODING_AAC, 5, 5, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5, 0)));
}

}

}
