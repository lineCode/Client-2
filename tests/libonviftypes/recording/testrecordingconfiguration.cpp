// testrecordingconfiguration.cpp
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

TEST(RecordingTestRecordingConfiguration, Compare)
{
  Compare(recording::RecordingConfiguration(RecordingSourceInformation(std::string("sourceid"), std::string("name"), std::string("location"), std::string("description"), std::string("address")), std::string("content"), Duration(5, 5, 5, 5, 5, 5, 0)));
}

}

}
