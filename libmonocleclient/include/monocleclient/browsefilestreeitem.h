// browsefilestreeitemitem.h
//

#ifndef IDSWDSFJZXCZXCXZCZZXCZXZZZCX0RGCLM
#define IDSWDSFJZXCZXCXZCZZXCZXZZZCX0RGCLM

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <QTreeWidgetItem>

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;

///// Classes /////

class BrowseFilesTreeItem : public QObject, public QTreeWidgetItem
{
 Q_OBJECT

 public:

  BrowseFilesTreeItem(const boost::shared_ptr<Device>& device);
  virtual ~BrowseFilesTreeItem();

  void Expanded();

 private:

  boost::shared_ptr<Device> device_;


};

}

#endif
