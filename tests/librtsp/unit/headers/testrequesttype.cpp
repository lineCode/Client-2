// testrequesttype.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/headers/requesttype.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

namespace tests
{

///// Tests /////

TEST(TestRequestType, ToString)
{
  ASSERT_EQ(std::string(), ToString(REQUESTTYPE_INVALID));
  ASSERT_EQ(std::string("DESCRIBE"), ToString(REQUESTTYPE_DESCRIBE));
  ASSERT_EQ(std::string("GET_PARAMETER"), ToString(REQUESTTYPE_GETPARAMETER));
  ASSERT_EQ(std::string("OPTIONS"), ToString(REQUESTTYPE_OPTIONS));
  ASSERT_EQ(std::string("PAUSE"), ToString(REQUESTTYPE_PAUSE));
  ASSERT_EQ(std::string("PLAY"), ToString(REQUESTTYPE_PLAY));
  ASSERT_EQ(std::string("SETUP"), ToString(REQUESTTYPE_SETUP));
  ASSERT_EQ(std::string("SET_PARAMETER"), ToString(REQUESTTYPE_SETPARAMETER));
  ASSERT_EQ(std::string("TEARDOWN"), ToString(REQUESTTYPE_TEARDOWN));
}

TEST(TestRequestType, FromString)
{
  ASSERT_EQ(REQUESTTYPE_INVALID, RequestTypeFromString("test"));
  ASSERT_EQ(REQUESTTYPE_DESCRIBE, RequestTypeFromString("DESCRIBE"));
  ASSERT_EQ(REQUESTTYPE_GETPARAMETER, RequestTypeFromString("GET_PARAMETER"));
  ASSERT_EQ(REQUESTTYPE_OPTIONS, RequestTypeFromString("OPTIONS"));
  ASSERT_EQ(REQUESTTYPE_PAUSE, RequestTypeFromString("PAUSE"));
  ASSERT_EQ(REQUESTTYPE_PLAY, RequestTypeFromString("PLAY"));
  ASSERT_EQ(REQUESTTYPE_SETUP, RequestTypeFromString("SETUP"));
  ASSERT_EQ(REQUESTTYPE_SETPARAMETER, RequestTypeFromString("SET_PARAMETER"));
  ASSERT_EQ(REQUESTTYPE_TEARDOWN, RequestTypeFromString("TEARDOWN"));
}

}

}

}
