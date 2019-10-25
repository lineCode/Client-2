// findmotionvideowidget.cpp
//

///// Includes /////

#include "monocleclient/findmotionvideowidget.h"

#include <cuda.h>
#include <cudaGL.h>

#include "monocleclient/decoder.h"
#include "monocleclient/findmotionwindow.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/shaders.h"

///// Namespaces /////

namespace client
{

const std::array<float, 8> FindMotionVideoWidget::texturecoords_ =
{
  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 1.0f,
  0.0f, 0.0f
};

///// Methods /////

FindMotionVideoWidget::FindMotionVideoWidget(QWidget* parent) :
  QOpenGLWidget(parent),
  actionrotate0_(new QAction(tr("Rotate 0"), this)),
  actionrotate90_(new QAction(tr("Rotate 90"), this)),
  actionrotate180_(new QAction(tr("Rotate 180"), this)),
  actionrotate270_(new QAction(tr("Rotate 270"), this)),
  actionmirror_(new QAction(tr("Mirror"), this)),
  actionstretch_(new QAction(tr("Stretch"), this)),
  playrequestindex_(0),
  paused_(false),
  type_(IMAGEBUFFERTYPE_INVALID),
  time_(0),
  playmarkertime_(0),
  sequencenum_(0),
  frametime_(std::chrono::steady_clock::time_point()),
  texturebuffer_(QOpenGLBuffer::VertexBuffer),
  vertexbuffer_(QOpenGLBuffer::VertexBuffer),
  textures_({ 0, 0, 0 }),
  cudaresources_({ nullptr, nullptr, nullptr }),
  state_(FINDMOTIONSTATE_SELECT)
{
  actionrotate0_->setCheckable(true);
  actionrotate90_->setCheckable(true);
  actionrotate180_->setCheckable(true);
  actionrotate270_->setCheckable(true);
  actionmirror_->setCheckable(true);
  actionmirror_->setChecked(GetFindMotionWindow()->mirror_);
  actionstretch_->setCheckable(true);
  actionstretch_->setChecked(GetFindMotionWindow()->stretch_);

  connect(actionrotate0_, &QAction::triggered, this, &FindMotionVideoWidget::Rotate0);
  connect(actionrotate90_, &QAction::triggered, this, &FindMotionVideoWidget::Rotate90);
  connect(actionrotate180_, &QAction::triggered, this, &FindMotionVideoWidget::Rotate180);
  connect(actionrotate270_, &QAction::triggered, this, &FindMotionVideoWidget::Rotate270);
  connect(actionmirror_, &QAction::triggered, this, &FindMotionVideoWidget::ToggleMirror);
  connect(actionstretch_, &QAction::triggered, this, &FindMotionVideoWidget::ToggleStretch);

  setCursor(Qt::CrossCursor);

  startTimer(std::chrono::milliseconds(40));
}

FindMotionVideoWidget::~FindMotionVideoWidget()
{
  unsetCursor();

  for (CUgraphicsResource& cudaresource : cudaresources_)
  {
    if (cudaresource)
    {
      cuGraphicsUnregisterResource(cudaresource);
      cudaresource = nullptr;
    }
  }

  imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { const_cast<ImageBuffer&>(imagebuffer).Destroy(); });
  cache_.Clear();
}

FindMotionWindow* FindMotionVideoWidget::GetFindMotionWindow()
{

  return static_cast<FindMotionWindow*>(parent());
}

FindMotionWindow* FindMotionVideoWidget::GetFindMotionWindow() const
{

  return static_cast<FindMotionWindow*>(parent());
}

FindMotionPlaybackWidget* FindMotionVideoWidget::GetFindMotionPlaybackWidget()
{

  return static_cast<FindMotionWindow*>(parent())->GetPlaybackWidget();
}

FindMotionPlaybackWidget* FindMotionVideoWidget::GetFindMotionPlaybackWidget() const
{

  return static_cast<FindMotionWindow*>(parent())->GetPlaybackWidget();
}

uint64_t FindMotionVideoWidget::GetNextPlayRequestIndex()
{
  cache_.Clear();
  return ++playrequestindex_;
}

void FindMotionVideoWidget::SetImage(const QImage& image)
{
  const QImage tmp = image.convertToFormat(QImage::Format_RGBA8888);
  ImageBuffer imagebuffer;
  imagebuffer.type_ = IMAGEBUFFERTYPE_RGBA;
  imagebuffer.widths_[0] = tmp.width();
  imagebuffer.heights_[0] = tmp.height();
  imagebuffer.strides_[0] = tmp.bytesPerLine() / 4;
  const size_t size = tmp.bytesPerLine() * tmp.height();
  imagebuffer.buffer_ = new uint8_t[size];
  imagebuffer.data_[0] = imagebuffer.buffer_;
  memcpy(imagebuffer.buffer_, tmp.bits(), size);
  imagequeue_.push(imagebuffer);
}

void FindMotionVideoWidget::SetSelectedRect(const QRectF& selectedrect)
{
  selectedrect_ = selectedrect;
  update();
}

void FindMotionVideoWidget::SetPaused(const bool paused)
{
  ++playrequestindex_;
  cache_.Clear();
  paused_ = paused;
}

bool FindMotionVideoWidget::IsPaused() const
{

  return paused_;
}

void FindMotionVideoWidget::contextMenuEvent(QContextMenuEvent* event)
{
  QMenu menu;
  QMenu* rotation = new QMenu(QString("Rotation"), &menu);
  actionrotate0_->setChecked(GetFindMotionWindow()->rotation_ == ROTATION::_0);
  actionrotate90_->setChecked(GetFindMotionWindow()->rotation_ == ROTATION::_90);
  actionrotate180_->setChecked(GetFindMotionWindow()->rotation_ == ROTATION::_180);
  actionrotate270_->setChecked(GetFindMotionWindow()->rotation_ == ROTATION::_270);
  rotation->addActions({ actionrotate0_, actionrotate90_, actionrotate180_, actionrotate270_ });
  menu.addMenu(rotation);
  menu.addAction(actionmirror_);
  menu.addAction(actionstretch_);
  menu.exec(event->globalPos());
}

void FindMotionVideoWidget::initializeGL()
{
  initializeOpenGLFunctions();

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Textures
  glGenTextures(static_cast<GLsizei>(textures_.size()), &textures_.at(0));
  for (size_t i = 0; i < textures_.size(); ++i)
  {
    glBindTexture(GL_TEXTURE_2D, textures_.at(i));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  // Setup the texture buffer
  texturebuffer_.create();
  texturebuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  texturebuffer_.bind();
  texturebuffer_.allocate(texturecoords_.data(), static_cast<int>(texturecoords_.size() * sizeof(float)));
  texturebuffer_.release();

  // Setup the vertex buffers
  vertexbuffer_.create();
  vertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  SetPosition(GetFindMotionWindow()->rotation_, GetFindMotionWindow()->mirror_, GetFindMotionWindow()->stretch_, false);

  // RGB shader
  if (!viewrgbshader_.addShaderFromSourceCode(QOpenGLShader::Vertex, RGB_VERTEX_SHADER))
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!viewrgbshader_.addShaderFromSourceCode(QOpenGLShader::Fragment, RGB_PIXEL_SHADER))
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

  // YUV shader
  if (!viewyuvshader_.addShaderFromSourceCode(QOpenGLShader::Vertex, YUV_VERTEX_SHADER))
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!viewyuvshader_.addShaderFromSourceCode(QOpenGLShader::Fragment, YUV_PIXEL_SHADER))
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

  yuvtexturesamplerlocation_.at(0) = viewyuvshader_.uniformLocation("texY");
  if (yuvtexturesamplerlocation_.at(0) == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  yuvtexturesamplerlocation_.at(1) = viewyuvshader_.uniformLocation("texU");
  if (yuvtexturesamplerlocation_.at(1) == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  yuvtexturesamplerlocation_.at(2) = viewyuvshader_.uniformLocation("texV");
  if (yuvtexturesamplerlocation_.at(2) == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  // NV12 shader
  if (!viewnv12shader_.addShaderFromSourceCode(QOpenGLShader::Vertex, NV12_VERTEX_SHADER))
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::addShaderFromSourceCode failed"));
    return;
  }

  if (!viewnv12shader_.addShaderFromSourceCode(QOpenGLShader::Fragment, NV12_PIXEL_SHADER))
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

  nv12texturesamplerlocation_.at(0) = viewnv12shader_.uniformLocation("texY");
  if (nv12texturesamplerlocation_.at(0) == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  nv12texturesamplerlocation_.at(1) = viewnv12shader_.uniformLocation("texUV");
  if (nv12texturesamplerlocation_.at(1) == -1)
  {
    LOG_GUI_WARNING(QString("QOpenGLShaderProgram::uniformLocation failed"));
    return;
  }

  // Select Rect
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
}

void FindMotionVideoWidget::mouseMoveEvent(QMouseEvent* event)
{
  if (state_ == FINDMOTIONSTATE_DRAWING)
  {
    update();

  }
}

void FindMotionVideoWidget::mousePressEvent(QMouseEvent* event)
{
  state_ = FINDMOTIONSTATE_DRAWING;
  selectionpoint_ = event->pos();
}

void FindMotionVideoWidget::mouseReleaseEvent(QMouseEvent* event)
{
  if (state_ == FINDMOTIONSTATE_DRAWING)
  {
    state_ = FINDMOTIONSTATE_SELECT;
    
    const QRect imagepixelrect = GetImagePixelRect();
    const QRect selectedrect(QPoint(std::max(imagepixelrect.x(), std::min(selectionpoint_.x(), event->pos().x())) - imagepixelrect.x(),
                                    std::max(imagepixelrect.y(), std::min(selectionpoint_.y(), event->pos().y())) - imagepixelrect.y()),
                             QPoint(std::min(imagepixelrect.right(), std::max(selectionpoint_.x(), event->pos().x())) - imagepixelrect.x(), std::min(imagepixelrect.bottom(),
                                    std::max(selectionpoint_.y(), event->pos().y())) - imagepixelrect.y()));
    if ((selectedrect.width() < 4) || (selectedrect.height() < 4)) // If the selection is smaller than a few pixels, ignore it
    {

      return;
    }
    selectedrect_ = ImageToRect(imagepixelrect, selectedrect, GetFindMotionWindow()->mirror_, GetFindMotionWindow()->rotation_);
    update();
  }
}

void FindMotionVideoWidget::resizeGL(int width, int height)
{
  SetPosition(GetFindMotionWindow()->rotation_, GetFindMotionWindow()->mirror_, GetFindMotionWindow()->stretch_, true);

}

void FindMotionVideoWidget::paintGL()
{
  ImageBuffer imagebuffer;
  if (GetImage(imagebuffer))
  {
    if (paused_ || (imagebuffer.playrequestindex_ != playrequestindex_)) // Are we paused or getting frames from a request we don't care about, use the old frame
    {
      if (type_ == IMAGEBUFFERTYPE_RGBA)
      {
        viewrgbshader_.bind();
        viewrgbshader_.setUniformValue(rgbtexturesamplerlocation_, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures_[0]);
      }
      else if (type_ == IMAGEBUFFERTYPE_YUV)
      {
        viewyuvshader_.bind();
        for (GLuint texture = 0; texture < 3; ++texture)
        {
          viewyuvshader_.setUniformValue(yuvtexturesamplerlocation_.at(texture), texture);
          glActiveTexture(GL_TEXTURE0 + texture);
          glBindTexture(GL_TEXTURE_2D, textures_[texture]);
        }
      }
      else if (type_ == IMAGEBUFFERTYPE_NV12)
      {
        viewnv12shader_.bind();
        for (GLuint texture = 0; texture < 2; ++texture)
        {
          viewnv12shader_.setUniformValue(nv12texturesamplerlocation_.at(texture), texture);
          glActiveTexture(GL_TEXTURE0 + texture);
          glBindTexture(GL_TEXTURE_2D, textures_[texture]);
        }
      }

      if (imagebuffer.playrequestindex_ == playrequestindex_)
      {
        cache_.AddImage(imagebuffer); // Collect the frames because we might be stepping through them soon

      }
      else
      {
        freeimagequeue_.AddFreeImage(imagebuffer);

      }
    }
    else // New frame that we want to see
    {
      if ((type_ != imagebuffer.type_) || (type_ != imagebuffer.widths_[0]) || (type_ != imagebuffer.heights_[0]))
      {
        type_ = imagebuffer.type_;
        GetFindMotionWindow()->imagewidth_ = imagebuffer.widths_[0];
        GetFindMotionWindow()->imageheight_ = imagebuffer.heights_[0];
      }

      time_ = imagebuffer.time_;
      playmarkertime_ = std::max(imagebuffer.time_, playmarkertime_);
      frametime_ = std::chrono::steady_clock::now();

      if (imagebuffer.type_ == IMAGEBUFFERTYPE_RGBA)
      {
        viewrgbshader_.bind();
        viewrgbshader_.setUniformValue(rgbtexturesamplerlocation_, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures_[0]);
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
          glBindTexture(GL_TEXTURE_2D, textures_[texture]);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, imagebuffer.widths_.at(texture), imagebuffer.heights_.at(texture), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imagebuffer.data_[texture]);
          glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        }
      }
      else if (imagebuffer.type_ == IMAGEBUFFERTYPE_NV12)
      {
        cuCtxPushCurrent_v2(imagebuffer.cudacontext_);
        bool resetresources = false; // Do we need to reinitialise the cuda stuff if dimensions and format have changed
        if ((imagebuffer.type_ != type_) || (imagebuffer.widths_[0] != GetFindMotionWindow()->imagewidth_) || (imagebuffer.heights_[0] != GetFindMotionWindow()->imageheight_) || !cudaresources_[0] || !cudaresources_[1])
        {
          // Destroy any old CUDA stuff we had laying around
          if (GetFindMotionWindow()->cudacontext_)
          {
            for (CUgraphicsResource& resource : cudaresources_)
            {
              if (resource)
              {
                cuGraphicsUnregisterResource(resource);
                resource = nullptr;
              }
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
          glBindTexture(GL_TEXTURE_2D, textures_[texture]);

          CUgraphicsResource resource = nullptr;
          if (resetresources)
          {
            glTexImage2D(GL_TEXTURE_2D, 0, (texture == 0) ? GL_RED : GL_RG, imagebuffer.widths_.at(texture), imagebuffer.heights_[texture], 0, (texture == 0) ? GL_RED : GL_RG, GL_UNSIGNED_BYTE, nullptr);

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
          copy.WidthInBytes = (texture == 0) ? imagebuffer.widths_[texture] : imagebuffer.widths_.at(texture) * 2;
          copy.Height = imagebuffer.heights_[texture];
          cuMemcpy2D(&copy);
          cuGraphicsUnmapResources(1, &resource, 0);
          glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        }

        CUcontext dummy;
        cuCtxPopCurrent_v2(&dummy);
      }
      freeimagequeue_.AddFreeImage(imagebuffer);
    }
  }
  else
  {
    if (type_ == IMAGEBUFFERTYPE_RGBA)
    {
      viewrgbshader_.bind();
      viewrgbshader_.setUniformValue(rgbtexturesamplerlocation_, 0);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textures_[0]);
    }
    else if (type_ == IMAGEBUFFERTYPE_YUV)
    {
      viewyuvshader_.bind();
      for (GLuint texture = 0; texture < 3; ++texture)
      {
        viewyuvshader_.setUniformValue(yuvtexturesamplerlocation_.at(texture), texture);
        glActiveTexture(GL_TEXTURE0 + texture);
        glBindTexture(GL_TEXTURE_2D, textures_[texture]);
      }
    }
    else if (type_ == IMAGEBUFFERTYPE_NV12)
    {
      viewnv12shader_.bind();
      for (GLuint texture = 0; texture < 2; ++texture)
      {
        viewnv12shader_.setUniformValue(nv12texturesamplerlocation_.at(texture), texture);
        glActiveTexture(GL_TEXTURE0 + texture);
        glBindTexture(GL_TEXTURE_2D, textures_[texture]);
      }
    }
  }
  texturebuffer_.bind();

  // Vertices
  if (type_ == IMAGEBUFFERTYPE_RGBA)
  {
    viewrgbshader_.enableAttributeArray(rgbtexturecoordlocation_);
    viewrgbshader_.setAttributeBuffer(rgbtexturecoordlocation_, GL_FLOAT, 0, 2);
    vertexbuffer_.bind();
    viewrgbshader_.enableAttributeArray(rgbpositionlocation_);
    viewrgbshader_.setAttributeBuffer(rgbpositionlocation_, GL_FLOAT, 0, 3);
  }
  else if (type_ == IMAGEBUFFERTYPE_YUV)
  {
    viewyuvshader_.enableAttributeArray(yuvtexturecoordlocation_);
    viewyuvshader_.setAttributeBuffer(yuvtexturecoordlocation_, GL_FLOAT, 0, 2);

    vertexbuffer_.bind();
    viewyuvshader_.enableAttributeArray(yuvpositionlocation_);
    viewyuvshader_.setAttributeBuffer(yuvpositionlocation_, GL_FLOAT, 0, 3);
  }
  else if (type_ == IMAGEBUFFERTYPE_NV12)
  {
    viewnv12shader_.enableAttributeArray(nv12texturecoordlocation_);
    viewnv12shader_.setAttributeBuffer(nv12texturecoordlocation_, GL_FLOAT, 0, 2);

    vertexbuffer_.bind();
    viewnv12shader_.enableAttributeArray(nv12positionlocation_);
    viewnv12shader_.setAttributeBuffer(nv12positionlocation_, GL_FLOAT, 0, 3);
  }

  // Draw
  if ((type_ == IMAGEBUFFERTYPE_RGBA) || (type_ == IMAGEBUFFERTYPE_YUV) || (type_ == IMAGEBUFFERTYPE_NV12))
  {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  }
  
  // Release
  if (type_ == IMAGEBUFFERTYPE_RGBA)
  {
    viewrgbshader_.disableAttributeArray(rgbpositionlocation_);
    vertexbuffer_.release();
    viewrgbshader_.disableAttributeArray(rgbtexturecoordlocation_);
    texturebuffer_.release();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    viewrgbshader_.release();
  }
  else if (type_ == IMAGEBUFFERTYPE_YUV)
  {
    viewyuvshader_.disableAttributeArray(yuvpositionlocation_);
    vertexbuffer_.release();
    viewyuvshader_.disableAttributeArray(yuvtexturecoordlocation_);
    texturebuffer_.release();
    for (GLuint texture = 0; texture < 3; ++texture)
    {
      glActiveTexture(GL_TEXTURE0 + texture);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    viewyuvshader_.release();
  }
  else if (type_ == IMAGEBUFFERTYPE_NV12)
  {
    viewnv12shader_.disableAttributeArray(nv12positionlocation_);
    vertexbuffer_.release();
    viewnv12shader_.disableAttributeArray(nv12texturecoordlocation_);
    texturebuffer_.release();
    for (GLuint texture = 0; texture < 2; ++texture)
    {
      glActiveTexture(GL_TEXTURE0 + texture);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    viewnv12shader_.release();
  }

  // Selected rect
  viewselectedshader_.bind();

  QOpenGLBuffer vertexbuffer;
  vertexbuffer.create();
  vertexbuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vertexbuffer.bind();

  const QRectF imagerect = GetImagePixelRectF();
  const QPointF topleft = ImageRectToOpenGL(imagerect, GetFindMotionWindow()->mirror_, GetFindMotionWindow()->rotation_, selectedrect_.left(), selectedrect_.top());
  const QPointF bottomright = ImageRectToOpenGL(imagerect, GetFindMotionWindow()->mirror_, GetFindMotionWindow()->rotation_, selectedrect_.right(), selectedrect_.bottom());
  const QRectF rectf = QRectF(topleft, bottomright);

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
  glDrawArrays(GL_LINE_STRIP, 0, 5);
  viewselectedshader_.disableAttributeArray(selectedpositionlocation_);
  vertexbuffer.release();
  vertexbuffer.destroy();

  // Drawing rect
  if (state_ == FINDMOTIONSTATE_DRAWING)
  {
    // Calcuate Opengl coords for the new rect
    const QPoint cursor = mapFromGlobal(QCursor::pos());
    const QRect rect(QPoint(std::min(cursor.x(), selectionpoint_.x()), std::min(cursor.y(), selectionpoint_.y())), QPoint(std::max(cursor.x(), selectionpoint_.x()), std::max(cursor.y(), selectionpoint_.y())));
    const QRectF selectedrect(QPointF(((static_cast<float>(rect.x()) / static_cast<float>(width())) * 2.0f) - 1.0f,
                                        1.0f - ((static_cast<float>(rect.y()) / static_cast<float>(height())) * 2.0f)),
                                        QPointF(((static_cast<float>(rect.right()) / static_cast<float>(width())) * 2.0f) - 1.0f,
                                        1.0f - ((static_cast<float>(rect.bottom()) / static_cast<float>(height())) * 2.0f)));
    
    vertexbuffer.create();
    vertexbuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexbuffer.bind();
    const std::array<float, 10> selectvertices =
    {
      static_cast<float>(selectedrect.right()), static_cast<float>(selectedrect.bottom()),
      static_cast<float>(selectedrect.right()), static_cast<float>(selectedrect.top()),
      static_cast<float>(selectedrect.left()), static_cast<float>(selectedrect.top()),
      static_cast<float>(selectedrect.left()), static_cast<float>(selectedrect.bottom()),
      static_cast<float>(selectedrect.right()), static_cast<float>(selectedrect.bottom())
    };
    vertexbuffer.allocate(selectvertices.data(), static_cast<int>(selectvertices.size() * sizeof(float)));

    viewselectedshader_.enableAttributeArray(selectedpositionlocation_);
    viewselectedshader_.setAttributeBuffer(selectedpositionlocation_, GL_FLOAT, 0, 2);
    glDrawArrays(GL_LINE_STRIP, 0, 5);
    viewselectedshader_.disableAttributeArray(selectedpositionlocation_);
    vertexbuffer.release();
    vertexbuffer.destroy();
  }
  
  viewselectedshader_.release();
}

void FindMotionVideoWidget::timerEvent(QTimerEvent* event)
{
  update();

}

bool FindMotionVideoWidget::GetImage(ImageBuffer& imagebuffer)
{
  bool hasimage = false;
  ImageBuffer previmagebuffer;
  while (true)
  {
    if (imagequeue_.pop(imagebuffer))
    {
      // If we have skipped frames, we should place them back into the temporary list, or destroy them if there is no room
      if (previmagebuffer.buffer_ || previmagebuffer.cudacontext_)
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

      return hasimage;
    }
  }
}

void FindMotionVideoWidget::WriteFrame(const ImageBuffer& imagebuffer)
{
  time_ = imagebuffer.time_;
  playmarkertime_ = imagebuffer.time_;
  frametime_ = std::chrono::steady_clock::now();
  type_ = imagebuffer.type_;
  sequencenum_ = imagebuffer.sequencenum_;

  makeCurrent();
  WriteImageBuffer(this, type_, GetFindMotionWindow()->imagewidth_, GetFindMotionWindow()->imageheight_, imagebuffer, textures_, cudaresources_);
  doneCurrent();
  update();
}

QRectF FindMotionVideoWidget::GetImageRect() const
{
  const QRectF rect(-1.0f, 1.0f, 2.0f, -2.0f);
  float aspectratio = 0.0f;
  if (GetFindMotionWindow()->imagewidth_ && GetFindMotionWindow()->imageheight_)
  {
    aspectratio = static_cast<double>(GetFindMotionWindow()->imagewidth_) / static_cast<double>(GetFindMotionWindow()->imageheight_);

  }

  if ((aspectratio == 0.0) || GetFindMotionWindow()->stretch_) // If we don't have the aspect ratio of the video yet, we can't place the black bars yet
  {
    // Don't need to do anything...
    return rect;
  }
  else // Maintain aspect ratio
  {
    const float frameaspectratio = static_cast<float>(width()) / static_cast<float>(height());
    if ((GetFindMotionWindow()->rotation_ == ROTATION::_0) || (GetFindMotionWindow()->rotation_ == ROTATION::_180))
    {
      if (aspectratio > frameaspectratio) // Black bars at top and bottom
      {
        const float blackbarheight = (2.0f - ((frameaspectratio / aspectratio) * 2.0f)) * 0.5f;
        return QRectF(QPointF(rect.left(), rect.top() - blackbarheight), QPointF(rect.right(), rect.bottom() + blackbarheight));
      }
      else // Black bars on left and right
      {
        const float blackbarwidth = (2.0f - ((aspectratio / frameaspectratio) * 2.0f)) * 0.5f;
        return QRectF(QPointF(rect.left() + blackbarwidth, rect.top()), QPointF(rect.right() - blackbarwidth, rect.bottom()));
      }
    }
    else // ((rotation == ROTATION::_90) || (rotation == ROTATION::_270))
    {
      aspectratio = 1.0 / aspectratio;
      if (aspectratio > frameaspectratio) // Black bars at top and bottom
      {
        const float blackbarheight = (2.0f - ((frameaspectratio / aspectratio) * 2.0f)) * 0.5f;
        return QRectF(QPointF(rect.left(), rect.top() - blackbarheight), QPointF(rect.right(), rect.bottom() + blackbarheight));
      }
      else // Black bars on left and right
      {
        const float blackbarwidth = (2.0f - ((aspectratio / frameaspectratio) * 2.0f)) * 0.5f;
        return QRectF(QPointF(rect.left() + blackbarwidth, rect.top()), QPointF(rect.right() - blackbarwidth, rect.bottom()));
      }
    }
  }
}

QRect FindMotionVideoWidget::GetImagePixelRect() const
{
  const QRectF imagerect = GetImageRect();
  return QRect(QPoint(((imagerect.left() + 1.0f) / 2.0f) * width(), (1.0f - ((imagerect.top() + 1.0f) / 2.0f)) * height()), QPoint(((imagerect.right() + 1.0f) / 2.0f) * width(), (1.0f - ((imagerect.bottom() + 1.0f) / 2.0f)) * height()));
}

QRectF FindMotionVideoWidget::GetImagePixelRectF() const
{
  const QRectF imagerect = GetImageRect();
  return QRectF(QPointF(((imagerect.left() + 1.0f) / 2.0f), (1.0f - (imagerect.top() + 1.0f) / 2.0f)), QPointF(((imagerect.right() + 1.0f) / 2.0f), (1.0f - (imagerect.bottom() + 1.0f) / 2.0f)));
}

void FindMotionVideoWidget::SetPosition(const ROTATION rotation, const bool mirror, const bool stretch, const bool makecurrent)
{
  if (makecurrent)
  {
    makeCurrent();

  }

  GetFindMotionWindow()->rotation_ = rotation;
  GetFindMotionWindow()->mirror_ = mirror;
  GetFindMotionWindow()->stretch_ = stretch;

  const std::array<float, 12> vertices = GetVertices(GetImageRect(), rotation, mirror);
  vertexbuffer_.bind();
  vertexbuffer_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(float)));
  vertexbuffer_.release();

  if (makecurrent)
  {
    doneCurrent();

  }
}

void FindMotionVideoWidget::Rotate0(bool)
{
  SetPosition(ROTATION::_0, GetFindMotionWindow()->mirror_, GetFindMotionWindow()->stretch_, true);

}

void FindMotionVideoWidget::Rotate90(bool)
{
  SetPosition(ROTATION::_90, GetFindMotionWindow()->mirror_, GetFindMotionWindow()->stretch_, true);

}

void FindMotionVideoWidget::Rotate180(bool)
{
  SetPosition(ROTATION::_180, GetFindMotionWindow()->mirror_, GetFindMotionWindow()->stretch_, true);

}

void FindMotionVideoWidget::Rotate270(bool)
{
  SetPosition(ROTATION::_270, GetFindMotionWindow()->mirror_, GetFindMotionWindow()->stretch_, true);

}

void FindMotionVideoWidget::ToggleMirror(bool)
{
  SetPosition(GetFindMotionWindow()->rotation_, !GetFindMotionWindow()->mirror_, GetFindMotionWindow()->stretch_, true);
}

void FindMotionVideoWidget::ToggleStretch(bool)
{
  SetPosition(GetFindMotionWindow()->rotation_, GetFindMotionWindow()->mirror_, !GetFindMotionWindow()->stretch_, true);

}

}
