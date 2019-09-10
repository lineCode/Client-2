// describeresponse.cpp
//

///// Includes /////

#include "rtsp/describeresponse.hpp"

///// Namespaces /////

namespace rtsp
{

///// Methods /////

DescribeResponse::DescribeResponse()
{

}


DescribeResponse::DescribeResponse(uint64_t latency) :
  Response(latency)
{

}

DescribeResponse::DescribeResponse(uint64_t latency, const std::string& contentbase, const sdp::Sdp& sdp) :
  Response(latency),
  contentbase_(contentbase),
  sdp_(sdp)
{

}

DescribeResponse::~DescribeResponse()
{

}

}
