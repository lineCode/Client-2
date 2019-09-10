// deviceio.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Classes /////

class DeviceIOSignals
{
 public:

  Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioOutputConfigurationResponse, std::string> getaudiooutputconfiguration_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioOutputConfigurationOptionsResponse, std::string> getaudiooutputconfigurationoptions_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioOutputsResponse> getaudiooutputs_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioSourceConfigurationResponse, std::string> getaudiosourceconfiguration_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioSourceConfigurationOptionsResponse, std::string> getaudiosourceconfigurationoptions_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioSourcesResponse> getaudiosources_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetDigitalInputConfigurationOptionsResponse, std::string> getdigitalinputconfigurationoptions_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetDigitalInputsResponse> getdigitalinputs_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetRelayOutputsResponse> getrelayoutputs_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetRelayOutputOptionsResponse, std::string> getrelayoutputoptions_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetSerialPortConfigurationResponse, std::string> getserialportconfiguration_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetSerialPortConfigurationOptionsResponse, std::string> getserialportconfigurationoptions_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetSerialPortsResponse> getserialports_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, GetServiceCapabilitiesResponse> getservicecapabilities_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, SetAudioOutputConfigurationResponse, AudioOutputConfiguration, bool> setaudiooutputconfiguration_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, SetAudioSourceConfigurationResponse, AudioSourceConfiguration, bool> setaudiosourceconfiguration_;
  Signal< DEVICEIOOPERATION, DeviceIOClient, SetDigitalInputConfigurationsResponse, std::vector<DigitalInput> > setdigitalinputconfigurations_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, SetRelayOutputSettingsResponse, RelayOutput> setrelayoutputsettings_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, SetRelayOutputStateResponse, std::string, RELAYLOGICALSTATE> setrelayoutputstate_;
  Signal<DEVICEIOOPERATION, DeviceIOClient, SetSerialPortConfigurationResponse, SerialPortConfiguration, bool> setserialportconfiguration_;

};

///// Methods /////

DeviceIOClient::DeviceIOClient() :
  signals_(new DeviceIOSignals(
  {
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioOutputConfigurationResponse, std::string>(this, DEVICEIOOPERATION_GETAUDIOOUTPUTCONFIGURATION, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetAudioOutputConfiguration"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioOutputConfigurationOptionsResponse, std::string>(this, DEVICEIOOPERATION_GETAUDIOOUTPUTCONFIGURATIONOPTIONS, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetAudioOutputConfigurationOptions"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioOutputsResponse>(this, DEVICEIOOPERATION_GETAUDIOOUTPUTS, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetAudioOutputs"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioSourceConfigurationResponse, std::string>(this, DEVICEIOOPERATION_GETAUDIOSOURCECONFIGURATION, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetAudioSourceConfiguration"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioSourceConfigurationOptionsResponse, std::string>(this, DEVICEIOOPERATION_GETAUDIOSOURCECONFIGURATIONOPTIONS, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetAudioSourceConfigurationOptions"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioSourcesResponse>(this, DEVICEIOOPERATION_GETAUDIOSOURCES, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetAudioSources"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetDigitalInputConfigurationOptionsResponse, std::string>(this, DEVICEIOOPERATION_GETDIGITALINPUTCONFIGURATIONOPTIONS, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetDigitalInputConfigurationOptions"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetDigitalInputsResponse>(this, DEVICEIOOPERATION_GETDIGITALINPUTS, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetDigitalInputs"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetRelayOutputsResponse>(this, DEVICEIOOPERATION_GETRELAYOUTPUTS, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetRelayOutputs"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetRelayOutputOptionsResponse, std::string>(this, DEVICEIOOPERATION_GETRELAYOUTPUTOPTIONS, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetRelayOutputOptions"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetSerialPortConfigurationResponse, std::string>(this, DEVICEIOOPERATION_GETSERIALPORTCONFIGURATION, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetSerialPortConfigurations"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetSerialPortConfigurationOptionsResponse, std::string>(this, DEVICEIOOPERATION_GETSERIALPORTCONFIGURATIONOPTIONS, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetSerialPortConfigurationOptions"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetSerialPortsResponse>(this, DEVICEIOOPERATION_GETSERIALPORTS, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetSerialPorts"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, GetServiceCapabilitiesResponse>(this, DEVICEIOOPERATION_GETSERVICECAPABILITIES, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/GetServiceCapabilities"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, SetAudioOutputConfigurationResponse, AudioOutputConfiguration, bool>(this, DEVICEIOOPERATION_SETAUDIOOUTPUTCONFIGURATION, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/SetAudioOutputConfiguration"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, SetAudioSourceConfigurationResponse, AudioSourceConfiguration, bool>(this, DEVICEIOOPERATION_SETAUDIOSOURCECONFIGURATION, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/SetAudioSourceConfiguration"), false),
    Signal< DEVICEIOOPERATION, DeviceIOClient, SetDigitalInputConfigurationsResponse, std::vector<DigitalInput> >(this, DEVICEIOOPERATION_SETDIGITALINPUTCONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/SetDigitalInputConfigurations"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, SetRelayOutputSettingsResponse, RelayOutput>(this, DEVICEIOOPERATION_SETRELAYOUTPUTSETTINGS, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/SetRelayOutputSettings"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, SetRelayOutputStateResponse, std::string, RELAYLOGICALSTATE>(this, DEVICEIOOPERATION_SETRELAYOUTPUTSTATE, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/SetRelayOutputState"), false),
    Signal<DEVICEIOOPERATION, DeviceIOClient, SetSerialPortConfigurationResponse, SerialPortConfiguration, bool>(this, DEVICEIOOPERATION_SETSERIALPORTCONFIGURATION, true, std::string("http://www.onvif.org/ver10/deviceio/wsdl/SetSerialPortConfiguration"), false)
  }))
{

}

DeviceIOClient::~DeviceIOClient()
{
  if (signals_)
  {
    delete signals_;
    signals_ = nullptr;
  }
}

void DeviceIOClient::Destroy()
{
  Client::Destroy();

  signals_->getaudiooutputconfiguration_.Destroy();
  signals_->getaudiooutputconfigurationoptions_.Destroy();
  signals_->getaudiooutputs_.Destroy();
  signals_->getaudiosourceconfiguration_.Destroy();
  signals_->getaudiosourceconfigurationoptions_.Destroy();
  signals_->getaudiosources_.Destroy();
  signals_->getdigitalinputconfigurationoptions_.Destroy();
  signals_->getdigitalinputs_.Destroy();
  signals_->getrelayoutputs_.Destroy();
  signals_->getrelayoutputoptions_.Destroy();
  signals_->getserialportconfiguration_.Destroy();
  signals_->getserialportconfigurationoptions_.Destroy();
  signals_->getserialports_.Destroy();
  signals_->setaudiooutputconfiguration_.Destroy();
  signals_->setaudiosourceconfiguration_.Destroy();
  signals_->setdigitalinputconfigurations_.Destroy();
  signals_->setrelayoutputsettings_.Destroy();
  signals_->setrelayoutputstate_.Destroy();
  signals_->setserialportconfiguration_.Destroy();
}

// Requests
void DeviceIOClient::GetAudioOutputConfiguration(const std::string& audiooutputtoken)
{
  signals_->getaudiooutputconfiguration_.Create(GetAudioOutputConfigurationBody(audiooutputtoken), audiooutputtoken);
}

void DeviceIOClient::GetAudioOutputConfigurationOptions(const std::string& audiooutputtoken)
{
  signals_->getaudiooutputconfigurationoptions_.Create(GetAudioOutputConfigurationOptionsBody(audiooutputtoken), audiooutputtoken);
}

void DeviceIOClient::GetAudioOutputs()
{
  signals_->getaudiooutputs_.Create(GetAudioOutputsBody());
}

void DeviceIOClient::GetAudioSourceConfiguration(const std::string& audiooutputtoken)
{
  signals_->getaudiosourceconfiguration_.Create(GetAudioSourceConfigurationBody(audiooutputtoken), audiooutputtoken);
}

void DeviceIOClient::GetAudioSourceConfigurationOptions(const std::string& audiooutputtoken)
{
  signals_->getaudiosourceconfigurationoptions_.Create(GetAudioSourceConfigurationOptionsBody(audiooutputtoken), audiooutputtoken);
}

void DeviceIOClient::GetAudioSources()
{
  signals_->getaudiosources_.Create(GetAudioSourcesBody());
}

void DeviceIOClient::GetDigitalInputConfigurationOptions(const std::string& token)
{
  signals_->getdigitalinputconfigurationoptions_.Create(GetDigitalInputConfigurationOptionsBody(token), token);
}

void DeviceIOClient::GetDigitalInputs()
{
  signals_->getdigitalinputs_.Create(GetDigitalInputsBody());
}

void DeviceIOClient::GetRelayOutputOptions(const std::string& relayoutputtoken)
{
  signals_->getrelayoutputoptions_.Create(GetRelayOutputOptionsBody(relayoutputtoken), relayoutputtoken);
}

void DeviceIOClient::GetRelayOutputs()
{
  signals_->getrelayoutputs_.Create(GetRelayOutputsBody());
}

void DeviceIOClient::GetSerialPortConfiguration(const std::string& serialporttoken)
{
  signals_->getserialportconfiguration_.Create(GetSerialPortConfigurationBody(serialporttoken), serialporttoken);
}

void DeviceIOClient::GetSerialPortConfigurationOptions(const std::string& serialporttoken)
{
  signals_->getserialportconfigurationoptions_.Create(GetSerialPortConfigurationOptionsBody(serialporttoken), serialporttoken);
}

void DeviceIOClient::GetSerialPorts()
{
  signals_->getserialports_.Create(GetSerialPortsBody());
}

void DeviceIOClient::GetServiceCapabilities()
{
  signals_->getservicecapabilities_.Create(GetServiceCapabilitiesBody());
}

void DeviceIOClient::SetAudioOutputConfiguration(const AudioOutputConfiguration& configuration, bool forcepersistence)
{
  signals_->setaudiooutputconfiguration_.Create(SetAudioOutputConfigurationBody(configuration, forcepersistence), configuration, forcepersistence);
}

void DeviceIOClient::SetAudioSourceConfiguration(const AudioSourceConfiguration& configuration, bool forcepersistence)
{
  signals_->setaudiosourceconfiguration_.Create(SetAudioSourceConfigurationBody(configuration, forcepersistence), configuration, forcepersistence);
}

void DeviceIOClient::SetDigitalInputConfigurations(const std::vector<DigitalInput>& digitalinputs)
{
  signals_->setdigitalinputconfigurations_.Create(SetDigitalInputConfigurationsBody(digitalinputs), digitalinputs);
}

void DeviceIOClient::SetRelayOutputSettings(const RelayOutput& relayoutput)
{
  signals_->setrelayoutputsettings_.Create(SetRelayOutputSettingsBody(relayoutput), relayoutput);
}

void DeviceIOClient::SetRelayOutputState(const std::string& relayoutputtoken, RELAYLOGICALSTATE logicalstate)
{
  signals_->setrelayoutputstate_.Create(SetRelayOutputStateBody(relayoutputtoken, logicalstate), relayoutputtoken, logicalstate);
}

void DeviceIOClient::SetSerialPortConfiguration(const SerialPortConfiguration& serialportconfiguration, bool forcepersistence)
{
  signals_->setserialportconfiguration_.Create(SetSerialPortConfigurationBody(serialportconfiguration, forcepersistence), serialportconfiguration, forcepersistence);
}

// Callbacks
Connection DeviceIOClient::GetAudioOutputConfigurationCallback(const std::string& audiooutputtoken, boost::function<void(const GetAudioOutputConfigurationResponse&)> callback)
{
  return signals_->getaudiooutputconfiguration_.CreateCallback(GetAudioOutputConfigurationBody(audiooutputtoken), callback, audiooutputtoken);
}

Connection DeviceIOClient::GetAudioOutputConfigurationOptionsCallback(const std::string& audiooutputtoken, boost::function<void(const GetAudioOutputConfigurationOptionsResponse&)> callback)
{
  return signals_->getaudiooutputconfigurationoptions_.CreateCallback(GetAudioOutputConfigurationOptionsBody(audiooutputtoken), callback, audiooutputtoken);
}

Connection DeviceIOClient::GetAudioOutputsCallback(boost::function<void(const GetAudioOutputsResponse&)> callback)
{
  return signals_->getaudiooutputs_.CreateCallback(GetAudioOutputsBody(), callback);
}

Connection DeviceIOClient::GetAudioSourceConfigurationCallback(const std::string& audiooutputtoken, boost::function<void(const GetAudioSourceConfigurationResponse&)> callback)
{
  return signals_->getaudiosourceconfiguration_.CreateCallback(GetAudioSourceConfigurationBody(audiooutputtoken), callback, audiooutputtoken);
}

Connection DeviceIOClient::GetAudioSourceConfigurationOptionsCallback(const std::string& audiooutputtoken, boost::function<void(const GetAudioSourceConfigurationOptionsResponse&)> callback)
{
  return signals_->getaudiosourceconfigurationoptions_.CreateCallback(GetAudioSourceConfigurationOptionsBody(audiooutputtoken), callback, audiooutputtoken);
}

Connection DeviceIOClient::GetAudioSourcesCallback(boost::function<void(const GetAudioSourcesResponse&)> callback)
{
  return signals_->getaudiosources_.CreateCallback(GetAudioSourcesBody(), callback);
}

Connection DeviceIOClient::GetDigitalInputConfigurationOptionsCallback(const std::string& token, boost::function<void(const GetDigitalInputConfigurationOptionsResponse&)> callback)
{
  return signals_->getdigitalinputconfigurationoptions_.CreateCallback(GetDigitalInputConfigurationOptionsBody(token), callback, token);
}

Connection DeviceIOClient::GetDigitalInputsCallback(boost::function<void(const GetDigitalInputsResponse&)> callback)
{
  return signals_->getdigitalinputs_.CreateCallback(GetDigitalInputsBody(), callback);
}

Connection DeviceIOClient::GetRelayOutputOptionsCallback(const std::string& relayoutputtoken, boost::function<void(const GetRelayOutputOptionsResponse&)> callback)
{
  return signals_->getrelayoutputoptions_.CreateCallback(GetRelayOutputOptionsBody(relayoutputtoken), callback, relayoutputtoken);
}

Connection DeviceIOClient::GetRelayOutputsCallback(boost::function<void(const GetRelayOutputsResponse&)> callback)
{
  return signals_->getrelayoutputs_.CreateCallback(GetRelayOutputsBody(), callback);
}

Connection DeviceIOClient::GetSerialPortConfigurationCallback(const std::string& serialporttoken, boost::function<void(const GetSerialPortConfigurationResponse&)> callback)
{
  return signals_->getserialportconfiguration_.CreateCallback(GetSerialPortConfigurationBody(serialporttoken), callback, serialporttoken);
}

Connection DeviceIOClient::GetSerialPortConfigurationOptionsCallback(const std::string& serialporttoken, boost::function<void(const GetSerialPortConfigurationOptionsResponse&)> callback)
{
  return signals_->getserialportconfigurationoptions_.CreateCallback(GetSerialPortConfigurationOptionsBody(serialporttoken), callback, serialporttoken);
}

Connection DeviceIOClient::GetSerialPortsCallback(boost::function<void(const GetSerialPortsResponse&)> callback)
{
  return signals_->getserialports_.CreateCallback(GetSerialPortsBody(), callback);
}

Connection DeviceIOClient::GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback)
{
  return signals_->getservicecapabilities_.CreateCallback(GetServiceCapabilitiesBody(), callback);
}

Connection DeviceIOClient::SetAudioOutputConfigurationCallback(const AudioOutputConfiguration& configuration, bool forcepersistence, boost::function<void(const SetAudioOutputConfigurationResponse&)> callback)
{
  return signals_->setaudiooutputconfiguration_.CreateCallback(SetAudioOutputConfigurationBody(configuration, forcepersistence), callback, configuration, forcepersistence);
}

Connection DeviceIOClient::SetAudioSourceConfigurationCallback(const AudioSourceConfiguration& configuration, bool forcepersistence, boost::function<void(const SetAudioSourceConfigurationResponse&)> callback)
{
  return signals_->setaudiosourceconfiguration_.CreateCallback(SetAudioSourceConfigurationBody(configuration, forcepersistence), callback, configuration, forcepersistence);
}

Connection DeviceIOClient::SetDigitalInputConfigurationsCallback(const std::vector<DigitalInput>& digitalinputs, boost::function<void(const SetDigitalInputConfigurationsResponse&)> callback)
{
  return signals_->setdigitalinputconfigurations_.CreateCallback(SetDigitalInputConfigurationsBody(digitalinputs), callback, digitalinputs);
}

Connection DeviceIOClient::SetRelayOutputSettingsCallback(const RelayOutput& relayoutput, boost::function<void(const SetRelayOutputSettingsResponse&)> callback)
{
  return signals_->setrelayoutputsettings_.CreateCallback(SetRelayOutputSettingsBody(relayoutput), callback, relayoutput);
}

Connection DeviceIOClient::SetRelayOutputStateCallback(const std::string& relayoutputtoken, RELAYLOGICALSTATE logicalstate, boost::function<void(const SetRelayOutputStateResponse&)> callback)
{
  return signals_->setrelayoutputstate_.CreateCallback(SetRelayOutputStateBody(relayoutputtoken, logicalstate), callback, relayoutputtoken, logicalstate);
}

Connection DeviceIOClient::SetSerialPortConfigurationCallback(const SerialPortConfiguration& serialportconfiguration, bool forcepersistence, boost::function<void(const SetSerialPortConfigurationResponse&)> callback)
{
  return signals_->setserialportconfiguration_.CreateCallback(SetSerialPortConfigurationBody(serialportconfiguration, forcepersistence), callback, serialportconfiguration, forcepersistence);
}

// Futures
boost::unique_future<GetAudioOutputConfigurationResponse> DeviceIOClient::GetAudioOutputConfigurationFuture(const std::string& audiooutputtoken)
{
  return signals_->getaudiooutputconfiguration_.CreateFuture(GetAudioOutputConfigurationBody(audiooutputtoken), audiooutputtoken);
}

boost::unique_future<GetAudioOutputConfigurationOptionsResponse> DeviceIOClient::GetAudioOutputConfigurationOptionsFuture(const std::string& audiooutputtoken)
{
  return signals_->getaudiooutputconfigurationoptions_.CreateFuture(GetAudioOutputConfigurationOptionsBody(audiooutputtoken), audiooutputtoken);
}

boost::unique_future<GetAudioOutputsResponse> DeviceIOClient::GetAudioOutputsFuture()
{
  return signals_->getaudiooutputs_.CreateFuture(GetAudioOutputsBody());
}

boost::unique_future<GetAudioSourceConfigurationResponse> DeviceIOClient::GetAudioSourceConfigurationFuture(const std::string& audiooutputtoken)
{
  return signals_->getaudiosourceconfiguration_.CreateFuture(GetAudioSourceConfigurationBody(audiooutputtoken), audiooutputtoken);
}

boost::unique_future<GetAudioSourceConfigurationOptionsResponse> DeviceIOClient::GetAudioSourceConfigurationOptionsFuture(const std::string& audiooutputtoken)
{
  return signals_->getaudiosourceconfigurationoptions_.CreateFuture(GetAudioSourceConfigurationOptionsBody(audiooutputtoken), audiooutputtoken);
}

boost::unique_future<GetAudioSourcesResponse> DeviceIOClient::GetAudioSourcesFuture()
{
  return signals_->getaudiosources_.CreateFuture(GetAudioSourcesBody());
}

boost::unique_future<GetDigitalInputConfigurationOptionsResponse> DeviceIOClient::GetDigitalInputConfigurationOptionsFuture(const std::string& token)
{
  return signals_->getdigitalinputconfigurationoptions_.CreateFuture(GetDigitalInputConfigurationOptionsBody(token), token);
}

boost::unique_future<GetDigitalInputsResponse> DeviceIOClient::GetDigitalInputsFuture()
{
  return signals_->getdigitalinputs_.CreateFuture(GetDigitalInputsBody());
}

boost::unique_future<GetRelayOutputOptionsResponse> DeviceIOClient::GetRelayOutputOptionsFuture(const std::string& relayoutputtoken)
{
  return signals_->getrelayoutputoptions_.CreateFuture(GetRelayOutputOptionsBody(relayoutputtoken), relayoutputtoken);
}

boost::unique_future<GetRelayOutputsResponse> DeviceIOClient::GetRelayOutputsFuture()
{
  return signals_->getrelayoutputs_.CreateFuture(GetRelayOutputsBody());
}

boost::unique_future<GetSerialPortConfigurationResponse> DeviceIOClient::GetSerialPortConfigurationFuture(const std::string& serialporttoken)
{
  return signals_->getserialportconfiguration_.CreateFuture(GetSerialPortConfigurationBody(serialporttoken), serialporttoken);
}

boost::unique_future<GetSerialPortConfigurationOptionsResponse> DeviceIOClient::GetSerialPortConfigurationOptionsFuture(const std::string& serialporttoken)
{
  return signals_->getserialportconfigurationoptions_.CreateFuture(GetSerialPortConfigurationOptionsBody(serialporttoken), serialporttoken);
}

boost::unique_future<GetSerialPortsResponse> DeviceIOClient::GetSerialPortsFuture()
{
  return signals_->getserialports_.CreateFuture(GetSerialPortsBody());
}

boost::unique_future<GetServiceCapabilitiesResponse> DeviceIOClient::GetServiceCapabilitiesFuture()
{
  return signals_->getservicecapabilities_.CreateFuture(GetServiceCapabilitiesBody());
}

boost::unique_future<SetAudioOutputConfigurationResponse> DeviceIOClient::SetAudioOutputConfigurationFuture(const AudioOutputConfiguration& configuration, bool forcepersistence)
{
  return signals_->setaudiooutputconfiguration_.CreateFuture(SetAudioOutputConfigurationBody(configuration, forcepersistence), configuration, forcepersistence);
}

boost::unique_future<SetAudioSourceConfigurationResponse> DeviceIOClient::SetAudioSourceConfigurationFuture(const AudioSourceConfiguration& configuration, bool forcepersistence)
{
  return signals_->setaudiosourceconfiguration_.CreateFuture(SetAudioSourceConfigurationBody(configuration, forcepersistence), configuration, forcepersistence);
}

boost::unique_future<SetDigitalInputConfigurationsResponse> DeviceIOClient::SetDigitalInputConfigurationsFuture(const std::vector<DigitalInput>& digitalinputs)
{
  return signals_->setdigitalinputconfigurations_.CreateFuture(SetDigitalInputConfigurationsBody(digitalinputs), digitalinputs);
}

boost::unique_future<SetRelayOutputSettingsResponse> DeviceIOClient::SetRelayOutputSettingsFuture(const RelayOutput& relayoutput)
{
  return signals_->setrelayoutputsettings_.CreateFuture(SetRelayOutputSettingsBody(relayoutput), relayoutput);
}

boost::unique_future<SetRelayOutputStateResponse> DeviceIOClient::SetRelayOutputStateFuture(const std::string& relayoutputtoken, RELAYLOGICALSTATE logicalstate)
{
  return signals_->setrelayoutputstate_.CreateFuture(SetRelayOutputStateBody(relayoutputtoken, logicalstate), relayoutputtoken, logicalstate);
}

boost::unique_future<SetSerialPortConfigurationResponse> DeviceIOClient::SetSerialPortConfigurationFuture(const SerialPortConfiguration& serialportconfiguration, bool forcepersistence)
{
  return signals_->setserialportconfiguration_.CreateFuture(SetSerialPortConfigurationBody(serialportconfiguration, forcepersistence), serialportconfiguration, forcepersistence);
}

// Signals
boost::signals2::signal<void(const GetAudioOutputConfigurationResponse&)>& DeviceIOClient::GetAudioOutputConfigurationSignal()
{
  return signals_->getaudiooutputconfiguration_.GetSignal();
}

boost::signals2::signal<void(const GetAudioOutputConfigurationOptionsResponse&)>& DeviceIOClient::GetAudioOutputConfigurationOptionsSignal()
{
  return signals_->getaudiooutputconfigurationoptions_.GetSignal();
}

boost::signals2::signal<void(const GetAudioOutputsResponse&)>& DeviceIOClient::GetAudioOutputsSignal()
{
  return signals_->getaudiooutputs_.GetSignal();
}

boost::signals2::signal<void(const GetAudioSourceConfigurationResponse&)>& DeviceIOClient::GetAudioSourceConfigurationSignal()
{
  return signals_->getaudiosourceconfiguration_.GetSignal();
}

boost::signals2::signal<void(const GetAudioSourceConfigurationOptionsResponse&)>& DeviceIOClient::GetAudioSourceConfigurationOptionsSignal()
{
  return signals_->getaudiosourceconfigurationoptions_.GetSignal();
}

boost::signals2::signal<void(const GetAudioSourcesResponse&)>& DeviceIOClient::GetAudioSourcesSignal()
{
  return signals_->getaudiosources_.GetSignal();
}

boost::signals2::signal<void(const GetDigitalInputConfigurationOptionsResponse&)>& DeviceIOClient::GetDigitalInputConfigurationOptionsSignal()
{
  return signals_->getdigitalinputconfigurationoptions_.GetSignal();
}

boost::signals2::signal<void(const GetDigitalInputsResponse&)>& DeviceIOClient::GetDigitalInputsSignal()
{
  return signals_->getdigitalinputs_.GetSignal();
}

boost::signals2::signal<void(const GetRelayOutputsResponse&)>& DeviceIOClient::GetRelayOutputsSignal()
{
  return signals_->getrelayoutputs_.GetSignal();
}

boost::signals2::signal<void(const GetRelayOutputOptionsResponse&)>& DeviceIOClient::GetRelayOutputOptionsSignal()
{
  return signals_->getrelayoutputoptions_.GetSignal();
}

boost::signals2::signal<void(const GetSerialPortConfigurationResponse&)>& DeviceIOClient::GetSerialPortConfigurationSignal()
{
  return signals_->getserialportconfiguration_.GetSignal();
}

boost::signals2::signal<void(const GetSerialPortConfigurationOptionsResponse&)>& DeviceIOClient::GetSerialPortConfigurationOptionsSignal()
{
  return signals_->getserialportconfigurationoptions_.GetSignal();
}

boost::signals2::signal<void(const GetSerialPortsResponse&)>& DeviceIOClient::GetSerialPortsSignal()
{
  return signals_->getserialports_.GetSignal();
}

boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& DeviceIOClient::GetServiceCapabilitiesSignal()
{
  return signals_->getservicecapabilities_.GetSignal();
}

boost::signals2::signal<void(const SetAudioOutputConfigurationResponse&)>& DeviceIOClient::SetAudioOutputConfigurationSignal()
{
  return signals_->setaudiooutputconfiguration_.GetSignal();
}

boost::signals2::signal<void(const SetAudioSourceConfigurationResponse&)>& DeviceIOClient::SetAudioSourceConfigurationSignal()
{
  return signals_->setaudiosourceconfiguration_.GetSignal();
}

boost::signals2::signal<void(const SetDigitalInputConfigurationsResponse&)>& DeviceIOClient::SetDigitalInputConfigurationsSignal()
{
  return signals_->setdigitalinputconfigurations_.GetSignal();
}

boost::signals2::signal<void(const SetRelayOutputSettingsResponse&)>& DeviceIOClient::SetRelayOutputSettingsSignal()
{
  return signals_->setrelayoutputsettings_.GetSignal();
}

boost::signals2::signal<void(const SetRelayOutputStateResponse&)>& DeviceIOClient::SetRelayOutputStateSignal()
{
  return signals_->setrelayoutputstate_.GetSignal();
}

boost::signals2::signal<void(const SetSerialPortConfigurationResponse&)>& DeviceIOClient::SetSerialPortConfigurationSignal()
{
  return signals_->setserialportconfiguration_.GetSignal();
}

void DeviceIOClient::Update(DEVICEIOOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata)
{
  switch (operation)
  {
    case DEVICEIOOPERATION_GETAUDIOOUTPUTCONFIGURATION:
    {
      auto getaudiooutputconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioOutputConfigurationResponse']");
      if (!getaudiooutputconfigurationresponse)
      {
        SignalError(DEVICEIOOPERATION_GETAUDIOOUTPUTCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioOutputConfigurationResponse element not found"));
        break;
      }

      signals_->getaudiooutputconfiguration_.Emit(handle, localendpoint, latency, std::string(), GetClass<AudioOutputConfiguration>(getaudiooutputconfigurationresponse, "*[local-name()='AudioOutputConfiguration']"));
      break;
    }
    case DEVICEIOOPERATION_GETAUDIOOUTPUTCONFIGURATIONOPTIONS:
    {
      auto getaudiooutputconfigurationoptionsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioOutputConfigurationOptionsResponse']");
      if (!getaudiooutputconfigurationoptionsresponse)
      {
        SignalError(DEVICEIOOPERATION_GETAUDIOOUTPUTCONFIGURATIONOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioOutputConfigurationOptionsResponse element not found"));
        break;
      }

      signals_->getaudiooutputconfigurationoptions_.Emit(handle, localendpoint, latency, std::string(), GetClass<AudioOutputConfigurationOptions>(getaudiooutputconfigurationoptionsresponse, "*[local-name()='AudioOutputOptions']"));
      break;
    }
    case DEVICEIOOPERATION_GETAUDIOOUTPUTS:
    {
      auto getaudiooutputsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioOutputsResponse']");
      if (!getaudiooutputsresponse)
      {
        SignalError(DEVICEIOOPERATION_GETAUDIOOUTPUTS, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioOutputsResponse element not found"));
        break;
      }

      std::vector<std::string> tokens;
      for (const auto& token : getaudiooutputsresponse.node().select_nodes("*[local-name()='Token']"))
      {
        tokens.push_back(token.node().text().get());

      }

      signals_->getaudiooutputs_.Emit(handle, localendpoint, latency, std::string(), tokens);
      break;
    }
    case DEVICEIOOPERATION_GETAUDIOSOURCECONFIGURATION:
    {
      auto getaudiosourceconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSourceConfigurationResponse']");
      if (!getaudiosourceconfigurationresponse)
      {
        SignalError(DEVICEIOOPERATION_GETAUDIOSOURCECONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioSourceConfigurationResponse element not found"));
        break;
      }

      signals_->getaudiosourceconfiguration_.Emit(handle, localendpoint, latency, std::string(), GetClass<AudioSourceConfiguration>(getaudiosourceconfigurationresponse, "*[local-name()='AudioSourceConfiguration']"));
      break;
    }
    case DEVICEIOOPERATION_GETAUDIOSOURCECONFIGURATIONOPTIONS:
    {
      auto getaudiosourceconfigurationoptionsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSourceConfigurationOptionsResponse']");
      if (!getaudiosourceconfigurationoptionsresponse)
      {
        SignalError(DEVICEIOOPERATION_GETAUDIOSOURCECONFIGURATIONOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioSourceConfigurationOptionsResponse element not found"));
        break;
      }

      signals_->getaudiosourceconfigurationoptions_.Emit(handle, localendpoint, latency, std::string(), GetClass<AudioSourceConfigurationOptions>(getaudiosourceconfigurationoptionsresponse, "*[local-name()='AudioSourceOptions']"));
      break;
    }
    case DEVICEIOOPERATION_GETAUDIOSOURCES:
    {
      auto getaudiosourcesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSourcesResponse']");
      if (!getaudiosourcesresponse)
      {
        SignalError(DEVICEIOOPERATION_GETAUDIOSOURCES, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioSourcesResponse element not found"));
        break;
      }

      std::vector<std::string> tokens;
      for (const auto& token : getaudiosourcesresponse.node().select_nodes("*[local-name()='Token']"))
      {
        tokens.push_back(token.node().text().get());

      }

      signals_->getaudiosources_.Emit(handle, localendpoint, latency, std::string(), tokens);
      break;
    }
    case DEVICEIOOPERATION_GETDIGITALINPUTCONFIGURATIONOPTIONS:
    {
      auto getdigitalinputconfigurationoptionsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDigitalInputConfigurationOptionsResponse']");
      if (!getdigitalinputconfigurationoptionsresponse)
      {
        SignalError(DEVICEIOOPERATION_GETDIGITALINPUTCONFIGURATIONOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetDigitalInputConfigurationOptionsResponse element not found"));
        break;
      }

      std::vector<DIGITALIDLESTATE> idlestates;
      for (const auto& idlestate : getdigitalinputconfigurationoptionsresponse.node().select_nodes("*[local-name()='IdleState']"))
      {
        const boost::optional<DIGITALIDLESTATE> idlestateenum = GetDigitalIdleState(idlestate.node().text().get());
        if (idlestateenum.is_initialized())
        {
          idlestates.push_back(*idlestateenum);

        }
      }

      signals_->getdigitalinputconfigurationoptions_.Emit(handle, localendpoint, latency, std::string(), idlestates);
      break;
    }
    case DEVICEIOOPERATION_GETDIGITALINPUTS:
    {
      auto getdigitalinputsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDigitalInputsResponse']");
      if (!getdigitalinputsresponse)
      {
        SignalError(DEVICEIOOPERATION_GETDIGITALINPUTS, handle, localendpoint, latency, std::string("/Envelope/Body/GetDigitalInputsResponse element not found"));
        break;
      }

      std::vector<DigitalInput> digitalinputs;
      for (const auto& digitalinput : getdigitalinputsresponse.node().select_nodes("*[local-name()='DigitalInputs']"))
      {
        digitalinputs.push_back(DigitalInput(digitalinput.node()));

      }

      signals_->getdigitalinputs_.Emit(handle, localendpoint, latency, std::string(), digitalinputs);
      break;
    }
    case DEVICEIOOPERATION_GETRELAYOUTPUTOPTIONS:
    {
      auto getrelayoutputoptionsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRelayOutputOptionsResponse']");
      if (!getrelayoutputoptionsresponse)
      {
        SignalError(DEVICEIOOPERATION_GETRELAYOUTPUTOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetRelayOutputOptionsResponse element not found"));
        break;
      }

      std::vector<RelayOutputOptions> relayoutputoptions;
      for (const auto& relayoutputoption : getrelayoutputoptionsresponse.node().select_nodes("*[local-name()='RelayOutputOptions']"))
      {
        relayoutputoptions.push_back(RelayOutputOptions(relayoutputoption.node()));

      }

      signals_->getrelayoutputoptions_.Emit(handle, localendpoint, latency, std::string(), relayoutputoptions);
      break;
    }
    case DEVICEIOOPERATION_GETRELAYOUTPUTS:
    {
      auto getrelayoutputsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRelayOutputsResponse']");
      if (!getrelayoutputsresponse)
      {
        SignalError(DEVICEIOOPERATION_GETRELAYOUTPUTS, handle, localendpoint, latency, std::string("/Envelope/Body/GetRelayOutputsResponse element not found"));
        break;
      }

      std::vector<RelayOutput> relayoutputs;
      for (const auto& relayoutput : getrelayoutputsresponse.node().select_nodes("*[local-name()='RelayOutputs']"))
      {
        relayoutputs.push_back(RelayOutput(relayoutput.node()));

      }

      signals_->getrelayoutputs_.Emit(handle, localendpoint, latency, std::string(), relayoutputs);
      break;
    }
    case DEVICEIOOPERATION_GETSERIALPORTCONFIGURATION:
    {
      auto getserialportconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSerialPortConfigurationResponse']");
      if (!getserialportconfigurationresponse)
      {
        SignalError(DEVICEIOOPERATION_GETSERIALPORTCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetSerialPortConfigurationResponse element not found"));
        break;
      }

      signals_->getserialportconfiguration_.Emit(handle, localendpoint, latency, std::string(), GetClass<SerialPortConfiguration>(getserialportconfigurationresponse, "*[local-name()='SerialPortConfiguration']"));
      break;
    }
    case DEVICEIOOPERATION_GETSERIALPORTCONFIGURATIONOPTIONS:
    {
      auto getserialportconfigurationoptionsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSerialPortConfigurationOptionsResponse']");
      if (!getserialportconfigurationoptionsresponse)
      {
        SignalError(DEVICEIOOPERATION_GETSERIALPORTCONFIGURATIONOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetSerialPortConfigurationOptionsResponse element not found"));
        break;
      }

      signals_->getserialportconfigurationoptions_.Emit(handle, localendpoint, latency, std::string(), GetClass<SerialPortConfigurationOptions>(getserialportconfigurationoptionsresponse, "*[local-name()='SerialPortOptions']"));
      break;
    }
    case DEVICEIOOPERATION_GETSERIALPORTS:
    {
      auto getserialportsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSerialPortsResponse']");
      if (!getserialportsresponse)
      {
        SignalError(DEVICEIOOPERATION_GETSERIALPORTS, handle, localendpoint, latency, std::string("/Envelope/Body/GetSerialPortsResponse element not found"));
        break;
      }

      std::vector<SerialPort> serialports;
      for (const auto& serialport : getserialportsresponse.node().select_nodes("*[local-name()='SerialPort']"))
      {
        serialports.push_back(SerialPort(serialport.node()));

      }

      signals_->getserialports_.Emit(handle, localendpoint, latency, std::string(), serialports);
      break;
    }
    case DEVICEIOOPERATION_GETSERVICECAPABILITIES:
    {
      auto getservicecapabilitiesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilitiesResponse']");
      if (!getservicecapabilitiesresponse)
      {
        SignalError(DEVICEIOOPERATION_GETSERVICECAPABILITIES, handle, localendpoint, latency, std::string("/Envelope/Body/GetServiceCapabilitiesResponse element not found"));
        break;
      }

      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, std::string(), GetClass<Capabilities>(getservicecapabilitiesresponse, "*[local-name()='Capabilities']"));
      break;
    }
    case DEVICEIOOPERATION_SETAUDIOOUTPUTCONFIGURATION:
    {
      auto setaudiooutputconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAudioOutputConfigurationResponse']");
      if (!setaudiooutputconfigurationresponse)
      {
        SignalError(DEVICEIOOPERATION_SETAUDIOOUTPUTCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetAudioOutputConfigurationResponse element not found"));
        break;
      }

      signals_->setaudiooutputconfiguration_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEIOOPERATION_SETAUDIOSOURCECONFIGURATION:
    {
      auto setaudiosourceconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAudioSourceConfigurationResponse']");
      if (!setaudiosourceconfigurationresponse)
      {
        SignalError(DEVICEIOOPERATION_SETAUDIOSOURCECONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetAudioSourceConfigurationResponse element not found"));
        break;
      }

      signals_->setaudiosourceconfiguration_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEIOOPERATION_SETDIGITALINPUTCONFIGURATIONS:
    {
      auto setdigitalinputconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetDigitalInputConfigurationsResponse']");
      if (!setdigitalinputconfigurationsresponse)
      {
        SignalError(DEVICEIOOPERATION_SETDIGITALINPUTCONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/SetDigitalInputConfigurationsResponse element not found"));
        break;
      }

      signals_->setdigitalinputconfigurations_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEIOOPERATION_SETRELAYOUTPUTSETTINGS:
    {
      auto setrelayoutputsettingsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRelayOutputSettingsResponse']");
      if (!setrelayoutputsettingsresponse)
      {
        SignalError(DEVICEIOOPERATION_SETRELAYOUTPUTSETTINGS, handle, localendpoint, latency, std::string("/Envelope/Body/SetRelayOutputSettingsResponse element not found"));
        break;
      }

      signals_->setrelayoutputsettings_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEIOOPERATION_SETRELAYOUTPUTSTATE:
    {
      auto setrelayoutputstateresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRelayOutputStateResponse']");
      if (!setrelayoutputstateresponse)
      {
        SignalError(DEVICEIOOPERATION_SETRELAYOUTPUTSTATE, handle, localendpoint, latency, std::string("/Envelope/Body/SetRelayOutputStateResponse element not found"));
        break;
      }

      signals_->setrelayoutputstate_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case DEVICEIOOPERATION_SETSERIALPORTCONFIGURATION:
    {
      auto setserialportconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetSerialPortConfigurationResponse']");
      if (!setserialportconfigurationresponse)
      {
        SignalError(DEVICEIOOPERATION_SETSERIALPORTCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetSerialPortConfigurationResponse element not found"));
        break;
      }

      signals_->setserialportconfiguration_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    default:
    {

      break;
    }
  }
}

void DeviceIOClient::SignalError(DEVICEIOOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message)
{
  switch (operation)
  {
    case DEVICEIOOPERATION_GETAUDIOOUTPUTCONFIGURATION:
    {
      signals_->getaudiooutputconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETAUDIOOUTPUTCONFIGURATIONOPTIONS:
    {
      signals_->getaudiooutputconfigurationoptions_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETAUDIOOUTPUTS:
    {
      signals_->getaudiooutputs_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETAUDIOSOURCECONFIGURATION:
    {
      signals_->getaudiosourceconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETAUDIOSOURCECONFIGURATIONOPTIONS:
    {
      signals_->getaudiosourceconfigurationoptions_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETAUDIOSOURCES:
    {
      signals_->getaudiosources_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETDIGITALINPUTS:
    {
      signals_->getdigitalinputs_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETDIGITALINPUTCONFIGURATIONOPTIONS:
    {
      signals_->getdigitalinputconfigurationoptions_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETRELAYOUTPUTOPTIONS:
    {
      signals_->getrelayoutputoptions_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETRELAYOUTPUTS:
    {
      signals_->getrelayoutputs_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETSERIALPORTCONFIGURATION:
    {
      signals_->getserialportconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETSERIALPORTCONFIGURATIONOPTIONS:
    {
      signals_->getserialportconfigurationoptions_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETSERIALPORTS:
    {
      signals_->getserialports_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_GETSERVICECAPABILITIES:
    {
      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_SETAUDIOOUTPUTCONFIGURATION:
    {
      signals_->setaudiooutputconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_SETAUDIOSOURCECONFIGURATION:
    {
      signals_->setaudiosourceconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_SETDIGITALINPUTCONFIGURATIONS:
    {
      signals_->setdigitalinputconfigurations_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_SETRELAYOUTPUTSETTINGS:
    {
      signals_->setrelayoutputsettings_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_SETRELAYOUTPUTSTATE:
    {
      signals_->setrelayoutputstate_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case DEVICEIOOPERATION_SETSERIALPORTCONFIGURATION:
    {
      signals_->setserialportconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    default:
    {
       assert(false);
       break;
    }
  }
}

std::string DeviceIOClient::GetAudioOutputConfigurationBody(const std::string& audiooutputtoken)
{
  return (std::string("<s:Body><deviceio:GetAudioOutputConfiguration>") + onvif::ToXml("tt:AudioOutputToken", boost::optional<std::string>(audiooutputtoken)) + std::string("</deviceio:GetAudioOutputConfiguration></s:Body>"));
}

std::string DeviceIOClient::GetAudioOutputConfigurationOptionsBody(const std::string& audiooutputtoken)
{
  return (std::string("<s:Body><deviceio:GetAudioOutputConfigurationOptions>") + onvif::ToXml("tt:AudioOutputToken", boost::optional<std::string>(audiooutputtoken)) + std::string("</deviceio:GetAudioOutputConfigurationOptions></s:Body>"));
}

std::string DeviceIOClient::GetAudioOutputsBody()
{
  return std::string("<s:Body><deviceio:GetAudioOutputs/></s:Body>");
}

std::string DeviceIOClient::GetAudioSourceConfigurationBody(const std::string& audiooutputtoken)
{
  return (std::string("<s:Body><deviceio:GetAudioSourceConfiguration>") + onvif::ToXml("tt:AudioOutputToken", boost::optional<std::string>(audiooutputtoken)) + std::string("</deviceio:GetAudioSourceConfiguration></s:Body>"));
}

std::string DeviceIOClient::GetAudioSourceConfigurationOptionsBody(const std::string& audiooutputtoken)
{
  return (std::string("<s:Body><deviceio:GetAudioSourceConfigurationOptions>") + onvif::ToXml("tt:AudioOutputToken", boost::optional<std::string>(audiooutputtoken)) + std::string("</deviceio:GetAudioSourceConfigurationOptions></s:Body>"));
}

std::string DeviceIOClient::GetAudioSourcesBody()
{
  return std::string("<s:Body><deviceio:GetAudioSources/></s:Body>");
}

std::string DeviceIOClient::GetDigitalInputConfigurationOptionsBody(const std::string& token)
{
  return (std::string("<s:Body><deviceio:GetDigitalInputConfigurationOptions>") + onvif::ToXml("tt:Token", boost::optional<std::string>(token)) + std::string("</deviceio:GetDigitalInputConfigurationOptions></s:Body>"));
}

std::string DeviceIOClient::GetDigitalInputsBody()
{
  return std::string("<s:Body><deviceio:GetDigitalInputs/></s:Body>");
}

std::string DeviceIOClient::GetRelayOutputOptionsBody(const std::string& relayoutputtoken)
{
  return (std::string("<s:Body><deviceio:GetRelayOutputOptions>") + onvif::ToXml("tt:RelayOutputToken", boost::optional<std::string>(relayoutputtoken)) + std::string("</deviceio:GetRelayOutputOptions></s:Body>"));
}

std::string DeviceIOClient::GetRelayOutputsBody()
{
  return std::string("<s:Body><deviceio:GetRelayOutputs/></s:Body>");
}

std::string DeviceIOClient::GetSerialPortConfigurationBody(const std::string& serialporttoken)
{
  return (std::string("<s:Body><deviceio:GetSerialPortConfiguration>") + onvif::ToXml("tt:SerialPortToken", boost::optional<std::string>(serialporttoken)) + std::string("</deviceio:GetSerialPortConfiguration></s:Body>"));
}

std::string DeviceIOClient::GetSerialPortConfigurationOptionsBody(const std::string& serialporttoken)
{
  return (std::string("<s:Body><deviceio:GetSerialPortConfigurationOptions>") + onvif::ToXml("SerialPortToken", boost::optional<std::string>(serialporttoken)) + std::string("</deviceio:GetSerialPortConfigurationOptions></s:Body>"));
}

std::string DeviceIOClient::GetSerialPortsBody()
{
  return std::string("<s:Body><deviceio:GetSerialPorts/></s:Body>");
}

std::string DeviceIOClient::GetServiceCapabilitiesBody()
{
  return std::string("<s:Body><deviceio:GetServiceCapabilities/></s:Body>");
}

std::string DeviceIOClient::SetAudioOutputConfigurationBody(const AudioOutputConfiguration& configuration, bool forcepersistence)
{
  return (std::string("<s:Body><deviceio:SetAudioOutputConfiguration>") + configuration.ToXml("tt:Configuration") + onvif::ToXml("tt:ForcePersistence", boost::optional<bool>(forcepersistence)) + std::string("</deviceio:SetAudioOutputConfiguration></s:Body>"));
}

std::string DeviceIOClient::SetAudioSourceConfigurationBody(const AudioSourceConfiguration& configuration, bool forcepersistence)
{
  return (std::string("<s:Body><deviceio:SetAudioSourceConfiguration>") + configuration.ToXml("tt:Configuration") + onvif::ToXml("tt:ForcePersistence", boost::optional<bool>(forcepersistence)) + std::string("</deviceio:SetAudioSourceConfiguration></s:Body>"));
}

std::string DeviceIOClient::SetDigitalInputConfigurationsBody(const std::vector<DigitalInput>& digitalinputs)
{
  return (std::string("<s:Body><deviceio:SetDigitalInputConfigurations>") + onvif::ToXml("tt:DigitalInputs", digitalinputs) + std::string("</deviceio:SetDigitalInputConfigurations></s:Body>"));
}

std::string DeviceIOClient::SetRelayOutputSettingsBody(const RelayOutput& relayoutput)
{
  return (std::string("<s:Body><deviceio:SetRelayOutputSettings>") + relayoutput.ToXml("tt:RelayOutput") + std::string("</deviceio:SetRelayOutputSettings></s:Body>"));
}

std::string DeviceIOClient::SetRelayOutputStateBody(const std::string& relayoutputtoken, RELAYLOGICALSTATE logicalstate)
{
  return (std::string("<s:Body><deviceio:SetRelayOutputState>") + onvif::ToXml("tt:RelayOutputToken", boost::optional<std::string>(relayoutputtoken)) + onvif::ToXmlEnum("tt:LogicalState", logicalstate) + std::string("</deviceio:SetRelayOutputState></s:Body>"));
}

std::string DeviceIOClient::SetSerialPortConfigurationBody(const SerialPortConfiguration& serialportconfiguration, bool forcepersistence)
{
  return (std::string("<s:Body><deviceio:SetSerialPortConfiguration>") + serialportconfiguration.ToXml("tt:SerialPortConfiguration") + onvif::ToXml("tt:ForcePersistence", boost::optional<bool>(forcepersistence)) + std::string("</deviceio:SetSerialPortConfiguration></s:Body>"));
}

}

}
