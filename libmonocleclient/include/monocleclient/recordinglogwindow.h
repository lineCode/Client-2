// recordinglogwindow.h
//

#ifndef IDSFSDASDSDSADSADSADSADASSADD5OPBW
#define IDSFSDASDSDSADSADSADSADASSADD5OPBW

///// Includes /////

#include <map>
#include <monocleprotocol/client/connection.hpp>
#include <monocleprotocol/monocleprotocol.hpp>
#include <QDialog>
#include <QSharedPointer>
#include <QTreeWidgetItem>
#include <tuple>
#include <vector>

#include "ui_recordinglogwindow.h"

///// Declarations /////

namespace monocle { enum class Severity : int8_t; }

///// Namespaces /////

namespace client
{

///// Declarations ///// 

class Device;
class Recording;
class RecordingJob;
class RecordingJobSource;
class RecordingJobSourceTrack;
class RecordingTrack;

///// Classes /////

class RecordingLogWindow : public QDialog
{
 Q_OBJECT

 public:

  RecordingLogWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const std::vector< QSharedPointer<client::RecordingTrack> >& filtertracks, const std::vector< QSharedPointer<client::RecordingJob> >& filterjobs, const std::vector< QSharedPointer<client::RecordingJobSourceTrack> >& filterjobsourcetracks);
  ~RecordingLogWindow();
  

 protected:



 private:

  void Update();
  bool Filter(const monocle::Severity severity) const;
  bool FilterRecordingTrack(const uint32_t recordingtrackid) const;
  bool FilterRecordingJob(const uint64_t recordingjobtoken) const;
  bool FilterRecordingJobSource(const uint64_t jobtoken, const uint64_t jobsourcetoken, const uint64_t jobsourcetracktoken) const;
  QString HTML(const monocle::Severity severity, const QString& message) const;
  void Message(const boost::shared_ptr< std::vector<std::string> >& messages, const size_t totalrequests) const;
  void AddRecordingTrack(const QSharedPointer<client::RecordingTrack>& track, const bool checked);
  void AddRecordingJob(const QSharedPointer<client::RecordingJob>& recordingjob, const bool checked);
  void AddRecordingJobSourceTrack(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack, const bool checked);
  QTreeWidgetItem* FindTrackItem(const uint64_t token) const;
  QTreeWidgetItem* FindJobItem(const uint64_t token) const;
  QTreeWidgetItem* FindJobSourceTrackItem(const uint64_t jobtoken, const uint64_t jobsourcetoken, const uint64_t jobsourcetracktoken) const;

  Ui::RecordingLogWindow ui_;

  QTreeWidgetItem* recordingtracksitem_;
  QTreeWidgetItem* recordingjobsitem_;

  boost::shared_ptr<Device> device_;
  QSharedPointer<client::Recording> recording_;
  std::vector<monocle::LOGMESSAGE> recordingmessages_;

  std::map< uint32_t, std::vector<monocle::LOGMESSAGE> > recordingtracks_;
  std::map< uint64_t, std::vector<monocle::LOGMESSAGE> > recordingjobs_;
  std::map< std::tuple<uint64_t, uint64_t, uint64_t>, std::vector<monocle::LOGMESSAGE> > recordingjobsourcetracks_; // JobToken, JobSourceToken, JobSourceTrackToken

  monocle::client::Connection subscriberecordingconnection_;
  monocle::client::Connection subscriberecordingtrackconnection_;
  monocle::client::Connection subscriberecordingjobconnection_;
  monocle::client::Connection subscriberecordingjobsourcetrackconnection_;

 private slots:

  void RecordingTrackAdded(const QSharedPointer<client::RecordingTrack>& track);
  void RecordingTrackChanged(const QSharedPointer<client::RecordingTrack>& track);
  void RecordingTrackRemoved(const uint32_t trackid);
  
  void RecordingJobAdded(const QSharedPointer<client::RecordingJob>& track);
  void RecordingJobChanged(const QSharedPointer<client::RecordingJob>& track);
  void RecordingJobRemoved(const uint64_t recordingtoken);

  void RecordingJobSourceTrackAdded(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack);
  void RecordingJobSourceTrackChanged(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack);
  void RecordingJobSourceTrackRemoved(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const uint64_t token);

  void RecordingTrackLogMessage(const QSharedPointer<client::RecordingTrack>& recordingtrack, const uint64_t time, const monocle::Severity severity, const QString& message);
  void RecordingJobLogMessage(const QSharedPointer<client::RecordingJob>& recordingjob, const uint64_t time, const monocle::Severity severity, const QString& message);
  void RecordingJobSourceTrackLogMessage(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack, const uint64_t time, const monocle::Severity severity, const QString& message);
  void RecordingLogMessage(const uint64_t time, const monocle::Severity severity, const QString& message);

  void on_checktrace_clicked();
  void on_checkdebug_clicked();
  void on_checkinfo_clicked();
  void on_checkwarning_clicked();
  void on_checkerror_clicked();
  void on_checkcritical_clicked();
  void on_treefilter_itemChanged(QTreeWidgetItem* item, int column);
  void on_buttoncopy_clicked();
  void on_buttonok_clicked();

};

}

#endif
