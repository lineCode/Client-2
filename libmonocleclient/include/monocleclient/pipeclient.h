// pipeclient.h
//

#ifndef IDLIY0KUAS4353453534535345UCV1EPVI
#define IDLIY0KUAS4353453534535345UCV1EPVI

///// Includes /////

#include <QObject>
#include <utility/pipeclient.hpp>

///// Namespaces /////

namespace client
{

///// Classes /////

class PipeClient : public QObject, public utility::PipeClient
{
 Q_OBJECT

 public:

  PipeClient();
  ~PipeClient();

  virtual void Message(const std::string& message) override;

 private:
 

 signals:

  void Message(const QString& message);

};

}

#endif
