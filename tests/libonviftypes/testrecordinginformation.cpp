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

TEST(RecordingTestRecordingInformation, Compare)
{
  Compare(RecordingInformation(std::string("recordingtoken"), RecordingSourceInformation(std::string("sourceid"), std::string("name"), std::string("location"), std::string("description"), std::string("address")), ws::DateTime(1900, 1, 2, 3, 4, 5, 6), ws::DateTime(1901, 1, 2, 3, 4, 5, 6), std::string("content"), std::vector<TrackInformation>({ TrackInformation(std::string("1"), TRACKTYPE_VIDEO, std::string("description"), ws::DateTime(1900, 1, 2, 3, 4, 5, 6), ws::DateTime(1901, 1, 2, 3, 4, 5, 6)), TrackInformation(std::string("2"), TRACKTYPE_VIDEO, std::string("description"), ws::DateTime(1900, 1, 2, 3, 4, 5, 6), ws::DateTime(1901, 1, 2, 3, 4, 5, 6)) }), RECORDINGSTATUS_RECORDING));
}

}

}
