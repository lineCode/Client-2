// interleaved.hpp
//dddd  

#ifndef ID2X4KSRI8K793X79HKE83TLQR7X7LARRZ
#define ID2X4KSRI8K793X79HKE83TLQR7X7LARRZ

///// Includes /////

#include <boost/shared_ptr.hpp>

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Declarations /////

class Session;

///// Enumerations /////

enum INTERLEAVEDCHANNELTYPE
{
  INTERLEAVEDCHANNEL_NONE,
  INTERLEAVEDCHANNEL_RTP,
  INTERLEAVEDCHANNEL_RTCP
};

///// Structures /////

class InterleavedChannel
{
 public:

  InterleavedChannel();

  void Reset();

  boost::shared_ptr<Session> session_;
  INTERLEAVEDCHANNELTYPE type_;

};

}

}

#endif
