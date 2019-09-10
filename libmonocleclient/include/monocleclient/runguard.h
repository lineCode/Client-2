// runguard.h
//

#ifndef IDSP1LDDPW7VEXEF2YJH29YUO0GQK8NPJA
#define IDSP1LDDPW7VEXEF2YJH29YUO0GQK8NPJA

///// Includes /////

#include <QSharedMemory>
#include <QString>
#include <QSystemSemaphore>

///// Namespaces /////

namespace client
{

///// Classes /////

class RunGuard
{
 public:

  RunGuard(const QString& key);
  ~RunGuard();

  bool Run();

 private:
 
  QString GenerateHash(const QString& key, const QString& salt);

  QSystemSemaphore semaphore_;
  QSharedMemory sharedmemory_;
    
};

}

#endif
