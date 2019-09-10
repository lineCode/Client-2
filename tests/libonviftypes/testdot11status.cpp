// testdot11status.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestDot11Status, Compare)
{
  Compare(Dot11Status(std::string("ssid"), std::string("bssid"), DOT11CIPHER_TKIP, DOT11CIPHER_TKIP, DOT11SIGNALSTRENGTH_GOOD, std::string("activeconfigalias")));
}

}

}
