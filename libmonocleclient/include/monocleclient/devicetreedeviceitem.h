// devicetreedeviceitem.h
//

#ifndef ID83Q9PV26459F1U39NZ9B2NWZNKI7PSMM
#define ID83Q9PV26459F1U39NZ9B2NWZNKI7PSMM

///// Includes /////

#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <QAction>
#include <QSharedPointer>

#include "monocleclient/devicetreeitem.h"
#include "monocleclient/connection.h"

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class DeviceTree;
class Map;
class Recording;

enum class DEVICESTATE : int;

///// Classes /////

class DeviceTreeDeviceItem : public DeviceTreeItem
{
 Q_OBJECT

 public:

  DeviceTreeDeviceItem(DeviceTree* parent, const boost::shared_ptr<Device>& device, const QIcon& latencygreen, const QIcon& latencyyellow, const QIcon& latencyred, const QIcon& latencynone, const QIcon& recordingicon, const QIcon& mapicon);
  virtual ~DeviceTreeDeviceItem();

  boost::shared_ptr<Device>& GetDevice() { return device_; }

  virtual void ContextMenuEvent(const QPoint& pos) override;

  virtual void Expanded() override;
  virtual void Collapsed() override;

  virtual void DoubleClicked() override;

  void SetFilter(const QString& filter);


 protected:



 private:

  void Init();
  void ResetLatency();
  void SetUnlicensed();
  void SetState(const DEVICESTATE state, const QString& message);

  const QIcon& latencygreen_;
  const QIcon& latencyyellow_;
  const QIcon& latencyred_;
  const QIcon& latencynone_;
  const QIcon& recordingicon_;
  const QIcon& mapicon_;

  QAction* reconnect_;
  QAction* edit_;
  QAction* remove_;
  QAction* manage_;
  QAction* managefiles_;
  QAction* managerecordings_;
  QAction* managemaps_;
  QAction* managegroups_;
  QAction* manageusers_;
  QAction* manageonvifusers_;
  QAction* viewlog_;
  QAction* properties_;

  boost::shared_ptr<Device> device_;


  


 private slots:

  void Reconnect(bool);
  void Edit(bool);
  void Remove(bool);
  void Manage(bool);
  void ManageFiles(bool);
  void ManageRecordings(bool);
  void ManageMaps(bool);
  void ManageGroups(bool);
  void ManageUsers(bool);
  void ManageONVIFUsers(bool);
  void ViewLog(bool);
  void Properties(bool);
  void Disconnected();
  void GuiOrderChanged(const std::vector<GUIORDER>& recordingsorder, const std::vector<GUIORDER>& mapsorder);
  void Latency(const std::chrono::steady_clock::duration latency);
  void MapAdded(const QSharedPointer<client::Map>& map);
  void MapChanged(const QSharedPointer<client::Map>& map);
  void MapRemoved(const uint64_t token);
  void NameChanged(const QString& name);
  void RecordingAdded(QSharedPointer<client::Recording>& recording);
  void RecordingChanged(QSharedPointer<client::Recording>& recording);
  void RecordingRemoved(const uint64_t token);
  void StateChanged(const client::DEVICESTATE state, const QString& message);
  void LicenseStateChanged(const bool valid);

};

}

#endif
