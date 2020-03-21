// locationtreerecordingitem.cpp
//

///// Includes /////

#include "monocleclient/locationtreerecordingitem.h"

#include "monocleclient/devicetreeitem.h"
#include "monocleclient/locationtree.h"
#include "monocleclient/locationtreeitem.h"
#include "monocleclient/locationtreemapitem.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/map.h"
#include "monocleclient/recording.h"

///// Namespaces /////

namespace client
{

///// Methods /////

LocationTreeRecordingItem::LocationTreeRecordingItem(LocationTree* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QIcon& recordingicon) :
  QTreeWidgetItem(parent, QStringList(std::initializer_list<QString>({ recording->GetName() })), DEVICE_TREE_ITEM_TYPE::DEVICE_RECORDING),
  device_(device),
  recording_(recording)
{
  setIcon(0, recordingicon);

}

LocationTreeRecordingItem::LocationTreeRecordingItem(LocationTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QIcon& recordingicon) :
  QTreeWidgetItem(parent, QStringList(std::initializer_list<QString>({ recording->GetName() })), DEVICE_TREE_ITEM_TYPE::DEVICE_RECORDING),
  device_(device),
  recording_(recording)
{
  setIcon(0, recordingicon);

}

LocationTreeRecordingItem::~LocationTreeRecordingItem()
{

}

void LocationTreeRecordingItem::ContextMenuEvent(const QPoint& pos)
{

}

void LocationTreeRecordingItem::Expanded()
{

}

void LocationTreeRecordingItem::Collapsed()
{

}

void LocationTreeRecordingItem::DoubleClicked()
{
  std::vector< QSharedPointer<client::RecordingTrack> > tracks = recording_->GetVideoTracks();
  if (tracks.empty())
  {
    Expanded();
    return;
  }

  MainWindow::Instance()->GetVideoWidgetsMgr().CreateVideoView(device_, recording_, tracks.front());
}

void LocationTreeRecordingItem::SetFilter(const QString& filter)
{
  if (filter.isEmpty())
  {
    setHidden(false);

  }
  else
  {
    if ((recording_->GetName().indexOf(filter, 0, Qt::CaseInsensitive) == -1) && (recording_->GetLocation().indexOf(filter, 0, Qt::CaseInsensitive) == -1))
    {
      setHidden(true);

    }
    else
    {
      setHidden(false);

    }
  }
}

uint64_t LocationTreeRecordingItem::GetToken() const
{
  return recording_->GetToken();
}

void LocationTreeRecordingItem::Clear()
{
  for (int i = (childCount()-1); i >= 0; --i)
  {
    removeChild(child(i));

  }
}

bool LocationTreeRecordingItem::operator<(const QTreeWidgetItem& rhs) const
{
  uint64_t guiorder = 0;
  if (rhs.type() == DEVICE_TREE_ITEM_TYPE::DEVICE_RECORDING)
  {
    const LocationTreeRecordingItem* recordingitem = static_cast<const LocationTreeRecordingItem*>(&rhs);
    guiorder = recordingitem->GetRecording()->GetGuiOrder();
  }
  else if (rhs.type() == DEVICE_TREE_ITEM_TYPE::DEVICE_MAP)
  {
    const LocationTreeMapItem* mapitem = static_cast<const LocationTreeMapItem*>(&rhs);
    guiorder = mapitem->GetMap()->GetGuiOrder();
  }
  return (recording_->GetGuiOrder() < guiorder);
}

}
