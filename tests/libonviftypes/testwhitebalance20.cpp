// testwhitebalance20.cpp
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

TEST(TestWhiteBalance20, Compare)
{
  Compare(WhiteBalance20(WHITEBALANCEMODE_AUTO, 4.0f, 5.0f));
}

}

}
