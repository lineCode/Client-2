// managetrackwindow.cpp
//

///// Includes /////

#include "monocleclient/managetrackwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageTrackWindow::ManageTrackWindow(QWidget* parent, boost::shared_ptr<Device>& device) :
  QDialog(parent),
  device_(device)
{
  ui_.setupUi(this);

}

ManageTrackWindow::~ManageTrackWindow()
{

}

}

QT_END_NAMESPACE
