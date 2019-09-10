// aboutwindow.cpp
//

///// Includes /////

#include "monocleclient/aboutwindow.h"

#include <utility/utility.hpp>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

AboutWindow::AboutWindow(QWidget* parent, const utility::Version& version) :
  QDialog(parent)
{
  ui_.setupUi(this);
  
  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);
  
  setWindowTitle(tr("About Monocle Client ") + QString::fromStdString(version.ToString()));

  const QString text = tr("Built with: ") + QString::fromStdString(utility::GetCompiler()) + QString(" ") + QString::fromStdString(utility::GetArchitecture()) + tr("\nDate built: ") + QString(__DATE__) + QString(" ") + QString(__TIME__);
  ui_.labelabout->setText(text);
}

AboutWindow::~AboutWindow()
{

}

}

QT_END_NAMESPACE
