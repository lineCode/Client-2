// testimagingsettings.cpp
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

TEST(TestImagingSettings, Compare)
{
  Compare(ImagingSettings(BacklightCompensation(BACKLIGHTCOMPENSATIONMODE_OFF, 5.0f),
                          1.0f,
                          2.0f,
                          3.0f,
                          Exposure(EXPOSUREMODE_MANUAL, EXPOSUREPRIORITY_FRAMERATE, Rectangle(1.0f, 2.0f, 3.0f, 4.0f), 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f),
                          FocusConfiguration(AUTOFOCUSMODE_AUTO, 4.0f, 5.0f, 6.0f),
                          IRCUTFILTERMODE_OFF,
                          4.0f,
                          WideDynamicRange(WIDEDYNAMICMODE_OFF, 4.0f),
                          WhiteBalance(WHITEBALANCEMODE_AUTO, 4.0f, 5.0f)));
}

}

}
