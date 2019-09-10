// rtspconnectiontest.hpp
//

#ifndef IDQKSSAQQDWGFDFMCVHSHPFYTG4JHLAXD0
#define IDQKSSAQQDWGFDFMCVHSHPFYTG4JHLAXD0

///// Includes /////

#include <rtsp/server/userconnection.hpp>

///// Namespaces /////

namespace rtsp
{

namespace server
{

namespace tests
{

///// Classes /////

class RtspConnectionTest : public UserConnection
{
 public:

  RtspConnectionTest();
  ~RtspConnectionTest();

  void Connected();

};

}

}

}

#endif
