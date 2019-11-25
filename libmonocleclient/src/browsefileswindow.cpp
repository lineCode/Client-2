// browsefileswindow.cpp
//

///// Includes /////

#include "monocleclient/browsefileswindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

BrowseFilesWindow::BrowseFilesWindow(QWidget* parent) :
  QDialog(parent)
{
  ui_.setupUi(this);
  
  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);
}

BrowseFilesWindow::~BrowseFilesWindow()
{

}

}

QT_END_NAMESPACE
