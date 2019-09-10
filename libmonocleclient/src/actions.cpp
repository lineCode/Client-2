// actions.cpp
//

///// Includes /////

#include "monocleclient/actions.h"

#include <onvifclient/ptzclient.hpp>

#include "monocleclient/filterlineedit.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/videoview.h"

///// Namespaces /////

namespace client
{

///// Methods /////

Action::Action()
{

}

Action::~Action()
{

}

bool Action::IsVideoWidgetWindowActive() const
{
  const std::vector<VideoWidget*>::const_iterator i = std::find_if(MainWindow::Instance()->GetVideoWidgetsMgr().GetVideoWidgets().begin(), MainWindow::Instance()->GetVideoWidgetsMgr().GetVideoWidgets().end(), [activewindow = QApplication::activeWindow()](const VideoWidget* videowidget){ return (videowidget->window() == activewindow); });
  if (i == MainWindow::Instance()->GetVideoWidgetsMgr().GetVideoWidgets().end())
  {
  
    return false;
  }
  else
  {
  
    return true;
  }
}

// CopyView
CopyViewAction::CopyViewAction()
{

}

CopyViewAction::~CopyViewAction()
{

}

bool CopyViewAction::KeyPress(const bool autorepeat)
{
  const QSharedPointer<View> view = MainWindow::Instance()->GetVideoWidgetsMgr().GetLastSelectedView();
  if (view == nullptr)
  {

    return true;
  }
  view->Copy(true);
  return true;
}

bool CopyViewAction::KeyRelease(const bool autorepeat)
{

  return true;
}

bool CopyViewAction::operator==(const Action& action)
{
  if (typeid(action) != typeid(CopyViewAction))
  {

    return false;
  }
  return true;
}

// ShowFullscreen
ShowFullscreenAction::ShowFullscreenAction(QWidget* window) :
  window_(window)
{

}

ShowFullscreenAction::~ShowFullscreenAction()
{

}

bool ShowFullscreenAction::KeyPress(const bool autorepeat)
{
  if (window_->isActiveWindow())
  {
    if (window_->isFullScreen())
    {
      window_->showNormal();

    }
    else
    {
      window_->showFullScreen();

    }
  }
  return true;
}

bool ShowFullscreenAction::KeyRelease(const bool autorepeat)
{

  return true;
}

bool ShowFullscreenAction::operator==(const Action& action)
{
  if (typeid(action) != typeid(ShowFullscreenAction))
  {

    return false;
  }
  return (window_ == static_cast<const ShowFullscreenAction*>(&action)->window_);
}

// ExitFullscreen
ExitFullscreenAction::ExitFullscreenAction()
{

}

ExitFullscreenAction::~ExitFullscreenAction()
{

}

bool ExitFullscreenAction::KeyPress(const bool autorepeat)
{
  QWidget* activewindow = QApplication::activeWindow();
  if (activewindow == nullptr)
  {

    return false;
  }

  if (activewindow->isFullScreen())
  {
    activewindow->showNormal();
    return true;
  }

  return false;
}

bool ExitFullscreenAction::KeyRelease(const bool autorepeat)
{

  return true;
}

bool ExitFullscreenAction::operator==(const Action& action)
{
  if (typeid(action) != typeid(ExitFullscreenAction))
  {

    return false;
  }
  return true;
}

// ShowHideDocksAction
ShowHideDocksAction::ShowHideDocksAction()
{

}

ShowHideDocksAction::~ShowHideDocksAction()
{

}

bool ShowHideDocksAction::KeyPress(const bool autorepeat)
{
  MainWindow::Instance()->ShowHideDocks();
  return true;
}

bool ShowHideDocksAction::KeyRelease(const bool autorepeat)
{

  return true;
}

bool ShowHideDocksAction::operator==(const Action& action)
{
  if (typeid(action) != typeid(ShowHideDocksAction))
  {

    return false;
  }
  return true;
}

// PanTiltLastCameraRightAction
PanTiltLastCameraAction::PanTiltLastCameraAction(const float x, const float y) :
  x_(x),
  y_(y)
{
  connect(qApp, &QApplication::focusChanged, this, &PanTiltLastCameraAction::FocusChanged);

}

PanTiltLastCameraAction::~PanTiltLastCameraAction()
{

}

bool PanTiltLastCameraAction::KeyPress(const bool autorepeat)
{
  if (!IsVideoWidgetWindowActive())
  {
  
    return false;
  }

  auto view = MainWindow::Instance()->GetVideoWidgetsMgr().GetLastSelectedView();
  if (!view)
  {

    return false;
  }

  if (autorepeat)
  {
    
    return true;
  }

  QSharedPointer<VideoView> videoview = view.staticCast<VideoView>();
  if (!videoview->GetONVIFPTZClient() || !videoview->GetONVIFPTZClient()->IsInitialised())
  {

    return false;
  }

  if (view->GetViewType() != VIEWTYPE_MONOCLE)
  {

    return false;
  }

  const std::vector<QString> profiletokens = videoview->GetProfileTokens();
  if (profiletokens.empty())
  {

    return false;
  }

  float x = x_;
  float y = y_;
  if (videoview->GetPTZXMin().is_initialized() && videoview->GetPTZXMax().is_initialized() && videoview->GetPTZYMin().is_initialized() && videoview->GetPTZYMax().is_initialized())
  {
    const float midx = (*videoview->GetPTZXMax() + *videoview->GetPTZXMin()) * 0.5f;
    if (x > 0.0f)
    {
      x = (*videoview->GetPTZXMax() - midx) * x;

    }
    else if (x < 0.0f)
    {
      x = (midx - *videoview->GetPTZXMin()) * x;

    }

    const float midy = (*videoview->GetPTZYMax() + *videoview->GetPTZYMin()) * 0.5f;
    if (y > 0.0f)
    {
      y = (*videoview->GetPTZYMax() - midy) * y;

    }
    else if (y < 0.0f)
    {
      y = (midy - *videoview->GetPTZYMin()) * y;

    }
  }

  videoview_ = videoview;
  profiletoken_ = profiletokens[0].toStdString();
  continuousmoveconnection_ = videoview->GetONVIFPTZClient()->ContinuousMoveCallback(profiletokens[0].toStdString(), onvif::PTZSpeed(onvif::Vector2d(x, y, boost::none), boost::none), onvif::Duration(0, 0, 0, 0, 0, 60), [this, videoview](const onvif::ptz::ContinuousMoveResponse& continuousmoveresponse)
  {
    if (continuousmoveresponse.Error())
    {
      videoview_.clear();
      profiletoken_.reset();
      LOG_GUI_WARNING_SOURCE(videoview->GetDevice(), QString::fromStdString(continuousmoveresponse.Message()));
      return;
    }
  });

  return true;
}

bool PanTiltLastCameraAction::KeyRelease(const bool autorepeat)
{
  if (autorepeat)
  {
    
    return true;
  }

  return Stop();
}

bool PanTiltLastCameraAction::operator==(const Action& action)
{
  if (typeid(action) != typeid(PanTiltLastCameraAction))
  {

    return false;
  }
  return ((x_ == static_cast<const PanTiltLastCameraAction*>(&action)->x_) && (y_ == static_cast<const PanTiltLastCameraAction*>(&action)->y_));
}

void PanTiltLastCameraAction::FocusChanged(QWidget*, QWidget* now)
{
  if (!now)
  {
    Stop();

  }
}

bool PanTiltLastCameraAction::Stop()
{
  auto videoview = videoview_.lock();
  if (!videoview)
  {

    return false;
  }

  if (!profiletoken_.is_initialized())
  {

    return false;
  }

  if (!videoview->GetONVIFPTZClient() || !videoview->GetONVIFPTZClient()->IsInitialised())
  {

    return false;
  }

  const std::string profiletoken = *profiletoken_;

  videoview_.clear();
  profiletoken_.reset();
  stopconnection_ = videoview->GetONVIFPTZClient()->StopCallback(profiletoken, true, false, [this, videoview](const onvif::ptz::StopResponse& stopresponse)
  {
    if (stopresponse.Error())
    {
      LOG_GUI_WARNING_SOURCE(videoview->GetDevice(), QString::fromStdString(stopresponse.Message()));
      return;
    }
  });
  return true;
}

// SelectAllAction
SelectAllAction::SelectAllAction()
{

}

SelectAllAction::~SelectAllAction()
{

}

bool SelectAllAction::KeyPress(const bool autorepeat)
{
  if (!IsVideoWidgetWindowActive())
  {
  
    return false;
  }

  // Bit of a hack here, but we don't want to steal the filter edits
  const QWidget* focuswidget = QApplication::focusWidget();
  if (focuswidget && (typeid(*focuswidget) == typeid(FilterLineEdit)))
  {

    return false;
  }
  
  if (autorepeat)
  {
    
    return true;
  }

  for (const auto& videowidget : MainWindow::Instance()->GetVideoWidgetsMgr().GetVideoWidgets())
  {
    if (videowidget->isActiveWindow())
    {
      videowidget->SetSelectedAll(!videowidget->AllSelected()); // If all the video views are selected already, unselect them all
      
    }
  }
  return true;
}

bool SelectAllAction::KeyRelease(const bool autorepeat)
{
  if (!IsVideoWidgetWindowActive())
  {
  
    return false;
  }
  
  if (autorepeat)
  {
    
    return true;
  }

  return true;
}

bool SelectAllAction::operator==(const Action& action)
{
  if (typeid(action) != typeid(SelectAllAction))
  {

    return false;
  }
  return true;
}

// SetMenuVisibleAction
SetMenuVisibleAction::SetMenuVisibleAction()
{

}

SetMenuVisibleAction::~SetMenuVisibleAction()
{

}

bool SetMenuVisibleAction::KeyPress(const bool autorepeat)
{
  MainWindow::Instance()->menuBar()->setVisible(!MainWindow::Instance()->menuBar()->isVisible());
  return true;
}

bool SetMenuVisibleAction::KeyRelease(const bool autorepeat)
{

  return true;
}

bool SetMenuVisibleAction::operator==(const Action& action)
{
  if (typeid(action) != typeid(SetMenuVisibleAction))
  {

    return false;
  }
  return true;
}

// ZoomLastCameraRightAction
ZoomLastCameraAction::ZoomLastCameraAction(float x) :
  x_(x)
{
  connect(qApp, &QApplication::focusChanged, this, &ZoomLastCameraAction::FocusChanged);

}

ZoomLastCameraAction::~ZoomLastCameraAction()
{

}

bool ZoomLastCameraAction::KeyPress(const bool autorepeat)
{
  if (!IsVideoWidgetWindowActive())
  {

    return false;
  }

  auto view = MainWindow::Instance()->GetVideoWidgetsMgr().GetLastSelectedView();
  if (!view)
  {

    return false;
  }

  if (autorepeat)
  {

    return true;
  }

  QSharedPointer<VideoView> videoview = view.staticCast<VideoView>();
  if (!videoview->GetONVIFPTZClient() || !videoview->GetONVIFPTZClient()->IsInitialised())
  {

    return false;
  }

  if (view->GetViewType() != VIEWTYPE_MONOCLE)
  {

    return false;
  }

  const std::vector<QString> profiletokens = videoview->GetProfileTokens();
  if (profiletokens.empty())
  {

    return false;
  }

  float x = x_;
  if (videoview->GetZoomMin().is_initialized() && videoview->GetZoomMax().is_initialized())
  {
    const float midx = (*videoview->GetZoomMax() + *videoview->GetZoomMin()) * 0.5f;
    if (x > 0.0f)
    {
      x = (*videoview->GetPTZXMax() - midx) * x;

    }
    else if (x < 0.0f)
    {
      x = (midx - *videoview->GetZoomMin()) * x;

    }
  }

  videoview_ = videoview;
  profiletoken_ = profiletokens[0].toStdString();
  continuousmoveconnection_ = videoview->GetONVIFPTZClient()->ContinuousMoveCallback(profiletokens[0].toStdString(), onvif::PTZSpeed(boost::none, onvif::Vector1d(x, boost::none)), onvif::Duration(0, 0, 0, 0, 0, 60), [this, videoview](const onvif::ptz::ContinuousMoveResponse& continuousmoveresponse)
  {
    if (continuousmoveresponse.Error())
    {
      videoview_.clear();
      profiletoken_.reset();
      LOG_GUI_WARNING_SOURCE(videoview->GetDevice(), QString::fromStdString(continuousmoveresponse.Message()));
      return;
    }
  });

  return true;
}

bool ZoomLastCameraAction::KeyRelease(const bool autorepeat)
{
  if (autorepeat)
  {
    
    return true;
  }

  return Stop();
}

bool ZoomLastCameraAction::operator==(const Action& action)
{
  if (typeid(action) != typeid(ZoomLastCameraAction))
  {

    return false;
  }
  return (x_ == static_cast<const ZoomLastCameraAction*>(&action)->x_);
}

void ZoomLastCameraAction::FocusChanged(QWidget*, QWidget* now)
{
  if (!now)
  {
    Stop();

  }
}

bool ZoomLastCameraAction::Stop()
{
  auto videoview = videoview_.lock();
  if (!videoview)
  {

    return false;
  }

  if (!profiletoken_.is_initialized())
  {

    return false;
  }

  if (!videoview->GetONVIFPTZClient() || !videoview->GetONVIFPTZClient()->IsInitialised())
  {

    return false;
  }

  const std::string profiletoken = *profiletoken_;

  videoview_.clear();
  profiletoken_.reset();
  stopconnection_ = videoview->GetONVIFPTZClient()->StopCallback(profiletoken, true, false, [this, videoview](const onvif::ptz::StopResponse& stopresponse)
  {
    if (stopresponse.Error())
    {
      LOG_GUI_WARNING_SOURCE(videoview->GetDevice(), QString::fromStdString(stopresponse.Message()));
      return;
    }
  });
  return true;
}

}
