// setupresponse.cpp
//

///// Includes /////

#include "rtsp/setupresponse.hpp"

///// Namespaces /////

namespace rtsp
{

///// Methods /////

SetupResponse::SetupResponse()
{

}

SetupResponse::SetupResponse(uint64_t latency) :
  Response(latency)
{

}

SetupResponse::SetupResponse(uint64_t latency, const std::string& session, int timeout) :
  Response(latency),
  session_(session),
  timeout_(timeout)
{

}

SetupResponse::~SetupResponse()
{

}

}
