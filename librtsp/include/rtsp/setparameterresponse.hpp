// setparameterresponse.hpp
//

#ifndef IDM239JBK4GYTMKU7YYA1H2IBT6B8YLOD8
#define IDM239JBK4GYTMKU7YYA1H2IBT6B8YLOD8

///// Includes /////

#include "headers/parameter.hpp"
#include "response.hpp"

#include <vector>

///// Namespaces /////

namespace rtsp
{

///// Classes /////

class SetParameterResponse : public Response
{
 public:

  SetParameterResponse();
  SetParameterResponse(uint64_t latency);
  SetParameterResponse(uint64_t latency, const std::vector<headers::Parameter>& parameters);
  ~SetParameterResponse();

  std::vector<headers::Parameter> parameters_;
};

}

#endif
