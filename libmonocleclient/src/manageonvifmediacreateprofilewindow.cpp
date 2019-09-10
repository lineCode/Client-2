// manageonvifmediacreateprofilewindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediacreateprofilewindow.h"

#include <onvifclient/mediaclient.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QApplication>
#include <QMessageBox>
#include <QPushButton>

///// Namespaces /////

namespace client
{

///// Methods /////

ManageONVIFMediaCreateProfileWindow::ManageONVIFMediaCreateProfileWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia) :
  QDialog(parent),
  onvifmedia_(onvifmedia)
{
  ui_.setupUi(this);
  
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);
}

ManageONVIFMediaCreateProfileWindow::~ManageONVIFMediaCreateProfileWindow()
{
  if (createprofileconnection_.IsConnected())
  {
    QApplication::restoreOverrideCursor();

  }
}

void ManageONVIFMediaCreateProfileWindow::on_buttonok_clicked()
{
  if (ui_.editname->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Error"), tr("Profile requires a name"), QMessageBox::Ok);
    return;
  }

  boost::optional<std::string> token;
  if (!ui_.edittoken->text().isEmpty())
  {
    token = ui_.edittoken->text().toStdString();

  }

  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui_.buttonok->setEnabled(false);
  ui_.buttoncancel->setEnabled(false);
  
  createprofileconnection_ = onvifmedia_->CreateProfileCallback(ui_.editname->text().toStdString(), token, [this](const onvif::media::CreateProfileResponse& createprofileresponse)
  {
    QApplication::restoreOverrideCursor();
    ui_.buttonok->setEnabled(true);
    ui_.buttoncancel->setEnabled(true);

    if (createprofileresponse.Error())
    {
      QMessageBox::warning(this, tr("Unable to create profile"), QString::fromStdString(createprofileresponse.Message()), QMessageBox::Ok);
      return;
    }

    profile_ = createprofileresponse.profile_;

    accept();
  });
}

}
