// testencodingtypes.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(MediaTestEncodingTypes, Compare)
{
  const media::EncodingTypes encodingtypes(std::vector<VIDEOENCODING>({ VIDEOENCODING_JPEG, VIDEOENCODING_MPEG4 }));
  ASSERT_EQ(encodingtypes, media::EncodingTypes(encodingtypes.ToString()));
}

}

}
