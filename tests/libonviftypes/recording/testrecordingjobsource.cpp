// testrecordingjobsource.cpp
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

TEST(RecordingTestRecordingJobSource, Compare)
{
  Compare(recording::RecordingJobSource(SourceReference(std::string("type"), std::string("token")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }));
}

}

}
