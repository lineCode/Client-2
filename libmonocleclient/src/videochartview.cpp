// videochartview.cpp
//

///// Includes /////

#include "monocleclient/videochartview.h"

#include <monocleprotocol/objectdetectorframetype_generated.h>
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
  View(videowidget, cudacontext, selectedcolour, x, y, width, height, ROTATION::_0, false, false, false, false, arial, false, false, false, false, false, false, false),
  device_(device),
  recording_(recording),
  tracks_(tracks),
  widget_(new QWidget()),
  layout_(new QGridLayout(widget_)),
  chart_(nullptr, nullptr)
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), rotation_, mirror_, stretch_, true);

  //TODO Stacked bar chart with each object type to start with

  //TODO only show objects that appear in the legend, no need to show giraffes if there aren't any imo

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

  // Setup the chart itself. It gets drawn offscreen and then copied in
  const QRect pixelrect = GetPixelRect();
  widget_->setMinimumWidth(pixelrect.width());
  widget_->setMinimumHeight(pixelrect.height());
  widget_->setGeometry(QRect(-10000, -10000, pixelrect.width(), pixelrect.height())); // This seems to be the only way to get it to display properly is to show it and then hide it, so do it miles off-screen...

  chart_.setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  chart_.chart()->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  chart_.chart()->setPreferredWidth(pixelrect.width());//TODO when the view gets resized, we want to set this again
  //TODO maybe we can override SetPosition and refresh all this?
  chart_.chart()->setPreferredHeight(pixelrect.height());

  //TODO Can we remove any of this stuff?
  widget_->setContentsMargins(QMargins(0, 0, 0, 0));
  layout_->setContentsMargins(0, 0, 0, 0);
  layout_->addWidget(&chart_);
  layout_->setVerticalSpacing(0);
  layout_->setHorizontalSpacing(0);
  layout_->setColumnStretch(0, 1);
  layout_->setRowStretch(0, 1);
  chart_.chart()->setBackgroundRoundness(0.0);
  chart_.setWindowFlag(Qt::SubWindow, true); // This hides the chart from the taskbar
  chart_.chart()->setContentsMargins(QMargins(0, 0, 0, 0));
  chart_.setContentsMargins(QMargins(0, 0, 0, 0));
  chart_.chart()->setMargins(QMargins(0, 0, 0, 0));
  chart_.chart()->setBackgroundRoundness(0.0);
  chart_.setRenderHint(QPainter::Antialiasing, true);
  chart_.setBackgroundBrush(QBrush(QColor(0, 0, 0), Qt::SolidPattern));
  chart_.chart()->setTheme(QChart::ChartTheme::ChartThemeBlueCerulean);
  widget_->show(); // For some bizarre reason, this is required.
  widget_->hide();

  CloseConnections();
  for (QSharedPointer<RecordingTrack>& track : tracks_)
  {
    streamsconnections_.push_back(device_->CreateTrackStatisticsStream(recording_->GetToken(), track->GetId(), [this](const std::chrono::nanoseconds latency, const monocle::client::CREATETRACKSTATISTICSSTREAMRESPONSE& createtrackstatisticsstreamresponse)
    {
      if (createtrackstatisticsstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        //TODO messagebox?
        return;
      }
      
      //TODO set time to something reasonable(different zoom levels may change this)
      streamsconnections_.push_back(device_->ControlTrackStatisticsStream(createtrackstatisticsstreamresponse.token_, 0, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() - std::chrono::hours(60)).count() + device_->GetTimeOffset(), std::numeric_limits<uint64_t>::max(), 60 * 1000, [this](const std::chrono::steady_clock::duration latency, const monocle::client::CONTROLTRACKSTATISTICSSTREAMRESPONSE& controltrackstatisticsstreamresponse)
      {
        if (controltrackstatisticsstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
        {
          //TODO messagebox?
          return;
        }
      }));
    }, VideoChartView::ControlStreamEnd, VideoChartView::ControlStreamResult, this));
  }

  startTimer(std::chrono::seconds(1));
  SendImage();
}

VideoChartView::~VideoChartView()
{
  CloseConnections();

  //TODO delete layout_ and widget_

  //TODO DestroyTrackStatisticsStream

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

void VideoChartView::ControlStreamEnd(const uint64_t streamtoken, const uint64_t requestindex, const monocle::ErrorCode error, void* callbackdata)
{
  //TODO because we always want live, don't think we need to do anything here?

}

void VideoChartView::ControlStreamResult(const uint64_t streamtoken, const uint64_t requestindex, const uint64_t starttime, const uint64_t endtime, const std::vector< std::pair<monocle::ObjectClass, uint64_t> >& results, void* callbackdata)
{
  qDebug() << results.size();//TODO
  //TODO lets do something?

}

void VideoChartView::SendImage()
{
  QImage image(chart_.chart()->preferredWidth(), chart_.chart()->preferredHeight(), QImage::Format::Format_RGBX8888);
  QPainter painter(&image);
  chart_.render(&painter);

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

void VideoChartView::CloseConnections()
{
  std::for_each(streamsconnections_.begin(), streamsconnections_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
  streamsconnections_.clear();
}

}
