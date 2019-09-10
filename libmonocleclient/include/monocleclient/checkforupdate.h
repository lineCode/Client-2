// checkforupdate.h
//

#ifndef IDZ8OT6KS0CSFKIQOND8WUJVNRX9KTDKWG
#define IDZ8OT6KS0CSFKIQOND8WUJVNRX9KTDKWG

///// Includes /////

#include <QDate>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>
#include <utility/version.hpp>

#include <vector>

///// Namespaces /////

namespace client
{

///// Structures /////

struct UpdateVersion
{
  UpdateVersion()
  {

  }

  UpdateVersion(const utility::Version& version) :
    version_(version)
  {

  }

  utility::Version version_;
  QDate date_;
  QUrl url_;
  QVector<QString> changelist_;

};

///// Classes /////

class CheckForUpdate : public QObject
{
 Q_OBJECT

 public:

  CheckForUpdate(const utility::Version& version);
  virtual ~CheckForUpdate();

  void Check();
  
 signals:

  void UpdateAvailable(bool updateavailable, const std::vector<UpdateVersion>& versions, const UpdateVersion& latestversion);

 private:
  
  const utility::Version& version_;

  QNetworkAccessManager* manager_;
  
 private slots:
  
  void NetworkReply(QNetworkReply* networkreply);

};

}

#endif
