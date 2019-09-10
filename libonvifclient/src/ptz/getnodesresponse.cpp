// getnodesresponse.cpp
//

///// Includes /////

#include "onvifclient/ptzclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace ptz
{

///// Methods /////

GetNodesResponse::GetNodesResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetNodesResponse::GetNodesResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<PTZNode>& ptznode) :
  Response(client, localendpoint, latency, message),
  ptznode_(ptznode)
{

}

GetNodesResponse::~GetNodesResponse()
{

}

}

}
