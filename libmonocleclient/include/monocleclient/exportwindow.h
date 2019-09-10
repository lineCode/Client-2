// exportwindow.h
//

#ifndef ID2VVC4FDKFJNSDJFFNOB4QAES19WKDN9G
#define ID2VVC4FDKFJNSDJFFNOB4QAES19WKDN9G

///// Includes /////

#include <boost/optional.hpp>
#include <memory>
#include <QDialog>
#include <vector>

#include "ui_exportwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Recording;

///// Classes /////

class ExportWindow : public QDialog
{
 Q_OBJECT

 public:

  ExportWindow(QWidget* parent, const std::vector< QSharedPointer<client::Recording> >& recordings, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime);
  virtual ~ExportWindow();

 private:

  Ui::ExportWindow ui_;


 public slots:

  void on_buttondirectory_clicked();
  void on_buttonexport_clicked();

 private slots:

  void DeviceAdded(const boost::shared_ptr<Device>& device);


};

}

QT_END_NAMESPACE

#endif
