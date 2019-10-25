// findmotionplaybackwidget.cpp
//

///// Includes /////

#include "monocleclient/findmotionplaybackwidget.h"

#include <QPainter>

#include "monocleclient/devicemgr.h"
#include "monocleclient/findmotionwindow.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"
#include "monocleclient/recordingblock.h"
#include "monocleclient/recordingtrack.h"
#include "monocleclient/shaders.h"

///// Namespaces /////

namespace client
{

///// Methods /////

FindMotionPlaybackWidget::FindMotionPlaybackWidget(QWidget* parent) :
  QOpenGLWidget(parent),
  actionvideo_(new QAction(tr("Video"), this)),
  actionmotion_(new QAction(tr("Motion"), this)),
  colour_(0.5f, 0.5f, 0.5f, 0.5f),
  recordingsblockspositionlocation_(-1),
  recordingsblockscolourlocation_(-1),
  markerpositionlocation_(-1),
  markercolourlocation_(-1),
  textpositionlocation_(-1),
  texttexturecoordlocation_(-1),
  texttexturesamplerlocation_(-1),
  endtime_(0),
  state_(PLAYBACKMOUSESTATE_IDLE),
  clickedpos_(0),
  timer_(-1)
{
  actionvideo_->setCheckable(true);
  actionvideo_->setChecked(true);
  actionmotion_->setCheckable(true);
  actionmotion_->setChecked(true);

  connect(&MainWindow::Instance()->GetDeviceMgr(), &DeviceMgr::TimeOffsetChanged, this, &FindMotionPlaybackWidget::TimeOffsetChanged);
  connect(actionvideo_, &QAction::triggered, this, &FindMotionPlaybackWidget::ShowVideo);
  connect(actionmotion_, &QAction::triggered, this, &FindMotionPlaybackWidget::ShowMotion);

  setMouseTracking(true);

  // Freetype
  if (FT_Init_FreeType(&freetype_))
  {
    LOG_GUI_WARNING(tr("Error initialising FreeType"));
    return;
  }

  const QResource arial(":/arial.ttf");
  if (FT_New_Memory_Face(freetype_, arial.data(), arial.size(), 0, &freetypearial_))
  {
    LOG_GUI_WARNING(tr("Error loading Arial resource"));
    return;
  }

  if (FT_Set_Pixel_Sizes(freetypearial_, PLAYBACK_TEXT_FONT_HEIGHT, 0))
  {
    LOG_GUI_WARNING(tr("Error initialising Arial resource"));
    return;
  }
}

FindMotionPlaybackWidget::~FindMotionPlaybackWidget()
{
  makeCurrent();

  recordingblocks_.clear();

  glDeleteTextures(1, &starttext_.texture_);
  starttext_.texture_ = 0;
  glDeleteTextures(1, &endtext_.texture_);
  endtext_.texture_ = 0;

  // Freetype
  if (freetypearial_)
  {
    if (FT_Done_Face(freetypearial_))
    {
      LOG_GUI_WARNING(tr("Error destroying arial face"));

    }
    freetypearial_ = nullptr;
  }

  if (freetype_)
  {
    if (FT_Done_FreeType(freetype_))
    {
      LOG_GUI_WARNING(tr("Error destroying FreeType"));

    }
    freetype_ = nullptr;
  }

  doneCurrent();
}

void FindMotionPlaybackWidget::SetTrack(const QSharedPointer<RecordingTrack>& track)
{
  track_ = track;
  
  recordingblocks_.clear();
  for (const std::pair<uint64_t, uint64_t> indices : track_->GetIndices())
  {
    recordingblocks_.emplace_back(std::make_unique<RecordingBlock>(false, indices.first, indices.second));

  }
  motionrecordingblocks_.clear();
}

void FindMotionPlaybackWidget::SetColour(const QVector4D& colour)
{
  colour_ = colour;

}

void FindMotionPlaybackWidget::UpdateRecordingBlocks()
{
  starttime_ = GetStartTime();
  if (!starttime_.is_initialized())
  {
    // no RecordingBlocks, nothing to do
    return;
  }
  endtime_ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

  const boost::optional< std::pair<uint64_t, uint64_t> > startendtime = GetStartEndTime();
  if (!startendtime.is_initialized())
  {

    return;
  }

  guitimelinedarkmarkers_.clear();
  guitimelinelightmarkers_.clear();
  const uint64_t diff = startendtime->second - startendtime->first;
  const float pixelheight = 2.0f / static_cast<float>(height());
  const float pixelwidth = 2.0f / static_cast<float>(width());
  int guitimelinetextsindex = 0;

  const TIMELINEGENERATOR timelinegenerator = TimelineGenerator(*startendtime);

  QTextOption textoption(Qt::AlignHCenter | Qt::AlignVCenter);
  textoption.setWrapMode(QTextOption::WrapMode::NoWrap);

  // Top
  QDateTime toptime = timelinegenerator.startdatetime_;
  const QDateTime endtime = QDateTime::fromMSecsSinceEpoch(startendtime->second, Qt::UTC);
  double maxheight = 0.0f;
  while (toptime <= endtime)
  {
    QDateTime nexttime = toptime;
    nexttime = nexttime.addSecs(timelinegenerator.topsecincrement_);
    nexttime = nexttime.addDays(timelinegenerator.topdayincrement_);
    nexttime = nexttime.addMonths(timelinegenerator.topmonthincrement_);
    nexttime = nexttime.addYears(timelinegenerator.topyearincrement_);
    const uint64_t step = nexttime.toMSecsSinceEpoch() - toptime.toMSecsSinceEpoch();

    // Lines
    const float position = -1.0 + (((static_cast<double>(toptime.toMSecsSinceEpoch()) - static_cast<double>(startendtime->first)) / static_cast<double>(diff)) * 2.0);
    guitimelinedarkmarkers_.push_back(position);
    guitimelinedarkmarkers_.push_back(-1.0f);
    guitimelinedarkmarkers_.push_back(position);
    guitimelinedarkmarkers_.push_back(1.0f);

    // Text
    const int pixelx = ((position + 1.0f) / 2.0f) * width();
    const int w = (step * width()) / diff;
    if (guitimelinetextsindex >= guitimelinetexts_.size())
    {
      TIMELINETEXT guitimelinetext;
      guitimelinetext.text_.setTextOption(textoption);
      guitimelinetext.text_.setTextWidth(w);
      guitimelinetext.text_.setText(timelinegenerator.toptext_(toptime));
      guitimelinetext.text_.setPerformanceHint(QStaticText::PerformanceHint::AggressiveCaching);
      guitimelinetext.position_ = QPoint(pixelx, 0);
      guitimelinetext.width_ = w;
      guitimelinetext.time_ = toptime;
      maxheight = std::max(maxheight, guitimelinetext.text_.size().height());
      guitimelinetexts_.push_back(guitimelinetext);
    }
    else
    {
      if ((guitimelinetexts_[guitimelinetextsindex].position_ != QPoint(pixelx, 0)) || (guitimelinetexts_[guitimelinetextsindex].width_ != w) || (guitimelinetexts_[guitimelinetextsindex].time_ != toptime))
      {
        guitimelinetexts_[guitimelinetextsindex].text_.setTextOption(textoption);
        guitimelinetexts_[guitimelinetextsindex].text_.setTextWidth(w);
        guitimelinetexts_[guitimelinetextsindex].text_.setText(timelinegenerator.toptext_(toptime));
        guitimelinetexts_[guitimelinetextsindex].text_.setPerformanceHint(QStaticText::PerformanceHint::AggressiveCaching);
        guitimelinetexts_[guitimelinetextsindex].position_ = QPoint(pixelx, 0);
        guitimelinetexts_[guitimelinetextsindex].width_ = w;
        guitimelinetexts_[guitimelinetextsindex].time_ = toptime;
      }
      maxheight = std::max(maxheight, guitimelinetexts_[guitimelinetextsindex].text_.size().height());
    }

    toptime = nexttime;
    ++guitimelinetextsindex;
  }

  // The horizontal line between the times
  float guitop = (((static_cast<double>(height()) - (maxheight + 1.0)) / static_cast<double>(height())) * 2.0) - 1.0;
  guitimelinedarkmarkers_.push_back(-1.0f);
  guitimelinedarkmarkers_.push_back(guitop);
  guitimelinedarkmarkers_.push_back(1.0f);
  guitimelinedarkmarkers_.push_back(guitop);

  // Bottom
  if (timelinegenerator.bottext_)
  {
    QDateTime bottime = timelinegenerator.startdatetime_;
    guitop -= pixelheight;
    while (bottime <= endtime)
    {
      QDateTime nexttime = bottime;
      nexttime = nexttime.addSecs(timelinegenerator.botsecincrement_);
      nexttime = nexttime.addDays(timelinegenerator.botdayincrement_);
      nexttime = nexttime.addMonths(timelinegenerator.botmonthincrement_);
      const uint64_t step = nexttime.toMSecsSinceEpoch() - bottime.toMSecsSinceEpoch();

      // Lines
      const float position = -1.0 + (((static_cast<double>(bottime.toMSecsSinceEpoch()) - static_cast<double>(startendtime->first)) / static_cast<double>(diff)) * 2.0);
      guitimelinelightmarkers_.push_back(position);
      guitimelinelightmarkers_.push_back(-1.0f);
      guitimelinelightmarkers_.push_back(position);
      guitimelinelightmarkers_.push_back(guitop);

      // Text
      const int pixelx = ((position + 1.0f) / 2.0f) * width();
      const int pixely = height() - ((guitop + 1.0f) / 2.0f) * height();
      const int w = (step * width()) / diff;
      if (guitimelinetextsindex >= guitimelinetexts_.size())
      {
        TIMELINETEXT guitimelinetext;
        guitimelinetext.text_.setTextOption(textoption);
        guitimelinetext.text_.setTextWidth(w);
        guitimelinetext.text_.setText(timelinegenerator.bottext_(bottime));
        guitimelinetext.text_.setPerformanceHint(QStaticText::PerformanceHint::AggressiveCaching);
        guitimelinetext.position_ = QPoint(pixelx, pixely);
        guitimelinetext.width_ = w;
        guitimelinetext.time_ = bottime;
        guitimelinetexts_.push_back(guitimelinetext);
      }
      else
      {
        if ((guitimelinetexts_[guitimelinetextsindex].position_ != QPoint(pixelx, pixely)) || (guitimelinetexts_[guitimelinetextsindex].width_ != w) || (guitimelinetexts_[guitimelinetextsindex].time_ != bottime))
        {
          guitimelinetexts_[guitimelinetextsindex].text_.setTextOption(textoption);
          guitimelinetexts_[guitimelinetextsindex].text_.setTextWidth(w);
          guitimelinetexts_[guitimelinetextsindex].text_.setText(timelinegenerator.bottext_(bottime));
          guitimelinetexts_[guitimelinetextsindex].text_.setPerformanceHint(QStaticText::PerformanceHint::AggressiveCaching);
          guitimelinetexts_[guitimelinetextsindex].position_ = QPoint(pixelx, pixely);
          guitimelinetexts_[guitimelinetextsindex].width_ = w;
          guitimelinetexts_[guitimelinetextsindex].time_ = bottime;
        }
      }

      bottime = nexttime;
      ++guitimelinetextsindex;
    }
  }

  guitimelinedarkvertices_.bind();
  guitimelinedarkvertices_.allocate(guitimelinedarkmarkers_.data(), static_cast<int>(guitimelinedarkmarkers_.size() * sizeof(float)));
  guitimelinedarkvertices_.release();

  guitimelinelightvertices_.bind();
  guitimelinelightvertices_.allocate(guitimelinelightmarkers_.data(), static_cast<int>(guitimelinelightmarkers_.size() * sizeof(float)));
  guitimelinelightvertices_.release();

  guitimelinetexts_.erase(guitimelinetexts_.begin() + guitimelinetextsindex, guitimelinetexts_.end()); // Remove the ends from guitimelinetexts_

  UpdateGUIHorizontalLines();

  // Marker
  if (GetFindMotionVideoWidget()->frametime_ != std::chrono::steady_clock::time_point())
  {
    uint64_t time = GetFindMotionVideoWidget()->playmarkertime_ - GetDevice()->GetTimeOffset();
    if (!IsPaused())
    {
      time += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - GetFindMotionVideoWidget()->frametime_).count();

    }

    const float recordingsblockstop = GetRecordingBlocksTop();
    const float top = recordingsblockstop - pixelheight;
    const float bottom = top - (pixelheight * (RECORDINGBLOCKS_HEIGHT - 1));
    const float position = std::max(-1.0, std::min(1.0, ((static_cast<double>(time) - static_cast<double>(startendtime->first)) / (static_cast<double>(startendtime->second) - static_cast<double>(startendtime->first)) * 2.0) - 1.0));
    const std::array<float, 4> vertices =
    {
      position, bottom,
      position, top
    };
    playmarkervertices_.bind();
    playmarkervertices_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(float)));
    playmarkervertices_.release();
  }

  // If a track is active, update the final recording block with the new time
  if (GetFindMotionWindow()->GetRecording()->GetState(GetFindMotionWindow()->GetTrack()) == monocle::RecordingJobState::Active)
  {
    recordingblocks_.back()->SetEndTime(endtime_);

  }

  recordingblockverticesdata_.clear();
  const float recordingsblockstop = GetRecordingBlocksTop();
  const float top = recordingsblockstop - ((0 * pixelheight * RECORDINGBLOCKS_HEIGHT) + pixelheight);
  const float bottom = top - (pixelheight * (RECORDINGBLOCKS_HEIGHT - 1));
  for (std::unique_ptr<RecordingBlock>& recordingblock : recordingblocks_)
  {
    recordingblock->Update(top, bottom, pixelwidth, startendtime->first, startendtime->second, recordingblockverticesdata_);

  }
  recordingblockvertices_.bind();
  recordingblockvertices_.allocate(recordingblockverticesdata_.data(), static_cast<int>(recordingblockverticesdata_.size() * sizeof(float)));
  recordingblockvertices_.release();

  metadatarecordingblockverticesdata_.clear();
  for (std::unique_ptr<RecordingBlock>& motionrecordingblock : motionrecordingblocks_)
  {
    motionrecordingblock->Update(top, bottom, pixelwidth, startendtime->first, startendtime->second, metadatarecordingblockverticesdata_);

  }
  metadatarecordingblockvertices_.bind();
  metadatarecordingblockvertices_.allocate(metadatarecordingblockverticesdata_.data(), static_cast<int>(metadatarecordingblockverticesdata_.size() * sizeof(float)));
  metadatarecordingblockvertices_.release();

  // Quick export start and end markers
  const float exporttextheight = pixelheight * 17.0f;
  const float exporttextwidth = pixelwidth * 150.0f;
  if (exportstarttime_.is_initialized())
  {
    exportstartvertices_.bind();
    const float position = (static_cast<float>(*exportstarttime_ - startendtime->first) / static_cast<float>(startendtime->second - startendtime->first) * 2.0f) - 1.0f;
    const std::array<float, 4> vertices =
    {
      position, -1.0f,
      position, 1.0f
    };
    exportstartvertices_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(float)));
    exportstartvertices_.release();

    starttext_.vertexbuffer_.bind();
    const float left = position - exporttextwidth - pixelwidth;
    const float right = position - pixelwidth;
    const float top = -1.0f + exporttextheight;
    const float bottom = -1.0f + pixelheight; // One pixel from the bottom
    const std::array<float, 12> textvertices =
    {
      right, bottom, 0.0f,
      right, top, 0.0f,
      left, bottom, 0.0f,
      left, top, 0.0f
    };
    starttext_.vertexbuffer_.allocate(textvertices.data(), static_cast<int>(textvertices.size() * sizeof(float)));
    starttext_.vertexbuffer_.release();
  }

  if (exportendtime_.is_initialized())
  {
    exportendvertices_.bind();
    const float position = (static_cast<float>(*exportendtime_ - startendtime->first) / static_cast<float>(startendtime->second - startendtime->first) * 2.0f) - 1.0f;
    const std::array<float, 4> vertices =
    {
      position, -1.0f,
      position, 1.0f
    };
    exportendvertices_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(float)));
    exportendvertices_.release();

    endtext_.vertexbuffer_.bind();
    const float left = position + pixelwidth;
    const float right = position + exporttextwidth + pixelwidth;
    const float top = -1.0f + exporttextheight;
    const float bottom = -1.0f + pixelheight; // One pixel from the bottom
    const std::array<float, 12> textvertices =
    {
      right, bottom, 0.0f,
      right, top, 0.0f,
      left, bottom, 0.0f,
      left, top, 0.0f
    };
    endtext_.vertexbuffer_.allocate(textvertices.data(), static_cast<int>(textvertices.size() * sizeof(float)));
    endtext_.vertexbuffer_.release();
  }

  update(); // Schedule redraw
}

void FindMotionPlaybackWidget::ZoomIn(const int x)
{
  const boost::optional< std::pair<uint64_t, uint64_t> > startendtime = GetStartEndTime();
  if (!startendtime.is_initialized())
  {

    return;
  }
  const int64_t diff = static_cast<int64_t>(startendtime->second) - static_cast<int64_t>(startendtime->first);
  const int64_t time = static_cast<int64_t>(startendtime->first) + (diff * static_cast<double>(x) / static_cast<double>(width()));
  if (diff < (40 * 1000)) // Don't zoom in too far(40 seconds)
  {

    return;
  }

  const uint64_t newstarttime = static_cast<int64_t>(time - ((time - static_cast<int64_t>(startendtime->first)) / 1.25));
  const uint64_t newendtime = static_cast<int64_t>(time + ((static_cast<int64_t>(startendtime->second) - static_cast<int64_t>(time)) / 1.25));
  zoomtimes_ = std::make_pair(newstarttime, newendtime);
  makeCurrent();
  UpdateRecordingBlocks();
  doneCurrent();
}

void FindMotionPlaybackWidget::ZoomOut(const int x)
{
  const boost::optional< std::pair<uint64_t, uint64_t> > startendtime = GetStartEndTime();
  if (!startendtime.is_initialized())
  {

    return;
  }
  const double diff = static_cast<double>(startendtime->second) - static_cast<double>(startendtime->first);
  const double time = static_cast<double>(startendtime->first) + (diff * static_cast<double>(x) / static_cast<double>(width()));
  if (!starttime_.is_initialized())
  {

    return;
  }

  const uint64_t newstarttime = std::max(START_EPOCH, static_cast<qint64>(static_cast<double>(startendtime->first) - ((time - static_cast<double>(startendtime->first)) / diff) * 0.25 * diff));
  const int64_t endepoch = std::min(END_EPOCH, QDateTime::fromMSecsSinceEpoch(newstarttime, Qt::UTC).addYears(20).toMSecsSinceEpoch()); // Don't let the user expand beyond 20 years or the END_EPOCH year
  const uint64_t newendtime = std::min(endepoch, static_cast<int64_t>(static_cast<double>(startendtime->second) + (((static_cast<double>(startendtime->second) - time) / diff) * 0.25 * diff)));
  zoomtimes_ = std::make_pair(newstarttime, newendtime);

  makeCurrent();
  UpdateRecordingBlocks();
  doneCurrent();
}

void FindMotionPlaybackWidget::FindMotionResult(const uint64_t start, const uint64_t end)
{
  motionrecordingblocks_.emplace_back(std::make_unique<RecordingBlock>(true, start, end));
  UpdateRecordingBlocks();
}

void FindMotionPlaybackWidget::SetPaused(const bool paused)
{

  return static_cast<FindMotionWindow*>(parent()->parent()->parent()->parent())->GetVideoWidget()->SetPaused(paused);
}

void FindMotionPlaybackWidget::SetPlayMarkerTime(const uint64_t time)
{
  GetFindMotionVideoWidget()->playmarkertime_ = time;
  update();
}

void FindMotionPlaybackWidget::SetFrameTime(const std::chrono::steady_clock::time_point time)
{
  GetFindMotionVideoWidget()->frametime_ = time;
  update();
}

bool FindMotionPlaybackWidget::IsPaused()
{

  return static_cast<FindMotionWindow*>(parent()->parent()->parent()->parent())->GetVideoWidget()->IsPaused();
}

const boost::shared_ptr<Connection>& FindMotionPlaybackWidget::GetConnection() const
{

  return static_cast<FindMotionWindow*>(parent()->parent()->parent()->parent())->GetConnection();
}

const boost::shared_ptr<Device>& FindMotionPlaybackWidget::GetDevice() const
{

  return static_cast<FindMotionWindow*>(parent()->parent()->parent()->parent())->GetDevice();
}

FindMotionWindow* FindMotionPlaybackWidget::GetFindMotionWindow()
{

  return static_cast<FindMotionWindow*>(parent()->parent()->parent()->parent());
}

FindMotionVideoWidget* FindMotionPlaybackWidget::GetFindMotionVideoWidget()
{

  return GetFindMotionWindow()->GetVideoWidget();
}

void FindMotionPlaybackWidget::SetExportStartTime(const uint64_t exportstarttime, const bool makecurrent)
{
  exportstarttime_ = exportstarttime;
  const QImage text = GetTextureTime(freetypearial_, exportstarttime, 0.0f, 1.0f, 0.0f);

  if (makecurrent)
  {
    makeCurrent();

  }

  glBindTexture(GL_TEXTURE_2D, starttext_.texture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text.width(), text.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, text.bits());
  glBindTexture(GL_TEXTURE_2D, 0);

  if (makecurrent)
  {
    doneCurrent();

  }
}

void FindMotionPlaybackWidget::SetExportEndTime(const uint64_t exportendtime, const bool makecurrent)
{
  exportendtime_ = exportendtime;
  const QImage text = GetTextureTime(freetypearial_, exportendtime, 0.0f, 0.0f, 1.0f);

  if (makecurrent)
  {
    makeCurrent();

  }

  glBindTexture(GL_TEXTURE_2D, endtext_.texture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text.width(), text.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, text.bits());
  glBindTexture(GL_TEXTURE_2D, 0);

  if (makecurrent)
  {
    doneCurrent();

  }
}

void FindMotionPlaybackWidget::initializeGL()
{
  connect(GetFindMotionWindow()->GetRecording().data(), &Recording::JobSourceTrackStateChanged, this, &FindMotionPlaybackWidget::JobSourceTrackStateChanged, Qt::UniqueConnection);
  connect(GetFindMotionWindow()->GetRecording().data(), &Recording::TrackSetData, this, &FindMotionPlaybackWidget::TrackSetData, Qt::UniqueConnection);
  connect(GetFindMotionWindow()->GetRecording().data(), &Recording::TrackDeleteData, this, &FindMotionPlaybackWidget::TrackDeleteData, Qt::UniqueConnection);

  initializeOpenGLFunctions();

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  const QColor backgroundcolor = palette().color(QPalette::Window);
  glClearColor(backgroundcolor.redF(), backgroundcolor.greenF(), backgroundcolor.blueF(), 0.0f);

  // Recordings blocks shader
  if (!recordingsblocksshader_.addShaderFromSourceCode(QOpenGLShader::Vertex, RECORDINGBLOCKS_VERTEX_SHADER))
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }
  if (!recordingsblocksshader_.addShaderFromSourceCode(QOpenGLShader::Fragment, RECORDINGBLOCKS_PIXEL_SHADER))
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  recordingsblocksshader_.bindAttributeLocation("position", 0);
  if (!recordingsblocksshader_.link())
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::link failed"));
    return;
  }
  recordingsblockspositionlocation_ = recordingsblocksshader_.attributeLocation("position");
  recordingsblockscolourlocation_ = recordingsblocksshader_.uniformLocation("colour");

  // Play marker shader
  if (!markershader_.addShaderFromSourceCode(QOpenGLShader::Vertex, MARKER_VERTEX_SHADER))
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!markershader_.addShaderFromSourceCode(QOpenGLShader::Fragment, MARKER_PIXEL_SHADER))
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  markershader_.bindAttributeLocation("position", 0);
  if (!markershader_.link())
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::bindAttributeLocation failed"));
    return;
  }
  markerpositionlocation_ = markershader_.attributeLocation("position");
  markercolourlocation_ = markershader_.uniformLocation("colour");

  // Text shader
  if (!textshader_.addShaderFromSourceCode(QOpenGLShader::Vertex, TEXT_VERTEX_SHADER))
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!textshader_.addShaderFromSourceCode(QOpenGLShader::Fragment, TEXT_PIXEL_SHADER))
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  textshader_.bindAttributeLocation("position", 0);
  textshader_.bindAttributeLocation("texcoord", 1);

  if (!textshader_.link())
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::link failed"));
    return;
  }

  texttexturecoordlocation_ = textshader_.attributeLocation("texcoord");
  if (texttexturecoordlocation_ == -1)
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  textpositionlocation_ = textshader_.attributeLocation("position");
  if (textpositionlocation_ == -1)
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  texttexturesamplerlocation_ = textshader_.uniformLocation("sampler");
  if (texttexturesamplerlocation_ == -1)
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  recordingblockvertices_.create();
  recordingblockvertices_.setUsagePattern(QOpenGLBuffer::DynamicDraw);

  metadatarecordingblockvertices_.create();
  metadatarecordingblockvertices_.setUsagePattern(QOpenGLBuffer::DynamicDraw);

  // Horizontal lines
  guihorizontalvertices_.create();
  guihorizontalvertices_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  UpdateGUIHorizontalLines();
  guitimelinedarkvertices_.create();
  guitimelinedarkvertices_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  guitimelinelightvertices_.create();
  guitimelinelightvertices_.setUsagePattern(QOpenGLBuffer::StaticDraw);

  // Export markers
  exportstartvertices_.create();
  exportstartvertices_.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  exportendvertices_.create();
  exportendvertices_.setUsagePattern(QOpenGLBuffer::DynamicDraw);

  // Export text
  starttext_.texturebuffer_.create();
  starttext_.texturebuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  starttext_.texturebuffer_.bind();
  starttext_.texturebuffer_.allocate(texturecoords_.data(), static_cast<int>(texturecoords_.size() * sizeof(float)));
  starttext_.texturebuffer_.release();

  endtext_.texturebuffer_.create();
  endtext_.texturebuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  endtext_.texturebuffer_.bind();
  endtext_.texturebuffer_.allocate(texturecoords_.data(), static_cast<int>(texturecoords_.size() * sizeof(float)));
  endtext_.texturebuffer_.release();

  glGenTextures(1, &starttext_.texture_);
  glBindTexture(GL_TEXTURE_2D, starttext_.texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glGenTextures(1, &endtext_.texture_);
  glBindTexture(GL_TEXTURE_2D, endtext_.texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  starttext_.vertexbuffer_.create();
  starttext_.vertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);

  endtext_.vertexbuffer_.create();
  endtext_.vertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);

  // Play marker
  playmarkervertices_.create();
  playmarkervertices_.setUsagePattern(QOpenGLBuffer::DynamicDraw);

  // Set the default export timers
  SetExportStartTime(GetFindMotionWindow()->GetStartTime().toMSecsSinceEpoch(), false);
  SetExportEndTime(GetFindMotionWindow()->GetEndTime().toMSecsSinceEpoch(), false);

  UpdateRecordingBlocks();

  if (timer_ == -1)
  {
    timer_ = startTimer(100);

  }
}

void FindMotionPlaybackWidget::resizeGL(int width, int height)
{
  UpdateRecordingBlocks();

}

void FindMotionPlaybackWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // static GUI elements
  if (recordingblocks_.size())
  {
    markershader_.bind();

    // Horizontal lines
    if (guihorizontalvertices_.size())
    {
      guihorizontalvertices_.bind();
      markershader_.enableAttributeArray(markerpositionlocation_);
      markershader_.setAttributeBuffer(markerpositionlocation_, GL_FLOAT, 0, 2);
      markershader_.setUniformValue(markercolourlocation_, QVector4D(0.4f, 0.4f, 0.4f, 1.0f));
      glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(4));
      guihorizontalvertices_.release();
    }

    if (guitimelinelightvertices_.size())
    {
      guitimelinelightvertices_.bind();
      markershader_.enableAttributeArray(markerpositionlocation_);
      markershader_.setAttributeBuffer(markerpositionlocation_, GL_FLOAT, 0, 2);
      markershader_.setUniformValue(markercolourlocation_, QVector4D(0.4f, 0.4f, 0.4f, 1.0f));
      glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(guitimelinelightvertices_.size() * 2));
      guitimelinelightvertices_.release();
    }

    if (guitimelinedarkvertices_.size())
    {
      guitimelinedarkvertices_.bind();
      markershader_.enableAttributeArray(markerpositionlocation_);
      markershader_.setAttributeBuffer(markerpositionlocation_, GL_FLOAT, 0, 2);
      markershader_.setUniformValue(markercolourlocation_, QVector4D(0.15f, 0.15f, 0.15f, 1.0f));
      glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(guitimelinedarkvertices_.size() * 2));
      guitimelinedarkvertices_.release();
    }
    markershader_.release();

    if (guitimelinetexts_.size())
    {
      QPainter painter(this);
      glEnable(GL_BLEND);
      glActiveTexture(GL_TEXTURE0);
      textshader_.bind();
      for (TIMELINETEXT& guitimelinetext : guitimelinetexts_)
      {
        painter.drawStaticText(guitimelinetext.position_, guitimelinetext.text_);

      }
      textshader_.release();
      glDisable(GL_BLEND);
    }
  }

  // Recording blocks
  recordingsblocksshader_.bind();
  if (actionvideo_->isChecked())
  {
    recordingblockvertices_.bind();
    recordingsblocksshader_.enableAttributeArray(recordingsblockspositionlocation_);
    recordingsblocksshader_.setAttributeBuffer(recordingsblockspositionlocation_, GL_FLOAT, 0, 2);
    recordingsblocksshader_.setUniformValue(recordingsblockscolourlocation_, colour_);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(recordingblockverticesdata_.size()));
    recordingblockvertices_.release();
  }

  if (actionmotion_->isChecked())
  {
    metadatarecordingblockvertices_.bind();
    recordingsblocksshader_.enableAttributeArray(recordingsblockspositionlocation_);
    recordingsblocksshader_.setAttributeBuffer(recordingsblockspositionlocation_, GL_FLOAT, 0, 2);
    recordingsblocksshader_.setUniformValue(recordingsblockscolourlocation_, colour_ * 2);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(metadatarecordingblockverticesdata_.size()));
    metadatarecordingblockvertices_.release();
  }
  recordingsblocksshader_.release();

  // Play markers
  markershader_.bind();
  if (starttime_.is_initialized() && (GetFindMotionVideoWidget()->frametime_ != std::chrono::steady_clock::time_point()))
  {
    playmarkervertices_.bind();
    markershader_.enableAttributeArray(markerpositionlocation_);
    markershader_.setAttributeBuffer(markerpositionlocation_, GL_FLOAT, 0, 2);
    markershader_.setUniformValue(markercolourlocation_, QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
    glDrawArrays(GL_LINE_STRIP, 0, 2);
    playmarkervertices_.release();
  }

  // Export markers
  if (recordingblocks_.size())
  {
    if (exportstarttime_.is_initialized())
    {
      // Marker
      exportstartvertices_.bind();
      markershader_.enableAttributeArray(markerpositionlocation_);
      markershader_.setAttributeBuffer(markerpositionlocation_, GL_FLOAT, 0, 2);
      markershader_.setUniformValue(markercolourlocation_, QVector4D(0.0f, 1.0f, 0.0f, 1.0f));
      glDrawArrays(GL_LINE_STRIP, 0, 2);
      exportstartvertices_.release();
    }

    if (exportendtime_.is_initialized())
    {
      exportendvertices_.bind();
      markershader_.enableAttributeArray(markerpositionlocation_);
      markershader_.setAttributeBuffer(markerpositionlocation_, GL_FLOAT, 0, 2);
      markershader_.setUniformValue(markercolourlocation_, QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
      glDrawArrays(GL_LINE_STRIP, 0, 2);
      exportendvertices_.release();
    }
  }
  markershader_.release();

  // Export markers text
  glEnable(GL_BLEND);
  glActiveTexture(GL_TEXTURE0);
  textshader_.bind();
  if (recordingblocks_.size())
  {
    if (exportstarttime_.is_initialized())
    {
      textshader_.setUniformValue(texttexturesamplerlocation_, 0);
      glBindTexture(GL_TEXTURE_2D, starttext_.texture_);

      starttext_.texturebuffer_.bind();
      textshader_.enableAttributeArray(texttexturecoordlocation_);
      textshader_.setAttributeBuffer(texttexturecoordlocation_, GL_FLOAT, 0, 2);
      starttext_.vertexbuffer_.bind();
      textshader_.enableAttributeArray(textpositionlocation_);
      textshader_.setAttributeBuffer(textpositionlocation_, GL_FLOAT, 0, 3);

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      textshader_.disableAttributeArray(textpositionlocation_);
      starttext_.vertexbuffer_.release();
      textshader_.disableAttributeArray(texttexturecoordlocation_);
      starttext_.texturebuffer_.release();
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    if (exportendtime_.is_initialized())
    {
      textshader_.setUniformValue(texttexturesamplerlocation_, 0);
      glBindTexture(GL_TEXTURE_2D, endtext_.texture_);

      endtext_.texturebuffer_.bind();
      textshader_.enableAttributeArray(texttexturecoordlocation_);
      textshader_.setAttributeBuffer(texttexturecoordlocation_, GL_FLOAT, 0, 2);
      endtext_.vertexbuffer_.bind();
      textshader_.enableAttributeArray(textpositionlocation_);
      textshader_.setAttributeBuffer(textpositionlocation_, GL_FLOAT, 0, 3);

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      textshader_.disableAttributeArray(textpositionlocation_);
      endtext_.vertexbuffer_.release();
      textshader_.disableAttributeArray(texttexturecoordlocation_);
      endtext_.texturebuffer_.release();
      glBindTexture(GL_TEXTURE_2D, 0);
    }
  }
  textshader_.release();
  glDisable(GL_BLEND);
}

void FindMotionPlaybackWidget::timerEvent(QTimerEvent*)
{
  makeCurrent();
  UpdateRecordingBlocks();
  doneCurrent();
}

void FindMotionPlaybackWidget::mouseMoveEvent(QMouseEvent* event)
{
  const boost::optional< std::pair<uint64_t, uint64_t> > startendtime = GetStartEndTime();
  if (!startendtime.is_initialized())
  {

    return;
  }

  if (state_ == PLAYBACKMOUSESTATE_MOVESTART)
  {
    if (!exportstarttime_.is_initialized()) // Shouldn't happen but just in case
    {
      SetState(PLAYBACKMOUSESTATE_IDLE);
      return;
    }

    if ((event->pos().x() <= 0) || (event->pos().x() > width()))
    {

      return;
    }

    const uint64_t newtime = std::min(static_cast<uint64_t>(((static_cast<double>(event->pos().x()) / static_cast<double>(width())) * (startendtime->second - startendtime->first)) + startendtime->first), exportendtime_.is_initialized() ? *exportendtime_ : std::numeric_limits<uint64_t>::max());
    GetFindMotionWindow()->SetStartTime(newtime);
    makeCurrent();
    SetExportStartTime(newtime, false);
    UpdateRecordingBlocks();
    doneCurrent();
  }
  else if (state_ == PLAYBACKMOUSESTATE_MOVEEND)
  {
    if (!exportendtime_.is_initialized()) // Shouldn't happen but just in case
    {
      SetState(PLAYBACKMOUSESTATE_IDLE);
      return;
    }

    if ((event->pos().x() <= 0) || (event->pos().x() > width()))
    {

      return;
    }

    const uint64_t newtime = std::max(static_cast<uint64_t>(((static_cast<double>(event->pos().x()) / static_cast<double>(width())) * (startendtime->second - startendtime->first)) + startendtime->first), exportstarttime_.is_initialized() ? *exportstarttime_ : std::numeric_limits<uint64_t>::max());
    GetFindMotionWindow()->SetEndTime(newtime);
    makeCurrent();
    SetExportEndTime(newtime, false); // Cap at the start time
    UpdateRecordingBlocks();
    doneCurrent();
  }
  else if (state_ == PLAYBACKMOUSESTATE_CLICKED)
  {
    if (starttime_.is_initialized() && !zoomtimes_.is_initialized())
    {

      return;
    }
    SetState(PLAYBACKMOUSESTATE_MOVE);
  }
  else if (state_ == PLAYBACKMOUSESTATE_MOVE)
  {
    if (!starttime_.is_initialized() || !zoomtimes_.is_initialized())
    {

      return;
    }
    const uint64_t diff = zoomtimes_->second - zoomtimes_->first;
    const uint64_t timediff = (static_cast<double>(clickedpos_ - event->x()) / static_cast<double>(width())) * diff;

    if (static_cast<int64_t>(zoomtimes_->first + timediff) < START_EPOCH)
    {
      zoomtimes_->first = START_EPOCH;
      zoomtimes_->second = START_EPOCH + diff;
    }
    else if (static_cast<int64_t>(zoomtimes_->second + timediff) > END_EPOCH)
    {
      zoomtimes_->first = END_EPOCH - diff;
      zoomtimes_->second = END_EPOCH;
    }
    else
    {
      zoomtimes_->first += timediff;
      zoomtimes_->second += timediff;
      clickedpos_ = event->pos().x();
    }

    makeCurrent();
    UpdateRecordingBlocks();
    doneCurrent();
  }
  else
  {
    // Set the cursor when it goes over the save times
    if ((exportstarttime_.is_initialized() ? Hit(*exportstarttime_, event->pos().x()) : false) || (exportendtime_.is_initialized() ? Hit(*exportendtime_, event->pos().x()) : false))
    {
      setCursor(Qt::SizeHorCursor);

    }
    else
    {
      unsetCursor();

    }
  }
}

void FindMotionPlaybackWidget::mousePressEvent(QMouseEvent* event)
{
  if (event->pos().x() < (width() / 2)) // Prioritise grabbing the end marker
  {
    if (exportendtime_.is_initialized())
    {
      if (Hit(*exportendtime_, event->pos().x()))
      {
        SetState(PLAYBACKMOUSESTATE_MOVEEND);
        return;
      }
    }

    if (exportstarttime_.is_initialized())
    {
      if (Hit(*exportstarttime_, event->pos().x()))
      {
        SetState(PLAYBACKMOUSESTATE_MOVESTART);
        return;
      }
    }
  }
  else // Prioritise grabbing the start marker
  {
    if (exportstarttime_.is_initialized())
    {
      if (Hit(*exportstarttime_, event->pos().x()))
      {
        SetState(PLAYBACKMOUSESTATE_MOVESTART);
        return;
      }
    }

    if (exportendtime_.is_initialized())
    {
      if (Hit(*exportendtime_, event->pos().x()))
      {
        SetState(PLAYBACKMOUSESTATE_MOVEEND);
        return;
      }
    }
  }

  SetState(PLAYBACKMOUSESTATE_CLICKED);
  clickedpos_ = event->pos().x();
}

void FindMotionPlaybackWidget::mouseReleaseEvent(QMouseEvent* event)
{
  if (state_ == PLAYBACKMOUSESTATE_CLICKED) // View the currently selected time
  {
    const boost::optional< std::pair<uint64_t, uint64_t> > startendtime = GetStartEndTime();
    if (!startendtime.is_initialized())
    {

      return;
    }

    makeCurrent();
    if (event->button() == Qt::LeftButton)
    {
      if (event->modifiers() & Qt::SHIFT)
      {
        const uint64_t time = startendtime->first + (static_cast<double>(event->pos().x()) / static_cast<double>(width()) * static_cast<double>(startendtime->second - startendtime->first));
        if (!exportstarttime_.is_initialized())
        {
          GetFindMotionWindow()->SetStartTime(time);
          SetExportStartTime(time, false);
        }
        else if (exportstarttime_.is_initialized() && !exportendtime_.is_initialized() && (time < *exportstarttime_))
        {
          GetFindMotionWindow()->SetStartTime(time);
          SetExportStartTime(time, false);
        }
        else if (!exportendtime_.is_initialized())
        {
          GetFindMotionWindow()->SetEndTime(time);
          SetExportEndTime(time, false);
        }
        else
        {
          exportstarttime_.reset();
          exportendtime_.reset();
        }
      }
      else
      {
        // User wants to just play from this point
        const uint64_t time = startendtime->first + ((static_cast<double>(event->pos().x()) / static_cast<double>(width())) * static_cast<double>(startendtime->second - startendtime->first));
        if (IsPaused())
        {
          GetFindMotionWindow()->Play(time, 1);
          GetFindMotionVideoWidget()->playmarkertime_ = time + GetDevice()->GetTimeOffset();
          GetFindMotionVideoWidget()->frametime_ = std::chrono::steady_clock::now();
        }
        else
        {
          if (time >= endtime_)
          {
            // User has selected a time beyond its latest time, so just go live
            GetFindMotionVideoWidget()->playmarkertime_ = endtime_;
            GetFindMotionVideoWidget()->frametime_ = std::chrono::steady_clock::now();
            GetFindMotionWindow()->Stop();
          }
          else
          {
            GetFindMotionWindow()->Play(time, boost::none);
            GetFindMotionVideoWidget()->playmarkertime_ = time + GetDevice()->GetTimeOffset();
            GetFindMotionVideoWidget()->frametime_ = std::chrono::steady_clock::now();
          }
        }
      }
    }
    doneCurrent();
    update();
  }
  SetState(PLAYBACKMOUSESTATE_IDLE);
}

void FindMotionPlaybackWidget::wheelEvent(QWheelEvent* event)
{
  if (event->delta() > 0)
  {
    ZoomIn(event->pos().x());

  }
  else if (event->delta() < 0)
  {
    ZoomOut(event->pos().x());

  }
}

void FindMotionPlaybackWidget::contextMenuEvent(QContextMenuEvent* event)
{
  QMenu* menu = new QMenu(GetFindMotionWindow());
  menu->addAction(actionvideo_);
  menu->addAction(actionmotion_);
  menu->exec(event->globalPos());
}

boost::optional<uint64_t> FindMotionPlaybackWidget::GetStartTime() const
{
  if (recordingblocks_.empty())
  {

    return boost::none;
  }
  
  uint64_t starttime = std::numeric_limits<uint64_t>::max();
  for (const std::unique_ptr<RecordingBlock>& recordingblock : recordingblocks_)
  {
    starttime = std::min(starttime, recordingblock->GetStartTime());

  }
  for (const std::unique_ptr<RecordingBlock>& motionrecordingblock : motionrecordingblocks_)
  {
    starttime = std::min(starttime, motionrecordingblock->GetStartTime());

  }
  return starttime;
}

boost::optional< std::pair<uint64_t, uint64_t> > FindMotionPlaybackWidget::GetStartEndTime() const
{
  if (!starttime_.is_initialized())
  {

    return boost::none;
  }

  if (zoomtimes_.is_initialized())
  {

    return zoomtimes_;
  }
  else
  {

    return std::make_pair(*starttime_, endtime_);
  }
}

float FindMotionPlaybackWidget::GetRecordingBlocksTop() const
{
  if (guitimelinetexts_.size())
  {
    auto i = std::max_element(guitimelinetexts_.cbegin(), guitimelinetexts_.cend(), [](const TIMELINETEXT& lhs, const TIMELINETEXT& rhs) { return ((lhs.position_.y() + lhs.text_.size().height()) < (rhs.position_.y() + rhs.text_.size().height())); });
    const float pixelheight = 2.0f / static_cast<float>(height());
    return (1.0f - pixelheight - (static_cast<float>(i->position_.y() + i->text_.size().height()) * pixelheight));
  }
  else
  {

    return 1.0f;
  }
}

void FindMotionPlaybackWidget::UpdateGUIHorizontalLines()
{
  const float pixelheight = 2.0f / static_cast<float>(height());
  std::vector<float> guihorizontalvertices;
  guihorizontalvertices.reserve(8);

  const float recordingsblockstop = GetRecordingBlocksTop();
  guihorizontalvertices.push_back(-1.0f);
  guihorizontalvertices.push_back(recordingsblockstop);
  guihorizontalvertices.push_back(1.0f);
  guihorizontalvertices.push_back(recordingsblockstop);
  const float height = recordingsblockstop - (pixelheight * RECORDINGBLOCKS_HEIGHT);
  guihorizontalvertices.push_back(-1.0f);
  guihorizontalvertices.push_back(height);
  guihorizontalvertices.push_back(1.0f);
  guihorizontalvertices.push_back(height);

  guihorizontalvertices_.bind();
  guihorizontalvertices_.allocate(guihorizontalvertices.data(), static_cast<int>(guihorizontalvertices.size() * sizeof(float)));
  guihorizontalvertices_.release();
}

bool FindMotionPlaybackWidget::Hit(const uint64_t time, const int x)
{
  const boost::optional< std::pair<uint64_t, uint64_t> > startendtime = GetStartEndTime();
  if (!startendtime.is_initialized())
  {

    return false;
  }

  const int timex = (static_cast<double>(time - startendtime->first) / static_cast<double>(startendtime->second - startendtime->first)) * width();
  return ((x == timex) || ((x - 1) == timex) || ((x + 1) == timex));
}

void FindMotionPlaybackWidget::SetState(const PLAYBACKMOUSESTATE state)
{
  if ((state == PLAYBACKMOUSESTATE_IDLE) || state == (PLAYBACKMOUSESTATE_CLICKED))
  {
    unsetCursor();

  }
  else if ((state == PLAYBACKMOUSESTATE_MOVESTART) || (state == PLAYBACKMOUSESTATE_MOVEEND))
  {
    setCursor(Qt::SizeHorCursor);

  }
  else if (state == PLAYBACKMOUSESTATE_MOVE)
  {
    setCursor(Qt::SizeAllCursor);

  }
  state_ = state;
}

void FindMotionPlaybackWidget::ShowVideo(bool)
{

}

void FindMotionPlaybackWidget::ShowMotion(bool)
{

}

void FindMotionPlaybackWidget::TimeOffsetChanged(const boost::shared_ptr<Device>& device)
{
  makeCurrent();
  UpdateRecordingBlocks();
  doneCurrent();
}

void FindMotionPlaybackWidget::JobSourceTrackStateChanged(const QSharedPointer<client::RecordingJob>& job, const QSharedPointer<client::RecordingJobSource>& source, const QSharedPointer<client::RecordingJobSourceTrack>& track, uint64_t time, const monocle::RecordingJobState state, const QString& error, const monocle::RecordingJobState prevstate)
{
  if (GetFindMotionWindow()->GetTrack() != track->GetTrack())
  {

    return;
  }

  time -= GetDevice()->GetTimeOffset();

  makeCurrent();
  const boost::optional< std::pair<uint64_t, uint64_t> > startendtime = GetStartEndTime();
  if (state == monocle::RecordingJobState::Active) // Create or extend the RecordingBlock
  {
    if (recordingblocks_.empty())
    {
      std::unique_ptr<RecordingBlock> rb = std::make_unique<RecordingBlock>(false, time, endtime_);
      recordingblocks_.emplace_back(std::move(rb));
    }
    else
    {
      if ((time > recordingblocks_.back()->GetEndTime()) && ((time - recordingblocks_.back()->GetEndTime()) < 5000)) // Extend the oldest RecordingBlock or create a new one
      {
        recordingblocks_.back()->SetEndTime(time);

      }
      else
      {
        std::unique_ptr<RecordingBlock> rb = std::make_unique<RecordingBlock>(false, time, endtime_);
        recordingblocks_.emplace_back(std::move(rb));
      }
    }
    if (startendtime.is_initialized())
    {
      UpdateRecordingBlocks();
      update();
    }
  }
  else if ((prevstate == monocle::RecordingJobState::Active) && ((state == monocle::RecordingJobState::Idle) || (state == monocle::RecordingJobState::Error))) // Close the oldest RecordingBlock
  {
    if (!recordingblocks_.empty())
    {
      recordingblocks_.back()->SetEndTime(time);
      if (startendtime.is_initialized())
      {
        UpdateRecordingBlocks();
        update();
      }
    }
  }
  doneCurrent();
}

void FindMotionPlaybackWidget::TrackSetData(const QSharedPointer<client::RecordingTrack>& track, const std::vector<monocle::INDEX>& indices)
{
  if (GetFindMotionWindow()->GetTrack() != track)
  {

    return;
  }

  makeCurrent();
  UpdateRecordingBlocks();
  doneCurrent();
  update();
}

void FindMotionPlaybackWidget::TrackDeleteData(const QSharedPointer<client::RecordingTrack>& track, const monocle::RecordingJobState state, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end)
{
  if (GetFindMotionWindow()->GetTrack() != track)
  {

    return;
  }

  makeCurrent();
  if (!start.is_initialized() && !end.is_initialized()) // Delete the whole track
  {
    recordingblocks_.clear();
    return;
  }

  const int64_t offset = GetDevice()->GetTimeOffset();
  const uint64_t s = start.is_initialized() ? (*start + offset) : 0;
  const uint64_t e = end.is_initialized() ? (*end + offset) : std::numeric_limits<uint64_t>::max();
  recordingblocks_.erase(std::remove_if(recordingblocks_.begin(), recordingblocks_.end(), [s, e](const std::unique_ptr<RecordingBlock>& index) { return ((index->GetStartTime() >= s) && (index->GetEndTime() <= e)); }), recordingblocks_.end()); // Remove any indices that are completely out of scope

  // If we are cutting an index from both sides, we need to split it. This can only happen if both start and end are both not chopping indefinitely
  if (start.is_initialized() && end.is_initialized())
  {
    std::vector< std::pair<uint64_t, uint64_t> > splitindices;
    for (std::vector< std::unique_ptr<RecordingBlock> >::iterator recordingblock = recordingblocks_.begin(); recordingblock != recordingblocks_.end();)
    {
      if ((*recordingblock)->GetStartTime() < *start && (*recordingblock)->GetEndTime() > *end)
      {
        splitindices.push_back(std::make_pair((*recordingblock)->GetStartTime(), *start));
        splitindices.push_back(std::make_pair(*end, (*recordingblock)->GetEndTime()));
        recordingblock = recordingblocks_.erase(recordingblock);
      }
      else
      {
        ++recordingblock;

      }
    }

    for (const std::pair<uint64_t, uint64_t>& splitindex : splitindices)
    {
      recordingblocks_.emplace_back(std::make_unique<RecordingBlock>(false, splitindex.first - offset, splitindex.second - offset));
    }
  }

  // If we are cutting it from one side
  for (std::unique_ptr<RecordingBlock>& recordingblock : recordingblocks_)
  {
    if ((s <= recordingblock->GetStartTime()) && (recordingblock->GetStartTime() < e) && (recordingblock->GetEndTime() >= e))
    {
      recordingblock->SetStartTime(e);

    }
    else if ((e > recordingblock->GetEndTime()) && (recordingblock->GetStartTime() <= s) && (recordingblock->GetEndTime() > s))
    {
      recordingblock->SetEndTime(s);

    }
  }

  if (end.is_initialized() && recordingblocks_.empty() && (state == monocle::RecordingJobState::Active)) // If there is an attempt to remove all the blocks, yet we are active, create one at the end
  {
    std::unique_ptr<RecordingBlock> recordingblock = std::make_unique<RecordingBlock>(false, *end + offset, *end + offset);
    recordingblocks_.emplace_back(std::move(recordingblock));
  }

  UpdateRecordingBlocks();
  doneCurrent();
}

}
