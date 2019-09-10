// videowidgetgrid.cpp
//

///// Includes /////

#include <boost/asio.hpp>

#include "monocleclient/videowidgetgrid.h"

#include "monocleclient/videoview.h"

///// Namespaces /////

namespace client
{

///// Methods /////

VideoWidgetGrid::VideoWidgetGrid() :
  shader_(nullptr),
  vertexbuffer_(QOpenGLBuffer::VertexBuffer),
  positionlocation_(-1),
  numindices_(0)
{

}

VideoWidgetGrid::~VideoWidgetGrid()
{
  Destroy();

}

int VideoWidgetGrid::Init()
{
  Destroy();

  // Shader
  shader_ = new QOpenGLShaderProgram();
  if (!shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,
    "#version 130\n"
    "in vec2 position;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(position, 1.0, 1.0);\n"
    "}\n"))
  {
    
    return 1;
  }

  if (!shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,
    "#version 130\n"
    "void main()\n"
    "{\n"
    "  gl_FragColor = vec4(0.0, 0.0, 1.0, 0.0);\n"
    "}\n"))
  {
    
    return 1;
  }

  positionlocation_ = 0;
  shader_->bindAttributeLocation("position", positionlocation_);
  if (!shader_->link())
  {
    
    return 1;
  }

  // Vertices
  if (!vertexbuffer_.create())
  {

    return 1;
  }
  vertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);

  // VAO
  if (!vao_.create())
  {

    return 1;
  }

  return 0;
}

void VideoWidgetGrid::Destroy()
{
  if (shader_)
  {
    delete shader_;
    shader_ = nullptr;
  }

  if (vao_.isCreated())
  {
    vao_.destroy();

  }

  if (vertexbuffer_.isCreated())
  {
    vertexbuffer_.destroy();

  }

  positionlocation_ = -1;
  numindices_ = 0;
}

void VideoWidgetGrid::Generate(const unsigned int width, const unsigned int height, const std::vector< QSharedPointer<View> >& views)
{
  vao_.bind();
  vertexbuffer_.bind();
  shader_->bind();
  shader_->enableAttributeArray(0);
  shader_->setAttributeBuffer(0, GL_FLOAT, 0, 2);

  std::vector<float> grid;
  grid.reserve(width * height * 4);

  // Vertical lines
  for (unsigned int x = 1; x < width; ++x)
  {
    for (unsigned int y = 0; y < height; ++y)
    {
      if (OverlapX(x, y, views))
      {

        continue;
      }

      grid.push_back((x * (2.0f / width)) - 1.0f);
      grid.push_back((y * (-2.0f / height)) + 1.0f);
      grid.push_back((x * (2.0f / width)) - 1.0f);
      grid.push_back(((y + 1) * (-2.0f / height)) + 1.0f);
    }
  }

  // Horizontal lines
  for (unsigned int x = 0; x < width; ++x)
  {
    for (unsigned int y = 1; y < height; ++y)
    {
      if (OverlapY(x, y, views))
      {

        continue;
      }

      grid.push_back((x * (2.0f / width)) - 1.0f);
      grid.push_back((y * (-2.0f / height)) + 1.0f);
      grid.push_back(((x + 1) * (2.0f / width)) - 1.0f);
      grid.push_back((y * (-2.0f / height)) + 1.0f);
    }
  }

  vertexbuffer_.allocate(grid.data(), static_cast<int>(grid.size() * sizeof(float)));
  numindices_ = static_cast<GLsizei>(grid.size() / 2);
  vertexbuffer_.release();
  shader_->release();
  vao_.release();

}

bool VideoWidgetGrid::OverlapX(const unsigned int x, const unsigned int y, const std::vector< QSharedPointer<View> >& views)
{
  for (const QSharedPointer<View>& view : views)
  {
    if (((view->GetRect().x() < static_cast<int>(x)) && (view->GetRect().x() + view->GetRect().width()) > static_cast<int>(x)) && ((view->GetRect().y() <= static_cast<int>(y)) && (view->GetRect().y() + view->GetRect().height()) > static_cast<int>(y)))
    {

      return true;
    }
  }
  return false;
}

bool VideoWidgetGrid::OverlapY(const unsigned int x, const unsigned int y, const std::vector< QSharedPointer<View> >& views)
{
  for (const QSharedPointer<View>& view : views)
  {
    if (((view->GetRect().x() <= static_cast<int>(x)) && (view->GetRect().x() + view->GetRect().width()) > static_cast<int>(x)) && ((view->GetRect().y() < static_cast<int>(y)) && (view->GetRect().y() + view->GetRect().height()) > static_cast<int>(y)))
    {

      return true;
    }
  }
  return false;
}

}
