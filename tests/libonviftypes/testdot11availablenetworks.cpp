// testdot11availablenetworks.cpp
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

TEST(TestDot11AvailableNetworks, Compare)
{
  Compare(Dot11AvailableNetworks(std::string("ssid"), std::string("bssid"), { DOT11AUTHANDMANAGEMENTSUITE_DOT1X, DOT11AUTHANDMANAGEMENTSUITE_PSK }, DOT11CIPHER_TKIP, DOT11CIPHER_TKIP, DOT11SIGNALSTRENGTH_GOOD));
}

}

}
