// managerecordingwindow.cpp
//

///// Includes /////

#include "monocleclient/managerecordingwindow.h"

#include <QCompleter>
#include <QMessageBox>
#include <QStringListModel>
#include <QUrl>

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/recording.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageRecordingWindow::ManageRecordingWindow(QWidget* parent, boost::shared_ptr<Device>& device, const boost::optional<uint64_t>& token) :
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

  if (!device_->SupportsRecordingLocation())
  {
    ui_.editlocation->setEnabled(false);

  }

  if (!device_->SupportsCreateDefaultJob())
  {
    ui_.checkcreatedefaultjob->setEnabled(false);
    ui_.checkcreatedefaultjob->setChecked(false);
    ui_.checkcreatedefaultjob->setToolTip("Please upgrade server for support of this option");
  }

  if (token_.is_initialized())
  {
    setWindowTitle("Edit Recording");

    connect(device.get(), QOverload<QSharedPointer<client::Recording>&>::of(&Device::SignalRecordingChanged), this, &ManageRecordingWindow::RecordingChanged);
    connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalRecordingRemoved), this, &ManageRecordingWindow::RecordingRemoved);

    QSharedPointer<client::Recording> recording = device_->GetRecording(*token_);
    ui_.editname->setText(recording->GetName());
    ui_.editlocation->setText(recording->GetLocation());
    ui_.spinretentiontime->setValue(recording->GetRetentionTime() / (86400 * 1000));

    ui_.checkcreatedefaulttracks->setHidden(true);
    ui_.checkcreatedefaultjob->setHidden(true);
    const QSize sizehint = sizeHint();
    if (sizehint.isValid())
    {
      resize(width(), sizehint.height());
      setMinimumHeight(sizehint.height());
      setMaximumHeight(sizehint.height());
    }
  }
}

ManageRecordingWindow::~ManageRecordingWindow()
{
  recordingconnection_.Close();

}

void ManageRecordingWindow::SetEnabled(const bool enabled)
{
  ui_.editname->setEnabled(enabled);
  ui_.editlocation->setEnabled(enabled);
  ui_.spinretentiontime->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageRecordingWindow::RecordingChanged(QSharedPointer<client::Recording>& recording)
{
  if (token_.is_initialized() && (*token_ == recording->GetToken()))
  {
    // Ignore the change if it is identical to what is currently selected
    if ((ui_.editname->text() == recording->GetName()) && (ui_.editlocation->text() == recording->GetLocation()) && (recording->GetRetentionTime() == ui_.spinretentiontime->value()))
    {

      return;
    }

    if (QMessageBox::question(this, tr("Recording Changed"), tr("Recording has been changed, Would you like to update with new values?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
      ui_.editname->setText(recording->GetName());
      ui_.editlocation->setText(recording->GetLocation());
      ui_.spinretentiontime->setValue(recording->GetRetentionTime());
    }
  }
}

void ManageRecordingWindow::RecordingRemoved(const uint64_t token)
{
  if (token_.is_initialized() && (*token_ == token))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Recording has been removed"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    reject();
  }
}

void ManageRecordingWindow::on_buttonok_clicked()
{
  if (ui_.editname->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Please enter a name for this Recording"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  SetEnabled(false);
  const uint64_t retentiontime = ui_.spinretentiontime->value() * 86400 * 1000;
  if (token_.is_initialized())
  {
    QSharedPointer<client::Recording> recording = device_->GetRecording(*token_);
    if (!recording)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error unable to retrieve recording: ") + QString::number(*token_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    recordingconnection_ = device_->ChangeRecording(*token_, recording->GetSourceId().toStdString(), ui_.editname->text().toStdString(), ui_.editlocation->text().toStdString(), recording->GetDescription().toStdString(), recording->GetAddress().toStdString(), recording->GetContent().toStdString(), retentiontime, [this](const std::chrono::nanoseconds latency, const monocle::client::CHANGERECORDINGRESPONSE& changerecordingresponse)
    {
      SetEnabled(true);
      if (changerecordingresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(changerecordingresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
  else
  {
    recordingconnection_ = device_->AddRecording(std::string(), ui_.editname->text().toStdString(), ui_.editlocation->text().toStdString(), std::string(), std::string(), std::string(), retentiontime, ui_.checkcreatedefaulttracks->isChecked(), ui_.checkcreatedefaultjob->isChecked(), [this](const std::chrono::nanoseconds latency, const monocle::client::ADDRECORDINGRESPONSE& addrecordingresponse)
    {
      SetEnabled(true);
      if (addrecordingresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(addrecordingresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
}

}

QT_END_NAMESPACE
