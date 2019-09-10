// testuser.cpp
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

TEST(TestUser, Compare)
{
  Compare(User(std::string("test"), std::string("test"), USERLEVEL_ADMINISTRATOR));
}

}

}
