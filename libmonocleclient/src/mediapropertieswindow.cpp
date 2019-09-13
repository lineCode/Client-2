// mediapropertieswindow.cpp
//

///// Includes /////

#include "monocleclient/mediapropertieswindow.h"

#include <boost/algorithm/string/join.hpp>
#include <monocleprotocol/codec_generated.h>
#include <QDateTime>
#include <QMessageBox>
#include <utility/publickey.hpp>

#include "monocleclient/media.h"
#include <QDebug>//TODO remove
///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Globals /////

const int DEVICEROLE = Qt::UserRole;
const int RECORDINGROLE = Qt::UserRole + 1;
const int TRACKROLE = Qt::UserRole + 2;

const int FAILEDREADFRAMESROLE = Qt::UserRole + 3;
const int VERIFIEDFRAMESROLE = Qt::UserRole + 4;
const int FAILEDVERIFIEDFRAMESROLE = Qt::UserRole + 5;
const int UNVERIFIEDFRAMESROLE = Qt::UserRole + 6;
const int SIZEROLE = Qt::UserRole + 7;
const int MINTIMEROLE = Qt::UserRole + 8;
const int MAXTIMEROLE = Qt::UserRole + 9;
const int CODECSROLE = Qt::UserRole + 10;

///// Methods /////

MediaPropertiesWindow::MediaPropertiesWindow(QWidget* parent, QSharedPointer<Media>& media, const boost::optional< std::pair<uint64_t, uint64_t> >& selectrecording) : // <deviceindex, recordingtoken>
  QDialog(parent),
  media_(media),
  running_(true)
{
  ui_.setupUi(this);
  
  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);
  connect(ui_.treemedia, &QTreeWidget::currentItemChanged, this, &MediaPropertiesWindow::CurrentItemChanged);
  connect(this, &MediaPropertiesWindow::Progress, this, &MediaPropertiesWindow::ProgressSlot, Qt::QueuedConnection);

  setWindowTitle(windowTitle() + " " + media_->GetPath());

  // Setup the tree
  std::vector<std::string> errors;
  for (const file::DEVICE& device : media_->GetMedia().GetFile().devices_)
  {
    std::string devicename;
    if (device.name_.empty())
    {
      if (device.address_.empty())
      {
        devicename = "device";

      }
      else
      {
        devicename = device.address_;

      }
    }
    else
    {
      devicename = device.name_;

    }

    QTreeWidgetItem* deviceitem = new QTreeWidgetItem({ QString::fromStdString(devicename) });
    for (const file::RECORDING& recording : device.recordings_)
    {
      std::string recordingname;
      if (recording.name_.empty())
      {
        recordingname = "recording";

      }
      else
      {
        recordingname = recording.name_;

      }

      QTreeWidgetItem* recordingitem = new QTreeWidgetItem({ QString::fromStdString(recording.name_) });
      for (const file::TRACK& track : recording.tracks_)
      {
        std::string trackname;
        if (track.description_.empty())
        {
          trackname = "track";

        }
        else
        {
          trackname = track.description_;

        }
        QTreeWidgetItem* trackitem = new QTreeWidgetItem({ QString::fromStdString(trackname) });
        
        trackitem->setData(0, DEVICEROLE, static_cast<qulonglong>(device.index_));
        trackitem->setData(0, RECORDINGROLE, static_cast<qulonglong>(recording.index_));
        trackitem->setData(0, TRACKROLE, static_cast<qulonglong>(track.index_));
        trackitem->setData(0, FAILEDREADFRAMESROLE, 0);
        trackitem->setData(0, VERIFIEDFRAMESROLE, 0);
        trackitem->setData(0, FAILEDVERIFIEDFRAMESROLE, 0);
        trackitem->setData(0, UNVERIFIEDFRAMESROLE, 0);
        trackitem->setData(0, SIZEROLE, 0);
        trackitem->setData(0, MINTIMEROLE, static_cast<qulonglong>(std::numeric_limits<uint64_t>::max()));
        trackitem->setData(0, MAXTIMEROLE, 0);

        QStringList codecs;
        for (const file::CODEC& codec : track.codecs_)
        {
          if (codec.codec_ < static_cast<int>(monocle::Codec::MIN) || codec.codec_ >(static_cast<int>(monocle::Codec::MAX)))
          {
            errors.push_back(std::string("Unrecognised codec device: ") + std::to_string(device.index_) + " recording: " + std::to_string(recording.index_) + " track: " + std::to_string(track.index_) + " codec: " + std::to_string(codec.codec_));
            continue;
          }
          codecs += QString(monocle::EnumNamesCodec()[codec.codec_]) + " " + QString::fromStdString(codec.parameters_);
        }
        trackitem->setData(0, CODECSROLE, codecs);

        recordingitem->addChild(trackitem);
      }
      recordingitem->setData(0, DEVICEROLE, static_cast<qulonglong>(device.index_));
      recordingitem->setData(0, RECORDINGROLE, static_cast<qulonglong>(recording.index_));
      deviceitem->addChild(recordingitem);
    }
    deviceitem->setData(0, DEVICEROLE, static_cast<qulonglong>(device.index_));
    ui_.treemedia->addTopLevelItem(deviceitem);
  }
  ui_.treemedia->expandAll();

  if (errors.size())
  {
    QMessageBox(QMessageBox::Information, tr("Warning"), QString::fromStdString(boost::algorithm::join(errors, "\n")), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();

  }

  if (selectrecording.is_initialized())
  {
    for (int i = 0; i < ui_.treemedia->topLevelItemCount(); ++i)
    {
      QTreeWidgetItem* device = ui_.treemedia->topLevelItem(i);
      if (device->data(0, DEVICEROLE).toULongLong() == selectrecording->first)
      {
        for (int j = 0; j < device->childCount(); ++j)
        {
          QTreeWidgetItem* recording = device->child(j);
          if (recording->data(0, RECORDINGROLE).toULongLong() == selectrecording->second)
          {
            recording->setSelected(true);
            break;
          }
        }
        break;
      }
    }
  }

  reader_ = std::thread([this]()
  {
    // Count up the total number of frames
    size_t totalframes = 0;
    for (const file::DEVICE& device : media_->GetMedia().GetFile().devices_)
    {
      for (const file::RECORDING& recording : device.recordings_)
      {
        for (const file::TRACK& track : recording.tracks_)
        {
          totalframes += track.frameheaders_.size();

        }
      }
    }

    // Read the frames and collect the data
    size_t currentframe = 0;
    unsigned int prevpercent = 0;
    std::vector<uint8_t> buffer;
    for (const file::DEVICE& device : media_->GetMedia().GetFile().devices_)
    {
      utility::PublicKey publickey;
      if (!device.signingkey_.empty())
      {
        if (publickey.Init(device.signingkey_))
        {
          // Ignoring this error because the user should know about it already after opening the media

        }
      }

      for (const file::RECORDING& recording : device.recordings_)
      {
        for (const file::TRACK& track : recording.tracks_)
        {
          uint64_t failedreadframes = 0;
          uint64_t verifiedframes = 0;
          uint64_t failedverifiedframes = 0;
          uint64_t unverifiedframes = 0;
          size_t size = 0;
          qulonglong mintime = std::numeric_limits<uint64_t>::max();
          qulonglong maxtime = 0;
          for (const std::shared_ptr<file::FRAMEHEADER>& frameheader : track.frameheaders_)
          {
            if (!running_)
            {

              return;
            }

            buffer.resize(frameheader->size_);
            if (media_->Read(frameheader->offset_, frameheader->size_, buffer.data()))
            {
              ++failedreadframes;

            }
            else
            {
              if (frameheader->signature_.empty())
              {
                ++unverifiedframes;

              }
              else
              {
                if (publickey.Verify(reinterpret_cast<const unsigned char*>(buffer.data()), buffer.size(), frameheader->signature_.data()))
                {
                  ++verifiedframes;

                }
                else
                {
                  ++failedverifiedframes;

                }
              }

              size += frameheader->size_;

              mintime = std::min(mintime, static_cast<qulonglong>(frameheader->time_));
              maxtime = std::max(maxtime, static_cast<qulonglong>(frameheader->time_));
            }

            // Send a signal every time the percent changes
            ++currentframe;
            const unsigned int currentpercent = static_cast<unsigned int>((100 * currentframe) / totalframes);
            if (currentpercent != prevpercent)
            {
              prevpercent = currentpercent;
              emit Progress(currentpercent, device.index_, recording.index_, track.index_, failedreadframes, verifiedframes, failedverifiedframes, unverifiedframes, size, mintime, maxtime);
              failedreadframes = 0;
              verifiedframes = 0;
              failedverifiedframes = 0;
              unverifiedframes = 0;
              size = 0;
              mintime = std::numeric_limits<uint64_t>::max();
              maxtime = 0;
            }
          }

          // Send anything left over
          emit Progress(prevpercent, device.index_, recording.index_, track.index_, failedreadframes, verifiedframes, failedverifiedframes, unverifiedframes, size, mintime, maxtime);
          failedreadframes = 0;
          verifiedframes = 0;
          failedverifiedframes = 0;
          unverifiedframes = 0;
          size = 0;
          mintime = std::numeric_limits<uint64_t>::max();
          maxtime = 0;
        }
      }
    }
  });
}

MediaPropertiesWindow::~MediaPropertiesWindow()
{
  running_ = false;
  reader_.join();
}

QString MediaPropertiesWindow::Size(size_t numbytes) const
{
  if (numbytes < 1024)
  {

    return (QString::number(numbytes) + "Bytes");
  }

  numbytes /= 1024;
  if (numbytes < 1024)
  {

    return (QString::number(numbytes) + "Kilobytes");
  }

  if (numbytes < (1024 * 1024))
  {

    return (QString::number(numbytes / 1024) + "." + QString::number(numbytes % 1024) + "Megabytes");
  }
  numbytes /= 1024;

  if (numbytes < (1024 * 1024))
  {

    return (QString::number(numbytes / 1024) + "." + QString::number(numbytes % 1024) + "Gigabytes");
  }
  numbytes /= 1024;

  return (QString::number(numbytes / 1024) + "." + QString::number(numbytes % 1024) + "Terabytes");
}

void MediaPropertiesWindow::CurrentItemChanged(QTreeWidgetItem* item, QTreeWidgetItem*)
{
  QVariant device;
  QVariant recording;
  QVariant track;
  if (item)
  {
    device = item->data(0, DEVICEROLE);
    if (device.isNull())
    {
      // This shouldn't happen, so we can just assume it will never happen I suppose...
      return;
    }
    recording = item->data(0, RECORDINGROLE);
    track = item->data(0, TRACKROLE);
  }

  // Collect the data
  uint64_t failedreadframes = 0;
  uint64_t verifiedframes = 0;
  uint64_t failedverifiedframes = 0;
  uint64_t unverifiedframes = 0;
  size_t size = 0;
  qulonglong mintime = std::numeric_limits<uint64_t>::max();
  qulonglong maxtime = 0;
  QStringList codecs;
  for (int i = 0; i < ui_.treemedia->topLevelItemCount(); ++i)
  {
    QTreeWidgetItem* deviceitem = ui_.treemedia->topLevelItem(i);
    if (deviceitem->data(0, DEVICEROLE).toInt() == device.toInt())
    {
      if (!device.isNull() && (device.toInt() != deviceitem->data(0, RECORDINGROLE).toInt()))
      {

        continue;
      }

      for (int j = 0; j < deviceitem->childCount(); ++j)
      {
        QTreeWidgetItem* recordingitem = deviceitem->child(j);
        if (!recording.isNull() && (recording.toInt() != recordingitem->data(0, RECORDINGROLE).toInt()))
        {

          continue;
        }

        for (int k = 0; k < recordingitem->childCount(); ++k)
        {
          QTreeWidgetItem* trackitem = recordingitem->child(k);
          if (!track.isNull() && (track.toInt() != trackitem->data(0, TRACKROLE).toInt()))
          {

            continue;
          }
          
          failedreadframes += trackitem->data(0, FAILEDREADFRAMESROLE).toInt();
          verifiedframes += trackitem->data(0, VERIFIEDFRAMESROLE).toInt();
          failedverifiedframes += trackitem->data(0, FAILEDVERIFIEDFRAMESROLE).toInt();
          unverifiedframes += trackitem->data(0, UNVERIFIEDFRAMESROLE).toInt();
          size += trackitem->data(0, SIZEROLE).toULongLong();
          mintime = std::min(mintime, trackitem->data(0, MINTIMEROLE).toULongLong());
          maxtime = std::max(maxtime, trackitem->data(0, MAXTIMEROLE).toULongLong());
          codecs += trackitem->data(0, CODECSROLE).toStringList();
        }
      }
    }
    break;
  }

  QString text;
  for (const QString& codec : codecs)
  {
    text += codec + "\n";

  }

  QString mintimestring;
  if (mintime != std::numeric_limits<uint64_t>::max())
  {
    mintimestring = "Min DateTime: " + QDateTime::fromMSecsSinceEpoch(mintime, Qt::UTC).toString() + "\n";

  }

  QString maxtimestring;
  if (maxtime != 0)
  {
    maxtimestring = "Max DateTime: " + QDateTime::fromMSecsSinceEpoch(maxtime, Qt::UTC).toString() + "\n";

  }

  text += "Frames failed to read: " + QString::number(failedreadframes) + "\n"
        + "Verified frames: " + QString::number(verifiedframes) + "\n"
        + "Failed Verification frames: " + QString::number(failedverifiedframes) + "\n"
        + "Unverification frames: " + QString::number(unverifiedframes) + "\n"
        + mintimestring
        + maxtimestring
        + "Size: " + Size(size) + "\n"
        + "Total frames: " + QString::number(verifiedframes + failedverifiedframes + unverifiedframes);

  ui_.labelproperties->setText(text);
}

void MediaPropertiesWindow::ProgressSlot(const unsigned int percent, const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const uint64_t failedreadframes, const uint64_t verifiedframes, const uint64_t failedverifiedframes, const uint64_t unverifiedframes, const size_t size, const qulonglong mintime, const qulonglong maxtime)
{
  ui_.progressread->setValue(percent);

  // Add the data to the roles
  for (int i = 0; i < ui_.treemedia->topLevelItemCount(); ++i)
  {
    QTreeWidgetItem* deviceitem = ui_.treemedia->topLevelItem(i);
    if (deviceitem->data(0, DEVICEROLE).toULongLong() == deviceindex)
    {
      for (int j = 0; j < deviceitem->childCount(); ++j)
      {
        QTreeWidgetItem* recordingitem = deviceitem->child(j);
        if (recordingitem->data(0, RECORDINGROLE).toULongLong() == recordingindex)
        {
          for (int k = 0; k < recordingitem->childCount(); ++k)
          {
            QTreeWidgetItem* trackitem = recordingitem->child(k);
            if (trackitem->data(0, TRACKROLE).toULongLong() == trackindex)
            {
              trackitem->setData(0, FAILEDREADFRAMESROLE, trackitem->data(0, FAILEDREADFRAMESROLE).toULongLong() + failedreadframes);
              trackitem->setData(0, VERIFIEDFRAMESROLE, trackitem->data(0, VERIFIEDFRAMESROLE).toULongLong() + verifiedframes);
              trackitem->setData(0, FAILEDVERIFIEDFRAMESROLE, trackitem->data(0, FAILEDVERIFIEDFRAMESROLE).toULongLong() + failedverifiedframes);
              trackitem->setData(0, UNVERIFIEDFRAMESROLE, trackitem->data(0, UNVERIFIEDFRAMESROLE).toULongLong() + unverifiedframes);
              trackitem->setData(0, SIZEROLE, trackitem->data(0, SIZEROLE).toULongLong() + size);
              trackitem->setData(0, MINTIMEROLE, std::min(mintime, trackitem->data(0, MINTIMEROLE).toULongLong()));
              trackitem->setData(0, MAXTIMEROLE, std::max(maxtime, trackitem->data(0, MAXTIMEROLE).toULongLong()));

              if (trackitem->isSelected())
              {
                CurrentItemChanged(trackitem, nullptr);

              }
              break;
            }
          }

          if (recordingitem->isSelected())
          {
            CurrentItemChanged(recordingitem, nullptr);

          }
          break;
        }
      }

      if (deviceitem->isSelected())
      {
        CurrentItemChanged(deviceitem, nullptr);

      }
      break;
    }
  }

  if (ui_.treemedia->selectedItems().isEmpty())
  {
    CurrentItemChanged(nullptr, nullptr);

  }
}

}

QT_END_NAMESPACE
