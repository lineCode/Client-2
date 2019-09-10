// livestreamcallback.h
//

#ifndef IDC3NZR9FMR8CA5N3DQY6OACQ1XV5293NS
#define IDC3NZR9FMR8CA5N3DQY6OACQ1XV5293NS

///// Includes /////

#include <rtsp/rtp/packet.hpp>
#include <stdint.h>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Classes /////
  
class LiveStreamCallback
{
 public:

  virtual ~LiveStreamCallback()
  {
  
  }

  virtual void Frame(const rtsp::rtp::Packet& rtppacket, const uint8_t* payload, unsigned int payloadsize) = 0;

};

}

QT_END_NAMESPACE

#endif
