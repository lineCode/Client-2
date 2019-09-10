// testtrackoptions.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(RecordingTestTrackOptions, Compare)
{
  Compare(recording::TrackOptions(5, 5, 5, 5));
}

}

}
