// testnetworkgateway.cpp
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

TEST(TestNetworkGateway, Compare)
{
  Compare(NetworkGateway({ "5.5.5.5", "6.6.6.6" }, { "FE80:0000:0000:0000:0202:B3FF:FE1E:8329", "FE80::0202:B3FF:FE1E:8329" }));
}

}

}
