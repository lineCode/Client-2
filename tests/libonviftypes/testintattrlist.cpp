// testintattrlist.cpp
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

TEST(TestIntAttrList, Compare)
{
  const IntAttrList intattrlist(std::vector<int>({ 1, 3 }));
  ASSERT_EQ(intattrlist, IntAttrList(intattrlist.ToString()));
}

}

}
