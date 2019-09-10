// managerecordingjobswindow.h
//

#ifndef ID2VVCSDASDASDASDSADSADAAJD9WKDN9G
#define ID2VVCSDASDASDASDSADSADAAJD9WKDN9G

///// Includes /////

#include <monocleprotocol/client/connection.hpp>
#include <QDialog>
#include <QSharedPointer>

#include "ui_managerecordingjobswindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Recording;
class RecordingJob;

///// Classes /////

class ManageRecordingJobsWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageRecordingJobsWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording);
  ~ManageRecordingJobsWindow();

 private:

  void AddRecordingJob(const QSharedPointer<client::RecordingJob>& recordingjob);

  Ui::ManageRecordingJobsWindow ui_;

  const boost::shared_ptr<Device> device_;
  const QSharedPointer<client::Recording> recording_;

  monocle::client::Connection removerecordingjobconnection_;

 private slots:

  void RecordingJobAdded(const QSharedPointer<client::RecordingJob>& recordingjob);
  void RecordingJobChanged(const QSharedPointer<client::RecordingJob>& recordingjob);
  void RecordingJobRemoved(const uint64_t token);
  void on_tablerecordingjobs_itemSelectionChanged();
  void on_buttonadd_clicked();
  void on_buttonedit_clicked();
  void on_buttonremove_clicked();


};

}

QT_END_NAMESPACE

#endif
