// testptzconfiguration.cpp
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

TEST(TestPTZConfiguration, Compare)
{
  Compare(PTZConfiguration(std::string("name"), 5, std::string("token"), std::string("nodetoken"), std::string("defaultabsolutepantiltpositionspace"), std::string("defaultabsolutezoompositionspace"), std::string("defaultrelativepantilttranslationspace"), std::string("defaultrelativezoomtranslationspace"), std::string("defaultcontinuouspantiltvelocityspace"), std::string("defaultcontinuouszoomvelocityspace"), DefaultPTZSpeed(Vector2d(5, 5, std::string("space")), 5), std::string("PT5S"), PanTiltLimits(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f))), ZoomLimits(Space1dDescription(std::string("uri"), FloatRange(5.0f, 5.0f)))));
}

}

}
