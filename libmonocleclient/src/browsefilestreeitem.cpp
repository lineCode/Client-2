// browsefilestreeitem.cpp
//

///// Includes /////

#include "monocleclient/browsefilestreeitem.h"

#include "monocleclient/device.h"

///// Namespaces /////

namespace client
{

///// Methods /////

BrowseFilesTreeItem::BrowseFilesTreeItem(const std::string& folder, const boost::shared_ptr<Device>& device) :
  QTreeWidgetItem(),
  folder_(folder),
  device_(device)
{
  
}

BrowseFilesTreeItem::~BrowseFilesTreeItem()
{

}

void BrowseFilesTreeItem::Expanded()
{
  getchildfoldersconnection_ = device_->GetChildFolders(folder_, [this](const std::chrono::steady_clock::duration latency, const monocle::client::GETCHILDFOLDERSRESPONSE& getchildfoldersresponse)
  {
    if (getchildfoldersresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      //TODO put a child error code thing in...
        //TODO why does "Documents and settings" fail
      return;
    }

    // Clear old children
    while (childCount())
    {
      removeChild(child(0));

    }

    //TODO if it is empty, show a TreeWidgetItem "empty" or something
      //TODO with no icon?

    for (const std::string& childfolder : getchildfoldersresponse.childfolders_)
    {
      //TODO we want to get rid of "folder_" substring at the beginning?

      BrowseFilesTreeItem* item = new BrowseFilesTreeItem(childfolder, device_);
      item->setText(0, childfolder.c_str());
      item->setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy::ShowIndicator);
      addChild(item);
    }
  });
}

}
