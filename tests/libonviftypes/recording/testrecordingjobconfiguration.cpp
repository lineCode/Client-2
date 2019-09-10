// testrecordingjobconfiguration.cpp
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

TEST(RecordingTestRecordingJobConfiguration, Compare)
{
  Compare(recording::RecordingJobConfiguration(std::string("RecordingToken"),
                                               recording::RECORDINGJOBMODE_ACTIVE,
                                               5,
                                               {
                                                 recording::RecordingJobSource(SourceReference(std::string("type1"), std::string("token1")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }),
                                                 recording::RecordingJobSource(SourceReference(std::string("type2"), std::string("token2")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) })
                                               }));
}

}

}
