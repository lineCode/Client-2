// devicemgr.h
//

#ifndef IDIZJDJDNPJ8VHU18PB7970ERP2MN34LJK
#define IDIZJDJDNPJ8VHU18PB7970ERP2MN34LJK

///// Includes /////

#include <boost/optional.hpp>
#include <memory>
#include <socket/proxyparams.hpp>
#include <QObject>
#include <QStringList>
#include <vector>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;

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

  boost::shared_ptr<Device> AddDevice(const sock::ProxyParams& proxyparams, const QString& address, const uint16_t port, const QString& username, const QString& password, bool save);
  int RemoveDevice(boost::shared_ptr<Device>& device);

  inline std::vector< boost::shared_ptr<Device> >& GetDevices() { return devices_; }
  std::vector< boost::shared_ptr<Device> > GetDevices(const uint64_t identifier);

  boost::shared_ptr<Device> GetDevice(const uint64_t identifier) const;

  QStringList GetLocations() const;

 protected:


 private:

  std::vector< boost::shared_ptr<Device> > devices_;

 signals:

  void DeviceAdded(const boost::shared_ptr<Device>& device);
  void DeviceRemoved(const boost::shared_ptr<Device>& device);
  void RecordingRemoved(const boost::shared_ptr<Device>& device, const uint64_t recordingtoken);
  void TimeOffsetChanged(const boost::shared_ptr<Device>& device);

};

}

QT_END_NAMESPACE

#endif
