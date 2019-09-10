// optionsresponse.cpp
//

///// Includes /////

#include "rtsp/optionsresponse.hpp"

///// Namespaces /////

namespace rtsp
{

///// Methods /////

OptionsResponse::OptionsResponse()
{

}

OptionsResponse::OptionsResponse(uint64_t latency) :
  Response(latency)
{

}

OptionsResponse::OptionsResponse(uint64_t latency, const boost::container::flat_set<headers::REQUESTTYPE>& options) :
  Response(latency),
  options_(options)
{

}

OptionsResponse::~OptionsResponse()
{

}

}
