// testcapabilities.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(ReceiverTestCapabilities, Compare)
{
  Compare(receiver::Capabilities(true, true, true, 5, 5));
  CompareElement(receiver::Capabilities(true, true, true, 5, 5));
}

}

}
