// getparameterresponse.hpp
//

#ifndef IDL4E55OTGA1BAKDJD3JR8YYHK4DOJC83I
#define IDL4E55OTGA1BAKDJD3JR8YYHK4DOJC83I

///// Includes /////

#include <vector>

#include "response.hpp"
#include "headers/parameter.hpp"

///// Namespaces /////

namespace rtsp
{

///// Classes /////

class GetParameterResponse : public Response
{
 public:

  GetParameterResponse();
  GetParameterResponse(uint64_t latency);
  GetParameterResponse(uint64_t latency, const std::vector<headers::Parameter>& parameters);
  ~GetParameterResponse();

  std::vector<headers::Parameter> parameters_;

};

}

#endif
