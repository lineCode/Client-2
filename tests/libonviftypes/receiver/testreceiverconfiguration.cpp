// testreceiverconfiguration.cpp
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

TEST(ReceiverTestReceiverConfiguration, Compare)
{
  Compare(receiver::ReceiverConfiguration(receiver::RECEIVERMODE_ALWAYSCONNECT, std::string("mediauri"), StreamSetup(STREAM_RTPMULTICAST, Transport(TRANSPORTPROTOCOL_HTTP, Transport(TRANSPORTPROTOCOL_RTSP)))));
}

}

}
