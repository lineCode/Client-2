// testsimpleitem.cpp
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

TEST(TestSimpleItem, Compare)
{
  Compare(SimpleItem(std::string("name"), std::string("value")));
}

}

}
