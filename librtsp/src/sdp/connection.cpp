// connection.cpp
//

///// Includes /////

#include "rtsp/sdp/connection.hpp"

#include <boost/regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Methods /////

Connection::Connection()
{

}

Connection::Connection(const std::vector<CONNECTIONDATA>& connections) :
  connections_(connections)
{

}

Connection::Connection(const std::vector<std::string>& connections)
{
  for (const auto& connection : connections)
  {
    AddConnection(connection);

  }
}

Connection::~Connection()
{

}

void Connection::AddConnection(const std::string& connection)
{
  static const boost::regex regex("[\\s]*c[\\s]*=[\\s]*([\\w]*)[\\s]+([\\w]*)[\\s]+([\\w\\.:]*)\\/?([\\d]+)?\\/?([\\d]+)?");
  boost::smatch match;
  if (!boost::regex_search(connection, match, regex))
  {

    return;
  }

  CONNECTIONDATA connectiondata;
  connectiondata.nettype_ = NetTypeFromString(match[1]);
  connectiondata.addresstype_ = AddressTypeFromString(match[2]);

  try
  {
    connectiondata.address_ = boost::asio::ip::address::from_string(match[3]);

  }
  catch (...)
  {

    return;
  }

  if (connectiondata.address_.is_v4())
  {
    if (match[4].matched)
    {
      connectiondata.ttl_ = std::stoi(match[4]);

    }

    if (match[5].matched)
    {
      connectiondata.numaddresses_ = std::stoi(match[5]);

    }
    else
    {
      connectiondata.numaddresses_ = 1;

    }
  }
  else
  {
    if (match[4].matched)
    {
      connectiondata.numaddresses_ = std::stoi(match[4]);

    }
    else
    {
      connectiondata.numaddresses_ = 1;

    }
  }

  connections_.push_back(connectiondata);
}

void Connection::Clear()
{
  connections_.clear();

}


bool Connection::IsValid() const
{
  for (const auto& connection : connections_)
  {
    if (!connection.IsValid())
    {

      return false;
    }
  }

  return true;
}

std::vector<std::string> Connection::ToString() const
{
  std::vector<std::string> connections;
  for (const auto& connection : connections_)
  {
    if (!IsValid())
    {

      continue;
    }

    connections.push_back(connection.ToString());
  }

  return connections;
}

bool Connection::operator==(const Connection& rhs) const
{
  return std::is_permutation(connections_.begin(), connections_.end(), rhs.connections_.begin(), rhs.connections_.end());
}

}

}
