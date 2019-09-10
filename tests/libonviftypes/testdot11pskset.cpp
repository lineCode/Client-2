// testdot11pskset.cpp
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

TEST(TestDot11PSKSet, Compare)
{
  Compare(Dot11PSKSet(std::string("key"), std::string("passphrase")));
}

}

}
