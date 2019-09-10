// testspace2ddescription.cpp
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

TEST(TestSpace2dDescription, Compare)
{
  Compare(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f)));
}

}

}
