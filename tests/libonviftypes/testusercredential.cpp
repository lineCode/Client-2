// testusercredential.cpp
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

TEST(TestUserCredential, Compare)
{
  Compare(UserCredential(std::string("username"), std::string("password")));
}

}

}
