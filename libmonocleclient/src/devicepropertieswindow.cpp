// devicepropertieswindow.cpp
//

///// Includes /////

#include <boost/asio.hpp>

#include "monocleclient/devicepropertieswindow.h"

#include <QStringList>
#include <QVector>

#include "monocleclient/device.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

DISKSERIES::DISKSERIES(const QString& device) :
  device_(device)
{

}

GPUSERIES::GPUSERIES(const QString& uuid, const QString& name) :
  uuid_(uuid),
  name_(name)
{

}

DevicePropertiesWindow::DevicePropertiesWindow(QWidget* parent, boost::shared_ptr<Device>& device) :
  QDialog(parent),
  device_(device),
  cpuaxisx_(new QValueAxis(this)),
  cpuaxisy_(new QValueAxis(this)),
  cpumemoryaxisy_(new QValueAxis(this)),
  gpuaxisx_(new QValueAxis(this)),
  gpuaxisy_(new QValueAxis(this)),
  gpumemoryaxisy_(new QValueAxis(this)),
  diskaxisx_(new QValueAxis(this)),
  utilityaxisy_(new QValueAxis(this)),
  speedaxisy_(new QValueAxis(this)),
  cpu_(new QLineSeries(this)),
  memory_(new QLineSeries(this)),
  prevtime_(0),
  maxspeed_(1024.0)
{
  ui_.setupUi(this);
  
  connect(device.get(), &Device::SignalHardwareStats, this, &DevicePropertiesWindow::SlotHardwareStats, Qt::QueuedConnection);
  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);
  
  // Parameters
  int paramrow = 0;
  int currentparamrow = paramrow++;
  if (device_->GetPublicKey().GetKey())
  {
    ui_.tableparameters->insertRow(currentparamrow);
    ui_.tableparameters->setItem(currentparamrow, 0, new QTableWidgetItem("Public Key"));
    ui_.tableparameters->setItem(currentparamrow, 1, new QTableWidgetItem(QString::fromStdString(device_->GetPublicKey().GetString())));
  }

  currentparamrow = paramrow++;
  ui_.tableparameters->insertRow(currentparamrow);
  ui_.tableparameters->setItem(currentparamrow, 0, new QTableWidgetItem("Architecture"));
  ui_.tableparameters->setItem(currentparamrow, 1, new QTableWidgetItem(device_->GetArchitecture()));

  currentparamrow = paramrow++;
  ui_.tableparameters->insertRow(currentparamrow);
  ui_.tableparameters->setItem(currentparamrow, 0, new QTableWidgetItem("Identifier"));
  ui_.tableparameters->setItem(currentparamrow, 1, new QTableWidgetItem(QString::number(device_->GetIdentifier())));

  if (device_->GetOperatingSystem().is_initialized())
  {
    currentparamrow = paramrow++;
    ui_.tableparameters->insertRow(currentparamrow);
    ui_.tableparameters->setItem(currentparamrow, 0, new QTableWidgetItem("Operating System"));
    ui_.tableparameters->setItem(currentparamrow, 1, new QTableWidgetItem(QString::fromStdString(utility::ToString(*device_->GetOperatingSystem()))));
  }

  currentparamrow = paramrow++;
  ui_.tableparameters->insertRow(currentparamrow);
  ui_.tableparameters->setItem(currentparamrow, 0, new QTableWidgetItem("Compiler"));
  ui_.tableparameters->setItem(currentparamrow, 1, new QTableWidgetItem(device_->GetCompiler()));

  currentparamrow = paramrow++;
  ui_.tableparameters->insertRow(currentparamrow);
  ui_.tableparameters->setItem(currentparamrow, 0, new QTableWidgetItem("Database Path"));
  ui_.tableparameters->setItem(currentparamrow, 1, new QTableWidgetItem(device_->GetDatabasePath()));

  currentparamrow = paramrow++;
  ui_.tableparameters->insertRow(currentparamrow);
  ui_.tableparameters->setItem(currentparamrow, 0, new QTableWidgetItem("Version"));
  ui_.tableparameters->setItem(currentparamrow, 1, new QTableWidgetItem(QString::fromStdString(device_->GetVersion().ToString())));

  currentparamrow = paramrow++;
  ui_.tableparameters->insertRow(currentparamrow);
  ui_.tableparameters->setItem(currentparamrow, 0, new QTableWidgetItem("Command Line Arguments"));
  const std::vector<QString> commandlinevariables = device_->GetCommandLineVariables();
  ui_.tableparameters->setItem(currentparamrow, 1, new QTableWidgetItem(QStringList::fromVector(QVector<QString>(commandlinevariables.cbegin(), commandlinevariables.cend())).join(" ")));

  currentparamrow = paramrow++;
  ui_.tableparameters->insertRow(currentparamrow);
  ui_.tableparameters->setItem(currentparamrow, 0, new QTableWidgetItem("Max Recordings"));
  ui_.tableparameters->setItem(currentparamrow, 1, new QTableWidgetItem(QString::number(device_->GetMaxRecordings())));

  currentparamrow = paramrow++;
  ui_.tableparameters->insertRow(currentparamrow);
  ui_.tableparameters->setItem(currentparamrow, 0, new QTableWidgetItem("Max Object Detectors"));
  ui_.tableparameters->setItem(currentparamrow, 1, new QTableWidgetItem(QString::number(device_->GetMaxObjectDetectors())));

  // Environment variables
  int envrow = 0;
  for (const QString& environmentvariable : device_->GetEnvironmentVariables())
  {
    const int currentrow = envrow++;
    ui_.tableenvironmentvariables->insertRow(currentrow);
    const int equals = environmentvariable.indexOf('=', 0);
    if (equals == -1)
    {
      ui_.tableenvironmentvariables->setItem(currentrow, 0, new QTableWidgetItem(environmentvariable));

    }
    else
    {
      ui_.tableenvironmentvariables->setItem(currentrow, 0, new QTableWidgetItem(environmentvariable.left(equals)));
      ui_.tableenvironmentvariables->setItem(currentrow, 1, new QTableWidgetItem(environmentvariable.mid(equals + 1)));
    }
  }

  // CPU Chart
  ui_.chartviewcpustatistics->chart()->addSeries(cpu_);
  ui_.chartviewcpustatistics->chart()->addSeries(memory_);
  cpu_->setName("CPU");
  memory_->setName("Memory");

  cpuaxisx_->setTitleText("Seconds");
  ui_.chartviewcpustatistics->chart()->addAxis(cpuaxisx_, Qt::AlignBottom);
  cpuaxisx_->setReverse(true);
  cpuaxisx_->setMin(0.0);
  cpuaxisx_->setMax(60.0);

  // CPU
  cpuaxisy_->setLabelFormat("%d%%");
  cpuaxisy_->setTitleText("CPU");
  cpuaxisy_->setMin(0.0);
  cpuaxisy_->setMax(100.0);
  ui_.chartviewcpustatistics->chart()->addAxis(cpuaxisy_, Qt::AlignLeft);
  cpu_->attachAxis(cpuaxisx_);
  cpu_->attachAxis(cpuaxisy_);

  // CPU Memory
  cpumemoryaxisy_->setLabelFormat("%dMB");
  cpumemoryaxisy_->setTitleText("Memory");
  cpumemoryaxisy_->setMin(0.0);
  cpumemoryaxisy_->setMax(1024.0);
  ui_.chartviewcpustatistics->chart()->addAxis(cpumemoryaxisy_, Qt::AlignRight);
  memory_->attachAxis(cpuaxisx_);
  memory_->attachAxis(cpumemoryaxisy_);

  // GPU Chart
  gpuaxisx_->setTitleText("Seconds");
  ui_.chartviewgpustatistics->chart()->addAxis(gpuaxisx_, Qt::AlignBottom);
  gpuaxisx_->setReverse(true);
  gpuaxisx_->setMin(0.0);
  gpuaxisx_->setMax(60.0);

  // GPU
  gpuaxisy_->setLabelFormat("%d%%");
  gpuaxisy_->setTitleText("GPU");
  gpuaxisy_->setMin(0.0);
  gpuaxisy_->setMax(100.0);
  ui_.chartviewgpustatistics->chart()->addAxis(gpuaxisy_, Qt::AlignLeft);

  // GPU Memory
  gpumemoryaxisy_->setLabelFormat("%dMB");
  gpumemoryaxisy_->setTitleText("Memory");
  gpumemoryaxisy_->setMin(0.0);
  gpumemoryaxisy_->setMax(1024.0);
  ui_.chartviewgpustatistics->chart()->addAxis(gpumemoryaxisy_, Qt::AlignRight);

  // Disk Chart
  diskaxisx_->setTitleText("Seconds");
  ui_.chartviewdiskstatistics->chart()->addAxis(diskaxisx_, Qt::AlignBottom);
  diskaxisx_->setReverse(true);
  diskaxisx_->setMin(0.0);
  diskaxisx_->setMax(60.0);

  // Utility
  utilityaxisy_->setLabelFormat("%d%%");
  utilityaxisy_->setTitleText("Utility");
  utilityaxisy_->setMin(0.0);
  utilityaxisy_->setMax(100.0);
  ui_.chartviewdiskstatistics->chart()->addAxis(utilityaxisy_, Qt::AlignLeft);

  // Speed
  speedaxisy_->setLabelFormat("%dkB/s");
  speedaxisy_->setTitleText("Speed");
  speedaxisy_->setMin(0.0);
  speedaxisy_->setMax(maxspeed_);
  ui_.chartviewdiskstatistics->chart()->addAxis(speedaxisy_, Qt::AlignRight);

  ui_.chartviewcpustatistics->setRenderHint(QPainter::Antialiasing);
  ui_.chartviewgpustatistics->setRenderHint(QPainter::Antialiasing);
  ui_.chartviewdiskstatistics->setRenderHint(QPainter::Antialiasing);

  // Hardware stats
  subscribehardwarestats_ = device_->SubscribeHardwareStats([this](const std::chrono::steady_clock::duration latency, const monocle::client::SUBSCRIBEHARDWARESTATSRESPONSE& subscribehardwarestatsresponse)
  {
    if (subscribehardwarestatsresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), QString::fromStdString(subscribehardwarestatsresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    prevtime_ = subscribehardwarestatsresponse.currenthardwarestats_.time_;
    cpumemoryaxisy_->setMax(subscribehardwarestatsresponse.currenthardwarestats_.totalmemory_ / (1024.0 * 1024.0));

    cpu_->append(0.0, subscribehardwarestatsresponse.currenthardwarestats_.cpuusage_ * 100.0);
    cpupoints_.push_back(QPointF(0.0, subscribehardwarestatsresponse.currenthardwarestats_.cpuusage_ * 100.0));
    memory_->append(0.0, (subscribehardwarestatsresponse.currenthardwarestats_.totalmemory_ - subscribehardwarestatsresponse.currenthardwarestats_.availablememory_) / (1024.0 * 1024.0));
    memorypoints_.push_back(QPointF(0.0, (subscribehardwarestatsresponse.currenthardwarestats_.totalmemory_ - subscribehardwarestatsresponse.currenthardwarestats_.availablememory_) / (1024.0 * 1024.0)));

    for (const monocle::GPUSTAT& gpustat : subscribehardwarestatsresponse.currenthardwarestats_.gpustats_)
    {
      AddSeries(gpustat);

    }

    for (const monocle::DISKSTAT& diskstat : subscribehardwarestatsresponse.currenthardwarestats_.diskstats_)
    {
      AddSeries(diskstat);

    }
  });
}

DevicePropertiesWindow::~DevicePropertiesWindow()
{
  subscribehardwarestats_.Close();

  device_->UnsubscribeHardwareStats(); // We don't really care if this fails, we are leaving this window
}

void DevicePropertiesWindow::AddSeries(const monocle::GPUSTAT& gpustat)
{
  std::unique_ptr<GPUSERIES> gpuseries = std::make_unique<GPUSERIES>(QString::fromStdString(gpustat.uuid_), QString::fromStdString(gpustat.name_));
  gpuseries->usage_ = new QLineSeries(this);
  gpuseries->memoryusage_ = new QLineSeries(this);
  gpuseries->memory_ = new QLineSeries(this);

  ui_.chartviewgpustatistics->chart()->addSeries(gpuseries->usage_);
  ui_.chartviewgpustatistics->chart()->addSeries(gpuseries->memoryusage_);
  ui_.chartviewgpustatistics->chart()->addSeries(gpuseries->memory_);

  gpuseries->usage_->setName(gpuseries->name_);
  gpuseries->memoryusage_->setName(gpuseries->name_);
  gpuseries->memory_->setName(gpuseries->name_);

  gpuseries->usage_->attachAxis(gpuaxisx_);
  gpuseries->memoryusage_->attachAxis(gpuaxisx_);
  gpuseries->memory_->attachAxis(gpuaxisx_);

  gpuseries->usage_->attachAxis(gpuaxisy_);
  gpuseries->memoryusage_->attachAxis(gpuaxisy_);
  gpuseries->memory_->attachAxis(gpumemoryaxisy_);

  // Each device gets one color
  gpuseries->memoryusage_->setColor(gpuseries->usage_->color());
  gpuseries->memory_->setColor(gpuseries->usage_->color());

  gpuseries->usagepoints_.push_back(QPointF(0.0, gpustat.gpuusage_));
  gpuseries->memoryusagepoints_.push_back(QPointF(0.0, gpustat.memoryusage_));
  gpuseries->memorypoints_.push_back(QPointF(0.0, gpustat.usedmemory_ / (1024 * 1024)));

  gpuseries->usage_->replace(gpuseries->usagepoints_);
  gpuseries->memoryusage_->replace(gpuseries->memoryusagepoints_);
  gpuseries->memory_->replace(gpuseries->memorypoints_);

  // Only show one of the series in the legend as they all share the same color
  for (QtCharts::QLegendMarker* marker : ui_.chartviewgpustatistics->chart()->legend()->markers())
  {
    if ((marker->series() == gpuseries->memoryusage_) || (marker->series() == gpuseries->memory_))
    {
      marker->setVisible(false);

    }
  }

  gpuseries_.emplace_back(std::move(gpuseries));
}

void DevicePropertiesWindow::AddSeries(const monocle::DISKSTAT& diskstat)
{
  std::unique_ptr<DISKSERIES> diskseries = std::make_unique<DISKSERIES>(QString::fromStdString(diskstat.device_));
  diskseries->utility_ = new QLineSeries(this);
  diskseries->readspeed_ = new QLineSeries(this);
  diskseries->writespeed_ = new QLineSeries(this);

  ui_.chartviewdiskstatistics->chart()->addSeries(diskseries->utility_);
  ui_.chartviewdiskstatistics->chart()->addSeries(diskseries->readspeed_);
  ui_.chartviewdiskstatistics->chart()->addSeries(diskseries->writespeed_);

  diskseries->utility_->setName(diskseries->device_);
  diskseries->readspeed_->setName(diskseries->device_);
  diskseries->writespeed_->setName(diskseries->device_);

  diskseries->utility_->attachAxis(diskaxisx_);
  diskseries->readspeed_->attachAxis(diskaxisx_);
  diskseries->writespeed_->attachAxis(diskaxisx_);

  diskseries->utility_->attachAxis(utilityaxisy_);
  diskseries->readspeed_->attachAxis(speedaxisy_);
  diskseries->writespeed_->attachAxis(speedaxisy_);

  // Each device gets one color
  diskseries->readspeed_->setColor(diskseries->utility_->color());
  diskseries->writespeed_->setColor(diskseries->utility_->color());

  diskseries->utility_->append(0.0, diskstat.utility_ * 100.0);
  diskseries->utilitypoints_.push_back(QPointF(0.0, diskstat.utility_ * 100.0));

  diskseries->readspeed_->append(0.0, diskstat.readspeed_ / 1024.0);
  diskseries->readspeedpoints_.push_back(QPointF(0.0, diskstat.readspeed_));

  diskseries->writespeed_->append(0.0, diskstat.writespeed_ / 1024.0);
  diskseries->writespeedpoints_.push_back(QPointF(0.0, diskstat.writespeed_));

  // Only show one of the series in the legend as they all share the same color
  for (QtCharts::QLegendMarker* marker : ui_.chartviewdiskstatistics->chart()->legend()->markers())
  {
    if ((marker->series() == diskseries->readspeed_) || (marker->series() == diskseries->writespeed_))
    {
      marker->setVisible(false);

    }
  }

  diskseries_.emplace_back(std::move(diskseries));
}

void DevicePropertiesWindow::on_checkgpuusage_stateChanged(int state)
{
  const bool visible = (state == Qt::Checked);
  for (std::unique_ptr<GPUSERIES>& gpuseries : gpuseries_)
  {
    gpuseries->usage_->setVisible(visible);

    // Maintain the correct visibility
    for (QtCharts::QLegendMarker* marker : ui_.chartviewgpustatistics->chart()->legend()->markers())
    {
      if (marker->series() == gpuseries->usage_)
      {
        marker->setVisible(true);

      }
    }
  }
}

void DevicePropertiesWindow::on_checkgpumemoryusage_stateChanged(int state)
{
  const bool visible = (state == Qt::Checked);
  for (std::unique_ptr<GPUSERIES>& gpuseries : gpuseries_)
  {
    gpuseries->memoryusage_->setVisible(visible);

    // Maintain the correct visibility
    for (QtCharts::QLegendMarker* marker : ui_.chartviewgpustatistics->chart()->legend()->markers())
    {
      if (marker->series() == gpuseries->memoryusage_)
      {
        marker->setVisible(false);

      }
    }
  }
}

void DevicePropertiesWindow::on_checkgpumemory_stateChanged(int state)
{
  const bool visible = (state == Qt::Checked);
  for (std::unique_ptr<GPUSERIES>& gpuseries : gpuseries_)
  {
    gpuseries->memory_->setVisible(visible);

    // Maintain the correct visibility
    for (QtCharts::QLegendMarker* marker : ui_.chartviewgpustatistics->chart()->legend()->markers())
    {
      if (marker->series() == gpuseries->memory_)
      {
        marker->setVisible(false);

      }
    }
  }
}

void DevicePropertiesWindow::on_checkutility_stateChanged(int state)
{
  const bool visible = (state == Qt::Checked);
  for (std::unique_ptr<DISKSERIES>& diskseries : diskseries_)
  {
    diskseries->utility_->setVisible(visible);

    // Maintain the correct visibility
    for (QtCharts::QLegendMarker* marker : ui_.chartviewdiskstatistics->chart()->legend()->markers())
    {
      if (marker->series() == diskseries->utility_)
      {
        marker->setVisible(true);

      }
    }
  }
}

void DevicePropertiesWindow::on_checkreadspeed_stateChanged(int state)
{
  const bool visible = state == Qt::Checked;
  for (std::unique_ptr<DISKSERIES>& diskseries : diskseries_)
  {
    diskseries->readspeed_->setVisible(visible);

    // Maintain the correct visibility
    for (QtCharts::QLegendMarker* marker : ui_.chartviewdiskstatistics->chart()->legend()->markers())
    {
      if (marker->series() == diskseries->readspeed_)
      {
        marker->setVisible(false);

      }
    }
  }
}

void DevicePropertiesWindow::on_checkwritespeed_stateChanged(int state)
{
  const bool visible = state == Qt::Checked;
  for (std::unique_ptr<DISKSERIES>& diskseries : diskseries_)
  {
    diskseries->writespeed_->setVisible(visible);

    // Maintain the correct visibility
    for (QtCharts::QLegendMarker* marker : ui_.chartviewdiskstatistics->chart()->legend()->markers())
    {
      if (marker->series() == diskseries->writespeed_)
      {
        marker->setVisible(false);

      }
    }
  }
}

void DevicePropertiesWindow::SlotHardwareStats(const uint64_t time, const std::vector<monocle::DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory, const uint64_t availablememory, const std::vector<monocle::GPUSTAT>& gpustats)
{
  if (time < prevtime_) // If the server has gone back in time(someone reset the utc clock) clear up and start again
  {
    cpupoints_.clear();
    memorypoints_.clear();
    gpuseries_.clear();
    diskseries_.clear();
    prevtime_ = time;
  }

  // Move the current set of points backwards
  const double diff = (static_cast<double>(time) - static_cast<double>(prevtime_)) / 1000.0;
  for (QPointF& cpupoint : cpupoints_)
  {
    cpupoint.setX(cpupoint.x() + diff);

  }
  for (QPointF& memorypoint : memorypoints_)
  {
    memorypoint.setX(memorypoint.x() + diff);

  }

  for (const monocle::GPUSTAT& gpustat : gpustats)
  {
    std::vector< std::unique_ptr<GPUSERIES> >::iterator gpuseries = std::find_if(gpuseries_.begin(), gpuseries_.end(), [&gpustat](const std::unique_ptr<GPUSERIES>& gpuseries) { return (gpuseries->uuid_ == QString::fromStdString(gpustat.uuid_)); });
    if (gpuseries == gpuseries_.end())
    {
      AddSeries(gpustat);

    }
    else
    {
      // Move the current set of points backwards
      for (QPointF& usagepoint : (*gpuseries)->usagepoints_)
      {
        usagepoint.setX(usagepoint.x() + diff);

      }
      for (QPointF& memoryusagepoint : (*gpuseries)->memoryusagepoints_)
      {
        memoryusagepoint.setX(memoryusagepoint.x() + diff);

      }
      for (QPointF& memorypoint : (*gpuseries)->memorypoints_)
      {
        memorypoint.setX(memorypoint.x() + diff);

      }

      (*gpuseries)->usagepoints_.push_back(QPointF(0.0, gpustat.gpuusage_));
      (*gpuseries)->memoryusagepoints_.push_back(QPointF(0.0, gpustat.memoryusage_));
      (*gpuseries)->memorypoints_.push_back(QPointF(0.0, gpustat.usedmemory_ / (1024 * 1024)));

      (*gpuseries)->usage_->replace((*gpuseries)->usagepoints_);
      (*gpuseries)->memoryusage_->replace((*gpuseries)->memoryusagepoints_);
      (*gpuseries)->memory_->replace((*gpuseries)->memorypoints_);

      gpumemoryaxisy_->setMax(std::max(gpumemoryaxisy_->max(), static_cast<qreal>(gpustat.totalmemory_ / (1024 * 1024))));
    }
  }

  for (const monocle::DISKSTAT& diskstat : diskstats)
  {
    std::vector< std::unique_ptr<DISKSERIES> >::iterator diskseries = std::find_if(diskseries_.begin(), diskseries_.end(), [&diskstat](const std::unique_ptr<DISKSERIES>& diskseries) { return (diskseries->device_ == QString::fromStdString(diskstat.device_)); });
    if (diskseries == diskseries_.end())
    {
      AddSeries(diskstat);

    }
    else
    {
      // Move the current set of points backwards
      for (QPointF& utilitypoint : (*diskseries)->utilitypoints_)
      {
        utilitypoint.setX(utilitypoint.x() + diff);

      }
      for (QPointF& readpoint : (*diskseries)->readspeedpoints_)
      {
        readpoint.setX(readpoint.x() + diff);

      }
      for (QPointF& writepoint : (*diskseries)->writespeedpoints_)
      {
        writepoint.setX(writepoint.x() + diff);

      }

      (*diskseries)->utilitypoints_.push_back(QPointF(0.0, diskstat.utility_ * 100.0));
      (*diskseries)->readspeedpoints_.push_back(QPointF(0.0, diskstat.readspeed_));
      (*diskseries)->writespeedpoints_.push_back(QPointF(0.0, diskstat.writespeed_));

      (*diskseries)->utility_->replace((*diskseries)->utilitypoints_);
      (*diskseries)->readspeed_->replace((*diskseries)->readspeedpoints_);
      (*diskseries)->writespeed_->replace((*diskseries)->writespeedpoints_);

      maxspeed_ = std::max(maxspeed_, diskstat.writespeed_);
      maxspeed_ = std::max(maxspeed_, diskstat.readspeed_);
    }
  }

  speedaxisy_->setMax(maxspeed_);

  cpupoints_.push_back(QPointF(0.0, cpuusage * 100.0));
  memorypoints_.push_back(QPointF(0.0, (static_cast<double>(totalmemory) - static_cast<double>(availablememory)) / (1024.0 * 1024.0)));

  // Replace the current set
  cpu_->replace(cpupoints_);
  memory_->replace(memorypoints_);

  cpuaxisx_->setMax(std::max(60.0, cpupoints_.front().x()));

  prevtime_ = time;
}

}

QT_END_NAMESPACE
