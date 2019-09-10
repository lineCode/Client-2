// testnetworkinterfacelink.cpp
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

TEST(TestNetworkInterfaceLink, Compare)
{
  Compare(NetworkInterfaceLink(NetworkInterfaceConnectionSetting(true, 5, DUPLEX_HALF), NetworkInterfaceConnectionSetting(true, 5, DUPLEX_HALF), 5));
}

}

}
