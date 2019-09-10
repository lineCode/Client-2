// testnetworkhost.cpp
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

TEST(TestNetworkHost, Compare)
{
  Compare(NetworkHost(NETWORKHOSTTYPE_IPV6, std::string("5.5.5.5"), std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), std::string("dnsname")));
}

}

}
