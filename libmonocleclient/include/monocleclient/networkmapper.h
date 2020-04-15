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

  NetworkMapperScanner(const uint8_t a, const std::pair<uint8_t, uint8_t>& b, const std::pair<uint8_t, uint8_t>& c, const std::pair<uint8_t, uint8_t>& d, const size_t maxconnections);
  ~NetworkMapperScanner();

 private:

  std::vector<uint8_t> CreateRange(const std::pair<uint8_t, uint8_t>& inputs) const; // <start, end> inclusive
  std::string TakeAddress();
  std::string TakeElement(std::vector<uint8_t>& elements); // This method assumes there are items in the vector to be taken

  std::thread thread_;
  std::atomic<bool> running_;

  boost::shared_ptr<std::recursive_mutex> mutex_;

  const std::string a_;
  const std::pair<uint8_t, uint8_t> b_;
  const std::pair<uint8_t, uint8_t> c_;
  const std::pair<uint8_t, uint8_t> d_;
  const size_t maxconnections_;
  std::vector<uint8_t> currentb_;
  std::vector<uint8_t> currentc_;
  std::vector<uint8_t> currentd_;

  std::vector< std::pair< boost::shared_ptr<onvif::Connection>, boost::shared_ptr<onvif::device::DeviceClient> > > connections_;

 signals:

  void DiscoverONVIFDevice(const std::string& address);

};

class NetworkMapper : public QObject
{
 Q_OBJECT

 public:

  NetworkMapper();
  ~NetworkMapper();

 protected:



 private:

  std::vector< std::unique_ptr<NetworkMapperScanner> > scanners_;

 signals:

  void DiscoverONVIFDevice(const std::string& address);

 private slots:

};

}

#endif
