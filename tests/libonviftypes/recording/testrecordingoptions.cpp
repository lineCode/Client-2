// testrecordingoptions.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(RecordingTestRecordingOptions, Compare)
{
  Compare(recording::RecordingOptions(recording::JobOptions(5, StringAttrList(std::vector<std::string>({ "test1", "test2" }))), recording::TrackOptions(5, 5, 5, 5)));
}

}

}
