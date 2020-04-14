// videowidgetsmgr.cpp
//

///// Includes /////

#include "monocleclient/videowidgetsmgr.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <QMessageBox>

#include "monocleclient/findmotionwindow.h"
#include "monocleclient/findobjectwindow.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingtrack.h"
#include "monocleclient/view.h"
#include "monocleclient/videochartview.h"

///// Namespaces /////

namespace client
{

///// Methods /////

VideoWidgetsMgr::VideoWidgetsMgr(const QResource* arial, const QIcon& showfullscreen) :
  arial_(arial),
  showfullscreen_(showfullscreen),
  selectionviewrect_(0, 0),
  selectionpoint_(0, 0)
{
  
}

VideoWidgetsMgr::~VideoWidgetsMgr()
{

}

void VideoWidgetsMgr::Init()
{

}

void VideoWidgetsMgr::CreateMapView(const boost::shared_ptr<Device>& device, const QSharedPointer<Map>& map)
{
  // Find a video widget to add this video view to
  for (VideoWidget* videowidget : videowidgets_)
  {
    const auto location = GetEmptyVideoLocation(videowidget, 1, 1);
    if (location.is_initialized())
    {
      QSharedPointer<MapView> mapview = videowidget->CreateMapView(location->x(), location->y(), 1, 1, Options::Instance().GetStretchVideo(), device, map);
      if (!mapview)
      {
        LOG_GUI_WARNING(QString("VideoWidget::CreateMapView failed"));

      }
      else
      {
        emit MapViewCreated(mapview);

      }
      return;
    }
  }

  // If we couldn't find somewhere to put it, create a video window for it
  VideoWindow* videowindow = MainWindow::Instance()->GetVideoWindowMgr().CreateVideoWindow(boost::none, arial_, showfullscreen_, Options::Instance().GetDefaultVideoWindowWidth(), Options::Instance().GetDefaultVideoWindowHeight(), Options::Instance().GetDefaultShowToolbar());
  QSharedPointer<MapView> mapview = videowindow->GetVideoWidget()->CreateMapView(0, 0, 1, 1, Options::Instance().GetStretchVideo(), device, map);
  if (!mapview)
  {
    LOG_GUI_WARNING(QString("VideoWidget::CreateMapView failed"));

  }
  else
  {
    emit MapViewCreated(mapview);

  }
}

void VideoWidgetsMgr::CreateMediaView(const QSharedPointer<Media>& media, const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex)
{
  // Find a video widget to add this video view to
  for (VideoWidget* videowidget : videowidgets_)
  {
    const auto location = GetEmptyVideoLocation(videowidget, 1, 1);
    if (location.is_initialized())
    {
      QSharedPointer<MediaView> mediaview = videowidget->CreateMediaView(location->x(), location->y(), 1, 1, Options::Instance().GetStretchVideo(), media, deviceindex, recordingindex, trackindex);
      if (!mediaview)
      {
        LOG_GUI_WARNING(QString("VideoWidget::CreateMediaView failed"));

      }
      else
      {
        emit MediaViewCreated(mediaview);

      }
      return;
    }
  }

  // If we couldn't find somewhere to put it, create a video window for it
  VideoWindow* videowindow = MainWindow::Instance()->GetVideoWindowMgr().CreateVideoWindow(boost::none, arial_, showfullscreen_, Options::Instance().GetDefaultVideoWindowWidth(), Options::Instance().GetDefaultVideoWindowHeight(), Options::Instance().GetDefaultShowToolbar());
  QSharedPointer<MediaView> mediaview = videowindow->GetVideoWidget()->CreateMediaView(0, 0, 1, 1, Options::Instance().GetStretchVideo(), media, deviceindex, recordingindex, trackindex);
  if (!mediaview)
  {
    LOG_GUI_WARNING(QString("VideoWidget::CreateMediaView failed"));

  }
  else
  {
    emit MediaViewCreated(mediaview);

  }
}

void VideoWidgetsMgr::CreateVideoView(const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingTrack>& track)
{
  const bool adaptivestreaming = track ? false : recording->GetAdaptiveStreaming();

  // Find a video widget to add this video view to
  for (VideoWidget* videowidget : videowidgets_)
  {
    const auto location = GetEmptyVideoLocation(videowidget, 1, 1);
    if (location.is_initialized())
    {
      QSharedPointer<VideoView> videoview = videowidget->CreateVideoView(location->x(), location->y(), 1, 1, Options::Instance().GetStretchVideo(), adaptivestreaming, device, recording, track);
      if (!videoview)
      {
        LOG_GUI_WARNING_SOURCE(device, QString("VideoWidget::CreateVideoView failed"));

      }
      else
      {
        emit VideoViewCreated(videoview);

      }
      return;
    }
  }

  // If we couldn't find somewhere to put it, create a video window for it
  VideoWindow* videowindow = MainWindow::Instance()->GetVideoWindowMgr().CreateVideoWindow(boost::none, arial_, showfullscreen_, Options::Instance().GetDefaultVideoWindowWidth(), Options::Instance().GetDefaultVideoWindowHeight(), Options::Instance().GetDefaultShowToolbar());
  QSharedPointer<VideoView> videoview = videowindow->GetVideoWidget()->CreateVideoView(0, 0, 1, 1, Options::Instance().GetStretchVideo(), adaptivestreaming, device, recording, track);
  if (!videoview)
  {
    LOG_GUI_WARNING_SOURCE(device, QString("VideoWidget::CreateVideoView"));
    
  }
  else
  {
    emit VideoViewCreated(videoview);
    
  }
}

void VideoWidgetsMgr::CreateVideoChartView(const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const std::vector< QSharedPointer<client::RecordingTrack> >& tracks)
{
  // Find a video widget to add this video view to
  for (VideoWidget* videowidget : videowidgets_)
  {
    const auto location = GetEmptyVideoLocation(videowidget, 1, 1);
    if (location.is_initialized())
    {
      QSharedPointer<VideoChartView> videochartview = videowidget->CreateVideoChartView(location->x(), location->y(), 1, 1, device, recording, tracks);
      if (!videochartview)
      {
        LOG_GUI_WARNING_SOURCE(device, QString("VideoWidget::CreateVideoChartView failed"));

      }
      else
      {
        emit VideoChartViewCreated(videochartview);

      }
      return;
    }
  }

  // If we couldn't find somewhere to put it, create a video window for it
  VideoWindow* videowindow = MainWindow::Instance()->GetVideoWindowMgr().CreateVideoWindow(boost::none, arial_, showfullscreen_, Options::Instance().GetDefaultVideoWindowWidth(), Options::Instance().GetDefaultVideoWindowHeight(), Options::Instance().GetDefaultShowToolbar());
  QSharedPointer<VideoChartView> videochartview = videowindow->GetVideoWidget()->CreateVideoChartView(0, 0, 1, 1, device, recording, tracks);
  if (!videochartview)
  {
    LOG_GUI_WARNING_SOURCE(device, QString("VideoWidget::CreateVideoChartView"));

  }
  else
  {
    emit VideoChartViewCreated(videochartview);

  }
}

uint64_t VideoWidgetsMgr::GetNumViews(const std::vector<VIEWTYPE>& viewtypes) const
{
  uint64_t count = 0;
  for (VideoWidget* videowidget : videowidgets_)
  {
    for (QSharedPointer<View>& view : videowidget->GetViews())
    {
      if (utility::Contains(viewtypes, view->GetViewType()))
      {
        ++count;

      }
    }
  }
  return count;
}

void VideoWidgetsMgr::ResetViews()
{
  for (VideoWidget* videowidget : videowidgets_)
  {
    videowidget->makeCurrent();
    for (QSharedPointer<View>& view : videowidget->GetViews())
    {
      view->ResetPosition(false);

    }
    videowidget->doneCurrent();
  }
}

std::vector< QSharedPointer<View> > VideoWidgetsMgr::GetViews()
{
  std::vector< QSharedPointer<View> > views;
  for (auto& videowidget : videowidgets_)
  {
    views.insert(views.end(), videowidget->GetViews().begin(), videowidget->GetViews().end());

  }
  return views;
}

void VideoWidgetsMgr::SetUpdateFrequency(const int frequency)
{
  for (auto& videowidget : videowidgets_)
  {
    videowidget->SetUpdateFrequency(frequency);

  }
}

QSharedPointer<View> VideoWidgetsMgr::GetLastSelectedView() const
{
  for (auto& view : selectedstack_)
  {
    auto v = view.lock();
    if (v)
    {

      return v;
    }
  }
  return QSharedPointer<View>();
}

void VideoWidgetsMgr::VideoWidgetCreated(VideoWidget* videowidget)
{
  videowidgets_.push_back(videowidget);

}

void VideoWidgetsMgr::VideoWidgetDestroyed(VideoWidget* videowidget)
{
  auto i = std::find_if(videowidgets_.begin(), videowidgets_.end(), [videowidget](const VideoWidget* i) { return (i == videowidget); });
  if (i == videowidgets_.end())
  {

    return;
  }
  videowidgets_.erase(i);
}

void VideoWidgetsMgr::MouseEnterEvent(QEvent* event)
{

}

void VideoWidgetsMgr::MouseLeaveEvent(QEvent* event)
{

}

void VideoWidgetsMgr::MouseMoveEvent(QMouseEvent* event)
{
  if (MainWindow::Instance()->GetMouseState() == MOUSESTATE_COLOURPICKER)
  {
    VideoWidget* videowidget = GetVideoWidget(event->globalPos());
    if (videowidget == nullptr)
    {

      return;
    }

    const QPoint pos = videowidget->mapFromGlobal(event->globalPos());
    QSharedPointer<View> view = videowidget->GetView(pos);
    if (!view || (view->GetViewType() != VIEWTYPE_MONOCLE)) // Find motion only works on video
    {

      return;
    }

    if (event->buttons() & Qt::LeftButton)
    {
      ColourPicker(view, pos);

    }
  }
  else if (MainWindow::Instance()->GetMouseState() == MOUSESTATE_SELECT)
  {
    if (selectionview_ && (event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::CTRL))
    {
      QSharedPointer<View> view = selectionview_.lock();
      if (view)
      {
        const QRect rect = view->GetPixelRect();
        const QPoint pos = QPoint(event->pos().x() - rect.x(), event->pos().y() - rect.y());
        view->MoveCentre(static_cast<float>(selectionpoint_.x() - pos.x()) / static_cast<float>(rect.width()), static_cast<float>(selectionpoint_.y() - pos.y()) / static_cast<float>(rect.height()));
        selectionpoint_ = pos;
      }
    }
  }
}

void VideoWidgetsMgr::MousePressEvent(QMouseEvent* event)
{
  VideoWidget* videowidget = GetVideoWidget(event->globalPos());
  if (videowidget == nullptr)
  {

    return;
  }

  const QPoint pos = videowidget->mapFromGlobal(event->globalPos());
  QSharedPointer<View> view = videowidget->GetView(pos);
  if ((MainWindow::Instance()->GetMouseState() == MOUSESTATE_FINDMOTION) || (MainWindow::Instance()->GetMouseState() == MOUSESTATE_FINDOBJECT))
  {
    if (!view || (view->GetViewType() != VIEWTYPE_MONOCLE)) // Find motion only works on video
    {

      return;
    }
    
    switch (event->button())
    {
      case Qt::LeftButton:
      {
        selectionview_ = view;
        selectionviewrect_ = pos;
        break;
      }
      case Qt::RightButton:
      {

        break;
      }
    }
  }
  else if (MainWindow::Instance()->GetMouseState() == MOUSESTATE_COLOURPICKER)
  {
    if (!view || ((view->GetViewType() != VIEWTYPE_MONOCLE) && (view->GetViewType() != VIEWTYPE_MEDIA)))
    {

      return;
    }

    switch (event->button())
    {
      case Qt::LeftButton:
      {
        ColourPicker(view, pos);

      }
      case Qt::RightButton:
      {

        break;
      }
    }
  }
  else // SELECT(Default)
  {
    switch (event->button())
    {
      case Qt::LeftButton:
      {
        if (view)
        {
          selectionview_ = view;
          const QRect rect = view->GetPixelRect();
          selectionviewrect_ = QPoint((static_cast<float>(pos.x() - rect.x()) / static_cast<float>(rect.width())) * view->GetRect().width(), static_cast<float>(pos.y() - rect.y()) / static_cast<float>(rect.height()) * view->GetRect().height());
          selectionpoint_ = QPoint(pos.x() - rect.x(), pos.y() - rect.y());
        }
      }
      case Qt::RightButton:
      {

        break;
      }
    }
  }
}

void VideoWidgetsMgr::MouseReleaseEvent(QMouseEvent* event)
{
  QSharedPointer<View> selectionview = selectionview_.lock(); // This is the previously selected view with the corresponding mouse press event
  selectionview_.clear();
  VideoWidget* videowidget = GetVideoWidget(event->globalPos());

  if ((MainWindow::Instance()->GetMouseState() == MOUSESTATE_FINDMOTION) || (MainWindow::Instance()->GetMouseState() == MOUSESTATE_FINDOBJECT))
  {
    if (!selectionview)
    {

      return;
    }

    switch (event->button())
    {
      case Qt::LeftButton:
      {
        if (selectionview->GetViewType() != VIEWTYPE_MONOCLE) // Only works on monocle views...
        {

          return;
        }

        boost::optional<uint64_t> starttime = MainWindow::Instance()->GetPlaybackWidget()->GetExportStartTime();
        boost::optional<uint64_t> endtime = MainWindow::Instance()->GetPlaybackWidget()->GetExportEndTime();
        const auto now = std::chrono::system_clock::now();
        if (!starttime.is_initialized())
        {
          starttime = std::chrono::duration_cast<std::chrono::milliseconds>((now - std::chrono::hours(1)).time_since_epoch()).count();

        }
        if (!endtime.is_initialized())
        {
          endtime = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

        }
        const QPoint selectionpoint = MainWindow::Instance()->GetVideoWidgetsMgr().GetSelectionViewRect();
        const QPoint cursor = videowidget->mapFromGlobal(QCursor::pos());
        const QRect imagepixelrect = selectionview->GetImagePixelRect();
        const QRect selectedrect(QPoint(std::max(imagepixelrect.x(), std::min(selectionpoint.x(), cursor.x())) - imagepixelrect.x(),
                                        std::max(imagepixelrect.y(), std::min(selectionpoint.y(), cursor.y())) - imagepixelrect.y()),
                                 QPoint(std::min(imagepixelrect.right(), std::max(selectionpoint.x(), cursor.x())) - imagepixelrect.x(), std::min(imagepixelrect.bottom(),
                                        std::max(selectionpoint.y(), cursor.y())) - imagepixelrect.y()));
        if ((selectedrect.width() < 6) || (selectedrect.height() < 6))
        {
          // Ignore this, because the user has made too small of a selection
          return;
        }
        const QRectF selectedrectf = ImageToRect(imagepixelrect, selectedrect, selectionview->GetMirror(), selectionview->GetRotation());
        MainWindow::Instance()->ResetMouseState();
        const QSharedPointer<VideoView> videoview = selectionview.staticCast<VideoView>();
        if (!videoview->GetDevice()->SupportsFindMotion())
        {
          QMessageBox(QMessageBox::Warning, tr("Error"), tr("Server does not support find motion"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
          return;
        }

        if (MainWindow::Instance()->GetMouseState() == MOUSESTATE_FINDMOTION)
        {
          FindMotionWindow(videowidget, videoview->GetQImage(boost::none), videoview->GetDevice(), videoview->GetRecording(), videoview->GetTrack(), videoview->GetSelectedColour(), *starttime, *endtime, selectedrectf, videoview->GetImageWidth(), videoview->GetImageHeight(), videoview->GetMirror(), videoview->GetRotation(), videoview->GetStretch()).exec();

        }
        else // (MainWindow::Instance()->GetMouseState() == MOUSESTATE_FINDOBJECT)
        {
          if (!videoview->GetDevice()->SupportsFindObject())
          {
            QMessageBox(QMessageBox::Warning, tr("Error"), tr("Device does not support find object, please upgrade"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
            return;
          }
          
          if (!videoview->GetRecording()->HasObjectDetectorTracks())
          {
            QMessageBox(QMessageBox::Warning, tr("Error"), tr("Recording does not contain any object detectors"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
            return;
          }

          FindObjectWindow(videowidget, videoview->GetQImage(boost::none), videoview->GetDevice(), videoview->GetRecording(), videoview->GetTrack(), videoview->GetSelectedColour(), *starttime, *endtime, selectedrectf, videoview->GetImageWidth(), videoview->GetImageHeight(), videoview->GetMirror(), videoview->GetRotation(), videoview->GetStretch(), videoview->GetShowInfo(), videoview->GetShowObjects()).exec();
        }
      }
    }
  }
  else
  {
    switch (event->button())
    {
      case Qt::LeftButton:
      {
        if (videowidget)
        {
          const QPoint pos = videowidget->mapFromGlobal(event->globalPos());
          QSharedPointer<View> view = videowidget->GetView(pos);
          if (view) // We released the left mouse button on top of a video view
          {
            if (selectionview == view) // If we pressed left click on the same video view as we released
            {
              if (event->modifiers() & Qt::SHIFT)
              {
                QSharedPointer<View> pivotview = GetLastSelection();
                if (!pivotview) // If we don't have a previous selection, just select this one item
                {
                  UnselectAll();
                  Select(view, true);
                }
                else
                {
                  if (pivotview->GetVideoWidget() == videowidget) // If we're clicking on the same video widget, select like normal
                  {
                    // Unselect all, then select a square around the previously selected and the currently selected
                    std::vector< QSharedPointer<View> > selectedlist;
                    const int xmin = std::min(pivotview->GetRect().x(), view->GetRect().x());
                    const int xmax = std::max(pivotview->GetRect().x(), view->GetRect().x());
                    const int ymin = std::min(pivotview->GetRect().y(), view->GetRect().y());
                    const int ymax = std::max(pivotview->GetRect().y(), view->GetRect().y());
                    for (int x = xmin; x <= xmax; ++x)
                    {
                      for (int y = ymin; y <= ymax; ++y)
                      {
                        auto selectedview = videowidget->GetView(x, y);
                        if (selectedview)
                        {
                          selectedlist.push_back(selectedview);

                        }
                      }
                    }

                    UnselectAll();
                    Select(pivotview, true);
                    for (auto& selected : selectedlist)
                    {
                      selected->SetSelected(true);// We want to avoid setting the last selected when using shift, so that we can continue to click elsewhere with the same pivot point
                      Selected(selected, true);
                    }
                  }
                  else // If we shift selected accross different video widgets, just select the single video view on the new widget
                  {
                    UnselectAll();
                    Select(view, true);
                  }
                }
              }
              else if (event->modifiers() & Qt::CTRL)
              {
                Select(view, !view->GetSelected());

              }
              else
              {
                const QPoint location = videowidget->GetLocation(pos.x(), pos.y());
                const QPoint relativelocation = location - QPoint(selectionview->GetRect().x(), selectionview->GetRect().y());
                if (selectionviewrect_ == relativelocation) // If user didn't move the mouse while pressed
                {
                  const auto selectedviews = GetSelectedViews();
                  if ((selectedviews.size() == 1) && utility::Contains(selectedviews, view))
                  {
                    const bool selected = !view->GetSelected();
                    UnselectAll();
                    Select(view, selected);
                  }
                  else
                  {
                    UnselectAll();
                    Select(view, true);
                  }
                }
                else // User moved the mouse while pressed
                {
                  MoveView(videowidget, selectionview, location - selectionviewrect_);

                }
              }
            }
            else if (selectionview) // If we pressed left click on a video view, then released it on top of a different video view
            {
              const auto selectedviews = GetSelectedViews();
              if (utility::Contains(selectedviews, selectionview)) // Are me moving a group(could be of size 1)
              {
                if (selectedviews.size() == 1) // User tried to move a single selected video view onto another
                {
                  Swap(selectionview, view);

                }
                else // User tried to move many video views onto another
                {

                }
              }
              else // We are moving a single videoview which has been selected onto another
              {
                Swap(selectionview, view);

              }
            }
            else // If we pressed left click in an empty space and released it on a video view
            {

            }
          }
          else // We are moving to an empty video slot
          {
            if (event->modifiers() & Qt::CTRL)
            {

              break;
            }

            if (selectionview) // If we were moving a single video view
            {
              std::vector< QSharedPointer<View> > selectedviews = GetSelectedViews();
              if (utility::Contains(selectedviews, selectionview)) // We have pressed on an already selected video view, and have moved it to an empty space
              {
                if (selectedviews.size() == 1)
                {
                  MoveView(videowidget, selectionview, videowidget->GetLocation(pos.x(), pos.y()) - selectionviewrect_);

                }
                else // Moving multiple video views
                {
                  if (selectionview->GetVideoWidget() == videowidget) // If we are moving a group of video views within the same video widget
                  {
                    std::vector< QSharedPointer<View> > localselectedviews = std::vector< QSharedPointer<View> >(selectedviews.begin(), std::partition(selectedviews.begin(), selectedviews.end(), [selectionview](const QSharedPointer<View>& view) { return (view->GetVideoWidget() == selectionview->GetVideoWidget()); })); // Get all the selected video views from the source video widget
                    const QPoint relativepos = videowidget->GetLocation(pos.x(), pos.y()) - (selectionview->GetRect().topLeft() + selectionviewrect_);
                    bool moved = true;
                    while (!localselectedviews.empty() && moved) // Move as many as we can and ignore others
                    {
                      moved = false;
                      for (auto localselectedview = localselectedviews.begin(); localselectedview != localselectedviews.end(); ++localselectedview)
                      {
                        if (MoveView(videowidget, *localselectedview, (*localselectedview)->GetRect().topLeft() + relativepos))
                        {
                          localselectedviews.erase(localselectedview);
                          moved = true;
                          break;
                        }
                      }
                    }
                  }
                  else // Moving a group of video views to another video widget
                  {
                    std::vector< QSharedPointer<View> > localselectedviews = std::vector< QSharedPointer<View> >(selectedviews.begin(), std::partition(selectedviews.begin(), selectedviews.end(), [selectionview](const QSharedPointer<View>& view) { return (view->GetVideoWidget() == selectionview->GetVideoWidget()); })); // Get all the selected video views from the source video widget
                    const QPoint relativepos = videowidget->GetLocation(pos.x(), pos.y()) - (selectionview->GetRect().topLeft() + selectionviewrect_);
                    bool moved = true;
                    while (!localselectedviews.empty() && moved) // Move as many as we can and ignore others
                    {
                      moved = false;
                      for (auto localselectedview = localselectedviews.begin(); localselectedview != localselectedviews.end(); ++localselectedview)
                      {
                        if (MoveView(videowidget, *localselectedview, (*localselectedview)->GetRect().topLeft() + relativepos))
                        {
                          localselectedviews.erase(localselectedview);
                          moved = true;
                          break;
                        }
                      }
                    }

                    // If there are any video views that did not get moved to a nice relative location, we should just dump them across
                    for (auto& localselectedview : localselectedviews)
                    {
                      const auto location = GetEmptyVideoLocation(videowidget, localselectedview->GetRect().width(), localselectedview->GetRect().height());
                      if (location.is_initialized())
                      {
                        MoveView(videowidget, localselectedview, *location);

                      }
                    }
                  }
                }
              }
              else // We could have many or none selected, but we have clicked on a nonselected video view, and released on an empty space
              {
                UnselectAll();
                MoveView(videowidget, selectionview, videowidget->GetLocation(pos.x(), pos.y()) - selectionviewrect_);
              }
            }
            else // If we weren't moving
            {
              UnselectAll();

            }
          }
        }
        else // Left click release away from a video widget
        {
          if (event->modifiers() & Qt::CTRL)
          {

            break;
          }

          if (selectionview) // We left clicked on a video and released outside a video widget
          {
            auto selectedviews = GetSelectedViews();
            std::vector< QSharedPointer<View> > localselectedviews = std::vector< QSharedPointer<View> >(selectedviews.begin(), std::partition(selectedviews.begin(), selectedviews.end(), [selectionview](const QSharedPointer<View>& view) { return (view->GetVideoWidget() == selectionview->GetVideoWidget()); })); // Get all the selected video views from the source video widget
            if (utility::Contains(localselectedviews, selectionview)) // Are me moving a group(could be of size 1)
            {
              // Get min/max width/height and create a video window
              const auto minx = *std::min_element(localselectedviews.begin(), localselectedviews.end(), [](const QSharedPointer<View>& lhs, const QSharedPointer<View>& rhs) { return (lhs->GetRect().x() < rhs->GetRect().x()); });
              const auto maxx = *std::max_element(localselectedviews.begin(), localselectedviews.end(), [](const QSharedPointer<View>& lhs, const QSharedPointer<View>& rhs) { return ((lhs->GetRect().x() + lhs->GetRect().width()) < (rhs->GetRect().x() + rhs->GetRect().width())); });
              const auto miny = *std::min_element(localselectedviews.begin(), localselectedviews.end(), [](const QSharedPointer<View>& lhs, const QSharedPointer<View>& rhs) { return (lhs->GetRect().y() < rhs->GetRect().y()); });
              const auto maxy = *std::max_element(localselectedviews.begin(), localselectedviews.end(), [](const QSharedPointer<View>& lhs, const QSharedPointer<View>& rhs) { return ((lhs->GetRect().y() + lhs->GetRect().height()) < (rhs->GetRect().y() + rhs->GetRect().height())); });

              const unsigned int width = std::max(static_cast<unsigned int>((maxx->GetRect().x() + maxx->GetRect().width()) - minx->GetRect().x()), Options::Instance().GetDefaultVideoWindowWidth());
              const unsigned int height = std::max(static_cast<unsigned int>((maxy->GetRect().y() + maxy->GetRect().height()) - miny->GetRect().y()), Options::Instance().GetDefaultVideoWindowHeight());

              VideoWindow* videowindow = MainWindow::Instance()->GetVideoWindowMgr().CreateVideoWindow(event->globalPos(), arial_, showfullscreen_, width, height, Options::Instance().GetDefaultShowToolbar());
              if (!videowindow)
              {
                LOG_GUI_WARNING(QString("VideoWindowMgr::CreateVideoWindow"));
                return;
              }

              // Move the video views into the new window
              for (auto& localselectedview : localselectedviews)
              {
                MoveView(videowindow->GetVideoWidget(), localselectedview, localselectedview->GetRect().topLeft() - QPoint(minx->GetRect().x(), miny->GetRect().y()));

              }
            }
            else // Left clicked on a video view that was not selected and released
            {
              VideoWindow* videowindow = MainWindow::Instance()->GetVideoWindowMgr().CreateVideoWindow(event->globalPos(), arial_, showfullscreen_, std::max(static_cast<unsigned int>(selectionview->GetRect().width()), Options::Instance().GetDefaultVideoWindowWidth()), std::max(static_cast<unsigned>(selectionview->GetRect().height()), Options::Instance().GetDefaultVideoWindowWidth()), Options::Instance().GetDefaultShowToolbar());
              if (!videowindow)
              {
                LOG_GUI_WARNING(QString("VideoWindowMgr::CreateVideoWindow"));
                return;
              }

              MoveView(videowindow->GetVideoWidget(), selectionview, QPoint(0, 0));
            }
          }
        }

        break;
      }
      case Qt::RightButton:
      {
        if (videowidget)
        {
          const auto pos = videowidget->mapFromGlobal(event->globalPos());
          auto view = videowidget->GetView(pos);

          QMenu menu;
          if (videowidget->GetToolbar()->isHidden())
          {
            menu.addAction(videowidget->GetShowToolbarAction());

          }
          else
          {
            menu.addAction(videowidget->GetHideToolbarAction());

          }

          if (view)
          {
            view->GetMenu(menu);

          }

          menu.exec(event->globalPos());
        }
        else // Right click release away from a video widget
        {

        }

        break;
      }
    }
  }
}

QSharedPointer<View> VideoWidgetsMgr::GetLastSelection() const
{
  for (auto view = selectedstack_.rbegin(); view != selectedstack_.rend(); ++view)
  {
    QSharedPointer<View> v = view->lock();
    if (v)
    {

      return v;
    }
  }
  return QSharedPointer<View>();
}

void VideoWidgetsMgr::Swap(QSharedPointer<View>& lhs, QSharedPointer<View>& rhs)
{
  // Get everything
  auto videowidget1 = lhs->GetVideoWidget();
  auto videowidget2 = rhs->GetVideoWidget();

  auto view1 = std::find(videowidget1->GetViews().begin(), videowidget1->GetViews().end(), lhs);
  if (view1 == lhs->GetVideoWidget()->GetViews().end())
  {

    return;
  }
  QRect rect1 = (*view1)->GetRect();

  auto view2 = std::find(videowidget2->GetViews().begin(), videowidget2->GetViews().end(), rhs);
  if (view2 == rhs->GetVideoWidget()->GetViews().end())
  {

    return;
  }
  QRect rect2 = (*view2)->GetRect();

  // Swap everything
  auto i = (*view1);
  auto j = (*view2);

  if (videowidget1 != videowidget2) // We only need to swap video widgets if they differ
  {
    (*view2)->SetVideoWidget(videowidget1);
    (*view1)->SetVideoWidget(videowidget2);

    videowidget1->GetViews().erase(view1);
    videowidget2->GetViews().erase(view2);

    videowidget2->GetViews().push_back(i);
    videowidget1->GetViews().push_back(j);
  }

  i->SetPosition(videowidget2, rect2.x(), rect2.y(), rect2.width(), rect2.height(), i->GetRotation(), i->GetMirror(), i->GetStretch(), true);
  j->SetPosition(videowidget1, rect1.x(), rect1.y(), rect1.width(), rect1.height(), j->GetRotation(), j->GetMirror(), j->GetStretch(), true);
}

bool VideoWidgetsMgr::MoveView(VideoWidget* videowidget, QSharedPointer<View>& view, QPoint location)
{
  if (location.x() < 0)
  {
    location.setX(0);

  }
  if ((location.y()) < 0)
  {
    location.setY(0);

  }
  if ((location.x() + view->GetRect().width()) > static_cast<int>(videowidget->GetWidth()))
  {
    location.setX(videowidget->GetWidth() - view->GetRect().width());

  }
  if ((location.y() + view->GetRect().height()) > static_cast<int>(videowidget->GetHeight()))
  {
    location.setY(videowidget->GetHeight() - view->GetRect().height());

  }

  // Make sure we're not going to overwrite any other video views
  if (!videowidget->IsEmpty(view, location.x(), location.y(), view->GetRect().width(), view->GetRect().height()))
  {

    return false;
  }

  // Put the video view in the correct place
  view->GetVideoWidget()->RemoveView(view);
  videowidget->GetViews().push_back(view);
  view->SetPosition(videowidget, location.x(), location.y(), view->GetRect().width(), view->GetRect().height(), view->GetRotation(), view->GetMirror(), view->GetStretch(), true);
  videowidget->ResetGrid();
  return true;
}

std::vector< QSharedPointer<View> > VideoWidgetsMgr::GetSelectedViews() const
{
  std::vector< QSharedPointer<View> > result;
  for (auto& videowidget : videowidgets_)
  {
    for (auto& view : videowidget->GetViews())
    {
      if (view->GetSelected())
      {
        result.push_back(view);

      }
    }
  }
  return result;
}

VideoWidget* VideoWidgetsMgr::GetVideoWidget(const QPoint& globalpos)
{
  // Find the video widget at the location(if there is one) and map the position to it
  // The reason we do this is because if left mouse is held, the events are not passed through to the underlying widget, but to the widget where the mouse was originaly pressed
  QWidget* window = QApplication::topLevelAt(globalpos); // We could use widgetAt() but our implementation should be faster, because we don't have to search through all widgets, but only top level windows, then look at the single VideoWiget*
  if (window == nullptr)
  {

    return nullptr;
  }

  VideoWidget* videowidget = nullptr;
  if (MainWindow::Instance() == window)
  {
    if (!MainWindow::Instance()->GetVideoWidget()->rect().contains(MainWindow::Instance()->GetVideoWidget()->mapFromGlobal(globalpos))) // Ignore movement outside the videowidget
    {

      return nullptr;
    }

    return MainWindow::Instance()->GetVideoWidget();
  }
  else
  {
    auto videowidget = std::find_if(videowidgets_.begin(), videowidgets_.end(), [window](const auto& videowidget) { return (videowidget->parent()->parent() == window); });
    if (videowidget == videowidgets_.end())
    {

      return nullptr;
    }

    if (!(*videowidget)->rect().contains((*videowidget)->mapFromGlobal(globalpos))) // Ignore movement outside the videowidget
    {

      return nullptr;
    }

    return (*videowidget);
  }
}

boost::optional<QPoint> VideoWidgetsMgr::GetEmptyVideoLocation(VideoWidget* videowidget, unsigned int width, unsigned int height)
{
  for (unsigned int y = 0; y < videowidget->GetHeight(); ++y)
  {
    for (unsigned int x = 0; x < videowidget->GetWidth(); ++x)
    {
      if (videowidget->GetView(x, y).isNull())
      {

        return QPoint(x, y);
      }
    }
  }

  return boost::none;
}

void VideoWidgetsMgr::Select(QSharedPointer<View>& view, bool select)
{
  auto i = std::find(selectedstack_.begin(), selectedstack_.end(), view); // Remove it if it exists in the stack already
  if (i != selectedstack_.end())
  {
    selectedstack_.erase(i);

  }

  if (view->GetSelected() != select)
  {
    view->SetSelected(select);
    Selected(view, select);
  }

  if (select)
  {
    selectedstack_.push_back(view); // Add it to the top

  }
}

void VideoWidgetsMgr::UnselectAll()
{
  for (auto videowidget : videowidgets_)
  {
    for (auto& view : videowidget->GetViews())
    {
      Select(view, false);

    }
  }
  selectedstack_.clear();
}

void VideoWidgetsMgr::ColourPicker(const QSharedPointer<View>& view, const QPoint& pos)
{
  const QRect imagepixelrect = view->GetImagePixelRect();
  if ((pos.x() < imagepixelrect.x()) || (pos.x() > imagepixelrect.right()) || (pos.y() < imagepixelrect.y()) || (pos.y() > imagepixelrect.bottom()))
  {
    // Ignore clicks outside the area(black bars mostly...)
    return;
  }

  const int x = (static_cast<float>((pos.x() - imagepixelrect.x())) / static_cast<float>(imagepixelrect.width())) * view->GetImageWidth();
  const int y = (static_cast<float>((pos.y() - imagepixelrect.y())) / static_cast<float>(imagepixelrect.height())) * view->GetImageHeight();
  const QColor colour = view->GetPixelColour(x, y);
  MainWindow::Instance()->SetColourPickerColour(QVector3D(colour.redF(), colour.greenF(), colour.blueF()));
}

}
