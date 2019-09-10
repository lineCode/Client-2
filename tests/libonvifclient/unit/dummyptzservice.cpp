// dummyptzservice.hpp
//

///// Includes /////

#include "dummyptzservice.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Methods /////

DummyPTZService::DummyPTZService()
{
  
}

DummyPTZService::~DummyPTZService()
{

}

boost::shared_future<server::ServerResponse> DummyPTZService::Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata)
{
  pugi::xml_document doc;
  if (!doc.load_buffer(content.data(), content.size()))
  {

    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  std::string response;
  if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='ContinuousMove']"))
  {
    response = std::string("<ptz:ContinuousMoveResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleConfigurations']"))
  {
    std::string compatibleconfigurations;
    for (const auto& compatibleconfiguration : getcompatibleconfigurations_)
    {
      compatibleconfigurations += compatibleconfiguration.ToXml("media:PTZConfiguration");

    }

    response = std::string("<media:GetCompatibleConfigurationsResponse>") + compatibleconfigurations + std::string("</media:GetCompatibleConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetConfiguration']"))
  {
    response = std::string("<ptz:GetConfigurationResponse>") + getconfiguration_.ToXml("tt:PTZConfiguration") + std::string("</ptz:GetConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetConfigurations']"))
  {
    std::string getconfigurations;
    for (const auto& getconfiguration : getconfigurations_)
    {
      getconfigurations += getconfiguration.ToXml("tt:PTZConfiguration");

    }

    response = std::string("<ptz:GetConfigurationsResponse>") + getconfigurations + std::string("</ptz:GetConfigurationsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetNodes']"))
  {
    std::string getnodes;
    for (const auto& getnode : getnodes_)
    {
      getnodes += getnode.ToXml("tt:PTZNode");

    }

    response = std::string("<ptz:GetNodesResponse>") + getnodes + std::string("</ptz:GetNodesResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilities']"))
  {
    response = std::string("<ptz:GetServiceCapabilitiesResponse>") + getservicecapabilities_.ToXml("tt:Capabilities") + std::string("</ptz:GetServiceCapabilitiesResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetConfiguration']"))
  {
    response = std::string("<ptz:SetConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='Stop']"))
  {
    response = std::string("<ptz:StopResponse/>");

  }
  else
  {
    
    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::OK, std::string("<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:ptz=\"http://www.onvif.org/ver20/ptz/wsdl\"><SOAP-ENV:Body>") + response + std::string("</SOAP-ENV:Body></SOAP-ENV:Envelope>")));
}

}

}
