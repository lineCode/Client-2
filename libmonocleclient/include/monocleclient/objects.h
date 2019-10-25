// objects.h
//

#ifndef IDLIY0KUSDJKFSKDFDFXCFHXCFXXX1EPVI
#define IDLIY0KUSDJKFSKDFDFXCFHXCFXXX1EPVI

///// Includes /////

#include <boost/asio.hpp>
#include <map>
#include <QPainter>
#include <QObject>
#include <QOpenGLBuffer>
#include <QStaticText>
#include <stdint.h>

///// Declarations /////

namespace monocle
{
enum class ObjectClass : uint16_t;
struct Objects;
}

///// Namespaces /////

namespace client
{

///// Declarations /////

enum class ROTATION : int;

///// Structures /////

struct Object
{
  Object(const uint64_t id, const monocle::ObjectClass classid, const uint64_t time, const float x, const float y, const float width, const float height);
  Object(Object&& rhs);

  void Allocate(const QRectF& imagepixelrect, const bool mirror, const ROTATION rotation);
  void DrawObjectText(const QRectF& imagepixelrect, const int width, const int height, const bool mirror, const ROTATION rotation, QPainter& painter);

  Object& operator=(Object&& rhs);

  uint64_t id_;
  monocle::ObjectClass classid_;
  uint64_t time_;
  float x_;
  float y_;
  float width_;
  float height_;
  std::chrono::steady_clock::time_point age_;

  QOpenGLBuffer vertexbuffer_;
  QStaticText text_;

};

///// Classes /////

class Objects : public QObject
{
 Q_OBJECT

 public:

  Objects();
  ~Objects();

  void Update(const QRectF& imagepixelrect, const bool mirror, const ROTATION rotation);
  void Update(const QRectF& imagepixelrect, const bool mirror, const ROTATION rotation, const monocle::Objects* objects, const uint64_t time, const uint64_t currenttime);

  void Clear();

  inline std::map< std::pair<monocle::ObjectClass, uint64_t>, std::vector<Object> >& GetObjects() { return objects_; }

 private:

  std::map< std::pair<monocle::ObjectClass, uint64_t>, std::vector<Object> > objects_;
 
};

}

#endif
