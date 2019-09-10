// managerecordingwindow.h
//

#ifndef ID2VVCASASASDJKADLASDASLDKSAWKDN9G
#define ID2VVCASASASDJKADLASDASLDKSAWKDN9G

///// Includes /////

#include <boost/optional.hpp>
#include <monocleprotocol/client/connection.hpp>
#include <QDialog>
#include <QSharedPointer>

#include "ui_managerecordingwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Recording;

///// Classes /////

class ManageRecordingWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageRecordingWindow(QWidget* parent, boost::shared_ptr<Device>& device, const boost::optional<uint64_t>& token);
  ~ManageRecordingWindow();

 private:

  void SetEnabled(const bool enabled);

  Ui::ManageRecordingWindow ui_;

  boost::shared_ptr<Device> device_;

  const boost::optional<uint64_t> token_;

  monocle::client::Connection recordingconnection_;

 private slots:

  void RecordingChanged(QSharedPointer<client::Recording>& recording);
  void RecordingRemoved(const uint64_t token);
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
