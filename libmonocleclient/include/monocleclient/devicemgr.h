// devicemgr.h
//

#ifndef IDIZJDJDNPJ8VHU18PB7970ERP2MN34LJK
#define IDIZJDJDNPJ8VHU18PB7970ERP2MN34LJK

///// Includes /////

#include <boost/asio/ip/address.hpp>
#include <boost/optional.hpp>
#include <memory>
#include <mutex>
#include <socket/proxyparams.hpp>
#include <QObject>
#include <QSharedPointer>
#include <QStringList>
#include <vector>

#include "monocleclient/connection.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Layout;

///// Classes /////

class DeviceMgr : public QObject
{
 Q_OBJECT

 public:

  DeviceMgr();
  virtual ~DeviceMgr();

  void Init();
  void Destroy();

  void Save();

  boost::shared_ptr<Device> AddDevice(const sock::ProxyParams& proxyparams, const QString& address, const uint16_t port, const QString& username, const QString& password, const uint64_t identifier, bool save);
  int RemoveDevice(boost::shared_ptr<Device>& device);

  inline std::vector< boost::shared_ptr<Device> >& GetDevices() { return devices_; }
  std::vector< boost::shared_ptr<Device> > GetDevices(const std::vector< std::pair<boost::asio::ip::address, uint16_t> >& addresses, const uint64_t identifier); // <address, port>
  std::vector< boost::shared_ptr<Device> > GetDevices(const uint64_t identifier);
  boost::shared_ptr<Device> GetDevice(const uint64_t identifier) const;

  QStringList GetLocations() const;

  std::vector< QSharedPointer<Layout> > GetLayouts() const;
  std::vector< QSharedPointer<Layout> > GetLayouts(const uint64_t token) const;
  uint64_t GetUniqueLayoutToken() const;

 protected:


 private:

  mutable std::recursive_mutex mutex_;
  std::vector< boost::shared_ptr<Device> > devices_;

 signals:

  void DeviceAdded(const boost::shared_ptr<Device>& device);
  void DeviceRemoved(const boost::shared_ptr<Device>& device);
  void GuiOrderChanged(const boost::shared_ptr<Device>& device, const std::vector<GUIORDER>& recordingsorder, const std::vector<GUIORDER>& mapsorder);
  void LayoutAdded(const QSharedPointer<Layout>& layout);
  void LayoutChanged(const QSharedPointer<Layout>& layout);
  void LayoutRemoved(const uint64_t token);
  void RecordingRemoved(const boost::shared_ptr<Device>& device, const uint64_t recordingtoken);
  void TimeOffsetChanged(const boost::shared_ptr<Device>& device);

};

}

QT_END_NAMESPACE

#endif
