// browsefileswindow.cpp
//

///// Includes /////

#include "monocleclient/browsefileswindow.h"

#include "monocleclient/browsefilestreeitem.h"
#include "monocleclient/device.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

BrowseFilesWindow::BrowseFilesWindow(QWidget* parent, const boost::shared_ptr<Device>& device) :
  QDialog(parent),
  device_(device)
{
  ui_.setupUi(this);
  
  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);

  //TODO prefill the text thing with file.dat
    //TODO when selecting things, use the filename... and add the folder pretext stuff?

  device_->GetChildFolders(std::string("/"), [this](const std::chrono::steady_clock::duration latency, const monocle::client::GETCHILDFOLDERSRESPONSE& getchildfoldersresponse)
  {
    if (getchildfoldersresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      //TODO QMessageBox
      return;
    }

    for (const std::string& childfolder : getchildfoldersresponse.childfolders_)
    {
      BrowseFilesTreeItem* item = new BrowseFilesTreeItem(childfolder, device_);
      item->setText(0, childfolder.c_str());
      item->setChildIndicatorPolicy(QTreeWidgetItem::ChildIndicatorPolicy::ShowIndicator);
      ui_.treebrowsefiles->addTopLevelItem(item);
    }
  });
}

BrowseFilesWindow::~BrowseFilesWindow()
{

}

}

QT_END_NAMESPACE
