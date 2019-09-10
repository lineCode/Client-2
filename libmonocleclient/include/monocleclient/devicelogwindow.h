// viewlogwindow.h
//

#ifndef IDSFFATNSDFJSD234087324DSFO5O5OPBW
#define IDSFFATNSDFJSD234087324DSFO5O5OPBW

///// Includes /////

#include <monocleprotocol/client/connection.hpp>
#include <monocleprotocol/monocleprotocol.hpp>
#include <QDialog>
#include <QSharedPointer>
#include <vector>

#include "ui_devicelogwindow.h"

///// Declarations /////

namespace monocle { enum class Severity : int8_t; }

///// Namespaces /////

namespace client
{

///// Declarations ///// 

class Device;

///// Classes /////

class DeviceLogWindow : public QDialog
{
 Q_OBJECT

 public:

  DeviceLogWindow(QWidget* parent, const std::vector< boost::shared_ptr<Device> >& devices);
  ~DeviceLogWindow();
  

 protected:



 private:

  void Update();
  bool Filter(const monocle::Severity severity) const;
  QString HTML(const monocle::Severity severity, const QString& message) const;

  Ui::DeviceLogWindow ui_;

  std::vector< boost::shared_ptr<Device> > devices_;

 private slots:

  void ServerLogMessage(const uint64_t time, const monocle::Severity severity, const QString& message);

  void on_checktrace_clicked();
  void on_checkdebug_clicked();
  void on_checkinfo_clicked();
  void on_checkwarning_clicked();
  void on_checkerror_clicked();
  void on_checkcritical_clicked();
  void on_buttoncopy_clicked();
  void on_buttonok_clicked();

};

}

#endif
