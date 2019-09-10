// devicetreemediaitem.h
//

#ifndef ID83Q9092834723048ZXFSDFDSSKI7PSMM
#define ID83Q9092834723048ZXFSDFDSSKI7PSMM

///// Includes /////

#include "monocleclient/devicetreeitem.h"

#include <QAction>

///// Namespaces /////

namespace client
{

///// Declarations /////

class Media;

///// Classes /////

class DeviceTreeMediaItem : public DeviceTreeItem
{
 Q_OBJECT

 public:

  DeviceTreeMediaItem(DeviceTree* parent, const QSharedPointer<Media>& media);
  ~DeviceTreeMediaItem();

  QSharedPointer<Media>& GetMedia() { return media_; }

  virtual void ContextMenuEvent(const QPoint& pos) override;

  virtual void Expanded() override;
  virtual void Collapsed() override;

  virtual void DoubleClicked() override;

  void SetFilter(const QString& filter);

 protected:



 private:

  void InitChildren();

  QSharedPointer<Media> media_;

  QAction* properties_;
  QAction* close_;

  bool success_;


 private slots:

  void Properties(bool);
  void Close(bool);
  void Progress(const float progress);
  void Finished(const int ret);

};

}

#endif
