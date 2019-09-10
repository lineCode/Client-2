// testvector2d.cpp
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

TEST(TestVector2d, Compare)
{
  Compare(Vector2d(5, 5, std::string("space")));
}

}

}
