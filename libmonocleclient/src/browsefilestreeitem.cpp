// browsefilestreeitem.cpp
//

///// Includes /////

#include "monocleclient/browsefilestreeitem.h"

///// Namespaces /////

namespace client
{

///// Methods /////

BrowseFilesTreeItem::BrowseFilesTreeItem(const boost::shared_ptr<Device>& device) :
  QTreeWidgetItem(),
  device_(device)
{

}

BrowseFilesTreeItem::~BrowseFilesTreeItem()
{

}

void BrowseFilesTreeItem::Expanded()
{
  int i = 0;//TODO use the device etc

}

}
