// testipaddressfilter.cpp
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

TEST(TestIPAddressFilter, Compare)
{
  Compare(IPAddressFilter(IPADDRESSFILTERTYPE_ALLOW,
                          {
                            PrefixedIPv4Address(std::string("5.5.5.5"), 5),
                            PrefixedIPv4Address(std::string("6.6.6.6"), 6)
                          },
                          {
                            PrefixedIPv6Address(std::string("5.5.5.5"), 5),
                            PrefixedIPv6Address(std::string("6.6.6.6"), 6)
                          }));
}

}

}
