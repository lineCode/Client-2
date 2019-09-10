// videoviewpropertieswindow.cpp
//

///// Includes /////

#include "monocleclient/videoviewpropertieswindow.h"

#include "monocleclient/videoview.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

VideoViewPropertiesWindow::VideoViewPropertiesWindow(QWidget* parent, const QSharedPointer<VideoView>& videoview) :
  QDialog(parent),
  videoview_(videoview)
{
  ui_.setupUi(this);

  startTimer(std::chrono::milliseconds(150));
  Render();
}

VideoViewPropertiesWindow::~VideoViewPropertiesWindow()
{
	
}

void VideoViewPropertiesWindow::timerEvent(QTimerEvent* event)
{
  Render();

}

void VideoViewPropertiesWindow::Render()
{
  const uint64_t duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - videoview_->GetStartTime()).count();
  if (duration == 0) // Shouldn't happen but lets be very careful!
  {

    return;
  }

  ui_.labelproperties->setText("Total Frames: " + QString::number(videoview_->GetTotalFrames()) +
                               "\nTotal Bytes: " + QString::number(videoview_->GetTotalBytes()) +
                               "\nFrame Rate: " + QString::number(videoview_->GetTotalFrames() / duration) +
                               "\nBit Rate: " + QString::number(videoview_->GetTotalBytes() / duration) + " B/s" +
                               "\nResolution: " + QString::number(videoview_->GetImageWidth()) + "x" + QString::number(videoview_->GetImageHeight()));
}

void VideoViewPropertiesWindow::on_buttonok_clicked()
{
  accept();

}

}

QT_END_NAMESPACE
