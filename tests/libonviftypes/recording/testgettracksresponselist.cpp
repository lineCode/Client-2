// testgettracksresponselist.cpp
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

TEST(RecordingTestGetTracksResponseList, Compare)
{
  Compare(recording::GetTracksResponseList({
                                             recording::GetTracksResponseItem(std::string("TrackToken1"), TrackConfiguration(TRACKTYPE_AUDIO, std::string("description1"))),
                                             recording::GetTracksResponseItem(std::string("TrackToken2"), TrackConfiguration(TRACKTYPE_AUDIO, std::string("description2")))
                                           }));
}

}

}
