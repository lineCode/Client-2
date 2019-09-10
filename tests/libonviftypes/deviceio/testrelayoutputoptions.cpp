// testrelayoutputoptions.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(DeviceIOTestRelayOutputOptions, Compare)
{
  Compare(deviceio::RelayOutputOptions(std::string("token"), { RELAYMODE_MONOSTABLE }, { 1, 2, 3 }, true));
}

}

}
