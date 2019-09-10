// describeresponse.hpp
//

#ifndef IDEE6WMUD0CZ3XPRPU50CX3WM84M7FSF2K
#define IDEE6WMUD0CZ3XPRPU50CX3WM84M7FSF2K

///// Includes /////

#include "response.hpp"
#include "sdp/sdp.hpp"

///// Namespaces /////

namespace rtsp
{

///// Classes /////

class DescribeResponse : public Response
{
 public:

  DescribeResponse();
  DescribeResponse(uint64_t latency);
  DescribeResponse(uint64_t latency, const std::string& contentbase, const sdp::Sdp& sdp);
  ~DescribeResponse();

  std::string contentbase_;
  sdp::Sdp sdp_;

};

}

#endif
