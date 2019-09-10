// updatewindow.cpp
//

///// Includes /////

#include "monocleclient/updatewindow.h"

#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QUrl>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Globals /////

const QString INSTALL("Install");

///// Methods /////

UpdateWindow::UpdateWindow(QWidget* parent, const utility::Version& currentversion) :
  QDialog(parent),
  currentversion_(currentversion),
  manager_(new QNetworkAccessManager(this)),
  checkforupdate_(currentversion)
{
  ui_.setupUi(this);
  
  connect(manager_, &QNetworkAccessManager::finished, this, &UpdateWindow::FinishedDownload);
  connect(&checkforupdate_, &CheckForUpdate::UpdateAvailable, this, &UpdateWindow::UpdateAvailable);
  
  ui_.buttondownload->setEnabled(false);

  checkforupdate_.Check();
}

UpdateWindow::~UpdateWindow()
{
  
}

QString UpdateWindow::InstallerPath() const
{
  return QDir::tempPath() + QString("/monocle_") + QString::number(latestversion_.version_.GetMajor()) + QString("_") + QString::number(latestversion_.version_.GetMinor()) + QString("_") + QString::number(latestversion_.version_.GetBuild()) + QString(".msi");
}

void UpdateWindow::on_buttoncancel_clicked()
{
  if (ui_.progressdownload->value() && ui_.buttondownload->isEnabled())
  {
    if (QMessageBox::question(this, tr("Cancel Install"), tr("Are you sure?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes) != QMessageBox::Yes)
    {
    
      return;
    }
  }

  reject();
}

void UpdateWindow::on_buttondownload_clicked()
{
  ui_.buttondownload->setEnabled(false);
  
  if (ui_.buttondownload->text() != INSTALL)
  {
    QNetworkReply* reply = manager_->get(QNetworkRequest(latestversion_.url_));
    connect(reply, &QNetworkReply::downloadProgress, this, &UpdateWindow::ProgressDownload);
    ui_.progressdownload->setValue(0);
    ui_.progressdownload->setMaximum(0);
  }
  else
  {
    // Open the installer
    if (!QDesktopServices::openUrl(QUrl("file:///" + InstallerPath())))
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to start installer"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    QCoreApplication::quit(); // Installer requires us to quit
  }
}

void UpdateWindow::ProgressDownload(qint64 received, qint64 total)
{
  if (total == -1)
  {
    
    return;
  }

  if (ui_.progressdownload->format().isEmpty())
  {
    ui_.progressdownload->setFormat(QString("%v/%mB"));
  
  }

  ui_.progressdownload->setValue(received);
  ui_.progressdownload->setMaximum(total);
}

void UpdateWindow::FinishedDownload(QNetworkReply* networkreply)
{
  networkreply->deleteLater();
  
  ui_.buttondownload->setEnabled(true);

  // Error checking
  if (networkreply->error())
  {
    QMessageBox::warning(this, tr("Error"), QString(tr("Failed to download update")), QMessageBox::Ok);
    return;
  }
  
  // Write the installer to the temp directory
  const QString filepath = InstallerPath();
  QFile file(filepath);
  if (!file.open(QIODevice::WriteOnly))
  {
    QMessageBox::warning(this, tr("Error"), QString(tr("Failed to open file: ") + filepath), QMessageBox::Ok);
    return;
  }
  
  if (file.write(networkreply->readAll()) == -1)
  {
    QMessageBox::warning(this, tr("Error"), QString(tr("Failed to open file: ") + filepath), QMessageBox::Ok);
    return;
  }

  file.close();

  ui_.buttondownload->setText(INSTALL);
}

void UpdateWindow::UpdateAvailable(bool updateavailable, const std::vector<UpdateVersion>& versions, const UpdateVersion& latestversion)
{
  if (!updateavailable)
  {
    ui_.labelinfo->setText(QString("No update available"));
    return;
  }

  latestversion_ = latestversion;

  ui_.labelinfo->setText(QString("Version ") + QString::fromStdString(latestversion_.version_.ToString()) + QString(" is available"));
#ifdef _WIN32
  ui_.buttondownload->setEnabled(true);
#endif

  QString changelist;
  for (const UpdateVersion& version : versions)
  {
    // Don't show changes from previous versions to the one we already are
    if (version.version_ <= currentversion_)
    {
      
      continue;
    }

    changelist += (QString("<h3>Version ") + QString::fromStdString(version.version_.ToString()) + QString(" - ") + version.date_.toString() + QString("</h3>"));
    changelist += QString("<ul>");
    for (const QString& change : version.changelist_)
    {
      changelist += QString("<li>") + change + QString("</li>");

    }
    changelist += QString("</ul>");
  }
  ui_.textchangelist->setHtml(changelist);
}

}
