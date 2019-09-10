// runguard.cpp
//

///// Includes /////

#include "monocleclient/runguard.h"

#include <QCryptographicHash>

///// Namespaces /////

namespace client
{

///// Methods /////

RunGuard::RunGuard(const QString& key) :
  semaphore_(GenerateHash(key, QString("semaphore")), 1),
  sharedmemory_(GenerateHash(key, QString("sharedmemory")))
{
  
}

RunGuard::~RunGuard()
{

}

bool RunGuard::Run()
{
  // Is another running
  semaphore_.acquire();
  bool isanotherrunning = sharedmemory_.attach();
  if (isanotherrunning)
  {
    sharedmemory_.detach();
    semaphore_.release();
    return false;
  }

  // Stop others running
  sharedmemory_.create(sizeof(quint64));
  semaphore_.release();
  return true;
}

QString RunGuard::GenerateHash(const QString& key, const QString& salt)
{
  QByteArray data;
  data.append(key.toUtf8());
  data.append(salt.toUtf8());
  data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();
  return data;
}

}
