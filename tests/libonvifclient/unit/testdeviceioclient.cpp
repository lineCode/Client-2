// testdeviceioclient.cpp
//

///// Includes /////

#include "testdeviceioclient.hpp"

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
const std::string DEVICEIOURI("/onvif/deviceio");

///// Methods /////

TestDeviceIOClient::TestDeviceIOClient()
{
  
}

TestDeviceIOClient::~TestDeviceIOClient()
{
  
}

void TestDeviceIOClient::SetUp()
{
  Init(PORT, DEVICEIOURI);

  // Setup dummy server
  dummydeviceioservice_ = std::make_unique<DummyDeviceIOService>();
  ASSERT_FALSE(httpserver_.AddServer(DEVICEIOURI, dummydeviceioservice_.get()));
}

void TestDeviceIOClient::TearDown()
{
  ASSERT_FALSE(httpserver_.RemoveServer(DEVICEIOURI));
  dummydeviceioservice_.reset();

  Destroy();
}

///// Tests /////

TEST_F(TestDeviceIOClient, GetAudioOutputConfiguration)
{
  const std::string audiooutputtoken("audiooutputtoken");
  const deviceio::AudioOutputConfiguration audiooutputconfiguration(std::string("name"), 5, std::string("token"), std::string("outputtoken"), std::string("sendprimacy"), 5);
  dummydeviceioservice_->SetGetAudioOutputConfiguration(audiooutputconfiguration);

  deviceio::GetAudioOutputConfigurationResponse getaudiooutputconfigurationresponse = client_->GetAudioOutputConfigurationFuture(audiooutputtoken).get();
  ASSERT_GE(getaudiooutputconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getaudiooutputconfigurationresponse.Error());
  ASSERT_EQ(audiooutputtoken, getaudiooutputconfigurationresponse.audiooutputtoken_);
  ASSERT_EQ(audiooutputconfiguration, *getaudiooutputconfigurationresponse.audiooutputconfiguration_);
}

TEST_F(TestDeviceIOClient, GetAudioOutputConfigurationOptions)
{
  const std::string audiooutputtoken("audiooutputtoken");
  const AudioOutputConfigurationOptions audiooutputconfigurationoptions({ std::string("token1"), std::string("token2") }, { std::string("primacy1"), std::string("primacy2") }, IntRange(5, 6));
  dummydeviceioservice_->SetGetAudioOutputConfigurationOptions(audiooutputconfigurationoptions);

  deviceio::GetAudioOutputConfigurationOptionsResponse getaudiooutputconfigurationoptionsresponse = client_->GetAudioOutputConfigurationOptionsFuture(audiooutputtoken).get();
  ASSERT_GE(getaudiooutputconfigurationoptionsresponse.latency_, 0);
  ASSERT_FALSE(getaudiooutputconfigurationoptionsresponse.Error());
  ASSERT_EQ(audiooutputtoken, getaudiooutputconfigurationoptionsresponse.audiooutputtoken_);
  ASSERT_EQ(audiooutputconfigurationoptions, *getaudiooutputconfigurationoptionsresponse.audiooutputoptions_);
}

TEST_F(TestDeviceIOClient, GetDigitalInputConfigurationOptions)
{
  const std::string token("token");
  const std::vector<DIGITALIDLESTATE> idlestate = { DIGITALIDLESTATE_OPEN };
  dummydeviceioservice_->SetGetDigitalInputConfigurationOptions(idlestate);

  deviceio::GetDigitalInputConfigurationOptionsResponse getdigitalinputconfigurationoptionsresponse = client_->GetDigitalInputConfigurationOptionsFuture(token).get();
  ASSERT_GE(getdigitalinputconfigurationoptionsresponse.latency_, 0);
  ASSERT_FALSE(getdigitalinputconfigurationoptionsresponse.Error());
  ASSERT_EQ(token, getdigitalinputconfigurationoptionsresponse.token_);
  ASSERT_EQ(idlestate, getdigitalinputconfigurationoptionsresponse.idlestate_);
}

TEST_F(TestDeviceIOClient, GetDigitalInputs)
{
  const std::vector<DigitalInput> digitalinputs;
  dummydeviceioservice_->SetGetDigitalInputs(digitalinputs);

  deviceio::GetDigitalInputsResponse getdigitalinputsresponse = client_->GetDigitalInputsFuture().get();
  ASSERT_GE(getdigitalinputsresponse.latency_, 0);
  ASSERT_FALSE(getdigitalinputsresponse.Error());
  ASSERT_EQ(digitalinputs, getdigitalinputsresponse.digitalinputs_);
}

TEST_F(TestDeviceIOClient, GetAudioOutputs)
{
  const std::vector<std::string> token =
  {
    std::string("token1"),
    std::string("token2")
  };
  dummydeviceioservice_->SetGetAudioOutputs(token);

  deviceio::GetAudioOutputsResponse getaudiooutputsresponse = client_->GetAudioOutputsFuture().get();
  ASSERT_GE(getaudiooutputsresponse.latency_, 0);
  ASSERT_FALSE(getaudiooutputsresponse.Error());
  ASSERT_EQ(token, getaudiooutputsresponse.token_);
}

TEST_F(TestDeviceIOClient, GetAudioSourceConfiguration)
{
  const std::string audiosourcetoken("audiosourcetoken");
  const AudioSourceConfiguration audiosourceconfiguration(std::string("name"), 5, std::string("token"), std::string("sourcetoken"));
  dummydeviceioservice_->SetGetAudioSourceConfiguration(audiosourceconfiguration);

  deviceio::GetAudioSourceConfigurationResponse getaudiosourceconfigurationresponse = client_->GetAudioSourceConfigurationFuture(audiosourcetoken).get();
  ASSERT_GE(getaudiosourceconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getaudiosourceconfigurationresponse.Error());
  ASSERT_EQ(audiosourcetoken, getaudiosourceconfigurationresponse.audiosourcetoken_);
  ASSERT_EQ(audiosourceconfiguration, *getaudiosourceconfigurationresponse.audiosourceconfiguration_);
}

TEST_F(TestDeviceIOClient, GetAudioSourceConfigurationOptions)
{
  const std::string audiosourcetoken("audiosourcetoken");
  const AudioSourceConfigurationOptions audiosourceconfigurationoptions({ std::string("token1"), std::string("token2") });
  dummydeviceioservice_->SetGetAudioSourceConfigurationOptions(audiosourceconfigurationoptions);

  deviceio::GetAudioSourceConfigurationOptionsResponse getaudiosourceconfigurationoptionsresponse = client_->GetAudioSourceConfigurationOptionsFuture(audiosourcetoken).get();
  ASSERT_GE(getaudiosourceconfigurationoptionsresponse.latency_, 0);
  ASSERT_FALSE(getaudiosourceconfigurationoptionsresponse.Error());
  ASSERT_EQ(audiosourcetoken, getaudiosourceconfigurationoptionsresponse.audiosourcetoken_);
  ASSERT_EQ(audiosourceconfigurationoptions, *getaudiosourceconfigurationoptionsresponse.audiosourceoptions_);
}

TEST_F(TestDeviceIOClient, GetAudioSources)
{
  const std::vector<std::string> token =
  {
    std::string("token1"),
    std::string("token2")
  };
  dummydeviceioservice_->SetGetAudioSources(token);

  deviceio::GetAudioSourcesResponse getaudiosourcesresponse = client_->GetAudioSourcesFuture().get();
  ASSERT_GE(getaudiosourcesresponse.latency_, 0);
  ASSERT_FALSE(getaudiosourcesresponse.Error());
  ASSERT_EQ(token, getaudiosourcesresponse.token_);
}

TEST_F(TestDeviceIOClient, GetRelayOutputOptions)
{
  const std::string relayoutputtoken("relayoutputtoken");
  const std::vector<deviceio::RelayOutputOptions> relayoutputoptions =
  {
    deviceio::RelayOutputOptions(std::string("token1"), { RELAYMODE_MONOSTABLE }, { 1, 2, 3 }, true),
    deviceio::RelayOutputOptions(std::string("token2"), { RELAYMODE_BISTABLE }, { 3, 4, 5 }, true)
  };
  dummydeviceioservice_->SetGetRelayOutputOptions(relayoutputoptions);

  deviceio::GetRelayOutputOptionsResponse getrelayoutputoptionsresponse = client_->GetRelayOutputOptionsFuture(relayoutputtoken).get();
  ASSERT_GE(getrelayoutputoptionsresponse.latency_, 0);
  ASSERT_FALSE(getrelayoutputoptionsresponse.Error());
  ASSERT_EQ(relayoutputtoken, getrelayoutputoptionsresponse.relayoutputtoken_);
  ASSERT_EQ(relayoutputoptions, getrelayoutputoptionsresponse.relayoutputoptions_);
}

TEST_F(TestDeviceIOClient, GetRelayOutputs)
{
  const std::vector<RelayOutput> relayoutputs =
  {
    RelayOutput(std::string("token1"), RelayOutputSettings(RELAYMODE_MONOSTABLE, Duration(1, 2, 3, 4, 5, 6), RELAYIDLESTATE_OPEN)),
    RelayOutput(std::string("token2"), RelayOutputSettings(RELAYMODE_MONOSTABLE, Duration(1, 2, 3, 4, 5, 6), RELAYIDLESTATE_OPEN))
  };
  dummydeviceioservice_->SetGetRelayOutputs(relayoutputs);

  deviceio::GetRelayOutputsResponse getrelayoutputsresponse = client_->GetRelayOutputsFuture().get();
  ASSERT_GE(getrelayoutputsresponse.latency_, 0);
  ASSERT_FALSE(getrelayoutputsresponse.Error());
  ASSERT_EQ(relayoutputs, getrelayoutputsresponse.relayoutputs_);
}

TEST_F(TestDeviceIOClient, GetSerialPortConfiguration)
{
  const std::string serialporttoken("serialporttoken");
  const deviceio::SerialPortConfiguration serialportconfiguration(std::string("token"), deviceio::SERIALPORTTYPE_RS485HALFDUPLEX, 5, deviceio::PARITYBIT_ODD, 5, 5.0f);
  dummydeviceioservice_->SetGetSerialPortConfiguration(serialportconfiguration);

  deviceio::GetSerialPortConfigurationResponse getserialportconfiguration = client_->GetSerialPortConfigurationFuture(serialporttoken).get();
  ASSERT_GE(getserialportconfiguration.latency_, 0);
  ASSERT_FALSE(getserialportconfiguration.Error());
  ASSERT_EQ(serialporttoken, getserialportconfiguration.serialporttoken_);
  ASSERT_EQ(serialportconfiguration, *getserialportconfiguration.serialportconfiguration_);
}

TEST_F(TestDeviceIOClient, GetSerialPortConfigurationOptions)
{
  const std::string serialporttoken("serialporttoken");
  const deviceio::SerialPortConfigurationOptions serialportoptions(IntList({ 5, 6, 7 }), deviceio::ParityBitList({ deviceio::PARITYBIT_MARK, deviceio::PARITYBIT_ODD }), IntList({ 5, 6, 7 }), FloatList({ 5.0f, 6.0f, 7.0f }), std::string("token"));
  dummydeviceioservice_->SetGetSerialPortConfigurationOptions(serialportoptions);

  deviceio::GetSerialPortConfigurationOptionsResponse getserialpotconfigurationoptionsresponse = client_->GetSerialPortConfigurationOptionsFuture(serialporttoken).get();
  ASSERT_GE(getserialpotconfigurationoptionsresponse.latency_, 0);
  ASSERT_FALSE(getserialpotconfigurationoptionsresponse.Error());
  ASSERT_EQ(serialporttoken, getserialpotconfigurationoptionsresponse.serialporttoken_);
  ASSERT_EQ(serialportoptions, *getserialpotconfigurationoptionsresponse.serialportoptions_);
}

TEST_F(TestDeviceIOClient, GetSerialPorts)
{
  const std::vector<deviceio::SerialPort> serialport =
  {
    deviceio::SerialPort(std::string("token1")),
    deviceio::SerialPort(std::string("token2"))
  };
  dummydeviceioservice_->SetGetSerialPorts(serialport);

  deviceio::GetSerialPortsResponse getserialportsresponse = client_->GetSerialPortsFuture().get();
  ASSERT_GE(getserialportsresponse.latency_, 0);
  ASSERT_FALSE(getserialportsresponse.Error());
  ASSERT_EQ(serialport, getserialportsresponse.serialport_);
}

TEST_F(TestDeviceIOClient, GetServiceCapabilities)
{
  const deviceio::Capabilities capabilities(5, 6, 7, 8, 9, 10, 11, true);
  dummydeviceioservice_->SetGetServiceCapabilities(capabilities);

  deviceio::GetServiceCapabilitiesResponse getptzservicecapabilitiesresponse = client_->GetServiceCapabilitiesFuture().get();
  ASSERT_GE(getptzservicecapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getptzservicecapabilitiesresponse.Error());
  ASSERT_TRUE(getptzservicecapabilitiesresponse.capabilities_.is_initialized());
  ASSERT_EQ(capabilities, *getptzservicecapabilitiesresponse.capabilities_);
}

TEST_F(TestDeviceIOClient, SetAudioOutputConfiguration)
{
  const deviceio::AudioOutputConfiguration configuration(std::string("name"), 5, std::string("token"), std::string("outputtoken"), std::string("sendprimacy"), 5);
  const bool forcepersistence = true;

  deviceio::SetAudioOutputConfigurationResponse setaudiooutputconfigurationresponse = client_->SetAudioOutputConfigurationFuture(configuration, forcepersistence).get();
  ASSERT_GE(setaudiooutputconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setaudiooutputconfigurationresponse.Error());
  ASSERT_EQ(configuration, setaudiooutputconfigurationresponse.configuration_);
  ASSERT_EQ(forcepersistence, setaudiooutputconfigurationresponse.forcepersistence_);
}

TEST_F(TestDeviceIOClient, SetAudioSourceConfiguration)
{
  const AudioSourceConfiguration configuration(std::string("name"), 5, std::string("token"), std::string("sourcetoken"));
  const bool forcepersistence = true;

  deviceio::SetAudioSourceConfigurationResponse setaudiosourceconfigurationresponse = client_->SetAudioSourceConfigurationFuture(configuration, forcepersistence).get();
  ASSERT_GE(setaudiosourceconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setaudiosourceconfigurationresponse.Error());
  ASSERT_EQ(configuration, setaudiosourceconfigurationresponse.configuration_);
  ASSERT_EQ(forcepersistence, setaudiosourceconfigurationresponse.forcepersistence_);
}

TEST_F(TestDeviceIOClient, SetDigitalInputConfigurations)
{
  const std::vector<DigitalInput> digitalinputs =
  {
    DigitalInput(std::string("token"))

  };

  deviceio::SetDigitalInputConfigurationsResponse setdigitalinputconfigurationsresponse = client_->SetDigitalInputConfigurationsFuture(digitalinputs).get();
  ASSERT_GE(setdigitalinputconfigurationsresponse.latency_, 0);
  ASSERT_FALSE(setdigitalinputconfigurationsresponse.Error());
  ASSERT_EQ(digitalinputs, setdigitalinputconfigurationsresponse.digitalinputs_);
}

TEST_F(TestDeviceIOClient, SetRelayOutputSettings)
{
  const RelayOutput relayoutput(std::string("token"), RelayOutputSettings(RELAYMODE_MONOSTABLE, Duration(1, 2, 3, 4, 5, 6), RELAYIDLESTATE_OPEN));

  deviceio::SetRelayOutputSettingsResponse setrelayoutputsettingsresponse = client_->SetRelayOutputSettingsFuture(relayoutput).get();
  ASSERT_GE(setrelayoutputsettingsresponse.latency_, 0);
  ASSERT_FALSE(setrelayoutputsettingsresponse.Error());
  ASSERT_EQ(relayoutput, setrelayoutputsettingsresponse.relayoutput_);
}

TEST_F(TestDeviceIOClient, SetRelayOutputState)
{
  const std::string relayoutputtoken("relayoutputtoken");
  const RELAYLOGICALSTATE logicalstate = RELAYLOGICALSTATE_ACTIVE;

  deviceio::SetRelayOutputStateResponse setrelayoutputstateresponse = client_->SetRelayOutputStateFuture(relayoutputtoken, logicalstate).get();
  ASSERT_GE(setrelayoutputstateresponse.latency_, 0);
  ASSERT_FALSE(setrelayoutputstateresponse.Error());
  ASSERT_EQ(relayoutputtoken, setrelayoutputstateresponse.relayoutputtoken_);
  ASSERT_EQ(logicalstate, setrelayoutputstateresponse.logicalstate_);
}

TEST_F(TestDeviceIOClient, SetSerialPortConfiguration)
{
  const deviceio::SerialPortConfiguration serialportconfiguration(std::string("token"), deviceio::SERIALPORTTYPE_RS485HALFDUPLEX, 5, deviceio::PARITYBIT_ODD, 5, 5.0f);
  const bool forcepersistence = true;

  deviceio::SetSerialPortConfigurationResponse setserialportconfigurationresponse = client_->SetSerialPortConfigurationFuture(serialportconfiguration, forcepersistence).get();
  ASSERT_GE(setserialportconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setserialportconfigurationresponse.Error());
  ASSERT_EQ(serialportconfiguration, setserialportconfigurationresponse.serialportconfiguration_);
  ASSERT_EQ(forcepersistence, setserialportconfigurationresponse.forcepersistence_);
}

}

}
