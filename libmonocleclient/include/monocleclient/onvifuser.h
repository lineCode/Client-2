// onvifuser.h
//

#ifndef IDLIY0KDFSKDFJSLFKSJFLSKFJSFF1EPVI
#define IDLIY0KDFSKDFJSLFKSJFLSKFJSFF1EPVI

///// Includes /////

#include <QObject>

///// Declarations /////

namespace monocle
{

enum class ONVIFUserlevel : int8_t;

}

///// Namespaces /////

namespace client
{

///// Classes /////

class ONVIFUser : public QObject
{
 Q_OBJECT

 public:

  ONVIFUser(const uint64_t token, const QString& username, const monocle::ONVIFUserlevel userlevel);
  ~ONVIFUser();
  
  inline uint64_t GetToken() const { return token_; }
  inline void SetUsername(const QString& username) { username_ = username; }
  inline const QString& GetUsername() const { return username_; }
  inline void SetUserlevel(const monocle::ONVIFUserlevel userlevel) { userlevel_ = userlevel; }
  inline monocle::ONVIFUserlevel GetUserlevel() const { return userlevel_; }

 private:

  uint64_t token_;
  QString username_;
  monocle::ONVIFUserlevel userlevel_;
 
};

}

#endif
