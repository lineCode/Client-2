// onvifitem.h
// 

#ifndef IDFBJDBRQZDFZXDFREWAEREEEWWWWZISRJ
#define IDFBJDBRQZDFZXDFREWAEREEEWWWWZISRJ

///// Includes /////

#include <QObject>
#include <QString>
#include <QTreeWidgetItem>

#include "monoclecameramanager/devicetreeitem.h"

///// Declarations /////

class QAction;
class QContextMenuEvent;

///// Namespaces /////

namespace cm
{

///// Classes /////

class DeviceTreeONVIFItem : public DeviceTreeItem
{
 Q_OBJECT;

 public:

  DeviceTreeONVIFItem(QTreeWidget* parent, const QString& uri, const QString& username, const QString& password);
  ~DeviceTreeONVIFItem();

  virtual void Expanded() override;
  virtual void Collapsed() override;
  virtual void DoubleClicked() override;
  virtual void ContextMenuEvent(QContextMenuEvent* event) override;

 private:

  QAction* hack_;

};

}

#endif
