// teststreamsetup.cpp
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

TEST(TestStreamSetup, Compare)
{
  Compare(StreamSetup(STREAM_RTPMULTICAST, Transport(TRANSPORTPROTOCOL_HTTP, Transport(TRANSPORTPROTOCOL_RTSP))));
}

}

}
