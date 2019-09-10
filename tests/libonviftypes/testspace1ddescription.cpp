// testspace1ddescription.cpp
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

TEST(TestSpace1dDescription, Compare)
{
  Compare(Space1dDescription(std::string("uri"), FloatRange(5.0f, 5.0f)));
}

}

}
