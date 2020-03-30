// videochartview.cpp
//

///// Includes /////

#include "monocleclient/videochartview.h"

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QWidget>

#include "monocleclient/mainwindow.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingtrack.h"
#include "monocleclient/videowidget.h"

///// Namespaces /////

namespace client
{

///// Methods /////

VideoChartView::VideoChartView(VideoWidget* videowidget, CUcontext cudacontext, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const boost::shared_ptr<client::Device>& device, const QSharedPointer<client::Recording>& recording, const std::vector< QSharedPointer<client::RecordingTrack> >& tracks, const QResource* arial) :
  View(videowidget, cudacontext, selectedcolour, x, y, width, height, ROTATION::_0, false, false, false, false, arial, false, false, false, false),
  device_(device),
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

  QWidget* widget = new QWidget();//TODO keep this as a member
  //TODO widget->setGeometry(QRect(-10000, -10000, widget->geometry().width(), widget->geometry().height())); // This seems to be the only way to get it to display properly is to show it and then hide it, so do it miles off-screen...


  chart_.setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  chart_.chart()->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  chart_.chart()->setPreferredWidth(pixelrect.width());//TODO when the view gets resized, we want to set this again
  chart_.chart()->setPreferredHeight(pixelrect.height());



  chart_.setWindowFlag(Qt::SubWindow, true); // This hides the chart from the taskbar

  widget->setMinimumWidth(pixelrect.width());
  widget->setMinimumHeight(pixelrect.height());
  QGridLayout* layout = new QGridLayout(widget);//TODO keep this as a member
  widget->setContentsMargins(QMargins(0, 0, 0, 0));
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(&chart_);
  layout->setVerticalSpacing(0);
  layout->setHorizontalSpacing(0);
  chart_.chart()->setBackgroundRoundness(0.0);

  chart_.setGeometry(QRect(-10000, -10000, pixelrect.width(), pixelrect.height())); // This seems to be the only way to get it to display properly is to show it and then hide it, so do it miles off-screen...
  //TODO chart_.show();
  //TODO chart_.hide();
  widget->show();
  widget->hide();

  chart_.chart()->setContentsMargins(QMargins(0, 0, 0, 0));
  chart_.setContentsMargins(QMargins(0, 0, 0, 0));
  chart_.chart()->setMargins(QMargins(0, 0, 0, 0));
  chart_.chart()->setBackgroundRoundness(0.0);

  chart_.setRenderHint(QPainter::Antialiasing, true);

  chart_.setBackgroundBrush(QBrush(QColor(0, 0, 0), Qt::SolidPattern));

  chart_.chart()->setTheme(QChart::ChartTheme::ChartThemeBlueCerulean);

  for (QSharedPointer<RecordingTrack>& track : tracks_)
  {
    createstreamsconnections_.push_back(device_->CreateStream(recording_->GetToken(), track->GetId(), [this](const std::chrono::nanoseconds latency, const monocle::client::CREATESTREAMRESPONSE& createstreamresponse)
    {
      if (createstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        //TODO messagebox?
        return;
      }

      //TODO we need to initiate live streaming

      //TODO do anything?

    }, VideoChartView::ControlStreamEnd, nullptr, nullptr, nullptr, nullptr, nullptr, VideoChartView::ObjectDetectorCallback, nullptr, this));
  }

  startTimer(std::chrono::seconds(1));
  SendImage();
}

VideoChartView::~VideoChartView()
{

}

void VideoChartView::GetMenu(QMenu& parent)
{
  View::GetMenu(parent);

  //TODO right click menu to show/hide object types in a submenu
    //TODO whole bunch of ticky boxes please

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
  SendImage();

}

void VideoChartView::ControlStreamEnd(const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error, void* callbackdata)
{
  //TODO do stuff

}

void VideoChartView::ObjectDetectorCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::ObjectDetectorFrameType objectdetectorframetype, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata)
{
  qDebug() << size;//TODO lets do stuff... make sure streamtoken is happy

}

void VideoChartView::SendImage()
{
  QImage image(chart_.chart()->preferredWidth(), chart_.chart()->preferredHeight(), QImage::Format::Format_RGBX8888);
  QPainter painter(&image);
  chart_.render(&painter);

  //TODO auto b = image.save("test.jpg");//TODO remove

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
}

}
