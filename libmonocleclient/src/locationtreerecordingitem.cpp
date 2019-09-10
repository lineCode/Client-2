// locationtreerecordingitem.cpp
//

///// Includes /////

#include "monocleclient/locationtreerecordingitem.h"

#include "monocleclient/locationtree.h"
#include "monocleclient/locationtreeitem.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/recording.h"

///// Namespaces /////

namespace client
{

///// Methods /////

LocationTreeRecordingItem::LocationTreeRecordingItem(LocationTree* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QIcon& recordingicon) :
  QTreeWidgetItem(parent, QStringList(std::initializer_list<QString>({ recording->GetName() }))),
  device_(device),
  recording_(recording)
{
  setIcon(0, recordingicon);

}

LocationTreeRecordingItem::LocationTreeRecordingItem(LocationTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QIcon& recordingicon) :
  QTreeWidgetItem(parent, QStringList(std::initializer_list<QString>({ recording->GetName() }))),
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

}
