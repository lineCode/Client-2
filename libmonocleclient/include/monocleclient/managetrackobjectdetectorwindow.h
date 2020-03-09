// managetrackobjectdetectorwindow.h
//

#ifndef ID2VVCASAASDHSADSADASDASDSD9WKDN9G
#define ID2VVCASAASDHSADSADASDASDSD9WKDN9G

///// Includes /////

#include <boost/optional.hpp>
#include <memory>
#include <monocleprotocol/client/connection.hpp>
#include <QDialog>
#include <QSharedPointer>
#include <QString>

#include "ui_managetrackobjectdetectorwindow.h"

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

class ManageTrackObjectDetectorWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageTrackObjectDetectorWindow(QWidget* parent,
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
                                  const double horsessensitivity);
  ~ManageTrackObjectDetectorWindow();

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

  Ui::ManageTrackObjectDetectorWindow ui_;

 private slots:

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
