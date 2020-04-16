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

const size_t MAX_CONNECTIONS = 200;

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
  currentb_(b.first),
  currentc_(c.first),
  currentd_(d.first)
{
  thread_ = std::thread([this]()
  {
    while (running_)
    {
      if (getsystemdateandtimeconnections_.size() < maxconnections_)
      {
        const std::string address = NextAddress();
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

        const std::string uri = std::string("http://") + address + "/onvif/device_service";
        boost::shared_ptr<onvif::device::DeviceClient> connection = boost::make_shared<onvif::device::DeviceClient>(boost::make_shared<std::recursive_mutex>());
        if (connection->Init(sock::ProxyParams(), uri, "username", "password", 1, false, true))
        {
          // This should never happen, but lets crack on...
          continue;
        }

        getsystemdateandtimeconnections_.emplace_back(std::move(std::make_pair(boost::make_shared<onvif::Connection>(std::move(connection->GetSystemDateAndTimeCallback([this, uri, connection](const onvif::device::GetSystemDateAndTimeResponse& response)
        {
          if (response.Message() == onvif::UNABLETOCONNECT)
          {

            return;
          }

          getcapabilitiesconnections_.emplace_back(std::move(std::make_pair(boost::make_shared<onvif::Connection>(std::move(connection->GetCapabilitiesCallback(onvif::CAPABILITYCATEGORY_MEDIA, [this, uri, connection](const onvif::device::GetCapabilitiesResponse& response)
          {
            if (response.capabilities_.is_initialized() && response.capabilities_->media_.is_initialized() && response.capabilities_->media_->xaddr_.is_initialized()) // We only care if it has a media profile
            {
              emit DiscoverONVIFDevice(uri);

            }
          }))), connection)));
        }))), connection)));
      }

      getsystemdateandtimeconnections_.erase(std::remove_if(getsystemdateandtimeconnections_.begin(), getsystemdateandtimeconnections_.end(), [](const std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> >& connection) { return connection.second->Update(); }), getsystemdateandtimeconnections_.end()); // Update and remove any that error
      getsystemdateandtimeconnections_.erase(std::remove_if(getsystemdateandtimeconnections_.begin(), getsystemdateandtimeconnections_.end(), [](const std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> >& connection) { return !connection.first->IsConnected(); }), getsystemdateandtimeconnections_.end());
      getcapabilitiesconnections_.erase(std::remove_if(getcapabilitiesconnections_.begin(), getcapabilitiesconnections_.end(), [](const std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> >& connection) { return connection.second->Update(); }), getcapabilitiesconnections_.end()); // Update and remove any that error
      getcapabilitiesconnections_.erase(std::remove_if(getcapabilitiesconnections_.begin(), getcapabilitiesconnections_.end(), [](const std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> >& connection) { return !connection.first->IsConnected(); }), getcapabilitiesconnections_.end());
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  });
}

NetworkMapperScanner::~NetworkMapperScanner()
{
  if (thread_.joinable())
  {
    running_ = false;
    thread_.join();
    std::for_each(getsystemdateandtimeconnections_.begin(), getsystemdateandtimeconnections_.end(), [](std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> >& connection) { connection.first->Close(); });
    std::for_each(getcapabilitiesconnections_.begin(), getcapabilitiesconnections_.end(), [](std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> >& connection) { connection.first->Close(); });
  }
}

std::string NetworkMapperScanner::NextAddress()
{
  const uint8_t b = currentb_;
  const uint8_t c = currentc_;
  const uint8_t d = currentd_;
  if (++currentd_ > d_.second) // Increment and find the next suitable address
  {
    currentd_ = d_.first;
    if (++currentc_ > c_.second)
    {
      currentc_ = c_.first;
      if (++currentb_ > b_.second)
      {

        return std::string(); // Done
      }
    }
  }
  
  return a_ + "." + std::to_string(b) + "." + std::to_string(c) + "." + std::to_string(d);
}

NetworkMapper::NetworkMapper()
{
  Init();

}

NetworkMapper::~NetworkMapper()
{
  Destroy();

}

void NetworkMapper::Init()
{
  Destroy();

  const std::pair< int, std::vector<utility::ADDRESS> > addresses = utility::GetAddresses();
  if (addresses.first)
  {
    LOG_GUI_WARNING(tr("utility::GetAddresses failed"));
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
  }
}

void NetworkMapper::Destroy()
{
  scanners_.clear();

}

}
