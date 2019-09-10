// testauthenticationtype.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/authenticationtype.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestAuthenticationType, ToString)
{
  ASSERT_EQ(std::string(), ToString(AUTHENTICATIONTYPE_INVALID));
  ASSERT_EQ(std::string("BASIC"), ToString(AUTHENTICATIONTYPE_BASIC));
  ASSERT_EQ(std::string("DIGEST"), ToString(AUTHENTICATIONTYPE_DIGEST));
}

TEST(TestAuthenticationType, FromString)
{
  ASSERT_EQ(AUTHENTICATIONTYPE_INVALID, AuthenticationTypeFromString("test"));
  ASSERT_EQ(AUTHENTICATIONTYPE_BASIC, AuthenticationTypeFromString("BASIC"));
  ASSERT_EQ(AUTHENTICATIONTYPE_DIGEST, AuthenticationTypeFromString("DIGEST"));
}

}

}

}
