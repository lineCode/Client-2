// getnetworkinterfacesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetNetworkInterfacesResponse::GetNetworkInterfacesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetNetworkInterfacesResponse::GetNetworkInterfacesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<NetworkInterface>& networkinterfaces) :
  Response(client, localendpoint, latency, message),
  networkinterfaces_(networkinterfaces)
{

}

GetNetworkInterfacesResponse::~GetNetworkInterfacesResponse()
{

}

boost::optional<NetworkInterface> GetNetworkInterfacesResponse::GetNetworkInterface(const std::string& token) const
{
  for (const auto& networkinterface : networkinterfaces_)
  {
    if (!networkinterface.token_.is_initialized())
    {

      continue;
    }

    if (*networkinterface.token_ == token)
    {

      return networkinterface;
    }
  }

  return boost::none;
}

}

}
