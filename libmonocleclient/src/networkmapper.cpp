// networkmapper.cpp
//

///// Includes /////

#include "monocleclient/networkmapper.h"

#include <boost/algorithm/string.hpp>
#include <boost/asio/ip/address.hpp>
#include <utility/utility.hpp>
#include <QDebug>//TODO
///// Namespaces /////

namespace client
{

///// Methods /////

NetworkMapper::NetworkMapper()
{
  const std::pair< int, std::vector<utility::ADDRESS> > addresses = utility::GetAddresses();
  if (addresses.first)
  {
    //TODO log error
    return;
  }

  // We don't do classb, because it is awkward with 172.16.x.y to 172.31.a.b and nobody uses it anyway
  bool classa = false;
  bool classc = false;
  bool linklocal = false;
  for (const utility::ADDRESS& address : addresses.second)
  {
    boost::system::error_code err;
    const boost::asio::ip::address a = boost::asio::ip::address::from_string(address.address_, err);
    if (err)
    {

      continue;
    }

    if (!a.is_v4()) // Maybe do ipv6 in the future
    {

      continue;
    }

    if (boost::starts_with(address.address_, "10."))
    {
      classa = true;

    }
    else if (boost::starts_with(address.address_, "192.168."))
    {
      classc = true;

    }
    else if (boost::starts_with(address.address_, "169.254."))
    {
      linklocal = true;

    }
    qDebug() << QString::fromStdString(address.address_);
  }



}

NetworkMapper::~NetworkMapper()
{

}

}
