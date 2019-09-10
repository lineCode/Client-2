// testintrectanglerange.cpp
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

TEST(TestIntRectangleRange, Compare)
{
  Compare(IntRectangleRange(IntRange(5, 6), IntRange(7, 8), IntRange(9, 10), IntRange(boost::none, 6)));
}

}

}
