// testfindeventresultlist.cpp
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

TEST(TestFindEventResultList, Compare)
{
  Compare(FindEventResultList(SEARCHSTATE_COMPLETED, { FindEventResult(std::string("RecordingToken"), std::string("TrackToken"), ws::DateTime(1900, 1, 1, 1, 1, 1, 1), Element("wsnt:Event", { { "i", "j" }, { "k", "l" } }, "text3", {}), true) }));
}

}

}
