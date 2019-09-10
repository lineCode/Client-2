// testreceiverstateinformation.cpp
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

TEST(ReceiverTestReceiverStateInformation, Compare)
{
  Compare(receiver::ReceiverStateInformation(receiver::RECEIVERSTATE_CONNECTING, true));
}

}

}
