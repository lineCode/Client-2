// testvideoencoderconfiguration.cpp
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

TEST(TestVideoEncoderConfiguration, Compare)
{
  Compare(VideoEncoderConfiguration(std::string("name"), 5, std::string("token"), VIDEOENCODING_H264, VideoResolution(5, 5), 5.0f, VideoRateControl(5, 5, 5), Mpeg4Configuration(5, MPEG4PROFILE_ASP), H264Configuration(5, H264PROFILE_BASELINE), MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5, 0)));
}

}

}
