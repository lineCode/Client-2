// managerecordingswindow.h
//

#ifndef ID2VVCASDASDHASMDNJSKDHLAJD9WKDN9G
#define ID2VVCASDASDHASMDNJSKDHLAJD9WKDN9G

///// Includes /////

#include <monocleprotocol/client/connection.hpp>
#include <QDialog>
#include <QSharedPointer>

#include "ui_managerecordingswindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Recording;

///// Classes /////

class ManageRecordingsWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageRecordingsWindow(QWidget* parent, boost::shared_ptr<Device>& device);
  ~ManageRecordingsWindow();

 private:

  void AddRecording(const QSharedPointer<client::Recording>& recording);
  QString ToString(const uint64_t retentiontime) const;

  Ui::ManageRecordingsWindow ui_;

  boost::shared_ptr<Device> device_;

  monocle::client::Connection removerecordingconnection_;

 private slots:

  void RecordingAdded(QSharedPointer<client::Recording>& recording);
  void RecordingChanged(QSharedPointer<client::Recording>& recording);
  void RecordingRemoved(const uint64_t token);
  void on_tablerecordings_itemSelectionChanged();
  void on_buttonadd_clicked();
  void on_buttonedit_clicked();
  void on_buttonmanagetracks_clicked();
  void on_buttonmanagerecordingjobs_clicked();
  void on_buttonremove_clicked();


};

}

QT_END_NAMESPACE

#endif
