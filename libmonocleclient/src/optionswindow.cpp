// optionswindow.cpp
//

///// Includes /////

#include "monocleclient/optionswindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "monocleclient/infoformatwindow.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/options.h"

///// Namespaces /////

namespace client
{

///// Methods /////

OptionsWindow::OptionsWindow(QWidget* parent) :
  QDialog(parent)
{
  ui_.setupUi(this);
  
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);
  
  // UI
  ui_.spindefaultvideowindowwidth->setMaximum(MAXVIDEOWINDOWWIDTH);
  ui_.spindefaultvideowindowheight->setMaximum(MAXVIDEOWINDOWHEIGHT);
  ui_.spindefaultvideowindowwidth->setValue(Options::Instance().GetDefaultVideoWindowWidth());
  ui_.spindefaultvideowindowheight->setValue(Options::Instance().GetDefaultVideoWindowHeight());
  ui_.spinupdatefrequency->setValue(Options::Instance().GetUpdateFrequency());
  ui_.checktoolbar->setCheckState(Options::Instance().GetDefaultShowToolbar() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  ui_.checkcheckforupdateonstartup->setCheckState(Options::Instance().GetCheckForUpdateOnStartup() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  ui_.checkshowtokens->setCheckState(Options::Instance().GetShowTokens() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  ui_.checkhidemediaclosedialog->setCheckState(Options::Instance().GetHideMediaCloseDialog() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  ui_.checkhidevideowindowclosedialog->setCheckState(Options::Instance().GetHideVideoWindowCloseDialog() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  ui_.checkhidemainwindowclosedialog->setCheckState(Options::Instance().GetHideMainWindowCloseDialog() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

  // Streaming
  ui_.editinfotextformat->setText(QString::fromStdString(Options::Instance().GetInfoTextFormat()));
  ui_.checkstretchvideo->setCheckState(Options::Instance().GetStretchVideo() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  ui_.checkshowinfo->setCheckState(Options::Instance().GetShowInfo() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  ui_.checkshowobjects->setCheckState(Options::Instance().GetShowObjects() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  ui_.spinmaxcudadecodersperdevice->setValue(Options::Instance().GetMaxCUDADecodersPerDevice());

  // Tray
  ui_.checkminimisetotray->setCheckState(Options::Instance().GetMinimiseToTray() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  ui_.checkalwaysshowtray->setCheckState(Options::Instance().GetAlwaysShowTray() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

  // Directory
  ui_.editbackupdirectory->setText(Options::Instance().GetBackupDirectory());
}

OptionsWindow::~OptionsWindow()
{

}

void OptionsWindow::on_buttoninfotexteditor_clicked()
{
  InfoFormatWindow dlg(this, ui_.editinfotextformat->text());
  if (dlg.exec() == QDialog::Accepted)
  {
    ui_.editinfotextformat->setText(QString::fromStdString(dlg.GetInfoTextFormat()));

  }
}

void OptionsWindow::on_editbackupdirectorybrowse_clicked()
{
  const QString directory = QFileDialog::getExistingDirectory(this, QString("Backup Directory"), ui_.editbackupdirectory->text(), QFileDialog::ShowDirsOnly);
  if (directory.isEmpty())
  {

    return;
  }

  ui_.editbackupdirectory->setText(directory);
}

void OptionsWindow::on_buttonok_clicked()
{
  // Directories, this comes first because it asks a question and can cancel the process
  if (!QDir(ui_.editbackupdirectory->text()).exists())
  {
    if (QMessageBox::question(this, QString("Warning"), QString("Backup directory does not exist: ") + ui_.editbackupdirectory->text() + QString(". Create?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
      if (!QDir().mkpath(ui_.editbackupdirectory->text()))
      {
        QMessageBox::warning(this, QString("Error"), QString("Unable to create directory: ") + ui_.editbackupdirectory->text(), QMessageBox::Ok);
        return;
      }
    }
    else
    {

      return;
    }
  }

  MainWindow::Instance()->GetVideoWidgetsMgr().SetUpdateFrequency(ui_.spinupdatefrequency->value());
  MainWindow::Instance()->GetVideoWidgetsMgr().ResetViews();

  // UI
  Options::Instance().SetDefaultVideoWindowWidth(ui_.spindefaultvideowindowwidth->value());
  Options::Instance().SetDefaultVideoWindowHeight(ui_.spindefaultvideowindowheight->value());
  Options::Instance().SetUpdateFrequency(ui_.spinupdatefrequency->value());
  Options::Instance().SetDefaultShowToolbar(ui_.checktoolbar->isChecked());
  Options::Instance().SetCheckForUpdateOnStartup(ui_.checkcheckforupdateonstartup->isChecked());
  Options::Instance().SetShowTokens(ui_.checkshowtokens->isChecked());
  Options::Instance().SetHideMediaCloseDialog(ui_.checkhidemediaclosedialog->isChecked());
  Options::Instance().SetHideVideoWindowCloseDialog(ui_.checkhidevideowindowclosedialog->isChecked());
  Options::Instance().SetHideMainWindowCloseDialog(ui_.checkhidemainwindowclosedialog->isChecked());

  // Streaming
  Options::Instance().SetInfoTextFormat(ui_.editinfotextformat->text().toStdString());
  Options::Instance().SetStretchVideo(ui_.checkstretchvideo->isChecked());
  Options::Instance().SetShowInfo(ui_.checkshowinfo->isChecked());
  Options::Instance().SetShowObjects(ui_.checkshowobjects->isChecked());
  Options::Instance().SetMaxCUDADecodersPerDevice(ui_.spinmaxcudadecodersperdevice->value());

  // Tray
  Options::Instance().SetMinimiseToTray(ui_.checkminimisetotray->isChecked());
  Options::Instance().SetAlwaysShowTray(ui_.checkalwaysshowtray->isChecked());

  // Directories
  Options::Instance().SetBackupDirectory(ui_.editbackupdirectory->text());

  MainWindow::Instance()->UpdateTray();

  Options::Instance().Save();

  accept();
}

}
