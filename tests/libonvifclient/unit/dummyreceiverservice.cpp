// dummyreceiverservice.hpp
//

///// Includes /////

#include "dummyreceiverservice.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Methods /////

DummyReceiverService::DummyReceiverService()
{
  
}

DummyReceiverService::~DummyReceiverService()
{

}

boost::shared_future<server::ServerResponse> DummyReceiverService::Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata)
{
  pugi::xml_document doc;
  if (!doc.load_buffer(content.data(), content.size()))
  {

    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  std::string response;
  if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='ConfigureReceiver']"))
  {
    response = std::string("<trv:ConfigureReceiverResponse><trv:ReceiverToken>") + configurereceiverreceivertoken_ + std::string("</trv:ReceiverToken>") + configurereceiverconfiguration_.ToXml("trv:Configuration") + std::string("</trv:ConfigureReceiverResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateReceiver']"))
  {
    response = std::string("<trv:CreateReceiverResponse>") + createreceiver_.ToXml("trv:Receiver") + std::string("</trv:CreateReceiverResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteReceiver']"))
  {
    response = std::string("<trv:DeleteReceiverResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetReceiver']"))
  {
    response = std::string("<trv:GetReceiverResponse>") + getreceiver_.ToXml("trv:Receiver") + std::string("</trv:GetReceiverResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetReceivers']"))
  {
    std::string getreceivers;
    for (const auto& getreceiver : getreceivers_)
    {
      getreceivers += getreceiver.ToXml("trv:Receivers");

    }

    response = std::string("<trv:GetReceiversResponse>") + getreceivers + std::string("</trv:GetReceiversResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetReceiverState']"))
  {
    response = std::string("<trv:GetReceiverStateResponse>") + getreceiverstate_.ToXml("trv:ReceiverState") + std::string("</trv:GetReceiverStateResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilities']"))
  {
    response = std::string("<trv:GetServiceCapabilitiesResponse>") + getservicecapabilities_.ToXml("tt:Capabilities") + std::string("</trv:GetServiceCapabilitiesResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetReceiverMode']"))
  {
    response = std::string("<trv:SetReceiverModeResponse/>");

  }
  else
  {
  
    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::OK, std::string("<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:trv=\"http://www.onvif.org/ver10/recording/wsdl\"><SOAP-ENV:Body>") + response + std::string("</SOAP-ENV:Body></SOAP-ENV:Envelope>")));
}

}

}
