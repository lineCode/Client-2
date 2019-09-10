// interleavedrtpclient.hpp
//

#ifndef IDX4V6021VIBI8O24NLWGS3L0VY686SHTH
#define IDX4V6021VIBI8O24NLWGS3L0VY686SHTH

///// Includes /////

#include "rtpclient.hpp"

#include <stdint.h>

///// Namespaces /////

namespace rtsp
{

///// Classes /////

template<class T>
class InterleavedRtpClient : public RtpClient<T>
{
 public:

  InterleavedRtpClient(uint32_t localssrc, double rtptimestampfrequency, RtpCallback rtpcallback, const boost::shared_ptr<T>& rtpcallbackdataobject, void* rtpcallbackdata, RtcpCallback rtcpcallback, uint8_t rtpchannel, uint8_t rtcpchannel) :
    RtpClient<T>(localssrc, rtptimestampfrequency, rtpcallback, rtpcallbackdataobject, rtpcallbackdata, rtcpcallback),
    rtpchannel_(rtpchannel),
    rtcpchannel_(rtcpchannel)
  {

  }

  ~InterleavedRtpClient()
  {

  }

  inline void SetRtpChannel(uint8_t rtpchannel) { rtpchannel_ = rtpchannel; }
  inline uint8_t GetRtpChannel() const { return rtpchannel_; }

  inline void SetRtcpChannel(uint8_t rtcpchannel) { rtcpchannel_ = rtcpchannel; }
  inline uint8_t GetRtcpChannel() const { return rtcpchannel_; }

 private:

  uint8_t rtpchannel_;
  uint8_t rtcpchannel_;

};

}

#endif
