// rtspconnection.cpp
//

///// Includes /////

#include "rtsp/server/rtspconnection.hpp"

#include <boost/bind.hpp>

#include "rtsp/server/userconnection.hpp"

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Methods /////

RTSPConnection::RTSPConnection(boost::asio::io_service& io, const boost::shared_ptr<Server>& server, const boost::shared_ptr<UserConnection>& userconnection) :
  Connection(io, server, userconnection)
{
  
}

RTSPConnection::~RTSPConnection()
{

}

}

}
