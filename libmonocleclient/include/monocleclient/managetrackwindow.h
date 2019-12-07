// managetrackwindow.h
//

#ifndef ID2VV1211212SADSADASDASDDD1212DN9G
#define ID2VV1211212SADSADASDASDDD1212DN9G

///// Includes /////

#include "ui_managetrackwindow.h"

#include <boost/shared_ptr.hpp>
#include <monocleprotocol/client/connection.hpp>
#include <QSharedPointer>

///// Declarations /////

namespace monocle { enum class TrackType : int8_t; }

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{
	
///// Declarations /////

class Device;
class Recording;

///// Classes /////

class ManageTrackWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageTrackWindow(QWidget* parent, const monocle::TrackType tracktype, boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording);
  ~ManageTrackWindow();

 private:

  void DisableSource();

  Ui::ManageTrackWindow ui_;

  monocle::TrackType tracktype_;

  boost::shared_ptr<Device> device_;
  QSharedPointer<Recording> recording_;

  monocle::client::Connection addtrack2connection_;

 private slots:

  void on_edituri_textChanged(const QString& text);
  void on_checkfixedfiles_stateChanged(int);
  void on_checkobjectdetector_stateChanged(int);
  void on_buttonfindonvifdevice_clicked();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
