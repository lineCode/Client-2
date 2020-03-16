// videowindowmgr.cpp
//

///// Includes /////

#include "monocleclient/videowindowmgr.h"

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Methods /////

VideoWindowMgr::VideoWindowMgr() :
  videowindowindex_(0)
{
  
}

VideoWindowMgr::~VideoWindowMgr()
{

}

void VideoWindowMgr::Destroy()
{
  for (VideoWindow* videowindow : videowindows_)
  {
    videowindow->close();

  }
  videowindows_.clear();

}

VideoWindow* VideoWindowMgr::CreateVideoWindow(const boost::optional<QPoint>& location, const QResource* arial, const QIcon& showfullscreen, const unsigned int videowindowwidth, const unsigned int videowindowheight, const bool showtoolbar)
{
  VideoWindow* videowindow = new VideoWindow(++videowindowindex_, arial, showfullscreen, videowindowwidth, videowindowheight, showtoolbar);
  connect(videowindow, &VideoWindow::destroyed, this, &VideoWindowMgr::VideoWindowDestroyed);
  if (location.is_initialized())
  {
    videowindow->move(*location);

  }
  videowindow->show();
  videowindows_.push_back(videowindow);
  return videowindow;
}

void VideoWindowMgr::VideoWindowDestroyed(QObject* videowindow)
{
  std::vector<VideoWindow*>::iterator i = std::find_if(videowindows_.begin(), videowindows_.end(), [videowindow](const VideoWindow* i) { return (i == videowindow); });
  if (i == videowindows_.end())
  {

    return;
  }

  // Unselect the video views contained in this window
  for (QSharedPointer<View>& view : (*i)->GetVideoWidget()->GetViews())
  {
    if (view->GetSelected())
    {
      emit MainWindow::Instance()->GetVideoWidgetsMgr().Selected(view, false);

    }
  }

  videowindows_.erase(i);
}

}
