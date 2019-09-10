// testnetworkinterfaceconnectionsetting.cpp
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

TEST(TestNetworkInterfaceConnectionSetting, Compare)
{
  Compare(NetworkInterfaceConnectionSetting(true, 5, DUPLEX_HALF));
}

}

}
