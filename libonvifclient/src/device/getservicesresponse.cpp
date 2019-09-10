// getservicesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetServicesResponse::GetServicesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool includecapability) :
  Response(client, localendpoint, latency, message),
  includecapability_(includecapability)
{

}

GetServicesResponse::GetServicesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool includecapability, const std::vector<Service>& services) :
  Response(client, localendpoint, latency, message),
  includecapability_(includecapability),
  services_(services)
{

}

GetServicesResponse::~GetServicesResponse()
{

}

boost::optional<Service> GetServicesResponse::GetService(const std::string& xaddr) const
{
  for (const auto& service : services_)
  {
    if (!service.xaddr_.is_initialized())
    {

      continue;
    }

    if (boost::algorithm::ends_with(*service.xaddr_, xaddr))
    {

      return service;
    }
  }

  return boost::none;
}

}

}
