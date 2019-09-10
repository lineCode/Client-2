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

///// Classes /////

class DevicePropertiesWindow : public QDialog
{
 Q_OBJECT

 public:

  DevicePropertiesWindow(QWidget* parent, boost::shared_ptr<Device>& device);
  ~DevicePropertiesWindow();

 private:

  void AddSeries(const monocle::DISKSTAT& diskstat);

  Ui::DevicePropertiesWindow ui_;

  boost::shared_ptr<Device> device_;

  monocle::client::Connection subscribehardwarestats_;

  // Hardware Statistics
  QValueAxis* hardwareaxisx_;
  QValueAxis* cpuaxisy_;
  QValueAxis* memoryaxisy_;

  QLineSeries* cpu_;
  QLineSeries* memory_;

  QVector<QPointF> cpupoints_; // <time, value>
  QVector<QPointF> memorypoints_; // <time, value>

  // Disk Statistics
  QValueAxis* diskaxisx_;
  QValueAxis* utilityaxisy_;
  QValueAxis* speedaxisy_;

  std::vector< std::unique_ptr<DISKSERIES> > diskseries_;

  uint64_t prevtime_;
  double maxspeed_;
  
 private slots:

  void on_checkutility_stateChanged(int state);
  void on_checkreadspeed_stateChanged(int state);
  void on_checkwritespeed_stateChanged(int state);
  void SlotHardwareStats(const uint64_t time, const std::vector<monocle::DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory, const uint64_t availablememory);

};

}

QT_END_NAMESPACE

#endif
