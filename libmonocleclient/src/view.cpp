// view.cpp
//

///// Includes /////

#include "monocleclient/videoview.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/bind.hpp>
#include <cuda.h>
#include <cudaGL.h>
#include <GL/gl.h>
#include <monocleprotocol/objects_generated.h>
#include <network/uri.hpp>
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPointF>
#include <QResource>
#include <QStandardPaths>
#include <utility/utility.hpp>

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/recording.h"
#include "monocleclient/videowidget.h"

///// Namespaces /////

namespace client
{

///// Declarations /////

const std::array<float, 8> View::texturecoords_ =
{
  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 1.0f,
  0.0f, 0.0f
};

///// Functions /////

QString ToString(const ROTATION rotation)
{
  if (rotation == ROTATION::_0)
  {

    return "0";
  }
  else if (rotation == ROTATION::_90)
  {

    return "90";
  }
  else if (rotation == ROTATION::_180)
  {

    return "180";
  }
  else if (rotation == ROTATION::_270)
  {

    return "270";
  }
  else
  {

    return "0"; // Shouldn't happen, but lets just be normal
  }
}

QPointF ImageRectToOpenGL(const QRectF& rect, const bool mirror, const ROTATION rotation, const float x, const float y)
{
  if (mirror)
  {
    if (rotation == ROTATION::_90)
    {
      const float pointx = ((rect.right() - (y * rect.width())) * 2.0f) - 1.0f;
      const float pointy = 1.0f - ((rect.bottom() - (x * rect.height())) * 2.0f);
      return QPointF(pointx, pointy);
    }
    else if (rotation == ROTATION::_180)
    {
      const float pointx = ((rect.left() + (x * rect.width())) * 2.0f) - 1.0f;
      const float pointy = 1.0f - ((rect.bottom() - (y * rect.height())) * 2.0f);
      return QPointF(pointx, pointy);
    }
    else if (rotation == ROTATION::_270)
    {
      const float pointx = ((rect.left() + (y * rect.width())) * 2.0f) - 1.0f;
      const float pointy = 1.0f - ((rect.top() + (x * rect.height())) * 2.0f);
      return QPointF(pointx, pointy);
    }
    else // (rotation == ROTATION::_0)
    {
      const float pointx = ((rect.right() - (x * rect.width())) * 2.0f) - 1.0f;
      const float pointy = 1.0f - ((rect.top() + (y * rect.height())) * 2.0f);
      return QPointF(pointx, pointy);
    }
  }
  else
  {
    if (rotation == ROTATION::_90)
    {
      const float pointx = ((rect.right() - (y * rect.width())) * 2.0f) - 1.0f;
      const float pointy = 1.0f - ((rect.top() + (x * rect.height())) * 2.0f);
      return QPointF(pointx, pointy);
    }
    else if (rotation == ROTATION::_180)
    {
      const float pointx = ((rect.right() - (x * rect.width())) * 2.0f) - 1.0f;
      const float pointy = 1.0f - ((rect.bottom() - (y * rect.height())) * 2.0f);
      return QPointF(pointx, pointy);
    }
    else if (rotation == ROTATION::_270)
    {
      const float pointx = ((rect.left() + (y * rect.width())) * 2.0f) - 1.0f;
      const float pointy = 1.0f - ((rect.bottom() - (x * rect.height())) * 2.0f);
      return QPointF(pointx, pointy);
    }
    else // (rotation == ROTATION::_0)
    {
      const float pointx = ((rect.left() + (x * rect.width())) * 2.0f) - 1.0f;
      const float pointy = 1.0f - ((rect.top() + (y * rect.height())) * 2.0f);
      return QPointF(pointx, pointy);
    }
  }
}

QRectF ImageToRect(const QRect& imagepixelrect, const QRect& rect, const bool mirror, const ROTATION rotation)
{
  QRectF selectedrectf(static_cast<float>(rect.left()) / static_cast<float>(imagepixelrect.width()),
                       static_cast<float>(rect.top()) / static_cast<float>(imagepixelrect.height()),
                       static_cast<float>(rect.width()) / static_cast<float>(imagepixelrect.width()),
                       static_cast<float>(rect.height()) / static_cast<float>(imagepixelrect.height()));

  if (mirror)
  {
    if (rotation == ROTATION::_90)
    {
      selectedrectf = QRectF(selectedrectf.left(), 1.0f - selectedrectf.bottom(), selectedrectf.width(), selectedrectf.height());
      QTransform transform;
      transform.rotate(270.0f);
      selectedrectf = transform.mapRect(selectedrectf);
      selectedrectf.adjust(0.0f, 1.0f, 0.0f, 1.0f);
      return selectedrectf;
    }
    else if (rotation == ROTATION::_180)
    {
      selectedrectf = QRectF(1.0f - selectedrectf.right(), selectedrectf.top(), selectedrectf.width(), selectedrectf.height());
      QTransform transform;
      transform.rotate(180.0f);
      selectedrectf = transform.mapRect(selectedrectf);
      selectedrectf.adjust(1.0f, 1.0f, 1.0f, 1.0f);
      return selectedrectf;
    }
    else if (rotation == ROTATION::_270)
    {
      selectedrectf = QRectF(selectedrectf.left(), 1.0f - selectedrectf.bottom(), selectedrectf.width(), selectedrectf.height());
      QTransform transform;
      transform.rotate(90.0f);
      selectedrectf = transform.mapRect(selectedrectf);
      selectedrectf.adjust(1.0f, 0.0f, 1.0f, 0.0f);
      return selectedrectf;
    }
    else // (rotation == ROTATION::_0)
    {
      return QRectF(1.0f - selectedrectf.right(), selectedrectf.top(), selectedrectf.width(), selectedrectf.height());

    }
  }
  else
  {
    if (rotation == ROTATION::_90)
    {
      QTransform transform;
      transform.rotate(270.0f);
      selectedrectf = transform.mapRect(selectedrectf);
      selectedrectf.adjust(0.0f, 1.0f, 0.0f, 1.0f);
      return selectedrectf;
    }
    else if (rotation == ROTATION::_180)
    {
      QTransform transform;
      transform.rotate(180.0f);
      selectedrectf = transform.mapRect(selectedrectf);
      selectedrectf.adjust(1.0f, 1.0f, 1.0f, 1.0f);
      return selectedrectf;
    }
    else if (rotation == ROTATION::_270)
    {
      QTransform transform;
      transform.rotate(90.0f);
      selectedrectf = transform.mapRect(selectedrectf);
      selectedrectf.adjust(1.0f, 0.0f, 1.0f, 0.0f);
      return selectedrectf;
    }
    else // (rotation == ROTATION::_0)
    {

      return selectedrectf;
    }
  }

}

///// Methods /////

Object::Object(const uint64_t id, const monocle::ObjectClass classid, const uint64_t time, const float x, const float y, const float width, const float height) :
  id_(id),
  classid_(classid),
  time_(time),
  x_(x),
  y_(y),
  width_(width),
  height_(height),
  age_(std::chrono::steady_clock::now())
{

}

Object::Object(Object&& rhs) :
  id_(rhs.id_),
  classid_(rhs.classid_),
  time_(rhs.time_),
  x_(rhs.x_),
  y_(rhs.y_),
  width_(rhs.width_),
  height_(rhs.height_),
  vertexbuffer_(std::move(rhs.vertexbuffer_)),
  text_(std::move(rhs.text_)),
  age_(rhs.age_)
{

}

void Object::Allocate(const QRectF& imagepixelrect, const bool mirror, const ROTATION rotation)
{
  const QPointF topleft = ImageRectToOpenGL(imagepixelrect, mirror, rotation, x_, y_);
  const QPointF bottomright = ImageRectToOpenGL(imagepixelrect, mirror, rotation, x_ + width_, y_ + height_);
  std::array<float, 10> vertices =
  {
    static_cast<float>(bottomright.x()), static_cast<float>(bottomright.y()),
    static_cast<float>(bottomright.x()), static_cast<float>(topleft.y()),
    static_cast<float>(topleft.x()), static_cast<float>(topleft.y()),
    static_cast<float>(topleft.x()), static_cast<float>(bottomright.y()),
    static_cast<float>(bottomright.x()), static_cast<float>(bottomright.y())
  };

  // Allocate the sort out the bufferbuffer
  vertexbuffer_.bind();
  vertexbuffer_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(float)));
  vertexbuffer_.release();
}

void Object::DrawObjectText(const QRectF& imagepixelrect, const int width, const int height, const bool mirror, const ROTATION rotation, QPainter& painter)
{
  QPointF point;
  if (mirror)
  {
    if (rotation == ROTATION::_90)
    {
      point = ImageRectToOpenGL(imagepixelrect, mirror, rotation, x_ + width_, y_ + height_);

    }
    else if (rotation == ROTATION::_180)
    {
      point = ImageRectToOpenGL(imagepixelrect, mirror, rotation, x_, y_ + height_);

    }
    else if (rotation == ROTATION::_270)
    {
      point = ImageRectToOpenGL(imagepixelrect, mirror, rotation, x_, y_);

    }
    else // (rotation == ROTATION::_0)
    {
      point = ImageRectToOpenGL(imagepixelrect, mirror, rotation, x_ + width_, y_);

    }
  }
  else
  {
    if (rotation == ROTATION::_90)
    {
      point = ImageRectToOpenGL(imagepixelrect, mirror, rotation, x_, y_ + height_);

    }
    else if (rotation == ROTATION::_180)
    {
      point = ImageRectToOpenGL(imagepixelrect, mirror, rotation, x_ + width_, y_ + height_);

    }
    else if (rotation == ROTATION::_270)
    {
      point = ImageRectToOpenGL(imagepixelrect, mirror, rotation, x_ + width_, y_);

    }
    else // (rotation == ROTATION::_0)
    {
      point = ImageRectToOpenGL(imagepixelrect, mirror, rotation, x_, y_);

    }
  }
  point.setX((point.x() + 1.0f) / 2.0f);
  point.setY((point.y() + 1.0f) / 2.0f);
  point.setX(point.x() * width);
  point.setY(height - ((point.y() * height) + text_.size().height()));
  painter.drawStaticText(point, text_);
}

Object& Object::operator=(Object&& rhs)
{
  id_ = rhs.id_;
  classid_ = rhs.classid_;
  time_ = rhs.time_;
  x_ = rhs.x_;
  y_ = rhs.y_;
  width_ = rhs.width_;
  height_ = rhs.height_;
  vertexbuffer_ = std::move(rhs.vertexbuffer_);
  text_ = std::move(rhs.text_);
  age_ = rhs.age_;
  return *this;
}

View::View(VideoWidget* videowidget, CUcontext cudacontext, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool mirror, const bool stretch, const bool showinfo, const bool showobjects, const QResource* arial, const bool showsaveimagemenu, const bool showcopymenu, const bool showinfomenu, const bool showobjectsmenu) :
  videowidget_(videowidget),
  cudacontext_(cudacontext),
  starttime_(std::chrono::steady_clock::now()),
  selectedcolour_(selectedcolour.redF(), selectedcolour.greenF(), selectedcolour.blueF(), selectedcolour.alphaF()),
  actionsaveimage_(showsaveimagemenu ? new QAction(tr("Save Image"), this) : nullptr),
  actioncopy_(showcopymenu ? new QAction(tr("Copy"), this) : nullptr),
  actionrotate0_(new QAction(tr("Rotate 0"), this)),
  actionrotate90_(new QAction(tr("Rotate 90"), this)),
  actionrotate180_(new QAction(tr("Rotate 180"), this)),
  actionrotate270_(new QAction(tr("Rotate 270"), this)),
  actionmirror_(new QAction(tr("Mirror"), this)),
  actionstretch_(new QAction(tr("Stretch"), this)),
  actioninfo_(showinfomenu ? new QAction(tr("Info"), this) : nullptr),
  actionobjects_(showobjectsmenu ? new QAction(tr("Objects"), this) : nullptr),
  freetype_(nullptr),
  totalframes_(0),
  totalbytes_(0),
  time_(0),
  playmarkertime_(0),
  type_(IMAGEBUFFERTYPE_INVALID),
  sequencenum_(0),
  texturebuffer_(QOpenGLBuffer::VertexBuffer),
  textvertexbuffer_(QOpenGLBuffer::VertexBuffer),
  vertexbuffer_(QOpenGLBuffer::VertexBuffer),
  textures_({ 0, 0, 0 }),
  cudaresources_({ nullptr, nullptr, nullptr }),
  infotexture_(0),
  infotexturebuffer_(QOpenGLBuffer::VertexBuffer),
  infovertexbuffer_(QOpenGLBuffer::VertexBuffer),
  infotime_(0),
  digitalsigntexturebuffer_(QOpenGLBuffer::VertexBuffer),
  digitalsignvertexbuffer_(QOpenGLBuffer::VertexBuffer),
  rect_(QPoint(x, y), QPoint(x + width - 1, y + height - 1)),
  rotation_(rotation),
  mirror_(mirror),
  stretch_(stretch),
  showinfo_(showinfo),
  showobjects_(showobjects),
  selected_(false),
  paused_(false),
  actionclose_(new QAction(tr("Close"), this)),
  imagewidth_(0),
  imageheight_(0),
  bandwidthsizes_(150),
  playrequestindex_(0)
{
  actionrotate0_->setCheckable(true);
  actionrotate90_->setCheckable(true);
  actionrotate180_->setCheckable(true);
  actionrotate270_->setCheckable(true);
  actionmirror_->setCheckable(true);
  actionmirror_->setChecked(mirror_);
  actionstretch_->setCheckable(true);
  actionstretch_->setChecked(stretch_);

  connect(actionrotate0_, &QAction::triggered, this, &View::Rotate0);
  connect(actionrotate90_, &QAction::triggered, this, &View::Rotate90);
  connect(actionrotate180_, &QAction::triggered, this, &View::Rotate180);
  connect(actionrotate270_, &QAction::triggered, this, &View::Rotate270);
  connect(actionmirror_, &QAction::triggered, this, &View::ToggleMirror);
  connect(actionstretch_, &QAction::triggered, this, &View::ToggleStretch);

  if (actionsaveimage_)
  {
    connect(actionsaveimage_, &QAction::triggered, this, &View::SaveImage);

  }

  if (actioncopy_)
  {
    connect(actioncopy_, &QAction::triggered, this, &View::Copy);

  }

  if (actioninfo_)
  {
    actioninfo_->setCheckable(true);
    actioninfo_->setChecked(showinfo_);
    connect(actioninfo_, &QAction::triggered, this, &View::ToggleShowInfo);
  }

  if (actionobjects_)
  {
    actionobjects_->setCheckable(true);
    actionobjects_->setChecked(showobjects_);
    connect(actionobjects_, &QAction::triggered, this, &View::ToggleShowObjects);
  }

  // Freetype
  if (FT_Init_FreeType(&freetype_))
  {
    LOG_GUI_WARNING(tr("Error initialising FreeType"));
    return;
  }

  if (FT_New_Memory_Face(freetype_, arial->data(), arial->size(), 0, &arial_))
  {
    LOG_GUI_WARNING(tr("Error loading Arial resource"));
    return;
  }

  if (FT_Set_Pixel_Sizes(arial_, 0, 16))
  {
    LOG_GUI_WARNING(tr("Error initialising Arial resource"));
    return;
  }

  videowidget->makeCurrent();

  // Textures, we create three, even though we may actually only use one, but we can't predict the formats.
  // We could lazily initialise but these consume such little resources we just bang them in now
  videowidget->glGenTextures(static_cast<GLsizei>(textures_.size()), &textures_.at(0));
  for (size_t i = 0; i < textures_.size(); ++i)
  {
    videowidget->glBindTexture(GL_TEXTURE_2D, textures_.at(i));
    videowidget->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    videowidget->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    videowidget->glBindTexture(GL_TEXTURE_2D, 0);
  }

  // Setup the texture buffer
  if (!texturebuffer_.create())
  {
    SetMessage(++playrequestindex_, true, tr("Error creating texture buffer"));
    return;
  }
  texturebuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  texturebuffer_.bind();
  texturebuffer_.allocate(texturecoords_.data(), static_cast<int>(texturecoords_.size() * sizeof(float)));
  texturebuffer_.release();

  // Setup the vertex buffers
  if (!textvertexbuffer_.create())
  {
    SetMessage(++playrequestindex_, true, tr("Error creating vertex buffer"));
    return;
  }
  textvertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);

  if (!vertexbuffer_.create())
  {
    SetMessage(++playrequestindex_, true, tr("Error creating vertex buffer"));
    return;
  }
  vertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);

  // Info
  infotexturebuffer_.create();
  infotexturebuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  infotexturebuffer_.bind();
  infotexturebuffer_.allocate(texturecoords_.data(), static_cast<int>(texturecoords_.size() * sizeof(float)));
  infotexturebuffer_.release();

  videowidget_->glGenTextures(1, &infotexture_);
  videowidget_->glBindTexture(GL_TEXTURE_2D, infotexture_);
  videowidget_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  videowidget_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  infovertexbuffer_.create();
  infovertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);

  // Digital Signature
  digitalsigntexturebuffer_.create();
  digitalsigntexturebuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  digitalsigntexturebuffer_.bind();
  digitalsigntexturebuffer_.allocate(texturecoords_.data(), static_cast<int>(texturecoords_.size() * sizeof(float)));
  digitalsigntexturebuffer_.release();

  infovertexbuffer_.create();
  infovertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);

  videowidget->doneCurrent();
}

View::~View()
{
  selected_ = false;

  if (FT_Done_Face(arial_))
  {
    LOG_GUI_WARNING(tr("Error destroying arial face"));

  }
  arial_ = nullptr;

  if (FT_Done_FreeType(freetype_))
  {
    LOG_GUI_WARNING(tr("Error destroying FreeType"));

  }
  freetype_ = nullptr;

  for (CUgraphicsResource& cudaresource : cudaresources_)
  {
    if (cudaresource)
    {
      cuGraphicsUnregisterResource(cudaresource);
      cudaresource = nullptr;
    }
  }

  // GL stuff
  if (videowidget_)
  {
    videowidget_->makeCurrent();
    videowidget_->glDeleteTextures(3, textures_.data());
    textures_.fill(0);

    texturebuffer_.destroy();
    textvertexbuffer_.destroy();
    vertexbuffer_.destroy();
    selectvertexbuffer_.destroy();
    videowidget_->glDeleteTextures(1, &infotexture_);
    infotexture_ = 0;
    infotexturebuffer_.destroy();
    infovertexbuffer_.destroy();
    videowidget_->doneCurrent();
  }

  imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { const_cast<ImageBuffer&>(imagebuffer).Destroy(); });
  cache_.Clear();
}

void View::GetMenu(QMenu& parent)
{
  if (actionsaveimage_)
  {
    parent.addAction(actionsaveimage_);

  }

  if (actioncopy_)
  {
    parent.addAction(actioncopy_);

  }

  // Rotation
  QMenu* rotation = new QMenu(QString("Rotation"), &parent);
  actionrotate0_->setChecked(rotation_ == ROTATION::_0);
  actionrotate90_->setChecked(rotation_ == ROTATION::_90);
  actionrotate180_->setChecked(rotation_ == ROTATION::_180);
  actionrotate270_->setChecked(rotation_ == ROTATION::_270);
  rotation->addActions({ actionrotate0_, actionrotate90_, actionrotate180_, actionrotate270_ });
  parent.addMenu(rotation);
  parent.addAction(actionmirror_);
  parent.addAction(actionstretch_);
  if (actioninfo_)
  {
    parent.addAction(actioninfo_);

  }
  if (actionobjects_)
  {
    parent.addAction(actionobjects_);

  }
  parent.addAction(actionclose_);
}

void View::ResetPosition(const bool makecurrent)
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), rotation_, mirror_, stretch_, makecurrent);

}

void View::SetPosition(VideoWidget* videowidget, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool mirror, const bool stretch, const bool makecurrent)
{
  videowidget_ = videowidget;
  rect_.setX(x);
  rect_.setY(y);
  rect_.setWidth(width);
  rect_.setHeight(height);
  rotation_ = rotation;
  mirror_ = mirror;
  stretch_ = stretch;

  double aspectratio = GetAspectRatio();
  const float pixelwidth = 1.0f / videowidget->width();
  const float pixelheight = 1.0f / videowidget->height();
  const QRectF rect = GetOpenglRect(x, y, width, height);
  if (makecurrent)
  {
    videowidget_->makeCurrent();

  }

  // Text vertex buffer
  std::array<float, 12> vertices = GetVertices(rect, ROTATION::_0, false);
  textvertexbuffer_.bind();
  textvertexbuffer_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(float)));
  textvertexbuffer_.release();

  // Vertex buffer
  vertexbuffer_.bind();
  const QRectF imagerect = GetImageRect();
  vertices = GetVertices(imagerect, rotation_, mirror_);
  vertexbuffer_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(float)));
  vertexbuffer_.release();

  // Select vertex buffer
  selectvertexbuffer_.destroy();
  selectvertexbuffer_.create();
  selectvertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  selectvertexbuffer_.bind();
  const std::array<float, 10> selectvertices =
  {
    static_cast<float>(rect.right() - pixelwidth), static_cast<float>(rect.bottom() + pixelheight),
    static_cast<float>(rect.right() - pixelwidth), static_cast<float>(rect.top() - pixelheight),
    static_cast<float>(rect.left() + pixelwidth), static_cast<float>(rect.top() - pixelheight),
    static_cast<float>(rect.left() + pixelwidth), static_cast<float>(rect.bottom() + pixelheight),
    static_cast<float>(rect.right() - pixelwidth), static_cast<float>(rect.bottom() + pixelheight)
  };
  selectvertexbuffer_.allocate(selectvertices.data(), static_cast<int>(selectvertices.size() * sizeof(float)));
  selectvertexbuffer_.release();

  // Info
  infovertexbuffer_.destroy();
  infovertexbuffer_.create();
  infovertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  infovertexbuffer_.bind();
  static const float heightpixels = 50.0f;
  const float infoheight = (2.0f / static_cast<float>(videowidget_->height())) * heightpixels;
  const float infowidth = (2.0f / static_cast<float>(videowidget_->width())) * (heightpixels / (static_cast<float>(INFO_HEIGHT) / static_cast<float>(INFO_WIDTH)));
  const float right = std::min(imagerect.left() + infowidth, imagerect.right());
  const std::array<float, 12> infovertices =
  {
    static_cast<float>(right), static_cast<float>(imagerect.bottom()), 0.0f,
    static_cast<float>(right), std::min(static_cast<float>(imagerect.bottom()) + infoheight, static_cast<float>(imagerect.top())), 0.0f,
    static_cast<float>(imagerect.left()), static_cast<float>(imagerect.bottom()), 0.0f,
    static_cast<float>(imagerect.left()), std::min(static_cast<float>(imagerect.bottom()) + infoheight, static_cast<float>(imagerect.top())), 0.0f
  };
  infovertexbuffer_.allocate(infovertices.data(), static_cast<int>(infovertices.size() * sizeof(float)));
  infovertexbuffer_.release();

  // Digital Signature
  if (digitalsignvertexbuffer_.isCreated())
  {
    digitalsignvertexbuffer_.destroy();

  }
  digitalsignvertexbuffer_.create();
  digitalsignvertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  digitalsignvertexbuffer_.bind();
  const float digitalsignaturewidthpixels = 15.0f;
  const float digitalsignatureheightpixels = 22.0f;
  const float digitalsignatureheight = (2.0f / static_cast<float>(videowidget_->height())) * digitalsignatureheightpixels;
  const float digitalsignaturewidth = (2.0f / static_cast<float>(videowidget_->width())) * (digitalsignatureheightpixels / (static_cast<float>(digitalsignatureheightpixels) / static_cast<float>(digitalsignaturewidthpixels)));
  const std::array<float, 12> digitalsignvertices =
  {
    static_cast<float>(imagerect.right()), std::max(static_cast<float>(imagerect.top() - digitalsignatureheight), static_cast<float>(imagerect.bottom())), 0.0f,
    static_cast<float>(imagerect.right()), static_cast<float>(imagerect.top()), 0.0f,
    static_cast<float>(imagerect.right() - digitalsignaturewidth), std::max(static_cast<float>(imagerect.top() - digitalsignatureheight), static_cast<float>(imagerect.bottom())), 0.0f,
    static_cast<float>(imagerect.right() - digitalsignaturewidth), static_cast<float>(imagerect.top()), 0.0f
  };
  digitalsignvertexbuffer_.allocate(digitalsignvertices.data(), static_cast<int>(digitalsignvertices.size() * sizeof(float)));
  digitalsignvertexbuffer_.release();

  // Update the objects
  const QRectF imagepixelrect = GetImagePixelRectF();
  for (std::pair< const std::pair<monocle::ObjectClass, uint64_t>, std::vector<Object> >& objects : objects_)
  {
    for (Object& object : objects.second)
    {
      object.Allocate(imagepixelrect, mirror_, rotation_);

    }
  }

  if (makecurrent)
  {
    videowidget_->doneCurrent();

  }

  videowidget_->update();
}

void View::AddCacheImage(ImageBuffer& imagebuffer)
{
  cache_.AddImage(imagebuffer);

}

QRect View::GetPixelRect() const
{
  const QRectF rect = GetOpenglRect(rect_.x(), rect_.y(), rect_.width(), rect_.height());
  return QRect(QPoint(((rect.x() + 1.0f) / 2.0f) * videowidget_->width(), videowidget_->height() - (((rect.y() + 1.0f) / 2.0f) * videowidget_->height())), QPoint(((rect.right() + 1.0f) / 2.0f) * videowidget_->width(), videowidget_->height() - (((rect.bottom() + 1.0f) / 2.0f) * videowidget_->height()))); // Move the origin, normalise and invert the y axis between 0.0 and 1.0f
}

QRectF View::GetImageRect() const
{
  const QRectF rect = GetOpenglRect(rect_.x(), rect_.y(), rect_.width(), rect_.height());
  float aspectratio = GetAspectRatio();
  if ((aspectratio == 0.0) || stretch_) // If we don't have the aspect ratio of the video yet, we can't place the black bars yet
  {
    
    return rect;
  }
  else // Maintain aspect ratio
  {
    const float frameaspectratio = static_cast<float>(videowidget_->width() / static_cast<float>(videowidget_->GetWidth())) / static_cast<float>(videowidget_->height() / static_cast<float>(videowidget_->GetHeight()));
    if ((rotation_ == ROTATION::_0) || (rotation_ == ROTATION::_180))
    {
      if (aspectratio > frameaspectratio) // Black bars at top and bottom
      {
        const float blackbarheight = (rect.height() - ((frameaspectratio / aspectratio) * rect.height())) * 0.5f;
        return QRectF(QPointF(rect.left(), rect.top() + blackbarheight), QPointF(rect.right(), rect.bottom() - blackbarheight));
      }
      else // Black bars on left and right
      {
        const float blackbarwidth = (rect.width() - ((aspectratio / frameaspectratio) * rect.width())) * 0.5f;
        return QRectF(QPointF(rect.left() + blackbarwidth, rect.top()), QPointF(rect.right() - blackbarwidth, rect.bottom()));
      }
    }
    else // ((rotation == ROTATION::_90) || (rotation == ROTATION::_270))
    {
      aspectratio = 1.0f / aspectratio;
      if (aspectratio > frameaspectratio) // Black bars at top and bottom
      {
        const float blackbarheight = (rect.height() - ((frameaspectratio / aspectratio) * rect.height())) * 0.5f;
        return QRectF(QPointF(rect.left(), rect.top() + blackbarheight), QPointF(rect.right(), rect.bottom() - blackbarheight));
      }
      else // Black bars on left and right
      {
        const float blackbarwidth = (rect.width() - ((aspectratio / frameaspectratio) * rect.width())) * 0.5f;
        return QRectF(QPointF(rect.left() + blackbarwidth, rect.top()), QPointF(rect.right() - blackbarwidth, rect.bottom()));
      }
    }
  }
}

QRect View::GetImagePixelRect() const
{
  const QRectF imagerect = GetImageRect();
  return QRect(QPoint(((imagerect.left() + 1.0f) / 2.0f) * videowidget_->width(), (1.0f - ((imagerect.top() + 1.0f) / 2.0f)) * videowidget_->height()), QPoint(((imagerect.right() + 1.0f) / 2.0f) * videowidget_->width(), (1.0f - ((imagerect.bottom() + 1.0f) / 2.0f)) * videowidget_->height()));
}

QRectF View::GetImagePixelRectF() const
{
  const QRectF imagerect = GetImageRect();
  return QRectF(QPointF(((imagerect.left() + 1.0f) / 2.0f), (1.0f - (imagerect.top() + 1.0f) / 2.0f)), QPointF(((imagerect.right() + 1.0f) / 2.0f), (1.0f - (imagerect.bottom() + 1.0f) / 2.0f)));
}

QImage View::GetQImage(const boost::optional<QRect>& rect) const
{
  int x;
  int y;
  int width;
  int height;
  if (rect.is_initialized())
  {
    x = rect->x();
    y = rect->y();
    width = rect->width();
    height = rect->height();
  }
  else
  {
    x = 0;
    y = 0;
    width = imagewidth_;
    height = imageheight_;
  }

  QImage image;
  videowidget_->makeCurrent();
  if (type_ == IMAGEBUFFERTYPE_RGBA)
  {
    GLuint framebuffer = 0;
    videowidget_->glGenFramebuffers(1, &framebuffer);
    videowidget_->glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    videowidget_->glBindTexture(GL_TEXTURE_2D, textures_.at(0));
    videowidget_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures_.at(0), 0);
    videowidget_->glViewport(0, 0, imagewidth_, imageheight_);
    std::unique_ptr<unsigned char[]> rgb = std::make_unique<unsigned char[]>(imagewidth_ * imageheight_ * 4);
    videowidget_->glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, rgb.get());
    videowidget_->glBindTexture(GL_TEXTURE_2D, 0);
    videowidget_->glBindFramebuffer(GL_FRAMEBUFFER, videowidget_->defaultFramebufferObject());
    videowidget_->glViewport(0, 0, videowidget_->width(), videowidget_->height());
    videowidget_->glDeleteFramebuffers(1, &framebuffer);

    image = QImage(rgb.get(), width, height, QImage::Format_RGBA8888);
  }
  else if (type_ == IMAGEBUFFERTYPE_YUV)
  {
    GLuint framebuffer = 0;
    videowidget_->glGenFramebuffers(1, &framebuffer);
    videowidget_->glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    std::array<std::unique_ptr<unsigned char[]>, 3> yuv;
    const std::array<GLsizei, 3> xs =
    {
      x,
      x / 2,
      x / 2
    };
    const std::array<GLsizei, 3> ys =
    {
      y,
      y / 2,
      y / 2
    };
    const std::array<GLsizei, 3> widths =
    {
      width,
      std::max(1, width / 2),
      std::max(1, width / 2)
    };
    const std::array<GLsizei, 3> heights =
    {
      height,
      std::max(1, height / 2),
      std::max(1, height / 2)
    };
    for (int i = 0; i < 3; ++i)
    {
      videowidget_->glBindTexture(GL_TEXTURE_2D, textures_.at(i));
      videowidget_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures_.at(i), 0);
      videowidget_->glViewport(0, 0, widths.at(i), heights.at(i));
      yuv.at(i) = std::make_unique<unsigned char[]>(widths.at(i) * heights.at(i));
      videowidget_->glReadPixels(xs.at(i), ys.at(i), widths.at(i), heights.at(i), GL_LUMINANCE, GL_UNSIGNED_BYTE, yuv.at(i).get());
      videowidget_->glBindTexture(GL_TEXTURE_2D, 0);
    }
    videowidget_->glBindFramebuffer(GL_FRAMEBUFFER, videowidget_->defaultFramebufferObject());
    videowidget_->glViewport(0, 0, videowidget_->width(), videowidget_->height());
    videowidget_->glDeleteFramebuffers(1, &framebuffer);

    image = QImage(width, height, QImage::Format_RGB888);
    for (int i = 0; i < width; ++i)
    {
      for (int j = 0; j < height; ++j)
      {
        const unsigned char yy = yuv.at(0).get()[(j * width) + i];
        const unsigned char uu = yuv.at(1).get()[((j / 2) * (width / 2)) + (i / 2)];
        const unsigned char vv = yuv.at(2).get()[((j / 2) * (width / 2)) + (i / 2)];
        const int r = (1.164f * (yy - 16)) + (1.596f * (vv - 128));
        const int g = (1.164f * (yy - 16)) - (0.813f * (vv - 128) + 0.391f * (uu - 128));
        const int b = (1.164f * (yy - 16)) + (2.018f * (uu - 128));
        image.setPixelColor(i, j, QColor(std::max(0, std::min(255, r)), std::max(0, std::min(255, g)), std::max(0, std::min(255, b))));
      }
    }
  }
  else if (type_ == IMAGEBUFFERTYPE_NV12)
  {
    GLuint framebuffer = 0;
    videowidget_->glGenFramebuffers(1, &framebuffer);
    videowidget_->glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    std::array<std::unique_ptr<unsigned char[]>, 2> nv;
    const std::array<GLsizei, 2> xs =
    {
      x,
      x / 2
    };
    const std::array<GLsizei, 2> ys =
    {
      y,
      y / 4
    };
    const std::array<GLsizei, 2> widths =
    {
      width,
      std::max(1, width / 2)
    };
    const std::array<GLsizei, 2> heights =
    {
      height,
      std::max(1, height / 2)
    };
    for (int i = 0; i < 2; ++i)
    {
      videowidget_->glBindTexture(GL_TEXTURE_2D, textures_.at(i));
      videowidget_->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures_.at(i), 0);
      videowidget_->glViewport(x, y, widths.at(i), heights.at(i));
      nv.at(i) = std::make_unique<unsigned char[]>(widths.at(i) * heights.at(i) * 8);
      videowidget_->glReadPixels(xs.at(i), ys.at(i), widths.at(i), heights.at(i), (i == 0) ? GL_RED : GL_RG, GL_UNSIGNED_BYTE, nv.at(i).get());
      videowidget_->glBindTexture(GL_TEXTURE_2D, 0);
    }

    image = QImage(width, height, QImage::Format_RGB888);
    for (int i = 0; i < width; ++i)
    {
      for (int j = 0; j < height; ++j)
      {
        const float y = static_cast<float>(nv.at(0).get()[(j * width) + i]);
        const float u = static_cast<float>(nv.at(1).get()[((j / 2) * width) + ((i / 2) * 2)]) - 128.0f;
        const float v = static_cast<float>(nv.at(1).get()[((j / 2) * width) + ((i / 2) * 2) + 1]) - 128.0f;

        const int r = y + (1.13983f * v);
        const int g = y - ((0.39465f * u) + (0.58060f * v));
        const int b = y + (2.03211f * u);
        image.setPixelColor(i, j, QColor(std::max(0, std::min(255, r)), std::max(0, std::min(255, g)), std::max(0, std::min(255, b))));
      }
    }
  }
  videowidget_->doneCurrent();
  return image;
}

QColor View::GetPixelColour(const int x, const int y) const
{
  const QImage pixel = GetQImage(QRect(x, y, 1, 1));
  if ((pixel.width() != 1) || (pixel.height() != 1))
  {

    return QColor(); // Shouldn't happen, but lets be careful
  }

  return QColor(pixel.pixel(0, 0));
}

void View::SetPaused(const bool paused)
{
  ++playrequestindex_;
  cache_.Clear();
  paused_ = paused;
}

void View::timerEvent(QTimerEvent* event)
{

}

QString View::HTMLColour(const int r, const int g, const int b)
{
  QString red = QString::number(r, 16).toUpper();
  if (red.size() == 1)
  {
    red = QString("0") + red;

  }
  QString green = QString::number(g, 16).toUpper();
  if (green.size() == 1)
  {
    green = QString("0") + green;

  }
  QString blue = QString::number(b, 16).toUpper();
  if (blue.size() == 1)
  {
    blue = QString("0") + blue;

  }
  return (red + green + blue);
}

QString View::FontText(const QVector4D& colour, const QString& text)
{
  return (QString("<b><font color=#") + HTMLColour(colour.x() * 255, colour.y() * 255, colour.z() * 255) + ">" + text + "</font></b>");
}

QRectF View::GetOpenglRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height) const
{
  const float pixelwidth = 1.0f / videowidget_->width();
  const float pixelheight = 1.0f / videowidget_->height();

  float left = (x * (2.0f / videowidget_->GetWidth())) - 1.0f;
  float right = ((x + width) * (2.0f / videowidget_->GetWidth())) - 1.0f;
  float top = ((videowidget_->GetHeight() - y) * (2.0f / videowidget_->GetHeight())) - 1.0f;
  float bottom = ((videowidget_->GetHeight() - y - height) * (2.0f / videowidget_->GetHeight())) - 1.0f;

  if (x > 0)
  {
    left += pixelwidth;

  }

  if ((x + width) < videowidget_->GetWidth())
  {
    right -= pixelwidth;

  }
  
  if (y > 0)
  {
    top -= pixelheight;

  }

  if ((y + height) < videowidget_->GetHeight())
  {
    bottom += pixelheight;

  }

  return QRectF(QPointF(left, top), QPointF(right, bottom));
}

uint64_t View::GetNextPlayRequestIndex(const bool clearcache)
{
  if (clearcache)
  {
    cache_.Clear();

  }
  return ++playrequestindex_;
}

void View::SetMessage(const uint64_t playrequestindex, bool error, const QString& text)
{
  ImageBuffer imagebuffer;
  imagebuffer.playrequestindex_ = playrequestindex;
  imagebuffer.type_ = IMAGEBUFFERTYPE_TEXT;
  imagebuffer.digitallysigned_.reset();

  // Allocate space the the text image
  const int imagewidth = 320;
  const int imageheight = 240;
  imagebuffer.widths_[0] = imagewidth;
  imagebuffer.heights_[0] = imageheight;
  imagebuffer.buffer_ = new uint8_t[imagewidth * imageheight * 4];
  imagebuffer.data_[0] = imagebuffer.buffer_;
    
  QImage texture(imagebuffer.data_[0], imagebuffer.widths_[0], imagebuffer.heights_[0], QImage::Format_RGBA8888);
  QPainter painter(&texture);
  texture.fill(QColor(0, 0, 0));
    
  int x = 10;
  int y = 10;
  for (int i = 0; i < text.size(); ++i)
  {
    if (FT_Load_Char(arial_, text.at(i).toLatin1(), FT_LOAD_RENDER))
    {

      continue; // Ignore errors
    }

    QImage character = QImage(arial_->glyph->bitmap.buffer, arial_->glyph->bitmap.width, arial_->glyph->bitmap.rows, arial_->glyph->bitmap.pitch, QImage::Format_Grayscale8).convertToFormat(QImage::Format_RGBA8888);
    if (error)
    {
      // Clear out green and blue
      for (int x = 0; x < character.width(); ++x)
      {
        for (int y = 0; y < character.height(); ++y)
        {
          character.setPixel(x, y, QColor(QColor(character.pixel(x, y)).red(), 0, 0).rgba());

        }
      }
    }
    else
    {
      // Clear out red and blue
      for (int x = 0; x < character.width(); ++x)
      {
        for (int y = 0; y < character.height(); ++y)
        {
          character.setPixel(x, y, QColor(0, QColor(character.pixel(x, y)).green(), 0).rgba());

        }
      }
    }

    if (static_cast<int>((x + arial_->glyph->bitmap.width)) > (imagebuffer.widths_[0] - 10))
    {
      x = 10;
      y += 20;
    }

    painter.drawImage(QRectF(x + arial_->glyph->bitmap_left, y + 16 - arial_->glyph->bitmap_top, arial_->glyph->bitmap.width, arial_->glyph->bitmap.rows), character);
    x += arial_->glyph->advance.x >> 6;
  }
    
  if (!imagequeue_.push(imagebuffer))
  {
    imagebuffer.Destroy();
  
  }
}

std::array<float, 12> View::GetVertices(const QRectF& rect, const ROTATION rotation, const bool mirror) const
{
  if (mirror)
  {
    switch (rotation)
    {
      case ROTATION::_90:
      {
        return std::array<float, 12>(
        {{
          static_cast<float>(rect.left()), static_cast<float>(rect.top()), 0.0f,
          static_cast<float>(rect.right()), static_cast<float>(rect.top()), 0.0f,
          static_cast<float>(rect.left()), static_cast<float>(rect.bottom()), 0.0f,
          static_cast<float>(rect.right()), static_cast<float>(rect.bottom()), 0.0f
        }});
      }
      case ROTATION::_180:
      {
        return std::array<float, 12>(
        {{
          static_cast<float>(rect.right()), static_cast<float>(rect.top()), 0.0f,
          static_cast<float>(rect.right()), static_cast<float>(rect.bottom()), 0.0f,
          static_cast<float>(rect.left()), static_cast<float>(rect.top()), 0.0f,
          static_cast<float>(rect.left()), static_cast<float>(rect.bottom()), 0.0f
        }});
      }
      case ROTATION::_270:
      {
        return std::array<float, 12>(
        {{
          static_cast<float>(rect.right()), static_cast<float>(rect.bottom()), 0.0f,
          static_cast<float>(rect.left()), static_cast<float>(rect.bottom()), 0.0f,
          static_cast<float>(rect.right()), static_cast<float>(rect.top()), 0.0f,
          static_cast<float>(rect.left()), static_cast<float>(rect.top()), 0.0f
        }});
      }
      case ROTATION::_0:
      default:
      {
        return std::array<float, 12>(
        {{
          static_cast<float>(rect.left()), static_cast<float>(rect.bottom()), 0.0f,
          static_cast<float>(rect.left()), static_cast<float>(rect.top()), 0.0f,
          static_cast<float>(rect.right()), static_cast<float>(rect.bottom()), 0.0f,
          static_cast<float>(rect.right()), static_cast<float>(rect.top()), 0.0f
        }});
      }
    }
  }
  else
  {
    switch (rotation)
    {
      case ROTATION::_90:
      {
        return std::array<float, 12>(
        {{
          static_cast<float>(rect.left()), static_cast<float>(rect.bottom()), 0.0f,
          static_cast<float>(rect.right()), static_cast<float>(rect.bottom()), 0.0f,
          static_cast<float>(rect.left()), static_cast<float>(rect.top()), 0.0f,
          static_cast<float>(rect.right()), static_cast<float>(rect.top()), 0.0f
        }});
      }
      case ROTATION::_180:
      {
        return std::array<float, 12>(
        {{
          static_cast<float>(rect.left()), static_cast<float>(rect.top()), 0.0f,
          static_cast<float>(rect.left()), static_cast<float>(rect.bottom()), 0.0f,
          static_cast<float>(rect.right()), static_cast<float>(rect.top()), 0.0f,
          static_cast<float>(rect.right()), static_cast<float>(rect.bottom()), 0.0f
        }});
      }
      case ROTATION::_270:
      {
        return std::array<float, 12>(
        {{
          static_cast<float>(rect.right()), static_cast<float>(rect.top()), 0.0f,
          static_cast<float>(rect.left()), static_cast<float>(rect.top()), 0.0f,
          static_cast<float>(rect.right()), static_cast<float>(rect.bottom()), 0.0f,
          static_cast<float>(rect.left()), static_cast<float>(rect.bottom()), 0.0f
        }});
      }
      case ROTATION::_0:
      default:
      {
        return std::array<float, 12>(
        {{
          static_cast<float>(rect.right()), static_cast<float>(rect.bottom()), 0.0f,
          static_cast<float>(rect.right()), static_cast<float>(rect.top()), 0.0f,
          static_cast<float>(rect.left()), static_cast<float>(rect.bottom()), 0.0f,
          static_cast<float>(rect.left()), static_cast<float>(rect.top()), 0.0f
        }});
      }
    }
  }
}

void View::WriteFrame(const ImageBuffer& imagebuffer)
{
  time_ = imagebuffer.time_;
  playmarkertime_ = imagebuffer.time_;
  frametime_ = std::chrono::steady_clock::now();
  type_ = imagebuffer.type_;
  sequencenum_ = imagebuffer.sequencenum_;
  digitallysigned_ = imagebuffer.digitallysigned_;

  videowidget_->makeCurrent();
  if ((imagebuffer.type_ == IMAGEBUFFERTYPE_TEXT) || (imagebuffer.type_ == IMAGEBUFFERTYPE_RGBA))
  {
    videowidget_->glActiveTexture(GL_TEXTURE0);
    videowidget_->glBindTexture(GL_TEXTURE_2D, textures_.at(0));
    videowidget_->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagebuffer.widths_[0], imagebuffer.heights_[0], 0, GL_RGBA, GL_UNSIGNED_BYTE, imagebuffer.data_[0]);
    videowidget_->glBindTexture(GL_TEXTURE_2D, 0);
  }
  else if (imagebuffer.type_ == IMAGEBUFFERTYPE_YUV)
  {
    for (GLuint texture = 0; texture < 3; ++texture)
    {
      videowidget_->glActiveTexture(GL_TEXTURE0 + texture);
      videowidget_->glBindTexture(GL_TEXTURE_2D, textures_.at(texture));
      videowidget_->glPixelStorei(GL_UNPACK_ROW_LENGTH, imagebuffer.strides_[texture]);
      videowidget_->glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, imagebuffer.widths_[texture], imagebuffer.heights_[texture], 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imagebuffer.data_[texture]);
      videowidget_->glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
      videowidget_->glBindTexture(GL_TEXTURE_2D, 0);
    }
  }
  else if (imagebuffer.type_ == IMAGEBUFFERTYPE_NV12)
  {
    cuCtxPushCurrent_v2(imagebuffer.cudacontext_);
    bool resetresources = false; // Do we need to reinitialise the cuda stuff if dimensions and format have changed
    if ((imagebuffer.type_ != type_) || (imagebuffer.widths_[0] != imagewidth_) || (imagebuffer.heights_[0] != imageheight_) || !cudaresources_[0] || !cudaresources_[1])
    {
      // Destroy any old CUDA stuff we had laying around
      for (CUgraphicsResource& resource : cudaresources_)
      {
        if (resource)
        {
          cuGraphicsUnregisterResource(resource);
          resource = nullptr;
        }
      }
      resetresources = true;
    }

    for (GLuint texture = 0; texture < 2; ++texture)
    {
      videowidget_->glActiveTexture(GL_TEXTURE0 + texture);
      videowidget_->glPixelStorei(GL_UNPACK_ROW_LENGTH, imagebuffer.strides_[texture]);
      videowidget_->glBindTexture(GL_TEXTURE_2D, textures_.at(texture));

      CUgraphicsResource resource = nullptr;
      if (resetresources)
      {
        videowidget_->glTexImage2D(GL_TEXTURE_2D, 0, (texture == 0) ? GL_RED : GL_RG, imagebuffer.widths_[texture], imagebuffer.heights_[texture], 0, (texture == 0) ? GL_RED : GL_RG, GL_UNSIGNED_BYTE, nullptr);
        cuGraphicsGLRegisterImage(&resource, textures_[texture], GL_TEXTURE_2D, CU_GRAPHICS_REGISTER_FLAGS_WRITE_DISCARD);
        cudaresources_[texture] = resource;
      }
      else
      {
        resource = cudaresources_[texture];

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

      videowidget_->glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
      videowidget_->glBindTexture(GL_TEXTURE_2D, 0);
    }

    CUcontext dummy;
    cuCtxPopCurrent_v2(&dummy);
  }
  videowidget_->doneCurrent();
  videowidget_->update();
}

void View::UpdateObjects(const monocle::Objects* objects, const uint64_t time)
{
  const QRectF imagepixelrect = GetImagePixelRectF();
  for (const monocle::Object* object : *objects->objects())
  {
    Object o(object->id(), object->classid(), time, object->x(), object->y(), object->width(), object->height());
    if (static_cast<size_t>(o.classid_) > static_cast<size_t>(monocle::ObjectClass::Suitcase))
    {
      // Ignore unknown types
      continue;
    }
    o.text_.setPerformanceHint(QStaticText::PerformanceHint::AggressiveCaching);
    o.text_.setTextFormat(Qt::TextFormat::RichText);
    o.vertexbuffer_.create();
    o.vertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    o.Allocate(imagepixelrect, mirror_, rotation_);

    auto i = std::find_if(objects_.begin(), objects_.end(), [object](const std::pair< const std::pair<monocle::ObjectClass, uint64_t>, const std::vector<Object>& >& o) { return ((o.first.first == object->classid()) && (o.first.second == object->id())); });
    if (i == objects_.end())
    {
      o.text_.setText(FontText(OBJECT_COLOURS[static_cast<size_t>(o.classid_)], QString(monocle::EnumNameObjectClass(object->classid())) + ": " + QString::number(o.id_)));
      std::vector<Object> objects;
      objects.emplace_back(std::move(o));
      objects_.insert({ std::make_pair(object->classid(), object->id()), std::move(objects) });
    }
    else
    {
      o.text_.setText(FontText(OBJECT_COLOURS[static_cast<size_t>(o.classid_)], QString(monocle::EnumNameObjectClass(object->classid())) + ": " + QString::number(o.id_)));
      auto j = std::find_if(i->second.begin(), i->second.end(), [time](const Object& object) { return (object.time_ == time); });
      if (j == i->second.end())
      {
        utility::EmplaceSorted(i->second, std::move(o), [](const Object& lhs, const Object& rhs) { return (lhs.time_ < rhs.time_); });

      }
      else
      {
        *j = std::move(o);

      }

      // Clear up any old objects while we're here...
      const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
      i->second.erase(std::remove_if(i->second.begin(), i->second.end(), [this, now](const Object& object) { return ((time_ > object.time_) && ((time_ - object.time_) > OBJECT_KILL_DELAY) && ((now - object.age_) > OBJECT_KILL_AGE)); }), i->second.end());
      if (i->second.empty())
      {
        objects_.erase(i);
      
      }
    }
  }
}

void View::SaveImage(bool)
{
  if ((type_ != IMAGEBUFFERTYPE_RGBA) && (type_ != IMAGEBUFFERTYPE_YUV) && (type_ != IMAGEBUFFERTYPE_NV12))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Invalid Image buffer type"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  const QString filename = QFileDialog::getSaveFileName(videowidget_, "Save Image", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "Bitmap (*.bmp)", nullptr);
  if (filename.isEmpty())
  {

    return;
  }

  const QImage image = GetQImage(boost::none);
  if (image.sizeInBytes() == 0)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to retrieve image"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (!image.save(filename, "bmp"))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to save image: ") + filename, QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }
}

void View::Copy(bool)
{
  const QImage image = GetQImage(boost::none);
  if (image.sizeInBytes() == 0)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to copy image"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  QApplication::clipboard()->setImage(image);
}

void View::Rotate0(bool)
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), ROTATION::_0, mirror_, stretch_, true);

}

void View::Rotate90(bool)
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), ROTATION::_90, mirror_, stretch_, true);

}

void View::Rotate180(bool)
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), ROTATION::_180, mirror_, stretch_, true);

}

void View::Rotate270(bool)
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), ROTATION::_270, mirror_, stretch_, true);

}

void View::ToggleMirror(bool)
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), rotation_, !mirror_, stretch_, true);

}

void View::ToggleStretch(bool)
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), rotation_, mirror_, !stretch_, true);

}

void View::ToggleShowInfo(bool)
{
  showinfo_ = !showinfo_;
  videowidget_->update();
}

void View::ToggleShowObjects(bool)
{
  showobjects_ = !showobjects_;
  videowidget_->update();
}

}
