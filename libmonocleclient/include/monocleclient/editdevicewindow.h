// editdevicewindow.h
//

#ifndef IDE2STN3CRCJIACWC1XWS1PJSCCP3IJ2T0
#define IDE2STN3CRCJIACWC1XWS1PJSCCP3IJ2T0

///// Includes /////

#include <QDialog>
#include <QSharedPointer>
#include <QString>

#include "monocleclient/device.h"
#include "ui_editdevicewindow.h"

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;

///// Classes /////

class EditDeviceWindow : public QDialog
{
 Q_OBJECT

 public:

  EditDeviceWindow(QWidget* parent, const boost::shared_ptr<Device>& device);
  EditDeviceWindow(QWidget* parent, const QString& address, const uint16_t port, const QString& username, const QString& password);
  virtual ~EditDeviceWindow();
  
 protected:


 private:

  void EnableProxy(bool enable);

  void SetConnectResult(const QString& text);

  Ui::EditDeviceWindow ui_;

  boost::shared_ptr<Device> device_;

  boost::shared_ptr<Device> testdevice_;

  sock::Connection connectconnection_;
  monocle::client::Connection monocleconnection_;

 private slots:

  void DiscoveryTreeItemClicked(QTreeWidgetItem* item, int);

  void on_checkipv4_clicked();
  void on_checkipv6_clicked();
  void on_checklinklocal_clicked();
  void on_buttondiscoverrefresh_clicked();
  void on_comboproxytype_currentIndexChanged(int index);
  void on_buttontest_clicked();
  void on_buttonok_clicked();

};

}

#endif
