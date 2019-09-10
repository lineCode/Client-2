// rtspconnection.hpp
//

#ifndef IDRPTMDSOXZ4LE0H1P695N78XC7EAVQAG2
#define IDRPTMDSOXZ4LE0H1P695N78XC7EAVQAG2

///// Includes /////

#include <boost/asio.hpp>

#include <array>
#include <memory>
#include <mutex>
#include <string>

#include "interleaved.hpp"
#include "rtsp/rtspresponse.hpp"
#include "session.hpp"

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Declarations /////

class Session;
class UserConnection;

///// Classes /////

class RTSPConnection : public Connection
{
 public:

  RTSPConnection(boost::asio::io_service& io, const boost::shared_ptr<Server>& server, const boost::shared_ptr<UserConnection>& userconnection);
  virtual ~RTSPConnection();

  inline virtual CONNECTIONTYPE GetConnectionType() const override { return CONNECTION_RTSP; }

};

}

}

#endif
