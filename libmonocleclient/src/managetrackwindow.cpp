// managetrackwindow.cpp
//

///// Includes /////

#include "monocleclient/managetrackwindow.h"

#include <monocleprotocol/streamingprotocol_generated.h>
#include <monocleprotocol/tracktype_generated.h>
#include <network/uri.hpp>
#include <QString>

#include "monocleclient/device.h"
#include "monocleclient/recording.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageTrackWindow::ManageTrackWindow(QWidget* parent, const monocle::TrackType tracktype, boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording) :
  QDialog(parent),
  tracktype_(tracktype),
  device_(device),
  recording_(recording)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &ManageTrackWindow::reject);

  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::TCPInterleaved)), static_cast<int>(monocle::StreamingProtocol::TCPInterleaved));
  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::UDPUnicast)), static_cast<int>(monocle::StreamingProtocol::UDPUnicast));
  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::UDPMulticast)), static_cast<int>(monocle::StreamingProtocol::UDPMulticast));

  ui_.comborotation->addItem("0", "0");
  ui_.comborotation->addItem("90", "90");
  ui_.comborotation->addItem("180", "180");
  ui_.comborotation->addItem("270", "270");
  
  if (tracktype == monocle::TrackType::Video)
  {
    //TODO find the object detector and put the checkobject thing in

  }
  else if (tracktype == monocle::TrackType::Metadata)
  {
    ui_.checkobjectdetector->setVisible(false);
    ui_.buttonobjectdetectorsettings->setVisible(false);
  }
  else
  {
    //TODO error and reject in 1ms

  }

  //TODO select and fill in the things if there is a stuff editing to put in

  on_checkfixedfiles_stateChanged(0);
  on_checkobjectdetector_stateChanged(0);
}

ManageTrackWindow::~ManageTrackWindow()
{
  addtrack2connection_.Close();

}

void ManageTrackWindow::DisableSource()
{
  ui_.editprofiletoken->setEnabled(false);
  ui_.editsourcetag->setEnabled(false);
  ui_.comboprotocol->setEnabled(false);
  ui_.editusername->setEnabled(false);
  ui_.editpassword->setEnabled(false);
  ui_.comborotation->setEnabled(false);
  ui_.checkobjectdetector->setEnabled(false);
  ui_.buttonobjectdetectorsettings->setEnabled(false);
}

void ManageTrackWindow::on_edituri_textChanged(const QString& text)
{
  try
  {
    const network::uri uri(text.toStdString());
    if (!uri.has_scheme())
    {
      DisableSource();
      //TODO disable test
      return;
    }

    if (uri.scheme().compare("rtsp") == 0)
    {
      ui_.editprofiletoken->setEnabled(false);
      ui_.editsourcetag->setEnabled(true);
      ui_.comboprotocol->setEnabled(true);
      ui_.editusername->setEnabled(true);
      ui_.editpassword->setEnabled(true);
      ui_.comborotation->setEnabled(true);
      ui_.checkobjectdetector->setEnabled(true);
      ui_.buttonobjectdetectorsettings->setEnabled(true);
      //TODO enable test
    }
    else if ((uri.scheme().compare("http") == 0) && uri.has_path() && (uri.path().compare("/onvif/device_service") == 0))//TODO check it works
    {
      ui_.editprofiletoken->setEnabled(true);
      ui_.editsourcetag->setEnabled(true);
      ui_.comboprotocol->setEnabled(true);
      ui_.editusername->setEnabled(true);
      ui_.editpassword->setEnabled(true);
      ui_.comborotation->setEnabled(true);
      ui_.checkobjectdetector->setEnabled(true);
      ui_.buttonobjectdetectorsettings->setEnabled(true);
      //TODO enable test
    }
    else
    {
      DisableSource();
      //TODO disable test

    }
  }
  catch (...)
  {
    DisableSource();
    //TODO disable test

  }
}

void ManageTrackWindow::on_checkfixedfiles_stateChanged(int)
{
  ui_.buttonfiles->setEnabled(ui_.checkfixedfiles->isChecked());

}

void ManageTrackWindow::on_checkobjectdetector_stateChanged(int)
{
  ui_.buttonobjectdetectorsettings->setEnabled(ui_.checkobjectdetector->isChecked());

}

void ManageTrackWindow::on_buttonfindonvifdevice_clicked()
{
  //TODO open the find a onvif device window...
}
//TODO on find onvif device
void ManageTrackWindow::on_buttonok_clicked()
{
  //TODO just pass in all the source details, and the server can create the source and rjst if needed

  //TODO make sure uri is valid

  //TODO find a recording job
    //TODO create one if necessary

  //TODO check uri looks like rtsp or http://1.2.3.4:99/onvif/device_service

  //TODO get the recording job...
    //TODO get the active recording job, if none, then get the highest priority, if none, create a job...

  //TODO pass in the recording job
  //TODO pass in the uri/username/protocol etc
  //TODO we will want to pass in the object detector track and rjst too...
  //TODO files needs to be sorted
  addtrack2connection_ = device_->AddTrack2(recording_->GetToken(), tracktype_, ui_.editdescription->text().toStdString(), ui_.checkfixedfiles->isChecked(), ui_.checkdigitalsigning->isChecked(), ui_.checkencrypt->isChecked(), ui_.spinflushfrequency->value(), {}, [this](const std::chrono::steady_clock::duration latency, const monocle::client::ADDTRACK2RESPONSE& addtrack2response)
  {

    //TODO enable buttons... again
      //TODO just create a method for tidy up all the enabled/disabled

    if (addtrack2response.GetErrorCode() != monocle::ErrorCode::Success)
    {
      //TODO QMessageBox
      return;
    }

    accept();
  });
}

}

QT_END_NAMESPACE
