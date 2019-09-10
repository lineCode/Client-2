// transport.cpp
//

///// Includes /////

#include "rtsp/headers/transport.hpp"

#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Methods /////

Transport::Transport()
{

}

Transport::Transport(const std::string& text)
{
  static const boost::regex regex("[\\s]*Transport[\\s]*:[\\s]*(.*)");
  boost::smatch match;
  if (!boost::regex_search(text, match, regex))
  {

    return;
  }

  std::vector<std::string> transports;
  boost::algorithm::split_regex(transports, match[1].str(), boost::regex("[\\s]*,[\\s]*"));

  for (const auto& transport : transports)
  {
    transports_.push_back(TransportData(transport));

  }
}

Transport::Transport(const std::vector<TransportData>& transports) :
  transports_(transports)
{

}

Transport::~Transport()
{

}

void Transport::Clear()
{
  transports_.clear();
}

bool Transport::IsValid() const
{
  for (const auto& transport : transports_)
  {
    if (!transport.IsValid())
    {

      return false;
    }
  }

  return true;
}

std::string Transport::ToString() const
{
  if (!IsValid())
  {

    return std::string();
  }

  if (transports_.empty())
  {

    return std::string();
  }

  std::vector<std::string> transports;
  for (const auto& transport : transports_)
  {
    transports.push_back(transport.ToString());

  }
  return (std::string("Transport: ") + boost::algorithm::join(transports, std::string(",")));
}

bool Transport::operator==(const Transport& rhs) const
{
  return (transports_ == rhs.transports_);
}

}

}
