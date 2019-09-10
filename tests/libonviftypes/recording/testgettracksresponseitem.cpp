// testgettracksresponseitem.cpp
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

TEST(RecordingTestGetTracksResponseItem, Compare)
{
  Compare(recording::GetTracksResponseItem(std::string("TrackToken"), TrackConfiguration(TRACKTYPE_AUDIO, std::string("description"))));
}

}

}
