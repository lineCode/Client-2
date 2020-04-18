// onvifitem.cpp
//

///// Includes /////

#include "monoclecameramanager/onvifitem.h"

#include <QUrl>

///// Namespaces /////

namespace cm
{

///// Globals /////



///// Methods /////

ONVIFItem::ONVIFItem(QTreeWidget* parent, const QString& uri, const QString& username, const QString& password) :
  QTreeWidgetItem(parent, { uri })
{
  const QUrl url(uri);
  if (!url.isValid())
  {

    return;
  }

  setText(0, url.host() + ":" + QString::number(url.port(80)));

  //TODO parse out the uri with QUrl I think
    //TODO change the name to the host if it has one
  
  //TODO if we have a username and password, then connect and start hacking away

  
  setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

}

ONVIFItem::~ONVIFItem()
{

}

void ONVIFItem::Expanded()
{
  //TODO kick off a connection and say "loading..."

}

void ONVIFItem::Collapsed()
{
  //TODO kill it all off

}

void ONVIFItem::DoubleClicked()
{
  //TODO expand it I think anyway?

}

}
