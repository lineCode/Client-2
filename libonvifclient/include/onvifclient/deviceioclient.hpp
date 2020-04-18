// deviceio.hpp
//

#ifndef IDDAVK9R4E940ELP9BV3YJ3UUD9JUR6SVZ
#define IDDAVK9R4E940ELP9BV3YJ3UUD9JUR6SVZ

///// Includes /////

#include <curl/curl.h>
#include <onviftypes/onviftypes.hpp>

#include "client.hpp"
#include "response.hpp"
#include "signal.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Enumerations /////

enum DEVICEIOOPERATION
{
  DEVICEIOOPERATION_GETAUDIOOUTPUTCONFIGURATION,
  DEVICEIOOPERATION_GETAUDIOOUTPUTCONFIGURATIONOPTIONS,
  DEVICEIOOPERATION_GETAUDIOOUTPUTS,
  DEVICEIOOPERATION_GETAUDIOSOURCECONFIGURATION,
  DEVICEIOOPERATION_GETAUDIOSOURCECONFIGURATIONOPTIONS,
  DEVICEIOOPERATION_GETAUDIOSOURCES,
  DEVICEIOOPERATION_GETDIGITALINPUTCONFIGURATIONOPTIONS,
  DEVICEIOOPERATION_GETDIGITALINPUTS,
  DEVICEIOOPERATION_GETRELAYOUTPUTOPTIONS,
  DEVICEIOOPERATION_GETRELAYOUTPUTS,
  DEVICEIOOPERATION_GETSERIALPORTCONFIGURATIONOPTIONS,
  DEVICEIOOPERATION_GETSERIALPORTCONFIGURATION,
  DEVICEIOOPERATION_GETSERIALPORTS,
  DEVICEIOOPERATION_GETSERVICECAPABILITIES,
  DEVICEIOOPERATION_SETAUDIOOUTPUTCONFIGURATION,
  DEVICEIOOPERATION_SETAUDIOSOURCECONFIGURATION,
  DEVICEIOOPERATION_SETDIGITALINPUTCONFIGURATIONS,
  DEVICEIOOPERATION_SETRELAYOUTPUTSETTINGS,
  DEVICEIOOPERATION_SETRELAYOUTPUTSTATE,
  DEVICEIOOPERATION_SETSERIALPORTCONFIGURATION
};

///// Declarations /////

class DeviceIOClient;
class DeviceIOSignals;

///// Classes /////

class GetAudioOutputConfigurationOptionsResponse : public Response<DeviceIOClient>
{
 public:

  GetAudioOutputConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiooutputtoken);
  GetAudioOutputConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiooutputtoken, const boost::optional<AudioOutputConfigurationOptions>& audiooutputoptions);
  virtual ~GetAudioOutputConfigurationOptionsResponse();

  std::string audiooutputtoken_;

  boost::optional<AudioOutputConfigurationOptions> audiooutputoptions_;

};

class GetAudioOutputConfigurationResponse : public Response<DeviceIOClient>
{
 public:

  GetAudioOutputConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiooutputtoken);
  GetAudioOutputConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiooutputtoken, const boost::optional<AudioOutputConfiguration>& audiooutputconfiguration);
  virtual ~GetAudioOutputConfigurationResponse();

  std::string audiooutputtoken_;

  boost::optional<AudioOutputConfiguration> audiooutputconfiguration_;

};

class GetAudioOutputsResponse : public Response<DeviceIOClient>
{
 public:

  GetAudioOutputsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetAudioOutputsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& token);
  virtual ~GetAudioOutputsResponse();

  std::vector<std::string> token_;

};

class GetAudioSourceConfigurationOptionsResponse : public Response<DeviceIOClient>
{
 public:

  GetAudioSourceConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiosourcetoken);
  GetAudioSourceConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiosourcetoken, const boost::optional<AudioSourceConfigurationOptions>& audiosourceoptions);
  virtual ~GetAudioSourceConfigurationOptionsResponse();

  std::string audiosourcetoken_;

  boost::optional<AudioSourceConfigurationOptions> audiosourceoptions_;

};

class GetAudioSourceConfigurationResponse : public Response<DeviceIOClient>
{
 public:

  GetAudioSourceConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiosourcetoken);
  GetAudioSourceConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiosourcetoken, const boost::optional<AudioSourceConfiguration>& audiosourceconfiguration);
  virtual ~GetAudioSourceConfigurationResponse();

  std::string audiosourcetoken_;

  boost::optional<AudioSourceConfiguration> audiosourceconfiguration_;

};

class GetAudioSourcesResponse : public Response<DeviceIOClient>
{
 public:

  GetAudioSourcesResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetAudioSourcesResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& token);
  virtual ~GetAudioSourcesResponse();

  std::vector<std::string> token_;

};

class GetDigitalInputConfigurationOptionsResponse : public Response<DeviceIOClient>
{
 public:

  GetDigitalInputConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token);
  GetDigitalInputConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const std::vector<DIGITALIDLESTATE>& idlestate);
  virtual ~GetDigitalInputConfigurationOptionsResponse();

  std::string token_;

  std::vector<DIGITALIDLESTATE> idlestate_;

};

class GetDigitalInputsResponse : public Response<DeviceIOClient>
{
 public:

  GetDigitalInputsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetDigitalInputsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<DigitalInput>& digitalinputs);
  virtual ~GetDigitalInputsResponse();

  std::vector<DigitalInput> digitalinputs_;

};

class GetRelayOutputOptionsResponse : public Response<DeviceIOClient>
{
 public:

  GetRelayOutputOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& relayoutputtoken);
  GetRelayOutputOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& relayoutputtoken, const std::vector<RelayOutputOptions>& relayoutputoptions);
  virtual ~GetRelayOutputOptionsResponse();

  std::string relayoutputtoken_;

  std::vector<RelayOutputOptions> relayoutputoptions_;

};

class GetRelayOutputsResponse : public Response<DeviceIOClient>
{
 public:

  GetRelayOutputsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetRelayOutputsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<RelayOutput>& relayoutputs);
  virtual ~GetRelayOutputsResponse();

  std::vector<RelayOutput> relayoutputs_;

};

class GetSerialPortConfigurationOptionsResponse : public Response<DeviceIOClient>
{
 public:

  GetSerialPortConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& serialporttoken);
  GetSerialPortConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& serialporttoken, const boost::optional<SerialPortConfigurationOptions>& serialportoptions);
  virtual ~GetSerialPortConfigurationOptionsResponse();

  std::string serialporttoken_;

  boost::optional<SerialPortConfigurationOptions> serialportoptions_;

};

class GetSerialPortConfigurationResponse : public Response<DeviceIOClient>
{
 public:

  GetSerialPortConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& serialporttoken);
  GetSerialPortConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& serialporttoken, const boost::optional<SerialPortConfiguration>& serialportconfiguration);
  virtual ~GetSerialPortConfigurationResponse();

  std::string serialporttoken_;

  boost::optional<SerialPortConfiguration> serialportconfiguration_;

};

class GetSerialPortsResponse : public Response<DeviceIOClient>
{
 public:

  GetSerialPortsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetSerialPortsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<SerialPort>& serialport);
  virtual ~GetSerialPortsResponse();

  std::vector<SerialPort> serialport_;

};

class GetServiceCapabilitiesResponse : public Response<DeviceIOClient>
{
 public:

  GetServiceCapabilitiesResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetServiceCapabilitiesResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities);
  virtual ~GetServiceCapabilitiesResponse();

  inline int GetNumVideoSources() const { return ((capabilities_.is_initialized() && capabilities_->videosources_.is_initialized()) ? *capabilities_->videosources_ : 0); }
  inline int GetNumVideoOutputs() const { return ((capabilities_.is_initialized() && capabilities_->videooutputs_.is_initialized()) ? *capabilities_->videooutputs_ : 0); }
  inline int GetNumAudioSources() const { return ((capabilities_.is_initialized() && capabilities_->audiosources_.is_initialized()) ? *capabilities_->audiosources_ : 0); }
  inline int GetNumAudioOutputs() const { return ((capabilities_.is_initialized() && capabilities_->audiooutputs_.is_initialized()) ? *capabilities_->audiooutputs_ : 0); }
  inline int GetNumRelayOutputs() const { return ((capabilities_.is_initialized() && capabilities_->relayoutputs_.is_initialized()) ? *capabilities_->relayoutputs_ : 0); }
  inline int GetNumSerialPorts() const { return ((capabilities_.is_initialized() && capabilities_->serialports_.is_initialized()) ? *capabilities_->serialports_ : 0); }
  inline int GetNumDigitalInputs() const { return ((capabilities_.is_initialized() && capabilities_->digitalinputs_.is_initialized()) ? *capabilities_->digitalinputs_ : 0); }
  inline bool GetDigitalInputOptions() const { return ((capabilities_.is_initialized() && capabilities_->digitalinputoptions_.is_initialized()) ? *capabilities_->digitalinputoptions_ : false); }

  boost::optional<Capabilities> capabilities_;

};

class SetAudioOutputConfigurationResponse : public Response<DeviceIOClient>
{
 public:

  SetAudioOutputConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const AudioOutputConfiguration& configuration, bool forcepersistence);
  virtual ~SetAudioOutputConfigurationResponse();

  AudioOutputConfiguration configuration_;
  bool forcepersistence_;

};

class SetAudioSourceConfigurationResponse : public Response<DeviceIOClient>
{
 public:

  SetAudioSourceConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const AudioSourceConfiguration& configuration, bool forcepersistence);
  virtual ~SetAudioSourceConfigurationResponse();

  AudioSourceConfiguration configuration_;
  bool forcepersistence_;

};

class SetDigitalInputConfigurationsResponse : public Response<DeviceIOClient>
{
 public:

  SetDigitalInputConfigurationsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<DigitalInput>& digitalinputs);
  virtual ~SetDigitalInputConfigurationsResponse();

  std::vector<DigitalInput> digitalinputs_;

};

class SetRelayOutputSettingsResponse : public Response<DeviceIOClient>
{
 public:

  SetRelayOutputSettingsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const RelayOutput& relayoutput);
  virtual ~SetRelayOutputSettingsResponse();

  RelayOutput relayoutput_;

};

class SetRelayOutputStateResponse : public Response<DeviceIOClient>
{
 public:

  SetRelayOutputStateResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string relayoutputtoken, RELAYLOGICALSTATE logicalstate);
  virtual ~SetRelayOutputStateResponse();

  std::string relayoutputtoken_;
  RELAYLOGICALSTATE logicalstate_;

};

class SetSerialPortConfigurationResponse : public Response<DeviceIOClient>
{
 public:

  SetSerialPortConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const SerialPortConfiguration& serialportconfiguration, bool forcepersistence);
  virtual ~SetSerialPortConfigurationResponse();

  SerialPortConfiguration serialportconfiguration_;
  bool forcepersistence_;

};

class DeviceIOClient : public Client<DEVICEIOOPERATION>, public boost::enable_shared_from_this<DeviceIOClient>
{
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioOutputConfigurationResponse, std::string>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioOutputConfigurationOptionsResponse, std::string>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioOutputsResponse>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioSourceConfigurationResponse, std::string>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioSourceConfigurationOptionsResponse, std::string>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetAudioSourcesResponse>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetDigitalInputConfigurationOptionsResponse, std::string>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetDigitalInputsResponse>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetRelayOutputsResponse>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetRelayOutputOptionsResponse, std::string>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetSerialPortConfigurationResponse, std::string>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetSerialPortConfigurationOptionsResponse, std::string>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetSerialPortsResponse>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, GetServiceCapabilitiesResponse>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, SetAudioOutputConfigurationResponse, AudioOutputConfiguration, bool>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, SetAudioSourceConfigurationResponse, AudioSourceConfiguration, bool>;
  friend Signal< DEVICEIOOPERATION, DeviceIOClient, SetDigitalInputConfigurationsResponse, std::vector<DigitalInput> >;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, SetRelayOutputSettingsResponse, RelayOutput>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, SetRelayOutputStateResponse, std::string, RELAYLOGICALSTATE>;
  friend Signal<DEVICEIOOPERATION, DeviceIOClient, SetSerialPortConfigurationResponse, SerialPortConfiguration, bool>;

 public:
  
  using Client::Update;

  DeviceIOClient(const boost::shared_ptr<std::recursive_mutex>& mutex);
  virtual ~DeviceIOClient();

  virtual int Init(const sock::ProxyParams& proxyparams, const std::string& address, const std::string& username, const std::string& password, const unsigned int maxconcurrentrequests, const bool forcehttpauthentication, const bool forbidreuse) override;
  virtual void Destroy() override;

  // Requests
  void GetAudioOutputConfiguration(const std::string& audiooutputtoken);
  void GetAudioOutputConfigurationOptions(const std::string& audiooutputtoken);
  void GetAudioOutputs();
  void GetAudioSourceConfiguration(const std::string& audiooutputtoken);
  void GetAudioSourceConfigurationOptions(const std::string& audiooutputtoken);
  void GetAudioSources();
  void GetDigitalInputConfigurationOptions(const std::string& token);
  void GetDigitalInputs();
  void GetRelayOutputOptions(const std::string& relayoutputtoken);
  void GetRelayOutputs();
  void GetSerialPortConfigurationOptions(const std::string& serialporttoken);
  void GetSerialPortConfiguration(const std::string& serialporttoken);
  void GetSerialPorts();
  void GetServiceCapabilities();
  void SetAudioOutputConfiguration(const AudioOutputConfiguration& configuration, bool forcepersistence);
  void SetAudioSourceConfiguration(const AudioSourceConfiguration& configuration, bool forcepersistence);
  void SetDigitalInputConfigurations(const std::vector<DigitalInput>& digitalinputs);
  void SetRelayOutputSettings(const RelayOutput& relayoutput);
  void SetRelayOutputState(const std::string& relayoutputtoken, RELAYLOGICALSTATE logicalstate);
  void SetSerialPortConfiguration(const SerialPortConfiguration& serialportconfiguration, bool forcepersistence);
  
  // Callback requests
  Connection GetAudioOutputConfigurationCallback(const std::string& audiooutputtoken, boost::function<void(const GetAudioOutputConfigurationResponse&)> callback);
  Connection GetAudioOutputConfigurationOptionsCallback(const std::string& audiooutputtoken, boost::function<void(const GetAudioOutputConfigurationOptionsResponse&)> callback);
  Connection GetAudioOutputsCallback(boost::function<void(const GetAudioOutputsResponse&)> callback);
  Connection GetAudioSourceConfigurationCallback(const std::string& audiooutputtoken, boost::function<void(const GetAudioSourceConfigurationResponse&)> callback);
  Connection GetAudioSourceConfigurationOptionsCallback(const std::string& audiooutputtoken, boost::function<void(const GetAudioSourceConfigurationOptionsResponse&)> callback);
  Connection GetAudioSourcesCallback(boost::function<void(const GetAudioSourcesResponse&)> callback);
  Connection GetDigitalInputConfigurationOptionsCallback(const std::string& token, boost::function<void(const GetDigitalInputConfigurationOptionsResponse&)> callback);
  Connection GetDigitalInputsCallback(boost::function<void(const GetDigitalInputsResponse&)> callback);
  Connection GetRelayOutputsCallback(boost::function<void(const GetRelayOutputsResponse&)> callback);
  Connection GetRelayOutputOptionsCallback(const std::string& relayoutputtoken, boost::function<void(const GetRelayOutputOptionsResponse&)> callback);
  Connection GetSerialPortConfigurationOptionsCallback(const std::string& serialporttoken, boost::function<void(const GetSerialPortConfigurationOptionsResponse&)> callback);
  Connection GetSerialPortConfigurationCallback(const std::string& serialporttoken, boost::function<void(const GetSerialPortConfigurationResponse&)> callback);
  Connection GetSerialPortsCallback(boost::function<void(const GetSerialPortsResponse&)> callback);
  Connection GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback);
  Connection SetAudioOutputConfigurationCallback(const AudioOutputConfiguration& configuration, bool forcepersistence, boost::function<void(const SetAudioOutputConfigurationResponse&)> callback);
  Connection SetAudioSourceConfigurationCallback(const AudioSourceConfiguration& configuration, bool forcepersistence, boost::function<void(const SetAudioSourceConfigurationResponse&)> callback);
  Connection SetDigitalInputConfigurationsCallback(const std::vector<DigitalInput>& digitalinputs, boost::function<void(const SetDigitalInputConfigurationsResponse&)> callback);
  Connection SetRelayOutputSettingsCallback(const RelayOutput& relayoutput, boost::function<void(const SetRelayOutputSettingsResponse&)> callback);
  Connection SetRelayOutputStateCallback(const std::string& relayoutputtoken, RELAYLOGICALSTATE logicalstate, boost::function<void(const SetRelayOutputStateResponse&)> callback);
  Connection SetSerialPortConfigurationCallback(const SerialPortConfiguration& serialportconfiguration, bool forcepersistence, boost::function<void(const SetSerialPortConfigurationResponse&)> callback);

  // Future requests
  boost::unique_future<GetAudioOutputConfigurationResponse> GetAudioOutputConfigurationFuture(const std::string& audiooutputtoken);
  boost::unique_future<GetAudioOutputConfigurationOptionsResponse> GetAudioOutputConfigurationOptionsFuture(const std::string& audiooutputtoken);
  boost::unique_future<GetAudioOutputsResponse> GetAudioOutputsFuture();
  boost::unique_future<GetAudioSourceConfigurationResponse> GetAudioSourceConfigurationFuture(const std::string& audiooutputtoken);
  boost::unique_future<GetAudioSourceConfigurationOptionsResponse> GetAudioSourceConfigurationOptionsFuture(const std::string& audiooutputtoken);
  boost::unique_future<GetAudioSourcesResponse> GetAudioSourcesFuture();
  boost::unique_future<GetDigitalInputConfigurationOptionsResponse> GetDigitalInputConfigurationOptionsFuture(const std::string& token);
  boost::unique_future<GetDigitalInputsResponse> GetDigitalInputsFuture();
  boost::unique_future<GetRelayOutputsResponse> GetRelayOutputsFuture();
  boost::unique_future<GetRelayOutputOptionsResponse> GetRelayOutputOptionsFuture(const std::string& relayoutputtoken);
  boost::unique_future<GetSerialPortConfigurationResponse> GetSerialPortConfigurationFuture(const std::string& serialporttoken);
  boost::unique_future<GetSerialPortConfigurationOptionsResponse> GetSerialPortConfigurationOptionsFuture(const std::string& serialporttoken);
  boost::unique_future<GetSerialPortsResponse> GetSerialPortsFuture();
  boost::unique_future<GetServiceCapabilitiesResponse> GetServiceCapabilitiesFuture();
  boost::unique_future<SetAudioOutputConfigurationResponse> SetAudioOutputConfigurationFuture(const AudioOutputConfiguration& configuration, bool forcepersistence);
  boost::unique_future<SetAudioSourceConfigurationResponse> SetAudioSourceConfigurationFuture(const AudioSourceConfiguration& configuration, bool forcepersistence);
  boost::unique_future<SetDigitalInputConfigurationsResponse> SetDigitalInputConfigurationsFuture(const std::vector<DigitalInput>& digitalinputs);
  boost::unique_future<SetRelayOutputSettingsResponse> SetRelayOutputSettingsFuture(const RelayOutput& relayoutput);
  boost::unique_future<SetRelayOutputStateResponse> SetRelayOutputStateFuture(const std::string& relayoutputtoken, RELAYLOGICALSTATE logicalstate);
  boost::unique_future<SetSerialPortConfigurationResponse> SetSerialPortConfigurationFuture(const SerialPortConfiguration& serialportconfiguration, bool forcepersistence);

  // Boost Signals2
  boost::signals2::signal<void(const GetAudioOutputConfigurationResponse&)>& GetAudioOutputConfigurationSignal();
  boost::signals2::signal<void(const GetAudioOutputConfigurationOptionsResponse&)>& GetAudioOutputConfigurationOptionsSignal();
  boost::signals2::signal<void(const GetAudioOutputsResponse&)>& GetAudioOutputsSignal();
  boost::signals2::signal<void(const GetAudioSourceConfigurationResponse&)>& GetAudioSourceConfigurationSignal();
  boost::signals2::signal<void(const GetAudioSourceConfigurationOptionsResponse&)>& GetAudioSourceConfigurationOptionsSignal();
  boost::signals2::signal<void(const GetAudioSourcesResponse&)>& GetAudioSourcesSignal();
  boost::signals2::signal<void(const GetDigitalInputConfigurationOptionsResponse&)>& GetDigitalInputConfigurationOptionsSignal();
  boost::signals2::signal<void(const GetDigitalInputsResponse&)>& GetDigitalInputsSignal();
  boost::signals2::signal<void(const GetRelayOutputsResponse&)>& GetRelayOutputsSignal();
  boost::signals2::signal<void(const GetRelayOutputOptionsResponse&)>& GetRelayOutputOptionsSignal();
  boost::signals2::signal<void(const GetSerialPortConfigurationResponse&)>& GetSerialPortConfigurationSignal();
  boost::signals2::signal<void(const GetSerialPortConfigurationOptionsResponse&)>& GetSerialPortConfigurationOptionsSignal();
  boost::signals2::signal<void(const GetSerialPortsResponse&)>& GetSerialPortsSignal();
  boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& GetServiceCapabilitiesSignal();
  boost::signals2::signal<void(const SetAudioOutputConfigurationResponse&)>& SetAudioOutputConfigurationSignal();
  boost::signals2::signal<void(const SetAudioSourceConfigurationResponse&)>& SetAudioSourceConfigurationSignal();
  boost::signals2::signal<void(const SetDigitalInputConfigurationsResponse&)>& SetDigitalInputConfigurationsSignal();
  boost::signals2::signal<void(const SetRelayOutputSettingsResponse&)>& SetRelayOutputSettingsSignal();
  boost::signals2::signal<void(const SetRelayOutputStateResponse&)>& SetRelayOutputStateSignal();
  boost::signals2::signal<void(const SetSerialPortConfigurationResponse&)>& SetSerialPortConfigurationSignal();

 private:

  virtual void Update(DEVICEIOOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata) override;
  virtual void SignalError(DEVICEIOOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) override;

  // Get the body of the request string
  std::string GetAudioOutputConfigurationBody(const std::string& audiooutputtoken);
  std::string GetAudioOutputConfigurationOptionsBody(const std::string& audiooutputtoken);
  std::string GetAudioOutputsBody();
  std::string GetAudioSourceConfigurationBody(const std::string& audiooutputtoken);
  std::string GetAudioSourceConfigurationOptionsBody(const std::string& audiooutputtoken);
  std::string GetAudioSourcesBody();
  std::string GetDigitalInputConfigurationOptionsBody(const std::string& token);
  std::string GetDigitalInputsBody();
  std::string GetRelayOutputsBody();
  std::string GetRelayOutputOptionsBody(const std::string& relayoutputtoken);
  std::string GetSerialPortConfigurationBody(const std::string& serialportconfiguration);
  std::string GetSerialPortConfigurationOptionsBody(const std::string& serialportconfiguration);
  std::string GetSerialPortsBody();
  std::string GetServiceCapabilitiesBody();
  std::string SetAudioOutputConfigurationBody(const AudioOutputConfiguration& configuration, bool forcepersistence);
  std::string SetAudioSourceConfigurationBody(const AudioSourceConfiguration& configuration, bool forcepersistence);
  std::string SetDigitalInputConfigurationsBody(const std::vector<DigitalInput>& digitalinputs);
  std::string SetRelayOutputSettingsBody(const RelayOutput& relayoutput);
  std::string SetRelayOutputStateBody(const std::string& relayoutputtoken, RELAYLOGICALSTATE logicalstate);
  std::string SetSerialPortConfigurationBody(const SerialPortConfiguration& serialportconfiguration, bool forcepersistence);

  DeviceIOSignals* signals_;

};

}

}

#endif
