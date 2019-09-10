// testnetworkinterfaceinfo.cpp
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

TEST(TestNetworkInterfaceInfo, Compare)
{
  Compare(NetworkInterfaceInfo(std::string("name"), std::string("hwaddress"), 5));
}

}

}
