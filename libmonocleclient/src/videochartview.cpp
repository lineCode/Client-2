// videochartview.cpp
//

///// Includes /////

#include "monocleclient/videochartview.h"

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QWidget>

#include "monocleclient/mainwindow.h"
#include "monocleclient/videowidget.h"

///// Namespaces /////

namespace client
{

///// Methods /////

VideoChartView::VideoChartView(VideoWidget* videowidget, CUcontext cudacontext, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const boost::shared_ptr<client::Device>& device, const QSharedPointer<client::Recording>& recording, const std::vector< QSharedPointer<client::RecordingTrack> >& tracks, const QResource* arial) :
  View(videowidget, cudacontext, selectedcolour, x, y, width, height, ROTATION::_0, false, false, false, false, arial, false, false, false, false),
  recording_(recording),
  tracks_(tracks),
  chart_(nullptr, nullptr)
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), rotation_, mirror_, stretch_, true);

  //TODO we want the server to do all the hard work
    //TODO CreateStatisticsStream and then we can request things perhaps
      //TODO starttime,endtime,interval
        //TODO we will need to grab history, but then also continue updating live...
  //TODO we want to kick off authentication and start streaming live object data from the tracks
    //TODO we will also want history too though... not sure how to deal with both...
    //TODO just copy video view and bang off we go

  //TODO tidy up all this shit
  QValueAxis* cpuaxisx_ = new QValueAxis(this);
  QValueAxis* cpuaxisy_ = new QValueAxis(this);
  //TODO QValueAxis* cpumemoryaxisy_;
  
  QLineSeries* cpu_ = new QLineSeries(this);
  QLineSeries* memory_ = new QLineSeries(this);
  
  cpu_->setName("CPU");
  memory_->setName("Memory");
  
  cpuaxisx_->setTitleText("Seconds");
  chart_.chart()->addAxis(cpuaxisx_, Qt::AlignBottom);
  cpuaxisx_->setReverse(true);
  cpuaxisx_->setMin(0.0);
  cpuaxisx_->setMax(60.0);
  
  chart_.chart()->addSeries(cpu_);
  chart_.chart()->addSeries(memory_);
  cpu_->setName("CPU");
  memory_->setName("Memory");
  
  // CPU
  cpuaxisy_->setLabelFormat("%d%%");
  cpuaxisy_->setTitleText("CPU");
  cpuaxisy_->setMin(0.0);
  cpuaxisy_->setMax(100.0);
  chart_.chart()->addAxis(cpuaxisy_, Qt::AlignLeft);
  cpu_->attachAxis(cpuaxisx_);
  cpu_->attachAxis(cpuaxisy_);
  
  const QRect pixelrect = GetPixelRect();
  chart_.setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  chart_.chart()->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  chart_.chart()->setPreferredWidth(pixelrect.width());
  chart_.chart()->setPreferredHeight(pixelrect.height());

  chart_.setContentsMargins(QMargins(0, 0, 0, 0));
  chart_.chart()->setMargins(QMargins(0, 0, 0, 0));

  chart_.setWindowFlag(Qt::SubWindow, true); // This hides the chart from the taskbar
  chart_.setGeometry(QRect(-10000, -10000, pixelrect.width(), pixelrect.height())); // This seems to be the only way to get it to display properly is to show it and then hide it, so do it miles off-screen...
  chart_.show();
  chart_.hide();

  startTimer(std::chrono::seconds(1));

  //TODO set background to black and everything else to some nice colour I think
}

VideoChartView::~VideoChartView()
{

}

void VideoChartView::GetMenu(QMenu& parent)
{
  View::GetMenu(parent);

  //TODO right click menu to show/hide object types

}

double VideoChartView::GetAspectRatio() const
{
  if (imagewidth_ && imageheight_)
  {

    return (static_cast<double>(imagewidth_) / static_cast<double>(imageheight_));
  }
  else
  {

    return 0.0;
  }
}

bool VideoChartView::GetImage(ImageBuffer& imagebuffer)
{
  //TODO we update the image every 1 with a Qt timer

  bool hasimage = false;
  ImageBuffer previmagebuffer;
  while (true)
  {
    if (imagequeue_.pop(imagebuffer))
    {
      // If we have skipped frames, we should place them back into the temporary list, or destroy them if there is no room
      if (previmagebuffer.buffer_)
      {
        if (paused_)
        {
          cache_.AddImage(previmagebuffer);
  
        }
        else
        {
          freeimagequeue_.AddFreeImage(previmagebuffer);
  
        }
      }
      previmagebuffer = imagebuffer;
      hasimage = true;
    }
    else
    {
      if (imagebuffer.buffer_)
      {
        imagewidth_ = imagebuffer.widths_[0];
        imageheight_ = imagebuffer.heights_[0];
      }
      return hasimage;
    }
  }
  return true;
}

void VideoChartView::FrameStep(const bool forwards)
{
  
}

void VideoChartView::Play(const uint64_t time, const boost::optional<uint64_t>& numframes)
{
  
}

void VideoChartView::Pause(const boost::optional<uint64_t>& frame)
{
  
}

void VideoChartView::Stop()
{
  
}

void VideoChartView::Scrub(const uint64_t time)
{

}

void VideoChartView::timerEvent(QTimerEvent* event)
{
  //TODO auto a = chart_.width();
  //TODO auto c = chart_.height();

  //TODO chart_.chart()->width
  {
    QPixmap pixmap(chart_.chart()->preferredWidth(), chart_.chart()->preferredHeight());
    pixmap.fill(Qt::black);

    //TODO putting the chart into some kind of stretching widget thing really does the trick...

    QImage image(chart_.chart()->preferredWidth(), chart_.chart()->preferredHeight(), QImage::Format::Format_RGBA8888);
    //TODO QPainter painter(&image);
    QPainter painter(&pixmap);
    painter.setPen(*(new QColor(255, 34, 255, 255)));
    chart_.render(&painter);//TODO wtf?
    QFile file("image.png");
    file.open(QIODevice::WriteOnly);
    auto d = pixmap.save(&file, "PNG");
    //tODO auto b = image.save("test.jpg");//TODO remove
    //TODO why is it a tiny chart inside the big image?
  }
  //TODO imagequeue_.push()//TODO now convert the image into an ImageBuffer

  int i = 0;//TODO remove
  

}

}
