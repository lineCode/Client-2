// testreceiver.cpp
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

TEST(ReceiverTestReceiver, Compare)
{
  Compare(receiver::Receiver(std::string("token"), receiver::ReceiverConfiguration(receiver::RECEIVERMODE_ALWAYSCONNECT, std::string("mediauri"), StreamSetup(STREAM_RTPMULTICAST, Transport(TRANSPORTPROTOCOL_HTTP, Transport(TRANSPORTPROTOCOL_RTSP))))));
}

}

}
