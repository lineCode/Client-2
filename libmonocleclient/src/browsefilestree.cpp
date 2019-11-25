// browsefilestree.cpp
//

///// Includes /////

#include "monocleclient/browsefilestree.h"

#include "monocleclient/browsefilestreeitem.h"

///// Namespaces /////

namespace client
{

///// Methods /////

BrowseFilesTree::BrowseFilesTree(QWidget* parent) :
  QTreeWidget(parent)
{
  connect(this, &BrowseFilesTree::itemExpanded, this, &BrowseFilesTree::ItemExpanded);

}

BrowseFilesTree::~BrowseFilesTree()
{

}

void BrowseFilesTree::ItemExpanded(QTreeWidgetItem* item)
{
  static_cast<BrowseFilesTreeItem*>(item)->Expanded();

}

}
