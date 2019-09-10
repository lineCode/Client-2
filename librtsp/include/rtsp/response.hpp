// response.hpp
//

#ifndef IDO0Q0TNP8DCGXO11SUKC5GH0ENEK88AC1
#define IDO0Q0TNP8DCGXO11SUKC5GH0ENEK88AC1

///// Includes /////

#include <memory>
#include <stdint.h>

///// Namespaces /////

namespace rtsp
{

///// Classes /////

class Response
{
 public:

  Response() :
    latency_(0)
  {

  }

  Response(uint64_t latency) :
    latency_(latency)
  {

  }

  virtual ~Response()
  {

  }

  uint64_t latency_;

};

}

#endif
