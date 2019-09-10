// pauseresponse.hpp
//

#ifndef IDVLUX4CNTZ3FMXH22SOM0T8AJIQ90L8Y3
#define IDVLUX4CNTZ3FMXH22SOM0T8AJIQ90L8Y3

///// Includes /////

#include <string>

#include "response.hpp"

///// Namespaces /////

namespace rtsp
{

///// Classes /////

class PauseResponse : public Response
{
 public:

  PauseResponse();
  PauseResponse(uint64_t latency);
  PauseResponse(uint64_t latency, const std::string& session);
  ~PauseResponse();

  std::string session_;

};

}

#endif
