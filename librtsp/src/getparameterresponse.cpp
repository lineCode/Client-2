// getparameterresponse.cpp
//

///// Includes /////

#include "rtsp/getparameterresponse.hpp"

///// Namespaces /////

namespace rtsp
{

///// Methods /////

GetParameterResponse::GetParameterResponse()
{

}

GetParameterResponse::GetParameterResponse(uint64_t latency) :
  Response(latency)
{

}

GetParameterResponse::GetParameterResponse(uint64_t latency, const std::vector<headers::Parameter>& parameters) :
  Response(latency),
  parameters_(parameters)
{

}

GetParameterResponse::~GetParameterResponse()
{

}

}
