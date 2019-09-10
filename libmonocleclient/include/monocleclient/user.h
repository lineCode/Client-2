// user.h
//

#ifndef IDLIY0123564654645646546456CV1EPVI
#define IDLIY0123564654645646546456CV1EPVI

///// Includes /////

#include <QObject>

///// Namespaces /////

namespace client
{

///// Classes /////

class User : public QObject
{
 Q_OBJECT

 public:

  User(const uint64_t token, const QString& username, const uint64_t grouptoken);
  ~User();
  
  inline uint64_t GetToken() const { return token_; }
  inline void SetUsername(const QString& username) { username_ = username; }
  inline const QString& GetUsername() const { return username_; }
  inline void SetGroupToken(const uint64_t grouptoken) { grouptoken_ = grouptoken; }
  inline uint64_t GetGroupToken() const { return grouptoken_; }

 private:

  uint64_t token_;
  QString username_;
  uint64_t grouptoken_;
 
};

}

#endif
