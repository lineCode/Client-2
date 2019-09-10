// exportwindow.cpp
//

///// Includes /////

#include "monocleclient/exportwindow.h"

#include <boost/filesystem.hpp>
#include <QFileDialog>
#include <QMessageBox>

#include "monocleclient/devicemgr.h"
#include "monocleclient/exportprogresswindow.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/options.h"
#include "monocleclient/recording.h"
#include "monocleclient/savevideo.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ExportWindow::ExportWindow(QWidget* parent, const std::vector< QSharedPointer<client::Recording> >& recordings, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime) :
  QDialog(parent)
{
  ui_.setupUi(this);
  
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::accept);
  connect(&MainWindow::Instance()->GetDeviceMgr(), &DeviceMgr::DeviceAdded, this, &ExportWindow::DeviceAdded);

  ui_.comboformat->insertItem(0, ToString(EXPORTFORMAT_MONOCLE));
  ui_.comboformat->insertItem(0, ToString(EXPORTFORMAT_AVI));
  
  for (const QSharedPointer<client::Recording>& recording : recordings)
  {
    QListWidgetItem* item = new QListWidgetItem(recording->GetName() + "(" + QString::number(recording->GetToken()) + ")");
    item->setData(Qt::UserRole, QVariant::fromValue(recording));
    ui_.listrecordings->addItem(item);
  }

  if (starttime.is_initialized())
  {
    ui_.datetimestart->setDateTime(QDateTime::fromMSecsSinceEpoch(*starttime, Qt::UTC));

  }
  else
  {
    boost::optional<uint64_t> starttime;
    for (const QSharedPointer<client::Recording>& recording : recordings)
    {
      for (const QSharedPointer<client::RecordingTrack>& track : recording->GetVideoTracks())
      {
        if (track->GetIndices().size())
        {
          if (starttime.is_initialized())
          {
            starttime = std::min(*starttime, track->GetIndices().front().first);

          }
          else
          {
            starttime = track->GetIndices().front().first;

          }
        }
      }
    }

    if (starttime.is_initialized())
    {
      ui_.datetimestart->setDateTime(QDateTime::fromMSecsSinceEpoch(*starttime, Qt::UTC));

    }
  }

  if (endtime.is_initialized())
  {
    ui_.datetimeend->setDateTime(QDateTime::fromMSecsSinceEpoch(*endtime, Qt::UTC));

  }
  else
  {
    boost::optional<uint64_t> endtime;
    for (const QSharedPointer<client::Recording>& recording : recordings)
    {
      for (const QSharedPointer<client::RecordingTrack>& track : recording->GetVideoTracks())
      {
        if (track->GetIndices().size())
        {
          if (endtime.is_initialized())
          {
            endtime = std::min(*endtime, track->GetIndices().back().first);

          }
          else
          {
            endtime = track->GetIndices().back().first;

          }
        }
      }
    }

    if (endtime.is_initialized())
    {
      ui_.datetimeend->setDateTime(QDateTime::fromMSecsSinceEpoch(*endtime, Qt::UTC));

    }
  }

  ui_.editdirectory->setText(Options::Instance().GetExportDirectory());
}

ExportWindow::~ExportWindow()
{

}

void ExportWindow::on_buttondirectory_clicked()
{
  const QString directory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), ui_.editdirectory->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (directory.isEmpty())
  {

    return;
  }
  ui_.editdirectory->setText(directory);
}

void ExportWindow::on_buttonexport_clicked()
{
  const boost::optional<EXPORTFORMAT> exportformat = GetExportFormat(ui_.comboformat->currentText());
  if (!exportformat.is_initialized())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Invalid export format"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (ui_.datetimestart->dateTime() > ui_.datetimeend->dateTime())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("End time must be greater than start time"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  std::vector< QSharedPointer<client::Recording> > recordings;
  for (int row = 0; row < ui_.listrecordings->count(); row++)
  {
    QListWidgetItem* item = ui_.listrecordings->item(row);
    QSharedPointer<client::Recording> recording = item->data(Qt::UserRole).value< QSharedPointer<client::Recording> >();
    recordings.push_back(recording);
  }

  if (recordings.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No recordings selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  const QDir dir(Options::Instance().GetExportDirectory());
  if (!dir.exists())
  {
    boost::system::error_code err;
    boost::filesystem::create_directory(dir.path().toStdString(), err);
    if (err)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to create directory: ") + dir.path(), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  }
  
  Options::Instance().SetExportDirectory(ui_.editdirectory->text());
  QDateTime starttime = ui_.datetimestart->dateTime();
  QDateTime endtime = ui_.datetimeend->dateTime();
  if (ExportProgressWindow(this, ui_.editdirectory->text(), *exportformat, starttime.toMSecsSinceEpoch(), endtime.toMSecsSinceEpoch(), recordings).exec() == QDialog::Accepted)
  {
    accept();
    return;
  }
}

void ExportWindow::DeviceAdded(const boost::shared_ptr<Device>& device)
{
  std::vector<QListWidgetItem*> items;
  for (int row = 0; row < ui_.listrecordings->count(); row++)
  {
    QSharedPointer<client::Recording> recording = ui_.listrecordings->item(row)->data(Qt::UserRole).value< QSharedPointer<client::Recording> >();
    if (recording->GetDevice() == device)
    {
      items.push_back(ui_.listrecordings->takeItem(row));

    }
  }

  for (QListWidgetItem* item : items)
  {
    delete ui_.listrecordings->takeItem(ui_.listrecordings->row(item));

  }
}

}

QT_END_NAMESPACE
