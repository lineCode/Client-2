// testrtpinfo.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/rtpinfo.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestRtpInfo, Compare)
{
  Compare(RtpInfo({
                    RTPINFODATA("a", 1, boost::none),
                    RTPINFODATA("b", 2, 2),
                    RTPINFODATA("c", 3, boost::none),
                    RTPINFODATA("d", 4, 3),
                    RTPINFODATA("e", 5, boost::none)
                  }));
}

}

}

}
