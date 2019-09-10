// testrelayoutputsettings.cpp
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

TEST(TestRelayOutputSettings, Compare)
{
  Compare(RelayOutputSettings(RELAYMODE_MONOSTABLE, Duration(1, 2, 3, 4, 5, 6, 0), RELAYIDLESTATE_OPEN));
}

}

}
