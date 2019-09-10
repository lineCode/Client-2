// managedevicewindow.cpp
//

///// Includes /////

#include "monocleclient/managedevicewindow.h"

#include <QDoubleValidator>
#include <QMessageBox>

///// Namespaces /////

namespace client
{

///// Methods /////

ManageDeviceWindow::ManageDeviceWindow(QWidget* parent, const boost::shared_ptr<Device>& device) :
  QDialog(parent),
  device_(device)
{
  ui_.setupUi(this);
  
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  ui_.editname->setText(device_->GetName());

  if (!device_->SupportsDeviceLocation())
  {
    ui_.editlatitude->setEnabled(false);
    ui_.editlongitude->setEnabled(false);
  }
  else
  {
    ui_.editlatitude->setValidator(new QDoubleValidator(-90.0, 90.0, 6));
    ui_.editlongitude->setValidator(new QDoubleValidator(-180.0, 180.0, 6));

    const std::pair<QString, QString> location = device_->GetLocation();
    ui_.editlatitude->setText(location.first);
    ui_.editlongitude->setText(location.second);
  }
}

ManageDeviceWindow::~ManageDeviceWindow()
{
  setnameconnection_.Close();
  setlocationconnection_.Close();
}

void ManageDeviceWindow::SetEnabled(const bool enabled)
{
  ui_.editname->setEnabled(enabled);
  if (device_->SupportsDeviceLocation())
  {
    ui_.editlatitude->setEnabled(enabled);
    ui_.editlongitude->setEnabled(enabled);
  }
  ui_.buttonok->setEnabled(enabled);
}

void ManageDeviceWindow::on_buttonok_clicked()
{
  if (ui_.editlatitude->text().isEmpty() != ui_.editlongitude->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Both longitude and latitude must be set, or not set"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  setEnabled(false);

  std::shared_ptr<int> count = std::make_shared<int>(0);
  std::shared_ptr<int> total = std::make_shared<int>(0);
  std::shared_ptr<QStringList> errors = std::make_shared<QStringList>();
  if (ui_.editname->text() != device_->GetName())
  {
    ++(*total);
    setnameconnection_ = device_->SetName(ui_.editname->text().toStdString(), [this, count, total, errors, name = ui_.editname->text()](const std::chrono::steady_clock::duration, const monocle::client::SETNAMERESPONSE& setnameresponse)
    {
      ++(*count);
      if (setnameresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        errors->push_back(tr("Failed to set device name: ") + QString::fromStdString(setnameresponse.GetErrorText()));

      }
      else
      {
        ui_.editname->setText(name);

      }

      if (*count == *total)
      {
        Finished(*errors);

      }
    });
  }

  const std::pair<QString, QString> location = device_->GetLocation();
  if ((ui_.editlatitude->text() != location.first) || (ui_.editlongitude->text() != location.second))
  {
    ++(*total);
    setlocationconnection_ = device_->SetLocation(ui_.editlatitude->text().toStdString(), ui_.editlongitude->text().toStdString(), [this, count, total, errors, latitude = ui_.editlatitude->text(), longitude = ui_.editlongitude->text()](const std::chrono::steady_clock::duration, const monocle::client::SETLOCATIONRESPONSE& setlocationresponse)
    {
      ++(*count);
      if (setlocationresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        errors->push_back(tr("Failed to set location: ") + QString::fromStdString(setlocationresponse.GetErrorText()));

      }
      else
      {
        ui_.editlatitude->setText(latitude);
        ui_.editlongitude->setText(longitude);
      }

      if (*count == *total)
      {
        Finished(*errors);

      }
    });
  }

  if ((*total) == 0) // Did we do anything?
  {
    accept();

  }
}

void ManageDeviceWindow::Finished(const QStringList& errors)
{
  if (errors.isEmpty())
  {
    accept();
    return;
  }

  SetEnabled(true);
  QMessageBox(QMessageBox::Warning, tr("Error"), errors.join("\n"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
}

}
