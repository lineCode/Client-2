// testbandwidth.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/bandwidth.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace tests
{

///// Tests /////

TEST(TestBandwidth, Compare)
{
  Compare(sdp::Bandwidth(sdp::BANDWIDTHTYPE_AS, 5000));
  Compare(sdp::Bandwidth(sdp::BANDWIDTHTYPE_CT, 6000));
}

}

}
