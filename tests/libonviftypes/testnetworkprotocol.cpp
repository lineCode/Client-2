// testnetworkprotocol.cpp
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

TEST(TestNetworkProtocol, Compare)
{
  Compare(NetworkProtocol(NETWORKPROTOCOLTYPE_HTTPS, true, { 5, 6 }));
}

}

}
