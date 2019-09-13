// managerecordingjobsourceonvifwindow.cpp
//

///// Includes /////

#include "monocleclient/managerecordingjobsourceonvifwindow.h"

#include <monocleprotocol/streamingprotocol_generated.h>
#include <network/uri.hpp>
#include <onvifclient/deviceclient.hpp>
#include <onvifclient/mediaclient.hpp>
#include <QMessageBox>
#include <QTimer>
#include <QUrl>
#include <rtsp/client/client.hpp>

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/receiver.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"
#include "monocleclient/managerecordingjobsourceonvifdiscoverytreeitem.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageRecordingJobSourceONVIFWindow::ManageRecordingJobSourceONVIFWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const bool editing, const bool enabled, const uint32_t trackid, const QString& mediauri, const QString& profiletoken, const QString& sourcetag, const monocle::StreamingProtocol streamingprotocol, const QString& username, const QString& password, const QString& rotation, const std::vector<uint32_t>& reservedtrackids) :
  QDialog(parent),
  device_(device),
  recording_(recording),
  recordingjob_(recordingjob)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);
  connect(recording_.data(), &Recording::TrackAdded, this, &ManageRecordingJobSourceONVIFWindow::TrackAdded, Qt::QueuedConnection);
  connect(recording_.data(), &Recording::TrackChanged, this, &ManageRecordingJobSourceONVIFWindow::TrackChanged, Qt::QueuedConnection);
  connect(recording_.data(), &Recording::TrackRemoved, this, &ManageRecordingJobSourceONVIFWindow::TrackRemoved, Qt::QueuedConnection);

  ui_.treediscovery->Init(device_, username.toStdString(), password.toStdString());

  if (editing)
  {
    setWindowTitle("Edit ONVIF Source");

  }

  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::TCPInterleaved)), static_cast<int>(monocle::StreamingProtocol::TCPInterleaved));
  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::UDPUnicast)), static_cast<int>(monocle::StreamingProtocol::UDPUnicast));
  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::UDPMulticast)), static_cast<int>(monocle::StreamingProtocol::UDPMulticast));

  if (editing)
  {
    const QSharedPointer<client::RecordingTrack> track = recording_->GetTrack(trackid);
    if (track == nullptr)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find track: ") + QString::number(trackid), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();

    }
    else
    {
      ui_.combotrack->addItem(track->GetDescription() + "(" + monocle::EnumNameTrackType(track->GetTrackType()) + ")", track->GetId());

    }
  }
  else
  {
    for (const QSharedPointer<client::RecordingTrack>& track : recording_->GetTracks())
    {
      if (track->GetId() != trackid) // We can select our current track
      {
        if (std::find_if(reservedtrackids.cbegin(), reservedtrackids.cend(), [id = track->GetId()](const uint64_t trackid){ return (trackid == id); }) != reservedtrackids.cend()) // Check to see if this is already in use by another source
        {

          continue;
        }
      }
      ui_.combotrack->addItem(track->GetDescription() + "(" + monocle::EnumNameTrackType(track->GetTrackType()) + ")", track->GetId());
    }
  }

  // Setup
  ui_.checkenabled->setChecked(enabled);
  for (int i = 0; i < ui_.combotrack->count(); ++i)
  {
    if (ui_.combotrack->itemData(i) == trackid)
    {
      ui_.combotrack->setCurrentIndex(i);
      break;
    }
  }
  ui_.edituri->setText(mediauri);
  ui_.editprofiletoken->setText(profiletoken);
  ui_.editsourcetag->setText(sourcetag);

  const int i = ui_.comboprotocol->findData(static_cast<int>(streamingprotocol));
  if (i != -1)
  {
    ui_.comboprotocol->setCurrentIndex(i);

  }
  ui_.editusername->setText(username);
  ui_.editpassword->setText(password);

  // Rotation
  ui_.comborotation->addItem("0", "0");
  ui_.comborotation->addItem("90", "90");
  ui_.comborotation->addItem("180", "180");
  ui_.comborotation->addItem("270", "270");
  for (int i = 0; i < ui_.comborotation->count(); ++i)
  {
    if (ui_.comborotation->itemData(i, Qt::UserRole).toString() == rotation)
    {
      ui_.comborotation->setCurrentIndex(i);
      break;
    }
  }

  if (!editing && (ui_.combotrack->count() == 0))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No tracks available for this recording, please add a track"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    QTimer::singleShot(1, this, [this]() { reject(); });
  }

  startTimer(std::chrono::milliseconds(40));
}

ManageRecordingJobSourceONVIFWindow::~ManageRecordingJobSourceONVIFWindow()
{
  onvifconnection_.Close();

  if (mediaclient_)
  {
    mediaclient_->Destroy();
    mediaclient_.reset();
  }

  if (deviceclient_)
  {
    deviceclient_->Destroy();
    deviceclient_.reset();
  }

  rtspconnectconnection_.Close();
  rtspconnection_.Close();
  if (rtspclient_)
  {
    rtspclient_->Destroy();
    rtspclient_.reset();
  }
}

void ManageRecordingJobSourceONVIFWindow::timerEvent(QTimerEvent*)
{
  if (deviceclient_ && deviceclient_->IsInitialised())
  {
    deviceclient_->Update();

  }

  if (mediaclient_ && mediaclient_->IsInitialised())
  {
    mediaclient_->Update();

  }
}

void ManageRecordingJobSourceONVIFWindow::SetEnabled(const bool enabled)
{
  ui_.checkenabled->setEnabled(enabled);
  ui_.edituri->setEnabled(enabled);
  ui_.editprofiletoken->setEnabled(enabled);
  ui_.editsourcetag->setEnabled(enabled);
  ui_.comboprotocol->setEnabled(enabled);
  ui_.editusername->setEnabled(enabled);
  ui_.editpassword->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageRecordingJobSourceONVIFWindow::GetProfileCallback(const onvif::Profile& profile)
{
  if (!profile.token_.is_initialized())
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid Profile token</font><br/>");
    return;
  }

  onvif::StreamSetup streamsetup;
  const monocle::StreamingProtocol streamingprotocol = monocle::EnumValuesStreamingProtocol()[ui_.comboprotocol->currentData().toInt()];
  if (streamingprotocol == monocle::StreamingProtocol::UDPUnicast)
  {
    streamsetup = onvif::StreamSetup(onvif::STREAM_RTPUNICAST, onvif::Transport(onvif::TRANSPORTPROTOCOL_UDP));

  }
  else if (streamingprotocol == monocle::StreamingProtocol::TCPInterleaved)
  {
    streamsetup = onvif::StreamSetup(onvif::STREAM_RTPUNICAST, onvif::Transport(onvif::TRANSPORTPROTOCOL_RTSP));

  }
  else if (streamingprotocol == monocle::StreamingProtocol::UDPMulticast)
  {
    streamsetup = onvif::StreamSetup(onvif::STREAM_RTPMULTICAST, onvif::Transport(onvif::TRANSPORTPROTOCOL_UDP));

  }
  else
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid protocol</font><br/>");
    return;
  }

  ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Retrieving Stream URI</font><br/>");
  onvifconnection_ = mediaclient_->GetStreamUriCallback(streamsetup, *profile.token_, [this, profile](const onvif::media::GetStreamUriResponse& getstreamuriresponse)
  {
    if (getstreamuriresponse.Error())
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetStreamUri failed: " + QString::fromStdString(getstreamuriresponse.Message()) + "</font><br/>");
      return;
    }

    if (!getstreamuriresponse.mediauri_.is_initialized() || !getstreamuriresponse.mediauri_->uri_.is_initialized())
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetStreamUri Invalid Media URI</font><br/>");
      return;
    }

    network::uri uri;
    try
    {
      uri = network::uri(*getstreamuriresponse.mediauri_->uri_);

    }
    catch (...)
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid RTSP URI: " + QString::fromStdString(*getstreamuriresponse.mediauri_->uri_) + "</font><br/>");
      return;
    }

    if (!uri.has_scheme())
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid RTSP URI: " + QString::fromStdString(*getstreamuriresponse.mediauri_->uri_) + " no schema present</font><br/>");
      return;
    }

    if (!boost::algorithm::iequals(uri.scheme().to_string(), "rtsp"))
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid RTSP URI schema: " + QString::fromStdString(uri.scheme().to_string()) + "</font><br/>");
      return;
    }

    if (!uri.has_host())
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid RTSP URI: " + QString::fromStdString(*getstreamuriresponse.mediauri_->uri_) + " no host present</font><br/>");
      return;
    }

    uint16_t port = 554;
    if (uri.has_port())
    {
      try
      {
        port = boost::lexical_cast<uint16_t>(uri.port().to_string());

      }
      catch (...)
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid RTSP URI port: " + QString::fromStdString(uri.port().to_string()) + "</font><br/>");
        return;
      }
    }

    rtspclient_ = boost::make_shared< rtsp::Client<ManageRecordingJobSourceONVIFWindow> >(MainWindow::Instance()->GetGUIIOService(), boost::posix_time::seconds(10), boost::posix_time::seconds(60));
    rtspclient_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), uri.host().to_string(), port, ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString());

    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">RTSP Connecting</font><br/>");
    rtspconnectconnection_ = rtspclient_->Connect([this, profile, uri = *getstreamuriresponse.mediauri_->uri_](const boost::system::error_code err)
    {
      if (err)
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Failed to connect</font><br/>");
        return;
      }

      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Connected</font><br/><font color=\"green\">Requesting Options</font><br/>");
      rtspconnection_ = rtspclient_->OptionsCallback(uri, [this, profile, uri](const boost::system::error_code err, const rtsp::OptionsResponse& optionsresponse) mutable
      {
        if (err)
        {
          ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">OPTIONS failed</font><br/>");
          return;
        }

        if (optionsresponse.options_.find(rtsp::headers::REQUESTTYPE_DESCRIBE) == optionsresponse.options_.end())
        {
          ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Describe option not found</font><br/>");
          return;
        }

        if (optionsresponse.options_.find(rtsp::headers::REQUESTTYPE_SETUP) == optionsresponse.options_.end())
        {
          ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Setup option not found</font><br/>");
          return;
        }

        if (optionsresponse.options_.find(rtsp::headers::REQUESTTYPE_PLAY) == optionsresponse.options_.end())
        {
          ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Play option not found</font><br/>");
          return;
        }

        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Options received</font><br/><font color=\"green\">Requesting Describe</font><br/>");
        rtspconnection_ = rtspclient_->DescribeCallback(uri, [this, profile, uri](const boost::system::error_code err, const rtsp::DescribeResponse& describeresponse) mutable
        {
          if (err)
          {
            ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">DESCRIBE failed</font><br/>");
            return;
          }
          ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Describe received</font><br/>");

          const QSharedPointer<client::RecordingTrack> track = recording_->GetTrack(ui_.combotrack->currentData(Qt::UserRole).toUInt());
          if (!track)
          {
            ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Unable to find trackid: " + QString::number(ui_.combotrack->currentData(Qt::UserRole).toUInt()) + "</font><br/>");
            return;
          }

          rtsp::sdp::MEDIATYPE mediatype;
          if (track->GetTrackType() == monocle::TrackType::Video)
          {
            mediatype = rtsp::sdp::MEDIATYPE_VIDEO;

          }
          else if (track->GetTrackType() == monocle::TrackType::Audio)
          {
            mediatype = rtsp::sdp::MEDIATYPE_AUDIO;

          }
          else if (track->GetTrackType() == monocle::TrackType::Metadata)
          {
            mediatype = rtsp::sdp::MEDIATYPE_APPLICATION;

          }
          else
          {
            ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid media type: " + monocle::EnumNameTrackType(track->GetTrackType()) + "</font><br/>");
            return;
          }

          const std::vector<rtsp::sdp::MediaDescription> mediadescriptions = describeresponse.sdp_.GetMediaDescriptions(mediatype);
          if (mediadescriptions.empty())
          {
            ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Media description type not found: " + monocle::EnumNameTrackType(track->GetTrackType()) + "</font><br/>");
            return;
          }

          if (ui_.editsourcetag->text().isEmpty())
          {
            if (mediadescriptions.size() > 1)
            {
              ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"orange\">Media ambiguity; set the source tag to remove this warning</font><br/>");

            }

            AddProfile(profile);

            ui_.treedetails->addTopLevelItem(new QTreeWidgetItem({ "RTSP Uri: " + QString::fromStdString(uri) }));

            for (const rtsp::sdp::MediaDescription& mediadescription : mediadescriptions)
            {
              if (!mediadescription.media_.is_initialized())
              {

                continue;
              }
              AddMediaDescription(mediadescription);
            }
          }
          else
          {
            static const boost::regex sourcetagregex("([A-Za-z]+)=([A-Za-z0-9]+)");
            boost::smatch match;
            const std::string sourcetag = ui_.editsourcetag->text().toStdString();
            if (!boost::regex_match(sourcetag, match, sourcetagregex))
            {
              ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid source tag: " + ui_.editsourcetag->text() + "</font><br/>");
              return;
            }

            if (!boost::algorithm::iequals(match[1].str(), "codec"))
            {
              ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid source tag: " + ui_.editsourcetag->text() + "</font><br/>");
              return;
            }

            int codec = 0;
            try
            {
              codec = boost::lexical_cast<decltype(codec)>(match[2].str());

            }
            catch (...)
            {
              ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid source tag: " + ui_.editsourcetag->text() + "</font><br/>");
              return;
            }

            auto mediadescription = std::find_if(mediadescriptions.cbegin(), mediadescriptions.cend(), [codec](const rtsp::sdp::MediaDescription& mediadescription) { return (mediadescription.media_.is_initialized() && utility::Contains(mediadescription.media_->formats_, codec)); });
            if (mediadescription == mediadescriptions.end())
            {
              ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Source tag: " + ui_.editsourcetag->text() + " not found</font><br/>");
              return;
            }

            AddProfile(profile);

            ui_.treedetails->addTopLevelItem(new QTreeWidgetItem({ "RTSP Uri: " + QString::fromStdString(uri) }));

            AddMediaDescription(*mediadescription);
          }
          ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Success</font><br/>");
        });
      });
    }, []()
    {
      // Do nothing

    });
  });
}

void ManageRecordingJobSourceONVIFWindow::AddProfile(const onvif::Profile& profile)
{
  QTreeWidgetItem* top = new QTreeWidgetItem({ "Profile" });
  ui_.treedetails->addTopLevelItem(top);
  top->setExpanded(true);

  top->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.token_) }));

  if (profile.videosourceconfiguration_->name_.is_initialized())
  {
    top->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.name_) }));

  }

  if (profile.videosourceconfiguration_.is_initialized())
  {
    if (profile.videosourceconfiguration_->token_.is_initialized())
    {
      QTreeWidgetItem* videosourceconfiguration = new QTreeWidgetItem({ "Video Source" });
      top->addChild(videosourceconfiguration);
      videosourceconfiguration->setExpanded(true);

      videosourceconfiguration->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.videosourceconfiguration_->token_) }));

      if (profile.videosourceconfiguration_->name_.is_initialized())
      {
        videosourceconfiguration->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.videosourceconfiguration_->name_) }));

      }

      if (profile.videosourceconfiguration_->bounds_.is_initialized() && profile.videosourceconfiguration_->bounds_->x_.is_initialized() && profile.videosourceconfiguration_->bounds_->y_.is_initialized() && profile.videosourceconfiguration_->bounds_->width_.is_initialized() && profile.videosourceconfiguration_->bounds_->height_.is_initialized())
      {
        QTreeWidgetItem* bounds = new QTreeWidgetItem({ "Bounds" });
        videosourceconfiguration->addChild(bounds);
        bounds->setExpanded(true);

        bounds->addChild(new QTreeWidgetItem({ "x: " + QString::number(*profile.videosourceconfiguration_->bounds_->x_) }));
        bounds->addChild(new QTreeWidgetItem({ "y: " + QString::number(*profile.videosourceconfiguration_->bounds_->y_) }));
        bounds->addChild(new QTreeWidgetItem({ "width: " + QString::number(*profile.videosourceconfiguration_->bounds_->width_) }));
        bounds->addChild(new QTreeWidgetItem({ "height: " + QString::number(*profile.videosourceconfiguration_->bounds_->height_) }));
      }

      if (profile.videosourceconfiguration_->rotate_.is_initialized() && profile.videosourceconfiguration_->rotate_->rotatemode_.is_initialized())
      {
        QTreeWidgetItem* rotation = new QTreeWidgetItem({ "Rotation" });
        videosourceconfiguration->addChild(rotation);
        rotation->setExpanded(true);

        rotation->addChild(new QTreeWidgetItem({ "Mode: " + QString::fromStdString(onvif::ToString(*profile.videosourceconfiguration_->rotate_->rotatemode_)) }));
        if (profile.videosourceconfiguration_->rotate_->degree_.is_initialized())
        {
          rotation->addChild(new QTreeWidgetItem({ "Degree: " + QString::number(*profile.videosourceconfiguration_->rotate_->degree_) }));

        }
        else
        {
          rotation->addChild(new QTreeWidgetItem({ "Degree: 180" })); // Default according to ONVIF

        }
      }
    }
  }

  if (profile.audiosourceconfiguration_.is_initialized())
  {
    if (profile.audiosourceconfiguration_->token_.is_initialized())
    {
      QTreeWidgetItem* audiosourceconfiguration = new QTreeWidgetItem({ "Audio Source" });
      top->addChild(audiosourceconfiguration);
      audiosourceconfiguration->setExpanded(true);

      audiosourceconfiguration->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.audiosourceconfiguration_->token_) }));

      if (profile.audiosourceconfiguration_->name_.is_initialized())
      {
        audiosourceconfiguration->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.audiosourceconfiguration_->name_) }));

      }

      if (profile.audiosourceconfiguration_->sourcetoken_.is_initialized())
      {
        audiosourceconfiguration->addChild(new QTreeWidgetItem({ "Source Token: " + QString::fromStdString(*profile.audiosourceconfiguration_->sourcetoken_) }));

      }
    }
  }

  if (profile.videoencoderconfiguration_.is_initialized())
  {
    if (profile.videoencoderconfiguration_->token_.is_initialized())
    {
      QTreeWidgetItem* videoencoderconfiguration = new QTreeWidgetItem({ "Video Encoder" });
      top->addChild(videoencoderconfiguration);
      videoencoderconfiguration->setExpanded(true);

      videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.videoencoderconfiguration_->token_) }));

      if (profile.videoencoderconfiguration_->name_.is_initialized())
      {
        videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.videoencoderconfiguration_->name_) }));

      }

      if (profile.videoencoderconfiguration_->encoding_.is_initialized())
      {
        videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Encoding: " + QString::fromStdString(onvif::ToString(*profile.videoencoderconfiguration_->encoding_)) }));

        if ((*profile.videoencoderconfiguration_->encoding_ == onvif::VIDEOENCODING_MPEG4) && profile.videoencoderconfiguration_->mpeg4_.is_initialized() && profile.videoencoderconfiguration_->mpeg4_->govlength_.is_initialized() && profile.videoencoderconfiguration_->mpeg4_->mpeg4profile_.is_initialized())
        {
          videoencoderconfiguration->addChild(new QTreeWidgetItem({ "GovLength: " + QString::number(*profile.videoencoderconfiguration_->mpeg4_->govlength_) }));
          videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Profile: " + QString::fromStdString(onvif::ToString(*profile.videoencoderconfiguration_->mpeg4_->mpeg4profile_)) }));
        }

        if ((*profile.videoencoderconfiguration_->encoding_ == onvif::VIDEOENCODING_H264) && profile.videoencoderconfiguration_->h264_.is_initialized() && profile.videoencoderconfiguration_->h264_->govlength_.is_initialized() && profile.videoencoderconfiguration_->h264_->h264profile_.is_initialized())
        {
          videoencoderconfiguration->addChild(new QTreeWidgetItem({ "GovLength: " + QString::number(*profile.videoencoderconfiguration_->h264_->govlength_) }));
          videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Profile: " + QString::fromStdString(onvif::ToString(*profile.videoencoderconfiguration_->h264_->h264profile_)) }));
        }
      }

      if (profile.videoencoderconfiguration_->resolution_.is_initialized() && profile.videoencoderconfiguration_->resolution_->width_.is_initialized() && profile.videoencoderconfiguration_->resolution_->height_.is_initialized())
      {
        videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Resolution: " + QString::number(*profile.videoencoderconfiguration_->resolution_->width_) + "x" + QString::number(*profile.videoencoderconfiguration_->resolution_->height_) }));

      }

      if (profile.videoencoderconfiguration_->quality_.is_initialized())
      {
        videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Quality: " + QString::number(*profile.videoencoderconfiguration_->quality_) }));

      }
    }
  }

  if (profile.audioencoderconfiguration_.is_initialized())
  {
    if (profile.audioencoderconfiguration_->token_.is_initialized())
    {
      QTreeWidgetItem* audioencoderconfiguration = new QTreeWidgetItem({ "Audio Encoder" });
      top->addChild(audioencoderconfiguration);
      audioencoderconfiguration->setExpanded(true);

      audioencoderconfiguration->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.audioencoderconfiguration_->token_) }));

      if (profile.audioencoderconfiguration_->name_.is_initialized())
      {
        audioencoderconfiguration->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.audioencoderconfiguration_->name_) }));

      }

      if (profile.audioencoderconfiguration_->encoding_.is_initialized())
      {
        audioencoderconfiguration->addChild(new QTreeWidgetItem({ "Encoding: " + QString::fromStdString(onvif::ToString(*profile.audioencoderconfiguration_->encoding_)) }));

      }

      if (profile.audioencoderconfiguration_->bitrate_.is_initialized())
      {
        audioencoderconfiguration->addChild(new QTreeWidgetItem({ "Bitrate: " + QString::number(*profile.audioencoderconfiguration_->bitrate_) }));

      }

      if (profile.audioencoderconfiguration_->samplerate_.is_initialized())
      {
        audioencoderconfiguration->addChild(new QTreeWidgetItem({ "Samplerate: " + QString::number(*profile.audioencoderconfiguration_->samplerate_) }));

      }
    }
  }

  if (profile.ptzconfiguration_.is_initialized())
  {
    if (profile.ptzconfiguration_->token_.is_initialized())
    {
      QTreeWidgetItem* ptzconfiguration = new QTreeWidgetItem({ "PTZ" });
      top->addChild(ptzconfiguration);
      ptzconfiguration->setExpanded(true);

      ptzconfiguration->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.ptzconfiguration_->token_) }));

      if (profile.ptzconfiguration_->name_.is_initialized())
      {
        ptzconfiguration->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.ptzconfiguration_->name_) }));

      }

      if (profile.ptzconfiguration_->nodetoken_.is_initialized())
      {
        ptzconfiguration->addChild(new QTreeWidgetItem({ "Node: " + QString::fromStdString(*profile.ptzconfiguration_->nodetoken_) }));

      }
    }
  }
  
  if (profile.metadataconfiguration_.is_initialized())
  {
    if (profile.metadataconfiguration_->token_.is_initialized())
    {
      QTreeWidgetItem* metadataconfiguration_ = new QTreeWidgetItem({ "PTZ" });
      top->addChild(metadataconfiguration_);
      metadataconfiguration_->setExpanded(true);

      metadataconfiguration_->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.metadataconfiguration_->token_) }));

      if (profile.metadataconfiguration_->name_.is_initialized())
      {
        metadataconfiguration_->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.metadataconfiguration_->name_) }));

      }

      if (profile.metadataconfiguration_->compressiontype_.is_initialized())
      {
        metadataconfiguration_->addChild(new QTreeWidgetItem({ "Compression: " + QString::fromStdString(onvif::ToString(*profile.metadataconfiguration_->compressiontype_)) }));

      }
    }
  }
}

void ManageRecordingJobSourceONVIFWindow::AddMediaDescription(const rtsp::sdp::MediaDescription& mediadescription)
{
  const std::vector<std::string> tmp = mediadescription.ToString();
  if (tmp.empty())
  {
    // Shouldn't be possible so just ignore it I guess?
    return;
  }

  QTreeWidgetItem* top = new QTreeWidgetItem({ QString::fromStdString(tmp.front()) });
  ui_.treedetails->addTopLevelItem(top);
  for (std::vector<std::string>::const_iterator i = tmp.cbegin() + 1; i != tmp.cend(); ++i)
  {
    if (i->empty())
    {

      continue;
    }
    top->addChild(new QTreeWidgetItem({ QString::fromStdString(*i) }));
  }
  top->setExpanded(true);
}

void ManageRecordingJobSourceONVIFWindow::TrackAdded(const QSharedPointer<client::RecordingTrack>& track)
{
  ui_.combotrack->addItem(track->GetDescription() + "(" + monocle::EnumNameTrackType(track->GetTrackType()) + ")", track->GetId());

}

void ManageRecordingJobSourceONVIFWindow::TrackChanged(const QSharedPointer<client::RecordingTrack>& track)
{
  const int row = ui_.combotrack->findData(track->GetId());
  if (row == -1)
  {

    return;
  }
  ui_.combotrack->setItemText(0, track->GetDescription() + "(" + monocle::EnumNameTrackType(track->GetTrackType()) + ")");
}

void ManageRecordingJobSourceONVIFWindow::TrackRemoved(const uint32_t id)
{
  const int row = ui_.combotrack->findData(id);
  if (row == -1)
  {

    return;
  }
  ui_.combotrack->removeItem(row);
}

void ManageRecordingJobSourceONVIFWindow::on_edittextfilter_textChanged()
{
  ui_.treediscovery->SetTextFilter(ui_.edittextfilter->text());

}

void ManageRecordingJobSourceONVIFWindow::on_checkipv4_clicked()
{
  ui_.treediscovery->ShowIpv4(ui_.checkipv4->isChecked());

}

void ManageRecordingJobSourceONVIFWindow::on_checkipv6_clicked()
{
  ui_.treediscovery->ShowIpv6(ui_.checkipv6->isChecked());

}

void ManageRecordingJobSourceONVIFWindow::on_checklinklocal_clicked()
{
  ui_.treediscovery->ShowLinkLocal(ui_.checklinklocal->isChecked());

}

void ManageRecordingJobSourceONVIFWindow::on_buttondiscoverrefresh_clicked()
{
  ui_.treediscovery->SetUserPass(ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString());
  ui_.treediscovery->Refresh();
}

void ManageRecordingJobSourceONVIFWindow::on_treediscovery_itemPressed(QTreeWidgetItem* item, int column)
{
  if (item && (item->data(0, Qt::UserRole) == RECEIVERDISCOVERYITEM_DEVICE))
  {
    ui_.edituri->setText(item->data(0, Qt::UserRole + 1).toString());
  }
  else if (item && (item->data(0, Qt::UserRole) == RECEIVERDISCOVERYITEM_PROFILE))
  {
    ui_.edituri->setText(item->data(0, Qt::UserRole + 1).toString());
    ui_.editprofiletoken->setText(item->data(0, Qt::UserRole + 2).toString());
  }
}

void ManageRecordingJobSourceONVIFWindow::on_buttontest_clicked()
{
  ui_.labeltestoutput->clear();
  ui_.treedetails->clear();

  rtspconnectconnection_.Close();
  rtspconnection_.Close();
  rtspclient_.reset();
  onvifconnection_.Close();
  if (mediaclient_)
  {
    mediaclient_->Destroy();
    mediaclient_.reset();
  }
  if (deviceclient_)
  {
    deviceclient_->Destroy();
    deviceclient_.reset();
  }

  network::uri uri;
  try
  {
    uri = network::uri(ui_.edituri->text().toStdString());

  }
  catch (...)
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI: " + ui_.edituri->text() + "</font><br/>");
    return;
  }

  if (!uri.has_scheme())
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI: " + ui_.edituri->text() + " no schema present</font><br/>");
    return;
  }

  if (!boost::algorithm::iequals(uri.scheme().to_string(), "http"))
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI schema: " + QString::fromStdString(uri.scheme().to_string()) + "</font><br/>");
    return;
  }

  if (!uri.has_host())
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI: " + ui_.edituri->text() + " no host present</font><br/>");
    return;
  }

  uint16_t port = 80;
  if (uri.has_port())
  {
    try
    {
      port = boost::lexical_cast<uint16_t>(uri.port().to_string());

    }
    catch (...)
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI port: " + QString::fromStdString(uri.port().to_string()) + "</font><br/>");
      return;
    }
  }

  deviceclient_ = boost::make_shared<onvif::device::DeviceClient>();
  mediaclient_ = boost::make_shared<onvif::media::MediaClient>();
  if (deviceclient_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), ui_.edituri->text().toStdString(), ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString(), 1, false, true))
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Failed to initialise ONVIF Device Client</font><br/>");
    return;
  }

  ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Retrieving System Date and Time</font><br/>");
  onvifconnection_ = deviceclient_->GetSystemDateAndTimeCallback([this](const onvif::device::GetSystemDateAndTimeResponse& getsystemdateandtimeresponse)
  {
    if (getsystemdateandtimeresponse.Message() == onvif::UNABLETOCONNECT)
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetSystemDateAndTime failed: " + QString::fromStdString(getsystemdateandtimeresponse.Message()) + "</font><br/>");
      return;
    }
    else if (getsystemdateandtimeresponse.Error())
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"orange\">GetSystemDateAndTime failed: " + QString::fromStdString(getsystemdateandtimeresponse.Message()) + "</font><br/>");

    }

    if (getsystemdateandtimeresponse.systemdatetime_.is_initialized())
    {
      deviceclient_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
      mediaclient_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
    }

    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Retrieving Capabilities</font><br/>");
    onvifconnection_ = deviceclient_->GetCapabilitiesCallback(onvif::CAPABILITYCATEGORY::CAPABILITYCATEGORY_ALL, [this](const onvif::device::GetCapabilitiesResponse& getcapabilitiesresponse)
    {
      if (getcapabilitiesresponse.Error())
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetCapabilities failed: " + QString::fromStdString(getcapabilitiesresponse.Message()) + "</font><br/>");
        return;
      }

      if (!getcapabilitiesresponse.capabilities_.is_initialized())
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetCapabilities invalid response no capabilities found</font><br/>");
        return;
      }

      if (!getcapabilitiesresponse.capabilities_->media_.is_initialized())
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetCapabilities invalid response no media capabilities</font><br/>");
        return;
      }

      const std::string mediaxaddr = getcapabilitiesresponse.capabilities_->media_->GetXAddr();
      if (mediaxaddr.empty())
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetCapabilities invalid response no media XADDR</font><br/>");
        return;
      }

      if (mediaclient_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), mediaxaddr, deviceclient_->GetUsername(), deviceclient_->GetPassword(), 1, false, true))
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Failed to initialise Media client</font><br/>");
        return;
      }

      if (ui_.editprofiletoken->text().isEmpty())
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Retrieving Profiles</font><br/>");
        onvifconnection_ = mediaclient_->GetProfilesCallback([this](const onvif::media::GetProfilesResponse& getprofilesresponse)
        {
          if (getprofilesresponse.Error())
          {
            ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetProfiles failed: " + QString::fromStdString(getprofilesresponse.Message()) + "</font><br/>");
            return;
          }

          if (getprofilesresponse.profiles_.empty())
          {
            ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">No profiles found</font><br/>");
            return;
          }

          GetProfileCallback(getprofilesresponse.profiles_.front());
        });
      }
      else
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Retrieving Profile</font><br/>");
        onvifconnection_ = mediaclient_->GetProfileCallback(ui_.editprofiletoken->text().toStdString(), [this](const onvif::media::GetProfileResponse& getprofileresponse)
        {
          if (getprofileresponse.Error())
          {
            ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetProfile failed: " + QString::fromStdString(getprofileresponse.Message()) + "</font><br/>");
            return;
          }

          if (!getprofileresponse.profile_.is_initialized())
          {
            ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Profile not initialised</font><br/>");
            return;
          }

          GetProfileCallback(*getprofileresponse.profile_);
        });
      }
    });
  });
}

void ManageRecordingJobSourceONVIFWindow::on_editusername_editingFinished()
{
  ui_.treediscovery->SetUserPass(ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString());

}

void ManageRecordingJobSourceONVIFWindow::on_editpassword_editingFinished()
{
  ui_.treediscovery->SetUserPass(ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString());

}

void ManageRecordingJobSourceONVIFWindow::on_buttonok_clicked()
{
  if (ui_.combotrack->currentIndex() == -1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), "Track not selected", QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  network::uri uri;
  try
  {
    uri = network::uri(ui_.edituri->text().toStdString());

  }
  catch (...)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), "Invalid URI: " + ui_.edituri->text(), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (!uri.has_scheme())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), "Invalid URI: " + ui_.edituri->text(), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (!boost::algorithm::iequals(uri.scheme().to_string(), "http"))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), "Invalid URI schema: " + QString::fromStdString(uri.scheme().to_string()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (!uri.has_host())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), "Invalid URI: " + ui_.edituri->text(), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  enabled_ = ui_.checkenabled->isChecked();
  trackid_ = ui_.combotrack->currentData().toUInt();
  uri_ = ui_.edituri->text();
  profiletoken_ = ui_.editprofiletoken->text();
  sourcetag_ = ui_.editsourcetag->text();
  streamingprotocol_ = monocle::EnumValuesStreamingProtocol()[ui_.comboprotocol->currentData().toUInt()];
  username_ = ui_.editusername->text();
  password_ = ui_.editpassword->text();
  rotation_ = ui_.comborotation->currentData(Qt::UserRole).toString();
  accept();
}

}

QT_END_NAMESPACE
