// testrecordingjobstatetracks.cpp
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

TEST(RecordingTestRecordingJobStateTracks, Compare)
{
  Compare(recording::RecordingJobStateTracks({
                                               recording::RecordingJobStateTrack(std::string("sourcetag1"), std::string("destination"), std::string("error"), recording::RECORDINGJOBSTATE_ACTIVE),
                                               recording::RecordingJobStateTrack(std::string("sourcetag2"), std::string("destination"), std::string("error"), recording::RECORDINGJOBSTATE_ACTIVE)
                                             }));
}

}

}
