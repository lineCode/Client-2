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

  //TODO I think the begin with, we ONLY listen to live and we draw graphs based upon live only...

//TODO stretch, rotation, mirror need to be removed from menu options

  //TODO we want the server to do all the hard work
    //TODO CreateStatisticsStream and then we can request things perhaps
      //TODO starttime,endtime,interval
        //TODO we will need to grab history, but then also continue updating live...
          //TODO if endtime is not null, it listens to live and just updates the client on that live time
            //TODO the graph will go mental with a massive gap while it captures all the info from starttime... but the client can manage that I think, it will just have to wait until it reaches the previous interval time to the live stream
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
  chart_.chart()->setPreferredWidth(pixelrect.width());//TODO when the view gets resized, we want to set this again
  chart_.chart()->setPreferredHeight(pixelrect.height());

  chart_.setWindowFlag(Qt::SubWindow, true); // This hides the chart from the taskbar
  chart_.setGeometry(QRect(-10000, -10000, pixelrect.width(), pixelrect.height())); // This seems to be the only way to get it to display properly is to show it and then hide it, so do it miles off-screen...

  QWidget* widget = new QWidget();//TODO keep this as a member
  widget->setMinimumWidth(pixelrect.width());
  widget->setMinimumHeight(pixelrect.height());
  QGridLayout* layout = new QGridLayout(widget);//TODO keep this as a member
  widget->setContentsMargins(QMargins(0, 0, 0, 0));
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(&chart_);
  chart_.chart()->setBackgroundRoundness(0.0);
  widget->show();
  widget->hide();

  chart_.chart()->setContentsMargins(QMargins(0, 0, 0, 0));
  chart_.setContentsMargins(QMargins(0, 0, 0, 0));
  chart_.chart()->setMargins(QMargins(0, 0, 0, 0));
  chart_.chart()->setBackgroundRoundness(0.0);
  //TODO chart_.setViewportMargins(0, 0, 0, 0);

  chart_.setStyleSheet("{ border: 0; }");
  widget->setStyleSheet("{ border: 0; }");

  auto aa = chart_.parent();
  auto bb = chart_.layout();

  chart_.setRenderHint(QPainter::Antialiasing, true);

  chart_.setBackgroundBrush(QBrush(QColor(0, 0, 0), Qt::SolidPattern));

  //TODO QPalette pal = chart_.palette();
  //TODO pal.setColor(QPalette::Window, QRgb(0x40434a));
  //TODO pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
  //TODO chart_.setPalette(pal);

  chart_.chart()->setTheme(QChart::ChartTheme::ChartThemeBlueCerulean);

  //TODO chart_.chart()->setBackgroundVisible(false);

  //TODO auto a = chart_.chart()->plotArea();
  //TODO chart_.chart()->setPlotArea(QRectF(0.0, 0.0, pixelrect.width(), pixelrect.height()));

  //TODO still need to get the background to be the proper nice colour, not sure how to do that... or make it transparent?

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
  //TODO putting the chart into some kind of stretching widget thing really does the trick...

  auto a = chart_.chart()->plotArea();

  QImage image(chart_.chart()->preferredWidth(), chart_.chart()->preferredHeight(), QImage::Format::Format_RGBX8888);
  QPainter painter(&image);
  //TODO chart_.render(&painter, QRectF(), QRect(a.left(), a.top(), a.width(), a.height()));
  chart_.render(&painter);

  //TODO auto b = image.save("test.jpg");//TODO remove
  //TODO why is it a tiny chart inside the big image?

  ImageBuffer imagebuffer = freeimagequeue_.GetFreeImage();
  imagebuffer.Destroy(); // Don't mind doing this, because the map will only update very infrequently
  imagebuffer.widths_[0] = image.width();
  imagebuffer.heights_[0] = image.height();
  imagebuffer.digitallysigned_ = boost::none;
  imagebuffer.marker_ = true;
  imagebuffer.time_ = 0;
  imagebuffer.type_ = IMAGEBUFFERTYPE_RGBA;
  imagebuffer.buffer_ = new uint8_t[image.sizeInBytes()];
  imagebuffer.data_[0] = imagebuffer.buffer_;

  std::memcpy(imagebuffer.buffer_, image.bits(), image.sizeInBytes());

  imagequeue_.push(imagebuffer);

  int i = 0;//TODO remove
  

}

}
