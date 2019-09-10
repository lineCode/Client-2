// testipv6networkinterfacesetconfiguration.cpp
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

TEST(TestIPv6NetworkInterfaceSetConfiguration, Compare)
{
  Compare(IPv6NetworkInterfaceSetConfiguration(true,
                                               true,
                                               { PrefixedIPv6Address(std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), 5), PrefixedIPv6Address(std::string("FE80::0202:B3FF:FE1E:8329"), 6) },
                                               IPV6DHCPCONFIGURATION_STATEFUL));
}

}

}
