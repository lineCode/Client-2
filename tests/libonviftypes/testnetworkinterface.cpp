// testnetworkinterface.cpp
//

///// Includes /////

#include <networkinterface.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestNetworkInterface, Compare)
{
  Compare(NetworkInterface(std::string("token"),
                           true,
                           NetworkInterfaceInfo(std::string("name"),
                           std::string("hwaddress"), 5),
                           NetworkInterfaceLink(NetworkInterfaceConnectionSetting(true, 5, DUPLEX_HALF),
                           NetworkInterfaceConnectionSetting(true, 5, DUPLEX_HALF), 5),
                           IPv4NetworkInterface(true, IPv4Configuration({ PrefixedIPv4Address(std::string("5.5.5.5"), 5), PrefixedIPv4Address(std::string("6.6.6.6"), 6) }, PrefixedIPv4Address(std::string("7.7.7.7"), 7), PrefixedIPv4Address(std::string("8.8.8.8"), 8), true)),
                           IPv6NetworkInterface(true, IPv6Configuration(true, IPV6DHCPCONFIGURATION_STATEFUL, { PrefixedIPv6Address(std::string("5.5.5.5"), 5), PrefixedIPv6Address(std::string("6.6.6.6"), 6) }, { PrefixedIPv6Address(std::string("7.7.7.7"), 7), PrefixedIPv6Address(std::string("8.8.8.8"), 8) }, { PrefixedIPv6Address(std::string("9.9.9.9"), 9), PrefixedIPv6Address(std::string("10.10.10.10"), 10) }, { PrefixedIPv6Address(std::string("11.11.11.11"), 11), PrefixedIPv6Address(std::string("12.12.12.12"), 12) }))));
}

}

}
