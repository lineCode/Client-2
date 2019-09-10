// rtspserver.cpp
//

///// Includes /////

#include "rtspservertest.hpp"

#include <boost/make_shared.hpp>

#include "rtspconnectiontest.hpp"

///// Namespaces /////

namespace rtsp
{

namespace server
{

namespace tests
{

///// Methods /////

boost::shared_ptr<UserConnection> RtspServer::CreateConnection()
{
  return boost::make_shared<RtspConnectionTest>();
}

}

}

}
