// setparameterresponse.cpp
//

///// Includes /////

#include "rtsp/setparameterresponse.hpp"

///// Namespaces /////

namespace rtsp
{

///// Methods /////

SetParameterResponse::SetParameterResponse()
{

}

SetParameterResponse::SetParameterResponse(uint64_t latency) :
  Response(latency)
{

}

SetParameterResponse::SetParameterResponse(uint64_t latency, const std::vector<headers::Parameter>& parameters) :
  Response(latency),
  parameters_(parameters)
{

}

SetParameterResponse::~SetParameterResponse()
{

}

}
