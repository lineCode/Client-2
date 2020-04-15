// networkmapper.cpp
//

///// Includes /////

#include "monocleclient/networkmapper.h"

#include <boost/algorithm/string.hpp>
#include <boost/asio/ip/address.hpp>
#include <utility/utility.hpp>
#include <QDebug>//TODO

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Globals /////

const size_t MAX_CONNECTIONS = 20;//TODO maybe more?

///// Methods /////

NetworkMapper::Scanner::Scanner(const boost::shared_ptr<std::recursive_mutex>& mutex, const uint8_t a, const std::pair<uint8_t, uint8_t>& b, const std::pair<uint8_t, uint8_t>& c, const std::pair<uint8_t, uint8_t>& d, const size_t maxconnections) :
  mutex_(mutex),
  a_(a),
  b_(b),
  c_(c),
  d_(d),
  maxconnections_(maxconnections),
  currentb_(CreateRange(b)),
  currentc_(CreateRange(c)),
  currentd_(CreateRange(d))
{

}

void NetworkMapper::Scanner::Update()
{
  while ((connections_.size() < maxconnections_) || (currentb_.size() && currentc_.size() && currentd_.size()))
  {
    //TODO steal an ip address..


    const std::string uri = "http://" + + "/onvif/device_service";



    boost::shared_ptr<onvif::device::DeviceClient> connection = boost::make_shared<onvif::device::DeviceClient>(mutex_);//TODO need to call update on all of these...
    if (connection->Init(sock::ProxyParams(), uri, "username", "password", 1, false, true))
    {
      // This should never happen, but lets crack on...
      continue;
    }

    //TODO now kick off

    connections_.push_back(connection);//TODO might need to mutex this?
  }

  for (const boost::shared_ptr<onvif::device::DeviceClient>& connection : connections_)
  {
    connection->Update();//TODO if this fails do we remove it?

  }
}

std::vector<uint8_t> NetworkMapper::Scanner::CreateRange(const std::pair<uint8_t, uint8_t>& inputs) const
{
  std::vector<uint8_t> range;
  range.reserve((inputs.second - inputs.first) + 1);
  for (uint8_t i = inputs.first; i <= inputs.second; ++i)
  {
    range.push_back(i);

  }
  return range;
}

NetworkMapper::NetworkMapper() :
  mutex_(boost::make_shared<std::recursive_mutex>())
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

    //TODO pass in the ports as well... we want http on 80,8888,8080,88 and https on 443
    if (boost::starts_with(address.address_, "10."))
    {
      classa = true;
      //TODO we want the subnet mask so we can decide how much we want to fiddle...

    }
    else if (boost::starts_with(address.address_, "192.168."))
    {
      classc = true;
      //TODO we want the subnet mask so we can decide how much we want to fiddle...

    }
    else if (boost::starts_with(address.address_, "169.254."))
    {
      linklocal = true;

    }
  }

  if (classa)
  {
    scanners_.push_back(Scanner(mutex_, 10, { 0, 255 }, { 0, 255 }, { 1, 255 }, MAX_CONNECTIONS));

  }

  if (classc)
  {
    scanners_.push_back(Scanner(mutex_, 192, { 168, 168 }, { 0, 255 }, { 1, 255 }, MAX_CONNECTIONS));

  }

  if (linklocal)
  {
    scanners_.push_back(Scanner(mutex_, 169, { 254, 168 }, { 0, 255 }, { 1, 255 }, MAX_CONNECTIONS));

  }

  startTimer(std::chrono::milliseconds(100));
}

NetworkMapper::~NetworkMapper()
{

}

void NetworkMapper::timerEvent(QTimerEvent*)
{
  for (Scanner& scanner : scanners_)
  {
    scanner.Update();

  }
}

}
