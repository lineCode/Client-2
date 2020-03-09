// managetrackobjectdetectorwindow.cpp
//

///// Includes /////

#include "monocleclient/managetrackobjectdetectorwindow.h"

#include <monocleprotocol/streamingprotocol_generated.h>
#include <network/uri.hpp>
#include <onvifclient/deviceclient.hpp>
#include <onvifclient/mediaclient.hpp>
#include <QMessageBox>
#include <QTimer>
#include <QUrl>
#include <rtsp/client/client.hpp>

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/receiver.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageTrackObjectDetectorWindow::ManageTrackObjectDetectorWindow(QWidget* parent,
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
                                                                 const double horsessensitivity) :
  QDialog(parent),
  accuracy_(accuracy),
  humans_(humans),
  bicycles_(bicycles),
  cars_(cars),
  motorbikes_(motorbikes),
  buses_(buses),
  trucks_(trucks),
  backpacks_(backpacks),
  umbrellas_(umbrellas),
  handbags_(handbags),
  suitcases_(suitcases),
  cats_(cats),
  dogs_(dogs),
  aeroplanes_(aeroplanes),
  trains_(trains),
  boats_(boats),
  horses_(horses),
  humanssensitivity_(humanssensitivity),
  bicyclessensitivity_(bicyclessensitivity),
  carssensitivity_(carssensitivity),
  motorbikessensitivity_(motorbikessensitivity),
  busessensitivity_(busessensitivity),
  truckssensitivity_(truckssensitivity),
  backpackssensitivity_(backpackssensitivity),
  umbrellassensitivity_(umbrellassensitivity),
  handbagssensitivity_(handbagssensitivity),
  suitcasessensitivity_(suitcasessensitivity),
  catssensitivity_(catssensitivity),
  dogssensitivity_(dogssensitivity),
  aeroplanessensitivity_(aeroplanessensitivity),
  trainssensitivity_(trainssensitivity),
  boatssensitivity_(boatssensitivity),
  horsessensitivity_(horsessensitivity)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  ui_.checkhumans->setChecked(humans);
  ui_.checkbicycles->setChecked(bicycles);
  ui_.checkcars->setChecked(cars);
  ui_.checkmotorbikes->setChecked(motorbikes);
  ui_.checkbuses->setChecked(buses);
  ui_.checktrucks->setChecked(trucks);
  ui_.checkbackpacks->setChecked(backpacks);
  ui_.checkumbrellas->setChecked(umbrellas);
  ui_.checkhandbags->setChecked(handbags);
  ui_.checksuitcases->setChecked(suitcases);
  ui_.checkcats->setChecked(cats);
  ui_.checkdogs->setChecked(dogs);
  ui_.checkaeroplanes->setChecked(aeroplanes);
  ui_.checktrains->setChecked(trains);
  ui_.checkboats->setChecked(boats);
  ui_.checkhorses->setChecked(horses);
  ui_.sliderhumans->setValue((1.0 - humanssensitivity) * static_cast<double>(ui_.sliderhumans->maximum()));
  ui_.sliderbicycles->setValue((1.0 - bicyclessensitivity) * static_cast<double>(ui_.sliderbicycles->maximum()));
  ui_.slidercars->setValue((1.0 - carssensitivity) * static_cast<double>(ui_.slidercars->maximum()));
  ui_.slidermotorbikes->setValue((1.0 - motorbikessensitivity) * static_cast<double>(ui_.slidermotorbikes->maximum()));
  ui_.sliderbuses->setValue((1.0 - busessensitivity) * static_cast<double>(ui_.sliderbuses->maximum()));
  ui_.slidertrucks->setValue((1.0 - truckssensitivity) * static_cast<double>(ui_.slidertrucks->maximum()));
  ui_.sliderbackpacks->setValue((1.0 - backpackssensitivity) * static_cast<double>(ui_.sliderbackpacks->maximum()));
  ui_.sliderumbrellas->setValue((1.0 - umbrellassensitivity) * static_cast<double>(ui_.sliderumbrellas->maximum()));
  ui_.sliderhandbags->setValue((1.0 - handbagssensitivity) * static_cast<double>(ui_.sliderhandbags->maximum()));
  ui_.slidersuitcases->setValue((1.0 - suitcasessensitivity) * static_cast<double>(ui_.slidersuitcases->maximum()));
  ui_.slidercats->setValue((1.0 - catssensitivity) * static_cast<double>(ui_.slidercats->maximum()));
  ui_.sliderdogs->setValue((1.0 - dogssensitivity) * static_cast<double>(ui_.sliderdogs->maximum()));
  ui_.slideraeroplanes->setValue((1.0 - aeroplanessensitivity) * static_cast<double>(ui_.slideraeroplanes->maximum()));
  ui_.slidertrains->setValue((1.0 - trainssensitivity) * static_cast<double>(ui_.slidertrains->maximum()));
  ui_.sliderboats->setValue((1.0 - boatssensitivity) * static_cast<double>(ui_.sliderboats->maximum()));
  ui_.sliderhorses->setValue((1.0 - horsessensitivity) * static_cast<double>(ui_.sliderhorses->maximum()));

  // Select current accuracy
  accuracy_ = std::min(ui_.comboaccuracy->count() - 1, accuracy_);
  accuracy_ = std::max(0, accuracy);
  ui_.comboaccuracy->setCurrentIndex(accuracy_);
}

ManageTrackObjectDetectorWindow::~ManageTrackObjectDetectorWindow()
{

}

void ManageTrackObjectDetectorWindow::on_sliderhumans_valueChanged(int value)
{
  ui_.checkhumans->setText("Humans(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_sliderbicycles_valueChanged(int value)
{
  ui_.checkbicycles->setText("Bicycles(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_slidercars_valueChanged(int value)
{
  ui_.checkcars->setText("Cars(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_slidermotorbikes_valueChanged(int value)
{
  ui_.checkmotorbikes->setText("Motorbikes(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_sliderbuses_valueChanged(int value)
{
  ui_.checkbuses->setText("Buses(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_slidertrucks_valueChanged(int value)
{
  ui_.checktrucks->setText("Trucks(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_sliderbackpacks_valueChanged(int value)
{
  ui_.checkbackpacks->setText("Backpacks(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_sliderumbrellas_valueChanged(int value)
{
  ui_.checkumbrellas->setText("Umbrellas(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_sliderhandbags_valueChanged(int value)
{
  ui_.checkhandbags->setText("Handbags(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_slidersuitcases_valueChanged(int value)
{
  ui_.checksuitcases->setText("Suitcases(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_slidercats_valueChanged(int value)
{
  ui_.checkcats->setText("Cats(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_sliderdogs_valueChanged(int value)
{
  ui_.checkdogs->setText("Dogs(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_slideraeroplanes_valueChanged(int value)
{
  ui_.checkaeroplanes->setText("Aeroplanes(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_slidertrains_valueChanged(int value)
{
  ui_.checktrains->setText("Trains(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_sliderboats_valueChanged(int value)
{
  ui_.checkboats->setText("Boats(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_sliderhorses_valueChanged(int value)
{
  ui_.checkhorses->setText("Horses(" + QString::number(value + 1) + ")");

}

void ManageTrackObjectDetectorWindow::on_buttonok_clicked()
{
  accuracy_ = ui_.comboaccuracy->currentIndex();
  humans_ = ui_.checkhumans->isChecked();
  bicycles_ = ui_.checkbicycles->isChecked();
  cars_ = ui_.checkcars->isChecked();
  motorbikes_ = ui_.checkmotorbikes->isChecked();
  buses_ = ui_.checkbuses->isChecked();
  trucks_ = ui_.checktrucks->isChecked();
  backpacks_ = ui_.checkbackpacks->isChecked();
  umbrellas_ = ui_.checkumbrellas->isChecked();
  handbags_ = ui_.checkhandbags->isChecked();
  suitcases_ = ui_.checksuitcases->isChecked();
  cats_ = ui_.checkcats->isChecked();
  dogs_ = ui_.checkdogs->isChecked();
  aeroplanes_ = ui_.checkaeroplanes->isChecked();
  trains_ = ui_.checktrains->isChecked();
  boats_ = ui_.checkboats->isChecked();
  horses_ = ui_.checkhorses->isChecked();
  humanssensitivity_ = (static_cast<double>(ui_.sliderhumans->maximum() - ui_.sliderhumans->value()) / static_cast<double>(ui_.sliderhumans->maximum()));
  bicyclessensitivity_ = (static_cast<double>(ui_.sliderbicycles->maximum() - ui_.sliderbicycles->value()) / static_cast<double>(ui_.sliderbicycles->maximum()));
  carssensitivity_ = (static_cast<double>(ui_.slidercars->maximum() - ui_.slidercars->value()) / static_cast<double>(ui_.slidercars->maximum()));
  motorbikessensitivity_ = (static_cast<double>(ui_.slidermotorbikes->maximum() - ui_.slidermotorbikes->value()) / static_cast<double>(ui_.slidermotorbikes->maximum()));
  busessensitivity_ = (static_cast<double>(ui_.sliderbuses->maximum() - ui_.sliderbuses->value()) / static_cast<double>(ui_.sliderbuses->maximum()));
  truckssensitivity_ = (static_cast<double>(ui_.slidertrucks->maximum() - ui_.slidertrucks->value()) / static_cast<double>(ui_.slidertrucks->maximum()));
  backpackssensitivity_ = (static_cast<double>(ui_.sliderbackpacks->maximum() - ui_.sliderbackpacks->value()) / static_cast<double>(ui_.sliderbackpacks->maximum()));
  umbrellassensitivity_ = (static_cast<double>(ui_.sliderumbrellas->maximum() - ui_.sliderumbrellas->value()) / static_cast<double>(ui_.sliderumbrellas->maximum()));
  handbagssensitivity_ = (static_cast<double>(ui_.sliderhandbags->maximum() - ui_.sliderhandbags->value()) / static_cast<double>(ui_.sliderhandbags->maximum()));
  suitcasessensitivity_ = (static_cast<double>(ui_.slidersuitcases->maximum() - ui_.slidersuitcases->value()) / static_cast<double>(ui_.slidersuitcases->maximum()));
  catssensitivity_ = (static_cast<double>(ui_.slidercats->maximum() - ui_.slidercats->value()) / static_cast<double>(ui_.slidercats->maximum()));
  dogssensitivity_ = (static_cast<double>(ui_.sliderdogs->maximum() - ui_.sliderdogs->value()) / static_cast<double>(ui_.sliderdogs->maximum()));
  aeroplanessensitivity_ = (static_cast<double>(ui_.slideraeroplanes->maximum() - ui_.slideraeroplanes->value()) / static_cast<double>(ui_.slideraeroplanes->maximum()));
  trainssensitivity_ = (static_cast<double>(ui_.slidertrains->maximum() - ui_.slidertrains->value()) / static_cast<double>(ui_.slidertrains->maximum()));
  boatssensitivity_ = (static_cast<double>(ui_.sliderboats->maximum() - ui_.sliderboats->value()) / static_cast<double>(ui_.sliderboats->maximum()));
  horsessensitivity_ = (static_cast<double>(ui_.sliderhorses->maximum() - ui_.sliderhorses->value()) / static_cast<double>(ui_.sliderhorses->maximum()));
  accept();
}

}

QT_END_NAMESPACE
