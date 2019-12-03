// managetrackwindow.cpp
//

///// Includes /////

#include "monocleclient/managetrackwindow.h"

#include <monocleprotocol/tracktype_generated.h>

#include "monocleclient/device.h"
#include "monocleclient/recording.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageTrackWindow::ManageTrackWindow(QWidget* parent, const monocle::TrackType tracktype, boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording) :
  QDialog(parent),
  device_(device),
  recording_(recording)
{
  ui_.setupUi(this);

  //TODO cancel reject connect

  //TODO fill in rotation and protocol?
  
  if (tracktype == monocle::TrackType::Video)
  {

  }
  else if (tracktype == monocle::TrackType::Metadata)
  {
    //TODO remove some bits

  }
  else
  {
    //TODO error and reject in 1ms

  }
}

ManageTrackWindow::~ManageTrackWindow()
{
  addtrack2connection_.Close();

}

//TODO on checkfiles enable button
//TODO on files
//TODO on find onvif device
//TODO on object detector checkbox(enable settings)
//TODO on object detector settings button

void ManageTrackWindow::on_buttonok_clicked()
{
  //TODO check uri looks like rtsp or http://1.2.3.4:99/onvif/device_service

  //TODO disable buttons

  //TODO scopedconnection
  //TODO pass in the recording
  //TODO pass in the track
  //TODO pass in the recording job
  //TODO pass in the uri/username/protocol etc
  //TODO we will want to pass in the object detector track and rjst too...
  addtrack2connection_ = device_->AddTrack2(recording_->GetToken(), [this](const std::chrono::steady_clock::duration latency, const monocle::client::ADDTRACK2RESPONSE& addtrack2response)
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
