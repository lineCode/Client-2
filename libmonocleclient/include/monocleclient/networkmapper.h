// networkmapper.h
// 

#ifndef IDFBJDBRQASDASDADADADADADADZZZISRJ
#define IDFBJDBRQASDASDADADADADADADZZZISRJ

///// Includes /////

#include <QObject>

///// Namespaces /////

namespace client
{

///// Classes /////

class NetworkMapper : public QObject
{
 Q_OBJECT

 public:

  NetworkMapper();
  ~NetworkMapper();

 signals:

  void DiscoverONVIFDevice();

 private slots:

};

}

#endif
