// testipv4networkinterface.cpp
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

TEST(TestIpv4NetworkInterface, Compare)
{
  Compare(IPv4NetworkInterface(true, IPv4Configuration({ PrefixedIPv4Address(std::string("5.5.5.5"), 5), PrefixedIPv4Address(std::string("6.6.6.6"), 6) }, PrefixedIPv4Address(std::string("7.7.7.7"), 7), PrefixedIPv4Address(std::string("8.8.8.8"), 8), true)));
}

}

}
