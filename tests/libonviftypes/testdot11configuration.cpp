// testdot11configuration.cpp
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

TEST(TestDot11Cconfiguration, Compare)
{
  Compare(Dot11Configuration(std::string("ssid"), DOT11STATIONMODE_INFRASTRUCTURE, std::string("alias"), 5, Dot11SecurityConfiguration(DOT11SECURITYMODE_WEP, DOT11CIPHER_TKIP, Dot11PSKSet(std::string("key"), std::string("passphrase")), std::string("dot1x"))));
}

}

}
