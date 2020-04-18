// mainwindow.cpp
//

///// Includes /////

#include "monoclecameramanager/mainwindow.h"

#include <QProgressBar>
#include <QStatusBar>
#include <QDebug>//TODO remove

#include "monoclecameramanager/devicetreeonvifitem.h"

///// Defines /////


///// Namespaces /////

namespace cm
{

///// Declarations /////


///// Globals /////


///// Methods /////

MainWindow::MainWindow()
{
  ui_.setupUi(this);

  connect(&networkmapper_, &NetworkMapper::DiscoverONVIFDevice, this, &MainWindow::DiscoverONVIFDevice);
  connect(&networkmapper_, &NetworkMapper::Progress, this, &MainWindow::DiscoverProgress);

  //TODO load manually added devices through QSettings
    //TODO need to remember uri and username/password

  //TODO tools->options
    //TODO enable/disable linklocal
    //TODO choose which http ports to search for
    //TODO choose which https ports to search for

  on_buttonrefresh_clicked();
}

//TODO right click menu to the devices themselves(ability to select many devices)
  //TODO Hack button to try usernames:passwords for a while
  //TODO Should do basics like GetCapabilities and GetProfiles and GetDeviceInformation etc
  //TODO check for update from local list(json file)... how do we display this?

MainWindow::~MainWindow()
{

}

void MainWindow::on_buttonadd_clicked()
{

  //TODO need an add button for manual entry of ip/uri

}

void MainWindow::on_buttonrefresh_clicked()
{
  for (int i = (ui_.devicetree->topLevelItemCount() - 1); i >= 0; --i)
  {
    //TODO don't delete items we have manually added... probably have a bool in these
    delete ui_.devicetree->takeTopLevelItem(i);

  }

  networkmapper_.Init();
  ui_.progressbar->setValue(0);
}

void MainWindow::on_devicetree_itemClicked(QTreeWidgetItem* item, int column)
{

}

//TODO listen for clicking items on the device tree, and depending on the item we need to update the widget displaying shit

void MainWindow::DiscoverONVIFDevice(const QString& uri)
{
  ui_.devicetree->addTopLevelItem(new DeviceTreeONVIFItem(ui_.devicetree, uri, QString(), QString()));

}

void MainWindow::DiscoverProgress(const int progress)
{
  ui_.progressbar->setValue(progress);

}

}
