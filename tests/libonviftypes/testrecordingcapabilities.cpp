// test.cpp
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

TEST(TestRecordingCapabilities, Compare)
{
  Compare(RecordingCapabilities(std::string("xaddr"), true, true, true, true, 5));
}

}

}
