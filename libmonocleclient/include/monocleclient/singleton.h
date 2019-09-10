// singleton.h
//

#ifndef IDAS8VPRTA3YHVHMMNP9UQCECWJWAYA9CD
#define IDAS8VPRTA3YHVHMMNP9UQCECWJWAYA9CD

///// Includes /////

#include <QObject>

///// Namespaces /////

namespace client
{

///// Classes /////

template<class T>
class Singleton : public QObject
{
 public:

  static T& Instance() { return instance_; }

 protected:

  Singleton()
  {

  }

  virtual ~Singleton()
  {

  }

 private:

  static T instance_;

};

///// Decalarations /////

template <class T>
T Singleton<T>::instance_;

}

#endif
