// testscope.cpp
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

TEST(TestScope, Compare)
{
  Compare(Scope(SCOPEDEFINITION_FIXED, std::string("test")));
}

}

}
