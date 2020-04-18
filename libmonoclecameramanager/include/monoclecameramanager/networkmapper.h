// networkmapper.h
// 

#ifndef IDFBJDBRQASDZCZZZZZZZZZADADZZZISRJ
#define IDFBJDBRQASDZCZZZZZZZZZADADZZZISRJ

///// Includes /////

#include <atomic>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <mutex>
#include <onvifclient/connection.hpp>
#include <onvifclient/deviceclient.hpp>
#include <QObject>
#include <QString>
#include <thread>
#include <vector>

///// Namespaces /////

namespace cm
{

///// Classes /////

class NetworkMapperScanner : public QObject
{
 Q_OBJECT

 public:

  NetworkMapperScanner(const uint8_t a, const std::pair<uint8_t, uint8_t>& b, const std::pair<uint8_t, uint8_t>& c, const std::pair<uint8_t, uint8_t>& d, const size_t maxconnections, const uint32_t network, const uint32_t netmask);
  ~NetworkMapperScanner();

  void Init();
  void Destroy();

  inline uint64_t GetCurrentCount() const { return currentcount_; }
  inline uint64_t GetTotalAddresses() const { return totaladdresses_; }
  
  inline uint32_t GetNetwork() const { return network_; }
  inline uint32_t GetNetmask() const { return netmask_; }

 private:

  int GetCurrentProgress() const;
  uint64_t CalculateTotalAddresses() const;
  std::string NextAddress();

  std::thread thread_;
  std::atomic<bool> running_;

  boost::shared_ptr<std::recursive_mutex> mutex_;

  const std::string blocka_;
  const uint8_t a_;
  const std::pair<uint8_t, uint8_t> b_;
  const std::pair<uint8_t, uint8_t> c_;
  const std::pair<uint8_t, uint8_t> d_;
  const size_t maxconnections_;
  const uint32_t network_;
  const uint32_t netmask_;
  uint16_t currentb_; // We use uint16_t so it doesn't wrap and cause problems
  uint16_t currentc_;
  uint16_t currentd_;
  uint64_t currentcount_;
  uint64_t totaladdresses_;
  int prevprogress_;

  std::vector< std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> > > getsystemdateandtimeconnections_;
  std::vector< std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> > > getcapabilitiesconnections_;

 signals:

  void DiscoverONVIFDevice(const QString& address);
  void Progress(int progress);

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

  void DiscoverONVIFDevice(const QString& address);
  void Progress(int progress);

 private slots:

  void ScannerProgress(int);

};

}

#endif
