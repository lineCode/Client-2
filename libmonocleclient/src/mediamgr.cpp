// mediamgr.cpp
//

///// Includes /////

#include "monocleclient/mediamgr.h"

#include <QMessageBox>

#include "monocleclient/media.h"

///// Namespaces /////

namespace client
{

///// Methods /////

MediaMgr::MediaMgr() :
  QObject(nullptr)
{
  
}

MediaMgr::~MediaMgr()
{
  Destroy();

}

void MediaMgr::Init()
{
  Destroy();

}

void MediaMgr::Destroy()
{

}

QSharedPointer<Media> MediaMgr::AddMedia(const QString& path)
{
  QSharedPointer<Media> media = QSharedPointer<Media>::create(path);
  media->Init();
  media_.push_back(media);
  emit MediaAdded(media);
  return media;
}

int MediaMgr::RemoveMedia(QSharedPointer<Media>& media)
{
  const int index = media_.indexOf(media);
  if (index == -1)
  {
    media->Destroy();
    return 1;
  }
  media_.remove(index);
  media->Destroy();
  emit MediaRemoved(media);
  return 0;
}

}
