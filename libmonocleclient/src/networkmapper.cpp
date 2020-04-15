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

NetworkMapper::Scanner::Scanner(const uint8_t a, const std::pair<uint8_t, uint8_t>& b, const std::pair<uint8_t, uint8_t>& c, const std::pair<uint8_t, uint8_t>& d, const size_t maxconnections) :
  running_(true),
  a_(std::to_string(a)),
  b_(b),
  c_(c),
  d_(d),
  maxconnections_(maxconnections),
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
            qDebug() << QString::fromStdString(uri);//TODO remove
            //TODO maybe send a signal?


            //TODO maybe do GetCapabilities/GetServices/GetServiceCapabilities etc here?

          }
        }))), connection)));
      }

      connections_.erase(std::remove_if(connections_.begin(), connections_.end(), [](const std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> >& connection) { return connection.second->Update(); }), connections_.end()); // Update and remove any that error
      connections_.erase(std::remove_if(connections_.begin(), connections_.end(), [](const std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> >& connection) { return !connection.first->IsConnected(); }), connections_.end());
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  });
}

NetworkMapper::Scanner::~Scanner()
{
  running_ = false;
  thread_.join();
  std::for_each(connections_.begin(), connections_.end(), [](std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> >& connection) { connection.first->Close(); });
}

std::string NetworkMapper::Scanner::TakeAddress()
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

std::string NetworkMapper::Scanner::TakeElement(std::vector<uint8_t>& elements)
{
  const std::string element = std::to_string(elements.front());
  elements.erase(elements.begin());
  return element;
}

std::vector<uint8_t> NetworkMapper::Scanner::CreateRange(const std::pair<uint8_t, uint8_t>& inputs) const
{
  std::vector<uint8_t> range;
  range.reserve((inputs.second - inputs.first) + 1);
  for (int i = inputs.first; i <= inputs.second; ++i)
  {
    range.push_back(static_cast<uint8_t>(i));

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
    scanners_.emplace_back(std::make_unique<Scanner>(10, std::make_pair(0, 255), std::make_pair(0, 255), std::make_pair(1, 255), MAX_CONNECTIONS));
    //TODO attach signal
  }

  if (classc)
  {
    scanners_.emplace_back(std::make_unique<Scanner>(192, std::make_pair(168, 168), std::make_pair(0, 255), std::make_pair(1, 255), MAX_CONNECTIONS));
    //TODO attach signal
  }

  if (linklocal)
  {
    scanners_.emplace_back(std::make_unique<Scanner>(169, std::make_pair(254, 254), std::make_pair(0, 255), std::make_pair(1, 255), MAX_CONNECTIONS));
    //TODO attach signal
  }
}

NetworkMapper::~NetworkMapper()
{

}

}
