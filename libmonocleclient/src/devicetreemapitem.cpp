// devicetreemapitem.cpp
//

///// Includes /////

#include "monocleclient/devicetreemapitem.h"

#include <QMenu>
#include <QMessageBox>

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/managemapwindow.h"
#include "monocleclient/map.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTreeMapItem::DeviceTreeMapItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Map>& map, const QIcon mapicon) :
  DeviceTreeItem(parent, map->GetName(), DEVICE_TREE_TOP_LEVEL_ITEM_TYPE::DEVICE_MAP),
  device_(device),
  map_(map),
  edit_(new QAction("Edit", this)),
  remove_(new QAction("Remove", this))
{
  connect(edit_, &QAction::triggered, this, &DeviceTreeMapItem::Edit);
  connect(remove_, &QAction::triggered, this, &DeviceTreeMapItem::Remove);

  setIcon(0, mapicon);
}

DeviceTreeMapItem::~DeviceTreeMapItem()
{
  removemapconnection_.Close();

}

void DeviceTreeMapItem::ContextMenuEvent(const QPoint& pos)
{
  QMenu* menu = new QMenu(treeWidget());
  menu->addAction(edit_);
  menu->addAction(remove_);
  menu->exec(pos);
}

void DeviceTreeMapItem::Expanded()
{

}

void DeviceTreeMapItem::Collapsed()
{

}

void DeviceTreeMapItem::DoubleClicked()
{
  MainWindow::Instance()->GetVideoWidgetsMgr().CreateMapView(device_, map_);
  
}

void DeviceTreeMapItem::SetFilter(const QString& filter)
{
  if (filter.isEmpty())
  {
    setHidden(false);

  }
  else
  {
    if ((map_->GetName().indexOf(filter, 0, Qt::CaseInsensitive) == -1) && (map_->GetLocation().indexOf(filter, 0, Qt::CaseInsensitive) == -1))
    {
      setHidden(true);

    }
    else
    {
      setHidden(false);

    }
  }
}

void DeviceTreeMapItem::Edit(bool)
{
  ManageMapWindow(treeWidget(), device_, map_->GetToken()).exec();

}

void DeviceTreeMapItem::Remove(bool)
{
  if (QMessageBox::question(treeWidget(), tr("Remove"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    removemapconnection_ = device_->RemoveMap(map_->GetToken(), [this](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVEMAPRESPONSE& removemapresponse)
    {
      if (removemapresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removemapresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
    });
  }
}

}
