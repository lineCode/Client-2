// testipv6configuration.cpp
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

TEST(TestIpv6Configuration, Compare)
{
  Compare(IPv6Configuration(true, IPV6DHCPCONFIGURATION_STATEFUL, { PrefixedIPv6Address(std::string("5.5.5.5"), 5), PrefixedIPv6Address(std::string("6.6.6.6"), 6) }, { PrefixedIPv6Address(std::string("7.7.7.7"), 7), PrefixedIPv6Address(std::string("8.8.8.8"), 8) }, { PrefixedIPv6Address(std::string("9.9.9.9"), 9), PrefixedIPv6Address(std::string("10.10.10.10"), 10) }, { PrefixedIPv6Address(std::string("11.11.11.11"), 11), PrefixedIPv6Address(std::string("12.12.12.12"), 12) }));
}

}

}
