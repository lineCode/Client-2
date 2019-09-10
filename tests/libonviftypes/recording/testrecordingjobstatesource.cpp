// testrecordingjobstatesource.cpp
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

TEST(RecordingTestRecordingJobStateSource, Compare)
{
  Compare(recording::RecordingJobStateSource(SourceReference(std::string("type"), std::string("token")),
                                             recording::RECORDINGJOBSTATE_ACTIVE,
                                             recording::RecordingJobStateTracks({
                                                                                  recording::RecordingJobStateTrack(std::string("sourcetag1"), std::string("destination"), std::string("error"), recording::RECORDINGJOBSTATE_ACTIVE),
                                                                                  recording::RecordingJobStateTrack(std::string("sourcetag2"), std::string("destination"), std::string("error"), recording::RECORDINGJOBSTATE_ACTIVE)
                                                                                })));
}

}

}
