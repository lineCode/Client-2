// map.h
//

#ifndef IDLIY0KUA5645646566666641RUCV1EPVI
#define IDLIY0KUA5645646566666641RUCV1EPVI

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <QString>

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;

///// Classes /////

class Map : public QObject
{
 Q_OBJECT

 public:

  Map(const boost::shared_ptr<Device>& device, const uint64_t token, const QString& name, const QString& location, const QString& imagemd5, const uint64_t guiorder);
  ~Map();
  
  inline const boost::shared_ptr<Device> GetDevice() const { return device_; }
  inline uint64_t GetToken() const { return token_; }
  void SetName(const QString& name) { name_ = name; }
  inline const QString& GetName() const { return name_; }
  void SetLocation(const QString& location) { location_ = location; }
  inline const QString& GetLocation() const { return location_; }
  void SetImageMD5(const QString& imagemd5) { imagemd5_ = imagemd5; }
  inline const QString& GetImageMD5() const { return imagemd5_; }
  inline uint64_t GetGuiOrder() const { return guiorder_; }

 private:

  boost::shared_ptr<Device> device_;
  uint64_t token_;
  QString name_;
  QString location_;
  QString imagemd5_;
  uint64_t guiorder_;

};

}

#endif
