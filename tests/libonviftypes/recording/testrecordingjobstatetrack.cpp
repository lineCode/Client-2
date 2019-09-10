// testrecordingjobstatetrack.cpp
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

TEST(RecordingTestRecordingJobStateTrack, Compare)
{
  Compare(recording::RecordingJobStateTrack(std::string("sourcetag"), std::string("destination"), std::string("error"), recording::RECORDINGJOBSTATE_ACTIVE));
}

}

}
