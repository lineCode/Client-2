// testprefixedipv6address.cpp
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

TEST(TestPrefixedIPv6Address, Compare)
{
  Compare(PrefixedIPv6Address(std::string("FE80:0000:0000:0000:0202:B3FF:FE1E:8329"), 5));
}

}

}
