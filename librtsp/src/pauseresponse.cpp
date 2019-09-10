// pauseresponse.cpp
//

///// Includes /////

#include "rtsp/pauseresponse.hpp"

///// Namespaces /////

namespace rtsp
{

///// Methods /////

PauseResponse::PauseResponse()
{

}

PauseResponse::PauseResponse(uint64_t latency) :
  Response(latency)
{

}

PauseResponse::PauseResponse(uint64_t latency, const std::string& session) :
  Response(latency),
  session_(session)
{

}

PauseResponse::~PauseResponse()
{

}

}
