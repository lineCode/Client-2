// playbackwidget.cpp
//

///// Includes /////

#include <boost/asio.hpp>

#include "monocleclient/playbackwidget.h"

#include <algorithm>
#include <boost/format.hpp>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <QPalette>
#include <QRgb>

#ifdef _WIN32
#include <GLES3/gl3.h>
#endif

#include "monocleclient/exportwindow.h"
#include "monocleclient/recordingblock.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/recording.h"
#include "monocleclient/videoview.h"

///// Namespaces /////

namespace client
{

///// Glboals /////

const std::chrono::milliseconds SCRUB_DELAY(100);
const qint64 START_EPOCH = QDateTime(QDate(2016, 1, 1), QTime(0, 0, 0)).toMSecsSinceEpoch(); // Stop a user going back further than this, just for convenience
const qint64 END_EPOCH = QDateTime(QDate(2200, 1, 1), QTime(0, 0, 0)).toMSecsSinceEpoch(); // Stop a user going further than this, just for convenience

const std::array<float, 8> texturecoords_ =
{
  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 1.0f,
  0.0f, 0.0f
};

const int RECORDINGBLOCKS_HEIGHT = 10;
const int PLAYBACK_TEXT_FONT_HEIGHT = 32;
const int PLAYBACK_TEXT_BORDER = 5;
const int PLAYBACK_TEXTURE_TEXT_WIDTH = 512;
const int PLAYBACK_TEXTURE_TEXT_HEIGHT = (PLAYBACK_TEXT_BORDER * 2) + PLAYBACK_TEXT_FONT_HEIGHT;

///// Functions /////

QString OrdinalIndicator(const int value)
{
  static const char suffixes[][3] = { "th", "st", "nd", "rd" };
  int ord = value % 100;
  if ((ord / 10) == 1)
  {
    ord = 0;

  }
  ord = ord % 10;
  if (ord > 3)
  {
    ord = 0;

  }
  return suffixes[ord];
}

QString Hour(const int hour)
{
  if (hour == 12)
  {

    return (QString::number(hour) + "pm");
  }
  else if (hour > 12)
  {

    return (QString::number(hour - 12) + "pm");
  }
  else
  {

    return (QString::number(hour) + "am");
  }
}

TIMELINEGENERATOR TimelineGenerator(const uint64_t starttime, const uint64_t endtime)
{
  const uint64_t diff = endtime - starttime;

  uint64_t topsecincrement = 0;
  uint64_t topdayincrement = 0;
  uint64_t topmonthincrement = 0;
  uint64_t topyearincrement = 0;
  uint64_t botsecincrement = 0;
  uint64_t botdayincrement = 0;
  uint64_t botmonthincrement = 0;
  QString(*toptext)(const QDateTime&) = nullptr;
  QString(*bottext)(const QDateTime&) = nullptr;
  QDateTime startdatetime = QDateTime::fromMSecsSinceEpoch(starttime, Qt::UTC);
  if (diff <= (60000ull * 1ull)) // Minutes + seconds
  {
    startdatetime = startdatetime.addSecs(-60);
    startdatetime = QDateTime(startdatetime.date(), QTime(startdatetime.time().hour(), startdatetime.time().minute(), 0), Qt::UTC);
    topsecincrement = 60;
    botsecincrement = 1;
    toptext = [](const QDateTime& datetime) { QString minute; return QString::number(datetime.time().hour()) + ":" + minute.sprintf("%02d ", datetime.time().minute()) + QString::number(datetime.date().day()) + OrdinalIndicator(datetime.date().day()) + " " + QDate::shortMonthName(datetime.date().month()); };
    bottext = [](const QDateTime& datetime) { return QString::number(datetime.time().second()); };
  }
  //TODO 15min + minutes ?
  else if (diff <= (60000ull * 60ull * 1ull)) // Hours + minutes
  {
    startdatetime = startdatetime.addSecs(-60 * 60);
    startdatetime = QDateTime(startdatetime.date(), QTime(startdatetime.time().hour(), 0, 0), Qt::UTC);
    topsecincrement = 60 * 60;
    botsecincrement = 60;
    toptext = [](const QDateTime& datetime) { return Hour(datetime.time().hour()) + " " + QString::number(datetime.date().day()) + OrdinalIndicator(datetime.date().day()) + " " + QDate::shortMonthName(datetime.date().month()); };
    bottext = [](const QDateTime& datetime) { return QString::number(datetime.time().minute()); };
  }
  else if (diff <= (60000ull * 60ull * 8ull)) // Hours + 15minutes
  {
    startdatetime = startdatetime.addSecs(-60 * 60);
    startdatetime = QDateTime(startdatetime.date(), QTime(startdatetime.time().hour(), 0, 0), Qt::UTC);
    topsecincrement = 60 * 60;
    botsecincrement = 60 * 15;
    toptext = [](const QDateTime& datetime) { return Hour(datetime.time().hour()) + " " + QString::number(datetime.date().day()) + OrdinalIndicator(datetime.date().day()) + " " + QDate::shortMonthName(datetime.date().month()); };
    bottext = [](const QDateTime& datetime) { return QString::number(datetime.time().minute()); };
  }
  else if (diff <= (90000000ull * 1ull)) // Days + hours
  {
    startdatetime = startdatetime.addDays(-1);
    startdatetime = QDateTime(startdatetime.date(), QTime(0, 0, 0), Qt::UTC);
    topdayincrement = 1;
    botsecincrement = 60 * 60;
    toptext = [](const QDateTime& datetime) { return QString::number(datetime.date().day()) + OrdinalIndicator(datetime.date().day()) + " " + QDate::shortMonthName(datetime.date().month()) + " " + QString::number(datetime.date().year()); };
    bottext = [](const QDateTime& datetime) { return Hour(datetime.time().hour()); };
  }
  else if (diff <= (86400000ull * 30ull * 1ull)) // Months + days
  {
    startdatetime = startdatetime.addMonths(-1);
    startdatetime = QDateTime(QDate(startdatetime.date().year(), startdatetime.date().month(), 1), QTime(0, 0, 0), Qt::UTC);
    topmonthincrement = 1;
    botdayincrement = 1;
    toptext = [](const QDateTime& datetime) { return (QDate::shortMonthName(datetime.date().month()) + " " + QString::number(datetime.date().year())); };
    bottext = [](const QDateTime& datetime) { return QString::number(datetime.date().day()) + OrdinalIndicator(datetime.date().day()); };
  }
  //TODO can we squeeze month + weeks in here?
    //TODO I think we would want 1-7th or something
    //TODO addDays(7)
  else if (diff <= (86400000ull * 30ull * 12ull * 3ull)) // Years + month
  {
    startdatetime = startdatetime.addYears(-1);
    startdatetime = QDateTime(QDate(startdatetime.date().year(), 1, 1), QTime(0, 0, 0), Qt::UTC);
    topyearincrement = 1;
    botmonthincrement = 1;
    toptext = [](const QDateTime& datetime) { return QString::number(datetime.date().year()); };
    bottext = [](const QDateTime& datetime) { return QDate::shortMonthName(datetime.date().month()); };
  }
  else // Years
  {
    startdatetime = startdatetime.addYears(-1);
    startdatetime = QDateTime(QDate(startdatetime.date().year(), 1, 1), QTime(0, 0, 0), Qt::UTC);
    topyearincrement = 1;
    toptext = [](const QDateTime& datetime) { return QString::number(datetime.date().year()); };
  }

  return TIMELINEGENERATOR(topsecincrement, topdayincrement, topmonthincrement, topyearincrement, botsecincrement, botdayincrement, botmonthincrement, toptext, bottext, startdatetime);
}

QImage GetTextureTime(const FT_Face freetypearial, const uint64_t exporttime, const float red, const float green, const float blue)
{
  QImage texturebuffer(PLAYBACK_TEXTURE_TEXT_WIDTH, PLAYBACK_TEXTURE_TEXT_HEIGHT, QImage::Format_RGBA8888);
  QPainter painter(&texturebuffer);
  memset(texturebuffer.bits(), 0, texturebuffer.sizeInBytes());
  int x = PLAYBACK_TEXT_BORDER;
  const int maxfontheight = static_cast<int>(PLAYBACK_TEXT_FONT_HEIGHT * (static_cast<float>(freetypearial->bbox.yMax) / static_cast<float>(freetypearial->bbox.yMax - freetypearial->bbox.yMin)));
  const int y = PLAYBACK_TEXT_BORDER + (PLAYBACK_TEXT_FONT_HEIGHT - maxfontheight) - 1;
  const std::string text = QDateTime::fromMSecsSinceEpoch(exporttime, Qt::UTC).toString("d/M/yy hh:mm:ss").toStdString();
  for (int i = 0; i < text.size(); ++i)
  {
    if (text.at(i) == '\0')
    {

      break;
    }

    if (FT_Load_Char(freetypearial, text.at(i), FT_LOAD_RENDER))
    {

      continue; // Ignore errors
    }
    
    QImage character = QImage(freetypearial->glyph->bitmap.buffer, freetypearial->glyph->bitmap.width, freetypearial->glyph->bitmap.rows, freetypearial->glyph->bitmap.pitch, QImage::Format_Grayscale8).convertToFormat(QImage::Format_RGBA8888);
    for (int y = 0; y < character.height(); ++y) // Fix the alpha mask to match the RGB
    {
      QRgb* line = reinterpret_cast<QRgb*>(character.scanLine(y));
      for (int x = 0; x < character.width(); ++x)
      {
        line[x] = qRgba(qRed(line[x]) * red, qGreen(line[x]) * green, qBlue(line[x]) * blue, std::min(255, qRed(line[x]) + qGreen(line[x]) + qBlue(line[x])));

      }
    }
    painter.drawImage(QRectF(x + freetypearial->glyph->bitmap_left, y + (maxfontheight - freetypearial->glyph->bitmap_top), freetypearial->glyph->bitmap.width, freetypearial->glyph->bitmap.rows), character);
    x += freetypearial->glyph->advance.x >> 6;
  }
  x += PLAYBACK_TEXT_BORDER; // End border
  return texturebuffer.copy(0, 0, x, texturebuffer.height()); // Crop the image so the right side aligns with the rightmost text
}

///// Methods /////


TIMELINEGENERATOR::TIMELINEGENERATOR(const uint64_t topsecincrement,
                                     const uint64_t topdayincrement,
                                     const uint64_t topmonthincrement,
                                     const uint64_t topyearincrement,
                                     const uint64_t botsecincrement,
                                     const uint64_t botdayincrement,
                                     const uint64_t botmonthincrement,
                                     QString(*toptext)(const QDateTime&),
                                     QString(*bottext)(const QDateTime&),
                                     const QDateTime& startdatetime) :
  topsecincrement_(topsecincrement),
  topdayincrement_(topdayincrement),
  topmonthincrement_(topmonthincrement),
  topyearincrement_(topyearincrement),
  botsecincrement_(botsecincrement),
  botdayincrement_(botdayincrement),
  botmonthincrement_(botmonthincrement),
  toptext_(toptext),
  bottext_(bottext),
  startdatetime_(startdatetime)
{

}

TIMELINETEXT::TIMELINETEXT() :
  position_(0, 0),
  width_(0)
{

}

PLAYBACKTEXT::PLAYBACKTEXT() :
  time_(0),
  texture_(0),
  texturebuffer_(QOpenGLBuffer::VertexBuffer),
  vertexbuffer_(QOpenGLBuffer::VertexBuffer)
{

}

PlaybackWidget::PlaybackWidget(QWidget* parent) :
  QOpenGLWidget(parent),
  actionvideo_(new QAction(tr("Video"), this)),
  actionmetadata_(new QAction(tr("Metadata"), this)),
  openglmajorversion_(0),
  recordingsblocksshader_(nullptr),
  recordingsblockspositionlocation_(-1),
  recordingsblockscolourlocation_(-1),
  markershader_(nullptr),
  markerpositionlocation_(-1),
  markercolourlocation_(-1),
  textshader_(nullptr),
  textpositionlocation_(-1),
  texttexturecoordlocation_(-1),
  texttexturesamplerlocation_(-1),
  globalstarttime_(0),
  globalendtime_(0),
  state_(PLAYBACKMOUSESTATE_IDLE),
  clickedpos_(0),
  timer_(-1)
{
  actionvideo_->setCheckable(true);
  actionvideo_->setChecked(true);
  actionmetadata_->setCheckable(true);
  actionmetadata_->setChecked(true);

  connect(&MainWindow::Instance()->GetDeviceMgr(), &DeviceMgr::TimeOffsetChanged, this, &PlaybackWidget::TimeOffsetChanged);
  connect(&MainWindow::Instance()->GetVideoWidgetsMgr(), &VideoWidgetsMgr::ViewDestroyed, this, &PlaybackWidget::ViewDestroyed);
  connect(&MainWindow::Instance()->GetVideoWidgetsMgr(), &VideoWidgetsMgr::Selected, this, &PlaybackWidget::Selected);
  connect(actionvideo_, &QAction::triggered, this, &PlaybackWidget::ShowVideo);
  connect(actionmetadata_, &QAction::triggered, this, &PlaybackWidget::ShowMetadata);

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

  // Sort out initial start and end times
  const auto now = std::chrono::system_clock::now();
  globalendtime_ = std::chrono::duration_cast<std::chrono::milliseconds>((now).time_since_epoch()).count();
  globalstarttime_ = std::chrono::duration_cast<std::chrono::milliseconds>((now - std::chrono::hours(24)).time_since_epoch()).count();
}

PlaybackWidget::~PlaybackWidget()
{
  Destroy();

}

void PlaybackWidget::Destroy()
{
  makeCurrent();

  recordingsblocks_.clear();
  if (recordingsblocksshader_)
  {
    delete recordingsblocksshader_;
    recordingsblocksshader_ = nullptr;
  }

  if (markershader_)
  {
    delete markershader_;
    markershader_ = nullptr;
  }

  if (textshader_)
  {
    delete textshader_;
    textshader_ = nullptr;
  }

  if (starttext_.texture_)
  {
    glDeleteTextures(1, &starttext_.texture_);
    starttext_.texture_ = 0;
  }

  if (endtext_.texture_)
  {
    glDeleteTextures(1, &endtext_.texture_);
    endtext_.texture_ = 0;
  }

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

void PlaybackWidget::UpdateRecordingsBlocks()
{
  UpdateGUITimelines();

  const float pixelheight = 2.0f / static_cast<float>(height());
  const float pixelwidth = 2.0f / static_cast<float>(width());
  
  const float recordingsblockstop = GetRecordingBlocksTop();
  for (size_t i = 0; i < recordingsblocks_.size(); ++i)
  {
    const float top = recordingsblockstop - ((i * pixelheight * RECORDINGBLOCKS_HEIGHT) + pixelheight);
    const float bottom = top - (pixelheight * (RECORDINGBLOCKS_HEIGHT - 1));
    recordingsblocks_.at(i)->Update(top, bottom, top - pixelheight, bottom + pixelheight, pixelwidth, globalstarttime_, globalendtime_);
  }

  // Quick export start and end markers
  const float exporttextheight = pixelheight * static_cast<float>(17);
  const float exporttextwidth = pixelwidth * static_cast<float>(150);
  if (exportstarttime_.is_initialized())
  {
    exportstartvertices_.bind();
    const float position = (static_cast<float>(*exportstarttime_ - globalstarttime_) / static_cast<float>(globalendtime_ - globalstarttime_) * 2.0f) - 1.0f;
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
    const float position = (static_cast<float>(*exportendtime_ - globalstarttime_) / static_cast<float>(globalendtime_ - globalstarttime_) * 2.0f) - 1.0f;
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

void PlaybackWidget::initializeGL()
{
  initializeOpenGLFunctions();

  glGetIntegerv(GL_MAJOR_VERSION, &openglmajorversion_);
  if (openglmajorversion_ < 3)
  {
    LOG_GUI_WARNING(QString("OpenGL version not supported"));
    return;
  }

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  const QColor backgroundcolor = palette().color(QPalette::Window);
  glClearColor(backgroundcolor.redF(), backgroundcolor.greenF(), backgroundcolor.blueF(), 0.0f);

  if (recordingsblocksshader_)
  {
    delete recordingsblocksshader_;
    recordingsblocksshader_ = nullptr;
  }

  if (markershader_)
  {
    delete markershader_;
    markershader_ = nullptr;
  }

  if (textshader_)
  {
    delete textshader_;
    textshader_ = nullptr;
  }

  // Recordings blocks shader
  recordingsblocksshader_ = new QOpenGLShaderProgram();
  if (!recordingsblocksshader_->addShaderFromSourceCode(QOpenGLShader::Vertex,
    "#version 130\n"
    "in vec2 position;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(position, 1.0, 1.0);\n"
    "}\n"))
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }
  if (!recordingsblocksshader_->addShaderFromSourceCode(QOpenGLShader::Fragment,
    "#version 130\n"
    "uniform vec4 colour;\n"
    "void main()\n"
    "{\n"
    "  gl_FragColor = colour;\n"
    "}\n"))
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  recordingsblocksshader_->bindAttributeLocation("position", 0);
  if (!recordingsblocksshader_->link())
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::link failed"));
    return;
  }
  recordingsblockspositionlocation_ = recordingsblocksshader_->attributeLocation("position");
  recordingsblockscolourlocation_ = recordingsblocksshader_->uniformLocation("colour");

  // Play marker shader
  markershader_ = new QOpenGLShaderProgram();
  if (!markershader_->addShaderFromSourceCode(QOpenGLShader::Vertex,
    "#version 130\n"
    "in vec2 position;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(position, 1.0, 1.0);\n"
    "}\n"))
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!markershader_->addShaderFromSourceCode(QOpenGLShader::Fragment,
    "#version 130\n"
    "uniform vec4 colour;\n"
    "void main()\n"
    "{\n"
    "  gl_FragColor = colour;\n"
    "}\n"))
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  markershader_->bindAttributeLocation("position", 0);
  if (!markershader_->link())
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::bindAttributeLocation failed"));
    return;
  }
  markerpositionlocation_ = markershader_->attributeLocation("position");
  markercolourlocation_ = markershader_->uniformLocation("colour");

  // Text shader
  textshader_ = new QOpenGLShaderProgram();
  if (!textshader_->addShaderFromSourceCode(QOpenGLShader::Vertex,
    "#version 130\n"
    "in vec2 texcoord;\n"
    "in vec3 position;\n"
    "out vec2 out_texcoord;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(position, 1.0);\n"
    "  out_texcoord = texcoord\n;"
    "}\n"))
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!textshader_->addShaderFromSourceCode(QOpenGLShader::Fragment,
    "#version 130\n"
    "in vec2 out_texcoord;\n"
    "uniform sampler2D sampler;\n"
    "out vec4 colour;\n"
    "void main()\n"
    "{\n"
    "  colour = texture(sampler, out_texcoord);\n"
    "}\n"))
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  textshader_->bindAttributeLocation("position", 0);
  textshader_->bindAttributeLocation("texcoord", 1);

  if (!textshader_->link())
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::link failed"));
    return;
  }

  texttexturecoordlocation_ = textshader_->attributeLocation("texcoord");
  if (texttexturecoordlocation_ == -1)
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  textpositionlocation_ = textshader_->attributeLocation("position");
  if (textpositionlocation_ == -1)
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  texttexturesamplerlocation_ = textshader_->uniformLocation("sampler");
  if (texttexturesamplerlocation_ == -1)
  {
    LOG_GUI_WARNING(tr("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  // GUI
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

  update();

  if (timer_ == -1)
  {
    timer_ = startTimer(100);

  }
}

void PlaybackWidget::resizeGL(int width, int height)
{
  prevstartendtime_ = std::make_pair(0, 0);
  UpdateGUIHorizontalLines();
  UpdateGUITimelines();
  UpdateRecordingsBlocks();
}

void PlaybackWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // static GUI elements
  markershader_->bind();
  guihorizontalvertices_.bind();
  markershader_->enableAttributeArray(markerpositionlocation_);
  markershader_->setAttributeBuffer(markerpositionlocation_, GL_FLOAT, 0, 2);
  markershader_->setUniformValue(markercolourlocation_, QVector4D(0.4f, 0.4f, 0.4f, 1.0f));
  glDrawArrays(GL_LINES, 0, static_cast<GLsizei>((recordingsblocks_.size() * 2) + 2));
  guihorizontalvertices_.release();

  if (guitimelinelightvertices_.size())
  {
    guitimelinelightvertices_.bind();
    markershader_->enableAttributeArray(markerpositionlocation_);
    markershader_->setAttributeBuffer(markerpositionlocation_, GL_FLOAT, 0, 2);
    markershader_->setUniformValue(markercolourlocation_, QVector4D(0.4f, 0.4f, 0.4f, 1.0f));
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(guitimelinelightvertices_.size() * 2));
    guitimelinelightvertices_.release();
  }

  if (guitimelinedarkvertices_.size())
  {
    guitimelinedarkvertices_.bind();
    markershader_->enableAttributeArray(markerpositionlocation_);
    markershader_->setAttributeBuffer(markerpositionlocation_, GL_FLOAT, 0, 2);
    markershader_->setUniformValue(markercolourlocation_, QVector4D(0.15f, 0.15f, 0.15f, 1.0f));
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(guitimelinedarkvertices_.size() * 2));
    guitimelinedarkvertices_.release();
  }
  markershader_->release();

  if (guitimelinetexts_.size())
  {
    QPainter painter(this);
    glEnable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    textshader_->bind();
    for (TIMELINETEXT& guitimelinetext : guitimelinetexts_)
    {
      painter.drawStaticText(guitimelinetext.position_, guitimelinetext.text_);

    }
    textshader_->release();
    glDisable(GL_BLEND);
  }
  
  // Recording blocks
  recordingsblocksshader_->bind();
  if (actionvideo_->isChecked())
  {
    for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_) // Video
    {
      recordingblocks->GetRecordingBlockVertices().bind();
      recordingsblocksshader_->enableAttributeArray(recordingsblockspositionlocation_);
      recordingsblocksshader_->setAttributeBuffer(recordingsblockspositionlocation_, GL_FLOAT, 0, 2);
      recordingsblocksshader_->setUniformValue(recordingsblockscolourlocation_, recordingblocks->GetView()->GetSelectedColour());
      glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(recordingblocks->GetNumRecordingBlockTriangles()));
      recordingblocks->GetRecordingBlockVertices().release();
    }
  }

  if (actionmetadata_->isChecked())
  {
    for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_) // Metadata blocks
    {
      recordingblocks->GetMetadataRecordingBlockVertices().bind();
      recordingsblocksshader_->enableAttributeArray(recordingsblockspositionlocation_);
      recordingsblocksshader_->setAttributeBuffer(recordingsblockspositionlocation_, GL_FLOAT, 0, 2);
      recordingsblocksshader_->setUniformValue(recordingsblockscolourlocation_, recordingblocks->GetView()->GetSelectedColour() * 2);
      glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(recordingblocks->GetNumMetadataRecordingBlockTriangles()));
      recordingblocks->GetMetadataRecordingBlockVertices().release();
    }
  }
  recordingsblocksshader_->release();

  // Play markers
  markershader_->bind();
  for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_)
  {
    if (recordingblocks->GetView()->GetFrameTime() == std::chrono::steady_clock::time_point())
    {

      continue;
    }

    recordingblocks->GetPlayMarkerVertices().bind();
    markershader_->enableAttributeArray(markerpositionlocation_);
    markershader_->setAttributeBuffer(markerpositionlocation_, GL_FLOAT, 0, 2);
    markershader_->setUniformValue(markercolourlocation_, QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
    glDrawArrays(GL_LINE_STRIP, 0, 2);
    recordingblocks->GetPlayMarkerVertices().release();
  }

  // Export markers
  if (recordingsblocks_.size())
  {
    if (exportstarttime_.is_initialized())
    {
      // Marker
      exportstartvertices_.bind();
      markershader_->enableAttributeArray(markerpositionlocation_);
      markershader_->setAttributeBuffer(markerpositionlocation_, GL_FLOAT, 0, 2);
      markershader_->setUniformValue(markercolourlocation_, QVector4D(0.0f, 1.0f, 0.0f, 1.0f));
      glDrawArrays(GL_LINE_STRIP, 0, 2);
      exportstartvertices_.release();
    }

    if (exportendtime_.is_initialized())
    {
      exportendvertices_.bind();
      markershader_->enableAttributeArray(markerpositionlocation_);
      markershader_->setAttributeBuffer(markerpositionlocation_, GL_FLOAT, 0, 2);
      markershader_->setUniformValue(markercolourlocation_, QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
      glDrawArrays(GL_LINE_STRIP, 0, 2);
      exportendvertices_.release();
    }
  }
  markershader_->release();

  // Export markers text
  glEnable(GL_BLEND);
  glActiveTexture(GL_TEXTURE0);
  textshader_->bind();
  if (recordingsblocks_.size())
  {
    if (exportstarttime_.is_initialized())
    {
      textshader_->setUniformValue(texttexturesamplerlocation_, 0);
      glBindTexture(GL_TEXTURE_2D, starttext_.texture_);

      starttext_.texturebuffer_.bind();
      textshader_->enableAttributeArray(texttexturecoordlocation_);
      textshader_->setAttributeBuffer(texttexturecoordlocation_, GL_FLOAT, 0, 2);
      starttext_.vertexbuffer_.bind();
      textshader_->enableAttributeArray(textpositionlocation_);
      textshader_->setAttributeBuffer(textpositionlocation_, GL_FLOAT, 0, 3);

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      textshader_->disableAttributeArray(textpositionlocation_);
      starttext_.vertexbuffer_.release();
      textshader_->disableAttributeArray(texttexturecoordlocation_);
      starttext_.texturebuffer_.release();
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    if (exportendtime_.is_initialized())
    {
      textshader_->setUniformValue(texttexturesamplerlocation_, 0);
      glBindTexture(GL_TEXTURE_2D, endtext_.texture_);

      endtext_.texturebuffer_.bind();
      textshader_->enableAttributeArray(texttexturecoordlocation_);
      textshader_->setAttributeBuffer(texttexturecoordlocation_, GL_FLOAT, 0, 2);
      endtext_.vertexbuffer_.bind();
      textshader_->enableAttributeArray(textpositionlocation_);
      textshader_->setAttributeBuffer(textpositionlocation_, GL_FLOAT, 0, 3);

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      textshader_->disableAttributeArray(textpositionlocation_);
      endtext_.vertexbuffer_.release();
      textshader_->disableAttributeArray(texttexturecoordlocation_);
      endtext_.texturebuffer_.release();
      glBindTexture(GL_TEXTURE_2D, 0);
    }
  }
  textshader_->release();
  glDisable(GL_BLEND);
}

void PlaybackWidget::timerEvent(QTimerEvent*)
{
  if (state_ == PLAYBACKMOUSESTATE_MOVEMARKER)
  {
    const int x = mapFromGlobal(QCursor::pos()).x();
    if ((clickedpos_ != x) && (std::chrono::steady_clock::now() - clickedtime_) > SCRUB_DELAY) // If we are scrubbing, because we block the client from spamming the server, we need to check the mouse position here
    {
      Scrub(std::make_pair(globalstarttime_, globalendtime_), x);
      clickedpos_ = x;
      clickedtime_ = std::chrono::steady_clock::now();
    }
  }

  if (recordingsblocks_.empty())
  {

    return;
  }
  makeCurrent();
  UpdateRecordingsBlocks();
  doneCurrent();
}

void PlaybackWidget::mouseMoveEvent(QMouseEvent* event)
{
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

    const uint64_t newtime = static_cast<uint64_t>(((static_cast<double>(event->pos().x()) / static_cast<double>(width())) * (globalendtime_ - globalstarttime_)) + globalstarttime_);
    makeCurrent();
    SetExportStartTime(std::min(newtime, exportendtime_.is_initialized() ? *exportendtime_ : std::numeric_limits<uint64_t>::max()));
    UpdateRecordingsBlocks();
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

    const uint64_t newtime = static_cast<uint64_t>(((static_cast<double>(event->pos().x()) / static_cast<double>(width())) * (globalendtime_ - globalstarttime_)) + globalstarttime_);
    makeCurrent();
    SetExportEndTime(std::max(newtime, exportstarttime_.is_initialized() ? *exportstarttime_ : std::numeric_limits<uint64_t>::max())); // Cap at the start time
    UpdateRecordingsBlocks();
    doneCurrent();
  }
  else if (state_ == PLAYBACKMOUSESTATE_CLICKED)
  {
    SetState(PLAYBACKMOUSESTATE_MOVE);

  }
  else if (state_ == PLAYBACKMOUSESTATE_MOVE)
  {
    const uint64_t diff = globalendtime_ - globalstarttime_;
    const uint64_t timediff = (static_cast<double>(clickedpos_ - event->x()) / static_cast<double>(width())) * diff;

    if (static_cast<int64_t>(globalendtime_ + timediff) < START_EPOCH)
    {
      globalstarttime_ = START_EPOCH;
      globalendtime_ = START_EPOCH + diff;
    }
    else if (static_cast<int64_t>(globalendtime_ + timediff) > END_EPOCH)
    {
      globalstarttime_ = END_EPOCH - diff;
      globalendtime_ = END_EPOCH;
    }
    else
    {
      globalstarttime_ += timediff;
      globalendtime_ += timediff;
      clickedpos_ = event->pos().x();
      clickedtime_ = std::chrono::steady_clock::now();
    }

    makeCurrent();
    UpdateRecordingsBlocks();
    doneCurrent();
  }
  else if (state_ == PLAYBACKMOUSESTATE_MOVEMARKER)
  {
    if (clickedpos_ == event->pos().x())
    {

      return;
    }

    if ((std::chrono::steady_clock::now() - clickedtime_) < SCRUB_DELAY) // Don't spam the server...
    {

      return;
    }

    Scrub(std::make_pair(globalstarttime_, globalendtime_), event->pos().x());

    clickedpos_ = event->pos().x();
    clickedtime_ = std::chrono::steady_clock::now();
  }
  else
  {
    // Set the cursor when it goes over the save times
    if ((exportstarttime_.is_initialized() ? Hit(*exportstarttime_, 0, height(), event->pos()) : false) || (exportendtime_.is_initialized() ? Hit(*exportendtime_, 0, height(), event->pos()) : false))
    {
      setCursor(Qt::SizeHorCursor);

    }
    else if (HitPlayMarker(event->pos()))
    {
      setCursor(Qt::SizeHorCursor);

    }
    else
    {
      unsetCursor();

    }
  }
}

void PlaybackWidget::mousePressEvent(QMouseEvent* event)
{
  if (event->pos().x() < (width() / 2)) // Prioritise grabbing the end marker
  {
    if (exportendtime_.is_initialized())
    {
      if (Hit(*exportendtime_, 0, height(), event->pos()))
      {
        SetState(PLAYBACKMOUSESTATE_MOVEEND);
        return;
      }
    }

    if (exportstarttime_.is_initialized())
    {
      if (Hit(*exportstarttime_, 0, height(), event->pos()))
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
      if (Hit(*exportstarttime_, 0, height(), event->pos()))
      {
        SetState(PLAYBACKMOUSESTATE_MOVESTART);
        return;
      }
    }

    if (exportendtime_.is_initialized())
    {
      if (Hit(*exportendtime_, 0, height(), event->pos()))
      {
        SetState(PLAYBACKMOUSESTATE_MOVEEND);
        return;
      }
    }
  }

  if (HitPlayMarker(event->pos()))
  {
    SetState(PLAYBACKMOUSESTATE_MOVEMARKER);

    for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_)
    {
      recordingblocks->GetView()->GetNextPlayRequestIndex(true);
      recordingblocks->GetView()->Pause(boost::none);
    }

    return;
  }

  SetState(PLAYBACKMOUSESTATE_CLICKED);
  clickedpos_ = event->pos().x();
  clickedtime_ = std::chrono::steady_clock::now();
}

void PlaybackWidget::mouseReleaseEvent(QMouseEvent* event)
{
  if (state_ == PLAYBACKMOUSESTATE_CLICKED) // View the currently selected time
  {
    makeCurrent();
    if (event->button() == Qt::LeftButton)
    {
      if (event->modifiers() & Qt::SHIFT)
      {
        const uint64_t time = globalstarttime_ + (static_cast<double>(event->pos().x()) / static_cast<double>(width()) * static_cast<double>(globalendtime_ - globalstarttime_));
        if (!exportstarttime_.is_initialized())
        {
          SetExportStartTime(time);

        }
        else if (exportstarttime_.is_initialized() && !exportendtime_.is_initialized() && (time < *exportstarttime_))
        {
          SetExportStartTime(time);

        }
        else if (!exportendtime_.is_initialized())
        {
          SetExportEndTime(time);

        }
        else
        {
          exportstarttime_.reset();
          exportendtime_.reset();
        }
        UpdateRecordingsBlocks();
      }
      else
      {
        const uint64_t time = globalstarttime_ + ((static_cast<double>(event->pos().x()) / static_cast<double>(width())) * static_cast<double>(globalendtime_ - globalstarttime_));
        for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_)
        {
          if (recordingblocks->GetView()->IsPaused())
          {
            recordingblocks->GetView()->Play(time, 1);
            recordingblocks->GetView()->SetPlayMarkerTime(time + recordingblocks->GetView()->GetTimeOffset());
            recordingblocks->GetView()->SetFrameTime(std::chrono::steady_clock::now());
          }
          else
          {
            const boost::optional<uint64_t> endtime = recordingblocks->GetEndTime();
            if (endtime.is_initialized() && (time >= *endtime))
            {
              // User has selected a time beyond its latest time, so just go live
              if (recordingblocks->GetView()->GetViewType() == VIEWTYPE_MEDIA)
              {
                const boost::optional<uint64_t> starttime = recordingblocks->GetStartTime();
                if (starttime.is_initialized())
                {
                  recordingblocks->GetView()->SetPlayMarkerTime(*starttime);

                }
              }
              else if (recordingblocks->GetView()->GetViewType() == VIEWTYPE_MONOCLE)
              {
                recordingblocks->GetView()->SetPlayMarkerTime(*endtime + recordingblocks->GetView()->GetTimeOffset());

              }
              recordingblocks->GetView()->SetFrameTime(std::chrono::steady_clock::now());
              recordingblocks->GetView()->Stop();
            }
            else
            {
              recordingblocks->GetView()->Play(time, boost::none);
              recordingblocks->GetView()->SetPlayMarkerTime(time + recordingblocks->GetView()->GetTimeOffset());
              recordingblocks->GetView()->SetFrameTime(std::chrono::steady_clock::now());
            }
          }
        }
      }
    }
    doneCurrent();
  }
  else if (state_ == PLAYBACKMOUSESTATE_MOVEMARKER)
  {
    Scrub(std::make_pair(globalstarttime_, globalendtime_), mapFromGlobal(QCursor::pos()).x());

  }

  SetState(PLAYBACKMOUSESTATE_IDLE);
}

void PlaybackWidget::wheelEvent(QWheelEvent* event)
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

void PlaybackWidget::contextMenuEvent(QContextMenuEvent* event)
{
  QMenu* menu = new QMenu(MainWindow::Instance());
  menu->addAction(actionvideo_);
  menu->addAction(actionmetadata_);
  menu->exec(event->globalPos());
}

bool PlaybackWidget::Hit(const uint64_t time, const int top, const int bottom, const QPoint& pos) const
{
  if ((pos.y() < top) || (pos.y() > bottom))
  {

    return false;
  }

  const int timex = (static_cast<double>(time - globalstarttime_) / static_cast<double>(globalendtime_ - globalstarttime_)) * width();
  return ((pos.x() == timex) || ((pos.x() - 1) == timex) || ((pos.x() + 1) == timex) || ((pos.x() - 2) == timex) || ((pos.x() + 2) == timex)); // Give it a bit of leeway...
}

bool PlaybackWidget::HitPlayMarker(const QPoint& pos) const
{
  for (const QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_)
  {
    const int top = height() * (1.0f - ((recordingblocks->GetTop() + 1.0f) * 0.5f));
    const int bottom = height() * (1.0f - ((recordingblocks->GetBottom() + 1.0f) * 0.5f));
    if (Hit(recordingblocks->GetPlayMarkerTime(), top, bottom, pos))
    {

      return true;
    }
  }
  return false;
}

void PlaybackWidget::SetState(const PLAYBACKMOUSESTATE state)
{
  if (state == PLAYBACKMOUSESTATE_MOVEMARKER)
  {
    setCursor(Qt::SizeHorCursor);

  }
  else if ((state == PLAYBACKMOUSESTATE_IDLE) || state == (PLAYBACKMOUSESTATE_CLICKED))
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

void PlaybackWidget::SetExportStartTime(const uint64_t exportstarttime)
{
  exportstarttime_ = exportstarttime;
  const QImage text = GetTextureTime(freetypearial_, exportstarttime, 0.0f, 1.0f, 0.0f);

  glBindTexture(GL_TEXTURE_2D, starttext_.texture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text.width(), text.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, text.bits());
  glBindTexture(GL_TEXTURE_2D, 0);
}

void PlaybackWidget::SetExportEndTime(const uint64_t exportendtime)
{
  exportendtime_ = exportendtime;
  const QImage text = GetTextureTime(freetypearial_, exportendtime, 0.0f, 0.0f, 1.0f);

  glBindTexture(GL_TEXTURE_2D, endtext_.texture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text.width(), text.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, text.bits());
  glBindTexture(GL_TEXTURE_2D, 0);
}

void PlaybackWidget::UpdateGUIHorizontalLines()
{
  const float pixelheight = 2.0f / static_cast<float>(height());
  std::vector<float> guihorizontalvertices;
  guihorizontalvertices.reserve((recordingsblocks_.size() + 1) * 4);
  const float recordingsblockstop = GetRecordingBlocksTop();
  for (size_t i = 0; i <= recordingsblocks_.size(); ++i)
  {
    const float height = recordingsblockstop - (i * pixelheight * RECORDINGBLOCKS_HEIGHT);
    guihorizontalvertices.push_back(-1.0f);
    guihorizontalvertices.push_back(height);
    guihorizontalvertices.push_back(1.0f);
    guihorizontalvertices.push_back(height);
  }
  guihorizontalvertices_.bind();
  guihorizontalvertices_.allocate(guihorizontalvertices.data(), static_cast<int>(guihorizontalvertices.size() * sizeof(float)));
  guihorizontalvertices_.release();
}

void PlaybackWidget::UpdateGUITimelines()
{
  if ((prevstartendtime_.first == globalstarttime_) && (prevstartendtime_.second == globalendtime_)) // Nothing has changed, just continue on
  {

    return;
  }
  prevstartendtime_.first = globalstarttime_;
  prevstartendtime_.first = globalendtime_;

  guitimelinedarkmarkers_.clear();
  guitimelinelightmarkers_.clear();
  const uint64_t diff = globalendtime_ - globalstarttime_;
  const float pixelheight = 2.0f / static_cast<float>(height());
  const float pixelwidth = 2.0f / static_cast<float>(width());
  int guitimelinetextsindex = 0;

  const TIMELINEGENERATOR timelinegenerator = TimelineGenerator(globalstarttime_, globalendtime_);
  
  QTextOption textoption(Qt::AlignHCenter | Qt::AlignVCenter);
  textoption.setWrapMode(QTextOption::WrapMode::NoWrap);

  // Top
  QDateTime toptime = timelinegenerator.startdatetime_;
  const QDateTime endtime = QDateTime::fromMSecsSinceEpoch(globalendtime_, Qt::UTC);
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
    const float position = -1.0 + (((static_cast<double>(toptime.toMSecsSinceEpoch()) - static_cast<double>(globalstarttime_)) / static_cast<double>(diff)) * 2.0);
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
  float top = (((static_cast<double>(height()) - (maxheight + 1.0)) / static_cast<double>(height())) * 2.0) - 1.0;
  guitimelinedarkmarkers_.push_back(-1.0f);
  guitimelinedarkmarkers_.push_back(top);
  guitimelinedarkmarkers_.push_back(1.0f);
  guitimelinedarkmarkers_.push_back(top);

  // Bottom
  if (timelinegenerator.bottext_)
  {
    QDateTime bottime = timelinegenerator.startdatetime_;
    top -= pixelheight;
    while (bottime <= endtime)
    {
      QDateTime nexttime = bottime;
      nexttime = nexttime.addSecs(timelinegenerator.botsecincrement_);
      nexttime = nexttime.addDays(timelinegenerator.botdayincrement_);
      nexttime = nexttime.addMonths(timelinegenerator.botmonthincrement_);
      const uint64_t step = nexttime.toMSecsSinceEpoch() - bottime.toMSecsSinceEpoch();

      // Lines
      const float position = -1.0 + (((static_cast<double>(bottime.toMSecsSinceEpoch()) - static_cast<double>(globalstarttime_)) / static_cast<double>(diff)) * 2.0);
      guitimelinelightmarkers_.push_back(position);
      guitimelinelightmarkers_.push_back(-1.0f);
      guitimelinelightmarkers_.push_back(position);
      guitimelinelightmarkers_.push_back(top);

      // Text
      const int pixelx = ((position + 1.0f) / 2.0f) * width();
      const int pixely = height() - ((top + 1.0f) / 2.0f) * height();
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
}

void PlaybackWidget::ZoomIn(const int x)
{
  const int64_t diff = static_cast<int64_t>(globalendtime_) - static_cast<int64_t>(globalstarttime_);
  const int64_t time = static_cast<int64_t>(globalstarttime_) + (diff * static_cast<double>(x) / static_cast<double>(width()));
  if (diff < (40 * 1000)) // Don't zoom in too far(40 seconds)
  {

    return;
  }

  const uint64_t newstarttime = static_cast<int64_t>(time - ((time - static_cast<int64_t>(globalstarttime_)) / 1.25));
  const uint64_t newendtime = static_cast<int64_t>(time + ((static_cast<int64_t>(globalendtime_) - static_cast<int64_t>(time)) / 1.25));
  globalstarttime_ = newstarttime;
  globalendtime_ = newendtime;
  makeCurrent();
  UpdateRecordingsBlocks();
  doneCurrent();
}

void PlaybackWidget::ZoomOut(const int x)
{
  const double diff = static_cast<double>(globalendtime_) - static_cast<double>(globalstarttime_);
  const double time = static_cast<double>(globalstarttime_) + (diff * static_cast<double>(x) / static_cast<double>(width()));
  const uint64_t newstarttime = std::max(START_EPOCH, static_cast<qint64>(static_cast<double>(globalstarttime_) - ((time - static_cast<double>(globalstarttime_)) / diff) * 0.25 * diff));
  const int64_t endepoch = std::min(END_EPOCH, QDateTime::fromMSecsSinceEpoch(newstarttime, Qt::UTC).addYears(20).toMSecsSinceEpoch()); // Don't let the user expand beyond 20 years or the END_EPOCH year
  const uint64_t newendtime = std::min(endepoch, static_cast<int64_t>(static_cast<double>(globalendtime_) + (((static_cast<double>(globalendtime_) - time) / diff) * 0.25 * diff)));
  globalstarttime_ = newstarttime;
  globalendtime_ = newendtime;
  makeCurrent();
  UpdateRecordingsBlocks();
  doneCurrent();
}

float PlaybackWidget::GetRecordingBlocksTop() const
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

void PlaybackWidget::Scrub(const std::pair<uint64_t, uint64_t>& startendtime, const int x)
{
  for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_)
  {
    const uint64_t time = startendtime.first + ((static_cast<double>(x) / static_cast<double>(width())) * static_cast<double>(startendtime.second - startendtime.first));
    recordingblocks->GetView()->Scrub(time);
    recordingblocks->GetView()->SetPlayMarkerTime(time + recordingblocks->GetView()->GetTimeOffset());
    recordingblocks->GetView()->SetFrameTime(std::chrono::steady_clock::now());
  }
}

void PlaybackWidget::on_buttonframestepbackwards_clicked()
{
  for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_)
  {
    if (recordingblocks->GetView()->IsPaused())
    {
      recordingblocks->GetView()->FrameStep(false);

    }
    else
    {
      recordingblocks->GetView()->Pause(recordingblocks->GetView()->GetTime());

    }
  }
}

void PlaybackWidget::on_buttonplay_clicked()
{
  for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_)
  {
    recordingblocks->GetView()->Play(recordingblocks->GetView()->GetTime() - recordingblocks->GetView()->GetTimeOffset(), boost::none);
    recordingblocks->GetView()->SetPlayMarkerTime(recordingblocks->GetView()->GetTime());
    recordingblocks->GetView()->SetFrameTime(std::chrono::steady_clock::now());
  }
}

void PlaybackWidget::on_buttonpause_clicked()
{
  for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_)
  {
    if (recordingblocks->GetView()->GetViewType() == VIEWTYPE_MEDIA)
    {
      recordingblocks->GetView()->Pause(recordingblocks->GetView()->GetSequenceNum());

    }
    else
    {
      recordingblocks->GetView()->Pause(recordingblocks->GetView()->GetTime());

    }
  }
}

void PlaybackWidget::on_buttonstop_clicked()
{
  for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_)
  {
    if (recordingblocks->GetView()->GetViewType() == VIEWTYPE_MAP)
    {

      continue;
    }
    else if (recordingblocks->GetView()->GetViewType() == VIEWTYPE_MEDIA)
    {
      const boost::optional<uint64_t> starttime = recordingblocks->GetStartTime();
      if (starttime.is_initialized())
      {
        recordingblocks->GetView()->SetPlayMarkerTime(*starttime);

      }
    }
    else if (recordingblocks->GetView()->GetViewType() == VIEWTYPE_MONOCLE)
    {
      const boost::optional<uint64_t> endtime = recordingblocks->GetEndTime();
      if (endtime.is_initialized())
      {
        recordingblocks->GetView()->SetPlayMarkerTime(*endtime);

      }
    }
    recordingblocks->GetView()->SetFrameTime(std::chrono::steady_clock::now());
    recordingblocks->GetView()->Stop();
  }
  makeCurrent();
  UpdateRecordingsBlocks();
  doneCurrent();
}

void PlaybackWidget::on_buttonframestepforwards_clicked()
{
  for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_)
  {
    if (recordingblocks->GetView()->IsPaused())
    {
      recordingblocks->GetView()->FrameStep(true);

    }
    else
    {
      recordingblocks->GetView()->Pause(recordingblocks->GetView()->GetTime());

    }
  }
}

void PlaybackWidget::on_buttonzoomin_clicked()
{
  ZoomIn(width() / 2);
  makeCurrent();
  UpdateRecordingsBlocks();
  doneCurrent();
}

void PlaybackWidget::on_buttonzoomout_clicked()
{
  ZoomOut(width() / 2);
  makeCurrent();
  UpdateRecordingsBlocks();
  doneCurrent();
}

void PlaybackWidget::on_buttonexport_clicked()
{
  std::vector< QSharedPointer<client::Recording> > recordings;
  recordings.reserve(recordingsblocks_.size());
  for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_)
  {
    if (recordingblocks->GetView()->GetViewType() == VIEWTYPE_MONOCLE)
    {
      QSharedPointer<VideoView> videoview = qSharedPointerCast<VideoView>(recordingblocks->GetView());
      if (std::find_if(recordings.cbegin(), recordings.cend(), [videoview](const QSharedPointer<client::Recording>& recording) { return (recording == videoview->GetRecording()); }) != recordings.cend()) // No duplicates
      {

        continue;
      }
      recordings.push_back(videoview->GetRecording());
    }
  }
  ExportWindow(MainWindow::Instance(), recordings, exportstarttime_, exportendtime_).exec();
}

void PlaybackWidget::ViewDestroyed(const QSharedPointer<View>& view) // We just listen to all videoviews being destroyed because it is more reliable and easy to implement(may well be faster too)
{
  recordingsblocks_.erase(std::remove_if(recordingsblocks_.begin(), recordingsblocks_.end(), [view](const QSharedPointer<RecordingBlocks>& recordingblocks) { return (recordingblocks->GetView() == view); }), recordingsblocks_.end());
  update();
}

void PlaybackWidget::Selected(const QSharedPointer<View>& view, bool select)
{
  if ((view->GetViewType() != VIEWTYPE_MEDIA) && (view->GetViewType() != VIEWTYPE_MONOCLE)) // Ignore other guff
  {

    return;
  }

  makeCurrent();
  if (select)
  {
    // Ignore duplicates
    std::vector< QSharedPointer<RecordingBlocks> >::iterator i = std::find_if(recordingsblocks_.begin(), recordingsblocks_.end(), [view](const QSharedPointer<RecordingBlocks>& recordingblocks) { return (recordingblocks->GetView() == view); });
    if (i == recordingsblocks_.end())
    {
      recordingsblocks_.push_back(QSharedPointer<RecordingBlocks>::create(this, view));
      UpdateRecordingsBlocks();
    }
  }
  else
  {
    std::vector< QSharedPointer<RecordingBlocks> >::iterator i = std::find_if(recordingsblocks_.begin(), recordingsblocks_.end(), [view](const QSharedPointer<RecordingBlocks>& recordingblocks) { return (recordingblocks->GetView() == view); });
    if (i != recordingsblocks_.end())
    {
      recordingsblocks_.erase(i);
      UpdateRecordingsBlocks();
    }
  }
  UpdateGUIHorizontalLines();
  doneCurrent();

  update(); // Schedule redraw
}

void PlaybackWidget::ShowVideo(bool)
{

}

void PlaybackWidget::ShowMetadata(bool)
{

}

void PlaybackWidget::TimeOffsetChanged(const boost::shared_ptr<Device>& device)
{
  // Reinitialise the recording blocks
  makeCurrent();
  for (QSharedPointer<RecordingBlocks>& recordingblocks : recordingsblocks_)
  {
    recordingblocks->Init();

  }
  doneCurrent();
}

}
