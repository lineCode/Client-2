// browsefilestreeitemitem.h
//

#ifndef IDSWDSFJZXCZXCXZCZZXCZXZZZCX0RGCLM
#define IDSWDSFJZXCZXCXZCZZXCZXZZZCX0RGCLM

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <monocleprotocol/client/connection.hpp>
#include <QObject>
#include <QTreeWidgetItem>
#include <string>

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

  BrowseFilesTreeItem(const std::string& folder, const boost::shared_ptr<Device>& device);
  virtual ~BrowseFilesTreeItem();

  void Expanded();

 private:

  const std::string folder_;

  boost::shared_ptr<Device> device_;

  monocle::client::Connection getchildfoldersconnection_;

};

}

#endif
