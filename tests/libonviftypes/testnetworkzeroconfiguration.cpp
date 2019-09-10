// testnetworkzeroconfiguration.cpp
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

TEST(TestNetworkZeroConfiguration, Compare)
{
  Compare(NetworkZeroConfiguration(std::string("interfacetoken1"),
                                   true,
                                   { std::string("5.5.5.5"), std::string("6.6.6.6") },
                                   { NetworkZeroConfiguration(std::string("interfacetoken2"), true, { std::string("6.6.6.6"), std::string("7.7.7.7") }, { }) }));
}

}

}
