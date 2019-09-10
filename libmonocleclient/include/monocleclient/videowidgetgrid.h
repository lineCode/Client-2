// videowidgetgrid.h
//

#ifndef IDG4K69QFWUO9HXHDP2JBJSU2J6RRYVBT4
#define IDG4K69QFWUO9HXHDP2JBJSU2J6RRYVBT4

///// Includes /////

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

///// Namespaces /////

namespace client
{

///// Declarations /////

class View;

///// Classes /////

class VideoWidgetGrid
{
 public:

  VideoWidgetGrid();
  ~VideoWidgetGrid();

  int Init();
  void Destroy();

  void Generate(const unsigned int width, const unsigned int height, const std::vector< QSharedPointer<View> >& views);

  inline QOpenGLShaderProgram* GetShader() { return shader_; }
  inline QOpenGLVertexArrayObject& GetVAO() { return vao_; }
  inline GLsizei GetNumIndices() const { return numindices_; }

 private:
  bool OverlapX(const unsigned int x, const unsigned int y, const std::vector< QSharedPointer<View> >& views);
  bool OverlapY(const unsigned int x, const unsigned int y, const std::vector< QSharedPointer<View> >& views);

  QOpenGLShaderProgram* shader_; // This is a pointer because QOpenGLShaderProgram does not provide a destruction method
  QOpenGLVertexArrayObject vao_;
  QOpenGLBuffer vertexbuffer_;

  int positionlocation_;

  GLsizei numindices_;

};

}

#endif
