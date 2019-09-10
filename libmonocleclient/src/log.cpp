// log.cpp
//

///// Includes /////

#include "monocleclient/log.h"

#include <assert.h>
#include <QCoreApplication>
#include <QStandardPaths>

#include "monocleclient/logtable.h"
#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Functions /////

QString ToString(LOGLEVEL loglevel)
{
  switch (loglevel)
  {
    case LOGLEVEL_EVENT:
    {

      return QString("Event");
    }
    case LOGLEVEL_MESSAGE:
    {

      return QString("Message");
    }
    case LOGLEVEL_WARNING:
    {

      return QString("Warning");
    }
    case LOGLEVEL_FATAL:
    {

      return QString("Fatal");
    }
    default:
    {
      assert(false);
      return QString();
    }
  }
}

///// Methods /////

Log::Log()
{
  
}

Log::~Log()
{
  Destroy();

}

int Log::Init(const QString& filename)
{
  Destroy();

  file_.setFileName(QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).filePath(filename));
  if (!file_.open(QIODevice::WriteOnly | QIODevice::Text))
  {

    return 1;
  }

  return 0;
}

void Log::Destroy()
{
  if (file_.isOpen())
  {
    file_.close();

  }
}

int Log::Message(client::LOGLEVEL loglevel, const char* file, int line, const char* function, const boost::shared_ptr<client::Device> device, const QString& message)
{
  if (device)
  {
  
    return Message(loglevel, file, line, function, device->GetAddress() + QString(" ") + message);
  }
  else
  {
  
    return Message(loglevel, file, line, function, message);
  }
}

int Log::Message(client::LOGLEVEL loglevel, const char* file, int line, const char* function, const QString& message)
{
  if (file_.write((ToString(loglevel) + QString(" ") + file + QString(" ") + function + QString(":") + QString::number(line) + QString(" ") + message + QString("\n")).toLocal8Bit().data()) == -1)
  {

    return 1;
  }

  if (!file_.flush())
  {

    return 1;
  }

  return 0;
}

}
