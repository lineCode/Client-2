// testdot11securityconfiguration.cpp
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

TEST(TestDot11SecurityConfiguration, Compare)
{
  Compare(Dot11SecurityConfiguration(DOT11SECURITYMODE_WEP, DOT11CIPHER_TKIP, Dot11PSKSet(std::string("key"), std::string("passphrase")), std::string("dot1x")));
}

}

}
