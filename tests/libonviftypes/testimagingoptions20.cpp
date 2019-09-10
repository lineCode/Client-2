// testimagingoptions20.cpp
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

TEST(TestImagingOptions20, Compare)
{
  Compare(ImagingOptions20(BacklightCompensationOptions20({ BACKLIGHTCOMPENSATIONMODE_ON, BACKLIGHTCOMPENSATIONMODE_OFF }, FloatRange(5.0f, 6.0f)),
                           FloatRange(5.0f, 6.0f),
                           FloatRange(5.0f, 6.0f),
                           FloatRange(5.0f, 6.0f),
                           ExposureOptions20({ EXPOSUREMODE_AUTO, EXPOSUREMODE_MANUAL },
                                             { EXPOSUREPRIORITY_LOWNOISE },
                                             FloatRange(5.0f, 6.0f),
                                             FloatRange(5.0f, 6.0f),
                                             FloatRange(5.0f, 6.0f),
                                             FloatRange(5.0f, 6.0f),
                                             FloatRange(5.0f, 6.0f),
                                             FloatRange(5.0f, 6.0f),
                                             FloatRange(5.0f, 6.0f),
                                             FloatRange(5.0f, 6.0f),
                                             FloatRange(5.0f, 6.0f)),
                           FocusOptions20({ AUTOFOCUSMODE_AUTO, AUTOFOCUSMODE_MANUAL }, FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f)),
                           { IRCUTFILTERMODE_OFF, IRCUTFILTERMODE_AUTO },
                           FloatRange(5.0f, 6.0f),
                           WideDynamicRangeOptions20({ WIDEDYNAMICMODE_ON, WIDEDYNAMICMODE_OFF }, FloatRange(5.0f, 6.0f)),
                           WhiteBalanceOptions20({ WHITEBALANCEMODE_AUTO, WHITEBALANCEMODE_MANUAL }, FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f)),
                           ImageStabilizationOptions({ IMAGESTABILIZATIONMODE_ON, IMAGESTABILIZATIONMODE_AUTO }, FloatRange(5.0f, 6.0f)),
                           IrCutFilterAutoAdjustmentOptions({ IRCUTFILTERAUTOBOUNDARYTYPE_TOON, IRCUTFILTERAUTOBOUNDARYTYPE_TOOFF }, true, DurationRange(Duration(0, 1, 2, 3, 4, 5, 0), Duration(1, 2, 3, 4, 5, 6, 0))),
                           ToneCompensationOptions({ TONECOMPENSATIONMODE_ON, TONECOMPENSATIONMODE_AUTO }, true),
                           DefoggingOptions({ DEFOGGINGMODE_ON, DEFOGGINGMODE_AUTO }, true),
                           NoiseReductionOptions(true)));
}

}

}
