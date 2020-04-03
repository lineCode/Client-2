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
  chart_(nullptr, nullptr),
  series_(new QStackedBarSeries()),
  xaxis_(new QBarCategoryAxis()),
  yaxis_(new QValueAxis())
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), rotation_, mirror_, stretch_, true);

  for (size_t z = 0; z <= static_cast<size_t>(monocle::ObjectClass::MAX); ++z)
  {
    const monocle::ObjectClass classtype = static_cast<monocle::ObjectClass>(z);
    barsets_[z] = std::make_unique<QBarSet>(monocle::EnumNameObjectClass(classtype));
  }

  chart_.chart()->addSeries(series_);
  //TODO chart_.chart()->setTitle()
  //TODO chart->setAnimationOptions(QChart::SeriesAnimations);

  chart_.chart()->addAxis(xaxis_, Qt::AlignBottom);
  chart_.chart()->addAxis(yaxis_, Qt::AlignLeft);
  series_->attachAxis(xaxis_);
  series_->attachAxis(yaxis_);
  
  //TODO categories=times

  chart_.chart()->legend()->setVisible(true);
  chart_.chart()->legend()->setAlignment(Qt::AlignBottom);

  //TODO only show objects that appear in the legend, no need to show giraffes if there aren't any imo

  //TODO tidy up all this shit
  //TODO QValueAxis* cpuaxisx_ = new QValueAxis(this);
  //TODO QValueAxis* cpuaxisy_ = new QValueAxis(this);
  //TODO QValueAxis* cpumemoryaxisy_;
  //TODO 
  //TODO QLineSeries* cpu_ = new QLineSeries(this);
  //TODO QLineSeries* memory_ = new QLineSeries(this);
  //TODO 
  //TODO cpu_->setName("CPU");
  //TODO memory_->setName("Memory");
  //TODO 
  //TODO cpuaxisx_->setTitleText("Seconds");
  //TODO chart_.chart()->addAxis(cpuaxisx_, Qt::AlignBottom);
  //TODO cpuaxisx_->setReverse(true);
  //TODO cpuaxisx_->setMin(0.0);
  //TODO cpuaxisx_->setMax(60.0);
  //TODO 
  //TODO chart_.chart()->addSeries(cpu_);
  //TODO chart_.chart()->addSeries(memory_);
  //TODO cpu_->setName("CPU");
  //TODO memory_->setName("Memory");
  //TODO 
  //TODO // CPU
  //TODO cpuaxisy_->setLabelFormat("%d%%");
  //TODO cpuaxisy_->setTitleText("CPU");
  //TODO cpuaxisy_->setMin(0.0);
  //TODO cpuaxisy_->setMax(100.0);
  //TODO chart_.chart()->addAxis(cpuaxisy_, Qt::AlignLeft);
  //TODO cpu_->attachAxis(cpuaxisx_);
  //TODO cpu_->attachAxis(cpuaxisy_);

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
//TODO we want to modulus the time with the start of a minute,hour,day so we can fill in a nice barchart
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
  // We are always extending to live at the moment, so this can be ignored

}

void VideoChartView::ControlStreamResult(const uint64_t streamtoken, const uint64_t requestindex, const uint64_t starttime, const uint64_t endtime, const std::vector< std::pair<monocle::ObjectClass, uint64_t> >& results, void* callbackdata)
{
  VideoChartView* videochartview = reinterpret_cast<VideoChartView*>(callbackdata);
  qDebug() << (starttime / 1000);//TODO remove
  for (const std::pair<monocle::ObjectClass, uint64_t>& result : results)//TODO remove
  {
    qDebug() << "  " << monocle::EnumNameObjectClass(result.first) << result.second;
    
  }

  videochartview->xaxis_->append(QString::number(starttime));//TODO make time pretty

  for (const std::pair<monocle::ObjectClass, uint64_t>& result : results)
  {
    videochartview->barsets_[static_cast<size_t>(result.first)]->append(result.second);
    videochartview->series_->append(videochartview->barsets_[static_cast<size_t>(result.first)].get());
  }

  videochartview->chart_.chart()->removeSeries(videochartview->series_);
  videochartview->chart_.chart()->addSeries(videochartview->series_);
}

void VideoChartView::SendImage()
{
  //TODO if nothing has changed, don't send anything... need a nice flag dirty_

  QImage image(chart_.chart()->preferredWidth(), chart_.chart()->preferredHeight(), QImage::Format::Format_RGBX8888);
  QPainter painter(&image);
  chart_.render(&painter);

  ImageBuffer imagebuffer = freeimagequeue_.GetFreeImage();
  imagebuffer.Destroy();
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
