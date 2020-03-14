// layout.h
//

#ifndef IDLIY0KUASASDASDSADADAZZZZUCV1EPVI
#define IDLIY0KUASASDASDSADADAZZZZUCV1EPVI

///// Includes /////

#include <QObject>
#include <QString>

///// Namespaces /////

namespace client
{

///// Classes /////

class Layout : public QObject
{
 Q_OBJECT

 public:

  Layout(const uint64_t token, const QString& name);
  ~Layout();

  inline uint64_t GetToken() const { return token_; }
  inline const QString& GetName() const { return name_; }

 private:

  uint64_t token_;
  QString name_;

  //TODO all the layout details go in here...
  

};

}

#endif
