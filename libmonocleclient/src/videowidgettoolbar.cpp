// videowidgettoolbar.cpp
//

///// Includes /////

#include "monocleclient/videowidgettoolbar.h"

#include <QMenu>

///// Namespaces /////

namespace client
{

///// Methods /////

VideoWidgetToolbar::VideoWidgetToolbar(QWidget* parent) :
  QWidget(parent),
  hidetoolbar_(new QAction(tr("Hide Toolbar"), this))
{
  connect(hidetoolbar_, &QAction::triggered, this, &VideoWidgetToolbar::HideToolbar);

}

VideoWidgetToolbar::~VideoWidgetToolbar()
{

}

QToolButton* VideoWidgetToolbar::GetToolButtonAddRow()
{
  return findChild<QToolButton*>(QString("toolbuttonaddrow"), Qt::FindDirectChildrenOnly);
}

QToolButton* VideoWidgetToolbar::GetToolButtonRemoveRow()
{
  return findChild<QToolButton*>(QString("toolbuttonremoverow"), Qt::FindDirectChildrenOnly);
}

QToolButton* VideoWidgetToolbar::GetToolButtonAddColumn()
{
  return findChild<QToolButton*>(QString("toolbuttonaddcolumn"), Qt::FindDirectChildrenOnly);
}

QToolButton* VideoWidgetToolbar::GetToolButtonRemoveColumn()
{
  return findChild<QToolButton*>(QString("toolbuttonremovecolumn"), Qt::FindDirectChildrenOnly);
}

QToolButton* VideoWidgetToolbar::GetToolButtonShowFullscreen()
{
  return findChild<QToolButton*>(QString("toolbuttonshowfullscreen"), Qt::FindDirectChildrenOnly);
}

void VideoWidgetToolbar::Disable()
{
  GetToolButtonAddRow()->setEnabled(false);
  GetToolButtonRemoveRow()->setEnabled(false);
  GetToolButtonAddColumn()->setEnabled(false);
  GetToolButtonRemoveColumn()->setEnabled(false);
  GetToolButtonShowFullscreen()->setEnabled(false);
}

void VideoWidgetToolbar::contextMenuEvent(QContextMenuEvent* event)
{
  QMenu menu;
  menu.addAction(hidetoolbar_);
  menu.exec(event->globalPos());
}

void VideoWidgetToolbar::HideToolbar(bool)
{
  hide();

}

}
