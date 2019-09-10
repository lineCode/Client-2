// testrecordingjobtrack.cpp
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

TEST(RecordingTestRecordingJobTrack, Compare)
{
  Compare(recording::RecordingJobTrack(std::string("sourcetag"), std::string("destination")));
}

}

}
