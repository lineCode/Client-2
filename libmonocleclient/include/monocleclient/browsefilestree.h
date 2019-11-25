// browsefilestree.h
//

#ifndef IDSWDSFJSKFJXZCLZXCZCXZCXZZQ0RGCLM
#define IDSWDSFJSKFJXZCLZXCZCXZCXZZQ0RGCLM

///// Includes /////

#include <QTreeWidget>

///// Declarations /////

class QTreeWidgetItem;

///// Namespaces /////

namespace client
{

///// Classes /////

class BrowseFilesTree : public QTreeWidget
{
 Q_OBJECT

 public:

  BrowseFilesTree(QWidget* parent);
  virtual ~BrowseFilesTree();

 protected:

  void ItemExpanded(QTreeWidgetItem* item);

 private:



};

}

#endif
