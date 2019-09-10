// parameters.hpp
//

#ifndef IDSFJF2YUVFVDZ1PLCF1KM3OF2OFSD6ETS
#define IDSFJF2YUVFVDZ1PLCF1KM3OF2OFSD6ETS

///// Includes /////

#include <string>
#include <vector>

#include "parameter.hpp"

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Classes /////

class Parameters
{
 public:

  Parameters();
  Parameters(const std::string& content);
  Parameters(const std::vector<Parameter>& parameters);
  ~Parameters();

  const std::vector<Parameter>& GetParameters() const { return parameters_; }

  std::string ToString() const;

  bool operator==(const Parameters& rhs) const;

  std::vector<Parameter> parameters_;

};

}

}

#endif
