// testwhitebalance.cpp
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

TEST(TestWhiteBalance, Compare)
{
  Compare(WhiteBalance(WHITEBALANCEMODE_AUTO, 4.0f, 5.0f));
}

}

}
