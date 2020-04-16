// networkmapper.h
// 

#ifndef IDFBJDBRQASDASDADADADADADADZZZISRJ
#define IDFBJDBRQASDASDADADADADADADZZZISRJ

///// Includes /////

#include <atomic>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <mutex>
#include <onvifclient/connection.hpp>
#include <onvifclient/deviceclient.hpp>
#include <QObject>
#include <thread>
#include <vector>

///// Namespaces /////

namespace client
{

///// Classes /////

class NetworkMapperScanner : public QObject
{
 Q_OBJECT

 public:

  NetworkMapperScanner(const uint8_t a, const std::pair<uint8_t, uint8_t>& b, const std::pair<uint8_t, uint8_t>& c, const std::pair<uint8_t, uint8_t>& d, const size_t maxconnections, const uint32_t network, const uint32_t netmask);
  ~NetworkMapperScanner();
  
  inline uint32_t GetNetwork() const { return network_; }
  inline uint32_t GetNetmask() const { return netmask_; }

 private:

  std::string NextAddress();

  std::thread thread_;
  std::atomic<bool> running_;

  boost::shared_ptr<std::recursive_mutex> mutex_;

  const std::string a_;
  const std::pair<uint8_t, uint8_t> b_;
  const std::pair<uint8_t, uint8_t> c_;
  const std::pair<uint8_t, uint8_t> d_;
  const size_t maxconnections_;
  const uint32_t network_;
  const uint32_t netmask_;
  uint16_t currentb_; // We use uint16_t so it doesn't wrap and cause problems
  uint16_t currentc_;
  uint16_t currentd_;

  std::vector< std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> > > getsystemdateandtimeconnections_;
  std::vector< std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> > > getcapabilitiesconnections_;

 signals:

  void DiscoverONVIFDevice(const std::string& address);

};

class NetworkMapper : public QObject
{
 Q_OBJECT

 public:

  NetworkMapper();
  ~NetworkMapper();

  void Init();
  void Destroy();

 protected:



 private:

  std::vector< std::unique_ptr<NetworkMapperScanner> > scanners_;

 signals:

  void DiscoverONVIFDevice(const std::string& address);

 private slots:

};

}

#endif
