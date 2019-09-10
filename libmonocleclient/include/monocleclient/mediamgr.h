// mediamgr.h
//

#ifndef IDIZJDZLDFSFDSFLSDFDSFSDRP2MN34LJK
#define IDIZJDZLDFSFDSFLSDFDSFSDRP2MN34LJK

///// Includes /////

#include <QObject>
#include <QSharedPointer>
#include <QVector>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Decalrations /////

class Media;

///// Classes /////

class MediaMgr : public QObject
{
 Q_OBJECT

 public:

  MediaMgr();
  ~MediaMgr();

  void Init();
  void Destroy();

  QSharedPointer<Media> AddMedia(const QString& path);
  int RemoveMedia(QSharedPointer<Media>& media);

 protected:


 private:

  QVector< QSharedPointer<Media> > media_;

 signals:

  void MediaAdded(QSharedPointer<Media>& media);
  void MediaRemoved(QSharedPointer<Media>& media);

};

}

QT_END_NAMESPACE

#endif
