// setsystemdateandtimeresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetSystemDateAndTimeResponse::SetSystemDateAndTimeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

SetSystemDateAndTimeResponse::SetSystemDateAndTimeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, DATETIMETYPE datetimetype, bool daylightsavings, const boost::optional<TimeZone>& timezone, const boost::optional<DateTime>& utcdatetime) :
  Response(client, localendpoint, latency, message),
  datetimetype_(datetimetype),
  daylightsavings_(daylightsavings),
  timezone_(timezone),
  utcdatetime_(utcdatetime)
{

}

SetSystemDateAndTimeResponse::~SetSystemDateAndTimeResponse()
{

}

}

}
