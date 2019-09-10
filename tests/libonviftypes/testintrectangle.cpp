// testintrectangle.cpp
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

TEST(TestIntRectangle, Compare)
{
  Compare(IntRectangle(5, 5, 5, 5));
}

}

}
