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
  
 private:

  boost::optional<QString> GetParameter(const QString& name) const;
  boost::optional<QString> GetActiveParameter(const QString& name) const;

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
