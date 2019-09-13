// managerecordingjobwindow.h
//

#ifndef ID2VV234DFSDFDFDFDFDFDFDFKSAWKDN9G
#define ID2VV234DFSDFDFDFDFDFDFDFKSAWKDN9G

///// Includes /////

#include <monocleprotocol/client/connection.hpp>
#include <QSharedPointer>

#include "ui_managerecordingjobwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class File;
class Receiver;
class Recording;
class RecordingJob;
class RecordingJobSource;
class RecordingJobSourceTrack;

///// Classes /////

class ManageRecordingJobWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageRecordingJobWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack);
  ~ManageRecordingJobWindow();

 private:

  void SetEnabled(const bool enabled);
  void Setup(const QSharedPointer<client::RecordingJob>& recordingjob);
  void AddRecordingJobSourceTrack(const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack);
  std::vector<uint32_t> GetTrackIds() const;
  bool SelectItem(const uint64_t token);
  std::vector<std::string> GetParameters(const int row) const;

  Ui::ManageRecordingJobWindow ui_;

  boost::shared_ptr<Device> device_;
  QSharedPointer<client::Recording> recording_;
  QSharedPointer<client::RecordingJob> recordingjob_;

  monocle::client::Connection recordingjobconnection_;

 private slots:

  void RecordingJobRemoved(const uint64_t token);

  void on_tablerecordingjobsources_itemSelectionChanged();
  void on_buttonaddrtspsource_clicked();
  void on_buttonaddonvifsource_clicked();
  void on_buttonaddobjectdetector_clicked();
  void on_buttonedit_clicked();
  void on_buttonremove_clicked();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
