// managerecordingjobsourceobjectdetectorwindow.h
//

#ifndef ID2VVCASASDKSDFJDSFDSFSSAJD9WKDN9G
#define ID2VVCASASDKSDFJDSFDSFSSAJD9WKDN9G

///// Includes /////

#include <boost/optional.hpp>
#include <memory>
#include <monocleprotocol/client/connection.hpp>
#include <QDialog>
#include <QSharedPointer>
#include <QString>

#include "ui_managerecordingjobsourceobjectdetectorwindow.h"

///// Declarations /////

namespace monocle
{

enum class ReceiverMode : int8_t;

}

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Receiver;
class Recording;
class RecordingJob;
class RecordingJobSource;
class RecordingJobSourceTrack;
class RecordingTrack;

///// Classes /////

class ManageRecordingJobSourceObjectDetectorWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageRecordingJobSourceObjectDetectorWindow(QWidget* parent,
                                               const boost::shared_ptr<Device>& device,
                                               const QSharedPointer<client::Recording>& recording,
                                               const QSharedPointer<client::RecordingJob>& recordingjob,
                                               const bool editing,
                                               const bool enabled,
                                               const uint32_t metadatatrackid,
                                               const QString& mediauri,
                                               const int accuracy,
                                               const bool humans,
                                               const bool bicycles,
                                               const bool cars,
                                               const bool motorbikes,
                                               const bool buses,
                                               const bool trucks,
                                               const bool backpacks,
                                               const bool umbrellas,
                                               const bool handbags,
                                               const bool suitcases,
                                               const bool cats,
                                               const bool dogs,
                                               const bool aeroplanes,
                                               const bool trains,
                                               const bool boats,
                                               const bool horses,
                                               const double humanssensitivity,
                                               const double bicyclessensitivity,
                                               const double carssensitivity,
                                               const double motorbikessensitivity,
                                               const double busessensitivity,
                                               const double truckssensitivity,
                                               const double backpackssensitivity,
                                               const double umbrellassensitivity,
                                               const double handbagssensitivity,
                                               const double suitcasessensitivity,
                                               const double catssensitivity,
                                               const double dogssensitivity,
                                               const double aeroplanessensitivity,
                                               const double trainssensitivity,
                                               const double boatssensitivity,
                                               const double horsessensitivity,
                                               const std::vector<uint32_t>& reservedtrackids);
  ~ManageRecordingJobSourceObjectDetectorWindow();

  bool enabled_;
  uint32_t metadatatrackid_;
  QString uri_;
  int accuracy_; // 0=Low,1=Medium,2=High,3=VeryHigh
  bool humans_;
  bool bicycles_;
  bool cars_;
  bool motorbikes_;
  bool buses_;
  bool trucks_;
  bool backpacks_;
  bool umbrellas_;
  bool handbags_;
  bool suitcases_;
  bool cats_;
  bool dogs_;
  bool aeroplanes_;
  bool trains_;
  bool boats_;
  bool horses_;
  double humanssensitivity_;
  double bicyclessensitivity_;
  double carssensitivity_;
  double motorbikessensitivity_;
  double busessensitivity_;
  double truckssensitivity_;
  double backpackssensitivity_;
  double umbrellassensitivity_;
  double handbagssensitivity_;
  double suitcasessensitivity_;
  double catssensitivity_;
  double dogssensitivity_;
  double aeroplanessensitivity_;
  double trainssensitivity_;
  double boatssensitivity_;
  double horsessensitivity_;

 private:

  Ui::ManageRecordingJobSourceObjectDetectorWindow ui_;

  const boost::shared_ptr<Device> device_;
  const QSharedPointer<client::Recording> recording_;
  const QSharedPointer<client::RecordingJob> recordingjob_;

 private slots:

  void RecordingJobSourceTrackRemoved(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const uint64_t token);
  void TrackAdded(const QSharedPointer<client::RecordingTrack>& track);
  void TrackChanged(const QSharedPointer<client::RecordingTrack>& track);
  void TrackRemoved(const uint32_t id);
  void on_sliderhumans_valueChanged(int value);
  void on_sliderbicycles_valueChanged(int value);
  void on_slidercars_valueChanged(int value);
  void on_slidermotorbikes_valueChanged(int value);
  void on_sliderbuses_valueChanged(int value);
  void on_slidertrucks_valueChanged(int value);
  void on_sliderbackpacks_valueChanged(int value);
  void on_sliderumbrellas_valueChanged(int value);
  void on_sliderhandbags_valueChanged(int value);
  void on_slidersuitcases_valueChanged(int value);
  void on_slidercats_valueChanged(int value);
  void on_sliderdogs_valueChanged(int value);
  void on_slideraeroplanes_valueChanged(int value);
  void on_slidertrains_valueChanged(int value);
  void on_sliderboats_valueChanged(int value);
  void on_sliderhorses_valueChanged(int value);
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
