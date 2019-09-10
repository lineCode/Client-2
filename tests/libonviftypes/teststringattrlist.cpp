// teststringattrlist.cpp
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

TEST(TestStringAttrList, Compare)
{
  const StringAttrList stringattrlist(std::vector<std::string>({ "test1", "test" }));
  ASSERT_EQ(stringattrlist, StringAttrList(stringattrlist.ToString()));
}

}

}
