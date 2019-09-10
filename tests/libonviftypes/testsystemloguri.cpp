// testsystemloguri.cpp
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

TEST(TestSystemLogUri, Compare)
{
  Compare(SystemLogUri(SYSTEMLOGLEVEL_ACCESS, std::string("uri")));
}

}

}
