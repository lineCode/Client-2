// testrelayoutput.cpp
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

TEST(TestRelayOutput, Compare)
{
  Compare(RelayOutput(std::string("token"), RelayOutputSettings(RELAYMODE_MONOSTABLE, Duration(1, 2, 3, 4, 5, 6, 0), RELAYIDLESTATE_OPEN)));
}

}

}
