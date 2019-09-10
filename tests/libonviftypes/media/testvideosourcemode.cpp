// testvideosourcemode.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestVideoSourceMode, Compare)
{
  Compare(media::VideoSourceMode(1.0f, VideoResolution(5, 6), { VIDEOENCODING_JPEG, VIDEOENCODING_MPEG4 }, false, std::string("description"), std::string("token"), true));
}

}

}
