// devicepropertieswindow.h
//

#ifndef ID2VVC9999999999999222222419WKDN9G
#define ID2VVC9999999999999222222419WKDN9G

///// Includes /////

#include "ui_devicepropertieswindow.h"

#include <monocleprotocol/client/connection.hpp>
#include <monocleprotocol/monocleprotocol.hpp>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <QPointF>
#include <QVector>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;

///// Structures /////

struct DISKSERIES
{
  DISKSERIES(const QString& device);

  QString device_;
  QLineSeries* utility_;
  QLineSeries* readspeed_;
  QLineSeries* writespeed_;

  QVector<QPointF> utilitypoints_; // <time, value>
  QVector<QPointF> readspeedpoints_; // <time, value>
  QVector<QPointF> writespeedpoints_; // <time, value>

};

struct GPUSERIES
{
  GPUSERIES(const QString& uuid, const QString& name);

  QString uuid_;
  QString name_;
  QLineSeries* usage_;
  QLineSeries* memoryusage_;
  QLineSeries* memory_;

  QVector<QPointF> usagepoints_; // <time, value>
  QVector<QPointF> memoryusagepoints_; // <time, value>
  QVector<QPointF> memorypoints_; // <time, value>

};

///// Classes /////

class DevicePropertiesWindow : public QDialog
{
 Q_OBJECT

 public:

  DevicePropertiesWindow(QWidget* parent, boost::shared_ptr<Device>& device);
  ~DevicePropertiesWindow();

 private:

  void AddSeries(const monocle::GPUSTAT& gpustat);
  void AddSeries(const monocle::DISKSTAT& diskstat);

  Ui::DevicePropertiesWindow ui_;

  boost::shared_ptr<Device> device_;

  monocle::client::Connection subscribehardwarestats_;

  // CPU Statistics
  QValueAxis* cpuaxisx_;
  QValueAxis* cpuaxisy_;
  QValueAxis* cpumemoryaxisy_;

  QLineSeries* cpu_;
  QLineSeries* memory_;
  QVector<QPointF> cpupoints_; // <time, value>
  QVector<QPointF> memorypoints_; // <time, value>

  // GPU Statistics
  QValueAxis* gpuaxisx_;
  QValueAxis* gpuaxisy_;
  QValueAxis* gpumemoryaxisy_;

  std::vector< std::unique_ptr<GPUSERIES> > gpuseries_;

  // Disk Statistics
  QValueAxis* diskaxisx_;
  QValueAxis* utilityaxisy_;
  QValueAxis* speedaxisy_;

  std::vector< std::unique_ptr<DISKSERIES> > diskseries_;

  uint64_t prevtime_;
  double maxspeed_;
  
 private slots:

  void on_checkgpuusage_stateChanged(int state);
  void on_checkgpumemoryusage_stateChanged(int state);
  void on_checkgpumemory_stateChanged(int state);
  void on_checkutility_stateChanged(int state);
  void on_checkreadspeed_stateChanged(int state);
  void on_checkwritespeed_stateChanged(int state);
  void SlotHardwareStats(const uint64_t time, const std::vector<monocle::DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory, const uint64_t availablememory, const std::vector<monocle::GPUSTAT>& gpustats);

};

}

QT_END_NAMESPACE

#endif
