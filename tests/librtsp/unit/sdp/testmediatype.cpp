// testmediatype.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <rtsp/sdp/mediatype.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

namespace tests
{

///// Tests /////

TEST(TestMediaType, ToString)
{
  ASSERT_EQ(std::string(), ToString(MEDIATYPE_INVALID));
  ASSERT_EQ(std::string("audio"), ToString(MEDIATYPE_AUDIO));
  ASSERT_EQ(std::string("video"), ToString(MEDIATYPE_VIDEO));
  ASSERT_EQ(std::string("text"), ToString(MEDIATYPE_TEXT));
  ASSERT_EQ(std::string("application"), ToString(MEDIATYPE_APPLICATION));
  ASSERT_EQ(std::string("message"), ToString(MEDIATYPE_MESSAGE));
}

TEST(TestMediaType, FromString)
{
  ASSERT_EQ(MEDIATYPE_INVALID, MediaTypeFromString("test"));
  ASSERT_EQ(MEDIATYPE_AUDIO, MediaTypeFromString("audio"));
  ASSERT_EQ(MEDIATYPE_VIDEO, MediaTypeFromString("video"));
  ASSERT_EQ(MEDIATYPE_TEXT, MediaTypeFromString("text"));
  ASSERT_EQ(MEDIATYPE_APPLICATION, MediaTypeFromString("application"));
  ASSERT_EQ(MEDIATYPE_MESSAGE, MediaTypeFromString("message"));
}

}

}

}
