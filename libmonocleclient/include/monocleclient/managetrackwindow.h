// managetrackwindow.h
//

#ifndef ID2VV1211212SADSADASDASDDD1212DN9G
#define ID2VV1211212SADSADASDASDDD1212DN9G

///// Includes /////

#include "ui_managetrackwindow.h"

#include <boost/shared_ptr.hpp>
#include <QSharedPointer>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{
	
///// Declarations /////

class Device;

///// Classes /////

class ManageTrackWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageTrackWindow(QWidget* parent, boost::shared_ptr<Device>& device);
  ~ManageTrackWindow();

 private:

  Ui::ManageTrackWindow ui_;

  boost::shared_ptr<Device> device_;

 private slots:

};

}

QT_END_NAMESPACE

#endif
