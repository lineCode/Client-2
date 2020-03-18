// videowidget.cpp
//

///// Includes /////

#include "monocleclient/videowidget.h"

#include <cuda.h>
#include <cudaGL.h>
#include <GL/gl.h>
#include <QApplication>
#include <QMenu>
#include <QMimeData>
#include <QPainter>
#include <thread>

#ifdef _WIN32
  #include <GL/glu.h>
  #include <GLES3/gl3.h>
#endif

#include "monocleclient/mainwindow.h"
#include "monocleclient/mapview.h"
#include "monocleclient/mediaview.h"
#include "monocleclient/recording.h"
#include "monocleclient/shaders.h"
#include "monocleclient/videoview.h"

///// Globals /////

const int RESIZE_BORDER = 10;

///// Namespaces /////

namespace client
{

///// Globals /////

const std::string RGB2LAB =
"vec3 rgb2lab(vec3 rgb)\n"
"{\n"
    "float r = (rgb.r > 0.04045) ? pow((rgb.r + 0.055) / 1.055, 2.4) : rgb.r / 12.92;\n"
    "float g = (rgb.g > 0.04045) ? pow((rgb.g + 0.055) / 1.055, 2.4) : rgb.g / 12.92;\n"
    "float b = (rgb.b > 0.04045) ? pow((rgb.b + 0.055) / 1.055, 2.4) : rgb.b / 12.92;\n"
    "float x = (r * 0.4124 + g * 0.3576 + b * 0.1805) / 0.95047;\n"
    "float y = (r * 0.2126 + g * 0.7152 + b * 0.0722) / 1.00000;\n"
    "float z = (r * 0.0193 + g * 0.1192 + b * 0.9505) / 1.08883;\n"
    "x = (x > 0.008856) ? pow(x, 0.333333) : (7.787 * x) + (16.0 / 116.0);\n"
    "y = (y > 0.008856) ? pow(y, 0.333333) : (7.787 * y) + (16.0 / 116.0);\n"
    "z = (z > 0.008856) ? pow(z, 0.333333) : (7.787 * z) + (16.0 / 116.0);\n"
    "return vec3((0.45490 * y) - 0.06274, 1.96078 * (x - y), 0.78431 * (y - z));\n"
"}";

const std::string DELTAE =
"float deltae(vec3 lhs, vec3 rhs)\n"
"{\n"
  "float deltaL = lhs[0] - rhs[0];\n"
  "float deltaA = lhs[1] - rhs[1];\n"
  "float deltaB = lhs[2] - rhs[2];\n"
  "float c1 = sqrt(lhs[1] * lhs[1] + lhs[2] * lhs[2]);\n"
  "float c2 = sqrt(rhs[1] * rhs[1] + rhs[2] * rhs[2]);\n"
  "float deltaC = c1 - c2;\n"
  "float deltaH = (deltaA * deltaA) + (deltaB * deltaB) - (deltaC * deltaC);\n"
  "deltaH = (deltaH < 0.0) ? 0.0 : sqrt(deltaH);\n"
  "float sc = 1.0 + (0.045 * c1);\n"
  "float sh = 1.0 + (0.015 * c1);\n"
  "float deltaLKlsl = deltaL / (1.0);\n"
  "float deltaCkcsc = deltaC / (sc);\n"
  "float deltaHkhsh = deltaH / (sh);\n"
  "float i = (deltaLKlsl * deltaLKlsl) + (deltaCkcsc * deltaCkcsc) + (deltaHkhsh * deltaHkhsh);\n"
  "return ((i < 0.0) ? 0.0 : sqrt(i));\n"
"}\n";

const std::array<QVector4D, 30> OBJECT_COLOURS =
{
  QVector4D(1.000f, 0.078f, 0.576f, 1.0f),
  QVector4D(0.729f, 0.996f, 0.011f, 1.0f),
  QVector4D(0.831f, 0.727f, 0.019f, 1.0f),
  QVector4D(0.968f, 0.972f, 0.031f, 1.0f),
  QVector4D(0.933f, 0.894f, 0.007f, 1.0f),
  QVector4D(0.715f, 0.764f, 0.443f, 1.0f),
  QVector4D(0.776f, 0.717f, 0.007f, 1.0f),
  QVector4D(0.968f, 0.211f, 0.015f, 1.0f),
  QVector4D(0.015f, 0.941f, 0.015f, 1.0f),
  QVector4D(0.094f, 0.494f, 0.039f, 1.0f),
  QVector4D(0.286f, 0.027f, 0.047f, 1.0f),
  QVector4D(0.780f, 0.913f, 0.039f, 1.0f),
  QVector4D(0.941f, 0.988f, 0.058f, 1.0f),
  QVector4D(0.321f, 0.364f, 0.427f, 1.0f),
  QVector4D(0.239f, 0.901f, 0.015f, 1.0f),
  QVector4D(0.803f, 0.082f, 0.415f, 1.0f),
  QVector4D(0.603f, 0.549f, 0.843f, 1.0f),
  QVector4D(0.596f, 0.988f, 0.556f, 1.0f),
  QVector4D(0.247f, 0.968f, 0.823f, 1.0f),
  QVector4D(0.454f, 0.388f, 0.472f, 1.0f),
  QVector4D(0.168f, 0.262f, 0.586f, 1.0f),
  QVector4D(0.403f, 0.345f, 0.870f, 1.0f),
  QVector4D(0.309f, 0.176f, 0.984f, 1.0f),
  QVector4D(0.913f, 0.188f, 0.023f, 1.0f),
  QVector4D(0.474f, 0.301f, 0.776f, 1.0f),
  QVector4D(0.323f, 0.372f, 0.419f, 1.0f),
  QVector4D(0.301f, 0.321f, 0.356f, 1.0f),
  QVector4D(0.949f, 0.274f, 0.149f, 1.0f),
  QVector4D(0.996f, 0.027f, 0.615f, 1.0f),
  QVector4D(0.972f, 0.403f, 0.023f, 1.0f)
};

///// Functions /////

void ToBandwidthString(size_t bandwidth, std::vector<char>& buffer)
{
  if (bandwidth <= 999)
  {
    utility::ToString999(static_cast<int>(bandwidth), buffer);
    buffer.push_back('B');
    buffer.push_back('/');
    buffer.push_back('s');
  }
  else
  {
    size_t bw = bandwidth / 1000;
    if (bw <= 999)
    {
      const size_t fraction = bandwidth % 1000;
      utility::ToString999(static_cast<int>(bw), buffer);
      buffer.push_back('.');
      utility::ToString999Padding(static_cast<int>(fraction), buffer);
      buffer.push_back('K');
      buffer.push_back('B');
      buffer.push_back('/');
      buffer.push_back('s');
    }
    else
    {
      bandwidth = bw;
      bw = bandwidth / 1000;
      if (bw <= 999)
      {
        const size_t fraction = bandwidth % 1000;
        utility::ToString999(static_cast<int>(bw), buffer);
        buffer.push_back('.');
        utility::ToString999Padding(static_cast<int>(fraction), buffer);
        buffer.push_back('M');
        buffer.push_back('B');
        buffer.push_back('/');
        buffer.push_back('s');
      }
      else
      {
        bandwidth = bw;
        bw = bandwidth / 1000;
        if (bw <= 999)
        {
          const size_t fraction = bandwidth % 1000;
          utility::ToString999(static_cast<int>(bw), buffer);
          buffer.push_back('.');
          utility::ToString999Padding(static_cast<int>(fraction), buffer);
          buffer.push_back('G');
          buffer.push_back('B');
          buffer.push_back('/');
          buffer.push_back('s');
        }
        // Ignore Terabytes/s because it makes no sense anyway!
      }
    }
  }
}

size_t GetBandwidth(const boost::circular_buffer< std::pair<std::chrono::steady_clock::time_point, size_t> >& bandwidthsizes)
{
  if (bandwidthsizes.empty())
  {

    return 0;
  }

  size_t totaldata = 0;
  const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  const long long duration = std::max(100ll, std::min(5000ll, static_cast<long long>(std::chrono::duration_cast<std::chrono::milliseconds>(now - bandwidthsizes.front().first).count())));
  for (const std::pair<std::chrono::steady_clock::time_point, size_t>& bandwidthsize : bandwidthsizes)
  {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - bandwidthsize.first).count() > duration)
    {

      continue;
    }
    totaldata += bandwidthsize.second;
  }
  return (totaldata * 1000) / duration;
}

void ToInfoText(const QDateTime& datetime, const std::string& format, const monocle::Codec codec, const boost::circular_buffer< std::pair<std::chrono::steady_clock::time_point, size_t> >& bandwidthsizes, const std::pair<const std::string&, const QString&>& location, const std::pair<const std::string&, const QString&>& recordingname, const int width, const int height, std::vector<char>& buffer)
{
  const QDate date = datetime.date();
  const QTime time = datetime.time();
  buffer.clear();
  for (size_t i = 0; i < format.size(); ++i)
  {
    if (format[i] == '%')
    {
      if ((i + 1) >= format.size())
      {

        break;
      }
      ++i;
      switch (format[i])
      {
        case '%':
        {
          buffer.push_back('%');
          break;
        }
        case 'a':
        {
          MainWindow::Instance()->ShortWeekDayName(date.dayOfWeek() - 1, buffer);
          break;
        }
        case 'b':
        {
          MainWindow::Instance()->ShortMonthName(date.month() - 1, buffer);
          break;
        }
        case 'd':
        {
          // We do it like this to avoid any allocations
          utility::ToString60Padding(date.day(), buffer);
          break;
        }
        case 'f':
        {
          utility::ToString999Padding(time.msec(), buffer);
          break;
        }
        case 'H':
        {
          utility::ToString60Padding(time.hour(), buffer);
          break;
        }
        case 'k':
        {
          const char* codecname = monocle::EnumNameCodec(codec);
          size_t i = 0;
          while (codecname[i] != '\0')
          {
            buffer.push_back(codecname[i++]);
            
          }
          break;
        }
        case 'M':
        {
          utility::ToString60Padding(time.minute(), buffer);
          break;
        }
        case 'q':
        {
          ToBandwidthString(GetBandwidth(bandwidthsizes), buffer);
          break;
        }
        case 'S':
        {
          utility::ToString60Padding(time.second(), buffer);
          break;
        }
        case 'T': // H:M:S
        {
          utility::ToString60Padding(time.hour(), buffer);
          buffer.push_back(':');
          utility::ToString60Padding(time.minute(), buffer);
          buffer.push_back(':');
          utility::ToString60Padding(time.second(), buffer);
          break;
        }
        case 'Y':
        {
          utility::ToString4999Padding(date.year(), buffer);
          break;
        }
        case 'L':
        {
          if (location.first.size())
          {
            buffer.insert(buffer.end(), location.first.cbegin(), location.first.cend());

          }
          else
          {
            for (const QChar& ch : location.second)
            {
              buffer.push_back(ch.toLatin1());

            }
          }
          break;
        }
        case 'R':
        {
          if (recordingname.first.size())
          {
            buffer.insert(buffer.end(), recordingname.first.cbegin(), recordingname.first.cend());

          }
          else
          {
            for (const QChar& ch : recordingname.second)
            {
              buffer.push_back(ch.toLatin1());

            }
          }
          break;
        }
        case 'w':
        {
          utility::ToString4999(width, buffer);
          break;
        }
        case 'h':
        {
          utility::ToString4999(height, buffer);
          break;
        }
        default:
        {
          // Just ignore it
          break;
        }
      }
    }
    else
    {
      buffer.push_back(format[i]);

    }
  }
}

QImage InfoTexture(const std::vector<char>& text, const FT_Face font)
{
  QImage infotexture(INFO_WIDTH, INFO_HEIGHT, QImage::Format_RGBA8888);
  infotexture.fill(QColor(0, 0, 0));
  QPainter painter(&infotexture);
  int x = INFO_BORDER;
  const int maxfontheight = static_cast<int>(INFO_FONT_HEIGHT * (static_cast<float>(font->bbox.yMax) / static_cast<float>(font->bbox.yMax - font->bbox.yMin)));
  const int y = INFO_BORDER + (INFO_FONT_HEIGHT - maxfontheight) - 1;
  for (int i = 0; i < text.size(); ++i)
  {
    if (text.at(i) == '\0')
    {

      break;
    }

    if (FT_Load_Char(font, text.at(i), FT_LOAD_RENDER))
    {

      continue; // Ignore errors
    }

    const QImage character = QImage(font->glyph->bitmap.buffer, font->glyph->bitmap.width, font->glyph->bitmap.rows, font->glyph->bitmap.pitch, QImage::Format_Grayscale8);
    painter.drawImage(QRectF(x + font->glyph->bitmap_left, y + (maxfontheight - font->glyph->bitmap_top), font->glyph->bitmap.width, font->glyph->bitmap.rows), character);
    x += font->glyph->advance.x >> 6;
  }
  x += INFO_BORDER; // End border

  // Set the alpha to chop off the erroneous end and fade the middle
  for (int i = 0; i < infotexture.height(); ++i)
  {
    QRgb* line = reinterpret_cast<QRgb*>(infotexture.scanLine(i));
    for (int j = 0; j < infotexture.width(); ++j)
    {
      int alpha = 0;
      if (j <= x)
      {
        alpha = 122;

      }
      line[j] = qRgba(qRed(line[j]), qGreen(line[j]), qBlue(line[j]), alpha);
    }
  }
  return infotexture;
}

///// Methods /////

VideoWidget::VideoWidget(QWidget* parent) :
  QOpenGLWidget(parent),
  timer_(-1),
  freetype_(nullptr),
  freetypearial_(nullptr),
  openglmajorversion_(0),
  rgbpositionlocation_(-1),
  rgbtexturecoordlocation_(-1),
  rgbtexturesamplerlocation_(-1),
  rgbcolourpickerlocation_(-1),
  yuvtexturecoordlocation_(-1),
  yuvpositionlocation_(-1),
  yuvcolourpickerlocation_(-1),
  yuvtexturesamplerlocation_({ -1, -1, -1 }),
  nv12texturecoordlocation_(-1),
  nv12positionlocation_(-1),
  nv12colourpickerlocation_(-1),
  nv12texturesamplerlocation_({ -1, -1 }),
  width_(0),
  height_(0),
  showtoolbar_(new QAction(tr("Show Toolbar"), this)),
  hidetoolbar_(new QAction(tr("Hide Toolbar"), this)),
  resizedirectionstate_(RESIZEDIRECTION::NONE),
  selectedpositionlocation_(-1),
  selectedcolourlocation_(-1),
  infopositionlocation_(-1),
  infotexturecoordlocation_(-1),
  infotexturesamplerlocation_(-1),
  digitalsigngreenpadlock_(QOpenGLTexture::Target2D),
  digitalsignredpadlock_(QOpenGLTexture::Target2D),
  digitalsignpositionlocation_(-1),
  digitalsigntexturecoordlocation_(-1),
  digitalsigntexturesamplerlocation_(-1)
{
  connect(showtoolbar_, &QAction::triggered, this, &VideoWidget::ShowToolbar);
  connect(hidetoolbar_, &QAction::triggered, this, &VideoWidget::HideToolbar);
  connect(&MainWindow::Instance()->GetMediaMgr(), &MediaMgr::MediaRemoved, this, &VideoWidget::MediaRemoved);
  
  setMouseTracking(true);

  if ((MainWindow::Instance()->GetMouseState() == MOUSESTATE_FINDMOTION) || (MainWindow::Instance()->GetMouseState() == MOUSESTATE_COLOURPICKER) || (MainWindow::Instance()->GetMouseState() == MOUSESTATE_FINDOBJECT))
  {
    parentWidget()->setCursor(Qt::CrossCursor);

  }

  MainWindow::Instance()->GetVideoWidgetsMgr().VideoWidgetCreated(this);
}

VideoWidget::~VideoWidget()
{
  MainWindow::Instance()->GetVideoWidgetsMgr().VideoWidgetDestroyed(this);
  Destroy();
}

void VideoWidget::Init(const QResource* arial, const QIcon& showfullscreen, const unsigned int width, const unsigned int height, const bool showtoolbar)
{
  // Freetype
  if (FT_Init_FreeType(&freetype_))
  {
    LOG_GUI_WARNING(tr("Error initialising FreeType"));
    return;
  }

  if (FT_New_Memory_Face(freetype_, arial->data(), arial->size(), 0, &freetypearial_))
  {
    LOG_GUI_WARNING(tr("Error loading Arial resource"));
    return;
  }

  if (FT_Set_Pixel_Sizes(freetypearial_, INFO_FONT_HEIGHT, 0))
  {
    LOG_GUI_WARNING(tr("Error initialising Arial resource"));
    return;
  }

  arial_ = arial;
  showfullscreen_ = showfullscreen;
  width_ = width;
  height_ = height;

  if (showtoolbar)
  {
    GetToolbar()->show();

  }
  else
  {
    GetToolbar()->hide();

  }
  
  connect(GetToolbar()->GetToolButtonAddRow(), &QToolButton::clicked, this, &VideoWidget::AddRow);
  connect(GetToolbar()->GetToolButtonRemoveRow(), &QToolButton::clicked, this, &VideoWidget::RemoveRow);
  connect(GetToolbar()->GetToolButtonAddColumn(), &QToolButton::clicked, this, &VideoWidget::AddColumn);
  connect(GetToolbar()->GetToolButtonRemoveColumn(), &QToolButton::clicked, this, &VideoWidget::RemoveColumn);
  connect(GetToolbar()->GetToolButtonShowFullscreen(), &QToolButton::clicked, this, &VideoWidget::Fullscreen);
  connect(&MainWindow::Instance()->GetDeviceMgr(), &DeviceMgr::RecordingRemoved, this, &VideoWidget::RecordingRemoved);
}

void VideoWidget::Destroy()
{
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

  makeCurrent();
  digitalsigngreenpadlock_.destroy();
  digitalsignredpadlock_.destroy();
  views_.clear();
  resizeview_.clear();
  doneCurrent();
}

QSharedPointer<MapView> VideoWidget::CreateMapView(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool stretch, const boost::shared_ptr<Device>& device, const QSharedPointer<Map>& map)
{
  if (openglmajorversion_ < 3)
  {

    return QSharedPointer<MapView>();
  }

  // Check the location is not already taken by another video view
  if (!IsEmpty(QSharedPointer<MediaView>(), x, y, width, height))
  {

    return QSharedPointer<MapView>();
  }

  QSharedPointer<MapView> mapview = QSharedPointer<MapView>::create(this, MainWindow::Instance()->GetNextCUDAContext(), MainWindow::Instance()->GetRandomHSVColour(), x, y, width, height, ROTATION::_0, Options::Instance().GetStretchVideo(), device, map, arial_);
  connect(mapview->GetActionClose(), &QAction::triggered, mapview.data(), [this, mapview = mapview.toWeakRef()](bool)
  {
    QSharedPointer<MapView> m = mapview.lock();
    if (!m)
    {

      return;
    }

    disconnect(m->GetActionClose(), &QAction::triggered, m.data(), nullptr);
    m->GetVideoWidget()->RemoveView(m);
    emit MainWindow::Instance()->GetVideoWidgetsMgr().ViewDestroyed(m);
  });
  views_.push_back(mapview);
  return mapview;
}

QSharedPointer<MediaView> VideoWidget::CreateMediaView(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool stretch, const QSharedPointer<Media>& media, const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex)
{
  if (openglmajorversion_ < 3)
  {

    return QSharedPointer<MediaView>();
  }

  // Check the location is not already taken by another video view
  if (!IsEmpty(QSharedPointer<MediaView>(), x, y, width, height))
  {

    return QSharedPointer<MediaView>();
  }

  QSharedPointer<MediaView> mediaview = QSharedPointer<MediaView>::create(this, MainWindow::Instance()->GetNextCUDAContext(), MainWindow::Instance()->GetRandomHSVColour(), x, y, width, height, ROTATION::_0, false, Options::Instance().GetStretchVideo(), Options::Instance().GetShowInfo(), Options::Instance().GetShowObjects(), media, deviceindex, recordingindex, trackindex, arial_);
  connect(mediaview->GetActionClose(), &QAction::triggered, mediaview.data(), [this, mediaview = mediaview.toWeakRef()](bool)
  {
    QSharedPointer<MediaView> m = mediaview.lock();
    if (!m)
    {

      return;
    }

    disconnect(m->GetActionClose(), &QAction::triggered, m.data(), nullptr);
    m->GetVideoWidget()->RemoveView(m);
    emit MainWindow::Instance()->GetVideoWidgetsMgr().ViewDestroyed(m);
  });
  views_.push_back(mediaview);
  return mediaview;
}

QSharedPointer<VideoView> VideoWidget::CreateVideoView(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool stretch, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingTrack>& track)
{
  if (openglmajorversion_ < 3)
  {

    return QSharedPointer<VideoView>();
  }

  // Check the location is not already taken by another video view
  if (!IsEmpty(QSharedPointer<VideoView>(), x, y, width, height))
  {

    return QSharedPointer<VideoView>();
  }

  const QSharedPointer<VideoView> videoview = QSharedPointer<VideoView>::create(this, MainWindow::Instance()->GetNextCUDAContext(), MainWindow::Instance()->GetRandomHSVColour(), x, y, width, height, ROTATION::_0, false, Options::Instance().GetStretchVideo(), Options::Instance().GetShowInfo(), Options::Instance().GetShowObjects(), device, recording, track, arial_);
  connect(videoview->GetActionClose(), &QAction::triggered, videoview.data(), [this, videoview = videoview.toWeakRef()](bool)
  {
    QSharedPointer<VideoView> v = videoview.lock();
    if (!v)
    {

      return;
    }

    disconnect(v->GetActionClose(), &QAction::triggered, v.data(), nullptr);
    v->GetVideoWidget()->RemoveView(v);
    emit MainWindow::Instance()->GetVideoWidgetsMgr().ViewDestroyed(v);
  });
  views_.push_back(videoview);
  return videoview;
}

bool VideoWidget::RemoveView(const QSharedPointer<View>& view)
{
  auto i = std::find(views_.begin(), views_.end(), view);
  if (i == views_.end())
  {

    return false;
  }
  emit MainWindow::Instance()->GetVideoWidgetsMgr().ViewDestroyed(*i);
  views_.erase(i);
  ResetGrid();
  return true;
}

void VideoWidget::VideoWindowMove(const int32_t x, const int32_t y)
{
  if (parent()->parent()->metaObject() == &VideoWindow::staticMetaObject)
  {
    VideoWindow* videowindow = static_cast<VideoWindow*>(parent()->parent());
    return videowindow->move(x, y);
  }
  else if (parent()->parent()->metaObject() == &MainWindow::staticMetaObject)
  {
    MainWindow* mainwindow = static_cast<MainWindow*>(parent()->parent());
    return mainwindow->move(x, y);
  }
}

void VideoWidget::VideoWindowResize(const int32_t width, const int32_t height)
{
  if (parent()->parent()->metaObject() == &VideoWindow::staticMetaObject)
  {
    VideoWindow* videowindow = static_cast<VideoWindow*>(parent()->parent());
    return videowindow->resize(width, height);
  }
  else if (parent()->parent()->metaObject() == &MainWindow::staticMetaObject)
  {
    MainWindow* mainwindow = static_cast<MainWindow*>(parent()->parent());
    return mainwindow->resize(width, height);
  }
}

void VideoWidget::SetWindowState(const Qt::WindowState windowstate)
{
  if (parent()->parent()->metaObject() == &VideoWindow::staticMetaObject)
  {
    VideoWindow* videowindow = static_cast<VideoWindow*>(parent()->parent());
    videowindow->setWindowState(windowstate);
  }
  else if (parent()->parent()->metaObject() == &MainWindow::staticMetaObject)
  {
    MainWindow* mainwindow = static_cast<MainWindow*>(parent()->parent());
    mainwindow->setWindowState(windowstate);
  }
}

int VideoWidget::GetVideoWindowX() const
{
  if (parent()->parent()->metaObject() == &VideoWindow::staticMetaObject)
  {
    VideoWindow* videowindow = static_cast<VideoWindow*>(parent()->parent());
    return videowindow->x();
  }
  else if (parent()->parent()->metaObject() == &MainWindow::staticMetaObject)
  {
    MainWindow* mainwindow = static_cast<MainWindow*>(parent()->parent());
    return mainwindow->x();
  }

  return 0;
}

int VideoWidget::GetVideoWindowY() const
{
  if (parent()->parent()->metaObject() == &VideoWindow::staticMetaObject)
  {
    VideoWindow* videowindow = static_cast<VideoWindow*>(parent()->parent());
    return videowindow->y();
  }
  else if (parent()->parent()->metaObject() == &MainWindow::staticMetaObject)
  {
    MainWindow* mainwindow = static_cast<MainWindow*>(parent()->parent());
    return mainwindow->y();
  }

  return 0;
}

int VideoWidget::GetVideoWindowWidth() const
{
  if (parent()->parent()->metaObject() == &VideoWindow::staticMetaObject)
  {
    VideoWindow* videowindow = static_cast<VideoWindow*>(parent()->parent());
    return videowindow->width();
  }
  else if (parent()->parent()->metaObject() == &MainWindow::staticMetaObject)
  {
    MainWindow* mainwindow = static_cast<MainWindow*>(parent()->parent());
    return mainwindow->width();
  }

  return 0;
}

int VideoWidget::GetVideoWindowHeight() const
{
  if (parent()->parent()->metaObject() == &VideoWindow::staticMetaObject)
  {
    VideoWindow* videowindow = static_cast<VideoWindow*>(parent()->parent());
    return videowindow->height();
  }
  else if (parent()->parent()->metaObject() == &MainWindow::staticMetaObject)
  {
    MainWindow* mainwindow = static_cast<MainWindow*>(parent()->parent());
    return mainwindow->height();
  }

  return 0;
}

VideoWidgetToolbar* VideoWidget::GetToolbar()
{
  return parent()->findChild<VideoWidgetToolbar*>(QString("widgettoolbar"), Qt::FindDirectChildrenOnly);
}

QPoint VideoWidget::GetLocation(unsigned int x, unsigned int y)
{
  return QPoint((static_cast<float>(x) / (static_cast<float>(width()))) * width_, (static_cast<float>(y) / (static_cast<float>(height()))) * height_);
}

bool VideoWidget::IsEmpty(const QSharedPointer<View>& ignoreview, unsigned int x, unsigned int y, unsigned int width, unsigned int height) const
{
  for (unsigned int i = x; i < (x + width); ++i)
  {
    for (unsigned int j = y; j < (y + height); ++j)
    {
      const auto v = GetView(i, j);
      if (v && (v != ignoreview))
      {

        return false;
      }
    }
  }
  return true;
}

QSharedPointer<View> VideoWidget::GetView(const QPoint& pos)
{
  for (const auto& view : views_)
  {
    if (view->GetPixelRect().contains(pos))
    {

      return view;
    }
  }

  return QSharedPointer<View>();
}

QSharedPointer<View> VideoWidget::GetView(const QPoint& pos) const
{
  for (const auto& view : views_)
  {
    if (view->GetPixelRect().contains(pos))
    {

      return view;
    }
  }

  return QSharedPointer<View>();
}

QSharedPointer<View> VideoWidget::GetView(const unsigned int x, const unsigned int y)
{
  for (auto view : views_)
  {
    if (view->GetRect().contains(x, y))
    {

      return view;
    }
  }

  return QSharedPointer<View>();
}

QSharedPointer<View> VideoWidget::GetView(const unsigned int x, const unsigned int y) const
{
  for (auto view : views_)
  {
    if (view->GetRect().contains(x, y))
    {

      return view;
    }
  }

  return QSharedPointer<View>();
}

std::vector< QSharedPointer<View> > VideoWidget::GetViews(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height) const
{
  std::vector< QSharedPointer<View> > views;
  for (unsigned int i = x; i < (x + width); ++i)
  {
    for (unsigned int j = y; j < (y + height); ++j)
    {
      const QSharedPointer<View> v = GetView(i, j);
      if (v && !utility::Contains(views, v))
      {
        views.push_back(v);

      }
    }
  }
  return views;
}

bool VideoWidget::AllSelected() const
{
  for (const auto& view : views_)
  {
    if (!view->GetSelected())
    {
    
      return false;
    }
  }
  return true;
}

void VideoWidget::SetSelectedAll(const bool selected)
{
  for (auto& view : views_)
  {
    if (view->GetSelected() != selected)
    {
      view->SetSelected(selected);
      MainWindow::Instance()->GetVideoWidgetsMgr().Selected(view, selected);
    }
  }
}

void VideoWidget::ResetGrid()
{
  makeCurrent();
  grid_.Generate(width_, height_, views_);
  doneCurrent();
}

void VideoWidget::SetUpdateFrequency(const unsigned int frequency)
{
  if (timer_ != -1)
  {
    killTimer(timer_);

  }
  timer_ = startTimer(static_cast<int>(frequency));
}

void VideoWidget::SetGridSize(const unsigned int width, const unsigned int height)
{
  if (openglmajorversion_ < 3)
  {

    return;
  }

  // Make sure no video views are in the way
  for (unsigned int x = width; x < width_; ++x)
  {
    for (unsigned int y = height; y < height_; ++y)
    {
      QSharedPointer<View> view = GetView(x, y);
      if (view)
      {
        RemoveView(view);

      }
    }
  }

  makeCurrent();
  width_ = width;
  height_ = height;
  grid_.Generate(width, height, views_);

  for (QSharedPointer<View>& videoview : views_)
  {
    videoview->ResetPosition(true);

  }

  doneCurrent();
}

void VideoWidget::dragEnterEvent(QDragEnterEvent* event)
{
  const QMimeData* mimedata = event->mimeData();
  if (mimedata == nullptr)
  {

    return;
  }

  const QByteArray recordingtoken = mimedata->data(MIME_DEVICE_TREE_RECORDING_TOKEN);
  if (recordingtoken.isEmpty())
  {

    return;
  }

  event->accept();
}

void VideoWidget::dragMoveEvent(QDragMoveEvent* event)
{
  const QMimeData* mimedata = event->mimeData();
  if (mimedata == nullptr)
  {

    return;
  }

  const QByteArray recordingtoken = mimedata->data(MIME_DEVICE_TREE_RECORDING_TOKEN);
  if (recordingtoken.isEmpty())
  {

    return;
  }

  event->accept();
}

void VideoWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
  event->accept();

}

void VideoWidget::dropEvent(QDropEvent* event)
{
  const QMimeData* mimedata = event->mimeData();
  if (mimedata == nullptr)
  {

    return;
  }

  // Device
  QByteArray bytearray = mimedata->data(MIME_DEVICE_TREE_DEVICE_IDENTIFIER);
  if (bytearray.size() != sizeof(uint64_t))
  {
    
    return;
  }
  
  const boost::shared_ptr<Device> device = MainWindow::Instance()->GetDeviceMgr().GetDevice(*reinterpret_cast<const uint64_t*>(bytearray.data()));
  if (device == nullptr)
  {
    // Not a lot to do here except leave
    return;
  }

  // Recording
  bytearray = mimedata->data(MIME_DEVICE_TREE_RECORDING_TOKEN);
  if (bytearray.size() != sizeof(uint64_t))
  {

    return;
  }

  const QSharedPointer<Recording> recording = device->GetRecording(*reinterpret_cast<const uint64_t*>(bytearray.data()));
  if (recording == nullptr)
  {
    // Not a lot to do here except leave
    return;
  }

  // Track
  bytearray = mimedata->data(MIME_DEVICE_TREE_RECORDING_TRACK_ID);
  QSharedPointer<RecordingTrack> recordingtrack;
  if (bytearray.isEmpty())
  {
    std::vector< QSharedPointer<client::RecordingTrack> > tracks = recording->GetVideoTracks();
    if (tracks.empty())
    {
      // Just ignore this drag and drop because we have nothing to show
      return;
    }
    recordingtrack = tracks.front();
  }
  else if (bytearray.size() == sizeof(uint32_t))
  {
    recordingtrack = recording->GetTrack(*reinterpret_cast<const uint32_t*>(bytearray.data()));
    if (recordingtrack == nullptr)
    {
      // Not a lot to do here except leave
      return;
    }

    if (recordingtrack->GetTrackType() != monocle::TrackType::Video)
    {
      // Ignore track types that are not video...
      return;
    }
  }
  else
  {
    // Shouldn't happen, but ok...
    return;
  }

  const QSharedPointer<View> view = GetView(event->pos());
  if (view)
  {
    const QRect rect = view->GetRect();
    const int x = rect.x();
    const int y = rect.y();
    const int w = rect.width();
    const int h = rect.height();
    const bool stretch = view->GetStretch();
    RemoveView(view);
    CreateVideoView(x, y, w, h, stretch, device, recording, recordingtrack);
  }
  else
  {
    const QPoint pos = GetLocation(event->pos().x(), event->pos().y());
    CreateVideoView(pos.x(), pos.y(), 1, 1, Options::Instance().GetStretchVideo(), device, recording, recordingtrack);
  }

  event->accept();
}

void VideoWidget::enterEvent(QEvent* event)
{
  MainWindow::Instance()->GetVideoWidgetsMgr().MouseEnterEvent(event);

}

void VideoWidget::leaveEvent(QEvent* event)
{
  const QCursor curs = cursor();
  if ((resizedirectionstate_ != RESIZEDIRECTION::NONE) || (curs == Qt::SizeFDiagCursor) || (curs == Qt::SizeBDiagCursor) || (curs == Qt::SizeHorCursor) || (curs == Qt::SizeVerCursor))
  {
    unsetCursor();
    resizedirectionstate_ = RESIZEDIRECTION::NONE;
    resizeview_.clear();
  }

  MainWindow::Instance()->GetVideoWidgetsMgr().MouseLeaveEvent(event);
}

void VideoWidget::mouseMoveEvent(QMouseEvent* event)
{
  if (resizedirectionstate_ != RESIZEDIRECTION::NONE)
  {
    QSharedPointer<View> resizeview = resizeview_ ? resizeview_.lock() : nullptr;
    if (!resizeview) // Somehow the view has been deleted(possibly by server request) from under us, so lets just try to recover as best as we can
    {
      resizedirectionstate_ = RESIZEDIRECTION::NONE;
      return;
    }

    // Get the x/y co-ordinates the cursor is in with respect to the direction it is being resized
    int cursorx = resizeview->GetRect().x();
    int cursory = resizeview->GetRect().y();
    switch (resizedirectionstate_)
    {
      case RESIZEDIRECTION::NORTH_WEST:
      case RESIZEDIRECTION::NORTH_EAST:
      case RESIZEDIRECTION::SOUTH_WEST:
      case RESIZEDIRECTION::SOUTH_EAST:
      {
        cursorx = std::min(width_ - 1, (utility::Clip(event->pos().x(), 0, width()) * width_) / width());
        cursory = std::min(height_ - 1, (utility::Clip(event->pos().y(), 0, height()) * height_) / height());
        break;
      }
      case RESIZEDIRECTION::NORTH:
      case RESIZEDIRECTION::SOUTH:
      {
        cursory = std::min(height_ - 1, (utility::Clip(event->pos().y(), 0, height()) * height_) / height());
        break;
      }
      case RESIZEDIRECTION::EAST:
      case RESIZEDIRECTION::WEST:
      {
        cursorx = std::min(width_ - 1, (utility::Clip(event->pos().x(), 0, width()) * width_) / width());
        break;
      }
      default:
      {
        // Should be impossible, ignore
        resizedirectionstate_ = RESIZEDIRECTION::NONE;
        resizeview_.clear();
        return;
      }
    }

    // Get the pivot x/y
    int pivotx = 0;
    int pivoty = 0;
    if (resizedirectionstate_ == RESIZEDIRECTION::NORTH_WEST)
    {
      pivotx = resizeview->GetRect().right();
      pivoty = resizeview->GetRect().bottom();
    }
    else if (resizedirectionstate_ == RESIZEDIRECTION::NORTH_EAST)
    {
      pivotx = resizeview->GetRect().x();
      pivoty = resizeview->GetRect().bottom();
    }
    else if (resizedirectionstate_ == RESIZEDIRECTION::SOUTH_WEST)
    {
      pivotx = resizeview->GetRect().right();
      pivoty = resizeview->GetRect().y();
    }
    else if (resizedirectionstate_ == RESIZEDIRECTION::SOUTH_EAST)
    {
      pivotx = resizeview->GetRect().x();
      pivoty = resizeview->GetRect().y();
    }
    else if (resizedirectionstate_ == RESIZEDIRECTION::NORTH)
    {
      pivotx = resizeview->GetRect().x();
      pivoty = resizeview->GetRect().bottom();
    }
    else if (resizedirectionstate_ == RESIZEDIRECTION::EAST)
    {
      pivotx = resizeview->GetRect().x();
      pivoty = resizeview->GetRect().y();
    }
    else if (resizedirectionstate_ == RESIZEDIRECTION::SOUTH)
    {
      pivotx = resizeview->GetRect().x();
      pivoty = resizeview->GetRect().y();
    }
    else if (resizedirectionstate_ == RESIZEDIRECTION::WEST)
    {
      pivotx = resizeview->GetRect().right();
      pivoty = resizeview->GetRect().y();
    }
    else
    {
      // Should be impossible, ignore
      resizedirectionstate_ = RESIZEDIRECTION::NONE;
      resizeview_.clear();
      return;
    }

    // Cap the x co-ordinate depending on which direction the user is pulling
    if ((resizedirectionstate_ == RESIZEDIRECTION::NORTH_EAST) || (resizedirectionstate_ == RESIZEDIRECTION::EAST) || (resizedirectionstate_ == RESIZEDIRECTION::SOUTH_EAST))
    {
      cursorx = std::max(cursorx, pivotx);

    }
    else if ((resizedirectionstate_ == RESIZEDIRECTION::NORTH_WEST) || (resizedirectionstate_ == RESIZEDIRECTION::WEST) || (resizedirectionstate_ == RESIZEDIRECTION::SOUTH_WEST))
    {
      cursorx = std::min(cursorx, pivotx);

    }

    // Cap the y co-ordinate depending on which direction the user is pulling
    if ((resizedirectionstate_ == RESIZEDIRECTION::NORTH_EAST) || (resizedirectionstate_ == RESIZEDIRECTION::NORTH) || (resizedirectionstate_ == RESIZEDIRECTION::NORTH_WEST))
    {
      cursory = std::min(cursory, pivoty);

    }
    else if ((resizedirectionstate_ == RESIZEDIRECTION::SOUTH_EAST) || (resizedirectionstate_ == RESIZEDIRECTION::SOUTH) || (resizedirectionstate_ == RESIZEDIRECTION::SOUTH_WEST))
    {
      cursory = std::max(cursory, pivoty);

    }

    const int newx = std::min(pivotx, cursorx);
    const int newy = std::min(pivoty, cursory);

    // Fix the width and height if we are adjusting the opposite
    int newwidth = (std::max(pivotx, cursorx) - newx) + 1;
    int newheight = (std::max(pivoty, cursory) - newy) + 1;
    if ((resizedirectionstate_ == RESIZEDIRECTION::NORTH) || (resizedirectionstate_ == RESIZEDIRECTION::SOUTH))
    {
      newwidth = resizeview->GetRect().width();

    }
    else if ((resizedirectionstate_ == RESIZEDIRECTION::EAST) || (resizedirectionstate_ == RESIZEDIRECTION::WEST))
    {
      newheight = resizeview->GetRect().height();

    }

    if (resizeview->GetRect() == QRect(newx, newy, newwidth, newheight)) // If we aren't changing the size, don't go any further
    {

      return;
    }

    if (IsEmpty(resizeview, newx, newy, newwidth, newheight))
    {
      resizeview->SetPosition(this, newx, newy, newwidth, newheight, resizeview->GetRotation(), resizeview->GetMirror(), resizeview->GetStretch(), true);

      makeCurrent();
      grid_.Generate(width_, height_, views_);
      doneCurrent();
    }
  }
  else if (MainWindow::Instance()->GetMouseState() == MOUSESTATE_SELECT)
  {
    QSharedPointer<View> view = GetView(event->pos());
    if (!view)
    {
      unsetCursor();
      return;
    }

    // If we are close to a border, show the resizing icon
    const RESIZEDIRECTION resizedirection = GetResizeDirection(view, event->pos());
    if (resizedirection == RESIZEDIRECTION::NORTH_WEST)
    {
      setCursor(Qt::SizeFDiagCursor);

    }
    else if (resizedirection == RESIZEDIRECTION::NORTH_EAST)
    {
      setCursor(Qt::SizeBDiagCursor);

    }
    else if (resizedirection == RESIZEDIRECTION::SOUTH_WEST)
    {
      setCursor(Qt::SizeBDiagCursor);

    }
    else if (resizedirection == RESIZEDIRECTION::SOUTH_EAST)
    {
      setCursor(Qt::SizeFDiagCursor);

    }
    else if (resizedirection == RESIZEDIRECTION::EAST)
    {
      setCursor(Qt::SizeHorCursor);

    }
    else if (resizedirection == RESIZEDIRECTION::WEST)
    {
      setCursor(Qt::SizeHorCursor);

    }
    else if (resizedirection == RESIZEDIRECTION::NORTH)
    {
      setCursor(Qt::SizeVerCursor);

    }
    else if (resizedirection == RESIZEDIRECTION::SOUTH)
    {
      setCursor(Qt::SizeVerCursor);

    }
    else
    {
      unsetCursor();

    }

    MainWindow::Instance()->GetVideoWidgetsMgr().MouseMoveEvent(event);
  }
  else if (MainWindow::Instance()->GetMouseState() == MOUSESTATE_COLOURPICKER)
  {
    MainWindow::Instance()->GetVideoWidgetsMgr().MouseMoveEvent(event);

  }
  else
  {
    MainWindow::Instance()->GetVideoWidgetsMgr().MouseMoveEvent(event);

  }
}

void VideoWidget::mousePressEvent(QMouseEvent* event)
{
  QSharedPointer<View> view = GetView(event->pos());
  if (!view)
  {
    unsetCursor();
    return;
  }

  if (MainWindow::Instance()->GetMouseState() == MOUSESTATE_SELECT)
  {
    // If we are close to a border, lets begin resizing
    resizedirectionstate_ = GetResizeDirection(view, event->pos());
    if (resizedirectionstate_ != RESIZEDIRECTION::NONE)
    {
      resizeview_ = view;

    }
    else
    {
      MainWindow::Instance()->GetVideoWidgetsMgr().MousePressEvent(event);

    }
  }
  else
  {
    MainWindow::Instance()->GetVideoWidgetsMgr().MousePressEvent(event);

  }
}

void VideoWidget::mouseReleaseEvent(QMouseEvent* event)
{
  if (resizedirectionstate_ != RESIZEDIRECTION::NONE)
  {
    resizedirectionstate_ = RESIZEDIRECTION::NONE;
    resizeview_.clear();
    unsetCursor();
  }
  else
  {
    MainWindow::Instance()->GetVideoWidgetsMgr().MouseReleaseEvent(event);

  }
}

void VideoWidget::timerEvent(QTimerEvent* event)
{
  if (event->timerId() == timer_)
  {
    update();

  }
}

void VideoWidget::initializeGL()
{
  initializeOpenGLFunctions();
  
  glGetIntegerv(GL_MAJOR_VERSION, &openglmajorversion_);
  if (openglmajorversion_ < 3)
  {
    LOG_GUI_WARNING(QString("OpenGL version not supported"));
    GetToolbar()->Disable();
    return;
  }

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  if (grid_.Init())
  {
    LOG_GUI_WARNING(QString("VideoWidgetGrid::Init failed"));
    return;
  }

  grid_.Generate(width_, height_, views_);

  // RGB shader
  if (!viewrgbshader_.addShaderFromSourceCode(QOpenGLShader::Vertex,
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
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!viewrgbshader_.addShaderFromSourceCode(QOpenGLShader::Fragment,
    ("#version 130\n"
    + RGB2LAB + DELTAE +
    "in vec2 out_texcoord;\n"
    "uniform sampler2D sampler;\n"
    "uniform vec3 colourpicker;\n"
    "out vec4 colour;\n"
    "void main()\n"
    "{\n"
    "  vec4 rgb = texture(sampler, out_texcoord);\n"
    "  if ((colourpicker.r <= 1.0) && (deltae(rgb2lab(colourpicker), rgb2lab(rgb.rgb)) > 0.1))\n" // Quick easy hack to disable colour picking
    "  {\n"
    "    float grey = (0.21 * rgb.r) + (0.71 * rgb.g) + (0.07 * rgb.b);\n"
    "    colour = vec4(grey, grey, grey, 1.0);\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "    colour = vec4(rgb.rgb, 1.0);\n"
    "  }\n"
    "}\n").c_str()))
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  viewrgbshader_.bindAttributeLocation("position", 0);
  viewrgbshader_.bindAttributeLocation("texcoord", 1);

  if (!viewrgbshader_.link())
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::link failed"));
    return;
  }

  rgbtexturecoordlocation_ = viewrgbshader_.attributeLocation("texcoord");
  if (rgbtexturecoordlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  rgbpositionlocation_ = viewrgbshader_.attributeLocation("position");
  if (rgbpositionlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  rgbtexturesamplerlocation_ = viewrgbshader_.uniformLocation("sampler");
  if (rgbtexturesamplerlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  rgbcolourpickerlocation_ = viewrgbshader_.uniformLocation("colourpicker");
  if (rgbcolourpickerlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  // YUV shader
  if (!viewyuvshader_.addShaderFromSourceCode(QOpenGLShader::Vertex,
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
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!viewyuvshader_.addShaderFromSourceCode(QOpenGLShader::Fragment,
    ("#version 130\n"
    + RGB2LAB + DELTAE +
    "in vec2 out_texcoord;\n"
    "uniform sampler2D texy;\n"
    "uniform sampler2D texu;\n"
    "uniform sampler2D texv;\n"
    "uniform vec3 colourpicker;\n"
    "out vec4 colour;\n"
    "void main()\n"
    "{\n"
    "  float y = 1.1643 * (texture2D(texy, out_texcoord.st).r - 0.0625);\n"
    "  float u = texture2D(texu, out_texcoord.st).r - 0.5;\n"
    "  float v = texture2D(texv, out_texcoord.st).r - 0.5;\n"
    "  float r = y + 1.5958 * v;\n"
    "  float g = y - 0.39173 * u - 0.81290 * v;\n"
    "  float b = y + 2.017 * u;\n"
    "  if ((colourpicker.r <= 1.0) && (deltae(rgb2lab(colourpicker), rgb2lab(vec3(r, g, b))) > 0.1))\n" // Quick easy hack to disable colour picking
    "  {\n"
    "    float grey = (0.21 * r) + (0.71 * g) + (0.07 * b);\n"
    "    colour = vec4(grey, grey, grey, 1.0);\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "    colour = vec4(r, g, b, 1.0);\n"
    "  }\n"
    "}\n").c_str()))
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }
  
  if (!viewyuvshader_.link())
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::link failed"));
    return;
  }

  yuvtexturecoordlocation_ = viewyuvshader_.attributeLocation("texcoord");
  if (yuvtexturecoordlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  yuvpositionlocation_ = viewyuvshader_.attributeLocation("position");
  if (yuvpositionlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  yuvcolourpickerlocation_ = viewyuvshader_.uniformLocation("colourpicker");
  if (yuvcolourpickerlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  yuvtexturesamplerlocation_.at(0) = viewyuvshader_.uniformLocation("texy");
  if (yuvtexturesamplerlocation_.at(0) == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  yuvtexturesamplerlocation_.at(1) = viewyuvshader_.uniformLocation("texu");
  if (yuvtexturesamplerlocation_.at(1) == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  yuvtexturesamplerlocation_.at(2) = viewyuvshader_.uniformLocation("texv");
  if (yuvtexturesamplerlocation_.at(2) == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  // NV12 shader
  if (!viewnv12shader_.addShaderFromSourceCode(QOpenGLShader::Vertex,
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
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!viewnv12shader_.addShaderFromSourceCode(QOpenGLShader::Fragment,
    ("#version 130\n"
    + RGB2LAB + DELTAE +
    "in vec2 out_texcoord;\n"
    "uniform sampler2D texy;\n"
    "uniform sampler2D texuv;\n"
    "uniform vec3 colourpicker;\n"
    "out vec4 colour;\n"
    "void main()\n"
    "{\n"
    "  float y = texture2D(texy, out_texcoord.st).r;\n"
    "  float u = texture2D(texuv, out_texcoord.st).r - 0.5;\n"
    "  float v = texture2D(texuv, out_texcoord.st).g - 0.5;\n"
    "  float r = y + (1.13983 * v);\n"
    "  float g = y - ((0.39465 * u) + (0.58060 * v));\n"
    "  float b = y + (2.03211 * u);\n"
    "  if ((colourpicker.r <= 1.0) && (deltae(rgb2lab(colourpicker), rgb2lab(vec3(r, g, b))) > 0.1))\n" // Quick easy hack to disable colour picking
    "  {\n"
    "    float grey = (0.21 * r) + (0.71 * g) + (0.07 * b);\n"
    "    colour = vec4(grey, grey, grey, 1.0);\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "    colour = vec4(r, g, b, 1.0);\n"
    "  }\n"
    "}\n").c_str()))
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!viewnv12shader_.link())
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::link failed"));
    return;
  }

  nv12texturecoordlocation_ = viewnv12shader_.attributeLocation("texcoord");
  if (nv12texturecoordlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  nv12positionlocation_ = viewnv12shader_.attributeLocation("position");
  if (nv12positionlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  nv12colourpickerlocation_ = viewnv12shader_.uniformLocation("colourpicker");
  if (nv12colourpickerlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  nv12texturesamplerlocation_.at(0) = viewnv12shader_.uniformLocation("texy");
  if (nv12texturesamplerlocation_.at(0) == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  nv12texturesamplerlocation_.at(1) = viewnv12shader_.uniformLocation("texuv");
  if (nv12texturesamplerlocation_.at(1) == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  // Selected
  if (!viewselectedshader_.addShaderFromSourceCode(QOpenGLShader::Vertex, SELECTED_VERTEX_SHADER))
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }
  
  if (!viewselectedshader_.addShaderFromSourceCode(QOpenGLShader::Fragment, SELECTED_PIXEL_SHADER))
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  viewselectedshader_.bindAttributeLocation("position", 0);
  if (!viewselectedshader_.link())
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::bindAttributeLocation failed"));
    return;
  }
  selectedpositionlocation_ = viewselectedshader_.attributeLocation("position");
  selectedcolourlocation_ = viewselectedshader_.uniformLocation("colour");

  // Info
  if (!viewinfoshader_.addShaderFromSourceCode(QOpenGLShader::Vertex, INFO_VERTEX_SHADER))
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!viewinfoshader_.addShaderFromSourceCode(QOpenGLShader::Fragment, INFO_PIXEL_SHADER))
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  viewinfoshader_.bindAttributeLocation("position", 0);
  viewinfoshader_.bindAttributeLocation("texcoord", 1);

  if (!viewinfoshader_.link())
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::link failed"));
    return;
  }

  infotexturecoordlocation_ = viewinfoshader_.attributeLocation("texcoord");
  if (infotexturecoordlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  infopositionlocation_ = viewinfoshader_.attributeLocation("position");
  if (infopositionlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  infotexturesamplerlocation_ = viewinfoshader_.uniformLocation("sampler");
  if (infotexturesamplerlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  // Digitally Sign
  if (!digitalsignshader_.addShaderFromSourceCode(QOpenGLShader::Vertex,
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
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!digitalsignshader_.addShaderFromSourceCode(QOpenGLShader::Fragment,
    "#version 130\n"
    "in vec2 out_texcoord;\n"
    "uniform sampler2D sampler;\n"
    "out vec4 colour;\n"
    "void main()\n"
    "{\n"
    "  colour = texture(sampler, out_texcoord);\n"
    "  colour.w = min(colour.w, 0.5);\n"
    "}\n"))
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  digitalsignshader_.bindAttributeLocation("position", 0);
  digitalsignshader_.bindAttributeLocation("texcoord", 1);

  if (!digitalsignshader_.link())
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::link failed"));
    return;
  }

  digitalsigntexturecoordlocation_ = digitalsignshader_.attributeLocation("texcoord");
  if (digitalsigntexturecoordlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  digitalsignpositionlocation_ = digitalsignshader_.attributeLocation("position");
  if (digitalsignpositionlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::attributeLocation failed"));
    return;
  }

  digitalsigntexturesamplerlocation_ = digitalsignshader_.uniformLocation("sampler");
  if (digitalsigntexturesamplerlocation_ == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  glActiveTexture(GL_TEXTURE0);
  const QImage greenpadlock(":/greenpadlock.png");
  if (greenpadlock.isNull())
  {
    LOG_GUI_WARNING(QString("Unable to load :/greenpadlock.png"));
    return;
  }
  digitalsigngreenpadlock_.setData(greenpadlock, QOpenGLTexture::MipMapGeneration::DontGenerateMipMaps);

  const QImage redpadlock(":/redpadlock.png");
  if (redpadlock.isNull())
  {
    LOG_GUI_WARNING(QString("Unable to load :/redpadlock.png"));
    return;
  }
  digitalsignredpadlock_.setData(redpadlock, QOpenGLTexture::MipMapGeneration::DontGenerateMipMaps);

  SetUpdateFrequency(Options::Instance().GetUpdateFrequency());
}

void VideoWidget::resizeGL(int width, int height)
{
  if (openglmajorversion_ < 3)
  {

    return;
  }

  for (auto& view : views_)
  {
    view->ResetPosition(true); // This is required because the pixel size changes

  }
}

void VideoWidget::paintGL()
{
  if (openglmajorversion_ < 3)
  {

    return;
  }

  glClear(GL_COLOR_BUFFER_BIT);

  // Grid
  grid_.GetShader()->bind();
  grid_.GetVAO().bind();
  glDrawArrays(GL_LINES, 0, grid_.GetNumIndices());
  grid_.GetVAO().release();
  grid_.GetShader()->release();
  
  // Video views
  for (QSharedPointer<View>& view : views_)
  {
    // Texture
    ImageBuffer imagebuffer;
    if (view->GetImage(imagebuffer)) // Do we have a new image waiting?
    {
      if (view->IsPaused() || (imagebuffer.playrequestindex_ != view->GetPlayRequestIndex())) // Are we paused or getting frames from a request we don't care about, use the old frame
      {
        if ((view->GetImageType() == IMAGEBUFFERTYPE_TEXT) || (view->GetImageType() == IMAGEBUFFERTYPE_RGBA))
        {
          viewrgbshader_.bind();
          viewrgbshader_.setUniformValue(rgbtexturesamplerlocation_, 0);

          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, view->GetTextures().at(0));
        }
        else if (view->GetImageType() == IMAGEBUFFERTYPE_YUV)
        {
          viewyuvshader_.bind();
          for (GLuint texture = 0; texture < 3; ++texture)
          {
            viewyuvshader_.setUniformValue(yuvtexturesamplerlocation_.at(texture), texture);
            glActiveTexture(GL_TEXTURE0 + texture);
            glBindTexture(GL_TEXTURE_2D, view->GetTextures().at(texture));
          }
        }
        else if (view->GetImageType() == IMAGEBUFFERTYPE_NV12)
        {
          viewnv12shader_.bind();
          for (GLuint texture = 0; texture < 2; ++texture)
          {
            viewnv12shader_.setUniformValue(nv12texturesamplerlocation_.at(texture), texture);
            glActiveTexture(GL_TEXTURE0 + texture);
            glBindTexture(GL_TEXTURE_2D, view->GetTextures().at(texture));
          }
        }

        if (imagebuffer.playrequestindex_ == view->GetPlayRequestIndex())
        {
          view->AddCacheImage(imagebuffer); // Collect the frames because we might be stepping through them soon

        }
        else
        {
          view->GetFreeFrameBuffers().AddFreeImage(imagebuffer);

        }
      }
      else // New frame that we want to see
      {
        if ((view->GetImageType() != imagebuffer.type_) || (view->GetImageWidth() != imagebuffer.widths_[0]) || (view->GetImageHeight() != imagebuffer.heights_[0]))
        {
          view->SetType(imagebuffer.type_);
          view->SetImageWidth(imagebuffer.widths_[0]);
          view->SetImageHeight(imagebuffer.heights_[0]);
          view->ResetPosition(false);
        }

        view->IncrementTotalFrames();
        view->AddTotalBytes(imagebuffer.originalsize_);
        view->SetCodec(imagebuffer.codec_);
        view->SetTime(imagebuffer.time_);
        view->SetPlayMarkerTime(std::max(imagebuffer.time_, view->GetPlayMarkerTime()));
        view->SetFrameTime(std::chrono::steady_clock::now());
        view->SetSequenceNum(imagebuffer.sequencenum_);
        view->SetDigitallySigned(imagebuffer.digitallysigned_);

        if ((imagebuffer.type_ == IMAGEBUFFERTYPE_TEXT) || (imagebuffer.type_ == IMAGEBUFFERTYPE_RGBA))
        {
          viewrgbshader_.bind();
          viewrgbshader_.setUniformValue(rgbtexturesamplerlocation_, 0);
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, view->GetTextures().at(0));
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagebuffer.widths_[0], imagebuffer.heights_[0], 0, GL_RGBA, GL_UNSIGNED_BYTE, imagebuffer.data_[0]);
        }
        else if (imagebuffer.type_ == IMAGEBUFFERTYPE_YUV)
        {
          viewyuvshader_.bind();
          for (GLuint texture = 0; texture < 3; ++texture)
          {
            viewyuvshader_.setUniformValue(yuvtexturesamplerlocation_.at(texture), texture);
            glActiveTexture(GL_TEXTURE0 + texture);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, imagebuffer.strides_[texture]);
            glBindTexture(GL_TEXTURE_2D, view->GetTextures().at(texture));
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, imagebuffer.widths_.at(texture), imagebuffer.heights_.at(texture), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imagebuffer.data_[texture]);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
          }
        }
        else if (imagebuffer.type_ == IMAGEBUFFERTYPE_NV12)
        {
          cuCtxPushCurrent_v2(imagebuffer.cudacontext_);
          bool resetresources = false; // Do we need to reinitialise the cuda stuff if dimensions and format have changed
          if ((imagebuffer.type_ != view->GetImageType()) || (imagebuffer.widths_[0] != view->GetImageWidth()) || (imagebuffer.heights_[0] != view->GetImageHeight()) || !view->GetCUDAResource(0) || !view->GetCUDAResource(1))
          {
            // Destroy any old CUDA stuff we had laying around
            for (CUgraphicsResource& resource : view->GetCUDAResources())
            {
              if (resource)
              {
                cuGraphicsUnregisterResource(resource);
                resource = nullptr;
              }
            }
            resetresources = true;
          }

          viewnv12shader_.bind();
          for (GLuint texture = 0; texture < 2; ++texture)
          {
            viewnv12shader_.setUniformValue(nv12texturesamplerlocation_.at(texture), texture);
            glActiveTexture(GL_TEXTURE0 + texture);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, imagebuffer.strides_[texture]);
            glBindTexture(GL_TEXTURE_2D, view->GetTextures().at(texture));

            CUgraphicsResource resource = nullptr;
            if (resetresources)
            {
              glTexImage2D(GL_TEXTURE_2D, 0, (texture == 0) ? GL_RED : GL_RG, imagebuffer.widths_[texture], imagebuffer.heights_[texture], 0, (texture == 0) ? GL_RED : GL_RG, GL_UNSIGNED_BYTE, nullptr);
              cuGraphicsGLRegisterImage(&resource, view->GetTextures().at(texture), GL_TEXTURE_2D, CU_GRAPHICS_REGISTER_FLAGS_WRITE_DISCARD);
              view->SetCUDAResource(texture, resource);
            }
            else
            {
              resource = view->GetCUDAResource(texture);

            }

            cuGraphicsMapResources(1, &resource, 0);
            CUarray resourceptr;
            cuGraphicsSubResourceGetMappedArray(&resourceptr, resource, 0, 0);

            CUDA_MEMCPY2D copy;
            memset(&copy, 0, sizeof(copy));
            copy.srcMemoryType = CU_MEMORYTYPE_DEVICE;
            copy.dstMemoryType = CU_MEMORYTYPE_ARRAY;
            copy.srcDevice = (CUdeviceptr)imagebuffer.data_[texture];
            copy.dstArray = resourceptr;
            copy.srcPitch = imagebuffer.strides_[texture];
            copy.dstPitch = imagebuffer.strides_[texture];
            copy.WidthInBytes = (texture == 0) ? imagebuffer.widths_[texture] : (imagebuffer.widths_[texture] * 2);
            copy.Height = imagebuffer.heights_[texture];
            cuMemcpy2D(&copy);
            cuGraphicsUnmapResources(1, &resource, 0);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
          }

          CUcontext dummy;
          cuCtxPopCurrent_v2(&dummy);
        }
        view->GetFreeFrameBuffers().AddFreeImage(imagebuffer);
      }
    }
    else // Just bind the previous texture that we used
    {
      if ((view->GetImageType() == IMAGEBUFFERTYPE_TEXT) || (view->GetImageType() == IMAGEBUFFERTYPE_RGBA))
      {
        viewrgbshader_.bind();
        viewrgbshader_.setUniformValue(rgbtexturesamplerlocation_, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, view->GetTextures().at(0));
      }
      else if (view->GetImageType() == IMAGEBUFFERTYPE_YUV)
      {
        viewyuvshader_.bind();
        for (GLuint texture = 0; texture < 3; ++texture)
        {
          viewyuvshader_.setUniformValue(yuvtexturesamplerlocation_.at(texture), texture);
          glActiveTexture(GL_TEXTURE0 + texture);
          glBindTexture(GL_TEXTURE_2D, view->GetTextures().at(texture));
        }
      }
      else if (view->GetImageType() == IMAGEBUFFERTYPE_NV12)
      {
        viewnv12shader_.bind();
        for (GLuint texture = 0; texture < 2; ++texture)
        {
          viewnv12shader_.setUniformValue(nv12texturesamplerlocation_.at(texture), texture);
          glActiveTexture(GL_TEXTURE0 + texture);
          glBindTexture(GL_TEXTURE_2D, view->GetTextures().at(texture));
        }
      }
    }
    view->GetTextureBuffer().bind();

    // Vertices
    if ((view->GetImageType() == IMAGEBUFFERTYPE_TEXT) || (view->GetImageType() == IMAGEBUFFERTYPE_RGBA))
    {
      viewrgbshader_.enableAttributeArray(rgbtexturecoordlocation_);
      viewrgbshader_.setAttributeBuffer(rgbtexturecoordlocation_, GL_FLOAT, 0, 2);

      if (view->GetImageType() == IMAGEBUFFERTYPE_TEXT)
      {
        view->GetTextVertexBuffer().bind();

      }
      else
      {
        view->GetVertexBuffer().bind();

      }
      viewrgbshader_.enableAttributeArray(rgbpositionlocation_);
      viewrgbshader_.setAttributeBuffer(rgbpositionlocation_, GL_FLOAT, 0, 3);

      if (view->GetImageType() == IMAGEBUFFERTYPE_TEXT)
      {
        viewrgbshader_.setUniformValue(rgbcolourpickerlocation_, QVector3D(2.0f, 2.0f, 2.0f));

      }
      else
      {
        viewrgbshader_.setUniformValue(rgbcolourpickerlocation_, MainWindow::Instance()->GetColourPickerColour());

      }
    }
    else if (view->GetImageType() == IMAGEBUFFERTYPE_YUV)
    {
      viewyuvshader_.enableAttributeArray(yuvtexturecoordlocation_);
      viewyuvshader_.setAttributeBuffer(yuvtexturecoordlocation_, GL_FLOAT, 0, 2);
      
      view->GetVertexBuffer().bind();
      viewyuvshader_.enableAttributeArray(yuvpositionlocation_);
      viewyuvshader_.setAttributeBuffer(yuvpositionlocation_, GL_FLOAT, 0, 3);

      viewyuvshader_.setUniformValue(yuvcolourpickerlocation_, MainWindow::Instance()->GetColourPickerColour());
    }
    else if (view->GetImageType() == IMAGEBUFFERTYPE_NV12)
    {
      viewnv12shader_.enableAttributeArray(nv12texturecoordlocation_);
      viewnv12shader_.setAttributeBuffer(nv12texturecoordlocation_, GL_FLOAT, 0, 2);

      view->GetVertexBuffer().bind();
      viewnv12shader_.enableAttributeArray(nv12positionlocation_);
      viewnv12shader_.setAttributeBuffer(nv12positionlocation_, GL_FLOAT, 0, 3);

      viewnv12shader_.setUniformValue(nv12colourpickerlocation_, MainWindow::Instance()->GetColourPickerColour());
    }

    if ((view->GetImageType() == IMAGEBUFFERTYPE_TEXT) || (view->GetImageType() == IMAGEBUFFERTYPE_RGBA) || (view->GetImageType() == IMAGEBUFFERTYPE_YUV) || (view->GetImageType() == IMAGEBUFFERTYPE_NV12))
    {
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    }

    // Release
    if ((view->GetImageType() == IMAGEBUFFERTYPE_TEXT) || (view->GetImageType() == IMAGEBUFFERTYPE_RGBA))
    {
      viewrgbshader_.disableAttributeArray(rgbpositionlocation_);
      if (view->GetImageType() == IMAGEBUFFERTYPE_TEXT)
      {
        view->GetTextVertexBuffer().release();

      }
      else
      {
        view->GetVertexBuffer().release();

      }
      viewrgbshader_.disableAttributeArray(rgbtexturecoordlocation_);
      view->GetTextureBuffer().release();
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, 0);
      viewrgbshader_.release();
    }
    else if (view->GetImageType() == IMAGEBUFFERTYPE_YUV)
    {
      viewyuvshader_.disableAttributeArray(yuvpositionlocation_);
      view->GetVertexBuffer().release();
      viewyuvshader_.disableAttributeArray(yuvtexturecoordlocation_);
      view->GetTextureBuffer().release();
      for (GLuint texture = 0; texture < 3; ++texture)
      {
        glActiveTexture(GL_TEXTURE0 + texture);
        glBindTexture(GL_TEXTURE_2D, 0);
      }
    }
    else if (view->GetImageType() == IMAGEBUFFERTYPE_NV12)
    {
      viewnv12shader_.disableAttributeArray(nv12positionlocation_);
      view->GetVertexBuffer().release();
      viewnv12shader_.disableAttributeArray(nv12texturecoordlocation_);
      view->GetTextureBuffer().release();
      for (GLuint texture = 0; texture < 2; ++texture)
      {
        glActiveTexture(GL_TEXTURE0 + texture);
        glBindTexture(GL_TEXTURE_2D, 0);
      }
    }
  }
  
  // Info boxes
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glActiveTexture(GL_TEXTURE0);
  viewinfoshader_.bind();
  for (QSharedPointer<View>& view : views_)
  {
    if ((view->GetImageType() == IMAGEBUFFERTYPE_TEXT) || (!view->GetShowInfo()))
    {

      continue;
    }

    if (view->GetInfoTime() != view->GetTime()) // Do we need to refresh the time
    {
      if (view->GetViewType() == VIEWTYPE_MEDIA)
      {
        ToInfoText(QDateTime::fromMSecsSinceEpoch(view->GetTime(), Qt::UTC), Options::Instance().GetInfoTextFormat(), view->GetCodec(), view->GetBandwidthSizes(), std::make_pair<const std::string&, const QString&>(view.staticCast<MediaView>()->GetVideoLocation(), QString()), std::make_pair<const std::string&, const QString&>(view.staticCast<MediaView>()->GetVideoName(), QString()), view->GetImageWidth(), view->GetImageHeight(), infotextformatbuffer_);

      }
      else if (view->GetViewType() == VIEWTYPE_MONOCLE)
      {
        ToInfoText(QDateTime::fromMSecsSinceEpoch(view->GetTime(), Qt::UTC), Options::Instance().GetInfoTextFormat(), view->GetCodec(), view->GetBandwidthSizes(), std::make_pair<const std::string&, const QString&>(std::string(), view.staticCast<VideoView>()->GetRecording()->GetLocation()), std::make_pair<const std::string&, const QString&>(std::string(), view.staticCast<VideoView>()->GetRecording()->GetName()), view->GetImageWidth(), view->GetImageHeight(), infotextformatbuffer_);

      }
      else
      {
        // Not much else to do here except ignore this view
        continue;
      }

      const QImage infotexture = InfoTexture(infotextformatbuffer_, freetypearial_);
      glBindTexture(GL_TEXTURE_2D, view->GetInfoTexture());
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, infotexture.width(), infotexture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, infotexture.bits());
      glBindTexture(GL_TEXTURE_2D, 0);

      view->SetInfoTime(view->GetTime());
    }

    // Draw info
    viewinfoshader_.setUniformValue(infotexturesamplerlocation_, 0);
    glBindTexture(GL_TEXTURE_2D, view->GetInfoTexture());

    view->GetInfoTextureBuffer().bind();
    viewinfoshader_.enableAttributeArray(infotexturecoordlocation_);
    viewinfoshader_.setAttributeBuffer(infotexturecoordlocation_, GL_FLOAT, 0, 2);
    view->GetInfoVertexBuffer().bind();
    viewinfoshader_.enableAttributeArray(infopositionlocation_);
    viewinfoshader_.setAttributeBuffer(infopositionlocation_, GL_FLOAT, 0, 3);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    viewinfoshader_.disableAttributeArray(infopositionlocation_);
    view->GetInfoVertexBuffer().release();
    viewinfoshader_.disableAttributeArray(infotexturecoordlocation_);
    view->GetInfoTextureBuffer().release();
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  viewinfoshader_.release();

  // Digital Signatures
  digitalsignshader_.bind();
  for (QSharedPointer<View>& view : views_)
  {
    if (!view->GetDigitallySigned().is_initialized())
    {

      continue;
    }

    digitalsignshader_.setUniformValue(digitalsigntexturesamplerlocation_, 0);
    if (*view->GetDigitallySigned())
    {
      digitalsigngreenpadlock_.bind();

    }
    else
    {
      digitalsignredpadlock_.bind();

    }

    view->GetDigitalSignTextureBuffer().bind();
    viewinfoshader_.enableAttributeArray(digitalsigntexturecoordlocation_);
    viewinfoshader_.setAttributeBuffer(digitalsigntexturecoordlocation_, GL_FLOAT, 0, 2);

    view->GetDigitalSignVertexBuffer().bind();
    viewinfoshader_.enableAttributeArray(digitalsignpositionlocation_);
    viewinfoshader_.setAttributeBuffer(digitalsignpositionlocation_, GL_FLOAT, 0, 3);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    digitalsignshader_.disableAttributeArray(digitalsignpositionlocation_);
    view->GetDigitalSignVertexBuffer().release();
    digitalsignshader_.disableAttributeArray(digitalsigntexturecoordlocation_);
    view->GetDigitalSignTextureBuffer().release();
    if (*view->GetDigitallySigned())
    {
      digitalsigngreenpadlock_.release();

    }
    else
    {
      digitalsignredpadlock_.release();

    }
  }
  digitalsignshader_.release();
  glDisable(GL_BLEND);

  // Selection boxes
  viewselectedshader_.bind();
  for (QSharedPointer<View>& view : views_)
  {
    if (view->GetSelected())
    {
      view->GetSelectedVertexBuffer().bind();
      viewselectedshader_.enableAttributeArray(selectedpositionlocation_);
      viewselectedshader_.setAttributeBuffer(selectedpositionlocation_, GL_FLOAT, 0, 2);
      viewselectedshader_.setUniformValue(selectedcolourlocation_, view->GetSelectedColour());
      glDrawArrays(GL_LINE_STRIP, 0, 5);
      viewselectedshader_.disableAttributeArray(selectedpositionlocation_);
      view->GetSelectedVertexBuffer().release();
    }
  }

  if (((MainWindow::Instance()->GetMouseState() == MOUSESTATE_FINDMOTION) || (MainWindow::Instance()->GetMouseState() == MOUSESTATE_FINDOBJECT)) && MainWindow::Instance()->GetVideoWidgetsMgr().GetSelectionView())
  {
    const QSharedPointer<View> view = MainWindow::Instance()->GetVideoWidgetsMgr().GetSelectionView().lock();
    if (view)
    {
      const QPoint selectionpoint = MainWindow::Instance()->GetVideoWidgetsMgr().GetSelectionPoint();
      const QPoint cursor = mapFromGlobal(QCursor::pos());
      const QRect pixelrect = view->GetPixelRect();
      const QRect rect(QPoint(std::max(pixelrect.x(), std::min(selectionpoint.x(), cursor.x())), std::max(pixelrect.y(), std::min(selectionpoint.y(), cursor.y()))), QPoint(std::min(pixelrect.right(), std::max(selectionpoint.x(), cursor.x())), std::min(pixelrect.bottom(), std::max(selectionpoint.y(), cursor.y()))));

      // Create
      QOpenGLBuffer vertexbuffer;
      vertexbuffer.create();
      vertexbuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
      vertexbuffer.bind();
      const QRectF rectf(QPointF((((static_cast<float>(rect.x()) / static_cast<float>(width())) * 2.0f) - 1.0f), (((static_cast<float>(rect.y()) / static_cast<float>(height())) * -2.0f) + 1.0f)), QPointF((((static_cast<float>(rect.right()) / static_cast<float>(width())) * 2.0f) - 1.0f), (((static_cast<float>(rect.bottom()) / static_cast<float>(height())) * -2.0f) + 1.0f)));
      const std::array<float, 10> selectvertices =
      {
        static_cast<float>(rectf.right()), static_cast<float>(rectf.bottom()),
        static_cast<float>(rectf.right()), static_cast<float>(rectf.top()),
        static_cast<float>(rectf.left()), static_cast<float>(rectf.top()),
        static_cast<float>(rectf.left()), static_cast<float>(rectf.bottom()),
        static_cast<float>(rectf.right()), static_cast<float>(rectf.bottom())
      };
      vertexbuffer.allocate(selectvertices.data(), static_cast<int>(selectvertices.size() * sizeof(float)));

      viewselectedshader_.enableAttributeArray(selectedpositionlocation_);
      viewselectedshader_.setAttributeBuffer(selectedpositionlocation_, GL_FLOAT, 0, 2);
      viewselectedshader_.setUniformValue(selectedcolourlocation_, QVector4D(255, 0, 0, 0));
      glDrawArrays(GL_LINE_STRIP, 0, 5);
      viewselectedshader_.disableAttributeArray(selectedpositionlocation_);
      vertexbuffer.release();
      
      // Destroy
      vertexbuffer.destroy();
    }
  }
  
  // Objects
  for (QSharedPointer<View>& view : views_)
  {
    if (!view->GetShowObjects())
    {

      continue;
    }

    if ((view->GetViewType() != VIEWTYPE_MEDIA) && (view->GetViewType() != VIEWTYPE_MONOCLE))
    {

      continue;
    }

    if ((view->GetImageType() != IMAGEBUFFERTYPE_RGBA) && (view->GetImageType() != IMAGEBUFFERTYPE_NV12) && (view->GetImageType() != IMAGEBUFFERTYPE_YUV)) // We should have a frame drawn before we begin drawing things on top
    {

      continue;
    }

    const QRectF imagepixelrect = view->GetImagePixelRect();
    for (std::pair< const std::pair<monocle::ObjectClass, uint64_t>, std::vector<Object> >& objects : view->GetObjects())
    {
      const uint64_t time = view->GetTime();
      auto object = std::find_if(objects.second.rbegin(), objects.second.rend(), [time](const Object& object) { return ((object.time_ <= time) && ((time - object.time_) < OBJECT_KILL_DELAY)); });
      if (object == objects.second.rend())
      {

        continue;
      }

      object->vertexbuffer_.bind();
      viewselectedshader_.enableAttributeArray(selectedpositionlocation_);
      viewselectedshader_.setAttributeBuffer(selectedpositionlocation_, GL_FLOAT, 0, 2);
      viewselectedshader_.setUniformValue(selectedcolourlocation_, OBJECT_COLOURS[static_cast<size_t>(object->classid_)]);
      glDrawArrays(GL_LINE_STRIP, 0, 5);
      viewselectedshader_.disableAttributeArray(selectedpositionlocation_);
      object->vertexbuffer_.release();
    }
  }

  // Object text
  QPainter painter(this);
  for (QSharedPointer<View>& view : views_)
  {
    if (!view->GetShowObjects())
    {

      continue;
    }

    if ((view->GetViewType() != VIEWTYPE_MEDIA) && (view->GetViewType() != VIEWTYPE_MONOCLE))
    {

      continue;
    }

    if ((view->GetImageType() != IMAGEBUFFERTYPE_RGBA) && (view->GetImageType() != IMAGEBUFFERTYPE_NV12) && (view->GetImageType() != IMAGEBUFFERTYPE_YUV)) // We should have a frame drawn before we begin drawing things on top
    {

      continue;
    }

    const QRectF imagepixelrect = view->GetImagePixelRectF();
    for (std::pair< const std::pair<monocle::ObjectClass, uint64_t>, std::vector<Object> >& objects : view->GetObjects())
    {
      const uint64_t time = view->GetTime();
      auto object = std::find_if(objects.second.rbegin(), objects.second.rend(), [time](const Object& object) { return ((object.time_ <= time) && ((time - object.time_) < OBJECT_KILL_DELAY)); });
      if (object == objects.second.rend())
      {

        continue;
      }
      object->DrawObjectText(imagepixelrect, width(), height(), view->GetMirror(), view->GetRotation(), painter);
    }
  }

  viewselectedshader_.release();
}

RESIZEDIRECTION VideoWidget::GetResizeDirection(const QSharedPointer<View>& view, const QPoint& pos) const
{
  const QRect pixelrect = view->GetPixelRect();
  if (((pos.x() - pixelrect.left()) < RESIZE_BORDER) && ((pos.y() - pixelrect.top()) < RESIZE_BORDER))
  {

    return RESIZEDIRECTION::NORTH_WEST;
  }
  else if (((pixelrect.right() - pos.x()) < RESIZE_BORDER) && ((pos.y() - pixelrect.top()) < RESIZE_BORDER))
  {

    return RESIZEDIRECTION::NORTH_EAST;
  }
  else if (((pos.x() - pixelrect.left()) < RESIZE_BORDER) && ((pixelrect.bottom() - pos.y()) < RESIZE_BORDER))
  {

    return RESIZEDIRECTION::SOUTH_WEST;
  }
  else if (((pixelrect.right() - pos.x()) < RESIZE_BORDER) && ((pixelrect.bottom() - pos.y()) < RESIZE_BORDER))
  {

    return RESIZEDIRECTION::SOUTH_EAST;
  }
  else if ((pos.x() - pixelrect.left()) < RESIZE_BORDER)
  {

    return RESIZEDIRECTION::WEST;
  }
  else if ((pixelrect.right() - pos.x()) < RESIZE_BORDER)
  {

    return RESIZEDIRECTION::EAST;
  }
  else if ((pos.y() - pixelrect.top()) < RESIZE_BORDER)
  {

    return RESIZEDIRECTION::NORTH;
  }
  else if ((pixelrect.bottom() - pos.y()) < RESIZE_BORDER)
  {

    return RESIZEDIRECTION::SOUTH;
  }
  else
  {

    return RESIZEDIRECTION::NONE;
  }
}

void VideoWidget::ShowToolbar(bool)
{
  GetToolbar()->show();

}

void VideoWidget::HideToolbar(bool)
{
  GetToolbar()->hide();

}

void VideoWidget::AddRow(bool)
{
  if (openglmajorversion_ < 3)
  {

    return;
  }

  if (height_ >= MAXVIDEOWINDOWHEIGHT)
  {

    return;
  }

  ++height_;

  makeCurrent();

  grid_.Generate(width_, height_, views_);

  for (auto& videoview : views_)
  {
    videoview->ResetPosition(true);

  }

  doneCurrent();
}

void VideoWidget::RemoveRow(bool)
{
  if (openglmajorversion_ < 3)
  {

    return;
  }

  if (height_ <= 1)
  {
    LOG_GUI_WARNING("Can not remove all rows");
    return;
  }

  // Make sure no video views are in the way
  for (auto& videoview : views_)
  {
    for (unsigned int x = 0; x < width_; ++x)
    {
      if (videoview->GetRect().contains(x, height_ - 1))
      {
        LOG_GUI_WARNING("Can not remove row obstructing video view");
        return;
      }
    }
  }

  --height_;

  makeCurrent();

  grid_.Generate(width_, height_, views_);

  for (auto& videoview : views_)
  {
    videoview->ResetPosition(true);

  }

  doneCurrent();
}

void VideoWidget::AddColumn(bool)
{
  if (openglmajorversion_ < 3)
  {

    return;
  }

  if (width_ >= MAXVIDEOWINDOWWIDTH)
  {

    return;
  }

  ++width_;

  makeCurrent();

  grid_.Generate(width_, height_, views_);

  for (auto& videoview : views_)
  {
    videoview->ResetPosition(true);

  }

  doneCurrent();
}

void VideoWidget::RemoveColumn(bool)
{
  if (openglmajorversion_ < 3)
  {

    return;
  }

  if (width_ <= 1)
  {
    LOG_GUI_WARNING("Can not remove all columns");
    return;
  }

  // Make sure no video views are in the way
  for (auto& view : views_)
  {
    for (unsigned int y = 0; y < width_; ++y)
    {
      if (view->GetRect().contains(width_ - 1, y))
      {
        LOG_GUI_WARNING("Can not remove row obstructing video view");
        return;
      }
    }
  }

  --width_;

  makeCurrent();

  grid_.Generate(width_, height_, views_);

  for (auto& view : views_)
  {
    view->ResetPosition(true);

  }

  doneCurrent();
}

void VideoWidget::Fullscreen(bool)
{
  QWidget* window = static_cast<QWidget*>(parent()->parent());
  if (window->isActiveWindow())
  {
    if (window->isFullScreen())
    {
      window->showNormal();

    }
    else
    {
      window->showFullScreen();

    }
  }
}

void VideoWidget::MapRemoved(const QSharedPointer<Map>& map)
{
  std::vector< QSharedPointer<View> > removemapviews;
  for (const QSharedPointer<View>& view : views_)
  {
    if ((view->GetViewType() != VIEWTYPE_MAP) && (static_cast<MapView*>(view.get())->GetMap() != map))
    {

      continue;
    }
    removemapviews.push_back(view);
  }

  // We have to do this because it removes from our current vector
  for (const QSharedPointer<View>& removemapview : removemapviews)
  {
    RemoveView(removemapview);
    emit MainWindow::Instance()->GetVideoWidgetsMgr().ViewDestroyed(removemapview);
  }
}

void VideoWidget::MediaRemoved(QSharedPointer<Media>& media)
{
  std::vector< QSharedPointer<View> > removemediaviews;
  for (const QSharedPointer<View>& view : views_)
  {
    if ((view->GetViewType() != VIEWTYPE_MEDIA) && (static_cast<MediaView*>(view.get())->GetMedia() != media))
    {

      continue;
    }
    removemediaviews.push_back(view);
  }

  // We have to do this because it removes from our current vector
  for (const QSharedPointer<View>& removemediaview : removemediaviews)
  {
    RemoveView(removemediaview);
    emit MainWindow::Instance()->GetVideoWidgetsMgr().ViewDestroyed(removemediaview);
  }
}

void VideoWidget::RecordingRemoved(const boost::shared_ptr<Device>& device, const uint64_t recordingtoken)
{
  std::vector< QSharedPointer<View> > removeviews;
  for (const QSharedPointer<View>& view : views_)
  {
    if ((view->GetViewType() != VIEWTYPE_MONOCLE))
    {

      continue;
    }

    if (static_cast<VideoView*>(view.get())->GetRecording()->GetToken() == recordingtoken)
    {
      removeviews.push_back(view);

    }
  }

  // We have to do this because it removes from our current vector
  for (const QSharedPointer<View>& removeview : removeviews)
  {
    RemoveView(removeview);

  }
}

}
