// browsefilestreeitem.cpp
//

///// Includes /////

#include "monocleclient/browsefilestreeitem.h"

#include <boost/filesystem/path.hpp>

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
  getchildfoldersconnection_.Close();

}

void BrowseFilesTreeItem::Expanded()
{
  getchildfoldersconnection_ = device_->GetChildFolders(folder_, false, [this](const std::chrono::steady_clock::duration latency, const monocle::client::GETCHILDFOLDERSRESPONSE& getchildfoldersresponse)
  {
    // Clear old children
    while (childCount())
    {
      removeChild(child(0));

    }

    if (getchildfoldersresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy::DontShowIndicator);
      return;
    }

    if (getchildfoldersresponse.childfolders_.empty())
    {
      setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy::DontShowIndicator);

    }
    else
    {
      // Add a separator if it doesn't already exist
      const std::string slash = boost::filesystem::path("/").make_preferred().string();
      std::string separator;
      if (folder_.size() && ((folder_.back() != '/') && (folder_.back() != '\\')))
      {
        separator = slash;

      }

      for (const std::string& childfolder : getchildfoldersresponse.childfolders_)
      {
        BrowseFilesTreeItem* item = new BrowseFilesTreeItem(folder_ + separator + childfolder, device_);
        item->setText(0, childfolder.c_str());
        item->setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy::ShowIndicator);
        addChild(item);
      }
    }
  });
}

}
