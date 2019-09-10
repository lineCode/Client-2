// testdnsinformation.cpp
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

TEST(TestDNSInformation, Compare)
{
  Compare(DNSInformation(true,
                         { std::string("searchdomain1"), std::string("searchdomain2") },
                         { IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), IPAddress(IPTYPE_IPV4, std::string("6.6.6.6"), boost::none) },
                         { IPAddress(IPTYPE_IPV4, std::string("7.7.7.7"), boost::none), IPAddress(IPTYPE_IPV4, std::string("8.8.8.8"), boost::none) }));
}

}

}
