// testvideosource.cpp
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

TEST(TestVideoSource, Compare)
{
  Compare(VideoSource(std::string("token"),
                      1.0f,
                      VideoResolution(5, 5),
                      ImagingSettings(BacklightCompensation(BACKLIGHTCOMPENSATIONMODE_OFF, 5.0f),
                                      1.0f,
                                      2.0f,
                                      3.0f,
                                      Exposure(EXPOSUREMODE_MANUAL, EXPOSUREPRIORITY_FRAMERATE, Rectangle(1.0f, 2.0f, 3.0f, 4.0f), 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f),
                                      FocusConfiguration(AUTOFOCUSMODE_AUTO, 4.0f, 5.0f, 6.0f),
                                      IRCUTFILTERMODE_OFF,
                                      4.0f,
                                      WideDynamicRange(WIDEDYNAMICMODE_OFF, 4.0f),
                                      WhiteBalance(WHITEBALANCEMODE_AUTO, 4.0f, 5.0f)),
                      ImagingSettings20(BacklightCompensation20(BACKLIGHTCOMPENSATIONMODE_OFF, 5.0f),
                                        1.0f,
                                        2.0f,
                                        3.0f,
                                        Exposure20(EXPOSUREMODE_MANUAL, EXPOSUREPRIORITY_FRAMERATE, Rectangle(1.0f, 2.0f, 3.0f, 4.0f), 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f),
                                        FocusConfiguration20(AUTOFOCUSMODE_AUTO, 4.0f, 5.0f, 6.0f),
                                        IRCUTFILTERMODE_OFF,
                                        4.0f,
                                        WideDynamicRange20(WIDEDYNAMICMODE_OFF, 4.0f),
                                        WhiteBalance20(WHITEBALANCEMODE_AUTO, 4.0f, 5.0f),
                                        ImageStabilization(IMAGESTABILIZATIONMODE_OFF, 5.0f),
                                        IrCutFilterAutoAdjustment(std::string("boundarytype"), 0.5f, Duration(0, 1, 2, 3, 4, 5, 0)),
                                        ToneCompensation(TONECOMPENSATIONMODE_ON, 0.5f),
                                        Defogging(DEFOGGINGMODE_ON, 0.5f),
                                        NoiseReduction(0.5f))));
}

}

}
