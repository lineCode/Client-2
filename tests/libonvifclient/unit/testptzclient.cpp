// testptzclient.cpp
//

///// Includes /////

#include "testptzclient.hpp"

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
const std::string PTZURI("/onvif/ptz");

///// Methods /////

TestPTZClient::TestPTZClient()
{
  
}

TestPTZClient::~TestPTZClient()
{
  
}

void TestPTZClient::SetUp()
{
  Init(PORT, PTZURI);

  // Setup dummy server
  dummyptzservice_ = std::make_unique<DummyPTZService>();
  ASSERT_FALSE(httpserver_.AddServer(PTZURI, dummyptzservice_.get()));
}

void TestPTZClient::TearDown()
{
  ASSERT_FALSE(httpserver_.RemoveServer(PTZURI));
  dummyptzservice_.reset();

  Destroy();
}

///// Tests /////

TEST_F(TestPTZClient, ContinuousMove)
{
  const std::string profiletoken("profiletoken");
  const PTZSpeed velocity(Vector2d(1, 2, std::string("space")), Vector1d(1, std::string("space")));
  const onvif::Duration duration(1, 2, 3, 4, 5, 6);

  ptz::ContinuousMoveResponse continuousmoveresponse = client_->ContinuousMoveFuture(profiletoken, velocity, duration).get();
  ASSERT_GE(continuousmoveresponse.latency_, 0);
  ASSERT_FALSE(continuousmoveresponse.Error()) << continuousmoveresponse.Message();
  ASSERT_EQ(profiletoken, continuousmoveresponse.profiletoken_);
  ASSERT_EQ(velocity, continuousmoveresponse.velocity_);
}

TEST_F(TestPTZClient, GetCompatibleConfigurations)
{
  const std::string profiletoken("profiletoken");
  const std::vector<PTZConfiguration> ptzconfigurations =
  {
    PTZConfiguration(std::string("name1"), 5, std::string("token1"), std::string("nodetoken"), std::string("defaultabsolutepantiltpositionspace"), std::string("defaultabsolutezoompositionspace"), std::string("defaultrelativepantilttranslationspace"), std::string("defaultrelativezoomtranslationspace"), std::string("defaultcontinuouspantiltvelocityspace"), std::string("defaultcontinuouszoomvelocityspace"), DefaultPTZSpeed(Vector2d(5, 5, std::string("space")), 5), std::string("PT5S"), PanTiltLimits(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f))), ZoomLimits(Space1dDescription(std::string("uri"), FloatRange(5.0f, 5.0f)))),
    PTZConfiguration(std::string("name2"), 5, std::string("token2"), std::string("nodetoken"), std::string("defaultabsolutepantiltpositionspace"), std::string("defaultabsolutezoompositionspace"), std::string("defaultrelativepantilttranslationspace"), std::string("defaultrelativezoomtranslationspace"), std::string("defaultcontinuouspantiltvelocityspace"), std::string("defaultcontinuouszoomvelocityspace"), DefaultPTZSpeed(Vector2d(5, 5, std::string("space")), 5), std::string("PT5S"), PanTiltLimits(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f))), ZoomLimits(Space1dDescription(std::string("uri"), FloatRange(5.0f, 5.0f))))
  };
  dummyptzservice_->SetGetCompatibleConfigurations(ptzconfigurations);

  ptz::GetCompatibleConfigurationsResponse getcompatibleconfigurationsresponse = client_->GetCompatibleConfigurationsFuture(profiletoken).get();
  ASSERT_GE(getcompatibleconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getcompatibleconfigurationsresponse.Error());
  ASSERT_EQ(profiletoken, getcompatibleconfigurationsresponse.profiletoken_);
  ASSERT_EQ(ptzconfigurations, getcompatibleconfigurationsresponse.ptzconfigurations_);
}

TEST_F(TestPTZClient, GetConfiguration)
{
  const PTZConfiguration ptzconfiguration(std::string("name"), 5, std::string("token"), std::string("nodetoken"), std::string("defaultabsolutepantiltpositionspace"), std::string("defaultabsolutezoompositionspace"), std::string("defaultrelativepantilttranslationspace"), std::string("defaultrelativezoomtranslationspace"), std::string("defaultcontinuouspantiltvelocityspace"), std::string("defaultcontinuouszoomvelocityspace"), DefaultPTZSpeed(Vector2d(5, 5, std::string("space")), 5), std::string("PT5S"), PanTiltLimits(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f))), ZoomLimits(Space1dDescription(std::string("uri"), FloatRange(5.0f, 5.0f))));
  dummyptzservice_->SetGetConfiguration(ptzconfiguration);

  ptz::GetConfigurationResponse getconfigurationresponse = client_->GetConfigurationFuture(*ptzconfiguration.token_).get();
  ASSERT_GE(getconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getconfigurationresponse.Error()) << getconfigurationresponse.Message();
  ASSERT_EQ(*ptzconfiguration.token_, getconfigurationresponse.ptzconfigurationtoken_);
  ASSERT_EQ(ptzconfiguration, *getconfigurationresponse.ptzconfiguration_);
}

TEST_F(TestPTZClient, GetConfigurations)
{
  const std::vector<PTZConfiguration> ptzconfiguration =
  {
    PTZConfiguration(std::string("name"), 5, std::string("token"), std::string("nodetoken"), std::string("defaultabsolutepantiltpositionspace"), std::string("defaultabsolutezoompositionspace"), std::string("defaultrelativepantilttranslationspace"), std::string("defaultrelativezoomtranslationspace"), std::string("defaultcontinuouspantiltvelocityspace"), std::string("defaultcontinuouszoomvelocityspace"), DefaultPTZSpeed(Vector2d(5, 5, std::string("space")), 5), std::string("PT5S"), PanTiltLimits(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f))), ZoomLimits(Space1dDescription(std::string("uri"), FloatRange(5.0f, 5.0f))))
  };
  dummyptzservice_->SetGetConfigurations(ptzconfiguration);

  ptz::GetConfigurationsResponse getconfigurationsresponse = client_->GetConfigurationsFuture().get();
  ASSERT_GE(getconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getconfigurationsresponse.Error()) << getconfigurationsresponse.Message();
  ASSERT_EQ(ptzconfiguration, getconfigurationsresponse.ptzconfiguration_);
}

TEST_F(TestPTZClient, GetNodes)
{
  const std::vector<PTZNode> ptznode =
  {
    PTZNode(std::string("token"), true, std::string("name"), PTZSpaces({ Space2dDescription(std::string("uri"), FloatRange(1.0f, 5.0f), FloatRange(2.0f, 3.0f)) },{ Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) },{ Space2dDescription(std::string("uri"), FloatRange(1.0f, 5.0f), FloatRange(2.0f, 3.0f)) },{ Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) },{ Space2dDescription(std::string("uri"), FloatRange(1.0f, 5.0f), FloatRange(2.0f, 3.0f)) },{ Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) },{ Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) },{ Space1dDescription(std::string("uri"), FloatRange(1.0f, 2.0f)) }), 5, true,{ std::string("auxiliarycommands1"), std::string("auxiliarycommands2") },{ PTZPresetTourSupported(5,{ PTZPRESETTOUROPERATION_STOP, PTZPRESETTOUROPERATION_PAUSE }), PTZPresetTourSupported(6,{ PTZPRESETTOUROPERATION_START, PTZPRESETTOUROPERATION_PAUSE }) })
  };
  dummyptzservice_->SetGetNodes(ptznode);

  ptz::GetNodesResponse getnodesresponse = client_->GetNodesFuture().get();
  ASSERT_GE(getnodesresponse.latency_, 0);
  ASSERT_FALSE(getnodesresponse.Error()) << getnodesresponse.Message();
  ASSERT_EQ(ptznode, getnodesresponse.ptznode_);
}

TEST_F(TestPTZClient, GetServiceCapabilities)
{
  const ptz::Capabilities capabilities(true, true, true, true, true);
  dummyptzservice_->SetGetServiceCapabilities(capabilities);

  ptz::GetServiceCapabilitiesResponse getptzservicecapabilitiesresponse = client_->GetServiceCapabilitiesFuture().get();
  ASSERT_GE(getptzservicecapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getptzservicecapabilitiesresponse.Error()) << getptzservicecapabilitiesresponse.Message();
  ASSERT_TRUE(getptzservicecapabilitiesresponse.capabilities_.is_initialized());
  ASSERT_EQ(capabilities, *getptzservicecapabilitiesresponse.capabilities_);
}

TEST_F(TestPTZClient, SetConfiguration)
{
  const PTZConfiguration ptzconfiguration(std::string("name"), 5, std::string("token"), std::string("nodetoken"), std::string("defaultabsolutepantiltpositionspace"), std::string("defaultabsolutezoompositionspace"), std::string("defaultrelativepantilttranslationspace"), std::string("defaultrelativezoomtranslationspace"), std::string("defaultcontinuouspantiltvelocityspace"), std::string("defaultcontinuouszoomvelocityspace"), DefaultPTZSpeed(Vector2d(5, 5, std::string("space")), 5), std::string("PT5S"), PanTiltLimits(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f))), ZoomLimits(Space1dDescription(std::string("uri"), FloatRange(5.0f, 5.0f))));
  const bool forcepersistence = true;

  ptz::SetConfigurationResponse setconfigurationresponse = client_->SetConfigurationFuture(ptzconfiguration, forcepersistence).get();
  ASSERT_GE(setconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setconfigurationresponse.Error()) << setconfigurationresponse.Message();
  ASSERT_EQ(ptzconfiguration, setconfigurationresponse.ptzconfiguration_);
  ASSERT_EQ(forcepersistence, setconfigurationresponse.forcepersistence_);
}

TEST_F(TestPTZClient, Stop)
{
  const std::string profiletoken("profiletoken");
  const bool pantilt = true;
  const bool zoom = false;

  ptz::StopResponse stopresponse = client_->StopFuture(profiletoken, pantilt, zoom).get();
  ASSERT_GE(stopresponse.latency_, 0);
  ASSERT_FALSE(stopresponse.Error()) << stopresponse.Message();
  ASSERT_EQ(profiletoken, stopresponse.profiletoken_);
  ASSERT_EQ(pantilt, stopresponse.pantilt_);
  ASSERT_EQ(zoom, stopresponse.zoom_);
}

}

}
