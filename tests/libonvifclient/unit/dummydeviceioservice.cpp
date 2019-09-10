// dummydeviceioservice.hpp
//

///// Includes /////

#include "dummydeviceioservice.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Methods /////

DummyDeviceIOService::DummyDeviceIOService()
{
  
}

DummyDeviceIOService::~DummyDeviceIOService()
{

}

boost::shared_future<server::ServerResponse> DummyDeviceIOService::Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata)
{
  pugi::xml_document doc;
  if (!doc.load_buffer(content.data(), content.size()))
  {

    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  std::string response;
  if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioOutputConfiguration']"))
  {
    response = std::string("<deviceio:GetAudioOutputConfigurationResponse>") + getaudiooutputconfiguration_.ToXml("tt:AudioOutputConfiguration") + std::string("</deviceio:GetAudioOutputConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioOutputConfigurationOptions']"))
  {
    response = std::string("<deviceio:GetAudioOutputConfigurationOptionsResponse>") + getaudiooutputconfigurationoptions_.ToXml("tt:AudioOutputOptions") + std::string("</deviceio:GetAudioOutputConfigurationOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioOutputs']"))
  {
    response = std::string("<deviceio:GetAudioOutputsResponse>") + onvif::ToXml("tt:Token", getaudiooutputs_) + std::string("</deviceio:GetAudioOutputsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSourceConfiguration']"))
  {
    response = std::string("<deviceio:GetAudioSourceConfigurationResponse>") + getaudiosourceconfiguration_.ToXml("tt:AudioSourceConfiguration") + std::string("</deviceio:GetAudioSourceConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSourceConfigurationOptions']"))
  {
    response = std::string("<deviceio:GetAudioSourceConfigurationOptionsResponse>") + getaudiosourceconfigurationoptions_.ToXml("tt:AudioSourceOptions") + std::string("</deviceio:GetAudioSourceConfigurationOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSources']"))
  {
    response = std::string("<deviceio:GetAudioSourcesResponse>") + onvif::ToXml("tt:Token", getaudiosources_) + std::string("</deviceio:GetAudioSourcesResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDigitalInputConfigurationOptions']"))
  {
    std::string idlestates;
    for (const auto idlestate : getdigitalinputconfigurationoptions_)
    {
      idlestates += onvif::ToXml("tds:IdleState", boost::optional<std::string>(ToString(idlestate)));

    }

    response = std::string("<deviceio:GetDigitalInputConfigurationOptionsResponse>") + idlestates + std::string("</deviceio:GetDigitalInputConfigurationOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetDigitalInputs']"))
  {
    response = std::string("<deviceio:GetDigitalInputsResponse>") + onvif::ToXml("tt:DigitalInput", getdigitalinputs_) + std::string("</deviceio:GetDigitalInputsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRelayOutputOptions']"))
  {
    response = std::string("<deviceio:GetRelayOutputOptionsResponse>") + onvif::ToXml("tt:RelayOutputOptions", getrelayoutputoptions_) + std::string("</deviceio:GetRelayOutputOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRelayOutputs']"))
  {
    response = std::string("<deviceio:GetRelayOutputsResponse>") + onvif::ToXml("tt:RelayOutputs", getrelayoutputs_) + std::string("</deviceio:GetRelayOutputsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSerialPortConfiguration']"))
  {
    response = std::string("<deviceio:GetSerialPortConfigurationResponse>") + getserialportconfiguration_.ToXml("tt:SerialPortConfiguration") + std::string("</deviceio:GetSerialPortConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSerialPortConfigurationOptions']"))
  {
    response = std::string("<deviceio:GetSerialPortConfigurationOptionsResponse>") + getserialportconfigurationoptions_.ToXml("tt:SerialPortOptions") + std::string("</deviceio:GetSerialPortConfigurationOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSerialPorts']"))
  {
    response = std::string("<deviceio:GetSerialPortsResponse>") + onvif::ToXml("tt:SerialPort", getserialports_) + std::string("</deviceio:GetSerialPortsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilities']"))
  {
    response = std::string("<deviceio:GetServiceCapabilitiesResponse>") + getservicecapabilities_.ToXml("tt:Capabilities") + std::string("</deviceio:GetServiceCapabilitiesResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAudioOutputConfiguration']"))
  {
    response = std::string("<deviceio:SetAudioOutputConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAudioSourceConfiguration']"))
  {
    response = std::string("<deviceio:SetAudioSourceConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetDigitalInputConfigurations']"))
  {
    response = std::string("<deviceio:SetDigitalInputConfigurationsResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRelayOutputSettings']"))
  {
    response = std::string("<deviceio:SetRelayOutputSettingsResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRelayOutputState']"))
  {
    response = std::string("<deviceio:SetRelayOutputStateResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetSerialPortConfiguration']"))
  {
    response = std::string("<deviceio:SetSerialPortConfigurationResponse/>");

  }
  else
  {
  
    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::OK, std::string("<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:deviceio=\"http://www.onvif.org/ver10/media/wsdl\"><SOAP-ENV:Body>") + response + std::string("</SOAP-ENV:Body></SOAP-ENV:Envelope>")));
}

}

}
