// parameters.cpp
//

///// Includes /////

#include "rtsp/headers/parameters.hpp"

#include <boost/algorithm/string_regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Methods /////

Parameters::Parameters()
{

}

Parameters::Parameters(const std::string& content)
{
  std::vector<std::string> parameters;
  static const boost::regex splitregex("\r\n|\r[^\n]|[^\r]\n");
  boost::algorithm::split_regex(parameters, content, splitregex);

  for (const auto& parameter : parameters)
  {
    const Parameter p(parameter);
    if (!p.IsValid())
    {

      continue;
    }
    parameters_.push_back(p);
  }
}

Parameters::Parameters(const std::vector<Parameter>& parameters) :
  parameters_(parameters)
{

}

Parameters::~Parameters()
{

}

std::string Parameters::ToString() const
{
  std::vector<std::string> parameters;
  for (const auto& parameter : parameters_)
  {
    parameters.push_back(parameter.ToString());

  }

  return boost::algorithm::join(parameters, std::string("\r\n"));
}

bool Parameters::operator==(const Parameters& rhs) const
{
  return (parameters_ == rhs.parameters_);
}

}

}
