// optionsresponse.hpp
//

#ifndef IDAA4K8HA6Z1OY03FR8CYILLLOWVE5GQTX
#define IDAA4K8HA6Z1OY03FR8CYILLLOWVE5GQTX

///// Includes /////

#include <boost/container/flat_set.hpp>

#include "response.hpp"
#include "headers/requesttype.hpp"

///// Namespaces /////

namespace rtsp
{

///// Classes /////

class OptionsResponse : public Response
{
 public:

  OptionsResponse();
  OptionsResponse(uint64_t latency);
  OptionsResponse(uint64_t latency, const boost::container::flat_set<headers::REQUESTTYPE>& options);
  ~OptionsResponse();

  boost::container::flat_set<headers::REQUESTTYPE> options_;

};

}

#endif
