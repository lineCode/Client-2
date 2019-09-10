// testremoteuser.cpp
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

TEST(TestRemoteUser, Compare)
{
  Compare(RemoteUser(std::string("username"), std::string("password"), true));
}

}

}
