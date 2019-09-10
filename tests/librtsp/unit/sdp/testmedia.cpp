// testmedia.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/media.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace tests
{

///// Tests /////

TEST(TestMedia, Compare)
{
  Compare(sdp::Media(sdp::MEDIATYPE_APPLICATION, 10000, 1, sdp::MEDIAPROTOCOL_RTPAVP, { 20, 30, 40 }));

}

}

}
