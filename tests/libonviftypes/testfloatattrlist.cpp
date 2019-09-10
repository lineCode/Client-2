// testfloatattrlist.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestFloatAttrList, Compare)
{
  const FloatAttrList floatattrlist(std::vector<float>({ 5.0f, 6.0f }));
  ASSERT_EQ(floatattrlist, FloatAttrList(floatattrlist.ToString()));
}

}

}
