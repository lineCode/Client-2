// dummydeviceioservice.hpp
//

#ifndef IDGRZA0EWRT56A68YPEORVJ0T4YFMJ2IV4
#define IDGRZA0EWRT56A68YPEORVJ0T4YFMJ2IV4

///// Includes /////

#include <onvifserver/onvifserver.hpp>
#include <onviftypes/onviftypes.hpp>
#include <vector>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class DummyDeviceIOService : public server::Service
{
 public:

  DummyDeviceIOService();
  virtual ~DummyDeviceIOService();

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const override { return false; }
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& path, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientcnonce, const std::string& clientresponse, const std::string& clientqop) const override { return false; }

  virtual boost::shared_future<server::ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) override;

  inline void SetGetAudioOutputConfiguration(const deviceio::AudioOutputConfiguration& getaudiooutputconfiguration) { getaudiooutputconfiguration_ = getaudiooutputconfiguration; }
  inline void SetGetAudioOutputConfigurationOptions(const AudioOutputConfigurationOptions& getaudiooutputconfigurationoptions) { getaudiooutputconfigurationoptions_ = getaudiooutputconfigurationoptions; }
  inline void SetGetAudioOutputs(const std::vector<std::string>& getaudiooutputs) { getaudiooutputs_ = getaudiooutputs; }
  inline void SetGetAudioSourceConfiguration(const AudioSourceConfiguration& getaudiosourceconfiguration) { getaudiosourceconfiguration_ = getaudiosourceconfiguration; }
  inline void SetGetAudioSourceConfigurationOptions(const AudioSourceConfigurationOptions& getaudiosourceconfigurationoptions) { getaudiosourceconfigurationoptions_ = getaudiosourceconfigurationoptions; }
  inline void SetGetAudioSources(const std::vector<std::string>& getaudiosources) { getaudiosources_ = getaudiosources; }
  inline void SetGetRelayOutputOptions(const std::vector<deviceio::RelayOutputOptions>& getrelayoutputoptions) { getrelayoutputoptions_ = getrelayoutputoptions; }
  inline void SetGetRelayOutputs(const std::vector<RelayOutput>& getrelayoutputs) { getrelayoutputs_ = getrelayoutputs; }
  inline void SetGetDigitalInputConfigurationOptions(const std::vector<DIGITALIDLESTATE>& getdigitalinputconfigurationoptions) { getdigitalinputconfigurationoptions_ = getdigitalinputconfigurationoptions;  }
  inline void SetGetDigitalInputs(const std::vector<DigitalInput>& getdigitalinputs) { getdigitalinputs_ = getdigitalinputs; }
  inline void SetGetSerialPortConfiguration(const deviceio::SerialPortConfiguration& getserialportconfiguration) { getserialportconfiguration_ = getserialportconfiguration; }
  inline void SetGetSerialPortConfigurationOptions(const deviceio::SerialPortConfigurationOptions& getserialportconfigurationoptions) { getserialportconfigurationoptions_ = getserialportconfigurationoptions; }
  inline void SetGetSerialPorts(const std::vector<deviceio::SerialPort>& getserialports) { getserialports_ = getserialports; }
  inline void SetGetServiceCapabilities(const deviceio::Capabilities& getservicecapabilities) { getservicecapabilities_ = getservicecapabilities; }

 private:

  deviceio::AudioOutputConfiguration getaudiooutputconfiguration_;
  AudioOutputConfigurationOptions getaudiooutputconfigurationoptions_;
  std::vector<std::string> getaudiooutputs_;
  AudioSourceConfiguration getaudiosourceconfiguration_;
  AudioSourceConfigurationOptions getaudiosourceconfigurationoptions_;
  std::vector<std::string> getaudiosources_;
  std::vector<deviceio::RelayOutputOptions> getrelayoutputoptions_;
  std::vector<RelayOutput> getrelayoutputs_;
  std::vector<DIGITALIDLESTATE> getdigitalinputconfigurationoptions_;
  std::vector<DigitalInput> getdigitalinputs_;
  deviceio::SerialPortConfiguration getserialportconfiguration_;
  deviceio::SerialPortConfigurationOptions getserialportconfigurationoptions_;
  std::vector<deviceio::SerialPort> getserialports_;
  deviceio::Capabilities getservicecapabilities_;

};

}

}

#endif
