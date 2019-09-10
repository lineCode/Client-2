// testrtpmap.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/rtpmap.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace tests
{

///// Tests /////

TEST(TestRtpMap, Compare)
{
  Compare(sdp::RtpMap({
                        { 0, sdp::RTPMAPDATA("a", 1, boost::none) },
                        { 1, sdp::RTPMAPDATA("b", 2, 2) },
                        { 2, sdp::RTPMAPDATA("c", 3, boost::none) },
                        { 3, sdp::RTPMAPDATA("d", 4, 3) },
                        { 4, sdp::RTPMAPDATA("e", 5, boost::none) }
                      }));
}

}

}
