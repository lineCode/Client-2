// managerecordingtrackwindow.h
//

#ifndef ID2VV2343243242432423432DKSAWKDN9G
#define ID2VV2343243242432423432DKSAWKDN9G

///// Includes /////

#include <monocleprotocol/client/connection.hpp>
#include <QSharedPointer>

#include "ui_managerecordingtrackwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class File;
class Recording;
class RecordingTrack;

///// Classes /////

class ManageRecordingTrackWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageRecordingTrackWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingTrack>& track);
  ~ManageRecordingTrackWindow();

 private:

  void AddFile(const QSharedPointer<client::File>& file);
  void SetEnabled(const bool enabled);
  void Setup(const QSharedPointer<client::RecordingTrack>& track);

  Ui::ManageRecordingTrackWindow ui_;

  boost::shared_ptr<Device> device_;
  QSharedPointer<client::Recording> recording_;
  QSharedPointer<client::RecordingTrack> track_;

  monocle::client::Connection trackconnection_;

 private slots:

  void FileAdded(const QSharedPointer<client::File>& file);
  void FileRemoved(const uint64_t token);
  void TrackChanged(const QSharedPointer<client::RecordingTrack>& track);
  void TrackRemoved(const uint32_t id);
  void on_checkfixedfiles_toggled(bool);
  void on_buttonright_clicked();
  void on_buttonleft_clicked();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
