// devicetree.h
// 

#ifndef IDFBJDBRQAQSSDASDADADADZZWWWWZISRJ
#define IDFBJDBRQAQSSDASDADADADZZWWWWZISRJ

///// Includes /////

#include <QObject>
#include <QTreeWidget>

///// Declarations /////

class QWidget;
class QTreeWidgetItem;

///// Namespaces /////

namespace cm
{

///// Classes /////

class DeviceTree : public QTreeWidget
{
 Q_OBJECT;

 public:

  DeviceTree(QWidget* parent);
  ~DeviceTree();
  

 protected:

  void contextMenuEvent(QContextMenuEvent* event) override;
  

 private:



 private slots:

  void ItemExpanded(QTreeWidgetItem* item);
  void ItemCollapsed(QTreeWidgetItem* item);
  void DoubleClicked(QTreeWidgetItem* item, int);

};

}

#endif
