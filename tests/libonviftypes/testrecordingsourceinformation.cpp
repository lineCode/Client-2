// testrecordingsourceinformation.cpp
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

TEST(RecordingTestRecordingSourceInformation, Compare)
{
  Compare(RecordingSourceInformation(std::string("sourceid"), std::string("name"), std::string("location"), std::string("description"), std::string("address")));
}

}

}
