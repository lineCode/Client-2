// setconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/ptzclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace ptz
{

///// Methods /////

SetConfigurationResponse::SetConfigurationResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const PTZConfiguration& ptzconfiguration, bool forcepersistence) :
  Response(client, localendpoint, latency, message),
  ptzconfiguration_(ptzconfiguration),
  forcepersistence_(forcepersistence)
{

}

SetConfigurationResponse::~SetConfigurationResponse()
{

}

}

}
