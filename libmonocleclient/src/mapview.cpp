// mapview.cpp
//

///// Includes /////

#include "monocleclient/mapview.h"

#include <QAuthenticator>
#include <QDir>
#include <QStandardPaths>

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/map.h"

///// Namespaces /////

namespace client
{

///// Methods /////

MapView::MapView(VideoWidget* videowidget, CUcontext cudacontext, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool stretch, const boost::shared_ptr<client::Device>& device, const QSharedPointer<Map>& map, const QResource* arial) :
  View(videowidget, cudacontext, selectedcolour, x, y, width, height, rotation, false, stretch, false, arial, false, false, false),
  device_(device),
  map_(map),
  networkaccessmanager_(new QNetworkAccessManager(this)),
  reply_(nullptr)
{
  connect(networkaccessmanager_, &QNetworkAccessManager::authenticationRequired, this, &MapView::AuthenticationRequired);
  connect(device_.get(), &Device::SignalMapChanged, this, &MapView::MapChanged);

  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), rotation_, mirror_, stretch_, true);

  const QString filename = GetMapFilename();
  QFile file(filename);
  if (file.exists())
  {
    if (!file.open(QIODevice::ReadOnly))
    {
      RequestMap();
      return;
    }

    const QByteArray data = file.readAll();
    if (data.isEmpty())
    {
      RequestMap();
      return;
    }

    if (QString::fromStdString(utility::Md5(data.data(), data.size())) == map_->GetImageMD5())
    {
      SendMap(data);

    }
    else
    {
      RequestMap();

    }
  }
  else
  {
    RequestMap();

  }
}

MapView::~MapView()
{
  if (reply_)
  {
    reply_->abort();
    delete reply_;
  }
}

void MapView::GetMenu(QMenu& parent)
{
  View::GetMenu(parent);

}

double MapView::GetAspectRatio() const
{
  if (imagewidth_ && imageheight_)
  {

    return (static_cast<double>(imagewidth_) / static_cast<double>(imageheight_));
  }
  else
  {

    return 0.0;
  }
}

bool MapView::GetImage(ImageBuffer& imagebuffer)
{
  bool hasimage = false;
  ImageBuffer previmagebuffer;
  while (true)
  {
    if (imagequeue_.pop(imagebuffer))
    {
      // If we have skipped frames, we should place them back into the temporary list, or destroy them if there is no room
      if (previmagebuffer.buffer_)
      {
        if (paused_)
        {
          cache_.push_back(previmagebuffer);
  
        }
        else
        {
          freeimagequeue_.AddFreeImage(previmagebuffer);
  
        }
      }
      previmagebuffer = imagebuffer;
      hasimage = true;
    }
    else
    {
      if (imagebuffer.buffer_)
      {
        imagewidth_ = imagebuffer.widths_[0];
        imageheight_ = imagebuffer.heights_[0];
      }
      return hasimage;
    }
  }
  return true;
}

void MapView::FrameStep(const bool forwards)
{
  
}

void MapView::Play(const uint64_t time, const boost::optional<uint64_t>& numframes)
{
  
}

void MapView::Pause(const boost::optional<uint64_t>& frame)
{
  
}

void MapView::Stop()
{
  
}

void MapView::Scrub(const uint64_t time)
{

}

void MapView::SendMap(const QByteArray& data)
{
  // Convert image and send it away
  QImage image;
  if (!image.loadFromData(data))
  {
    SetMessage(GetPlayRequestIndex(), true, tr("Failed to load map from server"));
    return;
  }
  image = image.convertToFormat(QImage::Format_RGBX8888);

  ImageBuffer imagebuffer = freeimagequeue_.GetFreeImage();
  imagebuffer.Destroy(); // Don't mind doing this, because the map will only update very infrequently
  imagebuffer.widths_[0] = image.width();
  imagebuffer.heights_[0] = image.height();
  imagebuffer.digitallysigned_ = boost::none;
  imagebuffer.marker_ = true;
  imagebuffer.time_ = 0;
  imagebuffer.type_ = IMAGEBUFFERTYPE_RGBA;
  imagebuffer.buffer_ = new uint8_t[image.sizeInBytes()];
  imagebuffer.data_[0] = imagebuffer.buffer_;

  std::memcpy(imagebuffer.buffer_, image.bits(), image.sizeInBytes());

  imagequeue_.push(imagebuffer);
}

void MapView::RequestMap()
{
  SetMessage(GetPlayRequestIndex(), false, QString("Loading Map..."));

  // Send off the HTTP request to grab the map from the server
  const QString mapurl = "http://" + device_->GetAddress() + ":" + QString::number(device_->GetPort()) + "/maps/" + QString::number(map_->GetToken());
  reply_ = networkaccessmanager_->get(QNetworkRequest(mapurl));
  connect(reply_, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &MapView::HTTPError);
  connect(reply_, &QNetworkReply::finished, this, &MapView::HTTPFinished);
  connect(reply_, &QNetworkReply::downloadProgress, this, &MapView::HTTPDownloadProgress);
}

QString MapView::GetMapFilename() const
{
  return QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).filePath(QString::number(device_->GetIdentifier()) + "_" + QString::number(map_->GetToken()) + ".map");
}

void MapView::AuthenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator)
{
  authenticator->setUser(device_->GetUsername());
  authenticator->setPassword(device_->GetPassword());
}

void MapView::HTTPError(QNetworkReply::NetworkError code)
{
  SetMessage(GetPlayRequestIndex(), true, tr("Failed to retrieve map from server"));

}

void MapView::HTTPFinished()
{
  if (reply_->error()) // Probably shouldn't happen?
  {
    SetMessage(GetPlayRequestIndex(), true, tr("Failed to retrieve map from server"));
    return;
  }

  const QString filename = GetMapFilename();
  const QByteArray data = reply_->readAll();
  if (data.isEmpty())
  {
    SetMessage(GetPlayRequestIndex(), true, tr("Failed to retrieve map from server"));
    return;
  }

  if (QString::fromStdString(utility::Md5(data.data(), data.size())) != map_->GetImageMD5())
  {
    LOG_GUI_WARNING_SOURCE(device_, tr("Map MD5 does not match"));

  }

  // Save the file to disk
  {
    QFile file(filename);
    if (file.exists())
    {
      if (!file.remove())
      {
        LOG_GUI_WARNING_SOURCE(device_, tr("Unable to remove map file"));

      }
    }

    if (!file.open(QIODevice::WriteOnly))
    {
      LOG_GUI_WARNING_SOURCE(device_, tr("Unable to open map file"));

    }
    else
    {
      if (file.write(data) == -1)
      {
        LOG_GUI_WARNING_SOURCE(device_, tr("Failed to write map file"));

      }
    }
  }

  SendMap(data);
}

void MapView::HTTPDownloadProgress(qint64 received, qint64 total)
{
  QString percent;
  if (total)
  {
    percent = QString::number((100 * received) / total);

  }
  else
  {
    percent = "0";

  }
  SetMessage(GetPlayRequestIndex(), false, tr("Downloading... ") + percent + "%");
}

void MapView::MapChanged(const QSharedPointer<Map>& map)
{
  RequestMap();

}

}
