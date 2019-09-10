// playresponse.cpp
//

///// Includes /////

#include "rtsp/playresponse.hpp"

///// Namespaces /////

namespace rtsp
{

///// Methods /////

PlayResponse::PlayResponse()
{

}

PlayResponse::PlayResponse(uint64_t latency) :
  Response(latency)
{

}

PlayResponse::~PlayResponse()
{

}

}
