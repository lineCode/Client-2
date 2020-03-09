// newcameraquestionwindow.cpp
//

///// Includes /////

#include "monocleclient/newcameraquestionwindow.h"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

NewCameraQuestionWindow::NewCameraQuestionWindow(QWidget* parent, const QString& address, const std::vector<std::string>& scopes) :
  QDialog(parent),
  donotaskdevice_(false),
  donotask_(false)
{
  ui_.setupUi(this);

  ui_.labeltext->setText(tr("Would you like to add this camera to a Monocle server?"));

  QString extensiontext;
  const std::vector<std::string>::const_iterator name = std::find_if(scopes.cbegin(), scopes.cend(), [](const std::string& scope) { return boost::starts_with(scope, "onvif://www.onvif.org/name/"); });
  if (name != scopes.cend())
  {
    extensiontext += " " + QString::fromStdString(name->substr(27));

  }

  const std::vector<std::string>::const_iterator hardware = std::find_if(scopes.cbegin(), scopes.cend(), [](const std::string& scope) { return boost::starts_with(scope, "onvif://www.onvif.org/hardware/"); });
  if (name != scopes.cend())
  {
    extensiontext += " " + QString::fromStdString(hardware->substr(31));

  }

  setWindowTitle(tr("New Camera Detected: ") + address + extensiontext);
}

NewCameraQuestionWindow::~NewCameraQuestionWindow()
{

}

void NewCameraQuestionWindow::on_buttonyes_clicked()
{
  donotaskdevice_ = ui_.checkdonotaskdevice->isChecked();
  donotask_ = ui_.checkdonotask->isChecked();
  accept();
}

void NewCameraQuestionWindow::on_buttonno_clicked()
{
  donotaskdevice_ = ui_.checkdonotaskdevice->isChecked();
  donotask_ = ui_.checkdonotask->isChecked();
  reject();
}

}

QT_END_NAMESPACE
