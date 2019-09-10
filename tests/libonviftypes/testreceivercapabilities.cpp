// testreceivercapabilities.cpp
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

TEST(TestReceiverCapabilities, Compare)
{
  Compare(ReceiverCapabilities(std::string("xaddr"), true, true, true, 5, 5));
}

}

}
