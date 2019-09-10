// mediapropertiestree.h
//

#ifndef ID2VVC1235434334534554423419WKDN9G
#define ID2VVC1235434334534554423419WKDN9G

///// Includes /////

#include <QTreeWidget>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Media;

///// Classes /////

class MediaPropertiesTree : public QTreeWidget
{
  Q_OBJECT

 public:

  MediaPropertiesTree(QWidget* parent);

 private:

  virtual void mousePressEvent(QMouseEvent* event) override;

};

}

QT_END_NAMESPACE

#endif
