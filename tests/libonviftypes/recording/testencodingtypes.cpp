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

TEST(RecordingTestEncodingTypes, Compare)
{
  const recording::EncodingTypes encodingtypes({ std::string("encoding1"), std::string("encoding2") });
  ASSERT_EQ(encodingtypes, recording::EncodingTypes(encodingtypes.ToString()));
}

}

}
