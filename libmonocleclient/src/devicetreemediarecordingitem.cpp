// devicetreemediarecordingitem.cpp
//

///// Includes /////

#include "monocleclient/devicetreemediarecordingitem.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>

#include "monocleclient/mainwindow.h"
#include "monocleclient/media.h"
#include "monocleclient/mediapropertieswindow.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTreeMediaRecordingItem::DeviceTreeMediaRecordingItem(DeviceTreeItem* parent, QSharedPointer<Media>& media, const uint64_t deviceindex, const uint64_t recordingindex, const QString& name) :
  DeviceTreeItem(parent, name),
  media_(media),
  deviceindex_(deviceindex),
  recordingindex_(recordingindex),
  properties_(new QAction("Properties", this))
{
  connect(properties_, &QAction::triggered, this, &DeviceTreeMediaRecordingItem::Properties);

}

DeviceTreeMediaRecordingItem::~DeviceTreeMediaRecordingItem()
{

}

void DeviceTreeMediaRecordingItem::ContextMenuEvent(const QPoint& pos)
{
  if (media_->GetRet().is_initialized() && (*media_->GetRet() == 0))
  {
    QMenu menu(treeWidget());
    menu.addAction(properties_);
    menu.exec(pos);
  }
}

void DeviceTreeMediaRecordingItem::Expanded()
{

}

void DeviceTreeMediaRecordingItem::Collapsed()
{

}

void DeviceTreeMediaRecordingItem::DoubleClicked()
{
  // Get the tracks, and pick the first one
  boost::container::flat_set<file::DEVICE>::const_iterator device = std::find_if(media_->GetMedia().GetFile().devices_.cbegin(), media_->GetMedia().GetFile().devices_.cend(), [this](const file::DEVICE& device) { return (device.index_ == deviceindex_); });
  if (device == media_->GetMedia().GetFile().devices_.cend())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Could not find device: ") + QString::number(deviceindex_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  boost::container::flat_set<file::RECORDING>::const_iterator recording = std::find_if(device->recordings_.cbegin(), device->recordings_.cend(), [this](const file::RECORDING& recording) { return (recording.index_ == recordingindex_); });
  if (device == media_->GetMedia().GetFile().devices_.cend())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Could not find recording: ") + QString::number(recordingindex_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (recording->tracks_.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No tracks available"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  MainWindow::Instance()->GetVideoWidgetsMgr().CreateMediaView(media_, deviceindex_, recordingindex_, recording->tracks_.begin()->index_);
}

void DeviceTreeMediaRecordingItem::SetFilter(const QString& filter)
{
  if (filter.isEmpty())
  {
    setHidden(false);

  }
  else
  {
    const boost::optional<file::RECORDING> recording = media_->GetMedia().GetRecording(deviceindex_, recordingindex_);
    if (!recording.is_initialized())
    {
      setHidden(true); // Shouldn't happen, lets just hide it
      return;
    }

    if ((QString::fromStdString(recording->name_).indexOf(filter, 0, Qt::CaseInsensitive) == -1) && (QString::fromStdString(recording->location_).indexOf(filter, 0, Qt::CaseInsensitive) == -1))
    {
      setHidden(true);

    }
    else
    {
      setHidden(false);

    }
  }
}

void DeviceTreeMediaRecordingItem::Properties(bool)
{
  MediaPropertiesWindow(treeWidget(), media_, std::make_pair(deviceindex_, recordingindex_)).exec();

}

}
