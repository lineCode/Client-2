// devicetreeitem.h
// 

#ifndef IDFBJDBRQERWERWEREWRWRWEEWWWWZISRJ
#define IDFBJDBRQERWERWEREWRWRWEEWWWWZISRJ

///// Includes /////

#include <QObject>
#include <QString>
#include <QTreeWidgetItem>

///// Declarations /////

class QAction;
class QContextMenuEvent;

///// Namespaces /////

namespace cm
{

///// Classes /////

class DeviceTreeItem : public QObject, public QTreeWidgetItem
{
 Q_OBJECT;

 public:

  DeviceTreeItem(QTreeWidget* parent, const QString& name);
  ~DeviceTreeItem();

  virtual void Expanded();
  virtual void Collapsed();
  virtual void DoubleClicked();
  virtual void ContextMenuEvent(QContextMenuEvent* event);

 private:

};

}

#endif
