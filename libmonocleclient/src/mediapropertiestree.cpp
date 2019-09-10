// mediapropertiestree.cpp
//

///// Includes /////

#include "monocleclient/mediapropertiestree.h"

#include <QMouseEvent>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

MediaPropertiesTree::MediaPropertiesTree(QWidget *parent) :
  QTreeWidget(parent)
{

}

void MediaPropertiesTree::mousePressEvent(QMouseEvent* event)
{
  QModelIndex item = indexAt(event->pos());
  bool selected = selectionModel()->isSelected(indexAt(event->pos()));
  QTreeView::mousePressEvent(event);
  if ((item.row() == -1 && item.column() == -1) || selected)
  {
    clearSelection();
    const QModelIndex index;
    selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
  }
}

}

QT_END_NAMESPACE
