// testcapabilities.cpp
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

TEST(RecordingTestCapabilities, Compare)
{
  Compare(recording::Capabilities(true, true, recording::EncodingTypes({ std::string("encoding1"), std::string("encoding2") }), 5.0f, 6.0f, 5, 6, true, true, StringAttrList(std::vector<std::string>({ "test1", "test" }))));
  CompareElement(recording::Capabilities(true, true, recording::EncodingTypes({ std::string("encoding1"), std::string("encoding2") }), 5.0f, 6.0f, 5, 6, true, true, StringAttrList(std::vector<std::string>({ "test1", "test" }))));
}

}

}
