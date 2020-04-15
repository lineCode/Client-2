// networkmapper.h
// 

#ifndef IDFBJDBRQASDASDADADADADADADZZZISRJ
#define IDFBJDBRQASDASDADADADADADADZZZISRJ

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <mutex>
#include <onvifclient/connection.hpp>
#include <onvifclient/deviceclient.hpp>
#include <QObject>
#include <vector>

///// Namespaces /////

namespace client
{

///// Classes /////

class NetworkMapper : public QObject
{
 Q_OBJECT

 public:

  NetworkMapper();
  ~NetworkMapper();

 protected:

  virtual void timerEvent(QTimerEvent*) override;

 private:

  class Scanner
  {
   public:

    Scanner(const boost::shared_ptr<std::recursive_mutex>& mutex, const uint8_t a, const std::pair<uint8_t, uint8_t>& b, const std::pair<uint8_t, uint8_t>& c, const std::pair<uint8_t, uint8_t>& d, const size_t maxconnections);
    ~Scanner();

    void Update();

   private:

    std::vector<uint8_t> CreateRange(const std::pair<uint8_t, uint8_t>& inputs) const; // <start, end> inclusive
    std::string TakeAddress();
    std::string TakeElement(std::vector<uint8_t>& elements); // This method assumes there are items in the vector to be taken

    boost::shared_ptr<std::recursive_mutex> mutex_;
    const std::string a_;
    const std::pair<uint8_t, uint8_t> b_;
    const std::pair<uint8_t, uint8_t> c_;
    const std::pair<uint8_t, uint8_t> d_;
    const size_t maxconnections_;
    std::vector<uint8_t> currentb_;
    std::vector<uint8_t> currentc_;
    std::vector<uint8_t> currentd_;
    
    std::vector< std::pair<onvif::Connection, boost::shared_ptr<onvif::device::DeviceClient> > > connections_;

  };

  boost::shared_ptr<std::recursive_mutex> mutex_;

  std::vector<Scanner> scanners_;

 signals:

  void DiscoverONVIFDevice();

 private slots:

};

}

#endif
