// videowindowmgr.h
// 

#ifndef ID6R9UTDK6T7T27AH4EI8QT9QQ9OFMLU4O
#define ID6R9UTDK6T7T27AH4EI8QT9QQ9OFMLU4O

///// Includes /////

#include <boost/optional.hpp>
#include <QObject>
#include <vector>

#include "videowindow.h"

///// Namespaces /////

namespace client
{

///// Classes /////

class VideoWindowMgr : public QObject
{
 Q_OBJECT

 public:

  VideoWindowMgr();
  ~VideoWindowMgr();

  void Destroy();

  VideoWindow* CreateVideoWindow(const boost::optional<QPoint>& location, const QResource* arial, const QIcon& showfullscreen, const unsigned int videowindowwidth, const unsigned int videowindowheight, const bool showtoolbar); // Not specifying location will place it in a default location Qt chooses

  const std::vector<VideoWindow*>& GetVideoWindows() { return videowindows_; }

 private:

  std::vector<VideoWindow*> videowindows_;

  unsigned int videowindowindex_; // The number that goes into the title of the Video Window that the user can refer to(increments every time a new VideoWindow is created)

 private slots:

  void VideoWindowDestroyed(QObject* videowindow);

};

}

#endif
