// testtrackconfiguration.cpp
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

TEST(RecordingTestTrackConfiguration, Compare)
{
  Compare(TrackConfiguration(TRACKTYPE_AUDIO, std::string("description")));
}

}

}
