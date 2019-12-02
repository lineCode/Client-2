// managerecordingjobsourcertspwindow.cpp
//

///// Includes /////

#include "monocleclient/managerecordingjobsourcertspwindow.h"

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <monocleprotocol/streamingprotocol_generated.h>
#include <network/uri.hpp>
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

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageRecordingJobSourceRTSPWindow::ManageRecordingJobSourceRTSPWindow(QWidget* parent,
                                                                       const boost::shared_ptr<Device>& device,
                                                                       const QSharedPointer<client::Recording>& recording,
                                                                       const QSharedPointer<client::RecordingJob>& recordingjob,
                                                                       const bool editing,
                                                                       const bool enabled,
                                                                       const uint32_t trackid,
                                                                       const QString& mediauri,
                                                                       const QString& sourcetag,
                                                                       const monocle::StreamingProtocol streamingprotocol,
                                                                       const QString& username,
                                                                       const QString& password,
                                                                       const QString& rotation,
                                                                       const std::vector<uint32_t>& reservedtrackids) :
  QDialog(parent),
  device_(device),
  recording_(recording),
  recordingjob_(recordingjob)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);
  connect(recording_.data(), &Recording::TrackAdded, this, &ManageRecordingJobSourceRTSPWindow::TrackAdded, Qt::QueuedConnection);
  connect(recording_.data(), &Recording::TrackChanged, this, &ManageRecordingJobSourceRTSPWindow::TrackChanged, Qt::QueuedConnection);
  connect(recording_.data(), &Recording::TrackRemoved, this, &ManageRecordingJobSourceRTSPWindow::TrackRemoved, Qt::QueuedConnection);

  if (editing)
  {
    setWindowTitle("Edit RTSP Source");

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
}

ManageRecordingJobSourceRTSPWindow::~ManageRecordingJobSourceRTSPWindow()
{
  rtspconnectconnection_.Close();
  rtspconnection_.Close();
  if (rtspclient_)
  {
    rtspclient_->Destroy();
    rtspclient_.reset();
  }
}

void ManageRecordingJobSourceRTSPWindow::SetEnabled(const bool enabled)
{
  ui_.checkenabled->setEnabled(enabled);
  ui_.edituri->setEnabled(enabled);
  ui_.editsourcetag->setEnabled(enabled);
  ui_.comboprotocol->setEnabled(enabled);
  ui_.editusername->setEnabled(enabled);
  ui_.editpassword->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageRecordingJobSourceRTSPWindow::AddMediaDescription(const rtsp::sdp::MediaDescription& mediadescription)
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

void ManageRecordingJobSourceRTSPWindow::TrackAdded(const QSharedPointer<client::RecordingTrack>& track)
{
  ui_.combotrack->addItem(track->GetDescription() + "(" + monocle::EnumNameTrackType(track->GetTrackType()) + ")", track->GetId());

}

void ManageRecordingJobSourceRTSPWindow::TrackChanged(const QSharedPointer<client::RecordingTrack>& track)
{
  const int row = ui_.combotrack->findData(track->GetId());
  if (row == -1)
  {

    return;
  }
  ui_.combotrack->setItemText(0, track->GetDescription() + "(" + monocle::EnumNameTrackType(track->GetTrackType()) + ")");
}

void ManageRecordingJobSourceRTSPWindow::TrackRemoved(const uint32_t id)
{
  const int row = ui_.combotrack->findData(id);
  if (row == -1)
  {

    return;
  }
  ui_.combotrack->removeItem(row);
}

void ManageRecordingJobSourceRTSPWindow::on_buttontest_clicked()
{
  ui_.labeltestoutput->clear();
  ui_.treedetails->clear();

  rtspconnectconnection_.Close();
  rtspconnection_.Close();
  rtspclient_.reset();

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

  if (!boost::algorithm::iequals(uri.scheme().to_string(), "rtsp"))
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI schema: " + QString::fromStdString(uri.scheme().to_string()) + "</font><br/>");
    return;
  }

  if (!uri.has_host())
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI: " + ui_.edituri->text() + " no host present</font><br/>");
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
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI port: " + QString::fromStdString(uri.port().to_string()) + "</font><br/>");
      return;
    }
  }

  rtspclient_ = boost::make_shared< rtsp::Client<ManageRecordingJobSourceRTSPWindow> >(MainWindow::Instance()->GetGUIIOService(), boost::posix_time::seconds(10), boost::posix_time::seconds(60));
  rtspclient_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), uri.host().to_string(), port, ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString());

  ui_.labeltestoutput->clear();
  ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Connecting</font><br/>");
  rtspconnectconnection_ = rtspclient_->Connect([this](const boost::system::error_code err)
  {
    if (err)
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Failed to connect</font><br/>");
      return;
    }

    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Connected</font><br/><font color=\"green\">Requesting Options</font><br/>");
    rtspconnection_ = rtspclient_->OptionsCallback(ui_.edituri->text().toStdString(), [this](const boost::system::error_code err, const rtsp::OptionsResponse& optionsresponse) mutable
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
      rtspconnection_ = rtspclient_->DescribeCallback(ui_.edituri->text().toStdString(), [this](const boost::system::error_code err, const rtsp::DescribeResponse& describeresponse) mutable
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
        else if ((track->GetTrackType() == monocle::TrackType::Metadata) || (track->GetTrackType() == monocle::TrackType::ObjectDetector))
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
          static const boost::regex sourcetagregex("([A-Za-z0-9]+)=([A-Za-z0-9]+)");
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

          if (!mediadescription->media_.is_initialized())
          {
            ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid Media Description not found</font><br/>");
            return;
          }

          AddMediaDescription(*mediadescription);
        }
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Success</font><br/>");
      });
    });
  }, []()
  {
    // Do nothing

  });
}

void ManageRecordingJobSourceRTSPWindow::on_buttonok_clicked()
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

  if (!boost::algorithm::iequals(uri.scheme().to_string(), "rtsp"))
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
  sourcetag_ = ui_.editsourcetag->text();
  streamingprotocol_ = monocle::EnumValuesStreamingProtocol()[ui_.comboprotocol->currentData().toUInt()];
  username_ = ui_.editusername->text();
  password_ = ui_.editpassword->text();
  rotation_ = ui_.comborotation->currentData(Qt::UserRole).toString();
  accept();
}

}

QT_END_NAMESPACE
