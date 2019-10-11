// recordingjobsourcetrack.h
//

#ifndef IDLIY0KCXN238SDFJALZAJSDALDCV1EPVI
#define IDLIY0KCXN238SDFJALZAJSDALDCV1EPVI

///// Includes /////

#include <boost/optional.hpp>
#include <QObject>
#include <QSharedPointer>
#include <QString>

///// Declarations /////

namespace monocle
{

enum class RecordingJobState : int8_t;

}

///// Namespaces /////

namespace client
{

///// Declarations /////

class RecordingTrack;
enum class ROTATION : int;

///// Globals /////

extern const QString PROFILE_TOKEN_PARAMETER_NAME;
extern const QString SOURCE_TAG_PARAMETER_NAME;
extern const QString ROTATION_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_ACCURACY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_HUMANS_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_HUMANS_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_BICYCLES_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_BICYCLES_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_CARS_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_CARS_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_MOTORBIKES_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_MOTORBIKES_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_BUSES_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_BUSES_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_TRUCKS_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_TRUCKS_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_BACKPACKS_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_BACKPACKS_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_UMBRELLAS_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_UMBRELLAS_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_HANDBAGS_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_HANDBAGS_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_SUITCASES_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_SUITCASES_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_CATS_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_CATS_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_DOGS_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_DOGS_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_AEROPLANES_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_AEROPLANES_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_TRAINS_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_TRAINS_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_BOATS_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_BOATS_SENSITIVITY_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_HORSES_ENABLED_PARAMETER_NAME;
extern const QString OBJECT_DETECTOR_HORSES_SENSITIVITY_PARAMETER_NAME;

///// Classes /////

class RecordingJobSourceTrack : public QObject
{
 Q_OBJECT

 public:

  RecordingJobSourceTrack(const uint64_t token, const std::vector<QString>& parameters, const monocle::RecordingJobState state, const QString& error, const std::vector<QString>& activeparameters, const QSharedPointer<client::RecordingTrack>& track);
  ~RecordingJobSourceTrack();
  
  inline uint64_t GetToken() const { return token_; }
  inline const std::vector<QString> GetParameters() const { return parameters_; }
  void SetState(const monocle::RecordingJobState state, const QString& error);
  inline monocle::RecordingJobState GetState() const { return state_; }
  inline const QString& GetError() const { return error_; }
  inline const std::vector<QString> GetActiveParameters() const { return activeparameters_; }
  inline void SetActiveParameters(const std::vector<QString>& activeparameters) { activeparameters_ = activeparameters; }
  inline const QSharedPointer<client::RecordingTrack>& GetTrack() const { return track_; }

  boost::optional<QString> GetSourceTag() const;
  boost::optional<QString> GetProfileToken() const;
  boost::optional<ROTATION> GetRotation() const;
  boost::optional<QString> GetActiveProfileToken() const;
  QVariant GetObjectDetectorAccuracy() const;
  QVariant GetObjectDetectorHumansEnabled() const;
  QVariant GetObjectDetectorHumansSensitivity() const;
  QVariant GetObjectDetectorBicyclesEnabled() const;
  QVariant GetObjectDetectorBicyclesSensitivity() const;
  QVariant GetObjectDetectorCarsEnabled() const;
  QVariant GetObjectDetectorCarsSensitivity() const;
  QVariant GetObjectDetectorMotorbikesEnabled() const;
  QVariant GetObjectDetectorMotorbikesSensitivity() const;
  QVariant GetObjectDetectorBusesEnabled() const;
  QVariant GetObjectDetectorBusesSensitivity() const;
  QVariant GetObjectDetectorTrucksEnabled() const;
  QVariant GetObjectDetectorTrucksSensitivity() const;
  QVariant GetObjectDetectorBackpacksEnabled() const;
  QVariant GetObjectDetectorBackpacksSensitivity() const;
  QVariant GetObjectDetectorUmbrellasEnabled() const;
  QVariant GetObjectDetectorUmbrellasSensitivity() const;
  QVariant GetObjectDetectorHandbagsEnabled() const;
  QVariant GetObjectDetectorHandbagsSensitivity() const;
  QVariant GetObjectDetectorSuitcasesEnabled() const;
  QVariant GetObjectDetectorSuitcasesSensitivity() const;
  QVariant GetObjectDetectorCatsEnabled() const;
  QVariant GetObjectDetectorCatsSensitivity() const;
  QVariant GetObjectDetectorDogsEnabled() const;
  QVariant GetObjectDetectorDogsSensitivity() const;
  QVariant GetObjectDetectorAeroplanesEnabled() const;
  QVariant GetObjectDetectorAeroplanesSensitivity() const;
  QVariant GetObjectDetectorTrainsEnabled() const;
  QVariant GetObjectDetectorTrainsSensitivity() const;
  QVariant GetObjectDetectorBoatsEnabled() const;
  QVariant GetObjectDetectorBoatsSensitivity() const;
  QVariant GetObjectDetectorHorsesEnabled() const;
  QVariant GetObjectDetectorHorsesSensitivity() const;
  
 private:

  boost::optional<QString> GetParameter(const QString& name) const;
  boost::optional<QString> GetActiveParameter(const QString& name) const;
  QVariant GetObjectDetectorEnabled(const QString& parameter) const;
  QVariant GetObjectDetectorSensitivity(const QString& parameter) const;

  uint64_t token_;
  std::vector<QString> parameters_;
  monocle::RecordingJobState state_;
  QString error_;
  std::vector<QString> activeparameters_;
  QSharedPointer<client::RecordingTrack> track_;

 signals:

  void StateChanged(const monocle::RecordingJobState state, const QString& error, const monocle::RecordingJobState prevstate);

};

}

#endif
