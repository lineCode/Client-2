// fmtp.cpp
//

///// Includes /////

#include "rtsp/sdp/fmtp.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Methods /////

Fmtp::Fmtp()
{

}

Fmtp::Fmtp(const std::map< int, std::vector<FMTPPARAMETER> >& fmtps) :
  fmtps_(fmtps)
{

}

Fmtp::Fmtp(const std::vector<std::string>& fmtps)
{
  for (const auto& fmtp : fmtps)
  {
    AddFmtp(fmtp);

  }
}

Fmtp::~Fmtp()
{

}

void Fmtp::AddFmtp(const std::string& fmtp)
{
  static const boost::regex tokensregex("[\\s]*a[\\s]*=[\\s]*fmtp[\\s]*:[\\s]*([\\d]*)[\\s]*(.*)");
  boost::smatch match;
  if (!boost::regex_search(fmtp, match, tokensregex))
  {

    return;
  }

  std::string tokens = match[2];
  static const boost::regex parametersregex("[\\w\\-\\=]+=[\\w\\-\\=,\\/\\+]+");
  std::vector<FMTPPARAMETER> parameters;
  for (boost::sregex_token_iterator i(tokens.begin(), tokens.end(), parametersregex); i != boost::sregex_token_iterator(); ++i)
  {
    std::string parameter = i->str();
    std::string::size_type pos = parameter.find('=');
    parameters.push_back(FMTPPARAMETER(parameter.substr(0, pos), parameter.substr(pos + 1, std::string::npos)));
  }

  fmtps_[std::stoi(match[1])] = parameters;
}

void Fmtp::Clear()
{
  fmtps_.clear();

}

bool Fmtp::IsValid() const
{
  for (const auto& fmtp : fmtps_)
  {
    for (const auto& parameter : fmtp.second)
    {
      if (!parameter.IsValid())
      {

        return false;
      }
    }
  }

  return true;
}

std::vector<std::string> Fmtp::ToString() const
{
  std::vector<std::string> fmtps;
  for (const auto& fmtp : fmtps_)
  {
    std::vector<std::string> parameters;
    for (auto parameter = fmtp.second.begin(); parameter != fmtp.second.end(); ++parameter)
    {
      if (!parameter->IsValid())
      {

        continue;
      }

      parameters.push_back(parameter->ToString());
    }

    fmtps.push_back(std::string("a=fmtp:") + std::to_string(fmtp.first) + std::string(" ") + boost::algorithm::join(parameters, std::string(";")));
  }

  return fmtps;
}

bool Fmtp::operator==(const Fmtp& rhs) const
{
  return (fmtps_ == rhs.fmtps_);
}

}

}
