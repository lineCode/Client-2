// testipv4networkinterfacesetconfiguration.cpp
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

TEST(TestIPv4NetworkInterfaceSetConfiguration, Compare)
{
  Compare(IPv4NetworkInterfaceSetConfiguration(true,
                                               { PrefixedIPv4Address(std::string("5.5.5.5"), 5), PrefixedIPv4Address(std::string("6.6.6.6"), 6) },
                                               true));
}

}

}
