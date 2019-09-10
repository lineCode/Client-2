// rtspserver.hpp
//

#ifndef IDEXQIR6XVWGCJ6NW5D9QWZHMWZ1YLEJEU
#define IDEXQIR6XVWGCJ6NW5D9QWZHMWZ1YLEJEU

///// Includes /////

#include <rtsp/server/server.hpp>

///// Namespaces /////

namespace rtsp
{

namespace server
{

namespace tests
{

///// Classes /////

class RtspServer : public Server
{
 public:

  virtual boost::shared_ptr<UserConnection> CreateConnection() override;

};

}

}

}

#endif
