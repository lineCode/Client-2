// dummyimagingservice.hpp
//

///// Includes /////

#include "dummyimagingservice.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Methods /////

DummyImagingService::DummyImagingService()
{
  
}

DummyImagingService::~DummyImagingService()
{

}

boost::shared_future<server::ServerResponse> DummyImagingService::Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata)
{
  pugi::xml_document doc;
  if (!doc.load_buffer(content.data(), content.size()))
  {

    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  std::string response;
  if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetImagingSettings']"))
  {
    response = std::string("<media:GetImagingSettingsResponse>") + getimagingsettings_.ToXml("tt:ImagingSettings") + std::string("</media:GetImagingSettingsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetOptions']"))
  {
    response = std::string("<media:GetOptionsResponse>") + getoptions_.ToXml("tt:ImagingOptions") + std::string("</media:GetOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilities']"))
  {
    response = std::string("<media:GetServiceCapabilitiesResponse>") + getservicecapabilities_.ToXml("tt:Capabilities") + std::string("</media:GetServiceCapabilitiesResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetImagingSettings']"))
  {
    response = std::string("<media:SetImagingSettingsResponse/>");

  }
  else
  {
  
    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::OK, std::string("<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:media=\"http://www.onvif.org/ver10/media/wsdl\"><SOAP-ENV:Body>") + response + std::string("</SOAP-ENV:Body></SOAP-ENV:Envelope>")));
}

}

}
