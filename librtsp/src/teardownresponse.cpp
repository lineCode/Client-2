// teardownresponse.cpp
//

///// Includes /////

#include "rtsp/teardownresponse.hpp"

///// Namespaces /////

namespace rtsp
{

///// Methods /////

TeardownResponse::TeardownResponse()
{

}

TeardownResponse::TeardownResponse(uint64_t latency) :
  Response(latency)
{

}

TeardownResponse::~TeardownResponse()
{

}

}
