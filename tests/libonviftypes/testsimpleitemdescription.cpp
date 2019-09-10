// testsimpleitemdescription.cpp
//

///// Includes /////

#include <types.hpp>
#include <gtest/gtest.h>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestSimpleItemDescription, Compare)
{
  Compare(SimpleItemDescription(std::string("name"), std::string("type")));
}

}

}
