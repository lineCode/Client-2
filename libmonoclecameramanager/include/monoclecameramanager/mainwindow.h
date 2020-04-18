// mainwindow.h
//

#ifndef ID0S1Q2OOFASDSADASSAZLU9WKGR3IPTHW
#define ID0S1Q2OOFASDSADASSAZLU9WKGR3IPTHW

///// Includes /////

#include <QMainWindow>

#include "networkmapper.h"

#include "ui_mainwindow.h"

///// Declarations /////

class QTreeWidgetItem;

///// Defines /////

namespace cm
{

///// Declarations /////

///// Typedefs /////

///// Structures /////

///// Classes /////

class MainWindow : public QMainWindow
{
 Q_OBJECT

 public:

  MainWindow();
  ~MainWindow();

 protected:


 private:

  Ui::MainWindow ui_;

  NetworkMapper networkmapper_;

 private slots:

  void on_buttonadd_clicked();
  void on_buttonrefresh_clicked();
  void on_devicetree_itemClicked(QTreeWidgetItem* item, int column);
  void DiscoverONVIFDevice(const QString& uri);
  void DiscoverProgress(const int progress);

};

}

#endif
