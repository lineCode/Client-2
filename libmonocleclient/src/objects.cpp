// objects.cpp
//

///// Includes /////

#include "monocleclient/objects.h"

#include <utility/utility.hpp>

#include "monocleclient/videowidget.h"
#include "monocleclient/view.h"

///// Namespaces /////

namespace client
{

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

Objects::Objects()
{

}

Objects::~Objects()
{

}

void Objects::Update(const QRectF& imagepixelrect, const bool mirror, const ROTATION rotation)
{
  for (std::pair< const std::pair<monocle::ObjectClass, uint64_t>, std::vector<Object> >& objects : objects_)
  {
    for (Object& object : objects.second)
    {
      object.Allocate(imagepixelrect, mirror, rotation);

    }
  }
}

void Objects::Update(const QRectF& imagepixelrect, const bool mirror, const ROTATION rotation, const monocle::Objects* objects, const uint64_t time, const uint64_t currenttime)
{
  for (const monocle::Object* object : *objects->objects())
  {
    Object o(object->id(), object->classid(), time, object->x(), object->y(), object->width(), object->height());
    if (static_cast<size_t>(o.classid_) > static_cast<size_t>(monocle::ObjectClass::MAX))
    {
      // Ignore unknown types
      continue;
    }
    o.text_.setPerformanceHint(QStaticText::PerformanceHint::AggressiveCaching);
    o.text_.setTextFormat(Qt::TextFormat::RichText);
    o.vertexbuffer_.create();
    o.vertexbuffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    o.Allocate(imagepixelrect, mirror, rotation);

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
        //TODO update the endtime here instead of creaeting a new Object
        *j = std::move(o);

      }

      // Clear up any old objects while we're here...
      const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
      i->second.erase(std::remove_if(i->second.begin(), i->second.end(), [currenttime, now](const Object& object) { return ((currenttime > object.time_) && ((currenttime - object.time_) > OBJECT_KILL_DELAY) && ((now - object.age_) > OBJECT_KILL_AGE)); }), i->second.end());
      if (i->second.empty())
      {
        objects_.erase(i);
      
      }
    }
  }
}

void Objects::Clear()
{
  objects_.clear();

}

}
