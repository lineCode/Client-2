// testmediaclient.cpp
//

///// Includes /////

#include "testmediaclient.hpp"

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
const std::string MEDIAURI("/onvif/media");

///// Methods /////

TestMediaClient::TestMediaClient()
{

}

TestMediaClient::~TestMediaClient()
{

}

void TestMediaClient::SetUp()
{
  Init(PORT, MEDIAURI);

  // Setup dummy server
  dummymediaservice_ = std::make_unique<DummyMediaService>();
  ASSERT_FALSE(httpserver_.AddServer(MEDIAURI, dummymediaservice_.get()));
}

void TestMediaClient::TearDown()
{
  ASSERT_FALSE(httpserver_.RemoveServer(MEDIAURI));
  dummymediaservice_.reset();

  Destroy();
}

///// Tests /////

TEST_F(TestMediaClient, AddAudioDecoderConfiguration)
{
  const std::string profiletoken("profiletoken");
  const std::string configurationtoken("configurationtoken");

  media::AddAudioDecoderConfigurationResponse addaudiodecoderconfigurationresponse = client_->AddAudioDecoderConfigurationFuture(profiletoken, configurationtoken).get();
  ASSERT_GE(addaudiodecoderconfigurationresponse.latency_, 0);
  ASSERT_FALSE(addaudiodecoderconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, addaudiodecoderconfigurationresponse.profiletoken_);
  ASSERT_EQ(configurationtoken, addaudiodecoderconfigurationresponse.configurationtoken_);
}

TEST_F(TestMediaClient, AddAudioEncoderConfiguration)
{
  const std::string profiletoken("profiletoken");
  const std::string configurationtoken("configurationtoken");

  media::AddAudioEncoderConfigurationResponse addaudioencoderconfigurationresponse = client_->AddAudioEncoderConfigurationFuture(profiletoken, configurationtoken).get();
  ASSERT_GE(addaudioencoderconfigurationresponse.latency_, 0);
  ASSERT_FALSE(addaudioencoderconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, addaudioencoderconfigurationresponse.profiletoken_);
  ASSERT_EQ(configurationtoken, addaudioencoderconfigurationresponse.configurationtoken_);
}

TEST_F(TestMediaClient, AddAudioOutputConfiguration)
{
  const std::string profiletoken("profiletoken");
  const std::string configurationtoken("configurationtoken");

  media::AddAudioOutputConfigurationResponse addaudiooutputconfigurationresponse = client_->AddAudioOutputConfigurationFuture(profiletoken, configurationtoken).get();
  ASSERT_GE(addaudiooutputconfigurationresponse.latency_, 0);
  ASSERT_FALSE(addaudiooutputconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, addaudiooutputconfigurationresponse.profiletoken_);
  ASSERT_EQ(configurationtoken, addaudiooutputconfigurationresponse.configurationtoken_);
}

TEST_F(TestMediaClient, AddAudioSourceConfiguration)
{
  const std::string profiletoken("profiletoken");
  const std::string configurationtoken("configurationtoken");

  media::AddAudioSourceConfigurationResponse addaudiosourceconfigurationresponse = client_->AddAudioSourceConfigurationFuture(profiletoken, configurationtoken).get();
  ASSERT_GE(addaudiosourceconfigurationresponse.latency_, 0);
  ASSERT_FALSE(addaudiosourceconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, addaudiosourceconfigurationresponse.profiletoken_);
  ASSERT_EQ(configurationtoken, addaudiosourceconfigurationresponse.configurationtoken_);
}

TEST_F(TestMediaClient, AddMetadataConfiguration)
{
  const std::string profiletoken("profiletoken");
  const std::string configurationtoken("configurationtoken");

  media::AddMetadataConfigurationResponse addmetadataconfigurationresponse = client_->AddMetadataConfigurationFuture(profiletoken, configurationtoken).get();
  ASSERT_GE(addmetadataconfigurationresponse.latency_, 0);
  ASSERT_FALSE(addmetadataconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, addmetadataconfigurationresponse.profiletoken_);
  ASSERT_EQ(configurationtoken, addmetadataconfigurationresponse.configurationtoken_);
}

TEST_F(TestMediaClient, AddPTZConfiguration)
{
  const std::string profiletoken("profiletoken");
  const std::string configurationtoken("configurationtoken");

  media::AddPTZConfigurationResponse addptzconfigurationresponse = client_->AddPTZConfigurationFuture(profiletoken, configurationtoken).get();
  ASSERT_GE(addptzconfigurationresponse.latency_, 0);
  ASSERT_FALSE(addptzconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, addptzconfigurationresponse.profiletoken_);
  ASSERT_EQ(configurationtoken, addptzconfigurationresponse.configurationtoken_);
}

TEST_F(TestMediaClient, AddVideoAnalyticsConfiguration)
{
  const std::string profiletoken("profiletoken");
  const std::string configurationtoken("configurationtoken");

  media::AddVideoAnalyticsConfigurationResponse addvideoanalyticsconfigurationresponse = client_->AddVideoAnalyticsConfigurationFuture(profiletoken, configurationtoken).get();
  ASSERT_GE(addvideoanalyticsconfigurationresponse.latency_, 0);
  ASSERT_FALSE(addvideoanalyticsconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, addvideoanalyticsconfigurationresponse.profiletoken_);
  ASSERT_EQ(configurationtoken, addvideoanalyticsconfigurationresponse.configurationtoken_);
}

TEST_F(TestMediaClient, AddVideoEncoderConfiguration)
{
  const std::string profiletoken("profiletoken");
  const std::string configurationtoken("configurationtoken");

  media::AddVideoEncoderConfigurationResponse addvideoencoderconfigurationresponse = client_->AddVideoEncoderConfigurationFuture(profiletoken, configurationtoken).get();
  ASSERT_GE(addvideoencoderconfigurationresponse.latency_, 0);
  ASSERT_FALSE(addvideoencoderconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, addvideoencoderconfigurationresponse.profiletoken_);
  ASSERT_EQ(configurationtoken, addvideoencoderconfigurationresponse.configurationtoken_);
}

TEST_F(TestMediaClient, AddVideoSourceConfiguration)
{
  const std::string profiletoken("profiletoken");
  const std::string configurationtoken("configurationtoken");

  media::AddVideoSourceConfigurationResponse addvideosourceconfigurationresponse = client_->AddVideoSourceConfigurationFuture(profiletoken, configurationtoken).get();
  ASSERT_GE(addvideosourceconfigurationresponse.latency_, 0);
  ASSERT_FALSE(addvideosourceconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, addvideosourceconfigurationresponse.profiletoken_);
  ASSERT_EQ(configurationtoken, addvideosourceconfigurationresponse.configurationtoken_);
}

TEST_F(TestMediaClient, CreateProfile)
{
  /*TODO const Profile profile(Profile(std::string("token"), VideoSourceConfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken"), IntRectangle(5, 5, 5, 5), Rotate(ROTATE_AUTO, 180)), AudioSourceConfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken")), VideoEncoderConfiguration(std::string("name"), 5, std::string("token"), VIDEOENCODING_H264, VideoResolution(5, 5), 5.0f, VideoRateControl(5, 5, 5), Mpeg4Configuration(5, MPEG4PROFILE_ASP), H264Configuration(5, H264PROFILE_BASELINE), MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5)), AudioEncoderConfiguration(std::string("name"), 5, std::string("token"), AUDIOENCODING_AAC, 5, 5, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5)), VideoAnalyticsConfiguration(std::string("name"), 5, std::string("token"), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) }, { ElementItem({Element("name", { { "a", "b" }, { "c", "d" } }, "text", { Element("name2", { { "e", "f" }, { "g", "h" } }, "text2", { Element("name3", { { "i", "j" }, { "k", "l" } }, "text3", {}) }) } )}), ElementItem({Element("name", { { "z", "y" }, { "c", "d" } }, "text", { Element("name5", { { "e", "f" }, { "g", "h" } }, "text5", { Element("name6", { { "i", "j" }, { "k", "l" } }, "text6", {}) }) } )}) }), std::string("name"), std::string("type")) })), PTZConfiguration(std::string("name"), 5, std::string("token"), std::string("nodetoken"), std::string("defaultabsolutepantiltpositionspace"), std::string("defaultabsolutezoompositionspace"), std::string("defaultrelativepantilttranslationspace"), std::string("defaultrelativezoomtranslationspace"), std::string("defaultcontinuouspantiltvelocityspace"), std::string("defaultcontinuouszoomvelocityspace"), DefaultPTZSpeed(Vector2d(5, 5, std::string("space")), 5), std::string("PT5S"), PanTiltLimits(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f))), ZoomLimits(Space1dDescription(std::string("uri"), FloatRange(5.0f, 5.0f)))), MetadataConfiguration(std::string("token"), METADATACOMPRESSIONTYPE_GZIP, std::string("name"), 5, PTZFilter(true, false), EventSubscription(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) })), true, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) })), AudioOutputConfiguration(std::string("name"), 5, std::string("token"), std::string("outputtoken"), std::string("sendprimacy"), 5), AudioDecoderConfiguration(std::string("name"), 5, std::string("token")), std::string("token"), true));
  dummymediaservice_->SetCreateProfile(profile);

  media::CreateProfileResponse getprofileresponse = client_->CreateProfileFuture(*profile.name_, profile.token_).get();
  ASSERT_GE(getprofileresponse.latency_, 0);
  ASSERT_FALSE(getprofileresponse.Error());
  ASSERT_EQ(*profile.name_, getprofileresponse.name_);
  ASSERT_TRUE(getprofileresponse.token_.is_initialized());
  ASSERT_EQ(*profile.token_, *getprofileresponse.token_);
  ASSERT_TRUE(getprofileresponse.profile_.is_initialized());
  ASSERT_EQ(profile, *getprofileresponse.profile_);*/
}

TEST_F(TestMediaClient, DeleteProfile)
{
  const std::string token("token");
  media::DeleteProfileResponse deleteprofileresponse = client_->DeleteProfileFuture(token).get();
  ASSERT_GE(deleteprofileresponse.latency_, 0);
  ASSERT_FALSE(deleteprofileresponse.Error());
  ASSERT_EQ(token, deleteprofileresponse.token_);
}

TEST_F(TestMediaClient, GetAudioDecoderConfiguration)
{
  const AudioDecoderConfiguration configuration(std::string("name"), 5, std::string("token"));
  dummymediaservice_->SetGetAudioDecoderConfiguration(configuration);

  media::GetAudioDecoderConfigurationResponse getaudiodecoderconfigurationresponse = client_->GetAudioDecoderConfigurationFuture(*configuration.token_).get();
  ASSERT_GE(getaudiodecoderconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getaudiodecoderconfigurationresponse.Error());
  ASSERT_EQ(*configuration.token_, getaudiodecoderconfigurationresponse.configurationtoken_);
  ASSERT_EQ(configuration, *getaudiodecoderconfigurationresponse.configuration_);
}

TEST_F(TestMediaClient, GetAudioDecoderConfigurationOptions)
{
  const AudioDecoderConfigurationOptions audiodecoderconfigurationoptions(AacDecOptions(IntList({ 1, 2, 3 }), IntList({ 4, 5, 6 })), G711DecOptions(IntList({ 7, 8, 9 }), IntList({ 10, 11, 12 })), G726DecOptions(IntList({ 13, 14, 15 }), IntList({ 16, 17, 18 })));
  dummymediaservice_->SetGetAudioDecoderConfigurationOptions(audiodecoderconfigurationoptions);

  media::GetAudioDecoderConfigurationOptionsResponse getaudiodecoderconfigurationoptionsresponse = client_->GetAudioDecoderConfigurationOptionsFuture(boost::none, boost::none).get();
  ASSERT_GE(getaudiodecoderconfigurationoptionsresponse.latency_, 0);
  ASSERT_FALSE(getaudiodecoderconfigurationoptionsresponse.Error());
  ASSERT_TRUE(getaudiodecoderconfigurationoptionsresponse.options_.is_initialized());
  ASSERT_EQ(audiodecoderconfigurationoptions, *getaudiodecoderconfigurationoptionsresponse.options_);
}

TEST_F(TestMediaClient, GetAudioDecoderConfigurations)
{
  const std::vector<AudioDecoderConfiguration> audiodecoderconfigurations =
  {
    AudioDecoderConfiguration(std::string("name1"), 5, std::string("token1")),
    AudioDecoderConfiguration(std::string("name2"), 6, std::string("token2"))
  };
  dummymediaservice_->SetGetAudioDecoderConfigurations(audiodecoderconfigurations);

  media::GetAudioDecoderConfigurationsResponse getaudiodecoderconfigurationsresponse = client_->GetAudioDecoderConfigurationsFuture().get();
  ASSERT_GE(getaudiodecoderconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getaudiodecoderconfigurationsresponse.Error());
  ASSERT_EQ(audiodecoderconfigurations, getaudiodecoderconfigurationsresponse.configurations_);
}

TEST_F(TestMediaClient, GetAudioEncoderConfiguration)
{
  const AudioEncoderConfiguration configuration(std::string("name"), 5, std::string("token1"), AUDIOENCODING_AAC, 5, 5, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5));
  dummymediaservice_->SetGetAudioEncoderConfiguration(configuration);

  media::GetAudioEncoderConfigurationResponse getaudioencoderconfigurationresponse = client_->GetAudioEncoderConfigurationFuture(*configuration.token_).get();
  ASSERT_GE(getaudioencoderconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getaudioencoderconfigurationresponse.Error());
  ASSERT_EQ(*configuration.token_, getaudioencoderconfigurationresponse.configurationtoken_);
  ASSERT_EQ(configuration, *getaudioencoderconfigurationresponse.configuration_);
}

TEST_F(TestMediaClient, GetAudioEncoderConfigurationOptions)
{
  const AudioEncoderConfigurationOptions audioencoderconfigurationoptions({ AudioEncoderConfigurationOption(AUDIOENCODING_G726, IntList({ 5, 6, 7 }), IntList({ 6, 7, 8 })), AudioEncoderConfigurationOption(AUDIOENCODING_G711, IntList({ 1, 2, 3 }), IntList({ 4, 5, 6 })) });
  dummymediaservice_->SetGetAudioEncoderConfigurationOptions(audioencoderconfigurationoptions);

  media::GetAudioEncoderConfigurationOptionsResponse getaudioencoderconfigurationoptionsresponse = client_->GetAudioEncoderConfigurationOptionsFuture(boost::none, boost::none).get();
  ASSERT_GE(getaudioencoderconfigurationoptionsresponse.latency_, 0);
  ASSERT_FALSE(getaudioencoderconfigurationoptionsresponse.Error());
  ASSERT_TRUE(getaudioencoderconfigurationoptionsresponse.options_.is_initialized());
  ASSERT_EQ(audioencoderconfigurationoptions, *getaudioencoderconfigurationoptionsresponse.options_);
}

TEST_F(TestMediaClient, GetAudioEncoderConfigurations)
{
  const std::vector<AudioEncoderConfiguration> audioencoderconfigurations =
  {
    AudioEncoderConfiguration(std::string("name"), 5, std::string("token1"), AUDIOENCODING_AAC, 5, 5, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5)),
    AudioEncoderConfiguration(std::string("name"), 5, std::string("token2"), AUDIOENCODING_AAC, 5, 5, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5))
  };
  dummymediaservice_->SetGetAudioEncoderConfigurations(audioencoderconfigurations);

  media::GetAudioEncoderConfigurationsResponse getaudioencoderconfigurationsresponse = client_->GetAudioEncoderConfigurationsFuture().get();
  ASSERT_GE(getaudioencoderconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getaudioencoderconfigurationsresponse.Error());
  ASSERT_EQ(audioencoderconfigurations, getaudioencoderconfigurationsresponse.configurations_);
}

TEST_F(TestMediaClient, GetAudioOutputConfiguration)
{
  const AudioOutputConfiguration configuration(std::string("name"), 5, std::string("token"), std::string("outputtoken"), std::string("sendprimacy"), 5);
  dummymediaservice_->SetGetAudioOutputConfiguration(configuration);

  media::GetAudioOutputConfigurationResponse getaudiooutputconfigurationresponse = client_->GetAudioOutputConfigurationFuture(*configuration.token_).get();
  ASSERT_GE(getaudiooutputconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getaudiooutputconfigurationresponse.Error());
  ASSERT_EQ(*configuration.token_, getaudiooutputconfigurationresponse.configurationtoken_);
  ASSERT_EQ(configuration, *getaudiooutputconfigurationresponse.configuration_);
}

TEST_F(TestMediaClient, GetAudioOutputConfigurationOptions)
{
  const AudioOutputConfigurationOptions audiooutputconfigurationoptions({ std::string("token1"), std::string("token2") }, { std::string("primacy1"), std::string("primacy2") }, IntRange(5, 6));
  dummymediaservice_->SetGetAudioOutputConfigurationOptions(audiooutputconfigurationoptions);

  media::GetAudioOutputConfigurationOptionsResponse getaudiooutputconfigurationoptionsresponse = client_->GetAudioOutputConfigurationOptionsFuture(boost::none, boost::none).get();
  ASSERT_GE(getaudiooutputconfigurationoptionsresponse.latency_, 0);
  ASSERT_FALSE(getaudiooutputconfigurationoptionsresponse.Error());
  ASSERT_TRUE(getaudiooutputconfigurationoptionsresponse.options_.is_initialized());
  ASSERT_EQ(audiooutputconfigurationoptions, *getaudiooutputconfigurationoptionsresponse.options_);
}

TEST_F(TestMediaClient, GetAudioOutputConfigurations)
{
  const std::vector<AudioOutputConfiguration> audiooutputconfigurations =
  {
    AudioOutputConfiguration(std::string("name1"), 5, std::string("token1"), std::string("outputtoken1"), std::string("sendprimacy1"), 5),
    AudioOutputConfiguration(std::string("name2"), 5, std::string("token2"), std::string("outputtoken2"), std::string("sendprimacy2"), 5)
  };
  dummymediaservice_->SetGetAudioOutputConfigurations(audiooutputconfigurations);

  media::GetAudioOutputConfigurationsResponse getaudiooutputconfigurationsresponse = client_->GetAudioOutputConfigurationsFuture().get();
  ASSERT_GE(getaudiooutputconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getaudiooutputconfigurationsresponse.Error());
  ASSERT_EQ(audiooutputconfigurations, getaudiooutputconfigurationsresponse.configurations_);
}

TEST_F(TestMediaClient, GetAudioSourceConfiguration)
{
  const AudioSourceConfiguration configuration(std::string("name"), 5, std::string("token"), std::string("sourcetoken"));
  dummymediaservice_->SetGetAudioSourceConfiguration(configuration);

  media::GetAudioSourceConfigurationResponse getaudiosourceconfigurationresponse = client_->GetAudioSourceConfigurationFuture(*configuration.token_).get();
  ASSERT_GE(getaudiosourceconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getaudiosourceconfigurationresponse.Error());
  ASSERT_EQ(*configuration.token_, getaudiosourceconfigurationresponse.configurationtoken_);
  ASSERT_EQ(configuration, *getaudiosourceconfigurationresponse.configuration_);
}

TEST_F(TestMediaClient, GetAudioSourceConfigurationOptions)
{
  const AudioSourceConfigurationOptions audiosourceconfigurationoptions({ std::string("token1"), std::string("token2") });
  dummymediaservice_->SetGetAudioSourceConfigurationOptions(audiosourceconfigurationoptions);

  media::GetAudioSourceConfigurationOptionsResponse getaudiosourceconfigurationoptionsresponse = client_->GetAudioSourceConfigurationOptionsFuture(boost::none, boost::none).get();
  ASSERT_GE(getaudiosourceconfigurationoptionsresponse.latency_, 0);
  ASSERT_FALSE(getaudiosourceconfigurationoptionsresponse.Error());
  ASSERT_TRUE(getaudiosourceconfigurationoptionsresponse.options_.is_initialized());
  ASSERT_EQ(audiosourceconfigurationoptions, *getaudiosourceconfigurationoptionsresponse.options_);
}

TEST_F(TestMediaClient, GetAudioSourceConfigurations)
{
  const std::vector<AudioSourceConfiguration> audiosourceconfigurations =
  {
    AudioSourceConfiguration(std::string("name"), 5, std::string("token1"), std::string("sourcetoken")),
    AudioSourceConfiguration(std::string("name"), 5, std::string("token2"), std::string("sourcetoken"))
  };
  dummymediaservice_->SetGetAudioSourceConfigurations(audiosourceconfigurations);

  media::GetAudioSourceConfigurationsResponse getaudiosourceconfigurationsresponse = client_->GetAudioSourceConfigurationsFuture().get();
  ASSERT_GE(getaudiosourceconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getaudiosourceconfigurationsresponse.Error());
  ASSERT_EQ(audiosourceconfigurations, getaudiosourceconfigurationsresponse.configurations_);
}

TEST_F(TestMediaClient, GetAudioSources)
{
  const std::vector<AudioSource> audiosources =
  {
    AudioSource(std::string("token1"), 5),
    AudioSource(std::string("token2"), 6)
  };
  dummymediaservice_->SetGetAudioSources(audiosources);


  media::GetAudioSourcesResponse getaudiosourcesresponse = client_->GetAudioSourcesFuture().get();
  ASSERT_GE(getaudiosourcesresponse.latency_, 0);
  ASSERT_FALSE(getaudiosourcesresponse.Error());
  ASSERT_EQ(audiosources, getaudiosourcesresponse.audiosources_);
}

TEST_F(TestMediaClient, GetCompatibleAudioDecoderConfigurations)
{
  const std::string profiletoken("profiletoken");
  const std::vector<AudioDecoderConfiguration> compatibleaudiodecoderconfigurations =
  {
    AudioDecoderConfiguration(std::string("name1"), 5, std::string("token1")),
    AudioDecoderConfiguration(std::string("name2"), 5, std::string("token2"))
  };
  dummymediaservice_->SetGetCompatibleAudioDecoderConfigurations(compatibleaudiodecoderconfigurations);

  media::GetCompatibleAudioDecoderConfigurationsResponse getcompatibleaudiodecoderconfigurationsresponse = client_->GetCompatibleAudioDecoderConfigurationsFuture(profiletoken).get();
  ASSERT_GE(getcompatibleaudiodecoderconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getcompatibleaudiodecoderconfigurationsresponse.Error());
  ASSERT_EQ(profiletoken, getcompatibleaudiodecoderconfigurationsresponse.profiletoken_);
  ASSERT_EQ(compatibleaudiodecoderconfigurations, getcompatibleaudiodecoderconfigurationsresponse.configurations_);
}

TEST_F(TestMediaClient, GetCompatibleAudioEncoderConfigurations)
{
  const std::string profiletoken("profiletoken");
  const std::vector<AudioEncoderConfiguration> compatibleaudioencoderconfigurations =
  {
    AudioEncoderConfiguration(std::string("name1"), 5, std::string("token1"), AUDIOENCODING_AAC, 5, 5, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5)),
    AudioEncoderConfiguration(std::string("name2"), 5, std::string("token2"), AUDIOENCODING_AAC, 5, 5, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5))
  };
  dummymediaservice_->SetGetCompatibleAudioEncoderConfigurations(compatibleaudioencoderconfigurations);

  media::GetCompatibleAudioEncoderConfigurationsResponse getcompatibleaudioencoderconfigurationsresponse = client_->GetCompatibleAudioEncoderConfigurationsFuture(profiletoken).get();
  ASSERT_GE(getcompatibleaudioencoderconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getcompatibleaudioencoderconfigurationsresponse.Error());
  ASSERT_EQ(profiletoken, getcompatibleaudioencoderconfigurationsresponse.profiletoken_);
  ASSERT_EQ(compatibleaudioencoderconfigurations, getcompatibleaudioencoderconfigurationsresponse.configurations_);
}

TEST_F(TestMediaClient, GetCompatibleAudioOutputConfigurations)
{
  const std::string profiletoken("profiletoken");
  const std::vector<AudioOutputConfiguration> compatibleaudiooutputconfigurations =
  {
    AudioOutputConfiguration(std::string("name1"), 5, std::string("token1"), std::string("outputtoken"), std::string("sendprimacy"), 5),
    AudioOutputConfiguration(std::string("name2"), 5, std::string("token2"), std::string("outputtoken"), std::string("sendprimacy"), 5)
  };
  dummymediaservice_->SetGetCompatibleAudioOutputConfigurations(compatibleaudiooutputconfigurations);

  media::GetCompatibleAudioOutputConfigurationsResponse getcompatibleaudiooutputconfigurationsresponse = client_->GetCompatibleAudioOutputConfigurationsFuture(profiletoken).get();
  ASSERT_GE(getcompatibleaudiooutputconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getcompatibleaudiooutputconfigurationsresponse.Error());
  ASSERT_EQ(profiletoken, getcompatibleaudiooutputconfigurationsresponse.profiletoken_);
  ASSERT_EQ(compatibleaudiooutputconfigurations, getcompatibleaudiooutputconfigurationsresponse.configurations_);
}

TEST_F(TestMediaClient, GetCompatibleAudioSourceConfigurations)
{
  const std::string profiletoken("profiletoken");
  const std::vector<AudioSourceConfiguration> compatibleaudiosourceconfigurations =
  {
    AudioSourceConfiguration(std::string("name1"), 5, std::string("token1"), std::string("sourcetoken")),
    AudioSourceConfiguration(std::string("name2"), 5, std::string("token2"), std::string("sourcetoken"))
  };
  dummymediaservice_->SetGetCompatibleAudioSourceConfigurations(compatibleaudiosourceconfigurations);

  media::GetCompatibleAudioSourceConfigurationsResponse getcompatibleaudiosourceconfigurationsresponse = client_->GetCompatibleAudioSourceConfigurationsFuture(profiletoken).get();
  ASSERT_GE(getcompatibleaudiosourceconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getcompatibleaudiosourceconfigurationsresponse.Error());
  ASSERT_EQ(profiletoken, getcompatibleaudiosourceconfigurationsresponse.profiletoken_);
  ASSERT_EQ(compatibleaudiosourceconfigurations, getcompatibleaudiosourceconfigurationsresponse.configurations_);
}

TEST_F(TestMediaClient, GetCompatibleMetadataConfigurations)
{
  /*TODO const std::string profiletoken("profiletoken");
  const std::vector<MetadataConfiguration> compatiblemetadataconfigurations =
  {
    MetadataConfiguration(std::string("token1"), METADATACOMPRESSIONTYPE_GZIP, std::string("name1"), 5, PTZFilter(true, false), EventSubscription(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) })), true, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) })),
    MetadataConfiguration(std::string("token2"), METADATACOMPRESSIONTYPE_GZIP, std::string("name2"), 5, PTZFilter(true, false), EventSubscription(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) })), true, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) }))
  };
  dummymediaservice_->SetGetCompatibleMetadataConfigurations(compatiblemetadataconfigurations);

  media::GetCompatibleMetadataConfigurationsResponse getcompatiblemetadataconfigurationsresponse = client_->GetCompatibleMetadataConfigurationsFuture(profiletoken).get();
  ASSERT_GE(getcompatiblemetadataconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getcompatiblemetadataconfigurationsresponse.Error());
  ASSERT_EQ(profiletoken, getcompatiblemetadataconfigurationsresponse.profiletoken_);
  ASSERT_EQ(compatiblemetadataconfigurations, getcompatiblemetadataconfigurationsresponse.configurations_);*/
}

TEST_F(TestMediaClient, GetCompatibleVideoAnalyticsConfigurations)
{
  /*TODO const std::string profiletoken("profiletoken");
  const std::vector<VideoAnalyticsConfiguration> compatiblevideoanalyticsconfigurations =
  {
    VideoAnalyticsConfiguration(std::string("name1"), 5, std::string("token1"), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) })),
    VideoAnalyticsConfiguration(std::string("name2"), 5, std::string("token2"), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) }))
  };
  dummymediaservice_->SetGetCompatibleVideoAnalyticsConfigurations(compatiblevideoanalyticsconfigurations);

  media::GetCompatibleVideoAnalyticsConfigurationsResponse getcompatiblevideoanalyticsconfigurationsresponse = client_->GetCompatibleVideoAnalyticsConfigurationsFuture(profiletoken).get();
  ASSERT_GE(getcompatiblevideoanalyticsconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getcompatiblevideoanalyticsconfigurationsresponse.Error());
  ASSERT_EQ(profiletoken, getcompatiblevideoanalyticsconfigurationsresponse.profiletoken_);
  ASSERT_EQ(compatiblevideoanalyticsconfigurations, getcompatiblevideoanalyticsconfigurationsresponse.configurations_);*/
}

TEST_F(TestMediaClient, GetCompatibleVideoEncoderConfigurations)
{
  const std::string profiletoken("profiletoken");
  const std::vector<VideoEncoderConfiguration> compatiblevideoencoderconfigurations =
  {
    VideoEncoderConfiguration(std::string("name1"), 5, std::string("token1"), VIDEOENCODING_H264, VideoResolution(5, 5), 5.0f, VideoRateControl(5, 5, 5), Mpeg4Configuration(5, MPEG4PROFILE_ASP), H264Configuration(5, H264PROFILE_BASELINE), MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5)),
    VideoEncoderConfiguration(std::string("name2"), 5, std::string("token2"), VIDEOENCODING_H264, VideoResolution(5, 5), 5.0f, VideoRateControl(5, 5, 5), Mpeg4Configuration(5, MPEG4PROFILE_ASP), H264Configuration(5, H264PROFILE_BASELINE), MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5))
  };
  dummymediaservice_->SetGetCompatibleVideoEncoderConfigurations(compatiblevideoencoderconfigurations);

  media::GetCompatibleVideoEncoderConfigurationsResponse getcompatiblevideoencoderconfigurationsresponse = client_->GetCompatibleVideoEncoderConfigurationsFuture(profiletoken).get();
  ASSERT_GE(getcompatiblevideoencoderconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getcompatiblevideoencoderconfigurationsresponse.Error());
  ASSERT_EQ(profiletoken, getcompatiblevideoencoderconfigurationsresponse.profiletoken_);
  ASSERT_EQ(compatiblevideoencoderconfigurations, getcompatiblevideoencoderconfigurationsresponse.configurations_);
}

TEST_F(TestMediaClient, GetCompatibleVideoSourceConfigurations)
{
  const std::string profiletoken("profiletoken");
  const std::vector<VideoSourceConfiguration> compatiblevideosourceconfigurations =
  {
    VideoSourceConfiguration(std::string("name1"), 5, std::string("token1"), std::string("sourcetoken1"), IntRectangle(5, 5, 5, 5), Rotate(ROTATE_AUTO, 180)),
    VideoSourceConfiguration(std::string("name2"), 6, std::string("token2"), std::string("sourcetoken2"), IntRectangle(6, 6, 6, 6), Rotate(ROTATE_ON, 90))
  };
  dummymediaservice_->SetGetCompatibleVideoSourceConfigurations(compatiblevideosourceconfigurations);

  media::GetCompatibleVideoSourceConfigurationsResponse getcompatiblevideosourceconfigurationsresponse = client_->GetCompatibleVideoSourceConfigurationsFuture(profiletoken).get();
  ASSERT_GE(getcompatiblevideosourceconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getcompatiblevideosourceconfigurationsresponse.Error());
  ASSERT_EQ(profiletoken, getcompatiblevideosourceconfigurationsresponse.profiletoken_);
  ASSERT_EQ(compatiblevideosourceconfigurations, getcompatiblevideosourceconfigurationsresponse.configurations_);
}

TEST_F(TestMediaClient, GetMetadataConfiguration)
{
  /*TODO const MetadataConfiguration configuration(std::string("token1"), METADATACOMPRESSIONTYPE_GZIP, std::string("name"), 5, PTZFilter(true, false), EventSubscription(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) })), true, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) }));
  dummymediaservice_->SetGetMetadataConfiguration(configuration);

  media::GetMetadataConfigurationResponse getmetadataconfigurationresponse = client_->GetMetadataConfigurationFuture(*configuration.token_).get();
  ASSERT_GE(getmetadataconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getmetadataconfigurationresponse.Error());
  ASSERT_EQ(*configuration.token_, getmetadataconfigurationresponse.configurationtoken_);
  ASSERT_EQ(configuration, *getmetadataconfigurationresponse.configuration_);*/
}

TEST_F(TestMediaClient, GetMetadataConfigurationOptions)
{
  std::string configurationtoken("configurationtoken");
  std::string profiletoken("profiletoken");
  const MetadataConfigurationOptions options(PTZStatusFilterOptions(true, false, true, false), { METADATACOMPRESSIONTYPE_GZIP, METADATACOMPRESSIONTYPE_EXI });
  dummymediaservice_->SetGetMetadataConfigurationOptions(options);

  media::GetMetadataConfigurationOptionsResponse getmetadataconfigurationoptions = client_->GetMetadataConfigurationOptionsFuture(configurationtoken, profiletoken).get();
  ASSERT_GE(getmetadataconfigurationoptions.latency_, 0);
  ASSERT_FALSE(getmetadataconfigurationoptions.Error());
  ASSERT_EQ(configurationtoken, *getmetadataconfigurationoptions.configurationtoken_);
  ASSERT_EQ(profiletoken, *getmetadataconfigurationoptions.profiletoken_);
  ASSERT_EQ(options, *getmetadataconfigurationoptions.options_);
}

TEST_F(TestMediaClient, GetMetadataConfigurations)
{
  /*TODO const std::vector<MetadataConfiguration> configurations =
  {
    MetadataConfiguration(std::string("token1"), METADATACOMPRESSIONTYPE_GZIP, std::string("name"), 5, PTZFilter(true, false), EventSubscription(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) })), true, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) })),
    MetadataConfiguration(std::string("token2"), METADATACOMPRESSIONTYPE_GZIP, std::string("name"), 5, PTZFilter(true, false), EventSubscription(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) })), true, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) }))
  };
  dummymediaservice_->SetGetMetadataConfigurations(configurations);

  media::GetMetadataConfigurationsResponse getmetadataconfigurationsresponse = client_->GetMetadataConfigurationsFuture().get();
  ASSERT_GE(getmetadataconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getmetadataconfigurationsresponse.Error());
  ASSERT_EQ(configurations, getmetadataconfigurationsresponse.configurations_);*/
}

TEST_F(TestMediaClient, GetProfile)
{
  /*TODO const Profile profile(Profile(std::string("token"), VideoSourceConfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken"), IntRectangle(5, 5, 5, 5), Rotate(ROTATE_AUTO, 180)), AudioSourceConfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken")), VideoEncoderConfiguration(std::string("name"), 5, std::string("token"), VIDEOENCODING_H264, VideoResolution(5, 5), 5.0f, VideoRateControl(5, 5, 5), Mpeg4Configuration(5, MPEG4PROFILE_ASP), H264Configuration(5, H264PROFILE_BASELINE), MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5)), AudioEncoderConfiguration(std::string("name"), 5, std::string("token"), AUDIOENCODING_AAC, 5, 5, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5)), VideoAnalyticsConfiguration(std::string("name"), 5, std::string("token"), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) })), PTZConfiguration(std::string("name"), 5, std::string("token"), std::string("nodetoken"), std::string("defaultabsolutepantiltpositionspace"), std::string("defaultabsolutezoompositionspace"), std::string("defaultrelativepantilttranslationspace"), std::string("defaultrelativezoomtranslationspace"), std::string("defaultcontinuouspantiltvelocityspace"), std::string("defaultcontinuouszoomvelocityspace"), DefaultPTZSpeed(Vector2d(5, 5, std::string("space")), 5), std::string("PT5S"), PanTiltLimits(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f))), ZoomLimits(Space1dDescription(std::string("uri"), FloatRange(5.0f, 5.0f)))), MetadataConfiguration(std::string("token"), METADATACOMPRESSIONTYPE_GZIP, std::string("name"), 5, PTZFilter(true, false), EventSubscription(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) })), true, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) })), AudioOutputConfiguration(std::string("name"), 5, std::string("token"), std::string("outputtoken"), std::string("sendprimacy"), 5), AudioDecoderConfiguration(std::string("name"), 5, std::string("token")), std::string("token"), true));
  dummymediaservice_->SetGetProfile(profile);

  media::GetProfileResponse getprofileresponse = client_->GetProfileFuture(*profile.token_).get();
  ASSERT_GE(getprofileresponse.latency_, 0);
  ASSERT_FALSE(getprofileresponse.Error());
  ASSERT_EQ(*profile.token_, getprofileresponse.token_);
  ASSERT_TRUE(getprofileresponse.profile_.is_initialized());
  ASSERT_EQ(profile, *getprofileresponse.profile_);*/
}

TEST_F(TestMediaClient, GetProfiles)
{
  /*TODO const std::vector<Profile> profiles =
  {
    Profile(Profile(std::string("token1"), VideoSourceConfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken"), IntRectangle(5, 5, 5, 5), Rotate(ROTATE_AUTO, 180)), AudioSourceConfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken")), VideoEncoderConfiguration(std::string("name"), 5, std::string("token"), VIDEOENCODING_H264, VideoResolution(5, 5), 5.0f, VideoRateControl(5, 5, 5), Mpeg4Configuration(5, MPEG4PROFILE_ASP), H264Configuration(5, H264PROFILE_BASELINE), MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5)), AudioEncoderConfiguration(std::string("name"), 5, std::string("token"), AUDIOENCODING_AAC, 5, 5, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5)), VideoAnalyticsConfiguration(std::string("name"), 5, std::string("token"), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) })), PTZConfiguration(std::string("name"), 5, std::string("token"), std::string("nodetoken"), std::string("defaultabsolutepantiltpositionspace"), std::string("defaultabsolutezoompositionspace"), std::string("defaultrelativepantilttranslationspace"), std::string("defaultrelativezoomtranslationspace"), std::string("defaultcontinuouspantiltvelocityspace"), std::string("defaultcontinuouszoomvelocityspace"), DefaultPTZSpeed(Vector2d(5, 5, std::string("space")), 5), std::string("PT5S"), PanTiltLimits(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f))), ZoomLimits(Space1dDescription(std::string("uri"), FloatRange(5.0f, 5.0f)))), MetadataConfiguration(std::string("token"), METADATACOMPRESSIONTYPE_GZIP, std::string("name"), 5, PTZFilter(true, false), EventSubscription(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) })), true, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) })), AudioOutputConfiguration(std::string("name"), 5, std::string("token"), std::string("outputtoken"), std::string("sendprimacy"), 5), AudioDecoderConfiguration(std::string("name"), 5, std::string("token")), std::string("token"), true)),
    Profile(Profile(std::string("token2"), VideoSourceConfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken"), IntRectangle(5, 5, 5, 5), Rotate(ROTATE_AUTO, 180)), AudioSourceConfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken")), VideoEncoderConfiguration(std::string("name"), 5, std::string("token"), VIDEOENCODING_H264, VideoResolution(5, 5), 5.0f, VideoRateControl(5, 5, 5), Mpeg4Configuration(5, MPEG4PROFILE_ASP), H264Configuration(5, H264PROFILE_BASELINE), MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5)), AudioEncoderConfiguration(std::string("name"), 5, std::string("token"), AUDIOENCODING_AAC, 5, 5, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5)), VideoAnalyticsConfiguration(std::string("name"), 5, std::string("token"), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) })), PTZConfiguration(std::string("name"), 5, std::string("token"), std::string("nodetoken"), std::string("defaultabsolutepantiltpositionspace"), std::string("defaultabsolutezoompositionspace"), std::string("defaultrelativepantilttranslationspace"), std::string("defaultrelativezoomtranslationspace"), std::string("defaultcontinuouspantiltvelocityspace"), std::string("defaultcontinuouszoomvelocityspace"), DefaultPTZSpeed(Vector2d(5, 5, std::string("space")), 5), std::string("PT5S"), PanTiltLimits(Space2dDescription(std::string("uri"), FloatRange(5.0f, 5.0f), FloatRange(5.0f, 5.0f))), ZoomLimits(Space1dDescription(std::string("uri"), FloatRange(5.0f, 5.0f)))), MetadataConfiguration(std::string("token"), METADATACOMPRESSIONTYPE_GZIP, std::string("name"), 5, PTZFilter(true, false), EventSubscription(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) })), true, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) })), AudioOutputConfiguration(std::string("name"), 5, std::string("token"), std::string("outputtoken"), std::string("sendprimacy"), 5), AudioDecoderConfiguration(std::string("name"), 5, std::string("token")), std::string("token"), true))
  };
  dummymediaservice_->SetGetProfiles(profiles);

  media::GetProfilesResponse getprofilesresponse = client_->GetProfilesFuture().get();
  ASSERT_GE(getprofilesresponse.latency_, 0);
  ASSERT_FALSE(getprofilesresponse.Error());
  ASSERT_EQ(profiles, getprofilesresponse.profiles_);*/
}

TEST_F(TestMediaClient, GetServiceCapabilities)
{
  const media::Capabilities capabilities(ProfileCapabilities(5), StreamingCapabilities(true, true, true, true, true), true, true, true, true, true);
  dummymediaservice_->SetGetServiceCapabilities(capabilities);

  media::GetServiceCapabilitiesResponse getmediaservicecapabilitiesresponse = client_->GetServiceCapabilitiesFuture().get();
  ASSERT_GE(getmediaservicecapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getmediaservicecapabilitiesresponse.Error());
  ASSERT_TRUE(getmediaservicecapabilitiesresponse.capabilities_.is_initialized());
  ASSERT_EQ(capabilities, *getmediaservicecapabilitiesresponse.capabilities_);
}

TEST_F(TestMediaClient, GetSnapshotUri)
{
  const std::string token("token");
  const MediaUri snapshoturi(std::string("uri"), true, true, Duration(0, 1, 2, 3, 4, 5));
  dummymediaservice_->SetGetSnapshotUri(snapshoturi);

  media::GetSnapshotUriResponse getsnapshoturiresponse = client_->GetSnapshotUriFuture(token).get();
  ASSERT_GE(getsnapshoturiresponse.latency_, 0);
  ASSERT_FALSE(getsnapshoturiresponse.Error());
  ASSERT_TRUE(getsnapshoturiresponse.mediauri_.is_initialized());
  ASSERT_EQ(snapshoturi, *getsnapshoturiresponse.mediauri_);
}

TEST_F(TestMediaClient, GetStreamUri)
{
  const MediaUri mediauri(std::string("uri"), true, true, Duration(0, 1, 2, 3, 4, 5));
  dummymediaservice_->SetGetStreamUri(mediauri);

  StreamSetup streamsetup(STREAM_RTPUNICAST, Transport(TRANSPORTPROTOCOL_TCP));
  std::string token("token");
  media::GetStreamUriResponse getstreamuriresponse = client_->GetStreamUriFuture(streamsetup, token).get();
  ASSERT_GE(getstreamuriresponse.latency_, 0);
  ASSERT_FALSE(getstreamuriresponse.Error());
  ASSERT_EQ(streamsetup, getstreamuriresponse.streamsetup_);
  ASSERT_EQ(token, getstreamuriresponse.token_);
  ASSERT_TRUE(getstreamuriresponse.mediauri_.is_initialized());
  ASSERT_EQ(mediauri, *getstreamuriresponse.mediauri_);
}

TEST_F(TestMediaClient, GetVideoAnalyticsConfigurations)
{
  /*TODO const std::vector<VideoAnalyticsConfiguration> videoanalyticsconfigurations =
  {
    VideoAnalyticsConfiguration(std::string("name1"), 5, std::string("token1"), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) }, { ElementItem({Element("name", { { "a", "b" }, { "c", "d" } }, "text", { Element("name2", { { "e", "f" }, { "g", "h" } }, "text2", { Element("name3", { { "i", "j" }, { "k", "l" } }, "text3", {}) }) } )}), ElementItem({Element("name", { { "z", "y" }, { "c", "d" } }, "text", { Element("name5", { { "e", "f" }, { "g", "h" } }, "text5", { Element("name6", { { "i", "j" }, { "k", "l" } }, "text6", {}) }) } )}) }), std::string("name"), std::string("type")) })),
    VideoAnalyticsConfiguration(std::string("name2"), 5, std::string("token2"), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) }, { ElementItem({Element("name", { { "a", "b" }, { "c", "d" } }, "text", { Element("name2", { { "e", "f" }, { "g", "h" } }, "text2", { Element("name3", { { "i", "j" }, { "k", "l" } }, "text3", {}) }) } )}), ElementItem({Element("name", { { "z", "y" }, { "c", "d" } }, "text", { Element("name5", { { "e", "f" }, { "g", "h" } }, "text5", { Element("name6", { { "i", "j" }, { "k", "l" } }, "text6", {}) }) } )}) }), std::string("name"), std::string("type")) }))
  };
  dummymediaservice_->SetGetVideoAnalyticsConfigurations(videoanalyticsconfigurations);

  media::GetVideoAnalyticsConfigurationsResponse getvideoanalyticsconfigurationsresponse = client_->GetVideoAnalyticsConfigurationsFuture().get();
  ASSERT_GE(getvideoanalyticsconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getvideoanalyticsconfigurationsresponse.Error());
  ASSERT_EQ(videoanalyticsconfigurations, getvideoanalyticsconfigurationsresponse.configurations_);*/
}

TEST_F(TestMediaClient, GetVideoEncoderConfiguration)
{
  const VideoEncoderConfiguration videoencoderconfiguration(std::string("name"), 5, std::string("token"), VIDEOENCODING_H264, VideoResolution(5, 5), 5.0f, VideoRateControl(5, 5, 5), Mpeg4Configuration(5, MPEG4PROFILE_ASP), H264Configuration(5, H264PROFILE_BASELINE), MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5));
  dummymediaservice_->SetGetVideoEncoderConfiguration(videoencoderconfiguration);

  media::GetVideoEncoderConfigurationResponse getvideoencoderconfigurationresponse = client_->GetVideoEncoderConfigurationFuture(*videoencoderconfiguration.token_).get();
  ASSERT_GE(getvideoencoderconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getvideoencoderconfigurationresponse.Error());
  ASSERT_TRUE(getvideoencoderconfigurationresponse.videoencoderconfiguration_.is_initialized());
  ASSERT_EQ(videoencoderconfiguration, *getvideoencoderconfigurationresponse.videoencoderconfiguration_);
}

TEST_F(TestMediaClient, GetVideoEncoderConfigurationOptions)
{
  const VideoEncoderConfigurationOptions videoencoderconfigurationoptions(IntRange(5, 5), JpegOptions({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5)), Mpeg4Options({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5), { MPEG4PROFILE_ASP }), H264Options({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5), { H264PROFILE_HIGH, H264PROFILE_MAIN }), JpegOptions2({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5)), Mpeg4Options2({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5), { MPEG4PROFILE_ASP }, IntRange(5, 5)), H264Options2({ VideoResolution(5, 5), VideoResolution(10, 11) }, IntRange(5, 5), IntRange(5, 5), IntRange(5, 5), { H264PROFILE_HIGH, H264PROFILE_MAIN }, IntRange(5, 5)));
  dummymediaservice_->SetGetVideoEncoderConfigurationOptions(videoencoderconfigurationoptions);

  media::GetVideoEncoderConfigurationOptionsResponse getvideoencoderconfigurationoptionsresponse = client_->GetVideoEncoderConfigurationOptionsFuture(boost::none, boost::none).get();
  ASSERT_GE(getvideoencoderconfigurationoptionsresponse.latency_, 0);
  ASSERT_FALSE(getvideoencoderconfigurationoptionsresponse.Error());
  ASSERT_FALSE(getvideoencoderconfigurationoptionsresponse.configurationtoken_.is_initialized());
  ASSERT_FALSE(getvideoencoderconfigurationoptionsresponse.profiletoken_.is_initialized());
  ASSERT_TRUE(getvideoencoderconfigurationoptionsresponse.videoencoderconfigurationoptions_.is_initialized());
  ASSERT_EQ(videoencoderconfigurationoptions, *getvideoencoderconfigurationoptionsresponse.videoencoderconfigurationoptions_);
}

TEST_F(TestMediaClient, GetVideoEncoderConfigurations)
{
  const std::vector<VideoEncoderConfiguration> videoencoderconfigurations =
  {
    VideoEncoderConfiguration(std::string("name1"), 5, std::string("token1"), VIDEOENCODING_H264, VideoResolution(5, 5), 5.0f, VideoRateControl(5, 5, 5), Mpeg4Configuration(5, MPEG4PROFILE_ASP), H264Configuration(5, H264PROFILE_BASELINE), MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5)),
    VideoEncoderConfiguration(std::string("name2"), 5, std::string("token2"), VIDEOENCODING_H264, VideoResolution(5, 5), 5.0f, VideoRateControl(5, 5, 5), Mpeg4Configuration(5, MPEG4PROFILE_ASP), H264Configuration(5, H264PROFILE_BASELINE), MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5))
  };
  dummymediaservice_->SetGetVideoEncoderConfigurations(videoencoderconfigurations);

  media::GetVideoEncoderConfigurationsResponse getvideoencoderconfigurationsresponse = client_->GetVideoEncoderConfigurationsFuture().get();
  ASSERT_GE(getvideoencoderconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(getvideoencoderconfigurationsresponse.Error());
  ASSERT_TRUE(utility::Contains(videoencoderconfigurations, getvideoencoderconfigurationsresponse.configurations_));
}

TEST_F(TestMediaClient, GetVideoSourceConfiguration)
{
  const VideoSourceConfiguration videosourceconfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken"), IntRectangle(5, 5, 5, 5), Rotate(ROTATE_AUTO, 180));
  dummymediaservice_->SetGetVideoSourceConfiguration(videosourceconfiguration);

  media::GetVideoSourceConfigurationResponse getvideosourceconfigurationresponse = client_->GetVideoSourceConfigurationFuture(*videosourceconfiguration.token_).get();
  ASSERT_GE(getvideosourceconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getvideosourceconfigurationresponse.Error());
  ASSERT_TRUE(getvideosourceconfigurationresponse.videosourceconfiguration_.is_initialized());
  ASSERT_EQ(videosourceconfiguration, *getvideosourceconfigurationresponse.videosourceconfiguration_);
}

TEST_F(TestMediaClient, GetVideoSourceConfigurationOptions)
{
  const VideoSourceConfigurationOptions videosourceconfigurationoptions(IntRectangleRange(IntRange(5, 6), IntRange(7, 8), IntRange(9, 10), IntRange(boost::none, 6)), { "token1", "token2" }, RotateOptions({ ROTATE_ON, ROTATE_AUTO }, IntList({ 90, 180 })));
  dummymediaservice_->SetGetVideoSourceConfigurationOptions(videosourceconfigurationoptions);

  media::GetVideoSourceConfigurationOptionsResponse getvideosourceconfigurationoptionsresponse = client_->GetVideoSourceConfigurationOptionsFuture(boost::none, boost::none).get();
  ASSERT_GE(getvideosourceconfigurationoptionsresponse.latency_, 0);
  ASSERT_FALSE(getvideosourceconfigurationoptionsresponse.Error());
  ASSERT_FALSE(getvideosourceconfigurationoptionsresponse.configurationtoken_.is_initialized());
  ASSERT_FALSE(getvideosourceconfigurationoptionsresponse.profiletoken_.is_initialized());
  ASSERT_TRUE(getvideosourceconfigurationoptionsresponse.videosourceconfigurationoptions_.is_initialized());
  ASSERT_EQ(videosourceconfigurationoptions, *getvideosourceconfigurationoptionsresponse.videosourceconfigurationoptions_);
}

TEST_F(TestMediaClient, GetVideoSourceConfigurations)
{
  const std::vector<VideoSourceConfiguration> videosourceconfigurations =
  {
    VideoSourceConfiguration(std::string("name1"), 5, std::string("token1"), std::string("sourcetoken1"), IntRectangle(5, 5, 5, 5), Rotate(ROTATE_AUTO, 180)),
    VideoSourceConfiguration(std::string("name2"), 6, std::string("token2"), std::string("sourcetoken2"), IntRectangle(6, 6, 6, 6), Rotate(ROTATE_ON, 90))
  };
  dummymediaservice_->SetGetVideoSourceConfigurations(videosourceconfigurations);

  media::GetVideoSourceConfigurationsResponse getvideosourceconfigurationresponse = client_->GetVideoSourceConfigurationsFuture().get();
  ASSERT_GE(getvideosourceconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getvideosourceconfigurationresponse.Error());
  ASSERT_EQ(videosourceconfigurations, getvideosourceconfigurationresponse.configurations_);
}

TEST_F(TestMediaClient, GetVideoSourceModes)
{
  const std::string token("token");
  const std::vector<media::VideoSourceMode> videosourcemodes =
  {
    media::VideoSourceMode(2.0f, VideoResolution(6, 7), { VIDEOENCODING_JPEG, VIDEOENCODING_H264 }, false, std::string("description"), std::string("token1"), true),
    media::VideoSourceMode(1.0f, VideoResolution(5, 6), { VIDEOENCODING_JPEG, VIDEOENCODING_MPEG4 }, false, std::string("description"), std::string("token2"), true)
  };
  dummymediaservice_->SetGetVideoSourceModes(videosourcemodes);

  media::GetVideoSourceModesResponse getvideosourcemodesresponse = client_->GetVideoSourceModesFuture(token).get();
  ASSERT_GE(getvideosourcemodesresponse.latency_, 0);
  ASSERT_FALSE(getvideosourcemodesresponse.Error());
  ASSERT_EQ(token, getvideosourcemodesresponse.token_);
  ASSERT_EQ(videosourcemodes, getvideosourcemodesresponse.videosourcemodes_);
}

TEST_F(TestMediaClient, GetVideoSources)
{
  const std::vector<VideoSource> videosources =
  {
    VideoSource(std::string("token1"), 1.0f, VideoResolution(5, 5), ImagingSettings(BacklightCompensation(BACKLIGHTCOMPENSATIONMODE_OFF, 5.0f), 1.0f, 2.0f, 3.0f, Exposure(EXPOSUREMODE_MANUAL, EXPOSUREPRIORITY_FRAMERATE, Rectangle(1.0f, 2.0f, 3.0f, 4.0f), 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f), FocusConfiguration(AUTOFOCUSMODE_AUTO, 4.0f, 5.0f, 6.0f), IRCUTFILTERMODE_OFF, 4.0f, WideDynamicRange(WIDEDYNAMICMODE_OFF, 4.0f), WhiteBalance(WHITEBALANCEMODE_AUTO, 4.0f, 5.0f)), ImagingSettings20(BacklightCompensation20(BACKLIGHTCOMPENSATIONMODE_OFF, 5.0f), 1.0f, 2.0f, 3.0f, Exposure20(EXPOSUREMODE_MANUAL, EXPOSUREPRIORITY_FRAMERATE, Rectangle(1.0f, 2.0f, 3.0f, 4.0f), 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f), FocusConfiguration20(AUTOFOCUSMODE_AUTO, 4.0f, 5.0f, 6.0f), IRCUTFILTERMODE_OFF, 4.0f, WideDynamicRange20(WIDEDYNAMICMODE_OFF, 4.0f), WhiteBalance20(WHITEBALANCEMODE_AUTO, 4.0f, 5.0f), ImageStabilization(IMAGESTABILIZATIONMODE_OFF, 4.0f), IrCutFilterAutoAdjustment(std::string("test"), 4.0f, Duration(0, 1, 2, 3, 4, 5)), ToneCompensation(TONECOMPENSATIONMODE_OFF, 4.0f), Defogging(DEFOGGINGMODE_OFF, 4.0f), NoiseReduction(4.0f))),
    VideoSource(std::string("token2"), 1.0f, VideoResolution(5, 5), ImagingSettings(BacklightCompensation(BACKLIGHTCOMPENSATIONMODE_OFF, 5.0f), 1.0f, 2.0f, 3.0f, Exposure(EXPOSUREMODE_MANUAL, EXPOSUREPRIORITY_FRAMERATE, Rectangle(1.0f, 2.0f, 3.0f, 4.0f), 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f), FocusConfiguration(AUTOFOCUSMODE_AUTO, 4.0f, 5.0f, 6.0f), IRCUTFILTERMODE_OFF, 4.0f, WideDynamicRange(WIDEDYNAMICMODE_OFF, 4.0f), WhiteBalance(WHITEBALANCEMODE_AUTO, 4.0f, 5.0f)), ImagingSettings20(BacklightCompensation20(BACKLIGHTCOMPENSATIONMODE_OFF, 5.0f), 1.0f, 2.0f, 3.0f, Exposure20(EXPOSUREMODE_MANUAL, EXPOSUREPRIORITY_FRAMERATE, Rectangle(1.0f, 2.0f, 3.0f, 4.0f), 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f), FocusConfiguration20(AUTOFOCUSMODE_AUTO, 4.0f, 5.0f, 6.0f), IRCUTFILTERMODE_OFF, 4.0f, WideDynamicRange20(WIDEDYNAMICMODE_OFF, 4.0f), WhiteBalance20(WHITEBALANCEMODE_AUTO, 4.0f, 5.0f), ImageStabilization(IMAGESTABILIZATIONMODE_OFF, 4.0f), IrCutFilterAutoAdjustment(std::string("test"), 4.0f, Duration(0, 1, 2, 3, 4, 5)), ToneCompensation(TONECOMPENSATIONMODE_OFF, 4.0f), Defogging(DEFOGGINGMODE_OFF, 4.0f), NoiseReduction(4.0f)))
  };
  dummymediaservice_->SetGetVideoSources(videosources);

  media::GetVideoSourcesResponse getvideosourcesresponse = client_->GetVideoSourcesFuture().get();
  ASSERT_GE(getvideosourcesresponse.latency_, 0);
  ASSERT_FALSE(getvideosourcesresponse.Error());
  ASSERT_EQ(videosources, getvideosourcesresponse.videosources_);
}





TEST_F(TestMediaClient, RemoveAudioDecoderConfiguration)
{
  const std::string profiletoken("profiletoken");

  media::RemoveAudioDecoderConfigurationResponse removeaudiodecoderconfigurationresponse = client_->RemoveAudioDecoderConfigurationFuture(profiletoken).get();
  ASSERT_GE(removeaudiodecoderconfigurationresponse.latency_, 0);
  ASSERT_FALSE(removeaudiodecoderconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, removeaudiodecoderconfigurationresponse.profiletoken_);
}

TEST_F(TestMediaClient, RemoveAudioEncoderConfiguration)
{
  const std::string profiletoken("profiletoken");

  media::RemoveAudioEncoderConfigurationResponse removeaudioencoderconfigurationresponse = client_->RemoveAudioEncoderConfigurationFuture(profiletoken).get();
  ASSERT_GE(removeaudioencoderconfigurationresponse.latency_, 0);
  ASSERT_FALSE(removeaudioencoderconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, removeaudioencoderconfigurationresponse.profiletoken_);
}

TEST_F(TestMediaClient, RemoveAudioOutputConfiguration)
{
  const std::string profiletoken("profiletoken");

  media::RemoveAudioOutputConfigurationResponse removeaudiooutputconfigurationresponse = client_->RemoveAudioOutputConfigurationFuture(profiletoken).get();
  ASSERT_GE(removeaudiooutputconfigurationresponse.latency_, 0);
  ASSERT_FALSE(removeaudiooutputconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, removeaudiooutputconfigurationresponse.profiletoken_);
}

TEST_F(TestMediaClient, RemoveAudioSourceConfiguration)
{
  const std::string profiletoken("profiletoken");

  media::RemoveAudioSourceConfigurationResponse removeaudiosourceconfigurationresponse = client_->RemoveAudioSourceConfigurationFuture(profiletoken).get();
  ASSERT_GE(removeaudiosourceconfigurationresponse.latency_, 0);
  ASSERT_FALSE(removeaudiosourceconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, removeaudiosourceconfigurationresponse.profiletoken_);
}

TEST_F(TestMediaClient, RemoveMetadataConfiguration)
{
  const std::string profiletoken("profiletoken");

  media::RemoveMetadataConfigurationResponse removemetadataconfigurationresponse = client_->RemoveMetadataConfigurationFuture(profiletoken).get();
  ASSERT_GE(removemetadataconfigurationresponse.latency_, 0);
  ASSERT_FALSE(removemetadataconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, removemetadataconfigurationresponse.profiletoken_);
}

TEST_F(TestMediaClient, RemovePTZConfiguration)
{
  const std::string profiletoken("profiletoken");

  media::RemovePTZConfigurationResponse removeptzconfigurationresponse = client_->RemovePTZConfigurationFuture(profiletoken).get();
  ASSERT_GE(removeptzconfigurationresponse.latency_, 0);
  ASSERT_FALSE(removeptzconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, removeptzconfigurationresponse.profiletoken_);
}

TEST_F(TestMediaClient, RemoveVideoAnalyticsConfiguration)
{
  const std::string profiletoken("profiletoken");

  media::RemoveVideoAnalyticsConfigurationResponse removevideoanalyticsconfigurationresponse = client_->RemoveVideoAnalyticsConfigurationFuture(profiletoken).get();
  ASSERT_GE(removevideoanalyticsconfigurationresponse.latency_, 0);
  ASSERT_FALSE(removevideoanalyticsconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, removevideoanalyticsconfigurationresponse.profiletoken_);
}

TEST_F(TestMediaClient, RemoveVideoEncoderConfiguration)
{
  const std::string profiletoken("profiletoken");

  media::RemoveVideoEncoderConfigurationResponse removevideoencoderconfigurationresponse = client_->RemoveVideoEncoderConfigurationFuture(profiletoken).get();
  ASSERT_GE(removevideoencoderconfigurationresponse.latency_, 0);
  ASSERT_FALSE(removevideoencoderconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, removevideoencoderconfigurationresponse.profiletoken_);
}

TEST_F(TestMediaClient, RemoveVideoSourceConfiguration)
{
  const std::string profiletoken("profiletoken");

  media::RemoveVideoSourceConfigurationResponse removevideosourceconfigurationresponse = client_->RemoveVideoSourceConfigurationFuture(profiletoken).get();
  ASSERT_GE(removevideosourceconfigurationresponse.latency_, 0);
  ASSERT_FALSE(removevideosourceconfigurationresponse.Error());
  ASSERT_EQ(profiletoken, removevideosourceconfigurationresponse.profiletoken_);
}

TEST_F(TestMediaClient, SetAudioDecoderConfiguration)
{
  const AudioDecoderConfiguration configuration(std::string("name"), 5, std::string("token"));

  media::SetAudioDecoderConfigurationResponse setaudiodecoderconfigurationresponse = client_->SetAudioDecoderConfigurationFuture(configuration).get();
  ASSERT_GE(setaudiodecoderconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setaudiodecoderconfigurationresponse.Error());
  ASSERT_EQ(configuration, setaudiodecoderconfigurationresponse.configuration_);
}

TEST_F(TestMediaClient, SetAudioOutputConfiguration)
{
  const AudioOutputConfiguration configuration(std::string("name"), 5, std::string("token"), std::string("outputtoken"), std::string("sendprimacy"), 5);

  media::SetAudioOutputConfigurationResponse setaudiooutputconfigurationresponse = client_->SetAudioOutputConfigurationFuture(configuration).get();
  ASSERT_GE(setaudiooutputconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setaudiooutputconfigurationresponse.Error());
  ASSERT_EQ(configuration, setaudiooutputconfigurationresponse.configuration_);
}

TEST_F(TestMediaClient, SetAudioSourceConfiguration)
{
  const AudioSourceConfiguration configuration(std::string("name"), 5, std::string("token"), std::string("sourcetoken"));

  media::SetAudioSourceConfigurationResponse setaudiosourceconfigurationresponse = client_->SetAudioSourceConfigurationFuture(configuration).get();
  ASSERT_GE(setaudiosourceconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setaudiosourceconfigurationresponse.Error());
  ASSERT_EQ(configuration, setaudiosourceconfigurationresponse.configuration_);
}

TEST_F(TestMediaClient, SetMetadataConfiguration)
{
  /*TODO const MetadataConfiguration configuration(std::string("token1"), METADATACOMPRESSIONTYPE_GZIP, std::string("name"), 5, PTZFilter(true, false), EventSubscription(ws::FilterType({ ws::TopicExpression(std::string("dialect1"), std::string("expression")), ws::TopicExpression(std::string("dialect2"), std::string("expression")) }, { ws::MessageContent(std::string("dialect1"), std::string("expression")), ws::MessageContent(std::string("dialect2"), std::string("expression")) })), true, MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5), AnalyticsEngineConfiguration({ Config(ItemList({ SimpleItem(std::string("name1"), std::string("value1")), SimpleItem(std::string("name2"), std::string("value2")) },{ ElementItem({ Element("name",{ { "a", "b" },{ "c", "d" } }, "text",{ Element("name2",{ { "e", "f" },{ "g", "h" } }, "text2",{ Element("name3",{ { "i", "j" },{ "k", "l" } }, "text3",{}) }) }) }), ElementItem({ Element("name",{ { "z", "y" },{ "c", "d" } }, "text",{ Element("name5",{ { "e", "f" },{ "g", "h" } }, "text5",{ Element("name6",{ { "i", "j" },{ "k", "l" } }, "text6",{}) }) }) }) }), std::string("name"), std::string("type")) }));

  media::SetMetadataConfigurationResponse setmetadataconfigurationresponse = client_->SetMetadataConfigurationFuture(configuration).get();
  ASSERT_GE(setmetadataconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setmetadataconfigurationresponse.Error());
  ASSERT_EQ(configuration, setmetadataconfigurationresponse.configuration_);*/
}

TEST_F(TestMediaClient, SetSynchronizationPoint)
{
  const std::string profiletoken("profiletoken");

  media::SetSynchronizationPointResponse setsynchronizationpointresponse = client_->SetSynchronizationPointFuture(profiletoken).get();
  ASSERT_GE(setsynchronizationpointresponse.latency_, 0);
  ASSERT_FALSE(setsynchronizationpointresponse.Error());
  ASSERT_EQ(profiletoken, setsynchronizationpointresponse.token_);
}

TEST_F(TestMediaClient, SetVideoEncoderConfiguration)
{
  const VideoEncoderConfiguration videoencoderconfiguration(std::string("name"), 5, std::string("token"), VIDEOENCODING_H264, VideoResolution(5, 5), 5.0f, VideoRateControl(5, 5, 5), Mpeg4Configuration(5, MPEG4PROFILE_ASP), H264Configuration(5, H264PROFILE_BASELINE), MulticastConfiguration(IPAddress(IPTYPE_IPV4, std::string("5.5.5.5"), boost::none), 5, 5, true), Duration(0, 1, 2, 3, 4, 5));

  media::SetVideoEncoderConfigurationResponse setvideoencoderconfigurationresponse = client_->SetVideoEncoderConfigurationFuture(videoencoderconfiguration).get();
  ASSERT_GE(setvideoencoderconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setvideoencoderconfigurationresponse.Error());
  ASSERT_EQ(videoencoderconfiguration, setvideoencoderconfigurationresponse.videoencoderconfiguration_);
}

TEST_F(TestMediaClient, SetVideoSourceConfiguration)
{
  const VideoSourceConfiguration videosourceconfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken"), IntRectangle(5, 5, 5, 5), Rotate(ROTATE_AUTO, 180));

  media::SetVideoSourceConfigurationResponse setvideosourceconfigurationresponse = client_->SetVideoSourceConfigurationFuture(videosourceconfiguration).get();
  ASSERT_GE(setvideosourceconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setvideosourceconfigurationresponse.Error());
  ASSERT_EQ(videosourceconfiguration, setvideosourceconfigurationresponse.videosourceconfiguration_);
}

TEST_F(TestMediaClient, StartMulticastStreaming)
{
  const std::string token("token");
  
  media::StartMulticastStreamingResponse startmulticaststreamingresponse = client_->StartMulticastStreamingFuture(token).get();
  ASSERT_GE(startmulticaststreamingresponse.latency_, 0);
  ASSERT_FALSE(startmulticaststreamingresponse.Error());
  ASSERT_EQ(token, startmulticaststreamingresponse.token_);
}

TEST_F(TestMediaClient, StopulticastStreaming)
{
  const std::string token("token");
  
  media::StopMulticastStreamingResponse stopmulticaststreamingresponse = client_->StopMulticastStreamingFuture(token).get();
  ASSERT_GE(stopmulticaststreamingresponse.latency_, 0);
  ASSERT_FALSE(stopmulticaststreamingresponse.Error());
  ASSERT_EQ(token, stopmulticaststreamingresponse.token_);
}

}

}
