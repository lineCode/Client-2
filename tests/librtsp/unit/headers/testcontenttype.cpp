// testcontenttype.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/contenttype.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestContentType, ToString)
{
  ASSERT_EQ(std::string(), ToString(CONTENTTYPE_INVALID));
  ASSERT_EQ(std::string("application/sdp"), ToString(CONTENTTYPE_APPLICATIONSDP));
  ASSERT_EQ(std::string("text/parameters"), ToString(CONTENTTYPE_TEXTPARAMETERS));
}

TEST(TestContentType, FromString)
{
  ASSERT_EQ(CONTENTTYPE_INVALID, ContentTypeFromString("test"));
  ASSERT_EQ(CONTENTTYPE_APPLICATIONSDP, ContentTypeFromString("application/sdp"));
  ASSERT_EQ(CONTENTTYPE_TEXTPARAMETERS, ContentTypeFromString("text/parameters"));
}

}

}

}
