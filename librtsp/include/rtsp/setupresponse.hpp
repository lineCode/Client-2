// setupresponse.hpp
//

#ifndef ID2TRAYCYE0ZUSYUA4UUGWPMQZVO9IORV6
#define ID2TRAYCYE0ZUSYUA4UUGWPMQZVO9IORV6

///// Includes /////

#include <string>

#include "response.hpp"

///// Namespaces /////

namespace rtsp
{

///// Classes /////

class SetupResponse : public Response
{
 public:

  SetupResponse();
  SetupResponse(uint64_t latency);
  SetupResponse(uint64_t latency, const std::string& session, int timeout);
  ~SetupResponse();

  std::string session_;
  int timeout_;

};

}

#endif
