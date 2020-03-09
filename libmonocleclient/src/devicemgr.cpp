// devicemgr.cpp
//

///// Includes /////

#include "monocleclient/devicemgr.h"

#include <monocleprotocol/server/server.hpp>
#include <QCoreApplication>
#include <QMessageBox>
#include <QSettings>
#include <socket/socket.hpp>

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Globals /////

const QString DEVICES("devices");
const QString ADDRESS("address");
const QString PORT("port");
const QString PROXYTYPE("proxytype");
const QString PROXYHOSTNAME("proxyhostname");
const QString PROXYPORT("proxyport");
const QString PROXYREMOTEDNS("proxyremotedns");
const QString PROXYUSERNAME("proxyusername");
const QString PROXYPASSWORD("proxypassword");
const QString USERNAME("username");
const QString PASSWORD("password");
const QString IDENTIFIER("identifier");

///// Methods /////

DeviceMgr::DeviceMgr() :
  QObject(nullptr)
{
  
}

DeviceMgr::~DeviceMgr()
{
  Destroy();

}

void DeviceMgr::Init()
{
  Destroy();

  std::lock_guard<std::recursive_mutex> lock(mutex_);
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
  const int onvifsize = settings.beginReadArray(DEVICES);
  for (int i = 0; i < onvifsize; ++i)
  {
    settings.setArrayIndex(i);
    AddDevice(sock::ProxyParams(sock::FromInt(settings.value(PROXYTYPE).toInt()), settings.value(PROXYHOSTNAME).toString().toStdString(), settings.value(PROXYPORT).toUInt(), settings.value(PROXYREMOTEDNS).toBool(), settings.value(PROXYUSERNAME).toString().toStdString(), settings.value(PROXYPASSWORD).toString().toStdString()),
              settings.value(ADDRESS).toString(),
              settings.value(PORT, monocle::server::DEFAULTPORT).toInt(),
              settings.value(USERNAME).toString(),
              settings.value(PASSWORD).toString(),
              settings.value(IDENTIFIER, 0).toULongLong(),
              false);
  }
  settings.endArray();
}

void DeviceMgr::Destroy()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  for (auto& device : devices_)
  {
    device->Destroy();

  }
  devices_.clear();
}

void DeviceMgr::Save()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
  settings.beginWriteArray(DEVICES);
  for (int i = 0; i < devices_.size(); ++i)
  {
    const boost::shared_ptr<Device> device = devices_.at(i);
    settings.setArrayIndex(i);
    settings.setValue(ADDRESS, device->GetAddress());
    settings.setValue(PORT, device->GetPort());
    settings.setValue(PROXYTYPE, device->GetProxyParams().GetType());
    settings.setValue(PROXYHOSTNAME, QString::fromStdString(device->GetProxyParams().GetHostname()));
    settings.setValue(PROXYPORT, device->GetProxyParams().GetPort());
    settings.setValue(PROXYREMOTEDNS, device->GetProxyParams().GetRemoteDns());
    settings.setValue(PROXYUSERNAME, QString::fromStdString(device->GetProxyParams().GetUsername()));
    settings.setValue(PROXYPASSWORD, QString::fromStdString(device->GetProxyParams().GetPassword()));
    settings.setValue(USERNAME, device->GetUsername());
    settings.setValue(PASSWORD, device->GetPassword());
    settings.setValue(IDENTIFIER, device->GetIdentifier());
  }
  settings.endArray();
}

boost::shared_ptr<Device> DeviceMgr::AddDevice(const sock::ProxyParams& proxyparams, const QString& address, const uint16_t port, const QString& username, const QString& password, const uint64_t identifier, bool save)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  boost::shared_ptr<Device> device = boost::make_shared<Device>(proxyparams, address, port, username, password, identifier);
  device->Subscribe();
  devices_.push_back(device);
  emit DeviceAdded(device);

  if (save)
  {
    Save();

  }
  return device;
}

int DeviceMgr::RemoveDevice(boost::shared_ptr<Device>& device)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  device->Destroy();
  std::vector< boost::shared_ptr<Device> >::iterator i = std::find(devices_.begin(), devices_.end(), device);
  if (i == devices_.end())
  {

    return 1;
  }

  devices_.erase(i);
  emit DeviceRemoved(device);
  Save();
  return 0;
}

std::vector< boost::shared_ptr<Device> > DeviceMgr::GetDevices(const uint64_t identifier)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  std::vector< boost::shared_ptr<Device> > devices;
  for (const boost::shared_ptr<Device>& device : devices_)
  {
    if (device->GetIdentifier() == identifier)
    {
      devices.push_back(device);

    }
  }
  return devices;
}

std::vector< boost::shared_ptr<Device> > DeviceMgr::GetDevices(const std::vector< std::pair<boost::asio::ip::address, uint16_t> >& addresses, const uint64_t identifier)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  std::vector< boost::shared_ptr<Device> > devices;
  for (const boost::shared_ptr<Device>& device : devices_)
  {
    if (device->GetIdentifier() != identifier)
    {

      continue;
    }

    boost::system::error_code err;
    const boost::asio::ip::address address = boost::asio::ip::address::from_string(device->GetAddress().toStdString(), err);
    if (err)
    {

      continue;
    }
    if (utility::Contains(addresses, std::make_pair(address, device->GetPort())))
    {
      devices.push_back(device);

    }
  }
  return devices;
}

boost::shared_ptr<Device> DeviceMgr::GetDevice(const uint64_t identifier) const
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  std::vector< boost::shared_ptr<Device> >::const_iterator device = std::find_if(devices_.cbegin(), devices_.cend(), [identifier](const boost::shared_ptr<Device>& device) { return (device->GetIdentifier() && (device->GetIdentifier() == identifier)); });
  if (device == devices_.cend())
  {

    return nullptr;
  }
  return *device;
}

QStringList DeviceMgr::GetLocations() const
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  QStringList locations;
  for (const boost::shared_ptr<Device>& device : devices_)
  {
    locations += device->GetLocations();

  }
  return locations;
}

}
