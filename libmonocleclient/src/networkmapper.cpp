// networkmapper.cpp
//

///// Includes /////

#include "monocleclient/networkmapper.h"

#include <boost/algorithm/string.hpp>
#include <boost/asio/ip/address.hpp>
#include <utility/utility.hpp>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Globals /////

const size_t MAX_CONNECTIONS = 500;

///// Methods /////

NetworkMapperScanner::NetworkMapperScanner(const uint8_t a, const std::pair<uint8_t, uint8_t>& b, const std::pair<uint8_t, uint8_t>& c, const std::pair<uint8_t, uint8_t>& d, const size_t maxconnections, const uint32_t network, const uint32_t netmask) :
  running_(true),
  a_(std::to_string(a)),
  b_(b),
  c_(c),
  d_(d),
  maxconnections_(maxconnections),
  network_(network),
  netmask_(netmask),
  currentb_(CreateRange(b)),
  currentc_(CreateRange(c)),
  currentd_(CreateRange(d))
{
  thread_ = std::thread([this]()
  {
    while (running_)
    {
      if (connections_.size() < maxconnections_)
      {
        const std::string address = TakeAddress();
        if (address.empty()) // Finished
        {

          return;
        }

        boost::system::error_code err;
        const boost::asio::ip::address a = boost::asio::ip::address::from_string(address, err);
        if (err || !a.is_v4())
        {

          continue;
        }

        if (!utility::IsIPV4InRange(a.to_v4().to_ulong(), network_, netmask_))
        {

          continue;
        }

        qDebug() << QString::fromStdString(address);//TODO

        //TODO check utility::IsIPV4InRange(address, network, netmask);
          //TODO check this is actually culling the correct range of ips, which would be beautiful to see...

        //TODO we want to pass in port_ which can be empty, 8080 OR 80 as well...

        //TODO add ports in here...
        const std::string uri = std::string("http://") + address + "/onvif/device_service";
        boost::shared_ptr<onvif::device::DeviceClient> connection = boost::make_shared<onvif::device::DeviceClient>(boost::make_shared<std::recursive_mutex>());
        if (connection->Init(sock::ProxyParams(), uri, "username", "password", 1, false, true))
        {
          // This should never happen, but lets crack on...
          continue;
        }

        connections_.emplace_back(std::move(std::make_pair(boost::make_shared<onvif::Connection>(std::move(connection->GetSystemDateAndTimeCallback([this, uri, connection](const onvif::device::GetSystemDateAndTimeResponse& response)
        {
          if (response.Error())
          {
            // Some cameras require authentication for GetSystemDateAndTime(DLink) which is totally bananas, so lets ignore them

          }
          else
          {
            // We could go further and grab GetServices GetServiceCapabilities, GetCapabilities, but they aren't really necessary
            emit DiscoverONVIFDevice(uri);
          }
        }))), connection)));
      }

      connections_.erase(std::remove_if(connections_.begin(), connections_.end(), [](const std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> >& connection) { return connection.second->Update(); }), connections_.end()); // Update and remove any that error
      connections_.erase(std::remove_if(connections_.begin(), connections_.end(), [](const std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> >& connection) { return !connection.first->IsConnected(); }), connections_.end());
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  });
}

NetworkMapperScanner::~NetworkMapperScanner()
{
  running_ = false;
  thread_.join();
  std::for_each(connections_.begin(), connections_.end(), [](std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> >& connection) { connection.first->Close(); });
}

std::string NetworkMapperScanner::TakeAddress()
{
  std::string b;
  std::string c;
  std::string d;
  if (currentd_.empty())
  {
    if (currentc_.empty())
    {
      if (currentb_.empty())
      {

        return std::string();
      }
      else
      {
        currentc_ = CreateRange(c_);
        currentd_ = CreateRange(d_);
        b = TakeElement(currentb_);
        c = TakeElement(currentc_);
        d = TakeElement(currentd_);
      }
    }
    else
    {
      currentd_ = CreateRange(d_);
      b = std::to_string(currentb_.front());
      c = TakeElement(currentc_);
      d = TakeElement(currentd_);
    }
  }
  else
  {
    b = std::to_string(currentb_.front());
    c = std::to_string(currentc_.front());
    d = TakeElement(currentd_);
  }

  return (a_ + "." + b + "." + c + "." + d);
}

std::string NetworkMapperScanner::TakeElement(std::vector<uint8_t>& elements)
{
  const std::string element = std::to_string(elements.front());
  elements.erase(elements.begin());
  return element;
}

std::vector<uint8_t> NetworkMapperScanner::CreateRange(const std::pair<uint8_t, uint8_t>& inputs) const
{
  std::vector<uint8_t> range;
  range.reserve((inputs.second - inputs.first) + 1);
  for (int i = inputs.first; i <= inputs.second; ++i)
  {
    range.push_back(static_cast<uint8_t>(i));

  }
  return range;
}

NetworkMapper::NetworkMapper()
{
  const std::pair< int, std::vector<utility::ADDRESS> > addresses = utility::GetAddresses();
  if (addresses.first)
  {
    //TODO log error
    return;
  }

  // We don't do classb, because it is awkward with 172.16.x.y to 172.31.a.b and nobody uses it anyway
  for (const utility::ADDRESS& address : addresses.second)
  {
    boost::system::error_code err;
    const boost::asio::ip::address network = boost::asio::ip::address::from_string(address.address_, err);
    if (err)
    {

      continue;
    }

    const boost::asio::ip::address netmask = boost::asio::ip::address::from_string(address.netmask_, err);
    if (err)
    {

      continue;
    }

    if (!network.is_v4() || !netmask.is_v4()) // Maybe do ipv6 in the future
    {

      continue;
    }

    const uint32_t netmaskdata = netmask.to_v4().to_ulong();
    const uint32_t networkdata = network.to_v4().to_ulong() & netmaskdata;

    //TODO pass in the ports as well... we want http on 80,8888,8080,88 and https on 443

    if (std::find_if(scanners_.cbegin(), scanners_.cend(), [networkdata, netmaskdata](const std::unique_ptr<NetworkMapperScanner>& scanner) { return ((scanner->GetNetwork() == networkdata) && (scanner->GetNetmask() == netmaskdata)); }) != scanners_.cend())
    {

      continue;
    }

    if (boost::starts_with(address.address_, "10."))
    {
      scanners_.emplace_back(std::make_unique<NetworkMapperScanner>(10, std::make_pair(0, 255), std::make_pair(0, 255), std::make_pair(1, 255), MAX_CONNECTIONS, networkdata, netmaskdata));
      connect(scanners_.back().get(), &NetworkMapperScanner::DiscoverONVIFDevice, this, &NetworkMapper::DiscoverONVIFDevice);
    }
    
    if (boost::starts_with(address.address_, "192.168."))
    {
      scanners_.emplace_back(std::make_unique<NetworkMapperScanner>(192, std::make_pair(168, 168), std::make_pair(0, 255), std::make_pair(1, 255), MAX_CONNECTIONS, networkdata, netmaskdata));
      connect(scanners_.back().get(), &NetworkMapperScanner::DiscoverONVIFDevice, this, &NetworkMapper::DiscoverONVIFDevice);
    }
    
    if (boost::starts_with(address.address_, "169.254."))
    {
      scanners_.emplace_back(std::make_unique<NetworkMapperScanner>(169, std::make_pair(254, 254), std::make_pair(0, 255), std::make_pair(1, 255), MAX_CONNECTIONS, networkdata, netmaskdata));
      connect(scanners_.back().get(), &NetworkMapperScanner::DiscoverONVIFDevice, this, &NetworkMapper::DiscoverONVIFDevice);
    }
  }
}

NetworkMapper::~NetworkMapper()
{

}

}
