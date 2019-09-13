// devicetreemediaitem.cpp
//

///// Includes /////

#include "monocleclient/devicetreemediaitem.h"

#include <QCheckBox>
#include <QMenu>
#include <QMessageBox>

#include "monocleclient/devicetreemediarecordingitem.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/media.h"
#include "monocleclient/mediapropertieswindow.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTreeMediaItem::DeviceTreeMediaItem(DeviceTree* parent, const QSharedPointer<Media>& media) :
  DeviceTreeItem(parent, media->GetPath()),
  media_(media),
  properties_(new QAction("Properties", this)),
  close_(new QAction("Close", this))
{
  connect(properties_, &QAction::triggered, this, &DeviceTreeMediaItem::Properties);
  connect(close_, &QAction::triggered, this, &DeviceTreeMediaItem::Close);

  if (media_->GetRet().is_initialized())
  {
    if (*media_->GetRet())
    {
      setBackground(0, Qt::red);
      setToolTip(0, "Failed to load error: " + QString::number(*media_->GetRet()));
    }
    else
    {
      setBackground(0, Qt::green);
      InitChildren();
    }
  }
  else
  {
    setBackground(0, Qt::yellow);

    connect(media.get(), &Media::Progress, this, &DeviceTreeMediaItem::Progress);
    connect(media.get(), &Media::Finished, this, &DeviceTreeMediaItem::Finished);
  }
}

DeviceTreeMediaItem::~DeviceTreeMediaItem()
{

}

void DeviceTreeMediaItem::ContextMenuEvent(const QPoint& pos)
{
  QMenu menu(treeWidget());
  if (media_->GetRet().is_initialized() && (*media_->GetRet() == 0))
  {
    menu.addAction(properties_);

  }
  menu.addAction(close_);
  menu.exec(pos);
}

void DeviceTreeMediaItem::Expanded()
{

}

void DeviceTreeMediaItem::Collapsed()
{

}

void DeviceTreeMediaItem::DoubleClicked()
{
  DeviceTreeItem::DoubleClicked();

}

void DeviceTreeMediaItem::SetFilter(const QString& filter)
{
  for (int i = 0; i < childCount(); ++i)
  {
    if (typeid(*child(i)) == typeid(DeviceTreeMediaRecordingItem))
    {
      static_cast<DeviceTreeMediaRecordingItem*>(child(i))->SetFilter(filter);

    }
  }

  if (filter.isEmpty())
  {
    setHidden(false);

  }
  else
  {
    // If all the children are hidden, then we can hide this one too
    bool hide = true;
    for (int i = 0; i < childCount(); ++i)
    {
      if (!child(i)->isHidden())
      {
        hide = false;
        break;
      }
    }

    setHidden(hide);
  }
}

void DeviceTreeMediaItem::InitChildren()
{
  for (const file::DEVICE& device : media_->GetMedia().GetFile().devices_)
  {
    for (const file::RECORDING& recording : device.recordings_)
    {
      addChild(new DeviceTreeMediaRecordingItem(this, media_, device.index_, recording.index_, QString::fromStdString(recording.name_)));

    }
  }
}

void DeviceTreeMediaItem::Properties(bool)
{
  MediaPropertiesWindow(treeWidget(), media_, boost::none).exec();

}

void DeviceTreeMediaItem::Close(bool)
{
  if (Options::Instance().GetHideMediaCloseDialog() == false)
  {
    QCheckBox* checkbox = new QCheckBox("Do not show this again");
    QMessageBox messagebox;
    messagebox.setText("Are you sure?");
    messagebox.setIcon(QMessageBox::Icon::Question);
    messagebox.addButton(QMessageBox::Yes);
    messagebox.addButton(QMessageBox::No);
    messagebox.setDefaultButton(QMessageBox::Yes);
    messagebox.setCheckBox(checkbox);
    bool donotshowagain = false;
    QObject::connect(checkbox, &QCheckBox::stateChanged, [&donotshowagain](int state)
    {
      if (static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked)
      {
        donotshowagain = true;

      }
      else
      {
        donotshowagain = false;

      }
    });
    if (messagebox.exec() != QMessageBox::Yes)
    {
      
      return;
    }

    if (donotshowagain)
    {
      Options::Instance().SetHideMediaCloseDialog(true);

    }
  }

  MainWindow::Instance()->GetMediaMgr().RemoveMedia(media_);
}

void DeviceTreeMediaItem::Progress(const float progress)
{
  if (progress >= 1.0f)
  {
    setText(0, media_->GetPath());
    setToolTip(0, QString());
  }
  else
  {
    const QString number = QString::number(static_cast<int>(progress * 100)) + "%";
    setText(0, media_->GetPath() + "(" + number + ")");
    setToolTip(0, number);
  }
}

void DeviceTreeMediaItem::Finished(const int ret)
{
  setText(0, media_->GetPath());

  if (ret)
  {
    setBackground(0, Qt::red);
    setToolTip(0, "Failed to load error: " + QString::number(ret));
  }
  else
  {
    setBackground(0, Qt::green);
    setToolTip(0, QString());
    InitChildren();
  }
}

}
