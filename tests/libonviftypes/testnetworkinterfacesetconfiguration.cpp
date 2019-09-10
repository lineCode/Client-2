// testnetworkinterfacesetconfiguration.cpp
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

TEST(TestNetworkInterfaceSetConfiguration, Compare)
{
  Compare(NetworkInterfaceSetConfiguration(true,
                                           NetworkInterfaceConnectionSetting(true, 5, DUPLEX_HALF),
                                           5,
                                           IPv4NetworkInterfaceSetConfiguration(true,
                                                                                { PrefixedIPv4Address(std::string("5.5.5.5"), 5), PrefixedIPv4Address(std::string("6.6.6.6"), 6) },
                                                                                true),
                                           IPv6NetworkInterfaceSetConfiguration(true,
                                                                                true,
                                                                                { PrefixedIPv6Address(std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), 5), PrefixedIPv6Address(std::string("FE80::0202:B3FF:FE1E:8329"), 6) },
                                                                                IPV6DHCPCONFIGURATION_STATEFUL),
                                           { Dot11Configuration(std::string("ssid"), DOT11STATIONMODE_INFRASTRUCTURE, std::string("alias"), 5, Dot11SecurityConfiguration(DOT11SECURITYMODE_WEP, DOT11CIPHER_TKIP, Dot11PSKSet(std::string("key"), std::string("passphrase")), std::string("dot1x"))) }));
}

}

}
