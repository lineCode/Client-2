// testmulticastconfiguration.cpp
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

TEST(TestMulticastConfiguration, Compare)
{
  Compare(MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true));
}

}

}
