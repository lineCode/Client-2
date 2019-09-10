// testgetrecordingsresponseitem.cpp
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

TEST(RecordingTestGetRecordingsResponseItem, Compare)
{
  Compare(recording::GetRecordingsResponseItem(std::string("RecordingToken"), recording::RecordingConfiguration(RecordingSourceInformation(std::string("sourceid"), std::string("name"), std::string("location"), std::string("description"), std::string("address")), std::string("content"), Duration(5, 5, 5, 5, 5, 5, 0)), recording::GetTracksResponseList({ recording::GetTracksResponseItem(std::string("TrackToken1"), TrackConfiguration(TRACKTYPE_AUDIO, std::string("description1"))), recording::GetTracksResponseItem(std::string("TrackToken2"), TrackConfiguration(TRACKTYPE_AUDIO, std::string("description2"))) })));
}

}

}
