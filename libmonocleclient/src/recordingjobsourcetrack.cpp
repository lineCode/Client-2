// recordingjobsourcetrack.cpp
//

///// Includes /////

#include "monocleclient/recordingjobsourcetrack.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "monocleclient/view.h"

///// Declarations /////

namespace monocle
{

enum class RecordingJobState : int8_t;

}

///// Namespaces /////

namespace client
{

///// Globals /////

const QString PROFILE_TOKEN_PARAMETER_NAME = "ProfileToken";
const QString PROFILE_WIDTH = "Width";
const QString PROFILE_HEIGHT = "Height";
const QString SOURCE_TAG_PARAMETER_NAME = "SourceTag";
const QString ROTATION_PARAMETER_NAME = "Rotation";
const QString OBJECT_DETECTOR_ACCURACY_PARAMETER_NAME = "ObjectDetectorAccuracy";
const QString OBJECT_DETECTOR_HUMANS_ENABLED_PARAMETER_NAME = "HumansEnabled";
const QString OBJECT_DETECTOR_HUMANS_SENSITIVITY_PARAMETER_NAME = "HumansSensitivity";
const QString OBJECT_DETECTOR_BICYCLES_ENABLED_PARAMETER_NAME = "BicyclesEnabled";
const QString OBJECT_DETECTOR_BICYCLES_SENSITIVITY_PARAMETER_NAME = "BicyclesSensitivity";
const QString OBJECT_DETECTOR_CARS_ENABLED_PARAMETER_NAME = "CarsEnabled";
const QString OBJECT_DETECTOR_CARS_SENSITIVITY_PARAMETER_NAME = "CarsSensitivity";
const QString OBJECT_DETECTOR_MOTORBIKES_ENABLED_PARAMETER_NAME = "MotorbikesEnabled";
const QString OBJECT_DETECTOR_MOTORBIKES_SENSITIVITY_PARAMETER_NAME = "MotorbikesSensitivity";
const QString OBJECT_DETECTOR_BUSES_ENABLED_PARAMETER_NAME = "BusesEnabled";
const QString OBJECT_DETECTOR_BUSES_SENSITIVITY_PARAMETER_NAME = "BusesSensitivity";
const QString OBJECT_DETECTOR_TRUCKS_ENABLED_PARAMETER_NAME = "TrucksEnabled";
const QString OBJECT_DETECTOR_TRUCKS_SENSITIVITY_PARAMETER_NAME = "TrucksSensitivity";
const QString OBJECT_DETECTOR_BACKPACKS_ENABLED_PARAMETER_NAME = "BackpacksEnabled";
const QString OBJECT_DETECTOR_BACKPACKS_SENSITIVITY_PARAMETER_NAME = "BackpacksSensitivity";
const QString OBJECT_DETECTOR_UMBRELLAS_ENABLED_PARAMETER_NAME = "UmbrellasEnabled";
const QString OBJECT_DETECTOR_UMBRELLAS_SENSITIVITY_PARAMETER_NAME = "UmbrellasSensitivity";
const QString OBJECT_DETECTOR_HANDBAGS_ENABLED_PARAMETER_NAME = "HandbagsEnabled";
const QString OBJECT_DETECTOR_HANDBAGS_SENSITIVITY_PARAMETER_NAME = "HandbagsSensitivity";
const QString OBJECT_DETECTOR_SUITCASES_ENABLED_PARAMETER_NAME = "SuitcasesEnabled";
const QString OBJECT_DETECTOR_SUITCASES_SENSITIVITY_PARAMETER_NAME = "SuitcasesSensitivity";
const QString OBJECT_DETECTOR_CATS_ENABLED_PARAMETER_NAME = "CatsEnabled";
const QString OBJECT_DETECTOR_CATS_SENSITIVITY_PARAMETER_NAME = "CatsSensitivity";
const QString OBJECT_DETECTOR_DOGS_ENABLED_PARAMETER_NAME = "DogsEnabled";
const QString OBJECT_DETECTOR_DOGS_SENSITIVITY_PARAMETER_NAME = "DogsSensitivity";
const QString OBJECT_DETECTOR_AEROPLANES_ENABLED_PARAMETER_NAME = "AeroplanesEnabled";
const QString OBJECT_DETECTOR_AEROPLANES_SENSITIVITY_PARAMETER_NAME = "AeroplanesSensitivity";
const QString OBJECT_DETECTOR_TRAINS_ENABLED_PARAMETER_NAME = "TrainsEnabled";
const QString OBJECT_DETECTOR_TRAINS_SENSITIVITY_PARAMETER_NAME = "TrainsSensitivity";
const QString OBJECT_DETECTOR_BOATS_ENABLED_PARAMETER_NAME = "BoatsEnabled";
const QString OBJECT_DETECTOR_BOATS_SENSITIVITY_PARAMETER_NAME = "BoatsSensitivity";
const QString OBJECT_DETECTOR_HORSES_ENABLED_PARAMETER_NAME = "HorsesEnabled";
const QString OBJECT_DETECTOR_HORSES_SENSITIVITY_PARAMETER_NAME = "HorsesSensitivity";


///// Methods /////

RecordingJobSourceTrack::RecordingJobSourceTrack(const uint64_t token, const std::vector<QString>& parameters, const monocle::RecordingJobState state, const QString& error, const std::vector<QString>& activeparameters, const QSharedPointer<client::RecordingTrack>& track) :
  token_(token),
  parameters_(parameters),
  state_(state),
  error_(error),
  activeparameters_(activeparameters),
  track_(track)
{

}

RecordingJobSourceTrack::~RecordingJobSourceTrack()
{

}

void RecordingJobSourceTrack::SetState(const monocle::RecordingJobState state, const QString& error)
{
  const monocle::RecordingJobState prevstate = state_;
  state_ = state;
  error_ = error;
  emit StateChanged(state, error, prevstate);
}

boost::optional<QString> RecordingJobSourceTrack::GetProfileToken() const
{
  return GetParameter(PROFILE_TOKEN_PARAMETER_NAME);
}

boost::optional<QString> RecordingJobSourceTrack::GetSourceTag() const
{
  return GetParameter(SOURCE_TAG_PARAMETER_NAME);
}

boost::optional<ROTATION> RecordingJobSourceTrack::GetRotation() const
{
  const boost::optional<QString> rotation = GetParameter(ROTATION_PARAMETER_NAME);
  if (!rotation.is_initialized())
  {

    return boost::none;
  }

  if (*rotation == "0")
  {

    return ROTATION::_0;
  }
  else if (*rotation == "90")
  {

    return ROTATION::_90;
  }
  else if (*rotation == "180")
  {

    return ROTATION::_180;
  }
  else if (*rotation == "270")
  {

    return ROTATION::_270;
  }
  else
  {

    return boost::none;
  }
}

boost::optional<QString> RecordingJobSourceTrack::GetActiveProfileToken() const
{
  return GetActiveParameter(PROFILE_TOKEN_PARAMETER_NAME);
}

boost::optional<QString> RecordingJobSourceTrack::GetActiveWidth() const
{
  return GetActiveParameter(PROFILE_WIDTH);
}

boost::optional<QString> RecordingJobSourceTrack::GetActiveHeight() const
{
  return GetActiveParameter(PROFILE_HEIGHT);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorAccuracy() const
{
  const boost::optional<QString> accuracy = GetParameter(OBJECT_DETECTOR_ACCURACY_PARAMETER_NAME);
  if (!accuracy.is_initialized())
  {

    return QVariant();
  }

  try
  {

    return boost::lexical_cast<int>(accuracy->toStdString());
  }
  catch (...)
  {

    return 1;
  }
}

QVariant RecordingJobSourceTrack::GetObjectDetectorHumansEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_HUMANS_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorHumansSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_HUMANS_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorBicyclesEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_BICYCLES_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorBicyclesSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_BICYCLES_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorCarsEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_CARS_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorCarsSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_CARS_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorMotorbikesEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_MOTORBIKES_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorMotorbikesSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_MOTORBIKES_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorBusesEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_BUSES_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorBusesSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_BUSES_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorTrucksEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_TRUCKS_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorTrucksSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_TRUCKS_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorBackpacksEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_BACKPACKS_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorBackpacksSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_BACKPACKS_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorUmbrellasEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_UMBRELLAS_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorUmbrellasSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_UMBRELLAS_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorHandbagsEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_HANDBAGS_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorHandbagsSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_HANDBAGS_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorSuitcasesEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_SUITCASES_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorSuitcasesSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_SUITCASES_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorCatsEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_CATS_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorCatsSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_CATS_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorDogsEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_DOGS_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorDogsSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_DOGS_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorAeroplanesEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_AEROPLANES_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorAeroplanesSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_AEROPLANES_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorTrainsEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_TRAINS_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorTrainsSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_TRAINS_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorBoatsEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_BOATS_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorBoatsSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_BOATS_SENSITIVITY_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorHorsesEnabled() const
{
  return GetObjectDetectorEnabled(OBJECT_DETECTOR_HORSES_ENABLED_PARAMETER_NAME);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorHorsesSensitivity() const
{
  return GetObjectDetectorSensitivity(OBJECT_DETECTOR_HORSES_SENSITIVITY_PARAMETER_NAME);
}

boost::optional<QString> RecordingJobSourceTrack::GetParameter(const QString& name) const
{
  const std::string tmp = name.toStdString() + "=";
  std::vector<QString>::const_iterator parameter = std::find_if(parameters_.cbegin(), parameters_.cend(), [&tmp](const QString& parameter) { return boost::istarts_with(parameter.toStdString(), tmp); });
  if (parameter == parameters_.cend())
  {

    return boost::none;
  }
  return parameter->mid(static_cast<int>(tmp.size()), -1);
}

boost::optional<QString> RecordingJobSourceTrack::GetActiveParameter(const QString& name) const
{
  const std::string tmp = name.toStdString() + "=";
  std::vector<QString>::const_iterator activeparameter = std::find_if(activeparameters_.cbegin(), activeparameters_.cend(), [&tmp](const QString& activeparameter) { return boost::istarts_with(activeparameter.toStdString(), tmp); });
  if (activeparameter == activeparameters_.cend())
  {

    return boost::none;
  }
  return activeparameter->mid(static_cast<int>(tmp.size()), -1);
}

QVariant RecordingJobSourceTrack::GetObjectDetectorEnabled(const QString& parameter) const
{
  const boost::optional<QString> enabled = GetParameter(parameter);
  if (!enabled.is_initialized())
  {

    return QVariant();
  }
  return ((*enabled) != "0");
}

QVariant RecordingJobSourceTrack::GetObjectDetectorSensitivity(const QString& parameter) const
{
  const boost::optional<QString> sensitivity = GetParameter(parameter);
  if (!sensitivity.is_initialized())
  {

    return QVariant();
  }

  try
  {

    return boost::lexical_cast<double>(sensitivity->toStdString());
  }
  catch (...)
  {

    return 0.5;
  }
}

}
