// testprefixedipv4address.cpp
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

TEST(TestPrefixedIPv4Address, Compare)
{
  Compare(PrefixedIPv4Address(std::string("5.5.5.5"), 5));
}

}

}
