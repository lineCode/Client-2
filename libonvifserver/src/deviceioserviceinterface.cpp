// deviceioserviceinterface.hpp
//

///// Includes /////

#include "onvifserver/deviceioserviceinterface.hpp"

#include "onvifserver/soapfault.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace deviceio
{

///// Globals /////

const std::string DEVICEIONAMESPACEURI("https://www.onvif.org/onvif/ver10/deviceio.wsdl");

const std::string GETSERVICECAPABILITIES("GetServiceCapabilities");

///// Declarations /////

const std::string DeviceIOServiceInterface::XADDR_("/onvif/deviceio");

///// Methods /////

DeviceIOServiceInterface::DeviceIOServiceInterface(const int onvifauthenticationtimediff) :
  OnvifService(2, 42, onvifauthenticationtimediff, { SOAP_NAMESPACE_ATTRIBUTE, SCHEMA_NAMESPACE_ATTRIBUTE, SCHEMA_INSTANCE_NAMESPACE_ATTRIBUTE, TYPES_NAMESPACE_ATTRIBUTE })
{

}

DeviceIOServiceInterface::~DeviceIOServiceInterface()
{
  Destroy();

}

int DeviceIOServiceInterface::Init()
{
  Destroy();
  return 0;
}

void DeviceIOServiceInterface::Destroy()
{

}

boost::shared_future<ServerResponse> DeviceIOServiceInterface::OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata)
{
  if (namespaceuri != DEVICEIONAMESPACEURI)
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NAMESPACE, std::string("Error parsing device namespace uri")).ToString()));
  }

  if (action == GETSERVICECAPABILITIES)
  {

    return boost::make_ready_future(GetServiceCapabilities());
  }
  else
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNACTION, std::string("Unknown Action: ") + action).ToString()));
  }
}

}

}

}
