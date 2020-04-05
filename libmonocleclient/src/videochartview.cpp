// videochartview.cpp
//

///// Includes /////

#include "monocleclient/videochartview.h"

#include <boost/date_time.hpp>
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
  View(videowidget, cudacontext, selectedcolour, x, y, width, height, ROTATION::_0, false, true, false, false, arial, false, false, false, false, false, false, false),
  device_(device),
  recording_(recording),
  tracks_(tracks),
  widget_(new QWidget()),
  layout_(new QGridLayout(widget_)),
  chart_(nullptr, nullptr),
  xaxis_(nullptr),
  yaxis_(nullptr)
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), rotation_, mirror_, stretch_, true);

  chart_.chart()->legend()->setVisible(true);
  chart_.chart()->legend()->setAlignment(Qt::AlignBottom);

  // Setup the chart itself. It gets drawn offscreen and then copied in
  //TODO const QRect pixelrect = GetPixelRect();
  //TODO widget_->setMinimumWidth(pixelrect.width());
  //TODO widget_->setMinimumHeight(pixelrect.height());
  //TODO widget_->setGeometry(QRect(-10000, -10000, pixelrect.width(), pixelrect.height())); // This seems to be the only way to get it to display properly is to show it and then hide it, so do it miles off-screen...
  //TODO 
  //TODO chart_.setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  //TODO chart_.chart()->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  //TODO chart_.chart()->setPreferredWidth(pixelrect.width());//TODO when the view gets resized, we want to set this again
  //TODO chart_.chart()->setPreferredHeight(pixelrect.height());
  //TODO //TODO maybe we can override SetPosition and refresh all this?
  //TODO 
  //TODO //TODO Can we remove any of this stuff?
  //TODO widget_->setContentsMargins(QMargins(0, 0, 0, 0));
  //TODO layout_->setContentsMargins(0, 0, 0, 0);
  //TODO layout_->addWidget(&chart_);
  //TODO layout_->setVerticalSpacing(0);
  //TODO layout_->setHorizontalSpacing(0);
  //TODO layout_->setColumnStretch(0, 1);
  //TODO layout_->setRowStretch(0, 1);
  //TODO chart_.chart()->setBackgroundRoundness(0.0);
  //TODO chart_.setWindowFlag(Qt::SubWindow, true); // This hides the chart from the taskbar
  //TODO chart_.chart()->setContentsMargins(QMargins(0, 0, 0, 0));
  //TODO chart_.setContentsMargins(QMargins(0, 0, 0, 0));
  //TODO chart_.chart()->setMargins(QMargins(0, 0, 0, 0));
  //TODO chart_.chart()->setBackgroundRoundness(0.0);
  chart_.setRenderHint(QPainter::Antialiasing, true);
  chart_.setBackgroundBrush(QBrush(QColor(0, 0, 0), Qt::SolidPattern));
  chart_.chart()->setTheme(QChart::ChartTheme::ChartThemeBlueCerulean);
  //TODO widget_->show(); // For some bizarre reason, this is required.
  //TODO widget_->hide();

  // Look at the last 24 hours by default
  auto now = boost::posix_time::second_clock::universal_time();
  now = now - boost::posix_time::seconds(now.time_of_day().seconds() - 1) - boost::posix_time::minutes(now.time_of_day().minutes()) - boost::posix_time::hours(24);
  const uint64_t endtime = boost::posix_time::to_time_t(now);
  const uint64_t starttime = (endtime * 1000) + device_->GetTimeOffset();

  CloseConnections();
  for (QSharedPointer<RecordingTrack>& track : tracks_)
  {
    const uint64_t interval = 60 * 60 * 1000;
    streamsconnections_.push_back(device_->GetObjectTrackStatistics(recording_->GetToken(), track->GetId(), starttime, std::numeric_limits<uint64_t>::max(), interval, [this, starttime, endtime, interval](const std::chrono::nanoseconds latency, const monocle::client::GETOBJECTTRACKSTATISTICSSRESPONSE& getobjecttrackstatisticssresponse)
    {
      if (getobjecttrackstatisticssresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        //TODO messagebox?
        return;
      }

      qreal max = 0.0;
      for (const monocle::client::OBJECTCLASSTRACKSTATISTICS& result : getobjecttrackstatisticssresponse.results_)
      {
        auto series = std::find_if(series_.begin(), series_.end(), [&result](const std::pair<monocle::ObjectClass, QLineSeries*>& series) { return (series.first == result.objectclass_); });
        if (series == series_.end())
        {
          // Create a new series
          series_.push_back(std::make_pair(result.objectclass_, new QLineSeries()));
          series = series_.end() - 1;
          series->second->setName(monocle::EnumNameObjectClass(result.objectclass_));
        }
        series->second->append(result.starttime_, result.count_);
        max = std::max(max, static_cast<double>(result.count_));
      }

      for (std::pair<monocle::ObjectClass, QLineSeries*>& series : series_)
      {
        if (AllZero(series.second)) // Don't show the series without any points
        {

          continue;
        }
        chart_.chart()->addSeries(series.second);
      }


      xaxis_ = new QDateTimeAxis();
      xaxis_->setFormat("hh:mm");
      xaxis_->setTitleText("Time");
      xaxis_->setMin(QDateTime::fromMSecsSinceEpoch(starttime));
      xaxis_->setMax(QDateTime::fromMSecsSinceEpoch(endtime));
      xaxis_->setTickCount(13);
      chart_.chart()->addAxis(xaxis_, Qt::AlignBottom);

      yaxis_ = new QValueAxis();
      yaxis_->setMax(max);
      yaxis_->setLabelFormat("%i");
      yaxis_->setTitleText("Count");
      chart_.chart()->addAxis(yaxis_, Qt::AlignLeft);

      for (std::pair<monocle::ObjectClass, QLineSeries*>& series : series_)
      {
        series.second->attachAxis(xaxis_);
        series.second->attachAxis(yaxis_);
      }

      //TODO maybe put this into the QTimer::singleShot?
      QTimer::singleShot(std::chrono::milliseconds(100), [this]() { SendImage(); });
      QTimer::singleShot(std::chrono::milliseconds(200), [this]() { SendImage(); });
      QTimer::singleShot(std::chrono::milliseconds(300), [this]() { SendImage(); });
      QTimer::singleShot(std::chrono::milliseconds(500), [this]() { SendImage(); });
      QTimer::singleShot(std::chrono::milliseconds(1000), [this]() { SendImage(); });
      QTimer::singleShot(std::chrono::milliseconds(2000), [this]() { SendImage(); });
      QTimer::singleShot(std::chrono::milliseconds(5000), [this]() { SendImage(); });
    }));
  }

  //TODO startTimer(std::chrono::seconds(1));
  SendImage();
}

VideoChartView::~VideoChartView()
{
  CloseConnections();

}

void VideoChartView::GetMenu(QMenu& parent)
{
  View::GetMenu(parent);

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
  //TODO seems like we need this, ebcause it doesn't finish drawing after a while...
  //TODO SendImage();

}

void VideoChartView::SetPosition(VideoWidget* videowidget, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool mirror, const bool stretch, const bool makecurrent)
{
  View::SetPosition(videowidget, x, y, width, height, rotation, mirror, stretch, makecurrent);
  //TODO QTimer::singleShot(std::chrono::milliseconds(500), [this]() { SendImage(); });
  SendImage();
}

void VideoChartView::SendImage()
{
  const QRect pixelrect = GetPixelRect();
  chart_.resize(pixelrect.width(), pixelrect.height());
  //TODO const QImage image = chart_.grab().toImage(); // We could keep this as a QPixmap, but we shouldn't be doing this all that often so it's ok I guess
  const QPixmap pixmap = chart_.grab();

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

bool VideoChartView::AllZero(const QLineSeries* series) const
{
  for (int i = 0; i < series->count(); ++i)
  {
    if (series->at(i).y())
    {

      return false;
    }
  }
  return true;
}

}
