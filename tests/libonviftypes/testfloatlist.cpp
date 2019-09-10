// testintlist.cpp
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

TEST(TestFloatList, Compare)
{
  Compare(FloatList({ 5.0f, 6.0f, 7.0f }));
}

}

}
