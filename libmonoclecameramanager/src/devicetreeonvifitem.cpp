// onvifitem.cpp
//

///// Includes /////

#include "monoclecameramanager/devicetreeonvifitem.h"

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QUrl>

///// Namespaces /////

namespace cm
{

///// Globals /////



///// Methods /////

DeviceTreeONVIFItem::DeviceTreeONVIFItem(QTreeWidget* parent, const QString& uri, const QString& username, const QString& password) :
  DeviceTreeItem(parent, { uri }),
  hack_(new QAction("Hack", this))
{
  const QUrl url(uri);
  if (!url.isValid())
  {

    return;
  }

  setText(0, url.host() + ":" + QString::number(url.port(80)));

  setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
}

DeviceTreeONVIFItem::~DeviceTreeONVIFItem()
{

}

void DeviceTreeONVIFItem::Expanded()
{
  //TODO kick off a connection and say "loading..." while we grab things

}

void DeviceTreeONVIFItem::Collapsed()
{
  //TODO kill it all off

}

void DeviceTreeONVIFItem::DoubleClicked()
{
  //TODO expand it I think anyway?

}

void DeviceTreeONVIFItem::ContextMenuEvent(QContextMenuEvent* event)
{
  QMenu* menu = new QMenu(treeWidget());
  menu->addAction(hack_);
  //TODO "Hack" button ?
  menu->exec(event->globalPos());
}

}
