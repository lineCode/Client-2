// testwhitebalanceoptions20.cpp
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

TEST(TestWhiteBalanceOptions20, Compare)
{
  Compare(WhiteBalanceOptions20({ WHITEBALANCEMODE_AUTO, WHITEBALANCEMODE_MANUAL }, FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f)));
}

}

}
