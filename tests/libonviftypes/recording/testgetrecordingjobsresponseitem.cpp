// testgetrecordingjobsresponseitem.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <types.hpp>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(RecordingTestRecordingJobResponseItem, Compare)
{
  Compare(recording::getrecordingjobsresponseitem(std::string("JobToken"),
                                                  recording::RecordingJobConfiguration(std::string("RecordingToken"),
                                                  recording::RECORDINGJOBMODE_ACTIVE,
                                                  5,
                                                  {
                                                    recording::RecordingJobSource(recording::SourceReference(std::string("type1"), std::string("token1")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }),
                                                    recording::RecordingJobSource(recording::SourceReference(std::string("type2"), std::string("token2")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) })
                                                  })));
}

}

}
