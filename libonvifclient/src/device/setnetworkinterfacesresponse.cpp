// setnetworkinterfacesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetNetworkInterfacesResponse::SetNetworkInterfacesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken, const onvif::NetworkInterfaceSetConfiguration& networkinterface) :
  Response(client, localendpoint, latency, message),
  interfacetoken_(interfacetoken),
  networkinterface_(networkinterface)
{

}

SetNetworkInterfacesResponse::SetNetworkInterfacesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken, const onvif::NetworkInterfaceSetConfiguration& networkinterface, const boost::optional<bool>& rebootneeded) :
  Response(client, localendpoint, latency, message),
  interfacetoken_(interfacetoken),
  networkinterface_(networkinterface),
  rebootneeded_(rebootneeded)
{

}

SetNetworkInterfacesResponse::~SetNetworkInterfacesResponse()
{

}

}

}
