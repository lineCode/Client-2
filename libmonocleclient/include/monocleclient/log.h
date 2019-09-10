// log.h
//

#ifndef IDNB7HNMYUDQIP73N068S91CDGAMZHITUK
#define IDNB7HNMYUDQIP73N068S91CDGAMZHITUK

///// Includes /////

#include <QDir>
#include <QFile>
#include <QSharedPointer>
#include <QString>

#include "device.h"

///// Defines /////

#if defined(WIN32)
  #define __func__ __FUNCTION__
#endif

///// Namespaces /////

namespace client
{

///// Enumerations /////

enum LOGLEVEL
{
  LOGLEVEL_EVENT,
  LOGLEVEL_MESSAGE,
  LOGLEVEL_WARNING,
  LOGLEVEL_FATAL
};

///// Prototypes /////

QString ToString(LOGLEVEL loglevel);

///// Classes /////

class Log : public QObject
{
 Q_OBJECT

 public:

  Log();
  virtual ~Log();

  int Init(const QString& filename);
  void Destroy();
  
  Q_INVOKABLE int Message(client::LOGLEVEL loglevel, const char* file, int line, const char* function, const boost::shared_ptr<client::Device> device, const QString& message);
  Q_INVOKABLE int Message(client::LOGLEVEL loglevel, const char* file, int line, const char* function, const QString& message); // This requires the fully qualified client::LEVEL for Qt to recognise it as Q_INVOKABLE

 private:

  QFile file_;

};

}

#endif
