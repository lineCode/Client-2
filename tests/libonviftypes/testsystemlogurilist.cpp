// testsystemlogurilist.cpp
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

TEST(TestSystemLogUriList, Compare)
{
  Compare(SystemLogUriList({ SystemLogUri(SYSTEMLOGLEVEL_ACCESS, std::string("uri1")), SystemLogUri(SYSTEMLOGLEVEL_SYSTEM, std::string("uri2"))}));
}

}

}
