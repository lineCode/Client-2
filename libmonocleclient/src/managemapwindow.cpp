// managemapwindow.cpp
//

///// Includes /////

#include "monocleclient/managemapwindow.h"

#include <QCompleter>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QStringListModel>
#include <QUrl>

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/map.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageMapWindow::ManageMapWindow(QWidget* parent, boost::shared_ptr<Device>& device, const boost::optional<uint64_t>& token) :
  QDialog(parent),
  device_(device),
  token_(token)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  QCompleter* completer = new QCompleter(this);
  QStringListModel* model = new QStringListModel(this);
  model->setStringList(MainWindow::Instance()->GetDeviceMgr().GetLocations());
  completer->setModel(model);
  ui_.editlocation->setCompleter(completer);

  if (token_.is_initialized())
  {
    setWindowTitle("Edit Map");

    connect(device.get(), QOverload<const QSharedPointer<client::Map>&>::of(&Device::SignalMapChanged), this, &ManageMapWindow::MapChanged);
    connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalMapRemoved), this, &ManageMapWindow::MapRemoved);

    QSharedPointer<client::Map> map = device_->GetMap(*token_);
    ui_.editname->setText(map->GetName());
    ui_.editlocation->setText(map->GetLocation());
  }
}

ManageMapWindow::~ManageMapWindow()
{
  mapconnection_.Close();

}

void ManageMapWindow::SetEnabled(const bool enabled)
{
  ui_.editname->setEnabled(enabled);
  ui_.editlocation->setEnabled(enabled);
  ui_.editfile->setEnabled(enabled);
  ui_.buttonbrowsefile->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageMapWindow::MapChanged(const QSharedPointer<client::Map>& map)
{
  if (token_.is_initialized() && (*token_ == map->GetToken()))
  {
    // Ignore the change if it is identical to what is currently selected
    if ((ui_.editname->text() == map->GetName()) && (map->GetLocation() == ui_.editlocation->text()))
    {
  
      return;
    }
  
    if (QMessageBox::question(this, tr("Map Changed"), tr("Map has been changed, Would you like to update with new values?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
      ui_.editname->setText(map->GetName());
      ui_.editlocation->setText(map->GetLocation());
    }
  }
}

void ManageMapWindow::MapRemoved(const uint64_t token)
{
  if (token_.is_initialized() && (*token_ == token))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Map has been removed"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    reject();
  }
}

void ManageMapWindow::on_buttonbrowsefile_clicked()
{
  const QString filename = QFileDialog::getOpenFileName(this, tr("Map"), QString(), tr("Image Files (*.bmp *.png *.jpg *.jpeg);;"));
  if (filename.isEmpty())
  {

    return;
  }

  QImage image;
  if (!image.load(filename))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to load file"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  ui_.editfile->setText(filename);
}

void ManageMapWindow::on_buttonok_clicked()
{
  if (ui_.editname->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Please enter a name for this Map"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (ui_.editlocation->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Please enter a location for this Map"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  {
    QImage image;
    if (!image.load(ui_.editfile->text()))
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to import file"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  }

  QFile file(ui_.editfile->text());
  if (!file.open(QIODevice::ReadOnly))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to load file"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  const QByteArray readdata = file.readAll();
  if (readdata.isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to read"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  const std::vector<int8_t> data(readdata.cbegin(), readdata.cend());

  SetEnabled(false);
  if (token_.is_initialized())
  {
    QSharedPointer<client::Map> map = device_->GetMap(*token_);
    if (!map)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error unable to retrieve map: ") + QString::number(*token_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
    
    mapconnection_ = device_->ChangeMap(*token_, ui_.editname->text().toStdString(), ui_.editlocation->text().toStdString(), data, [this](const std::chrono::nanoseconds latency, const monocle::client::CHANGEMAPRESPONSE& changemapresponse)
    {
      SetEnabled(true);
      if (changemapresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(changemapresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
  else
  {
    mapconnection_ = device_->AddMap(ui_.editname->text().toStdString(), ui_.editlocation->text().toStdString(), data, [this](const std::chrono::nanoseconds latency, const monocle::client::ADDMAPRESPONSE& addmapresponse)
    {
      SetEnabled(true);
      if (addmapresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(addmapresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
}

}

QT_END_NAMESPACE
