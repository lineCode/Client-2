// managedevicewindow.h
//

#ifndef IDE2STN3SDFJSDKFJSDFDSFSXXCP3IJ2T0
#define IDE2STN3SDFJSDKFJSDFDSFSXXCP3IJ2T0

///// Includes /////

#include <QDialog>
#include <QSharedPointer>
#include <QString>
#include <QStringList>

#include "monocleclient/device.h"
#include "ui_managedevicewindow.h"

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;

///// Classes /////

class ManageDeviceWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageDeviceWindow(QWidget* parent, const boost::shared_ptr<Device>& device);
  ~ManageDeviceWindow();
  
 protected:


 private:

  void SetEnabled(const bool enabled);
  void Finished(const QStringList& errors);

  Ui::ManageDeviceWindow ui_;

  boost::shared_ptr<Device> device_;

  monocle::client::Connection setnameconnection_;
  monocle::client::Connection setlocationconnection_;

 private slots:

  void on_buttonok_clicked();

};

}

#endif
