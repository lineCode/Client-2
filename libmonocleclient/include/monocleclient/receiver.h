// receiver.h
//

#ifndef IDLIY0KUASDNSANSDALWJDAD1RUCV1EPVI
#define IDLIY0KUASDNSANSDALWJDAD1RUCV1EPVI

///// Includes /////

#include <boost/optional.hpp>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <vector>

///// Declarations /////

namespace monocle
{
enum class ReceiverMode : int8_t;
enum class StreamingProtocol : int8_t;
enum class ReceiverState : int8_t;
}

///// Namespaces /////

namespace client
{
///// Globals /////

extern const QString STREAMING_PROTOCOL_PARAMETER_NAME;

///// Classes /////

class Receiver : public QObject
{
 Q_OBJECT

 public:

  Receiver(const uint64_t token, const monocle::ReceiverMode mode, const QString& mediauri, const bool autocreated, const QString& username, const QString& password, const std::vector<QString>& parameters, const monocle::ReceiverState state);
  ~Receiver();

  void SetConfiguration(const monocle::ReceiverMode mode, const QString& mediauri, const bool autocreated, const QString& username, const QString& password, const std::vector<QString>& parameters);

  boost::optional<monocle::StreamingProtocol> GetStreamingProtocol() const;

  inline uint64_t GetToken() const { return token_; }
  inline void SetMode(const monocle::ReceiverMode mode) { mode_ = mode; }
  inline monocle::ReceiverMode GetReceiverMode() const { return mode_; }
  inline void SetMediaUri(const QString& mediauri) { mediauri_ = mediauri; }
  inline const QString& GetMediaUri() const { return mediauri_; }
  inline void SetAutoCreated(const bool autocreated) { autocreated_ = autocreated; }
  inline bool GetAutoCreated() const { return autocreated_; }
  inline void SetUsername(const QString& username) { username_ = username; }
  inline const QString& GetUsername() const { return username_; }
  inline void SetPassword(const QString& password) { password_ = password; }
  inline const QString& GetPassword() const { return password_; }
  inline void SetParameters(const std::vector<QString>& parameters) { parameters_ = parameters; }
  inline const std::vector<QString>& GetParameters() const { return parameters_; }
  inline void SetReceiverState(const monocle::ReceiverState state) { state_ = state; }
  inline monocle::ReceiverState GetReceiverState() const { return state_; }
  
  bool IsONVIF() const;

 private:

  boost::optional<QString> GetParameter(const QString& name) const;

  uint64_t token_;
  monocle::ReceiverMode mode_;
  QString mediauri_;
  bool autocreated_;
  QString username_;
  QString password_;
  std::vector<QString> parameters_;
  monocle::ReceiverState state_;

 signals:

  

};

}

#endif
