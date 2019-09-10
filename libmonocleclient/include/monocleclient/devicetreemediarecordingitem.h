// devicetreemediarecordingitem.h
//

#ifndef ID83QDSFDSFSFDSFDSSXFSDFDSSKI7PSMM
#define ID83QDSFDSFSFDSFDSSXFSDFDSSKI7PSMM

///// Includes /////

#include <stdint.h>
#include <QString>

#include "monocleclient/devicetreeitem.h"

///// Declarations /////

class QAction;

///// Namespaces /////

namespace client
{

///// Declarations /////

class Media;

///// Classes /////

class DeviceTreeMediaRecordingItem : public DeviceTreeItem
{
 Q_OBJECT

 public:

  DeviceTreeMediaRecordingItem(DeviceTreeItem* parent, QSharedPointer<Media>& media, const uint64_t deviceindex, const uint64_t recordingindex, const QString& name);
  ~DeviceTreeMediaRecordingItem();

  virtual void ContextMenuEvent(const QPoint& pos) override;

  virtual void Expanded() override;
  virtual void Collapsed() override;

  virtual void DoubleClicked() override;

  void SetFilter(const QString& filter);

 protected:



 private:

  QSharedPointer<Media> media_;
  uint64_t deviceindex_;
  uint64_t recordingindex_;

  QAction* properties_;


 private slots:

  void Properties(bool);

};

}

#endif
