// testorigin.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/origin.hpp>

#include "../testrtsp.hpp"

///// Namespaces /////

namespace rtsp
{

namespace tests
{

///// Tests /////

TEST(TestOrigin, Compare)
{
  Compare(sdp::Origin(std::string("username"), 5000, 6000, sdp::NETTYPE_INTERNET, sdp::ADDRESSTYPE_IP4, "hostname"));

}

}

}
