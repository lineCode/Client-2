// findobjectvideowidget.cpp
//

///// Includes /////

#include "monocleclient/findobjectvideowidget.h"

#include <cuda.h>
#include <cudaGL.h>

#include "monocleclient/decoder.h"
#include "monocleclient/findobjectwindow.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/recording.h"
#include "monocleclient/shaders.h"

///// Namespaces /////

namespace client
{

const std::array<float, 8> FindObjectVideoWidget::texturecoords_ =
{
  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 1.0f,
  0.0f, 0.0f
};

///// Methods /////

FindObjectVideoWidget::FindObjectVideoWidget(QWidget* parent) :
  QOpenGLWidget(parent),
  actionrotate0_(new QAction(tr("Rotate 0"), this)),
  actionrotate90_(new QAction(tr("Rotate 90"), this)),
  actionrotate180_(new QAction(tr("Rotate 180"), this)),
  actionrotate270_(new QAction(tr("Rotate 270"), this)),
  actionmirror_(new QAction(tr("Mirror"), this)),
  actionstretch_(new QAction(tr("Stretch"), this)),
  actioninfo_(new QAction(tr("Info"), this)),
  actionobjects_(new QAction(tr("Objects"), this)),
  freetype_(nullptr),
  freetypearial_(nullptr),
  videoplayrequestindex_(0),
  metadataplayrequestindex_(0),
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
  infotexture_(0),
  infotime_(0),
  bandwidthsizes_(150),
  state_(FINDOBJECTSTATE_SELECT)
{
  actionrotate0_->setCheckable(true);
  actionrotate90_->setCheckable(true);
  actionrotate180_->setCheckable(true);
  actionrotate270_->setCheckable(true);
  actionmirror_->setCheckable(true);
  actionmirror_->setChecked(GetFindObjectWindow()->mirror_);
  actionstretch_->setCheckable(true);
  actionstretch_->setChecked(GetFindObjectWindow()->stretch_);
  actioninfo_->setCheckable(true);
  actioninfo_->setChecked(GetFindObjectWindow()->showinfo_);
  actionobjects_->setCheckable(true);
  actionobjects_->setChecked(GetFindObjectWindow()->showobjects_);

  connect(actionrotate0_, &QAction::triggered, this, &FindObjectVideoWidget::Rotate0);
  connect(actionrotate90_, &QAction::triggered, this, &FindObjectVideoWidget::Rotate90);
  connect(actionrotate180_, &QAction::triggered, this, &FindObjectVideoWidget::Rotate180);
  connect(actionrotate270_, &QAction::triggered, this, &FindObjectVideoWidget::Rotate270);
  connect(actionmirror_, &QAction::triggered, this, &FindObjectVideoWidget::ToggleMirror);
  connect(actionstretch_, &QAction::triggered, this, &FindObjectVideoWidget::ToggleStretch);
  connect(actioninfo_, &QAction::triggered, this, &FindObjectVideoWidget::ToggleInfo);
  connect(actionobjects_, &QAction::triggered, this, &FindObjectVideoWidget::ToggleObjects);

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

  if (FT_Set_Pixel_Sizes(freetypearial_, INFO_FONT_HEIGHT, 0))
  {
    LOG_GUI_WARNING(tr("Error initialising Arial resource"));
    return;
  }

  setCursor(Qt::CrossCursor);

  startTimer(std::chrono::milliseconds(40));
}

FindObjectVideoWidget::~FindObjectVideoWidget()
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

  // GL stuff
  makeCurrent();
  glDeleteTextures(3, textures_.data());
  textures_.fill(0);
  texturebuffer_.destroy();
  //TODO textvertexbuffer_.destroy();
  vertexbuffer_.destroy();
  glDeleteTextures(1, &infotexture_);
  infotexture_ = 0;
  infotexturebuffer_.destroy();
  infovertexbuffer_.destroy();
  doneCurrent();

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

}

FindObjectWindow* FindObjectVideoWidget::GetFindObjectWindow()
{

  return static_cast<FindObjectWindow*>(parent());
}

FindObjectWindow* FindObjectVideoWidget::GetFindObjectWindow() const
{

  return static_cast<FindObjectWindow*>(parent());
}

FindObjectPlaybackWidget* FindObjectVideoWidget::GetFindObjectPlaybackWidget()
{

  return static_cast<FindObjectWindow*>(parent())->GetPlaybackWidget();
}

FindObjectPlaybackWidget* FindObjectVideoWidget::GetFindObjectPlaybackWidget() const
{

  return static_cast<FindObjectWindow*>(parent())->GetPlaybackWidget();
}

uint64_t FindObjectVideoWidget::GetNextVideoPlayRequestIndex()
{
  cache_.Clear();
  return ++videoplayrequestindex_;
}

uint64_t FindObjectVideoWidget::GetNextMetadataPlayRequestIndex()
{
  return ++metadataplayrequestindex_;
}

void FindObjectVideoWidget::SetImage(const QImage& image)
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

void FindObjectVideoWidget::SetSelectedRect(const QRectF& selectedrect)
{
  selectedrect_ = selectedrect;
  update();
}

void FindObjectVideoWidget::SetPaused(const bool paused)
{
  ++videoplayrequestindex_;
  ++metadataplayrequestindex_;
  cache_.Clear();
  paused_ = paused;
}

bool FindObjectVideoWidget::IsPaused() const
{

  return paused_;
}

void FindObjectVideoWidget::contextMenuEvent(QContextMenuEvent* event)
{
  QMenu menu;
  QMenu* rotation = new QMenu(QString("Rotation"), &menu);
  actionrotate0_->setChecked(GetFindObjectWindow()->rotation_ == ROTATION::_0);
  actionrotate90_->setChecked(GetFindObjectWindow()->rotation_ == ROTATION::_90);
  actionrotate180_->setChecked(GetFindObjectWindow()->rotation_ == ROTATION::_180);
  actionrotate270_->setChecked(GetFindObjectWindow()->rotation_ == ROTATION::_270);
  rotation->addActions({ actionrotate0_, actionrotate90_, actionrotate180_, actionrotate270_ });
  menu.addMenu(rotation);
  menu.addAction(actionmirror_);
  menu.addAction(actionstretch_);
  menu.addAction(actioninfo_);
  menu.addAction(actionobjects_);
  menu.exec(event->globalPos());
}

void FindObjectVideoWidget::initializeGL()
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
  SetPosition(GetFindObjectWindow()->rotation_, GetFindObjectWindow()->mirror_, GetFindObjectWindow()->stretch_, false);

  // Info
  infotexturebuffer_.create();
  infotexturebuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  infotexturebuffer_.bind();
  infotexturebuffer_.allocate(texturecoords_.data(), static_cast<int>(texturecoords_.size() * sizeof(float)));
  infotexturebuffer_.release();

  glGenTextures(1, &infotexture_);
  glBindTexture(GL_TEXTURE_2D, infotexture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  infovertexbuffer_.create();
  infovertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);

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
  selectedcolourlocation_ = viewselectedshader_.attributeLocation("position");

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

}

void FindObjectVideoWidget::mouseMoveEvent(QMouseEvent* event)
{
  if (state_ == FINDOBJECTSTATE_DRAWING)
  {
    update();

  }
}

void FindObjectVideoWidget::mousePressEvent(QMouseEvent* event)
{
  state_ = FINDOBJECTSTATE_DRAWING;
  selectionpoint_ = event->pos();
}

void FindObjectVideoWidget::mouseReleaseEvent(QMouseEvent* event)
{
  if (state_ == FINDOBJECTSTATE_DRAWING)
  {
    state_ = FINDOBJECTSTATE_SELECT;
    
    const QRect imagepixelrect = GetImagePixelRect();
    const QRect selectedrect(QPoint(std::max(imagepixelrect.x(), std::min(selectionpoint_.x(), event->pos().x())) - imagepixelrect.x(),
                                    std::max(imagepixelrect.y(), std::min(selectionpoint_.y(), event->pos().y())) - imagepixelrect.y()),
                             QPoint(std::min(imagepixelrect.right(), std::max(selectionpoint_.x(), event->pos().x())) - imagepixelrect.x(), std::min(imagepixelrect.bottom(),
                                    std::max(selectionpoint_.y(), event->pos().y())) - imagepixelrect.y()));
    if ((selectedrect.width() < 4) || (selectedrect.height() < 4)) // If the selection is smaller than a few pixels, ignore it
    {

      return;
    }
    selectedrect_ = ImageToRect(imagepixelrect, selectedrect, GetFindObjectWindow()->mirror_, GetFindObjectWindow()->rotation_);
    update();
  }
}

void FindObjectVideoWidget::resizeGL(int width, int height)
{
  SetPosition(GetFindObjectWindow()->rotation_, GetFindObjectWindow()->mirror_, GetFindObjectWindow()->stretch_, true);

}

void FindObjectVideoWidget::paintGL()
{
  ImageBuffer imagebuffer;
  if (GetImage(imagebuffer))
  {
    if (paused_ || (imagebuffer.playrequestindex_ != videoplayrequestindex_)) // Are we paused or getting frames from a request we don't care about, use the old frame
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

      if (imagebuffer.playrequestindex_ == videoplayrequestindex_)
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
        GetFindObjectWindow()->imagewidth_ = imagebuffer.widths_[0];
        GetFindObjectWindow()->imageheight_ = imagebuffer.heights_[0];
      }

      codec_ = imagebuffer.codec_;
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
        if ((imagebuffer.type_ != type_) || (imagebuffer.widths_[0] != GetFindObjectWindow()->imagewidth_) || (imagebuffer.heights_[0] != GetFindObjectWindow()->imageheight_) || !cudaresources_[0] || !cudaresources_[1])
        {
          // Destroy any old CUDA stuff we had laying around
          if (GetFindObjectWindow()->cudacontext_)
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
  const QPointF topleft = ImageRectToOpenGL(imagerect, GetFindObjectWindow()->mirror_, GetFindObjectWindow()->rotation_, selectedrect_.left(), selectedrect_.top());
  const QPointF bottomright = ImageRectToOpenGL(imagerect, GetFindObjectWindow()->mirror_, GetFindObjectWindow()->rotation_, selectedrect_.right(), selectedrect_.bottom());
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
  viewselectedshader_.setUniformValue(selectedcolourlocation_, QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
  glDrawArrays(GL_LINE_STRIP, 0, 5);
  viewselectedshader_.disableAttributeArray(selectedpositionlocation_);
  vertexbuffer.release();
  vertexbuffer.destroy();

  // Drawing rect
  if (state_ == FINDOBJECTSTATE_DRAWING)
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
    viewselectedshader_.setUniformValue(selectedcolourlocation_, QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
    glDrawArrays(GL_LINE_STRIP, 0, 5);
    viewselectedshader_.disableAttributeArray(selectedpositionlocation_);
    vertexbuffer.release();
    vertexbuffer.destroy();
  }
  
  // Objects
  if (GetFindObjectWindow()->showobjects_ && ((type_ == IMAGEBUFFERTYPE_RGBA) || (type_ == IMAGEBUFFERTYPE_NV12) || (type_ == IMAGEBUFFERTYPE_YUV)))
  {
    const QRectF imagepixelrect = GetImagePixelRect();
    for (std::pair< const std::pair<monocle::ObjectClass, uint64_t>, std::vector<Object> >& objects : objects_.GetObjects())
    {
      auto object = std::find_if(objects.second.rbegin(), objects.second.rend(), [this](const Object& object) { return ((object.time_ <= time_) && ((time_ - object.time_) < OBJECT_KILL_DELAY)); });
      if (object == objects.second.rend())
      {

        continue;
      }

      if (GetFindObjectWindow()->Filter(object->classid_))
      {
        object->vertexbuffer_.bind();
        viewselectedshader_.enableAttributeArray(selectedpositionlocation_);
        viewselectedshader_.setAttributeBuffer(selectedpositionlocation_, GL_FLOAT, 0, 2);
        viewselectedshader_.setUniformValue(selectedcolourlocation_, OBJECT_COLOURS[static_cast<size_t>(object->classid_)]);
        glDrawArrays(GL_LINE_STRIP, 0, 5);
        viewselectedshader_.disableAttributeArray(selectedpositionlocation_);
        object->vertexbuffer_.release();
      }
    }
  }

  // Object text
  QPainter painter(this);
  if (GetFindObjectWindow()->showobjects_ && ((type_ == IMAGEBUFFERTYPE_RGBA) || (type_ == IMAGEBUFFERTYPE_NV12) || (type_ == IMAGEBUFFERTYPE_YUV)))
  {
    const QRectF imagepixelrect = GetImagePixelRectF();
    for (std::pair< const std::pair<monocle::ObjectClass, uint64_t>, std::vector<Object> >& objects : objects_.GetObjects())
    {
      auto object = std::find_if(objects.second.rbegin(), objects.second.rend(), [this](const Object& object) { return ((object.time_ <= time_) && ((time_ - object.time_) < OBJECT_KILL_DELAY)); });
      if (object == objects.second.rend())
      {
  
        continue;
      }

      if (GetFindObjectWindow()->Filter(object->classid_))
      {
        object->DrawObjectText(imagepixelrect, width(), height(), GetFindObjectWindow()->mirror_, GetFindObjectWindow()->rotation_, painter);

      }
    }
  }

  viewselectedshader_.release();

  // Info boxes
  ToInfoText(QDateTime::fromMSecsSinceEpoch(time_, Qt::UTC), Options::Instance().GetInfoTextFormat(), codec_, bandwidthsizes_, std::make_pair<const std::string&, const QString&>(std::string(), GetFindObjectWindow()->recording_->GetLocation()), std::make_pair<const std::string&, const QString&>(std::string(), GetFindObjectWindow()->recording_->GetName()), GetFindObjectWindow()->imagewidth_, GetFindObjectWindow()->imageheight_, infotextformatbuffer_);

  if (infotime_ != time_)
  {
    QImage texture(INFO_WIDTH, INFO_HEIGHT, QImage::Format_RGBA8888);
    QPainter painter(&texture);
    texture.fill(QColor(0, 0, 0));
    int x = INFO_BORDER;
    const int maxfontheight = static_cast<int>(INFO_FONT_HEIGHT * (static_cast<float>(freetypearial_->bbox.yMax) / static_cast<float>(freetypearial_->bbox.yMax - freetypearial_->bbox.yMin)));
    const int y = INFO_BORDER + (INFO_FONT_HEIGHT - maxfontheight) - 1;
    for (int i = 0; i < infotextformatbuffer_.size(); ++i)
    {
      if (infotextformatbuffer_.at(i) == '\0')
      {

        break;
      }

      if (FT_Load_Char(freetypearial_, infotextformatbuffer_.at(i), FT_LOAD_RENDER))
      {

        continue; // Ignore errors
      }

      const QImage character = QImage(freetypearial_->glyph->bitmap.buffer, freetypearial_->glyph->bitmap.width, freetypearial_->glyph->bitmap.rows, freetypearial_->glyph->bitmap.pitch, QImage::Format_Grayscale8);
      painter.drawImage(QRectF(x + freetypearial_->glyph->bitmap_left, y + (maxfontheight - freetypearial_->glyph->bitmap_top), freetypearial_->glyph->bitmap.width, freetypearial_->glyph->bitmap.rows), character);
      x += freetypearial_->glyph->advance.x >> 6;
    }
    x += INFO_BORDER; // End border

    // Set the alpha to chop off the erroneous end and fade the middle
    for (int i = 0; i < texture.height(); ++i)
    {
      QRgb* line = reinterpret_cast<QRgb*>(texture.scanLine(i));
      for (int j = 0; j < texture.width(); ++j)
      {
        int alpha = 0;
        if (j <= x)
        {
          alpha = 122;

        }
        line[j] = qRgba(qRed(line[j]), qGreen(line[j]), qBlue(line[j]), alpha);
      }
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, infotexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
    glBindTexture(GL_TEXTURE_2D, 0);

    infotime_ = time_;
  }

  // Draw Info
  if (((type_ == IMAGEBUFFERTYPE_NV12) || (type_ == IMAGEBUFFERTYPE_RGBA) || (type_ == IMAGEBUFFERTYPE_YUV)) && GetFindObjectWindow()->showinfo_)
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);

    viewinfoshader_.bind();
    viewinfoshader_.setUniformValue(infotexturesamplerlocation_, 0);
    glBindTexture(GL_TEXTURE_2D, infotexture_);

    infotexturebuffer_.bind();
    viewinfoshader_.enableAttributeArray(infotexturecoordlocation_);
    viewinfoshader_.setAttributeBuffer(infotexturecoordlocation_, GL_FLOAT, 0, 2);
    infovertexbuffer_.bind();
    viewinfoshader_.enableAttributeArray(infopositionlocation_);
    viewinfoshader_.setAttributeBuffer(infopositionlocation_, GL_FLOAT, 0, 3);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    viewinfoshader_.disableAttributeArray(infopositionlocation_);
    infovertexbuffer_.release();
    viewinfoshader_.disableAttributeArray(infotexturecoordlocation_);
    infotexturebuffer_.release();
    glBindTexture(GL_TEXTURE_2D, 0);
    viewinfoshader_.release();

    glDisable(GL_BLEND);
  }

}

void FindObjectVideoWidget::timerEvent(QTimerEvent* event)
{
  update();

}

bool FindObjectVideoWidget::GetImage(ImageBuffer& imagebuffer)
{
  bool hasimage = false;
  ImageBuffer previmagebuffer;
  while (true)
  {
    if (imagequeue_.pop(imagebuffer))
    {
      bandwidthsizes_.push_back(std::make_pair(std::chrono::steady_clock::now(), imagebuffer.originalsize_));

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

void FindObjectVideoWidget::WriteFrame(const ImageBuffer& imagebuffer)
{
  time_ = imagebuffer.time_;
  playmarkertime_ = imagebuffer.time_;
  frametime_ = std::chrono::steady_clock::now();
  type_ = imagebuffer.type_;
  sequencenum_ = imagebuffer.sequencenum_;

  makeCurrent();
  WriteImageBuffer(this, type_, GetFindObjectWindow()->imagewidth_, GetFindObjectWindow()->imageheight_, imagebuffer, textures_, cudaresources_);
  doneCurrent();
  update();
}

QRectF FindObjectVideoWidget::GetImageRect() const
{
  const QRectF rect(-1.0f, 1.0f, 2.0f, -2.0f);
  float aspectratio = 0.0f;
  if (GetFindObjectWindow()->imagewidth_ && GetFindObjectWindow()->imageheight_)
  {
    aspectratio = static_cast<double>(GetFindObjectWindow()->imagewidth_) / static_cast<double>(GetFindObjectWindow()->imageheight_);

  }

  if ((aspectratio == 0.0) || GetFindObjectWindow()->stretch_) // If we don't have the aspect ratio of the video yet, we can't place the black bars yet
  {
    // Don't need to do anything...
    return rect;
  }
  else // Maintain aspect ratio
  {
    const float frameaspectratio = static_cast<float>(width()) / static_cast<float>(height());
    if ((GetFindObjectWindow()->rotation_ == ROTATION::_0) || (GetFindObjectWindow()->rotation_ == ROTATION::_180))
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

QRect FindObjectVideoWidget::GetImagePixelRect() const
{
  const QRectF imagerect = GetImageRect();
  return QRect(QPoint(((imagerect.left() + 1.0f) / 2.0f) * width(), (1.0f - ((imagerect.top() + 1.0f) / 2.0f)) * height()), QPoint(((imagerect.right() + 1.0f) / 2.0f) * width(), (1.0f - ((imagerect.bottom() + 1.0f) / 2.0f)) * height()));
}

QRectF FindObjectVideoWidget::GetImagePixelRectF() const
{
  const QRectF imagerect = GetImageRect();
  return QRectF(QPointF(((imagerect.left() + 1.0f) / 2.0f), (1.0f - (imagerect.top() + 1.0f) / 2.0f)), QPointF(((imagerect.right() + 1.0f) / 2.0f), (1.0f - (imagerect.bottom() + 1.0f) / 2.0f)));
}

void FindObjectVideoWidget::SetPosition(const ROTATION rotation, const bool mirror, const bool stretch, const bool makecurrent)
{
  if (makecurrent)
  {
    makeCurrent();

  }

  GetFindObjectWindow()->rotation_ = rotation;
  GetFindObjectWindow()->mirror_ = mirror;
  GetFindObjectWindow()->stretch_ = stretch;

  const std::array<float, 12> vertices = GetVertices(GetImageRect(), rotation, mirror);
  vertexbuffer_.bind();
  vertexbuffer_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(float)));
  vertexbuffer_.release();

  objects_.Update(GetImagePixelRectF(), GetFindObjectWindow()->mirror_, GetFindObjectWindow()->rotation_);

  // Info
  const QRectF imagerect = GetImageRect();
  infovertexbuffer_.destroy();
  infovertexbuffer_.create();
  infovertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  infovertexbuffer_.bind();
  static const float heightpixels = 50.0f;
  const float infoheight = (2.0f / static_cast<float>(height())) * heightpixels;
  const float infowidth = (2.0f / static_cast<float>(width())) * (heightpixels / (static_cast<float>(INFO_HEIGHT) / static_cast<float>(INFO_WIDTH)));
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

  if (makecurrent)
  {
    doneCurrent();

  }
}

void FindObjectVideoWidget::Rotate0(bool)
{
  SetPosition(ROTATION::_0, GetFindObjectWindow()->mirror_, GetFindObjectWindow()->stretch_, true);

}

void FindObjectVideoWidget::Rotate90(bool)
{
  SetPosition(ROTATION::_90, GetFindObjectWindow()->mirror_, GetFindObjectWindow()->stretch_, true);

}

void FindObjectVideoWidget::Rotate180(bool)
{
  SetPosition(ROTATION::_180, GetFindObjectWindow()->mirror_, GetFindObjectWindow()->stretch_, true);

}

void FindObjectVideoWidget::Rotate270(bool)
{
  SetPosition(ROTATION::_270, GetFindObjectWindow()->mirror_, GetFindObjectWindow()->stretch_, true);

}

void FindObjectVideoWidget::ToggleMirror(bool)
{
  SetPosition(GetFindObjectWindow()->rotation_, !GetFindObjectWindow()->mirror_, GetFindObjectWindow()->stretch_, true);
}

void FindObjectVideoWidget::ToggleStretch(bool)
{
  SetPosition(GetFindObjectWindow()->rotation_, GetFindObjectWindow()->mirror_, !GetFindObjectWindow()->stretch_, true);

}

void FindObjectVideoWidget::ToggleInfo(bool)
{
  GetFindObjectWindow()->showinfo_ = !GetFindObjectWindow()->showinfo_;
  update();
}

void FindObjectVideoWidget::ToggleObjects(bool)
{
  GetFindObjectWindow()->showobjects_ = !GetFindObjectWindow()->showobjects_;
  update();
}

}
