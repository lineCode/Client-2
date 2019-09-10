// testimagingclient.cpp
//

///// Includes /////

#include "testimagingclient.hpp"

#include <boost/optional.hpp>
#include <onviftypes/onviftypes.hpp>
#include <string>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Globals /////

const unsigned short PORT = 8000;
const std::string IMAGINGURI("/onvif/imaging");

///// Methods /////

TestImagingClient::TestImagingClient()
{
  
}

TestImagingClient::~TestImagingClient()
{
  
}

void TestImagingClient::SetUp()
{
  Init(PORT, IMAGINGURI);

  // Setup dummy server
  dummyimagingservice_ = std::make_unique<DummyImagingService>();
  ASSERT_FALSE(httpserver_.AddServer(IMAGINGURI, dummyimagingservice_.get()));
}

void TestImagingClient::TearDown()
{
  ASSERT_FALSE(httpserver_.RemoveServer(IMAGINGURI));
  dummyimagingservice_.reset();

  Destroy();
}

///// Tests /////

TEST_F(TestImagingClient, GetImagingSettings)
{
/*  const std::string videosourcetoken("videosourcetoken");
  const ImagingSettings20 imagingsettings(BacklightCompensation20(BACKLIGHTCOMPENSATIONMODE_OFF, 5.0f), 1.0f, 2.0f, 3.0f, Exposure20(EXPOSUREMODE_MANUAL, EXPOSUREPRIORITY_FRAMERATE, Rectangle(1.0f, 2.0f, 3.0f, 4.0f), 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f), FocusConfiguration20(AUTOFOCUSMODE_AUTO, 4.0f, 5.0f, 6.0f), IRCUTFILTERMODE_OFF, 4.0f, WideDynamicRange20(WIDEDYNAMICMODE_OFF, 4.0f), WhiteBalance20(WHITEBALANCEMODE_AUTO, 4.0f, 5.0f), ImageStabilization(IMAGESTABILIZATIONMODE_OFF, 5.0f), IrCutFilterAutoAdjustment(std::string("boundarytype"), 0.5f, Duration(0, 1, 2, 3, 4, 5)), ToneCompensation(TONECOMPENSATIONMODE_ON, 0.5f), Defogging(DEFOGGINGMODE_ON, 0.5f), NoiseReduction(0.5f));
  dummyimagingservice_->SetGetImagingSettings(imagingsettings);

  imaging::GetImagingSettingsResponse getimagingsettingsresponse = client_->GetImagingSettingsFuture(videosourcetoken).get();
  ASSERT_GE(getimagingsettingsresponse.latency_, 0);
  ASSERT_FALSE(getimagingsettingsresponse.Error());
  ASSERT_EQ(videosourcetoken, getimagingsettingsresponse.videosourcetoken_);
  ASSERT_TRUE(getimagingsettingsresponse.imagingsettings_.is_initialized());
  ASSERT_EQ(imagingsettings, *getimagingsettingsresponse.imagingsettings_);*/
}

TEST_F(TestImagingClient, GetOptions)
{
  const std::string videosourcetoken("videosourcetoken");
  const ImagingOptions20 imagingoptions(BacklightCompensationOptions20({ BACKLIGHTCOMPENSATIONMODE_ON, BACKLIGHTCOMPENSATIONMODE_OFF }, FloatRange(5.0f, 6.0f)), FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f), ExposureOptions20({ EXPOSUREMODE_AUTO, EXPOSUREMODE_MANUAL }, { EXPOSUREPRIORITY_LOWNOISE }, FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f),  FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f)), FocusOptions20({ AUTOFOCUSMODE_AUTO, AUTOFOCUSMODE_MANUAL }, FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f)), { IRCUTFILTERMODE_OFF, IRCUTFILTERMODE_AUTO }, FloatRange(5.0f, 6.0f), WideDynamicRangeOptions20({ WIDEDYNAMICMODE_ON, WIDEDYNAMICMODE_OFF }, FloatRange(5.0f, 6.0f)), WhiteBalanceOptions20({ WHITEBALANCEMODE_AUTO, WHITEBALANCEMODE_MANUAL }, FloatRange(5.0f, 6.0f), FloatRange(5.0f, 6.0f)), ImageStabilizationOptions({ IMAGESTABILIZATIONMODE_ON, IMAGESTABILIZATIONMODE_AUTO }, FloatRange(5.0f, 6.0f)), IrCutFilterAutoAdjustmentOptions({ IRCUTFILTERAUTOBOUNDARYTYPE_TOON, IRCUTFILTERAUTOBOUNDARYTYPE_TOOFF }, true, DurationRange(Duration(0, 1, 2, 3, 4, 5), Duration(1, 2, 3, 4, 5, 6))), ToneCompensationOptions({ TONECOMPENSATIONMODE_ON, TONECOMPENSATIONMODE_AUTO }, true), DefoggingOptions({ DEFOGGINGMODE_ON, DEFOGGINGMODE_AUTO }, true), NoiseReductionOptions(true));
  dummyimagingservice_->SetGetOptions(imagingoptions);

  imaging::GetOptionsResponse getoptionsresponse = client_->GetOptionsFuture(videosourcetoken).get();
  ASSERT_GE(getoptionsresponse.latency_, 0);
  ASSERT_FALSE(getoptionsresponse.Error());
  ASSERT_EQ(videosourcetoken, getoptionsresponse.videosourcetoken_);
  ASSERT_TRUE(getoptionsresponse.imagingoptions_.is_initialized());
  ASSERT_EQ(imagingoptions, *getoptionsresponse.imagingoptions_);
}

TEST_F(TestImagingClient, GetServiceCapabilities)
{
  const imaging::Capabilities capabilities(true);
  dummyimagingservice_->SetGetServiceCapabilities(capabilities);

  imaging::GetServiceCapabilitiesResponse getimagingservicecapabilitiesresponse = client_->GetServiceCapabilitiesFuture().get();
  ASSERT_GE(getimagingservicecapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getimagingservicecapabilitiesresponse.Error());
  ASSERT_TRUE(getimagingservicecapabilitiesresponse.capabilities_.is_initialized());
  ASSERT_EQ(capabilities, *getimagingservicecapabilitiesresponse.capabilities_);
}

TEST_F(TestImagingClient, SetImagingSettings)
{
  const std::string videosourcetoken("videosourcetoken");
  const ImagingSettings20 imagingsettings(BacklightCompensation20(BACKLIGHTCOMPENSATIONMODE_OFF, 5.0f), 1.0f, 2.0f, 3.0f, Exposure20(EXPOSUREMODE_MANUAL, EXPOSUREPRIORITY_FRAMERATE, Rectangle(1.0f, 2.0f, 3.0f, 4.0f), 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f), FocusConfiguration20(AUTOFOCUSMODE_AUTO, 4.0f, 5.0f, 6.0f), IRCUTFILTERMODE_OFF, 4.0f, WideDynamicRange20(WIDEDYNAMICMODE_OFF, 4.0f), WhiteBalance20(WHITEBALANCEMODE_AUTO, 4.0f, 5.0f), ImageStabilization(IMAGESTABILIZATIONMODE_OFF, 5.0f), IrCutFilterAutoAdjustment(std::string("boundarytype"), 0.5f, Duration(0, 1, 2, 3, 4, 5)), ToneCompensation(TONECOMPENSATIONMODE_ON, 0.5f), Defogging(DEFOGGINGMODE_ON, 0.5f), NoiseReduction(0.5f));
  const bool forcepersistence = true;

  imaging::SetImagingSettingsResponse setimagingsettingsresponse = client_->SetImagingSettingsFuture(videosourcetoken, imagingsettings, forcepersistence).get();
  ASSERT_GE(setimagingsettingsresponse.latency_, 0);
  ASSERT_FALSE(setimagingsettingsresponse.Error());
  ASSERT_EQ(videosourcetoken, setimagingsettingsresponse.videosourcetoken_);
  ASSERT_EQ(imagingsettings, setimagingsettingsresponse.imagingsettings_);
  ASSERT_EQ(forcepersistence, setimagingsettingsresponse.forcepersistence_);
}

}

}
