// client.hpp
//

///// Includes /////

#include <boost/asio.hpp>

#include "monocleprotocol/client/client.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/beast.hpp>
#include <boost/bind.hpp>
#include <socket/connectionblock.hpp>
#include <utility/utility.hpp>

#include "monocleprotocol/addfilerequest_generated.h"
#include "monocleprotocol/addgrouprequest_generated.h"
#include "monocleprotocol/addlayoutrequest_generated.h"
#include "monocleprotocol/addmaprequest_generated.h"
#include "monocleprotocol/addonvifuserrequest_generated.h"
#include "monocleprotocol/addreceiverrequest_generated.h"
#include "monocleprotocol/addrecordingjobrequest_generated.h"
#include "monocleprotocol/addrecordingjobresponse_generated.h"
#include "monocleprotocol/addrecordingjobsource_generated.h"
#include "monocleprotocol/addrecordingrequest_generated.h"
#include "monocleprotocol/addrecordingresponse_generated.h"
#include "monocleprotocol/addtrackrequest_generated.h"
#include "monocleprotocol/addtrackrequest2_generated.h"
#include "monocleprotocol/addtrackresponse_generated.h"
#include "monocleprotocol/adduserrequest_generated.h"
#include "monocleprotocol/authenticaterequest_generated.h"
#include "monocleprotocol/changegrouprequest_generated.h"
#include "monocleprotocol/changemaprequest_generated.h"
#include "monocleprotocol/changeonvifuserrequest_generated.h"
#include "monocleprotocol/changereceiverrequest_generated.h"
#include "monocleprotocol/changerecordingjobrequest_generated.h"
#include "monocleprotocol/changerecordingjobsource_generated.h"
#include "monocleprotocol/changerecordingrequest_generated.h"
#include "monocleprotocol/changetrackrequest_generated.h"
#include "monocleprotocol/changetrackrequest2_generated.h"
#include "monocleprotocol/changeuserrequest_generated.h"
#include "monocleprotocol/codecindex_generated.h"
#include "monocleprotocol/controlstreamend_generated.h"
#include "monocleprotocol/controlstreamframesteprequest_generated.h"
#include "monocleprotocol/controlstreamplayrequest_generated.h"
#include "monocleprotocol/controlstreamliverequest_generated.h"
#include "monocleprotocol/controlstreampauserequest_generated.h"
#include "monocleprotocol/createfindmotionrequest_generated.h"
#include "monocleprotocol/createfindmotionresponse_generated.h"
#include "monocleprotocol/createfindobjectrequest_generated.h"
#include "monocleprotocol/createfindobjectresponse_generated.h"
#include "monocleprotocol/createstreamrequest_generated.h"
#include "monocleprotocol/createstreamresponse_generated.h"
#include "monocleprotocol/destroyfindmotionrequest_generated.h"
#include "monocleprotocol/destroyfindobjectrequest_generated.h"
#include "monocleprotocol/destroystreamrequest_generated.h"
#include "monocleprotocol/discoveryhello_generated.h"
#include "monocleprotocol/filemonitorstatechanged_generated.h"
#include "monocleprotocol/fileremoved_generated.h"
#include "monocleprotocol/filestatechanged_generated.h"
#include "monocleprotocol/findmotionend_generated.h"
#include "monocleprotocol/findmotionprogress_generated.h"
#include "monocleprotocol/findmotionresult_generated.h"
#include "monocleprotocol/findobjectend_generated.h"
#include "monocleprotocol/findobjectprogress_generated.h"
#include "monocleprotocol/findobjectresult_generated.h"
#include "monocleprotocol/getauthenticationnonceresponse_generated.h"
#include "monocleprotocol/getchildfoldersrequest_generated.h"
#include "monocleprotocol/getchildfoldersresponse_generated.h"
#include "monocleprotocol/getfilesresponse_generated.h"
#include "monocleprotocol/getreceiversresponse_generated.h"
#include "monocleprotocol/getrecordingsresponse_generated.h"
#include "monocleprotocol/getsnapshotrequest_generated.h"
#include "monocleprotocol/getsnapshotresponse_generated.h"
#include "monocleprotocol/getstateresponse_generated.h"
#include "monocleprotocol/gettimeresponse_generated.h"
#include "monocleprotocol/gpustat_generated.h"
#include "monocleprotocol/groupadded_generated.h"
#include "monocleprotocol/groupchanged_generated.h"
#include "monocleprotocol/groupremoved_generated.h"
#include "monocleprotocol/h265frameheader_generated.h"
#include "monocleprotocol/h264frameheader_generated.h"
#include "monocleprotocol/hardwarestats_generated.h"
#include "monocleprotocol/jpegframeheader_generated.h"
#include "monocleprotocol/layoutadded_generated.h"
#include "monocleprotocol/locationchanged_generated.h"
#include "monocleprotocol/mapadded_generated.h"
#include "monocleprotocol/mapchanged_generated.h"
#include "monocleprotocol/mapremoved_generated.h"
#include "monocleprotocol/metadataframeheader_generated.h"
#include "monocleprotocol/mountfilerequest_generated.h"
#include "monocleprotocol/mountpointadded_generated.h"
#include "monocleprotocol/mountpointremoved_generated.h"
#include "monocleprotocol/mpeg4frameheader_generated.h"
#include "monocleprotocol/namechanged_generated.h"
#include "monocleprotocol/newcodecindex_generated.h"
#include "monocleprotocol/objectdetectorframeheader_generated.h"
#include "monocleprotocol/onvifuseradded_generated.h"
#include "monocleprotocol/onvifuserchanged_generated.h"
#include "monocleprotocol/onvifuserremoved_generated.h"
#include "monocleprotocol/receiverchanged_generated.h"
#include "monocleprotocol/receiverremoved_generated.h"
#include "monocleprotocol/recordingactivejobchanged_generated.h"
#include "monocleprotocol/recordingadded_generated.h"
#include "monocleprotocol/recordingchanged_generated.h"
#include "monocleprotocol/recordingjobadded_generated.h"
#include "monocleprotocol/recordingjobchanged_generated.h"
#include "monocleprotocol/recordingjoblogmessage_generated.h"
#include "monocleprotocol/recordingjobremoved_generated.h"
#include "monocleprotocol/recordingjobsourceadded_generated.h"
#include "monocleprotocol/recordingjobsourceremoved_generated.h"
#include "monocleprotocol/recordingjobsourcetrackactiveparameterschanged_generated.h"
#include "monocleprotocol/recordingjobsourcetrackadded_generated.h"
#include "monocleprotocol/recordingjobsourcetracklogmessage_generated.h"
#include "monocleprotocol/recordingjobsourcetrackremoved_generated.h"
#include "monocleprotocol/recordingjobsourcetrackstatechanged_generated.h"
#include "monocleprotocol/recordinglogmessage_generated.h"
#include "monocleprotocol/recordingremoved_generated.h"
#include "monocleprotocol/recordingsstatistics_generated.h"
#include "monocleprotocol/recordingtrackcodecadded_generated.h"
#include "monocleprotocol/recordingtrackcodecremoved_generated.h"
#include "monocleprotocol/recordingtracklogmessage_generated.h"
#include "monocleprotocol/removefilerequest_generated.h"
#include "monocleprotocol/removegrouprequest_generated.h"
#include "monocleprotocol/removemaprequest_generated.h"
#include "monocleprotocol/removeonvifuserrequest_generated.h"
#include "monocleprotocol/removereceiverrequest_generated.h"
#include "monocleprotocol/removerecordingjobrequest_generated.h"
#include "monocleprotocol/removerecordingjobsourcerequest_generated.h"
#include "monocleprotocol/removerecordingrequest_generated.h"
#include "monocleprotocol/removetrackrequest_generated.h"
#include "monocleprotocol/removetracksrequest_generated.h"
#include "monocleprotocol/removeuserrequest_generated.h"
#include "monocleprotocol/setlocationrequest_generated.h"
#include "monocleprotocol/setnamerequest_generated.h"
#include "monocleprotocol/subscribediscoveryrequest_generated.h"
#include "monocleprotocol/subscribehardwarestatsresponse_generated.h"
#include "monocleprotocol/subscriberecordingjoblogrequest_generated.h"
#include "monocleprotocol/subscriberecordingjoblogresponse_generated.h"
#include "monocleprotocol/subscriberecordingjobsourcetracklogrequest_generated.h"
#include "monocleprotocol/subscriberecordingjobsourcetracklogresponse_generated.h"
#include "monocleprotocol/subscriberecordinglogrequest_generated.h"
#include "monocleprotocol/subscriberecordinglogresponse_generated.h"
#include "monocleprotocol/subscriberecordingtracklogrequest_generated.h"
#include "monocleprotocol/subscriberecordingtracklogresponse_generated.h"
#include "monocleprotocol/subscriberesponse_generated.h"
#include "monocleprotocol/trackchanged_generated.h"
#include "monocleprotocol/trackdeletedata_generated.h"
#include "monocleprotocol/trackadded_generated.h"
#include "monocleprotocol/trackchanged_generated.h"
#include "monocleprotocol/trackremoved_generated.h"
#include "monocleprotocol/tracksetdata_generated.h"
#include "monocleprotocol/unmountfilerequest_generated.h"
#include "monocleprotocol/unsubscriberecordingjoblogrequest_generated.h"
#include "monocleprotocol/unsubscriberecordingjoblogresponse_generated.h"
#include "monocleprotocol/unsubscriberecordingjobsourcetracklogrequest_generated.h"
#include "monocleprotocol/unsubscriberecordingjobsourcetracklogresponse_generated.h"
#include "monocleprotocol/unsubscriberecordinglogrequest_generated.h"
#include "monocleprotocol/unsubscriberecordinglogresponse_generated.h"
#include "monocleprotocol/unsubscriberecordingtracklogrequest_generated.h"
#include "monocleprotocol/unsubscriberecordingtracklogresponse_generated.h"
#include "monocleprotocol/useradded_generated.h"
#include "monocleprotocol/userchanged_generated.h"
#include "monocleprotocol/userremoved_generated.h"

///// Namespaces /////

namespace monocle
{

namespace client
{

///// Globals /////

const int32_t DEFAULT_TIMEOUT = 20;

///// Methods /////

int32_t Client::GetDefaultTimeout()
{
  return DEFAULT_TIMEOUT;
}

Client::Client(boost::asio::io_service& io) :
  io_(io),
  strand_(io_),
  socket_(boost::make_shared<sock::TcpSocket>(io_)),
  sequence_(0),
  addfile_(DEFAULT_TIMEOUT, this),
  addgroup_(DEFAULT_TIMEOUT, this),
  addlayout_(DEFAULT_TIMEOUT, this),
  addmap_(DEFAULT_TIMEOUT, this),
  addonvifuser_(DEFAULT_TIMEOUT, this),
  addreceiver_(DEFAULT_TIMEOUT, this),
  addrecording_(DEFAULT_TIMEOUT, this),
  addrecordingjob_(DEFAULT_TIMEOUT, this),
  addtrack_(DEFAULT_TIMEOUT, this),
  addtrack2_(DEFAULT_TIMEOUT, this),
  adduser_(DEFAULT_TIMEOUT, this),
  authenticate_(DEFAULT_TIMEOUT, this),
  changegroup_(DEFAULT_TIMEOUT, this),
  changemap_(DEFAULT_TIMEOUT, this),
  changeonvifuser_(DEFAULT_TIMEOUT, this),
  changereceiver_(DEFAULT_TIMEOUT, this),
  changerecording_(DEFAULT_TIMEOUT, this),
  changerecordingjob_(DEFAULT_TIMEOUT, this),
  changetrack_(DEFAULT_TIMEOUT, this),
  changetrack2_(DEFAULT_TIMEOUT, this),
  changeuser_(DEFAULT_TIMEOUT, this),
  controlstream_(DEFAULT_TIMEOUT, this),
  controlstreamframestep_(DEFAULT_TIMEOUT, this),
  controlstreamlive_(DEFAULT_TIMEOUT, this),
  controlstreampause_(DEFAULT_TIMEOUT, this),
  createfindmotion_(DEFAULT_TIMEOUT, this),
  createfindobject_(DEFAULT_TIMEOUT, this),
  createstream_(DEFAULT_TIMEOUT, this),
  destroyfindmotion_(DEFAULT_TIMEOUT, this),
  destroyfindobject_(DEFAULT_TIMEOUT, this),
  destroystream_(DEFAULT_TIMEOUT, this),
  discoverybroadcast_(DEFAULT_TIMEOUT, this),
  getauthenticationnonce_(DEFAULT_TIMEOUT, this),
  getchildfolders_(DEFAULT_TIMEOUT, this),
  getfiles_(DEFAULT_TIMEOUT, this),
  getreceivers_(DEFAULT_TIMEOUT, this),
  getrecordings_(DEFAULT_TIMEOUT, this),
  getsnapshot_(DEFAULT_TIMEOUT, this),
  getstate_(DEFAULT_TIMEOUT, this),
  gettime_(DEFAULT_TIMEOUT, this),
  keepalive_(DEFAULT_TIMEOUT, this),
  mountfile_(DEFAULT_TIMEOUT, this),
  removefile_(DEFAULT_TIMEOUT, this),
  removegroup_(DEFAULT_TIMEOUT, this),
  removemap_(DEFAULT_TIMEOUT, this),
  removeonvifuser_(DEFAULT_TIMEOUT, this),
  removereceiver_(DEFAULT_TIMEOUT, this),
  removerecording_(DEFAULT_TIMEOUT, this),
  removerecordingjob_(DEFAULT_TIMEOUT, this),
  removerecordingjobsource_(DEFAULT_TIMEOUT, this),
  removetrack_(DEFAULT_TIMEOUT, this),
  removetracks_(DEFAULT_TIMEOUT, this),
  removeuser_(DEFAULT_TIMEOUT, this),
  setlocation_(DEFAULT_TIMEOUT, this),
  setname_(DEFAULT_TIMEOUT, this),
  subscribe_(DEFAULT_TIMEOUT, this),
  subscribediscovery_(DEFAULT_TIMEOUT, this),
  subscribehardwarestats_(DEFAULT_TIMEOUT, this),
  subscriberecordingjoblog_(DEFAULT_TIMEOUT, this),
  subscriberecordingjobsourcetracklog_(DEFAULT_TIMEOUT, this),
  subscriberecordinglog_(DEFAULT_TIMEOUT, this),
  subscriberecordingtracklog_(DEFAULT_TIMEOUT, this),
  unmountfile_(DEFAULT_TIMEOUT, this),
  unsubscribediscovery_(DEFAULT_TIMEOUT, this),
  unsubscribehardwarestats_(DEFAULT_TIMEOUT, this),
  unsubscriberecordingjoblog_(DEFAULT_TIMEOUT, this),
  unsubscriberecordingjobsourcetracklog_(DEFAULT_TIMEOUT, this),
  unsubscriberecordinglog_(DEFAULT_TIMEOUT, this),
  unsubscriberecordingtracklog_(DEFAULT_TIMEOUT, this)
{
  inflate_.zalloc = Z_NULL;
  inflate_.zfree = Z_NULL;
  inflate_.opaque = Z_NULL;
  inflate_.avail_in = 0;
  inflate_.next_in = Z_NULL;
  if (inflateInit(&inflate_) != Z_OK)
  {
    // We trust that this never fails

  }
}

Client::~Client()
{
  Destroy();

  if (inflateEnd(&inflate_) != Z_OK)
  {
    // We trust that this never fails

  }
}

sock::Connection Client::Init(const sock::ProxyParams& proxyparams, const std::string& hostname, const uint16_t port, const bool httpupgrade, const boost::function<void(const boost::system::error_code&)> connectcallback)
{
  Destroy();

  socket_ = boost::make_shared<sock::TcpSocket>(io_);
  return socket_->Connect(proxyparams, hostname, port, [this, weakptr = weak_from_this(), hostname, port, httpupgrade, connectcallback](const boost::system::error_code& err, const boost::shared_ptr<sock::ConnectionBlock>& connectionblock)
  {
    auto ptr = weakptr.lock(); // Keep ourselves alive just in case
    if (!ptr)
    {
      connectcallback(boost::system::errc::make_error_code(boost::system::errc::connection_aborted));
      return;
    }

    if (err)
    {
      connectcallback(err);
      return;
    }

    if (httpupgrade)
    {
      const std::string httpupgrade = "GET /monocle HTTP/1.1\r\nHost: " + hostname + ":" + std::to_string(port) + "\r\nUpgrade: monocle\r\n\r\n";
      boost::system::error_code err;
      {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        boost::asio::write(socket_->GetSocket(), boost::asio::buffer(httpupgrade), boost::asio::transfer_all(), err);
        if (err)
        {
          connectcallback(err);
          return;
        }
      }

      connectionblock->connected_ = true; // Extend the life of this connection block because we are going to make another async_call, we are already inside its mutex, so we don't need to relock it here
      boost::asio::async_read_until(socket_->GetSocket(), buffer_, std::string("\r\n\r\n"), strand_.wrap(boost::bind(&Client::HandleHTTPUpgradeResponse, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, connectcallback, connectionblock)));
    }
    else
    {
      HandleConnect(err);
      connectcallback(err);
    }
  });
}

void Client::Destroy()
{
  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    addfile_.Destroy();
    addgroup_.Destroy();
    addlayout_.Destroy();
    addmap_.Destroy();
    addonvifuser_.Destroy();
    addonvifuser_.Destroy();
    addreceiver_.Destroy();
    addrecording_.Destroy();
    addrecordingjob_.Destroy();
    adduser_.Destroy();
    authenticate_.Destroy();
    changegroup_.Destroy();
    changemap_.Destroy();
    changeonvifuser_.Destroy();
    changereceiver_.Destroy();
    changerecording_.Destroy();
    changeuser_.Destroy();
    controlstream_.Destroy();
    controlstreamframestep_.Destroy();
    controlstreamlive_.Destroy();
    controlstreampause_.Destroy();
    createfindmotion_.Destroy();
    createfindobject_.Destroy();
    createstream_.Destroy();
    destroyfindmotion_.Destroy();
    destroyfindobject_.Destroy();
    destroystream_.Destroy();
    discoverybroadcast_.Destroy();
    getauthenticationnonce_.Destroy();
    getfiles_.Destroy();
    getreceivers_.Destroy();
    getrecordings_.Destroy();
    getsnapshot_.Destroy();
    getstate_.Destroy();
    gettime_.Destroy();
    keepalive_.Destroy();
    mountfile_.Destroy();
    removefile_.Destroy();
    removegroup_.Destroy();
    removemap_.Destroy();
    removeonvifuser_.Destroy();
    removereceiver_.Destroy();
    removerecording_.Destroy();
    removerecordingjob_.Destroy();
    removerecordingjobsource_.Destroy();
    removeuser_.Destroy();
    setlocation_.Destroy();
    setname_.Destroy();
    subscribe_.Destroy();
    subscribediscovery_.Destroy();
    subscribehardwarestats_.Destroy();
    subscriberecordingjoblog_.Destroy();
    subscriberecordinglog_.Destroy();
    subscriberecordingtracklog_.Destroy();
    unmountfile_.Destroy();
    unsubscribediscovery_.Destroy();
    unsubscribehardwarestats_.Destroy();
    unsubscriberecordingjoblog_.Destroy();
    unsubscriberecordinglog_.Destroy();
    unsubscriberecordingtracklog_.Destroy();
    boost::system::error_code err;
    socket_->GetSocket().close(err);
  }
}

boost::unique_future<ADDFILERESPONSE> Client::AddFile(const std::string& mountpoint, const std::string& path, const bool filldisk, const uint64_t numchunks, const uint64_t chunksize, const bool automount)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddFileSend(mountpoint, path, filldisk, numchunks, chunksize, automount))
  {

    return boost::make_ready_future(ADDFILERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return addfile_.CreateFuture(sequence_);
}

boost::unique_future<ADDGROUPRESPONSE> Client::AddGroup(const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddGroupSend(name, manageusers, managerecordings, managemaps, managedevice, allrecordings, recordings))
  {

    return boost::make_ready_future(ADDGROUPRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return addgroup_.CreateFuture(sequence_);
}

boost::unique_future<ADDLAYOUTRESPONSE> Client::AddLayout(const LAYOUT& layout)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddLayoutSend(layout))
  {

    return boost::make_ready_future(ADDLAYOUTRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return addlayout_.CreateFuture(sequence_);
}

boost::unique_future<ADDMAPRESPONSE> Client::AddMap(const std::string& name, const std::string& location, const std::vector<int8_t>& image)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddMapSend(name, location, image))
  {

    return boost::make_ready_future(ADDMAPRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return addmap_.CreateFuture(sequence_);
}

boost::unique_future<ADDONVIFUSERRESPONSE> Client::AddONVIFUser(const std::string& username, const std::string& password, const ONVIFUserlevel onvifuserlevel)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddONVIFUserSend(username, password, onvifuserlevel))
  {

    return boost::make_ready_future(ADDONVIFUSERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return addonvifuser_.CreateFuture(sequence_);
}

boost::unique_future<ADDRECEIVERRESPONSE> Client::AddReceiver(const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddReceiverSend(mode, uri, username, password, parameters))
  {

    return boost::make_ready_future(ADDRECEIVERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return addreceiver_.CreateFuture(sequence_);
}

boost::unique_future<ADDRECORDINGRESPONSE> Client::AddRecording(const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const bool createdefaulttracks, const bool createdefaultjob)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddRecordingSend(sourceid, name, location, description, address, content, retentiontime, createdefaulttracks, createdefaultjob))
  {

    return boost::make_ready_future(ADDRECORDINGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return addrecording_.CreateFuture(sequence_);
}

boost::unique_future<ADDRECORDINGJOBRESPONSE> Client::AddRecordingJob(const uint64_t recordingtoken, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<ADDRECORDINGJOBSOURCE>& sources)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddRecordingJobSend(recordingtoken, name, enabled, priority, sources))
  {

    return boost::make_ready_future(ADDRECORDINGJOBRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return addrecordingjob_.CreateFuture(sequence_);
}

boost::unique_future<ADDTRACKRESPONSE> Client::AddTrack(const uint64_t recordingtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddTrackSend(recordingtoken, tracktype, description, fixedfiles, digitalsigning, encrypt, flushfrequency, files))
  {

    return boost::make_ready_future(ADDTRACKRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return addtrack_.CreateFuture(sequence_);
}

boost::unique_future<ADDTRACK2RESPONSE> Client::AddTrack2(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& receiverparameters, const std::vector<std::string>& sourceparameters, const std::vector<std::string>& objectdetectorsourceparameters)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddTrack2Send(recordingtoken, recordingjobtoken, tracktype, description, fixedfiles, digitalsigning, encrypt, flushfrequency, files, mediauri, username, password, receiverparameters, sourceparameters, objectdetectorsourceparameters))
  {

    return boost::make_ready_future(ADDTRACK2RESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return addtrack2_.CreateFuture(sequence_);
}

boost::unique_future<ADDUSERRESPONSE> Client::AddUser(const std::string& username, const std::string& password, const uint64_t group)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddUserSend(username, password, group))
  {

    return boost::make_ready_future(ADDUSERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return adduser_.CreateFuture(sequence_);
}

boost::unique_future<AUTHENTICATERESPONSE> Client::Authenticate(const std::string& username, const std::string& clientnonce, const std::string& authdigest)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AuthenticateSend(username, clientnonce, authdigest))
  {

    return boost::make_ready_future(AUTHENTICATERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return authenticate_.CreateFuture(sequence_);
}

boost::unique_future<CHANGEGROUPRESPONSE> Client::ChangeGroup(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeGroupSend(token, name, manageusers, managerecordings, managemaps, managedevice, allrecordings, recordings))
  {

    return boost::make_ready_future(CHANGEGROUPRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return changegroup_.CreateFuture(sequence_);
}

boost::unique_future<CHANGEMAPRESPONSE> Client::ChangeMap(const uint64_t token, const std::string& name, const std::string& location, const std::vector<int8_t>& image)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeMapSend(token, name, location, image))
  {

    return boost::make_ready_future(CHANGEMAPRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return changemap_.CreateFuture(sequence_);
}

boost::unique_future<CHANGEONVIFUSERRESPONSE> Client::ChangeONVIFUser(const uint64_t token, const boost::optional<std::string>& username, const boost::optional<std::string>& password, const monocle::ONVIFUserlevel onvifuserlevel)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeONVIFUserSend(token, username, password, onvifuserlevel))
  {

    return boost::make_ready_future(CHANGEONVIFUSERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return changeonvifuser_.CreateFuture(sequence_);
}

boost::unique_future<CHANGERECEIVERRESPONSE> Client::ChangeReceiver(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeReceiverSend(token, mode, uri, username, password, parameters))
  {

    return boost::make_ready_future(CHANGERECEIVERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return changereceiver_.CreateFuture(sequence_);
}

boost::unique_future<CHANGERECORDINGRESPONSE> Client::ChangeRecording(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeRecordingSend(token, sourceid, name, location, description, address, content, retentiontime))
  {

    return boost::make_ready_future(CHANGERECORDINGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return changerecording_.CreateFuture(sequence_);
}

boost::unique_future<CHANGERECORDINGJOBRESPONSE> Client::ChangeRecordingJob(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<CHANGERECORDINGJOBSOURCE>& sources)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeRecordingJobSend(recordingtoken, token, name, enabled, priority, sources))
  {

    return boost::make_ready_future(CHANGERECORDINGJOBRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return changerecordingjob_.CreateFuture(sequence_);
}

boost::unique_future<CHANGETRACKRESPONSE> Client::ChangeTrack(const uint64_t recordingtoken, const uint32_t id, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeTrackSend(recordingtoken, id, tracktype, description, fixedfiles, digitalsigning, encrypt, flushfrequency, files))
  {

    return boost::make_ready_future(CHANGETRACKRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return changetrack_.CreateFuture(sequence_);
}

boost::unique_future<CHANGETRACK2RESPONSE> Client::ChangeTrack2(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken, const uint32_t objectdetectorrecordingtrackid, const uint64_t objectdetectorrecordingjobsourcetoken, const uint64_t objectdetectorrecordingjobsourcetracktoken, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& receiverparameters, const std::vector<std::string>& sourceparameters, const std::vector<std::string>& objectdetectorsourceparameters)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeTrack2Send(recordingtoken, recordingtrackid, recordingjobtoken, recordingjobsourcetoken, recordingjobsourcetracktoken, objectdetectorrecordingtrackid, objectdetectorrecordingtrackid, objectdetectorrecordingjobsourcetoken, description, fixedfiles, digitalsigning, encrypt, flushfrequency, files, mediauri, username, password, receiverparameters, sourceparameters, objectdetectorsourceparameters))
  {

    return boost::make_ready_future(CHANGETRACK2RESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return changetrack2_.CreateFuture(sequence_);
}

boost::unique_future<CHANGEUSERRESPONSE> Client::ChangeUser(const uint64_t token, const boost::optional<std::string>& digest, const uint64_t group)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeUserSend(token, digest, group))
  {

    return boost::make_ready_future(CHANGEUSERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return changeuser_.CreateFuture(sequence_);
}

boost::unique_future<CONTROLSTREAMRESPONSE> Client::ControlStream(const uint64_t streamtoken, const uint64_t playrequest, const bool fetchmarker, const bool ratecontrol, const bool forwards, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes, const bool iframes)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ControlStreamSend(streamtoken, playrequest, fetchmarker, ratecontrol, forwards, starttime, endtime, numframes, iframes))
  {

    return boost::make_ready_future(CONTROLSTREAMRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return controlstream_.CreateFuture(sequence_);
}

boost::unique_future<CONTROLSTREAMRESPONSE> Client::ControlStreamFrameStep(const uint64_t streamtoken, const uint64_t playrequest, const bool forwards, const uint64_t sequencenum)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ControlStreamFrameStepSend(streamtoken, playrequest, forwards, sequencenum))
  {

    return boost::make_ready_future(CONTROLSTREAMRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return controlstreamframestep_.CreateFuture(sequence_);
}

boost::unique_future<CONTROLSTREAMRESPONSE> Client::ControlStreamLive(const uint64_t streamtoken, const uint64_t playrequest)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ControlStreamLiveSend(streamtoken, playrequest))
  {

    return boost::make_ready_future(CONTROLSTREAMRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return controlstreamlive_.CreateFuture(sequence_);
}

boost::unique_future<CONTROLSTREAMRESPONSE> Client::ControlStreamPause(const uint64_t streamtoken, const boost::optional<uint64_t>& time)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ControlStreamPauseSend(streamtoken, time))
  {

    return boost::make_ready_future(CONTROLSTREAMRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return controlstreampause_.CreateFuture(sequence_);
}

boost::unique_future<CREATEFINDMOTIONRESPONSE> Client::CreateFindMotion(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height, const float sensitivity, const bool fast)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (CreateFindMotionSend(recordingtoken, tracktoken, starttime, endtime, x, y, width, height, sensitivity, fast))
  {

    return boost::make_ready_future(CREATEFINDMOTIONRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return createfindmotion_.CreateFuture(sequence_);
}

boost::unique_future<CREATEFINDOBJECTRESPONSE> Client::CreateFindObject(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (CreateFindObjectSend(recordingtoken, tracktoken, starttime, endtime, x, y, width, height))
  {

    return boost::make_ready_future(CREATEFINDOBJECTRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return createfindobject_.CreateFuture(sequence_);
}

boost::unique_future<CREATESTREAMRESPONSE> Client::CreateStream(const uint64_t recordingtoken, const uint32_t tracktoken)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (CreateStreamSend(recordingtoken, tracktoken))
  {

    return boost::make_ready_future(CREATESTREAMRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return createstream_.CreateFuture(sequence_);
}

boost::unique_future<DESTROYFINDMOTIONRESPONSE> Client::DestroyFindMotion(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (DestroyFindMotionSend(token))
  {

    return boost::make_ready_future(DESTROYFINDMOTIONRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return destroyfindmotion_.CreateFuture(sequence_);
}

boost::unique_future<DESTROYFINDOBJECTRESPONSE> Client::DestroyFindObject(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (DestroyFindObjectSend(token))
  {

    return boost::make_ready_future(DESTROYFINDOBJECTRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return destroyfindobject_.CreateFuture(sequence_);
}

boost::unique_future<DESTROYSTREAMRESPONSE> Client::DestroyStream(const uint64_t streamtoken)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (DestroyStreamSend(streamtoken))
  {

    return boost::make_ready_future(DESTROYSTREAMRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return destroystream_.CreateFuture(sequence_);
}

boost::unique_future<DISCOVERYBROADCASTRESPONSE> Client::DiscoveryBroadcast()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (DiscoveryBroadcastSend())
  {

    return boost::make_ready_future(DISCOVERYBROADCASTRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return discoverybroadcast_.CreateFuture(sequence_);
}

boost::unique_future<GETAUTHENTICATIONNONCERESPONSE> Client::GetAuthenticationNonce()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (GetAuthenticationNonceSend())
  {

    return boost::make_ready_future(GETAUTHENTICATIONNONCERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return getauthenticationnonce_.CreateFuture(sequence_);
}

boost::unique_future<GETFILESRESPONSE> Client::GetFiles()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  const HEADER header(0, false, false, Message::GETFILES, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return boost::make_ready_future(GETFILESRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return getfiles_.CreateFuture(sequence_);
}

boost::unique_future<GETRECEIVERSRESPONSE> Client::GetReceivers()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  const HEADER header(0, false, false, Message::GETRECEIVERS, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return boost::make_ready_future(GETRECEIVERSRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return getreceivers_.CreateFuture(sequence_);
}

boost::unique_future<GETRECORDINGSRESPONSE> Client::GetRecordings()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  const HEADER header(0, false, false, Message::GETRECORDINGS, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return boost::make_ready_future(GETRECORDINGSRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return getrecordings_.CreateFuture(sequence_);
}

boost::unique_future<GETSNAPSHOTRESPONSE> Client::GetSnapshot(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t time, const float x, const float y, const float width, const float height)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (GetSnapshotSend(recordingtoken, recordingtrackid, time, x, y, width, height))
  {

    return boost::make_ready_future(GETSNAPSHOTRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return getsnapshot_.CreateFuture(sequence_);
}

boost::unique_future<GETSTATERESPONSE> Client::GetState()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (GetStateSend())
  {

    return boost::make_ready_future(GETSTATERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return getstate_.CreateFuture(sequence_);
}

boost::unique_future<GETTIMERESPONSE> Client::GetTime()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (GetTimeSend())
  {

    return boost::make_ready_future(GETTIMERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return gettime_.CreateFuture(sequence_);
}

boost::unique_future<KEEPALIVERESPONSE> Client::Keepalive()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  const HEADER header(0, false, false, Message::KEEPALIVE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return boost::make_ready_future(KEEPALIVERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return keepalive_.CreateFuture(sequence_);
}

boost::unique_future<MOUNTFILERESPONSE> Client::MountFile(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (MountFileSend(token))
  {

    return boost::make_ready_future(MOUNTFILERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return mountfile_.CreateFuture(sequence_);
}

boost::unique_future<REMOVEFILERESPONSE> Client::RemoveFile(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveFileSend(token))
  {

    return boost::make_ready_future(REMOVEFILERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return removefile_.CreateFuture(sequence_);
}

boost::unique_future<REMOVEGROUPRESPONSE> Client::RemoveGroup(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveGroupSend(token))
  {

    return boost::make_ready_future(REMOVEGROUPRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return removegroup_.CreateFuture(sequence_);
}

boost::unique_future<REMOVEMAPRESPONSE> Client::RemoveMap(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveMapSend(token))
  {

    return boost::make_ready_future(REMOVEMAPRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return removemap_.CreateFuture(sequence_);
}

boost::unique_future<REMOVEONVIFUSERRESPONSE> Client::RemoveONVIFUser(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveONVIFUserSend(token))
  {

    return boost::make_ready_future(REMOVEONVIFUSERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return removeonvifuser_.CreateFuture(sequence_);
}

boost::unique_future<REMOVERECEIVERRESPONSE> Client::RemoveReceiver(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveReceiverSend(token))
  {

    return boost::make_ready_future(REMOVERECEIVERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return removereceiver_.CreateFuture(sequence_);
}

boost::unique_future<REMOVERECORDINGRESPONSE> Client::RemoveRecording(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveRecordingSend(token))
  {

    return boost::make_ready_future(REMOVERECORDINGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return removerecording_.CreateFuture(sequence_);
}

boost::unique_future<REMOVERECORDINGJOBRESPONSE> Client::RemoveRecordingJob(const uint64_t recordingtoken, const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveRecordingJobSend(recordingtoken, token))
  {

    return boost::make_ready_future(REMOVERECORDINGJOBRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return removerecordingjob_.CreateFuture(sequence_);
}

boost::unique_future<REMOVERECORDINGJOBSOURCERESPONSE> Client::RemoveRecordingJobSource(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveRecordingJobSourceSend(recordingtoken, recordingjobtoken, token))
  {

    return boost::make_ready_future(REMOVERECORDINGJOBSOURCERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return removerecordingjobsource_.CreateFuture(sequence_);
}

boost::unique_future<REMOVETRACKRESPONSE> Client::RemoveTrack(const uint64_t recordingtoken, const uint32_t id)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveTrackSend(recordingtoken, id))
  {

    return boost::make_ready_future(REMOVETRACKRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return removetrack_.CreateFuture(sequence_);
}

boost::unique_future<REMOVETRACKSRESPONSE> Client::RemoveTracks(const uint64_t recordingtoken, const std::vector<uint32_t>& ids)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveTracksSend(recordingtoken, ids))
  {

    return boost::make_ready_future(REMOVETRACKSRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return removetracks_.CreateFuture(sequence_);
}

boost::unique_future<REMOVEUSERRESPONSE> Client::RemoveUser(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveUserSend(token))
  {

    return boost::make_ready_future(REMOVEUSERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return removeuser_.CreateFuture(sequence_);
}

boost::unique_future<SETLOCATIONRESPONSE> Client::SetLocation(const std::string& latitude, const std::string& longitude)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SetLocationSend(latitude, longitude))
  {

    return boost::make_ready_future(SETLOCATIONRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return setlocation_.CreateFuture(sequence_);
}

boost::unique_future<SETNAMERESPONSE> Client::SetName(const std::string& name)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SetNameSend(name))
  {

    return boost::make_ready_future(SETNAMERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return setname_.CreateFuture(sequence_);
}

boost::unique_future<SUBSCRIBERESPONSE> Client::Subscribe()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeSend())
  {

    return boost::make_ready_future(SUBSCRIBERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return subscribe_.CreateFuture(sequence_);
}

boost::unique_future<SUBSCRIBEDISCOVERYRESPONSE> Client::SubscribeDiscovery(const bool broadcast)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeDiscoverySend(broadcast))
  {

    return boost::make_ready_future(SUBSCRIBEDISCOVERYRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return subscribediscovery_.CreateFuture(sequence_);
}

boost::unique_future<SUBSCRIBEHARDWARESTATSRESPONSE> Client::SubscribeHardwareStats()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeHardwareStatsSend())
  {

    return boost::make_ready_future(SUBSCRIBEHARDWARESTATSRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return subscribehardwarestats_.CreateFuture(sequence_);
}

boost::unique_future<SUBSCRIBERECORDINGJOBLOGRESPONSE> Client::SubscribeRecordingJobLog(const uint64_t recordingtoken)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeRecordingJobLogSend(recordingtoken))
  {

    return boost::make_ready_future(SUBSCRIBERECORDINGJOBLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return subscriberecordingjoblog_.CreateFuture(sequence_);
}

boost::unique_future<SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE> Client::SubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeRecordingJobSourceTrackLogSend(recordingtoken))
  {

    return boost::make_ready_future(SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return subscriberecordingjobsourcetracklog_.CreateFuture(sequence_);
}

boost::unique_future<SUBSCRIBERECORDINGLOGRESPONSE> Client::SubscribeRecordingLog(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeRecordingLogSend(token))
  {

    return boost::make_ready_future(SUBSCRIBERECORDINGLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return subscriberecordinglog_.CreateFuture(sequence_);
}

boost::unique_future<SUBSCRIBERECORDINGTRACKLOGRESPONSE> Client::SubscribeRecordingTrackLog(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeRecordingTrackLogSend(token))
  {

    return boost::make_ready_future(SUBSCRIBERECORDINGTRACKLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return subscriberecordingtracklog_.CreateFuture(sequence_);
}

boost::unique_future<UNMOUNTFILERESPONSE> Client::UnmountFile(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnmountFileSend(token))
  {

    return boost::make_ready_future(UNMOUNTFILERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return unmountfile_.CreateFuture(sequence_);
}

boost::unique_future<UNSUBSCRIBEDISCOVERYRESPONSE> Client::UnsubscribeDiscovery()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnsubscribeDiscoverySend())
  {

    return boost::make_ready_future(UNSUBSCRIBEDISCOVERYRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return unsubscribediscovery_.CreateFuture(sequence_);
}

boost::unique_future<UNSUBSCRIBEHARDWARESTATSRESPONSE> Client::UnsubscribeHardwareStats()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnsubscribeHardwareStatsSend())
  {

    return boost::make_ready_future(UNSUBSCRIBEHARDWARESTATSRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return unsubscribehardwarestats_.CreateFuture(sequence_);
}

boost::unique_future<UNSUBSCRIBERECORDINGJOBLOGRESPONSE> Client::UnsubscribeRecordingJobLog(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnsubscribeRecordingJobLogSend(token))
  {

    return boost::make_ready_future(UNSUBSCRIBERECORDINGJOBLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return unsubscriberecordingjoblog_.CreateFuture(sequence_);
}

boost::unique_future<UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE> Client::UnsubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnsubscribeRecordingJobSourceTrackLogSend(recordingtoken))
  {

    return boost::make_ready_future(UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return unsubscriberecordingjobsourcetracklog_.CreateFuture(sequence_);
}

boost::unique_future<UNSUBSCRIBERECORDINGLOGRESPONSE> Client::UnsubscribeRecordingLog(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnsubscribeRecordingLogSend(token))
  {

    return boost::make_ready_future(UNSUBSCRIBERECORDINGLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return unsubscriberecordinglog_.CreateFuture(sequence_);
}

boost::unique_future<UNSUBSCRIBERECORDINGTRACKLOGRESPONSE> Client::UnsubscribeRecordingTrackLog(const uint64_t token)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnsubscribeRecordingTrackLogSend(token))
  {

    return boost::make_ready_future(UNSUBSCRIBERECORDINGTRACKLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return unsubscriberecordingtracklog_.CreateFuture(sequence_);
}

Connection Client::AddFile(const std::string& mountpoint, const std::string& path, const bool filldisk, const uint64_t numchunks, const uint64_t chunksize, const bool automount, boost::function<void(const std::chrono::steady_clock::duration, const ADDFILERESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddFileSend(mountpoint, path, filldisk, numchunks, chunksize, automount))
  {
    callback(std::chrono::steady_clock::duration(), ADDFILERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return addfile_.CreateCallback(sequence_, callback);
}

Connection Client::AddGroup(const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings, boost::function<void(const std::chrono::steady_clock::duration, const ADDGROUPRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddGroupSend(name, manageusers, managerecordings, managemaps, managedevice, allrecordings, recordings))
  {
    callback(std::chrono::steady_clock::duration(), ADDGROUPRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return addgroup_.CreateCallback(sequence_, callback);
}

Connection Client::AddLayout(const LAYOUT& layout, boost::function<void(const std::chrono::steady_clock::duration, const ADDLAYOUTRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddLayoutSend(layout))
  {
    callback(std::chrono::steady_clock::duration(), ADDLAYOUTRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return addlayout_.CreateCallback(sequence_, callback);
}

Connection Client::AddMap(const std::string& name, const std::string& location, const std::vector<int8_t>& image, boost::function<void(const std::chrono::steady_clock::duration, const ADDMAPRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddMapSend(name, location, image))
  {
    callback(std::chrono::steady_clock::duration(), ADDMAPRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return addmap_.CreateCallback(sequence_, callback);
}

Connection Client::AddONVIFUser(const std::string& username, const std::string& password, const ONVIFUserlevel onvifuserlevel, boost::function<void(const std::chrono::steady_clock::duration, const ADDONVIFUSERRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddONVIFUserSend(username, password, onvifuserlevel))
  {
    callback(std::chrono::steady_clock::duration(), ADDONVIFUSERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return addonvifuser_.CreateCallback(sequence_, callback);
}

Connection Client::AddReceiver(const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, boost::function<void(const std::chrono::steady_clock::duration, const ADDRECEIVERRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddReceiverSend(mode, uri, username, password, parameters))
  {
    callback(std::chrono::steady_clock::duration(), ADDRECEIVERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return addreceiver_.CreateCallback(sequence_, callback);
}

Connection Client::AddRecording(const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const bool createdefaulttracks, const bool createdefaultjob, boost::function<void(const std::chrono::steady_clock::duration, const ADDRECORDINGRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddRecordingSend(sourceid, name, location, description, address, content, retentiontime, createdefaulttracks, createdefaultjob))
  {
    callback(std::chrono::steady_clock::duration(), ADDRECORDINGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return addrecording_.CreateCallback(sequence_, callback);
}

Connection Client::AddRecordingJob(const uint64_t recordingtoken, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<ADDRECORDINGJOBSOURCE>& sources, boost::function<void(const std::chrono::steady_clock::duration, const ADDRECORDINGJOBRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddRecordingJobSend(recordingtoken, name, enabled, priority, sources))
  {
    callback(std::chrono::steady_clock::duration(), ADDRECORDINGJOBRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return addrecordingjob_.CreateCallback(sequence_, callback);
}

Connection Client::AddTrack(const uint64_t recordingtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, boost::function<void(const std::chrono::steady_clock::duration, const ADDTRACKRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddTrackSend(recordingtoken, tracktype, description, fixedfiles, digitalsigning, encrypt, flushfrequency, files))
  {
    callback(std::chrono::steady_clock::duration(), ADDTRACKRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return addtrack_.CreateCallback(sequence_, callback);
}

Connection Client::AddTrack2(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& receiverparameters, const std::vector<std::string>& sourceparameters, const std::vector<std::string>& objectdetectorsourceparameters, boost::function<void(const std::chrono::steady_clock::duration, const ADDTRACK2RESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddTrack2Send(recordingtoken, recordingjobtoken, tracktype, description, fixedfiles, digitalsigning, encrypt, flushfrequency, files, mediauri, username, password, receiverparameters, sourceparameters, objectdetectorsourceparameters))
  {
    callback(std::chrono::steady_clock::duration(), ADDTRACK2RESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return addtrack2_.CreateCallback(sequence_, callback);
}

Connection Client::AddUser(const std::string& username, const std::string& password, const uint64_t group, boost::function<void(const std::chrono::steady_clock::duration, const ADDUSERRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AddUserSend(username, password, group))
  {
    callback(std::chrono::steady_clock::duration(), ADDUSERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return adduser_.CreateCallback(sequence_, callback);
}

Connection Client::Authenticate(const std::string& username, const std::string& clientnonce, const std::string& authdigest, boost::function<void(const std::chrono::steady_clock::duration latency, const AUTHENTICATERESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (AuthenticateSend(username, clientnonce, authdigest))
  {
    callback(std::chrono::steady_clock::duration(), AUTHENTICATERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return authenticate_.CreateCallback(sequence_, callback);
}

Connection Client::ChangeGroup(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings, boost::function<void(const std::chrono::steady_clock::duration, const CHANGEGROUPRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeGroupSend(token, name, manageusers, managerecordings, managemaps, managedevice, allrecordings, recordings))
  {
    callback(std::chrono::steady_clock::duration(), CHANGEGROUPRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return changegroup_.CreateCallback(sequence_, callback);
}

Connection Client::ChangeMap(const uint64_t token, const std::string& name, const std::string& location, const std::vector<int8_t>& image, boost::function<void(const std::chrono::steady_clock::duration, const CHANGEMAPRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeMapSend(token, name, location, image))
  {
    callback(std::chrono::steady_clock::duration(), CHANGEMAPRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return changemap_.CreateCallback(sequence_, callback);
}

Connection Client::ChangeONVIFUser(const uint64_t token, const boost::optional<std::string>& username, const boost::optional<std::string>& password, const monocle::ONVIFUserlevel onvifuserlevel, boost::function<void(const std::chrono::steady_clock::duration, const CHANGEONVIFUSERRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeONVIFUserSend(token, username, password, onvifuserlevel))
  {
    callback(std::chrono::steady_clock::duration(), CHANGEONVIFUSERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return changeonvifuser_.CreateCallback(sequence_, callback);
}

Connection Client::ChangeReceiver(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, boost::function<void(const std::chrono::steady_clock::duration, const CHANGERECEIVERRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeReceiverSend(token, mode, uri, username, password, parameters))
  {
    callback(std::chrono::steady_clock::duration(), CHANGERECEIVERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return changereceiver_.CreateCallback(sequence_, callback);
}

Connection Client::ChangeRecording(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, boost::function<void(const std::chrono::steady_clock::duration, const CHANGERECORDINGRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeRecordingSend(token, sourceid, name, location, description, address, content, retentiontime))
  {
    callback(std::chrono::steady_clock::duration(), CHANGERECORDINGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return changerecording_.CreateCallback(sequence_, callback);
}

Connection Client::ChangeRecordingJob(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<CHANGERECORDINGJOBSOURCE>& sources, boost::function<void(const std::chrono::steady_clock::duration, const CHANGERECORDINGJOBRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeRecordingJobSend(recordingtoken, token, name, enabled, priority, sources))
  {
    callback(std::chrono::steady_clock::duration(), CHANGERECORDINGJOBRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return changerecordingjob_.CreateCallback(sequence_, callback);
}

Connection Client::ChangeTrack(const uint64_t recordingtoken, const uint32_t id, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, boost::function<void(const std::chrono::steady_clock::duration, const CHANGETRACKRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeTrackSend(recordingtoken, id, tracktype, description, fixedfiles, digitalsigning, encrypt, flushfrequency, files))
  {
    callback(std::chrono::steady_clock::duration(), CHANGETRACKRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return changetrack_.CreateCallback(sequence_, callback);
}

Connection Client::ChangeTrack2(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken, const uint32_t objectdetectorrecordingtrackid, const uint64_t objectdetectorrecordingjobsourcetoken, const uint64_t objectdetectorrecordingjobsourcetracktoken, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& receiverparameters, const std::vector<std::string>& sourceparameters, const std::vector<std::string>& objectdetectorsourceparameters, boost::function<void(const std::chrono::steady_clock::duration, const CHANGETRACK2RESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeTrack2Send(recordingtoken, recordingtrackid, recordingjobtoken, recordingjobsourcetoken, recordingjobsourcetracktoken, objectdetectorrecordingtrackid, objectdetectorrecordingjobsourcetoken, objectdetectorrecordingjobsourcetracktoken, description, fixedfiles, digitalsigning, encrypt, flushfrequency, files, mediauri, username, password, receiverparameters, sourceparameters, objectdetectorsourceparameters))
  {
    callback(std::chrono::steady_clock::duration(), CHANGETRACK2RESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return changetrack2_.CreateCallback(sequence_, callback);
}

Connection Client::ChangeUser(const uint64_t token, const boost::optional<std::string>& digest, const uint64_t group, boost::function<void(const std::chrono::steady_clock::duration, const CHANGEUSERRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ChangeUserSend(token, digest, group))
  {
    callback(std::chrono::steady_clock::duration(), CHANGEUSERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return changeuser_.CreateCallback(sequence_, callback);
}

Connection Client::ControlStream(const uint64_t streamtoken, const uint64_t playrequest, const bool fetchmarker, const bool ratecontrol, const bool forwards, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes, const bool iframes, boost::function<void(const std::chrono::steady_clock::duration latency, const CONTROLSTREAMRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ControlStreamSend(streamtoken, playrequest, fetchmarker, ratecontrol, forwards, starttime, endtime, numframes, iframes))
  {
    callback(std::chrono::steady_clock::duration(), CONTROLSTREAMRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return controlstream_.CreateCallback(sequence_, callback);
}

Connection Client::ControlStreamFrameStep(const uint64_t streamtoken, const uint64_t playrequest, const bool forwards, const uint64_t sequencenum, boost::function<void(const std::chrono::steady_clock::duration, const CONTROLSTREAMRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ControlStreamFrameStepSend(streamtoken, playrequest, forwards, sequencenum))
  {
    callback(std::chrono::steady_clock::duration(), CONTROLSTREAMRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return controlstreamframestep_.CreateCallback(sequence_, callback);
}

Connection Client::ControlStreamLive(const uint64_t streamtoken, const uint64_t playrequest, boost::function<void(const std::chrono::steady_clock::duration, const CONTROLSTREAMRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ControlStreamLiveSend(streamtoken, playrequest))
  {
    callback(std::chrono::steady_clock::duration(), CONTROLSTREAMRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return controlstreamlive_.CreateCallback(sequence_, callback);
}

Connection Client::ControlStreamPause(const uint64_t streamtoken, const boost::optional<uint64_t>& time, boost::function<void(const std::chrono::steady_clock::duration, const CONTROLSTREAMRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (ControlStreamPauseSend(streamtoken, time))
  {
    callback(std::chrono::steady_clock::duration(), CONTROLSTREAMRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return controlstreampause_.CreateCallback(sequence_, callback);
}

Connection Client::CreateFindMotion(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height, const float sensitivity, const bool fast, boost::function<void(const std::chrono::steady_clock::duration, const CREATEFINDMOTIONRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (CreateFindMotionSend(recordingtoken, tracktoken, starttime, endtime, x, y, width, height, sensitivity, fast))
  {
    callback(std::chrono::steady_clock::duration(), CREATEFINDMOTIONRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return createfindmotion_.CreateCallback(sequence_, callback);
}

Connection Client::CreateFindObject(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height, boost::function<void(const std::chrono::steady_clock::duration, const CREATEFINDOBJECTRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (CreateFindObjectSend(recordingtoken, tracktoken, starttime, endtime, x, y, width, height))
  {
    callback(std::chrono::steady_clock::duration(), CREATEFINDOBJECTRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return createfindobject_.CreateCallback(sequence_, callback);
}

Connection Client::CreateStream(const uint64_t recordingtoken, const uint32_t tracktoken, boost::function<void(const std::chrono::steady_clock::duration latency, const CREATESTREAMRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (CreateStreamSend(recordingtoken, tracktoken))
  {
    callback(std::chrono::steady_clock::duration(), CREATESTREAMRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return createstream_.CreateCallback(sequence_, callback);
}

Connection Client::DestroyFindMotion(const uint64_t streamtoken, boost::function<void(const std::chrono::steady_clock::duration latency, const DESTROYFINDMOTIONRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (DestroyFindMotionSend(streamtoken))
  {
    callback(std::chrono::steady_clock::duration(), DESTROYFINDMOTIONRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return destroyfindmotion_.CreateCallback(sequence_, callback);
}

Connection Client::DestroyFindObject(const uint64_t streamtoken, boost::function<void(const std::chrono::steady_clock::duration latency, const DESTROYFINDOBJECTRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (DestroyFindObjectSend(streamtoken))
  {
    callback(std::chrono::steady_clock::duration(), DESTROYFINDOBJECTRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return destroyfindobject_.CreateCallback(sequence_, callback);
}

Connection Client::DestroyStream(const uint64_t streamtoken, boost::function<void(const std::chrono::steady_clock::duration latency, const DESTROYSTREAMRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (DestroyStreamSend(streamtoken))
  {
    callback(std::chrono::steady_clock::duration(), DESTROYSTREAMRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return destroystream_.CreateCallback(sequence_, callback);
}

Connection Client::DiscoveryBroadcast(boost::function<void(const std::chrono::steady_clock::duration, const DISCOVERYBROADCASTRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (DiscoveryBroadcastSend())
  {
    callback(std::chrono::steady_clock::duration(), DISCOVERYBROADCASTRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return discoverybroadcast_.CreateCallback(sequence_, callback);
}

Connection Client::GetAuthenticationNonce(boost::function<void(const std::chrono::steady_clock::duration latency, const GETAUTHENTICATIONNONCERESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (GetAuthenticationNonceSend())
  {
    callback(std::chrono::steady_clock::duration(), GETAUTHENTICATIONNONCERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return getauthenticationnonce_.CreateCallback(sequence_, callback);
}

Connection Client::GetChildFolders(const std::string& path, const bool parentpaths, boost::function<void(const std::chrono::steady_clock::duration, const GETCHILDFOLDERSRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (GetChildFoldersSend(path, parentpaths))
  {
    callback(std::chrono::steady_clock::duration(), GETCHILDFOLDERSRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return getchildfolders_.CreateCallback(sequence_, callback);
}

Connection Client::GetFiles(boost::function<void(const std::chrono::steady_clock::duration, const GETFILESRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (GetFilesSend())
  {
    callback(std::chrono::steady_clock::duration(), GETFILESRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return getfiles_.CreateCallback(sequence_, callback);
}

Connection Client::GetReceivers(boost::function<void(const std::chrono::steady_clock::duration, const GETRECEIVERSRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (GetReceiversSend())
  {
    callback(std::chrono::steady_clock::duration(), GETRECEIVERSRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return getreceivers_.CreateCallback(sequence_, callback);
}

Connection Client::GetRecordings(boost::function<void(const std::chrono::steady_clock::duration, const GETRECORDINGSRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (GetRecordingsSend())
  {
    callback(std::chrono::steady_clock::duration(), GETRECORDINGSRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return getrecordings_.CreateCallback(sequence_, callback);
}

Connection Client::GetSnapshot(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t time, const float x, const float y, const float width, const float height, boost::function<void(const std::chrono::steady_clock::duration latency, const GETSNAPSHOTRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (GetSnapshotSend(recordingtoken, recordingtrackid, time, x, y, width, height))
  {
    callback(std::chrono::steady_clock::duration(), GETSNAPSHOTRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return getsnapshot_.CreateCallback(sequence_, callback);
}

Connection Client::GetState(boost::function<void(const std::chrono::steady_clock::duration latency, const GETSTATERESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (GetStateSend())
  {
    callback(std::chrono::steady_clock::duration(), GETSTATERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return getstate_.CreateCallback(sequence_, callback);
}

Connection Client::GetTime(boost::function<void(const std::chrono::steady_clock::duration latency, const GETTIMERESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (GetTimeSend())
  {
    callback(std::chrono::steady_clock::duration(), GETTIMERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return gettime_.CreateCallback(sequence_, callback);
}

Connection Client::Keepalive(boost::function<void(const std::chrono::steady_clock::duration latency, const KEEPALIVERESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  const HEADER header(0, false, false, Message::KEEPALIVE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    return Connection();
    callback(std::chrono::steady_clock::duration(), KEEPALIVERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
  }
  return keepalive_.CreateCallback(sequence_, callback);
}

Connection Client::MountFile(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const MOUNTFILERESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (MountFileSend(token))
  {
    callback(std::chrono::steady_clock::duration(), MOUNTFILERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return mountfile_.CreateCallback(sequence_, callback);
}

Connection Client::RemoveFile(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const REMOVEFILERESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveFileSend(token))
  {
    callback(std::chrono::steady_clock::duration(), REMOVEFILERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return removefile_.CreateCallback(sequence_, callback);
}

Connection Client::RemoveGroup(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const REMOVEGROUPRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveGroupSend(token))
  {
    callback(std::chrono::steady_clock::duration(), REMOVEGROUPRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return removegroup_.CreateCallback(sequence_, callback);
}

Connection Client::RemoveMap(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const REMOVEMAPRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveMapSend(token))
  {
    callback(std::chrono::steady_clock::duration(), REMOVEMAPRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return removemap_.CreateCallback(sequence_, callback);
}

Connection Client::RemoveONVIFUser(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const REMOVEONVIFUSERRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveONVIFUserSend(token))
  {
    callback(std::chrono::steady_clock::duration(), REMOVEONVIFUSERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return removeonvifuser_.CreateCallback(sequence_, callback);
}

Connection Client::RemoveReceiver(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const REMOVERECEIVERRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveReceiverSend(token))
  {
    callback(std::chrono::steady_clock::duration(), REMOVERECEIVERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return removereceiver_.CreateCallback(sequence_, callback);
}

Connection Client::RemoveRecording(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const REMOVERECORDINGRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveRecordingSend(token))
  {
    callback(std::chrono::steady_clock::duration(), REMOVERECORDINGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return removerecording_.CreateCallback(sequence_, callback);
}

Connection Client::RemoveRecordingJob(const uint64_t recordingtoken, const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const REMOVERECORDINGJOBRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveRecordingJobSend(recordingtoken, token))
  {
    callback(std::chrono::steady_clock::duration(), REMOVERECORDINGJOBRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return removerecordingjob_.CreateCallback(sequence_, callback);
}

Connection Client::RemoveRecordingJobSource(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const REMOVERECORDINGJOBSOURCERESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveRecordingJobSourceSend(recordingtoken, recordingjobtoken, token))
  {
    callback(std::chrono::steady_clock::duration(), REMOVERECORDINGJOBSOURCERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return removerecordingjobsource_.CreateCallback(sequence_, callback);
}

Connection Client::RemoveTrack(const uint64_t recordingtoken, const uint32_t id, boost::function<void(const std::chrono::steady_clock::duration latency, const REMOVETRACKRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveTrackSend(recordingtoken, id))
  {
    callback(std::chrono::steady_clock::duration(), REMOVETRACKRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return removetrack_.CreateCallback(sequence_, callback);
}

Connection Client::RemoveTracks(const uint64_t recordingtoken, const std::vector<uint32_t>& ids, boost::function<void(const std::chrono::steady_clock::duration latency, const REMOVETRACKSRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveTracksSend(recordingtoken, ids))
  {
    callback(std::chrono::steady_clock::duration(), REMOVETRACKSRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return removetracks_.CreateCallback(sequence_, callback);
}

Connection Client::RemoveUser(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const REMOVEUSERRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (RemoveUserSend(token))
  {
    callback(std::chrono::steady_clock::duration(), REMOVEUSERRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return removeuser_.CreateCallback(sequence_, callback);
}

Connection Client::SetLocation(const std::string& latitude, const std::string& longitude, boost::function<void(const std::chrono::steady_clock::duration latency, const SETLOCATIONRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SetLocationSend(latitude, longitude))
  {
    callback(std::chrono::steady_clock::duration(), SETLOCATIONRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return setlocation_.CreateCallback(sequence_, callback);
}

Connection Client::SetName(const std::string& name, boost::function<void(const std::chrono::steady_clock::duration latency, const SETNAMERESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SetNameSend(name))
  {
    callback(std::chrono::steady_clock::duration(), SETNAMERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return setname_.CreateCallback(sequence_, callback);
}

Connection Client::Subscribe(boost::function<void(const std::chrono::steady_clock::duration latency, const SUBSCRIBERESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeSend())
  {
    callback(std::chrono::steady_clock::duration(), SUBSCRIBERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return subscribe_.CreateCallback(sequence_, callback);
}

Connection Client::SubscribeDiscovery(const bool broadcast, boost::function<void(const std::chrono::steady_clock::duration latency, const SUBSCRIBEDISCOVERYRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeDiscoverySend(broadcast))
  {
    callback(std::chrono::steady_clock::duration(), SUBSCRIBEDISCOVERYRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return subscribediscovery_.CreateCallback(sequence_, callback);
}

Connection Client::SubscribeHardwareStats(boost::function<void(const std::chrono::steady_clock::duration latency, const SUBSCRIBEHARDWARESTATSRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeHardwareStatsSend())
  {
    callback(std::chrono::steady_clock::duration(), SUBSCRIBEHARDWARESTATSRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return subscribehardwarestats_.CreateCallback(sequence_, callback);
}

Connection Client::SubscribeRecordingJobLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration latency, const SUBSCRIBERECORDINGJOBLOGRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeRecordingJobLogSend(recordingtoken))
  {
    callback(std::chrono::steady_clock::duration(), SUBSCRIBERECORDINGJOBLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return subscriberecordingjoblog_.CreateCallback(sequence_, callback);
}

Connection Client::SubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration, const SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeRecordingJobSourceTrackLogSend(recordingtoken))
  {
    callback(std::chrono::steady_clock::duration(), SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return subscriberecordingjobsourcetracklog_.CreateCallback(sequence_, callback);
}

Connection Client::SubscribeRecordingLog(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const SUBSCRIBERECORDINGLOGRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeRecordingLogSend(token))
  {
    callback(std::chrono::steady_clock::duration(), SUBSCRIBERECORDINGLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return subscriberecordinglog_.CreateCallback(sequence_, callback);
}

Connection Client::SubscribeRecordingTrackLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration latency, const SUBSCRIBERECORDINGTRACKLOGRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SubscribeRecordingTrackLogSend(recordingtoken))
  {
    callback(std::chrono::steady_clock::duration(), SUBSCRIBERECORDINGTRACKLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return subscriberecordingtracklog_.CreateCallback(sequence_, callback);
}

Connection Client::UnmountFile(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const UNMOUNTFILERESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnmountFileSend(token))
  {
    callback(std::chrono::steady_clock::duration(), UNMOUNTFILERESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return unmountfile_.CreateCallback(sequence_, callback);
}

Connection Client::UnsubscribeDiscovery(boost::function<void(const std::chrono::steady_clock::duration latency, const UNSUBSCRIBEDISCOVERYRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnsubscribeDiscoverySend())
  {
    callback(std::chrono::steady_clock::duration(), UNSUBSCRIBEDISCOVERYRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return unsubscribediscovery_.CreateCallback(sequence_, callback);
}

Connection Client::UnsubscribeHardwareStats(boost::function<void(const std::chrono::steady_clock::duration latency, const UNSUBSCRIBEHARDWARESTATSRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnsubscribeHardwareStatsSend())
  {
    callback(std::chrono::steady_clock::duration(), UNSUBSCRIBEHARDWARESTATSRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return unsubscribehardwarestats_.CreateCallback(sequence_, callback);
}

Connection Client::UnsubscribeRecordingJobLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration latency, const UNSUBSCRIBERECORDINGJOBLOGRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnsubscribeRecordingJobLogSend(recordingtoken))
  {
    callback(std::chrono::steady_clock::duration(), UNSUBSCRIBERECORDINGJOBLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return unsubscriberecordingjoblog_.CreateCallback(sequence_, callback);
}

Connection Client::UnsubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration, const UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnsubscribeRecordingJobSourceTrackLogSend(recordingtoken))
  {
    callback(std::chrono::steady_clock::duration(), UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return unsubscriberecordingjobsourcetracklog_.CreateCallback(sequence_, callback);
}

Connection Client::UnsubscribeRecordingLog(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const UNSUBSCRIBERECORDINGLOGRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnsubscribeRecordingLogSend(token))
  {
    callback(std::chrono::steady_clock::duration(), UNSUBSCRIBERECORDINGLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return unsubscriberecordinglog_.CreateCallback(sequence_, callback);
}

Connection Client::UnsubscribeRecordingTrackLog(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration latency, const UNSUBSCRIBERECORDINGTRACKLOGRESPONSE&)> callback)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (UnsubscribeRecordingTrackLogSend(token))
  {
    callback(std::chrono::steady_clock::duration(), UNSUBSCRIBERECORDINGTRACKLOGRESPONSE(Error(ErrorCode::Disconnected, "Disconnected")));
    return Connection();
  }
  return unsubscriberecordingtracklog_.CreateCallback(sequence_, callback);
}

boost::system::error_code Client::AddFileSend(const std::string& mountpoint, const std::string& path, const bool filldisk, const uint64_t numchunks, const uint64_t chunksize, const bool automount)
{
  fbb_.Clear();
  fbb_.Finish(CreateAddFileRequest(fbb_, fbb_.CreateString(mountpoint), fbb_.CreateString(path), filldisk, numchunks, chunksize, automount));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ADDFILE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::AddGroupSend(const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{
  fbb_.Clear();
  fbb_.Finish(CreateAddGroupRequest(fbb_, fbb_.CreateString(name), manageusers, managerecordings, allrecordings, fbb_.CreateVector(recordings), managemaps, managedevice));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ADDGROUP, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::AddLayoutSend(const LAYOUT& layout)
{
  fbb_.Clear();

  std::vector< flatbuffers::Offset<LayoutWindow> > layoutwindows;
  layoutwindows.reserve(layout.windows_.size());
  for (const LAYOUTWINDOW& window : layout.windows_)
  {
    std::vector< flatbuffers::Offset<LayoutView> > maps;
    maps.reserve(window.maps_.size());
    for (const LAYOUTVIEW& map : window.maps_)
    {
      maps.push_back(CreateLayoutView(fbb_, map.token_, map.x_, map.y_, map.width_, map.height_));

    }

    std::vector< flatbuffers::Offset<LayoutView> > recordings;
    recordings.reserve(window.recordings_.size());
    for (const LAYOUTVIEW& recording : window.recordings_)
    {
      recordings.push_back(CreateLayoutView(fbb_, recording.token_, recording.x_, recording.y_, recording.width_, recording.height_));

    }

    layoutwindows.push_back(CreateLayoutWindow(fbb_, window.token_, window.mainwindow_, window.maximised_, window.screenx_, window.screeny_, window.screenwidth_, window.screenheight_, window.x_, window.y_, window.width_, window.height_, window.gridwidth_, window.gridheight_, fbb_.CreateVector(maps), fbb_.CreateVector(recordings)));
  }

  fbb_.Finish(CreateAddLayoutRequest(fbb_, CreateLayout(fbb_, layout.token_, fbb_.CreateString(layout.name_), fbb_.CreateVector(layoutwindows))));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ADDLAYOUT, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::AddMapSend(const std::string& name, const std::string& location, const std::vector<int8_t>& image)
{
  fbb_.Clear();
  fbb_.Finish(CreateAddMapRequest(fbb_, fbb_.CreateString(name), fbb_.CreateString(location), fbb_.CreateVector(image)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ADDMAP, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::AddONVIFUserSend(const std::string& username, const std::string& password, const ONVIFUserlevel onvifuserlevel)
{
  fbb_.Clear();
  fbb_.Finish(CreateAddONVIFUserRequest(fbb_, fbb_.CreateString(username), fbb_.CreateString(password), onvifuserlevel));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ADDONVIFUSER, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::AddReceiverSend(const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters)
{
  fbb_.Clear();
  fbb_.Finish(CreateAddReceiverRequest(fbb_, mode, fbb_.CreateString(uri), fbb_.CreateString(username), fbb_.CreateString(password), fbb_.CreateVectorOfStrings(parameters)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ADDRECEIVER, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::AddRecordingSend(const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const bool createdefaulttracks, const bool createdefaultjob)
{
  fbb_.Clear();
  fbb_.Finish(CreateAddRecordingRequest(fbb_, fbb_.CreateString(sourceid), fbb_.CreateString(name), fbb_.CreateString(location), fbb_.CreateString(description), fbb_.CreateString(address), fbb_.CreateString(content), retentiontime, createdefaulttracks, createdefaultjob));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ADDRECORDING, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::AddRecordingJobSend(const uint64_t recordingtoken, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<ADDRECORDINGJOBSOURCE>& sources)
{
  fbb_.Clear();
  std::vector< flatbuffers::Offset<monocle::AddRecordingJobSource> > sourcesbuffer;
  sourcesbuffer.reserve(sources.size());
  for (const ADDRECORDINGJOBSOURCE& source : sources)
  {
    std::vector<std::string> sourceparameters;
    sourceparameters.reserve(source.sourceparameters_.size());
    for (const std::string& sourceparameter : source.sourceparameters_)
    {
      sourceparameters.push_back(sourceparameter);

    }

    std::vector<std::string> recordingjobsourcetrackparameters;
    recordingjobsourcetrackparameters.reserve(source.recordingjobsourcetrackparameters_.size());
    for (const std::string& recordingjobsourcetrackparameter : source.recordingjobsourcetrackparameters_)
    {
      recordingjobsourcetrackparameters.push_back(recordingjobsourcetrackparameter);

    }

    sourcesbuffer.push_back(CreateAddRecordingJobSource(fbb_, source.recordingtrackid_, source.receivermode_, fbb_.CreateVectorOfStrings(sourceparameters), fbb_.CreateString(source.mediauri_), fbb_.CreateString(source.username_), fbb_.CreateString(source.password_), fbb_.CreateVectorOfStrings(recordingjobsourcetrackparameters)));
  }

  fbb_.Finish(CreateAddRecordingJobRequest(fbb_, recordingtoken, fbb_.CreateString(name), enabled, priority, fbb_.CreateVector(sourcesbuffer)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ADDRECORDINGJOB, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::AddTrackSend(const uint64_t recordingtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files)
{
  fbb_.Clear();
  fbb_.Finish(CreateAddTrackRequest(fbb_, recordingtoken, tracktype, fbb_.CreateString(description), fixedfiles, digitalsigning, encrypt, flushfrequency, fbb_.CreateVector(files)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ADDTRACK, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::AddTrack2Send(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& receiverparameters, const std::vector<std::string>& sourceparameters, const std::vector<std::string>& objectdetectorsourceparameters)
{
  fbb_.Clear();
  fbb_.Finish(CreateAddTrackRequest2(fbb_, recordingtoken, recordingjobtoken, tracktype, fbb_.CreateString(description), fixedfiles, digitalsigning, encrypt, flushfrequency, fbb_.CreateVector(files), fbb_.CreateString(mediauri), fbb_.CreateString(username), fbb_.CreateString(password), fbb_.CreateVectorOfStrings(receiverparameters), fbb_.CreateVectorOfStrings(sourceparameters), fbb_.CreateVectorOfStrings(objectdetectorsourceparameters)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ADDTRACK2, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::AddUserSend(const std::string& username, const std::string& password, const uint64_t group)
{
  fbb_.Clear();
  fbb_.Finish(CreateAddUserRequest(fbb_, fbb_.CreateString(username), fbb_.CreateString(password), group));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ADDUSER, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::AuthenticateSend(const std::string& username, const std::string& clientnonce, const std::string& authdigest)
{
  fbb_.Clear();
  fbb_.Finish(CreateAuthenticateRequest(fbb_, fbb_.CreateString(username), fbb_.CreateString(clientnonce), fbb_.CreateString(authdigest)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::AUTHENTICATE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ChangeGroupSend(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{
  fbb_.Clear();
  fbb_.Finish(CreateChangeGroupRequest(fbb_, token, fbb_.CreateString(name), manageusers, managerecordings, allrecordings, fbb_.CreateVector(recordings), managemaps, managedevice));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CHANGEGROUP, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ChangeMapSend(const uint64_t token, const std::string& name, const std::string& location, const std::vector<int8_t>& image)
{
  fbb_.Clear();
  fbb_.Finish(CreateChangeMapRequest(fbb_, token, fbb_.CreateString(name), fbb_.CreateString(location), fbb_.CreateVector(image)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CHANGEMAP, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ChangeONVIFUserSend(const uint64_t token, const boost::optional<std::string>& username, const boost::optional<std::string>& password, const monocle::ONVIFUserlevel onvifuserlevel)
{
  fbb_.Clear();
  flatbuffers::Offset<flatbuffers::String> usernamebuffer = 0;
  if (username.is_initialized())
  {
    usernamebuffer = fbb_.CreateString(*username);

  }
  flatbuffers::Offset<flatbuffers::String> passwordbuffer = 0;
  if (password.is_initialized())
  {
    passwordbuffer = fbb_.CreateString(*password);

  }
  fbb_.Finish(CreateChangeONVIFUserRequest(fbb_, token, usernamebuffer, passwordbuffer, onvifuserlevel));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CHANGEONVIFUSER, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ChangeReceiverSend(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters)
{
  fbb_.Clear();
  fbb_.Finish(CreateChangeReceiverRequest(fbb_, token, mode, fbb_.CreateString(uri), fbb_.CreateString(username), fbb_.CreateString(password), fbb_.CreateVectorOfStrings(parameters)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CHANGERECEIVER, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ChangeRecordingSend(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime)
{
  fbb_.Clear();
  fbb_.Finish(CreateChangeRecordingRequest(fbb_, token, fbb_.CreateString(sourceid), fbb_.CreateString(name), fbb_.CreateString(location), fbb_.CreateString(description), fbb_.CreateString(address), fbb_.CreateString(content), retentiontime));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CHANGERECORDING, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ChangeRecordingJobSend(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<CHANGERECORDINGJOBSOURCE>& sources)
{
  fbb_.Clear();
  std::vector< flatbuffers::Offset<monocle::ChangeRecordingJobSource> > sourcesbuffer;
  sourcesbuffer.reserve(sources.size());
  for (const CHANGERECORDINGJOBSOURCE& source : sources)
  {
    std::vector<std::string> sourceparameters;
    sourceparameters.reserve(source.sourceparameters_.size());
    for (const std::string& sourceparameter : source.sourceparameters_)
    {
      sourceparameters.push_back(sourceparameter);

    }

    std::vector<std::string> recordingjobsourcetrackparameters;
    recordingjobsourcetrackparameters.reserve(source.recordingjobsourcetrackparameters_.size());
    for (const std::string& recordingjobsourcetrackparameter : source.recordingjobsourcetrackparameters_)
    {
      recordingjobsourcetrackparameters.push_back(recordingjobsourcetrackparameter);

    }

    sourcesbuffer.push_back(CreateChangeRecordingJobSource(fbb_, source.recordingtrackid_, source.receivermode_, fbb_.CreateVectorOfStrings(sourceparameters), fbb_.CreateString(source.mediauri_), fbb_.CreateString(source.username_), fbb_.CreateString(source.password_), fbb_.CreateVectorOfStrings(recordingjobsourcetrackparameters)));
  }

  fbb_.Finish(CreateChangeRecordingJobRequest(fbb_, recordingtoken, token, fbb_.CreateString(name), enabled, priority, fbb_.CreateVector(sourcesbuffer)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CHANGERECORDINGJOB, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ChangeTrackSend(const uint64_t recordingtoken, const uint32_t id, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files)
{
  fbb_.Clear();
  fbb_.Finish(CreateChangeTrackRequest(fbb_, recordingtoken, id, tracktype, fbb_.CreateString(description), fixedfiles, digitalsigning, encrypt, flushfrequency, fbb_.CreateVector(files)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CHANGETRACK, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ChangeTrack2Send(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken, const uint32_t objectdetectorrecordingtrackid, const uint64_t objectdetectorrecordingjobsourcetoken, const uint64_t objectdetectorrecordingjobsourcetracktoken, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& receiverparameters, const std::vector<std::string>& sourceparameters, const std::vector<std::string>& objectdetectorsourceparameters)
{
  fbb_.Clear();
  fbb_.Finish(CreateChangeTrackRequest2(fbb_, recordingtoken, recordingtrackid, recordingjobtoken, recordingjobsourcetoken, recordingjobsourcetracktoken, objectdetectorrecordingtrackid, objectdetectorrecordingjobsourcetoken, objectdetectorrecordingjobsourcetracktoken, fbb_.CreateString(description), fixedfiles, digitalsigning, encrypt, flushfrequency, fbb_.CreateVector(files), fbb_.CreateString(mediauri), fbb_.CreateString(username), fbb_.CreateString(password), fbb_.CreateVectorOfStrings(receiverparameters), fbb_.CreateVectorOfStrings(sourceparameters), fbb_.CreateVectorOfStrings(objectdetectorsourceparameters)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CHANGETRACK2, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ChangeUserSend(const uint64_t token, const boost::optional<std::string>& digest, const uint64_t group)
{
  fbb_.Clear();
  flatbuffers::Offset<flatbuffers::String> digestbuffer = 0;
  if (digest.is_initialized())
  {
    digestbuffer = fbb_.CreateString(*digest);

  }
  fbb_.Finish(CreateChangeUserRequest(fbb_, token, digestbuffer, group));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CHANGEUSER, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ControlStreamSend(const uint64_t streamtoken, const uint64_t playrequest, const bool fetchmarker, const bool ratecontrol, const bool forwards, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes, const bool iframes)
{
  fbb_.Clear();
  fbb_.Finish(CreateControlStreamPlayRequest(fbb_, streamtoken, playrequest, fetchmarker, ratecontrol, forwards, starttime.is_initialized() ? *starttime : 0, endtime.is_initialized() ? *endtime : 0, numframes.is_initialized() ? *numframes : 0, iframes));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CONTROLSTREAM, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ControlStreamFrameStepSend(const uint64_t streamtoken, const uint64_t playrequest, const bool forwards, const uint64_t sequencenum)
{
  fbb_.Clear();
  fbb_.Finish(CreateControlStreamFrameStepRequest(fbb_, streamtoken, playrequest, forwards, sequencenum));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CONTROLSTREAMFRAMESTEP, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ControlStreamLiveSend(const uint64_t streamtoken, const uint64_t playrequest)
{
  fbb_.Clear();
  fbb_.Finish(CreateControlStreamLiveRequest(fbb_, streamtoken, playrequest));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CONTROLSTREAMLIVE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::ControlStreamPauseSend(const uint64_t streamtoken, const boost::optional<uint64_t>& time)
{
  fbb_.Clear();
  fbb_.Finish(CreateControlStreamPauseRequest(fbb_, streamtoken, time.is_initialized() ? *time : 0));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CONTROLSTREAMPAUSE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::CreateFindMotionSend(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height, const float sensitivity, const bool fast)
{
  fbb_.Clear();
  fbb_.Finish(CreateCreateFindMotionRequest(fbb_, recordingtoken, tracktoken, starttime, endtime, x, y, width, height, sensitivity, fast));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CREATEFINDMOTION, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::CreateFindObjectSend(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height)
{
  fbb_.Clear();
  fbb_.Finish(CreateCreateFindObjectRequest(fbb_, recordingtoken, tracktoken, starttime, endtime, x, y, width, height));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CREATEFINDOBJECT, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::CreateStreamSend(const uint64_t recordingtoken, const uint32_t tracktoken)
{
  fbb_.Clear();
  fbb_.Finish(CreateCreateStreamRequest(fbb_, recordingtoken, tracktoken));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CREATESTREAM, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::DestroyFindMotionSend(const uint64_t streamtoken)
{
  fbb_.Clear();
  fbb_.Finish(CreateDestroyFindMotionRequest(fbb_, streamtoken));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::DESTROYFINDMOTION, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::DestroyFindObjectSend(const uint64_t streamtoken)
{
  fbb_.Clear();
  fbb_.Finish(CreateDestroyFindObjectRequest(fbb_, streamtoken));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::DESTROYFINDOBJECT, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::DestroyStreamSend(const uint64_t streamtoken)
{
  fbb_.Clear();
  fbb_.Finish(CreateDestroyStreamRequest(fbb_, streamtoken));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::DESTROYSTREAM, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::DiscoveryBroadcastSend()
{
  const HEADER header(0, false, false, Message::DISCOVERYBROADCAST, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::GetAuthenticationNonceSend()
{
  const HEADER header(0, false, false, Message::GETAUTHNETICATIONNONCE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::GetChildFoldersSend(const std::string& path, const bool parentpaths)
{
  fbb_.Clear();
  fbb_.Finish(CreateGetChildFoldersRequest(fbb_, fbb_.CreateString(path), parentpaths));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::GETCHILDFOLDERS, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::GetFilesSend()
{
  const HEADER header(0, false, false, Message::GETFILES, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::GetReceiversSend()
{
  const HEADER header(0, false, false, Message::GETRECEIVERS, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::GetRecordingsSend()
{
  const HEADER header(0, false, false, Message::GETRECORDINGS, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::GetSnapshotSend(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t time, const float x, const float y, const float width, const float height)
{
  fbb_.Clear();
  fbb_.Finish(CreateGetSnapshotRequest(fbb_, recordingtoken, recordingtrackid, time, x, y, width, height));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::GETSNAPSHOT, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::GetStateSend()
{
  const HEADER header(0, false, false, Message::GETSTATE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::GetTimeSend()
{
  const HEADER header(0, false, false, Message::GETTIME, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::MountFileSend(const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateMountFileRequest(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::MOUNTFILE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::RemoveFileSend(const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateRemoveFileRequest(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::REMOVEFILE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::RemoveGroupSend(const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateRemoveGroupRequest(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::REMOVEGROUP, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::RemoveMapSend(const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateRemoveMapRequest(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::REMOVEMAP, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::RemoveONVIFUserSend(const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateRemoveONVIFUserRequest(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::REMOVEONVIFUSER, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::RemoveReceiverSend(const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateRemoveReceiverRequest(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::REMOVERECEIVER, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::RemoveRecordingSend(const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateRemoveRecordingRequest(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::REMOVERECORDING, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::RemoveRecordingJobSend(const uint64_t recordingtoken, const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateRemoveRecordingJobRequest(fbb_, recordingtoken, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::REMOVERECORDINGJOB, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::RemoveRecordingJobSourceSend(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateRemoveRecordingJobSourceRequest(fbb_, recordingtoken, recordingjobtoken, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::REMOVERECORDINGJOBSOURCE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::RemoveTrackSend(const uint64_t recordingtoken, const uint32_t id)
{
  fbb_.Clear();
  fbb_.Finish(CreateRemoveTrackRequest(fbb_, recordingtoken, id));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::REMOVETRACK, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::RemoveTracksSend(const uint64_t recordingtoken, const std::vector<uint32_t>& ids)
{
  fbb_.Clear();
  fbb_.Finish(CreateRemoveTracksRequest(fbb_, recordingtoken, fbb_.CreateVector(ids)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::REMOVETRACKS, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::RemoveUserSend(const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateRemoveUserRequest(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::REMOVEUSER, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::SetLocationSend(const std::string& latitude, const std::string& longitude)
{
  fbb_.Clear();
  fbb_.Finish(CreateSetLocationRequest(fbb_, fbb_.CreateString(latitude), fbb_.CreateString(longitude)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::SETLOCATION, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::SetNameSend(const std::string& name)
{
  fbb_.Clear();
  fbb_.Finish(CreateSetNameRequest(fbb_, fbb_.CreateString(name)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::SETNAME, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::SubscribeSend()
{
  const HEADER header(0, false, false, Message::SUBSCRIBE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::SubscribeDiscoverySend(const bool broadcast)
{
  fbb_.Clear();
  fbb_.Finish(CreateSubscribeDiscoveryRequest(fbb_, broadcast));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::SUBSCRIBEDISCOVERY, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::SubscribeHardwareStatsSend()
{
  const HEADER header(0, false, false, Message::SUBSCRIBEHARDWARESTATS, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::SubscribeRecordingJobLogSend(const uint64_t recordingtoken)
{
  fbb_.Clear();
  fbb_.Finish(CreateSubscribeRecordingJobLogRequest(fbb_, recordingtoken));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::SUBSCRIBERECORDINGJOBLOG, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::SubscribeRecordingJobSourceTrackLogSend(const uint64_t recordingtoken)
{
  fbb_.Clear();
  fbb_.Finish(CreateSubscribeRecordingJobLogRequest(fbb_, recordingtoken));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::SUBSCRIBERECORDINGJOBSOURCETRACKLOG, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::SubscribeRecordingLogSend(const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateSubscribeRecordingLogRequest(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::SUBSCRIBERECORDINGLOG, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::SubscribeRecordingTrackLogSend(const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateSubscribeRecordingTrackLogRequest(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::SUBSCRIBERECORDINGTRACKLOG, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::UnmountFileSend(const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateUnmountFileRequest(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::UNMOUNTFILE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::UnsubscribeDiscoverySend()
{
  const HEADER header(0, false, false, Message::UNSUBSCRIBEDISCOVERY, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::UnsubscribeHardwareStatsSend()
{
  const HEADER header(0, false, false, Message::UNSUBSCRIBEHARDWARESTATS, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::UnsubscribeRecordingJobLogSend(const uint64_t recordingtoken)
{
  fbb_.Clear();
  fbb_.Finish(CreateUnsubscribeRecordingJobLogRequest(fbb_, recordingtoken));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::UNSUBSCRIBERECORDINGJOBLOG, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::UnsubscribeRecordingJobSourceTrackLogSend(const uint64_t recordingtoken)
{
  fbb_.Clear();
  fbb_.Finish(CreateUnsubscribeRecordingJobSourceTrackLogRequest(fbb_, recordingtoken));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::UNSUBSCRIBERECORDINGJOBSOURCETRACKLOG, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::UnsubscribeRecordingLogSend(const uint64_t token)
{
  fbb_.Clear();
  fbb_.Finish(CreateUnsubscribeRecordingLogRequest(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::UNSUBSCRIBERECORDINGLOG, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

boost::system::error_code Client::UnsubscribeRecordingTrackLogSend(const uint64_t recordingtoken)
{
  fbb_.Clear();
  fbb_.Finish(CreateUnsubscribeRecordingTrackLogRequest(fbb_, recordingtoken));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::UNSUBSCRIBERECORDINGTRACKLOG, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  boost::asio::write(socket_->GetSocket(), boost::asio::buffer(fbb_.GetBufferPointer(), messagesize), boost::asio::transfer_all(), err);
  if (err)
  {
    Disconnected();
    return err;
  }
  return err;
}

void Client::HandleConnect(const boost::system::error_code& err)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (err)
  {
    buffer_.consume(buffer_.size());
    Disconnected();
    return;
  }
  boost::asio::async_read(socket_->GetSocket(), buffer_, boost::asio::transfer_exactly(sizeof(HEADER)), strand_.wrap(boost::bind(&Client::HandleReadHeader, shared_from_this(), boost::asio::placeholders::error)));
}

void Client::HandleHTTPUpgradeResponse(const boost::system::error_code& err, const size_t size, const boost::function<void(const boost::system::error_code&)> connectcallback, boost::shared_ptr<sock::ConnectionBlock> connectionblock)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);

  if (err)
  {
    std::lock_guard<std::mutex> lock(connectionblock->mutex_);
    if (connectionblock->connected_ == true)
    {
      connectionblock->connected_ = false;
      connectcallback(boost::system::errc::make_error_code(boost::system::errc::bad_message));
    }
    return;
  }

  const std::string response(boost::asio::buffer_cast<const char*>(buffer_.data()), size);
  buffer_.consume(size);
  std::vector<std::string> headers;
  boost::algorithm::split(headers, response, boost::is_any_of("\r\n"), boost::algorithm::token_compress_on);
  if (headers.empty())
  {
    std::lock_guard<std::mutex> lock(connectionblock->mutex_);
    if (connectionblock->connected_ == true)
    {
      connectionblock->connected_ = false;
      connectcallback(boost::system::errc::make_error_code(boost::system::errc::bad_message));
    }
    return;
  }

  if (headers.front() != "HTTP/1.1 200 OK")
  {
    std::lock_guard<std::mutex> lock(connectionblock->mutex_);
    if (connectionblock->connected_ == true)
    {
      connectionblock->connected_ = false;
      connectcallback(boost::system::errc::make_error_code(boost::system::errc::bad_message));
    }
    return;
  }

  {
    std::lock_guard<std::mutex> lock(connectionblock->mutex_);
    if (connectionblock->connected_ == true)
    {
      connectionblock->connected_ = false;
      connectcallback(boost::system::errc::make_error_code(boost::system::errc::success));
    }
  }

  boost::asio::async_read(socket_->GetSocket(), buffer_, boost::asio::transfer_exactly(sizeof(HEADER)), strand_.wrap(boost::bind(&Client::HandleReadHeader, shared_from_this(), boost::asio::placeholders::error)));
}

void Client::HandleReadHeader(const boost::system::error_code& err)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (err)
  {
    buffer_.consume(buffer_.size());
    Disconnected();
    return;
  }

  const HEADER header = *boost::asio::buffer_cast<const HEADER*>(buffer_.data());
  buffer_.consume(sizeof(HEADER));
  if (header.size_ == 0)
  {
    HandleMessage(header.IsError(), header.IsCompressed(), header.message_, header.sequence_, nullptr, 0);
    boost::asio::async_read(socket_->GetSocket(), buffer_, boost::asio::transfer_exactly(sizeof(HEADER)), strand_.wrap(boost::bind(&Client::HandleReadHeader, shared_from_this(), boost::asio::placeholders::error)));
  }
  else if (buffer_.in_avail() >= header.size_)
  {
    HandleMessage(header.IsError(), header.IsCompressed(), header.message_, header.sequence_, boost::asio::buffer_cast<const char*>(buffer_.data()), header.size_);
    buffer_.consume(header.size_);
    boost::asio::async_read(socket_->GetSocket(), buffer_, boost::asio::transfer_exactly(sizeof(HEADER)), strand_.wrap(boost::bind(&Client::HandleReadHeader, shared_from_this(), boost::asio::placeholders::error)));
  }
  else
  {
    boost::asio::async_read(socket_->GetSocket(), buffer_, boost::asio::transfer_exactly(header.size_ - buffer_.in_avail()), strand_.wrap(boost::bind(&Client::HandleReadMessage, shared_from_this(), boost::asio::placeholders::error, header.IsError(), header.IsCompressed(), header.message_, header.sequence_, header.size_)));
    
  }
}

void Client::HandleReadMessage(const boost::system::error_code& err, const bool error, const bool compressed, const Message message, const uint16_t sequence, const size_t size)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (err)
  {
    buffer_.consume(buffer_.size());
    Disconnected();
    return;
  }

  HandleMessage(error, compressed, message, sequence, boost::asio::buffer_cast<const char*>(buffer_.data()), size);
  buffer_.consume(size);
  boost::asio::async_read(socket_->GetSocket(), buffer_, boost::asio::transfer_exactly(sizeof(HEADER)), strand_.wrap(boost::bind(&Client::HandleReadHeader, shared_from_this(), boost::asio::placeholders::error)));
}

void Client::HandleMessage(const bool error, const bool compressed, const Message message, const uint16_t sequence, const char* data, size_t datasize)
{
  if (compressed)
  {
    if (utility::Decompress(inflate_, data, datasize, inflatebuffer_))
    {

      return;
    }
    data = inflatebuffer_.data();
    datasize = inflatebuffer_.size();
  }

  switch (message)
  {
    case Message::ADDFILE:
    {
      if (error)
      {
        HandleError(addfile_, sequence, data, datasize);
        return;
      }

      addfile_.Response(sequence, ADDFILERESPONSE());
      break;
    }
    case Message::ADDGROUP:
    {
      if (error)
      {
        HandleError(addgroup_, sequence, data, datasize);
        return;
      }

      addgroup_.Response(sequence, ADDGROUPRESPONSE());
      break;
    }
    case Message::ADDLAYOUT:
    {
      if (error)
      {
        HandleError(addlayout_, sequence, data, datasize);
        return;
      }

      addlayout_.Response(sequence, ADDLAYOUTRESPONSE());
      break;
    }
    case Message::ADDMAP:
    {
      if (error)
      {
        HandleError(addmap_, sequence, data, datasize);
        return;
      }

      addmap_.Response(sequence, ADDMAPRESPONSE());
      break;
    }
    case Message::ADDONVIFUSER:
    {
      if (error)
      {
        HandleError(addonvifuser_, sequence, data, datasize);
        return;
      }

      addonvifuser_.Response(sequence, ADDONVIFUSERRESPONSE());
      break;
    }
    case Message::ADDRECEIVER:
    {
      if (error)
      {
        HandleError(addreceiver_, sequence, data, datasize);
        return;
      }

      addreceiver_.Response(sequence, ADDRECEIVERRESPONSE());
      break;
    }
    case Message::ADDRECORDING:
    {
      if (error)
      {
        HandleError(addrecording_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddRecordingResponse>(nullptr))
      {
        addrecording_.Response(sequence, ADDRECORDINGRESPONSE(Error(ErrorCode::InvalidMessage, "AddRecordingResponse verification failed")));
        return;
      }

      const AddRecordingResponse* addrecordingresponse = flatbuffers::GetRoot<AddRecordingResponse>(data);
      if (!addrecordingresponse)
      {
        addrecording_.Response(sequence, ADDRECORDINGRESPONSE(Error(ErrorCode::MissingParameter, "AddRecordingResponse missing parameter")));
        return;
      }

      addrecording_.Response(sequence, ADDRECORDINGRESPONSE(addrecordingresponse->token()));
      break;
    }
    case Message::ADDRECORDINGJOB:
    {
      if (error)
      {
        HandleError(addrecordingjob_, sequence, data, datasize);
        return;
      }

      if (data && datasize)
      {
        if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddRecordingJobResponse>(nullptr))
        {
          addrecordingjob_.Response(sequence, ADDRECORDINGJOBRESPONSE(Error(ErrorCode::InvalidMessage, "AddRecordingJobResponse verification failed")));
          return;
        }

        const AddRecordingJobResponse* addrecordingjobresponse = flatbuffers::GetRoot<AddRecordingJobResponse>(data);
        if (!addrecordingjobresponse)
        {
          addrecordingjob_.Response(sequence, ADDRECORDINGJOBRESPONSE(Error(ErrorCode::MissingParameter, "AddRecordingJobResponse missing parameter")));
          return;
        }
        addrecordingjob_.Response(sequence, ADDRECORDINGJOBRESPONSE(addrecordingjobresponse->recordingjobtoken()));
      }
      else
      {
        addrecordingjob_.Response(sequence, ADDRECORDINGJOBRESPONSE(0));

      }

      break;
    }
    case Message::ADDTRACK:
    {
      if (error)
      {
        HandleError(addtrack_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddTrackResponse>(nullptr))
      {
        addtrack_.Response(sequence, ADDTRACKRESPONSE(Error(ErrorCode::InvalidMessage, "AddTrackResponse verification failed")));
        return;
      }

      const AddTrackResponse* addtrackresponse = flatbuffers::GetRoot<AddTrackResponse>(data);
      if (!addtrackresponse)
      {
        addtrack_.Response(sequence, ADDTRACKRESPONSE(Error(ErrorCode::MissingParameter, "AddTrackResponse missing parameter")));
        return;
      }

      addtrack_.Response(sequence, ADDTRACKRESPONSE(addtrackresponse->token()));
      break;
    }
    case Message::ADDTRACK2:
    {
      if (error)
      {
        HandleError(addtrack2_, sequence, data, datasize);
        return;
      }

      addtrack2_.Response(sequence, ADDTRACK2RESPONSE());
      break;
    }
    case Message::ADDUSER:
    {
      if (error)
      {
        HandleError(adduser_, sequence, data, datasize);
        return;
      }

      adduser_.Response(sequence, ADDUSERRESPONSE());
      break;
    }
    case Message::AUTHENTICATE:
    {
      if (error)
      {
        HandleError(authenticate_, sequence, data, datasize);
        return;
      }

      authenticate_.Response(sequence, AUTHENTICATERESPONSE());
      break;
    }
    case Message::CHANGEGROUP:
    {
      if (error)
      {
        HandleError(changegroup_, sequence, data, datasize);
        return;
      }

      changegroup_.Response(sequence, CHANGEGROUPRESPONSE());
      break;
    }
    case Message::CHANGEMAP:
    {
      if (error)
      {
        HandleError(changemap_, sequence, data, datasize);
        return;
      }

      changemap_.Response(sequence, CHANGEMAPRESPONSE());
      break;
    }
    case Message::CHANGEONVIFUSER:
    {
      if (error)
      {
        HandleError(changeonvifuser_, sequence, data, datasize);
        return;
      }

      changeonvifuser_.Response(sequence, CHANGEONVIFUSERRESPONSE());
      break;
    }
    case Message::CHANGERECEIVER:
    {
      if (error)
      {
        HandleError(changereceiver_, sequence, data, datasize);
        return;
      }

      changereceiver_.Response(sequence, CHANGERECEIVERRESPONSE());
      break;
    }
    case Message::CHANGERECORDING:
    {
      if (error)
      {
        HandleError(changerecording_, sequence, data, datasize);
        return;
      }

      changerecording_.Response(sequence, CHANGERECORDINGRESPONSE());
      break;
    }
    case Message::CHANGERECORDINGJOB:
    {
      if (error)
      {
        HandleError(changerecordingjob_, sequence, data, datasize);
        return;
      }

      changerecordingjob_.Response(sequence, CHANGERECORDINGJOBRESPONSE());
      break;
    }
    case Message::CHANGETRACK:
    {
      if (error)
      {
        HandleError(changetrack_, sequence, data, datasize);
        return;
      }

      changetrack_.Response(sequence, CHANGETRACKRESPONSE());
      break;
    }
    case Message::CHANGETRACK2:
    {
      if (error)
      {
        HandleError(changetrack2_, sequence, data, datasize);
        return;
      }

      changetrack2_.Response(sequence, CHANGETRACK2RESPONSE());
      break;
    }
    case Message::CHANGEUSER:
    {
      if (error)
      {
        HandleError(changeuser_, sequence, data, datasize);
        return;
      }

      changeuser_.Response(sequence, CHANGEUSERRESPONSE());
      break;
    }
    case Message::CONTROLSTREAM:
    {
      if (error)
      {
        HandleError(controlstream_, sequence, data, datasize);
        return;
      }
      controlstream_.Response(sequence, CONTROLSTREAMRESPONSE());
      break;
    }
    case Message::CONTROLSTREAMEND:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::ControlStreamEnd>(nullptr))
      {

        return;
      }

      const monocle::ControlStreamEnd* controlstreamend = flatbuffers::GetRoot<monocle::ControlStreamEnd>(data);
      if (!controlstreamend)
      {

        return;
      }

      ControlStreamEnd(controlstreamend->token(), controlstreamend->playrequestindex(), controlstreamend->error());
      break;
    }
    case Message::CONTROLSTREAMFRAMESTEP:
    {
      if (error)
      {
        HandleError(controlstreamframestep_, sequence, data, datasize);
        return;
      }
      controlstreamframestep_.Response(sequence, CONTROLSTREAMRESPONSE());
      break;
    }
    case Message::CONTROLSTREAMLIVE:
    {
      if (error)
      {
        HandleError(controlstreamlive_, sequence, data, datasize);
        return;
      }
      controlstreamlive_.Response(sequence, CONTROLSTREAMRESPONSE());
      break;
    }
    case Message::CONTROLSTREAMPAUSE:
    {
      if (error)
      {
        HandleError(controlstreampause_, sequence, data, datasize);
        return;
      }
      controlstreampause_.Response(sequence, CONTROLSTREAMRESPONSE());
      break;
    }
    case Message::CREATEFINDMOTION:
    {
      if (error)
      {
        HandleError(createfindmotion_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<CreateFindMotionResponse>(nullptr))
      {
        createfindmotion_.Response(sequence, CREATEFINDMOTIONRESPONSE(Error(ErrorCode::InvalidMessage, "CreateFindMotionResponse verification failed")));
        return;
      }

      const CreateFindMotionResponse* createfindmotionresponse = flatbuffers::GetRoot<CreateFindMotionResponse>(data);
      if (!createfindmotionresponse)
      {
        createfindmotion_.Response(sequence, CREATEFINDMOTIONRESPONSE(Error(ErrorCode::MissingParameter, "CreateFindMotionResponse missing parameter")));
        return;
      }

      createfindmotion_.Response(sequence, CREATEFINDMOTIONRESPONSE(createfindmotionresponse->token()));
      break;
    }
    case Message::CREATEFINDOBJECT:
    {
      if (error)
      {
        HandleError(createfindobject_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<CreateFindObjectResponse>(nullptr))
      {
        createfindobject_.Response(sequence, CREATEFINDOBJECTRESPONSE(Error(ErrorCode::InvalidMessage, "CreateFindObjectResponse verification failed")));
        return;
      }

      const CreateFindObjectResponse* createfindobjectresponse = flatbuffers::GetRoot<CreateFindObjectResponse>(data);
      if (!createfindobjectresponse)
      {
        createfindobject_.Response(sequence, CREATEFINDOBJECTRESPONSE(Error(ErrorCode::MissingParameter, "CreateFindObjectResponse missing parameter")));
        return;
      }

      createfindobject_.Response(sequence, CREATEFINDOBJECTRESPONSE(createfindobjectresponse->token()));
      break;
    }
    case Message::CREATESTREAM:
    {
      if (error)
      {
        HandleError(createstream_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<CreateStreamResponse>(nullptr))
      {
        createstream_.Response(sequence, CREATESTREAMRESPONSE(Error(ErrorCode::InvalidMessage, "CreateStreamResponse verification failed")));
        return;
      }

      const CreateStreamResponse* createstreamresponse = flatbuffers::GetRoot<CreateStreamResponse>(data);
      if (!createstreamresponse)
      {
        createstream_.Response(sequence, CREATESTREAMRESPONSE(Error(ErrorCode::MissingParameter, "CreateStreamResponse missing parameter")));
        return;
      }

      std::vector<CODECINDEX> codecindices;
      codecindices.reserve(createstreamresponse->codecindices()->size());
      for (const CodecIndex* codecindex : *createstreamresponse->codecindices())
      {
        codecindices.push_back(CODECINDEX(codecindex->id(), codecindex->codec(), codecindex->parameters()->str(), codecindex->timestamp()));

      }

      createstream_.Response(sequence, CREATESTREAMRESPONSE(createstreamresponse->token(), codecindices));
      break;
    }
    case Message::DESTROYFINDMOTION:
    {
      if (error)
      {
        HandleError(destroyfindmotion_, sequence, data, datasize);
        return;
      }
      destroyfindmotion_.Response(sequence, DESTROYFINDMOTIONRESPONSE());
      break;
    }
    case Message::DESTROYFINDOBJECT:
    {
      if (error)
      {
        HandleError(destroyfindobject_, sequence, data, datasize);
        return;
      }
      destroyfindobject_.Response(sequence, DESTROYFINDOBJECTRESPONSE());
      break;
    }
    case Message::DESTROYSTREAM:
    {
      if (error)
      {
        HandleError(destroystream_, sequence, data, datasize);
        return;
      }
      destroystream_.Response(sequence, DESTROYSTREAMRESPONSE());
      break;
    }
    case Message::DISCOVERYBROADCAST:
    {
      if (error)
      {
        HandleError(discoverybroadcast_, sequence, data, datasize);
        return;
      }

      discoverybroadcast_.Response(sequence, DISCOVERYBROADCASTRESPONSE());
      break;
    }
    case Message::DISCOVERYHELLO:
    {
      if (error)
      {
        // Ignore
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::DiscoveryHello>(nullptr))
      {

        return;
      }

      const monocle::DiscoveryHello* discoveryhello = flatbuffers::GetRoot<monocle::DiscoveryHello>(data);
      if (!discoveryhello)
      {

        return;
      }

      if (discoveryhello->addresses() == nullptr)
      {
        // Ignore
        return;
      }

      std::vector<std::string> addresses;
      addresses.reserve(discoveryhello->addresses()->size());
      for (const flatbuffers::String* address : *discoveryhello->addresses())
      {
        addresses.push_back(address->str());

      }

      std::vector<std::string> scopes;
      scopes.reserve(discoveryhello->scopes()->size());
      for (const flatbuffers::String* scope : *discoveryhello->scopes())
      {
        scopes.push_back(scope->str());

      }

      DiscoveryHello(addresses, scopes);
      break;
    }
    case Message::GETAUTHNETICATIONNONCE:
    {
      if (error)
      {
        HandleError(authenticate_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<GetAuthenticationNonceResponse>(nullptr))
      {
        getauthenticationnonce_.Response(sequence, GETAUTHENTICATIONNONCERESPONSE(Error(ErrorCode::InvalidMessage, "GetAuthenticationNonceResponse verification failed")));
        return;
      }

      const GetAuthenticationNonceResponse* getauthenticationnonceresponse = flatbuffers::GetRoot<GetAuthenticationNonceResponse>(data);
      if (!getauthenticationnonceresponse)
      {
        getauthenticationnonce_.Response(sequence, GETAUTHENTICATIONNONCERESPONSE(Error(ErrorCode::MissingParameter, "GetAuthenticationNonceResponse missing nonce")));
        return;
      }

      if (getauthenticationnonceresponse->nonce() == nullptr)
      {
        getauthenticationnonce_.Response(sequence, GETAUTHENTICATIONNONCERESPONSE(Error(ErrorCode::MissingParameter, "GetAuthenticationNonceResponse missing nonce")));
        return;
      }

      getauthenticationnonce_.Response(sequence, GETAUTHENTICATIONNONCERESPONSE(getauthenticationnonceresponse->nonce()->str()));
      break;
    }
    case Message::GETFILES:
    {
      if (error)
      {
        HandleError(getfiles_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<GetFilesResponse>(nullptr))
      {
        getfiles_.Response(sequence, GETFILESRESPONSE(Error(ErrorCode::InvalidMessage, "GetFilesResponse verification failed")));
        return;
      }

      const GetFilesResponse* getfilesresponse = flatbuffers::GetRoot<GetFilesResponse>(data);
      if (!getfilesresponse || (getfilesresponse->files() == nullptr))
      {
        getfiles_.Response(sequence, GETFILESRESPONSE(Error(ErrorCode::MissingParameter, "GetFilesResponse missing files")));
        return;
      }
      
      const std::pair< Error, std::vector<FILE> > files = GetFilesBuffer(*getfilesresponse->files());
      if (files.first.code_ != ErrorCode::Success)
      {
        getfiles_.Response(sequence, GETFILESRESPONSE(files.first));
      }

      getfiles_.Response(sequence, GETFILESRESPONSE(files.second));
      break;
    }
    case Message::GETRECEIVERS:
    {
      if (error)
      {
        HandleError(getreceivers_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<GetReceiversResponse>(nullptr))
      {
        getreceivers_.Response(sequence, GETRECEIVERSRESPONSE(Error(ErrorCode::InvalidMessage, "GetReceiversResponse verification failed")));
        return;
      }

      const GetReceiversResponse* getreceiversresponse = flatbuffers::GetRoot<GetReceiversResponse>(data);
      if (!getreceiversresponse || (getreceiversresponse->receivers() == nullptr))
      {
        getreceivers_.Response(sequence, GETRECEIVERSRESPONSE(Error(ErrorCode::MissingParameter, "GetReceiversResponse missing receivers")));
        return;
      }

      const std::pair< Error, std::vector<RECEIVER> > receivers = GetReceiversBuffer(*getreceiversresponse->receivers());
      if (receivers.first.code_ != ErrorCode::Success)
      {
        getreceivers_.Response(sequence, GETRECEIVERSRESPONSE(receivers.first));
        return;
      }

      getreceivers_.Response(sequence, GETRECEIVERSRESPONSE(receivers.second));
      break;
    }
    case Message::GETRECORDINGS:
    {
      if (error)
      {
        HandleError(getrecordings_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<GetRecordingsResponse>(nullptr))
      {
        getrecordings_.Response(sequence, GETRECORDINGSRESPONSE(Error(ErrorCode::InvalidMessage, "GetRecordingsResponse verification failed")));
        return;
      }

      const GetRecordingsResponse* getrecordingsresponse = flatbuffers::GetRoot<GetRecordingsResponse>(data);
      if (!getrecordingsresponse || (getrecordingsresponse->recordings() == nullptr))
      {
        getrecordings_.Response(sequence, GETRECORDINGSRESPONSE(Error(ErrorCode::MissingParameter, "GetRecordingsResponse missing Recordings")));
        return;
      }

      const std::pair< Error, std::vector<RECORDING> > recordings = GetRecordingsBuffer(*getrecordingsresponse->recordings());
      if (recordings.first.code_ != ErrorCode::Success)
      {
        getrecordings_.Response(sequence, GETRECORDINGSRESPONSE(recordings.first));
        return;
      }

      getrecordings_.Response(sequence, GETRECORDINGSRESPONSE(recordings.second));
      break;
    }
    case Message::GETSNAPSHOT:
    {
      if (error)
      {
        HandleError(getsnapshot_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<GetSnapshotResponse>(nullptr))
      {
        getsnapshot_.Response(sequence, GETSNAPSHOTRESPONSE(Error(ErrorCode::InvalidMessage, "GetSnapshotResponse verification failed")));
        return;
      }

      const GetSnapshotResponse* getsnapshotresponse = flatbuffers::GetRoot<GetSnapshotResponse>(data);
      if (!getsnapshotresponse || (getsnapshotresponse->data() == nullptr))
      {
        getsnapshot_.Response(sequence, GETSNAPSHOTRESPONSE(Error(ErrorCode::MissingParameter, "GetSnapshotResponse missing Recordings")));
        return;
      }

      getsnapshot_.Response(sequence, GETSNAPSHOTRESPONSE(std::vector<uint8_t>(getsnapshotresponse->data()->begin(), getsnapshotresponse->data()->end())));
      break;
    }
    case Message::GETSTATE:
    {
      if (error)
      {
        HandleError(getstate_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<GetStateResponse>(nullptr))
      {
        getstate_.Response(sequence, GETSTATERESPONSE(Error(ErrorCode::InvalidMessage, "GetStateResponse verification failed")));
        return;
      }

      const GetStateResponse* getstateresponse = flatbuffers::GetRoot<GetStateResponse>(data);
      if (!getstateresponse || (getstateresponse->name() == nullptr) || (getstateresponse->publickey() == nullptr) || (getstateresponse->architecture() == nullptr) || (getstateresponse->compiler() == nullptr) || (getstateresponse->version() == nullptr) || (getstateresponse->environmentvariables() == nullptr) || (getstateresponse->commandlinevariables() == nullptr))
      {
        getstate_.Response(sequence, GETSTATERESPONSE(Error(ErrorCode::MissingParameter, "GetStateResponse missing Parameters")));
        return;
      }

      // Name
      std::string name;
      if (getstateresponse->name())
      {
        name = getstateresponse->name()->str();

      }

      // Public Key
      std::string publickey;
      if (getstateresponse->publickey())
      {
        publickey = getstateresponse->publickey()->str();

      }

      // Architecture
      std::string architecture;
      if (getstateresponse->architecture())
      {
        architecture = getstateresponse->architecture()->str();

      }

      // Compiler
      std::string compiler;
      if (getstateresponse->compiler())
      {
        compiler = getstateresponse->compiler()->str();

      }

      // Database Path
      std::string databasepath;
      if (getstateresponse->databasepath())
      {
        databasepath = getstateresponse->databasepath()->str();

      }

      // Environmental variables
      std::vector<std::string> environmentvariables;
      for (const flatbuffers::String* environmentvariable : *getstateresponse->environmentvariables())
      {
        if (environmentvariable == nullptr)
        {

          continue;
        }
        environmentvariables.push_back(environmentvariable->str());
      }

      // Command line variables
      std::vector<std::string> commandlinevariables;
      for (const flatbuffers::String* commandlinevariable : *getstateresponse->commandlinevariables())
      {
        if (commandlinevariable == nullptr)
        {

          continue;
        }
        commandlinevariables.push_back(commandlinevariable->str());
      }

      // Version
      const utility::Version version(getstateresponse->version()->major(), getstateresponse->version()->minor(), getstateresponse->version()->build());

      // ONVIF users
      if (getstateresponse->onvifusers() == nullptr)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing ONVIF users")));
        return;
      }

      const std::pair< Error, std::vector<ONVIFUSER> > onvifusers = GetONVIFUsersBuffer(*getstateresponse->onvifusers());
      if (onvifusers.first.code_ != ErrorCode::Success)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(onvifusers.first));
        return;
      }

      // Groups
      if (getstateresponse->groups() == nullptr)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing Groups")));
        return;
      }

      const std::pair< Error, std::vector<GROUP> > groups = GetGroupsBuffer(*getstateresponse->groups());
      if (groups.first.code_ != ErrorCode::Success)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(groups.first));
        return;
      }

      // Users
      if (getstateresponse->users() == nullptr)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing Users")));
        return;
      }

      const std::pair< Error, std::vector<USER> > users = GetUsersBuffer(*getstateresponse->users());
      if (users.first.code_ != ErrorCode::Success)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(users.first));
        return;
      }

      // Files
      if (getstateresponse->files() == nullptr)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing Files")));
        return;
      }

      const std::pair< Error, std::vector<FILE> > files = GetFilesBuffer(*getstateresponse->files());
      if (files.first.code_ != ErrorCode::Success)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(files.first));
        return;
      }

      // Receivers
      if (getstateresponse->receivers() == nullptr)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing Receivers")));
        return;
      }

      const std::pair< Error, std::vector<RECEIVER> > receivers = GetReceiversBuffer(*getstateresponse->receivers());
      if (receivers.first.code_ != ErrorCode::Success)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(receivers.first));
        return;
      }

      // Recordings
      if (getstateresponse->recordings() == nullptr)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing Recordings")));
        return;
      }

      const std::pair< Error, std::vector<RECORDING> > recordings = GetRecordingsBuffer(*getstateresponse->recordings());
      if (recordings.first.code_ != ErrorCode::Success)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(recordings.first));
        return;
      }

      // Server log
      if (getstateresponse->serverlogmessages() == nullptr)
      {
        getstate_.Response(sequence, GETSTATERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing server log messages")));
        return;
      }

      std::vector<LOGMESSAGE> serverlogmessages;
      serverlogmessages.reserve(getstateresponse->serverlogmessages()->size());
      for (const monocle::LogMessage* logmessage : *getstateresponse->serverlogmessages())
      {
        if (logmessage && logmessage->message())
        {
          serverlogmessages.push_back(LOGMESSAGE(logmessage->time(), logmessage->severity(), logmessage->message()->str()));

        }
      }

      // Maps
      std::vector<MAP> maps;
      if (getstateresponse->maps())
      {
        maps.reserve(getstateresponse->maps()->size());
        for (const monocle::Map* map : *getstateresponse->maps())
        {
          if (map && map->name() && map->location() && map->md5sum())
          {
            maps.push_back(MAP(map->token(), map->name()->str(), map->location()->str(), map->md5sum()->str()));

          }
        }
      }

      // Mount points
      std::vector<MOUNTPOINT> mountpoints;
      if (getstateresponse->mountpoints())
      {
        mountpoints.reserve(getstateresponse->mountpoints()->size());
        for (const monocle::MountPoint* mountpoint : *getstateresponse->mountpoints())
        {
          std::string path;
          if (mountpoint->path())
          {
            path = mountpoint->path()->str();

          }

          std::string type;
          if (mountpoint->type())
          {
            type = mountpoint->type()->str();

          }

          std::string source;
          if (mountpoint->source())
          {
            source = mountpoint->source()->str();

          }

          mountpoints.push_back(MOUNTPOINT(mountpoint->id(), mountpoint->parentid(), mountpoint->majorstdev(), mountpoint->minorstdev(), path, type, source));
        }
      }

      std::string latitude;
      std::string longitude;
      if (getstateresponse->latitude() && getstateresponse->longitude())
      {
        latitude = getstateresponse->latitude()->str();
        longitude = getstateresponse->longitude()->str();
      }

      getstate_.Response(sequence, GETSTATERESPONSE(name, publickey, architecture, getstateresponse->operatingsystem(), compiler, databasepath, version, getstateresponse->identifier(), environmentvariables, commandlinevariables, onvifusers.second, groups.second, users.second, files.second, receivers.second, recordings.second, serverlogmessages, getstateresponse->maxrecordings(), maps, mountpoints, latitude, longitude));
      break;
    }
    case Message::GETTIME:
    {
      if (error)
      {
        HandleError(keepalive_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<GetTimeResponse>(nullptr))
      {
        gettime_.Response(sequence, GETTIMERESPONSE(Error(ErrorCode::InvalidMessage, "GetTimeResponse verification failed")));
        return;
      }

      const GetTimeResponse* gettimeresponse = flatbuffers::GetRoot<GetTimeResponse>(data);
      if (!gettimeresponse)
      {
        gettime_.Response(sequence, GETTIMERESPONSE(Error(ErrorCode::MissingParameter, "GetTimeResponse invalid time response")));
        return;
      }

      gettime_.Response(sequence, GETTIMERESPONSE(gettimeresponse->utctime()));
      break;
    }
    case Message::KEEPALIVE:
    {
      if (error)
      {
        HandleError(keepalive_, sequence, data, datasize);
        return;
      }

      keepalive_.Response(sequence, KEEPALIVERESPONSE());
      break;
    }
    case Message::MOUNTFILE:
    {
      if (error)
      {
        HandleError(mountfile_, sequence, data, datasize);
        return;
      }

      mountfile_.Response(sequence, MOUNTFILERESPONSE());
      break;
    }
    case Message::FILEADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::File>(nullptr))
      {

        return;
      }

      const monocle::File* file = flatbuffers::GetRoot<monocle::File>(data);
      if (!file)
      {

        return;
      }

      std::string path;
      if (file->path())
      {
        path = file->path()->str();

      }

      std::string mountpoint;
      if (file->mountpoint())
      {
        mountpoint = file->mountpoint()->str();

      }

      FileAdded(file->token(), path, mountpoint, file->numchunks(), file->chunksize(), file->automount(), file->state(), file->monitorstate());
      break;
    }
    case Message::FILEREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::FileRemoved>(nullptr))
      {

        return;
      }

      const monocle::FileRemoved* fileremoved = flatbuffers::GetRoot<monocle::FileRemoved>(data);
      if (!fileremoved)
      {

        return;
      }

      FileRemoved(fileremoved->token());
      break;
    }
    case Message::FILEMONITORSTATECHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::FileMonitorStateChanged>(nullptr))
      {

        return;
      }

      const monocle::FileMonitorStateChanged* filemonitorstatechanged = flatbuffers::GetRoot<monocle::FileMonitorStateChanged>(data);
      if (!filemonitorstatechanged)
      {

        return;
      }

      FileMonitorStateChanged(filemonitorstatechanged->token(), filemonitorstatechanged->state());
      break;
    }
    case Message::FILESTATECHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::FileStateChanged>(nullptr))
      {

        return;
      }

      const monocle::FileStateChanged* filestatechanged = flatbuffers::GetRoot<monocle::FileStateChanged>(data);
      if (!filestatechanged)
      {

        return;
      }

      FileStateChanged(filestatechanged->token(), filestatechanged->state());
      break;
    }
    case Message::FINDMOTIONEND:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::FindMotionEnd>(nullptr))
      {

        return;
      }

      const monocle::FindMotionEnd* findmotionend = flatbuffers::GetRoot<monocle::FindMotionEnd>(data);
      if (!findmotionend)
      {

        return;
      }

      FindMotionEnd(findmotionend->token(), findmotionend->ret());
      break;
    }
    case Message::FINDMOTIONPROGRESS:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::FindMotionProgress>(nullptr))
      {

        return;
      }

      const monocle::FindMotionProgress* findmotionprogress = flatbuffers::GetRoot<monocle::FindMotionProgress>(data);
      if (!findmotionprogress)
      {

        return;
      }

      FindMotionProgress(findmotionprogress->token(), findmotionprogress->progress());
      break;
    }
    case Message::FINDMOTIONRESULT:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::FindMotionResult>(nullptr))
      {

        return;
      }

      const monocle::FindMotionResult* findmotionresult = flatbuffers::GetRoot<monocle::FindMotionResult>(data);
      if (!findmotionresult)
      {

        return;
      }

      FindMotionResult(findmotionresult->token(), findmotionresult->start(), findmotionresult->end());
      break;
    }
    case Message::FINDOBJECTEND:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::FindObjectEnd>(nullptr))
      {

        return;
      }

      const monocle::FindObjectEnd* findobjectend = flatbuffers::GetRoot<monocle::FindObjectEnd>(data);
      if (!findobjectend)
      {

        return;
      }

      FindObjectEnd(findobjectend->token(), findobjectend->ret());
      break;
    }
    case Message::FINDOBJECTPROGRESS:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::FindObjectProgress>(nullptr))
      {

        return;
      }

      const monocle::FindObjectProgress* findobjectprogress = flatbuffers::GetRoot<monocle::FindObjectProgress>(data);
      if (!findobjectprogress)
      {

        return;
      }

      FindObjectProgress(findobjectprogress->token(), findobjectprogress->progress());
      break;
    }
    case Message::FINDOBJECTRESULT:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::FindObjectResult>(nullptr))
      {

        return;
      }

      const monocle::FindObjectResult* findobjectresult = flatbuffers::GetRoot<monocle::FindObjectResult>(data);
      if (!findobjectresult)
      {

        return;
      }

      FindObjectResult(findobjectresult->token(), findobjectresult->start(), findobjectresult->end(), findobjectresult->objectclass(), findobjectresult->id(), findobjectresult->largesttime(), findobjectresult->largestx(), findobjectresult->largesty(), findobjectresult->largestwidth(), findobjectresult->largestheight());
      break;
    }
    case Message::GETCHILDFOLDERS:
    {
      if (error)
      {
        HandleError(getchildfolders_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::GetChildFoldersResponse>(nullptr))
      {
        HandleError(getchildfolders_, sequence, data, datasize);
        return;
      }

      const monocle::GetChildFoldersResponse* getchildfoldersresponse = flatbuffers::GetRoot<monocle::GetChildFoldersResponse>(data);
      if (!getchildfoldersresponse)
      {
        HandleError(getchildfolders_, sequence, data, datasize);
        return;
      }

      std::vector<std::string> childfolders;
      if (getchildfoldersresponse->childfolders())
      {
        childfolders.reserve(getchildfoldersresponse->childfolders()->size());
        for (const flatbuffers::String* childfolder : *getchildfoldersresponse->childfolders())
        {
          childfolders.push_back(childfolder->str());

        }
      }

      getchildfolders_.Response(sequence, GETCHILDFOLDERSRESPONSE(childfolders));
      break;
    }
    case Message::GOODBYE:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }
      Goodbye();
      break;
    }
    case Message::GROUPADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::GroupAdded>(nullptr))
      {

        return;
      }

      const monocle::GroupAdded* groupadded = flatbuffers::GetRoot<monocle::GroupAdded>(data);
      if (!groupadded || (groupadded->name() == nullptr))
      {

        return;
      }

      std::vector<uint64_t> recordings;
      if (groupadded->recordings())
      {
        recordings.reserve(groupadded->recordings()->size());
        for (const uint64_t recording : *groupadded->recordings())
        {
          recordings.push_back(recording);

        }
      }

      GroupAdded(groupadded->token(), groupadded->name()->str(), groupadded->manageusers(), groupadded->managerecordings(), groupadded->managemaps(), groupadded->managedevice(), groupadded->allrecordings(), recordings);
      break;
    }
    case Message::GROUPCHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::GroupChanged>(nullptr))
      {

        return;
      }

      const monocle::GroupChanged* groupchanged = flatbuffers::GetRoot<monocle::GroupChanged>(data);
      if (!groupchanged || (groupchanged->name() == nullptr))
      {

        return;
      }

      std::vector<uint64_t> recordings;
      if (groupchanged->recordings())
      {
        recordings.reserve(groupchanged->recordings()->size());
        for (const uint64_t recording : *groupchanged->recordings())
        {
          recordings.push_back(recording);

        }
      }

      GroupChanged(groupchanged->token(), groupchanged->name()->str(), groupchanged->manageusers(), groupchanged->managerecordings(), groupchanged->managemaps(), groupchanged->managedevice(), groupchanged->allrecordings(), recordings);
      break;
    }
    case Message::GROUPREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::GroupRemoved>(nullptr))
      {

        return;
      }

      const monocle::GroupRemoved* groupremoved = flatbuffers::GetRoot<monocle::GroupRemoved>(data);
      if (!groupremoved)
      {

        return;
      }

      GroupRemoved(groupremoved->token());
      break;
    }
    case Message::H265FRAME:
    {
      if (sizeof(uint32_t) > datasize)
      {
        // Ignore illegal packets
        return;
      }
      const uint32_t headersize = *reinterpret_cast<const uint32_t*>(data);
      if ((sizeof(uint32_t) + headersize) > datasize)
      {
        // Ignore illegal packets
        return;
      }
      data += sizeof(headersize);
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), headersize).VerifyBuffer<H265FrameHeader>(nullptr))
      {
        // Ignore illegal packets
        return;
      }
      const H265FrameHeader* h265frameheader = flatbuffers::GetRoot<H265FrameHeader>(data);
      if (!h265frameheader)
      {
        // Ignore illegal packets
        return;
      }
      data += headersize;

      const uint32_t* offsets = nullptr;
      size_t numoffsets = 0;
      if (h265frameheader->offsets())
      {
        offsets = h265frameheader->offsets()->data();
        numoffsets = h265frameheader->offsets()->size();
      }

      H265Frame(h265frameheader->token(), h265frameheader->playrequestindex(), h265frameheader->codecindex(), h265frameheader->marker(), h265frameheader->timestamp(), h265frameheader->sequencenum(), h265frameheader->progress(), h265frameheader->signature() ? h265frameheader->signature()->data() : nullptr, h265frameheader->signature() ? h265frameheader->signature()->size() : 0, h265frameheader->donlfield(), offsets, numoffsets, data, datasize - (sizeof(headersize) + headersize));
      break;
    }
    case Message::H264FRAME:
    {
      if (sizeof(uint32_t) > datasize)
      {
        // Ignore illegal packets
        return;
      }
      const uint32_t headersize = *reinterpret_cast<const uint32_t*>(data);
      if ((sizeof(uint32_t) + headersize) > datasize)
      {
        // Ignore illegal packets
        return;
      }
      data += sizeof(headersize);
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), headersize).VerifyBuffer<H264FrameHeader>(nullptr))
      {
        // Ignore illegal packets
        return;
      }
      const H264FrameHeader* h264frameheader = flatbuffers::GetRoot<H264FrameHeader>(data);
      if (!h264frameheader)
      {
        // Ignore illegal packets
        return;
      }
      data += headersize;

      const uint32_t* offsets = nullptr;
      size_t numoffsets = 0;
      if (h264frameheader->offsets())
      {
        offsets = h264frameheader->offsets()->data();
        numoffsets = h264frameheader->offsets()->size();
      }

      H264Frame(h264frameheader->token(), h264frameheader->playrequestindex(), h264frameheader->codecindex(), h264frameheader->marker(), h264frameheader->timestamp(), h264frameheader->sequencenum(), h264frameheader->progress(), h264frameheader->signature() ? h264frameheader->signature()->data() : nullptr, h264frameheader->signature() ? h264frameheader->signature()->size() : 0, offsets, numoffsets, data, datasize - (sizeof(headersize) + headersize));
      break;
    }
    case Message::HARDWARESTATS:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::HardwareStats>(nullptr))
      {

        return;
      }

      const monocle::HardwareStats* hardwarestats = flatbuffers::GetRoot<monocle::HardwareStats>(data);
      if (!hardwarestats)
      {

        return;
      }

      std::vector<monocle::DISKSTAT> diskstats;
      if (hardwarestats->diskstats())
      {
        diskstats.reserve(hardwarestats->diskstats()->size());
        for (const monocle::DiskStat* diskstat : *hardwarestats->diskstats())
        {
          if (!diskstat->device())
          {

            continue;
          }
          diskstats.push_back(monocle::DISKSTAT(diskstat->device()->str(), diskstat->utility(), diskstat->readspeed(), diskstat->writespeed()));
        }
      }

      std::vector<monocle::GPUSTAT> gpustats;
      if (hardwarestats->gpustats())
      {
        gpustats.reserve(hardwarestats->gpustats()->size());
        for (const monocle::GPUStat* gpustat : *hardwarestats->gpustats())
        {
          if (!gpustat->uuid() || !gpustat->name())
          {

            continue;
          }
          gpustats.push_back(monocle::GPUSTAT(gpustat->uuid()->str(), gpustat->name()->str(), gpustat->gpuusage(), gpustat->memoryusage(), gpustat->totalmemory(), gpustat->usedmemory()));
        }
      }

      HardwareStatsMessage(hardwarestats->time(), diskstats, hardwarestats->cpuusage(), hardwarestats->totalmemory(), hardwarestats->availablememory(), gpustats);
      break;
    }
    case Message::JPEGFRAME:
    {
      if (sizeof(uint32_t) > datasize)
      {
        // Ignore illegal packets
        return;
      }
      const uint32_t headersize = *reinterpret_cast<const uint32_t*>(data);
      if ((sizeof(uint32_t) + headersize) > datasize)
      {
        // Ignore illegal packets
        return;
      }
      data += sizeof(headersize);
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), headersize).VerifyBuffer<JPEGFrameHeader>(nullptr))
      {
        // Ignore illegal packets
        return;
      }
      const JPEGFrameHeader* jpegframeheader = flatbuffers::GetRoot<JPEGFrameHeader>(data);
      if (!jpegframeheader)
      {
        // Ignore illegal packets
        return;
      }
      data += headersize;

      const uint8_t* lqt = nullptr;
      if (jpegframeheader->lqt())
      {
        lqt = jpegframeheader->lqt()->data();

      }

      const uint8_t* cqt = nullptr;
      if (jpegframeheader->cqt())
      {
        cqt = jpegframeheader->lqt()->data();

      }

      JPEGFrame(jpegframeheader->token(), jpegframeheader->playrequestindex(), jpegframeheader->codecindex(), jpegframeheader->timestamp(), jpegframeheader->sequencenum(), jpegframeheader->progress(), jpegframeheader->signature() ? jpegframeheader->signature()->data() : nullptr, jpegframeheader->signature() ? jpegframeheader->signature()->size() : 0, jpegframeheader->restartinterval(), jpegframeheader->typespecificfragmentoffset(), jpegframeheader->type(), jpegframeheader->q(), jpegframeheader->width(), jpegframeheader->height(), lqt, cqt, data, datasize - (sizeof(headersize) + headersize));
      break;
    }
    case Message::LOCATIONCHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::LocationChanged>(nullptr))
      {

        return;
      }

      const monocle::LocationChanged* locationchanged = flatbuffers::GetRoot<monocle::LocationChanged>(data);
      if (locationchanged == nullptr)
      {

        return;
      }

      std::string latitude;
      std::string longitude;
      if (locationchanged->latitude() && locationchanged->longitude())
      {
        latitude = locationchanged->latitude()->str();
        longitude = locationchanged->longitude()->str();
      }

      LocationChanged(latitude, longitude);
      break;
    }
    case Message::LAYOUTADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::LayoutAdded>(nullptr))
      {

        return;
      }

      const monocle::LayoutAdded* layoutadded = flatbuffers::GetRoot<monocle::LayoutAdded>(data);
      if ((layoutadded == nullptr) || (layoutadded->layout() == nullptr))
      {

        return;
      }

      std::vector<monocle::LAYOUTWINDOW> windows;
      if (layoutadded->layout()->windows())
      {
        windows.reserve(layoutadded->layout()->windows()->size());
        for (const monocle::LayoutWindow* window : *layoutadded->layout()->windows())
        {
          std::vector<monocle::LAYOUTVIEW> maps;
          if (window->maps())
          {
            maps.reserve(window->maps()->size());
            for (const monocle::LayoutView* map : *window->maps())
            {
              maps.push_back(monocle::LAYOUTVIEW(map->token(), map->x(), map->y(), map->width(), map->height()));

            }
          }

          std::vector<monocle::LAYOUTVIEW> recordings;
          if (window->recordings())
          {
            recordings.reserve(window->recordings()->size());
            for (const monocle::LayoutView* recording : *window->recordings())
            {
              recordings.push_back(monocle::LAYOUTVIEW(recording->token(), recording->x(), recording->y(), recording->width(), recording->height()));

            }
          }

          windows.push_back(monocle::LAYOUTWINDOW(window->token(), window->mainwindow(), window->maximised(), window->screenx(), window->screeny(), window->screenwidth(), window->screenheight(), window->x(), window->y(), window->width(), window->height(), window->gridwidth(), window->gridheight(), maps, recordings));
        }
      }

      LayoutAdded(monocle::LAYOUT(layoutadded->layout()->token(), layoutadded->layout()->name() ? layoutadded->layout()->name()->str() : std::string(), windows));
      break;
    }
    case Message::MAPADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::MapAdded>(nullptr))
      {

        return;
      }

      const monocle::MapAdded* mapadded = flatbuffers::GetRoot<monocle::MapAdded>(data);
      if (mapadded == nullptr)
      {

        return;
      }

      std::string name;
      if (mapadded->name())
      {
        name = mapadded->name()->str();

      }

      std::string location;
      if (mapadded->location())
      {
        location = mapadded->location()->str();

      }

      std::string imagemd5;
      if (mapadded->imagemd5())
      {
        imagemd5 = mapadded->imagemd5()->str();

      }

      MapAdded(mapadded->token(), name, location, imagemd5);
      break;
    }
    case Message::MAPCHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::MapChanged>(nullptr))
      {

        return;
      }
      
      const monocle::MapChanged* mapchanged = flatbuffers::GetRoot<monocle::MapChanged>(data);
      if (mapchanged == nullptr)
      {

        return;
      }

      std::string name;
      if (mapchanged->name())
      {
        name = mapchanged->name()->str();

      }

      std::string location;
      if (mapchanged->location())
      {
        location = mapchanged->location()->str();

      }

      std::string imagemd5;
      if (mapchanged->imagemd5())
      {
        imagemd5 = mapchanged->imagemd5()->str();

      }

      MapChanged(mapchanged->token(), name, location, imagemd5);
      break;
    }
    case Message::MAPREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::MapRemoved>(nullptr))
      {

        return;
      }

      const monocle::MapRemoved* mapremoved = flatbuffers::GetRoot<monocle::MapRemoved>(data);
      if (!mapremoved)
      {

        return;
      }

      MapRemoved(mapremoved->token());
      break;
    }
    case Message::METADATAFRAME:
    {
      if (sizeof(uint32_t) > datasize)
      {
        // Ignore illegal packets
        return;
      }
      const uint32_t headersize = *reinterpret_cast<const uint32_t*>(data);
      if ((sizeof(uint32_t) + headersize) > datasize)
      {
        // Ignore illegal packets
        return;
      }
      data += sizeof(headersize);
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), headersize).VerifyBuffer<MetadataFrameHeader>(nullptr))
      {
        // Ignore illegal packets
        return;
      }
      const MetadataFrameHeader* metadataframeheader = flatbuffers::GetRoot<MetadataFrameHeader>(data);
      if (!metadataframeheader)
      {
        // Ignore illegal packets
        return;
      }
      data += headersize;
      MetadataFrame(metadataframeheader->token(), metadataframeheader->playrequestindex(), metadataframeheader->codecindex(), metadataframeheader->timestamp(), metadataframeheader->sequencenum(), metadataframeheader->progress(), metadataframeheader->signature() ? metadataframeheader->signature()->data() : nullptr, metadataframeheader->signature() ? metadataframeheader->signature()->size() : 0, metadataframeheader->metadataframetype(), data, datasize - (sizeof(headersize) + headersize));
      break;
    }
    case Message::MOUNTPOINTADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::MountPointAdded>(nullptr))
      {

        return;
      }

      const monocle::MountPointAdded* mountpointadded = flatbuffers::GetRoot<monocle::MountPointAdded>(data);
      if (!mountpointadded)
      {

        return;
      }

      std::string path;
      if (mountpointadded->path())
      {
        path = mountpointadded->path()->str();

      }

      std::string type;
      if (mountpointadded->type())
      {
        type = mountpointadded->type()->str();

      }

      std::string source;
      if (mountpointadded->source())
      {
        source = mountpointadded->source()->str();

      }

      MountPointAdded(mountpointadded->id(), mountpointadded->parentid(), mountpointadded->majorstdev(), mountpointadded->minorstdev(), path, type, source);
      break;
    }
    case Message::MOUNTPOINTREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::MountPointRemoved>(nullptr))
      {

        return;
      }

      const monocle::MountPointRemoved* mountpointremoved = flatbuffers::GetRoot<monocle::MountPointRemoved>(data);
      if (!mountpointremoved)
      {

        return;
      }

      std::string path;
      if (mountpointremoved->path())
      {
        path = mountpointremoved->path()->str();

      }

      std::string type;
      if (mountpointremoved->type())
      {
        type = mountpointremoved->type()->str();

      }

      std::string source;
      if (mountpointremoved->source())
      {
        source = mountpointremoved->source()->str();

      }

      MountPointRemoved(mountpointremoved->id(), mountpointremoved->parentid(), mountpointremoved->majorstdev(), mountpointremoved->minorstdev(), path, type, source);
      break;
    }
    case Message::MPEG4FRAME:
    {
      if (sizeof(uint32_t) > datasize)
      {
        // Ignore illegal packets
        return;
      }
      const uint32_t headersize = *reinterpret_cast<const uint32_t*>(data);
      if ((sizeof(uint32_t) + headersize) > datasize)
      {
        // Ignore illegal packets
        return;
      }
      data += sizeof(headersize);
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), headersize).VerifyBuffer<MPEG4FrameHeader>(nullptr))
      {
        // Ignore illegal packets
        return;
      }
      const MPEG4FrameHeader* mpeg4frameheader = flatbuffers::GetRoot<MPEG4FrameHeader>(data);
      if (!mpeg4frameheader)
      {
        // Ignore illegal packets
        return;
      }
      data += headersize;
      MPEG4Frame(mpeg4frameheader->token(), mpeg4frameheader->playrequestindex(), mpeg4frameheader->codecindex(), mpeg4frameheader->marker(), mpeg4frameheader->timestamp(), mpeg4frameheader->sequencenum(), mpeg4frameheader->progress(), mpeg4frameheader->signature() ? mpeg4frameheader->signature()->data() : nullptr, mpeg4frameheader->signature() ? mpeg4frameheader->signature()->size() : 0, data, datasize - (sizeof(headersize) + headersize));
      break;
    }
    case Message::NAMECHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::NameChanged>(nullptr))
      {

        return;
      }

      const monocle::NameChanged* namechanged = flatbuffers::GetRoot<monocle::NameChanged>(data);
      if ((namechanged == nullptr))
      {

        return;
      }

      NameChanged(namechanged->name() ? namechanged->name()->str() : std::string());
      break;
    }
    case Message::OBJECTDETECTORFRAME:
    {
      if (sizeof(uint32_t) > datasize)
      {
        // Ignore illegal packets
        return;
      }
      const uint32_t headersize = *reinterpret_cast<const uint32_t*>(data);
      if ((sizeof(uint32_t) + headersize) > datasize)
      {
        // Ignore illegal packets
        return;
      }
      data += sizeof(headersize);
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), headersize).VerifyBuffer<ObjectDetectorFrameHeader>(nullptr))
      {
        // Ignore illegal packets
        return;
      }
      const ObjectDetectorFrameHeader* objectdetectorframeheader = flatbuffers::GetRoot<ObjectDetectorFrameHeader>(data);
      if (!objectdetectorframeheader)
      {
        // Ignore illegal packets
        return;
      }
      data += headersize;
      ObjectDetectorFrame(objectdetectorframeheader->token(), objectdetectorframeheader->playrequestindex(), objectdetectorframeheader->codecindex(), objectdetectorframeheader->timestamp(), objectdetectorframeheader->sequencenum(), objectdetectorframeheader->progress(), objectdetectorframeheader->signature() ? objectdetectorframeheader->signature()->data() : nullptr, objectdetectorframeheader->signature() ? objectdetectorframeheader->signature()->size() : 0, objectdetectorframeheader->objectdetectorframetype(), data, datasize - (sizeof(headersize) + headersize));
      break;
    }
    case Message::NEWCODECINDEX:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::NewCodecIndex>(nullptr))
      {

        return;
      }

      const monocle::NewCodecIndex* newcodecindex = flatbuffers::GetRoot<monocle::NewCodecIndex>(data);
      if ((newcodecindex == nullptr) || (newcodecindex->codecindex() == nullptr))
      {

        return;
      }

      std::string parameters;
      if (newcodecindex->codecindex()->parameters())
      {
        parameters = newcodecindex->codecindex()->parameters()->str();

      }

      NewCodecIndex(newcodecindex->stream(), newcodecindex->codecindex()->id(), newcodecindex->codecindex()->codec(), parameters, newcodecindex->codecindex()->timestamp());
      break;
    }
    case Message::ONVIFUSERADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::ONVIFUserAdded>(nullptr))
      {

        return;
      }

      const monocle::ONVIFUserAdded* onvifuseradded = flatbuffers::GetRoot<monocle::ONVIFUserAdded>(data);
      if (!onvifuseradded || (onvifuseradded->username() == nullptr))
      {

        return;
      }

      ONVIFUserAdded(onvifuseradded->token(), onvifuseradded->username()->str(), onvifuseradded->onvifuserlevel());
      break;
    }
    case Message::ONVIFUSERCHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::ONVIFUserChanged>(nullptr))
      {

        return;
      }

      const monocle::ONVIFUserChanged* onvifuserchanged = flatbuffers::GetRoot<monocle::ONVIFUserChanged>(data);
      if (!onvifuserchanged)
      {

        return;
      }

      boost::optional<std::string> username;
      if (onvifuserchanged->username())
      {
        username = onvifuserchanged->username()->str();

      }

      ONVIFUserChanged(onvifuserchanged->token(), username, onvifuserchanged->onvifuserlevel());
      break;
    }
    case Message::ONVIFUSERREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::ONVIFUserRemoved>(nullptr))
      {

        return;
      }

      const monocle::ONVIFUserRemoved* onvifuserremoved = flatbuffers::GetRoot<monocle::ONVIFUserRemoved>(data);
      if (!onvifuserremoved)
      {

        return;
      }

      ONVIFUserRemoved(onvifuserremoved->token());
      break;
    }
    case Message::RECEIVERADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::Receiver>(nullptr))
      {

        return;
      }

      const monocle::Receiver* receiver = flatbuffers::GetRoot<monocle::Receiver>(data);
      if (!receiver || (receiver->mediauri() == nullptr))
      {

        return;
      }

      std::string username;
      if (receiver->username())
      {
        username = receiver->username()->str();

      }

      std::string password;
      if (receiver)
      {
        password = receiver->password()->str();

      }

      std::vector<std::string> parameters;
      if (receiver->parameters())
      {
        parameters.reserve(receiver->parameters()->size());
        for (const flatbuffers::String* parameter : *receiver->parameters())
        {
          parameters.push_back(parameter->str());

        }
      }

      ReceiverAdded(receiver->token(), receiver->mode(), receiver->mediauri()->str(), receiver->autocreated(), username, password, parameters, receiver->state());
      break;
    }
    case Message::RECEIVERREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::ReceiverRemoved>(nullptr))
      {

        return;
      }

      const monocle::ReceiverRemoved* receiverremoved = flatbuffers::GetRoot<monocle::ReceiverRemoved>(data);
      if (!receiverremoved)
      {

        return;
      }

      ReceiverRemoved(receiverremoved->token());
      break;
    }
    case Message::RECORDINGREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingRemoved>(nullptr))
      {

        return;
      }

      const monocle::RecordingRemoved* recordingremoved = flatbuffers::GetRoot<monocle::RecordingRemoved>(data);
      if (!recordingremoved)
      {

        return;
      }

      RecordingRemoved(recordingremoved->token());
      break;
    }
    case Message::RECORDINGJOBREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingJobRemoved>(nullptr))
      {

        return;
      }

      const monocle::RecordingJobRemoved* recordingjobremoved = flatbuffers::GetRoot<monocle::RecordingJobRemoved>(data);
      if (!recordingjobremoved)
      {

        return;
      }

      RecordingJobRemoved(recordingjobremoved->recordingtoken(), recordingjobremoved->token());
      break;
    }
    case Message::RECORDINGJOBSOURCEADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingJobSourceAdded>(nullptr))
      {

        return;
      }

      const monocle::RecordingJobSourceAdded* recordingjobsourceadded = flatbuffers::GetRoot<monocle::RecordingJobSourceAdded>(data);
      if (!recordingjobsourceadded)
      {

        return;
      }

      RecordingJobSourceAdded(recordingjobsourceadded->recordingtoken(), recordingjobsourceadded->recordingjobtoken(), recordingjobsourceadded->recordingjobsourcetoken(), recordingjobsourceadded->receivertoken());
      break;
    }
    case Message::RECORDINGJOBSOURCEREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingJobSourceRemoved>(nullptr))
      {

        return;
      }

      const monocle::RecordingJobSourceRemoved* recordingjobsourceremoved = flatbuffers::GetRoot<monocle::RecordingJobSourceRemoved>(data);
      if (!recordingjobsourceremoved)
      {

        return;
      }

      RecordingJobSourceRemoved(recordingjobsourceremoved->recordingtoken(), recordingjobsourceremoved->recordingjobtoken(), recordingjobsourceremoved->token());
      break;
    }
    case Message::RECORDINGJOBSOURCETRACKADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingJobSourceTrackAdded>(nullptr))
      {

        return;
      }

      const monocle::RecordingJobSourceTrackAdded* recordingjobsourcetrackadded = flatbuffers::GetRoot<monocle::RecordingJobSourceTrackAdded>(data);
      if (!recordingjobsourcetrackadded)
      {

        return;
      }

      std::vector<std::string> parameters;
      if (recordingjobsourcetrackadded->parameters())
      {
        parameters.reserve(recordingjobsourcetrackadded->parameters()->size());
        for (const flatbuffers::String* parameter : *recordingjobsourcetrackadded->parameters())
        {
          parameters.push_back(parameter->str());

        }
      }

      std::string error;
      if (recordingjobsourcetrackadded->error())
      {
        error = recordingjobsourcetrackadded->error()->str();

      }

      std::vector<std::string> activeparameters;
      if (recordingjobsourcetrackadded->activeparameters())
      {
        activeparameters.reserve(recordingjobsourcetrackadded->activeparameters()->size());
        for (const flatbuffers::String* activeparameter : *recordingjobsourcetrackadded->activeparameters())
        {
          activeparameters.push_back(activeparameter->str());

        }
      }

      RecordingJobSourceTrackAdded(recordingjobsourcetrackadded->recordingtoken(), recordingjobsourcetrackadded->recordingjobtoken(), recordingjobsourcetrackadded->recordingjobsourcetoken(), recordingjobsourcetrackadded->recordingjobsourcetracktoken(), recordingjobsourcetrackadded->recordingtrackid(), parameters, recordingjobsourcetrackadded->state(), error, activeparameters);
      break;
    }
    case Message::RECORDINGJOBSOURCETRACKLOGMESSAGE:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingJobSourceTrackLogMessage>(nullptr))
      {

        return;
      }

      const monocle::RecordingJobSourceTrackLogMessage* recordingjobsourcetracklogmessage = flatbuffers::GetRoot<monocle::RecordingJobSourceTrackLogMessage>(data);
      if (!recordingjobsourcetracklogmessage || (recordingjobsourcetracklogmessage->message() == nullptr) || (recordingjobsourcetracklogmessage->message()->message() == nullptr))
      {

        return;
      }

      RecordingJobSourceTrackLogMessage(recordingjobsourcetracklogmessage->recordingtoken(), recordingjobsourcetracklogmessage->recordingjobtoken(), recordingjobsourcetracklogmessage->recordingjobsourcetoken(), recordingjobsourcetracklogmessage->token(), recordingjobsourcetracklogmessage->message()->time(), recordingjobsourcetracklogmessage->message()->severity(), recordingjobsourcetracklogmessage->message()->message()->str());
      break;
    }
    case Message::RECORDINGJOBSOURCETRACKREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingJobSourceTrackRemoved>(nullptr))
      {

        return;
      }

      const monocle::RecordingJobSourceTrackRemoved* recordingjobsourcetrackremoved = flatbuffers::GetRoot<monocle::RecordingJobSourceTrackRemoved>(data);
      if (!recordingjobsourcetrackremoved)
      {

        return;
      }

      RecordingJobSourceTrackRemoved(recordingjobsourcetrackremoved->recordingtoken(), recordingjobsourcetrackremoved->recordingjobtoken(), recordingjobsourcetrackremoved->recordingjobsourcetoken(), recordingjobsourcetrackremoved->token());
      break;
    }
    case Message::RECEIVERCHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::ReceiverChanged>(nullptr))
      {

        return;
      }
      
      const monocle::ReceiverChanged* receiverchanged = flatbuffers::GetRoot<monocle::ReceiverChanged>(data);
      if (!receiverchanged)
      {

        return;
      }

      std::string mediauri;
      if (receiverchanged->mediauri())
      {
        mediauri = receiverchanged->mediauri()->str();

      }

      std::string username;
      if (receiverchanged->username())
      {
        username = receiverchanged->username()->str();

      }

      std::string password;
      if (receiverchanged->password())
      {
        password = receiverchanged->password()->str();

      }

      std::vector<std::string> parameters;
      if (receiverchanged->parameters())
      {
        parameters.reserve(receiverchanged->parameters()->size());
        for (const flatbuffers::String* parameter : *receiverchanged->parameters())
        {
          parameters.push_back(parameter->str());

        }
      }

      ReceiverChanged(receiverchanged->token(), receiverchanged->mode(), mediauri, receiverchanged->autocreated(), username, password, parameters);
      break;
    }
    case Message::RECORDINGACTIVEJOBCHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingActiveJobChanged>(nullptr))
      {

        return;
      }

      const monocle::RecordingActiveJobChanged* recordingactivejobchanged = flatbuffers::GetRoot<monocle::RecordingActiveJobChanged>(data);
      if (!recordingactivejobchanged)
      {

        return;
      }

      boost::optional<uint64_t> activejob;
      if (recordingactivejobchanged->activejob())
      {
        activejob = recordingactivejobchanged->activejob()->token();

      }

      RecordingActiveJobChanged(recordingactivejobchanged->token(), activejob);
      break;
    }
    case Message::RECORDINGADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingAdded>(nullptr))
      {

        return;
      }

      const monocle::RecordingAdded* recordingadded = flatbuffers::GetRoot<monocle::RecordingAdded>(data);
      if (!recordingadded)
      {

        return;
      }

      std::string sourceid;
      if (recordingadded->sourceid())
      {
        sourceid = recordingadded->sourceid()->str();

      }

      std::string name;
      if (recordingadded->name())
      {
        name = recordingadded->name()->str();

      }

      std::string location;
      if (recordingadded->location())
      {
        location = recordingadded->location()->str();

      }

      std::string description;
      if (recordingadded->description())
      {
        description = recordingadded->description()->str();

      }

      std::string address;
      if (recordingadded->address())
      {
        address = recordingadded->address()->str();

      }

      std::string content;
      if (recordingadded->content())
      {
        content = recordingadded->content()->str();

      }

      boost::optional<uint64_t> activejob;
      if (recordingadded->activejob())
      {
        activejob = recordingadded->activejob()->token();

      }

      RecordingAdded(recordingadded->token(), sourceid, name, location, description, address, content, recordingadded->retentiontime(), activejob);
      break;
    }
    case Message::RECORDINGCHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingChanged>(nullptr))
      {

        return;
      }

      const monocle::RecordingChanged* recordingchanged = flatbuffers::GetRoot<monocle::RecordingChanged>(data);
      if (!recordingchanged)
      {

        return;
      }

      std::string sourceid;
      if (recordingchanged->sourceid())
      {
        sourceid = recordingchanged->sourceid()->str();

      }

      std::string name;
      if (recordingchanged->name())
      {
        name = recordingchanged->name()->str();

      }

      std::string location;
      if (recordingchanged->location())
      {
        location = recordingchanged->location()->str();

      }

      std::string description;
      if (recordingchanged->description())
      {
        description = recordingchanged->description()->str();

      }

      std::string address;
      if (recordingchanged->address())
      {
        address = recordingchanged->address()->str();

      }

      std::string content;
      if (recordingchanged->content())
      {
        content = recordingchanged->content()->str();

      }

      boost::optional<uint64_t> activejob;
      if (recordingchanged->activejob())
      {
        activejob = recordingchanged->activejob()->token();

      }

      RecordingChanged(recordingchanged->token(), sourceid, name, location, description, address, content, recordingchanged->retentiontime(), activejob);
      break;
    }
    case Message::RECORDINGJOBADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingJobAdded>(nullptr))
      {

        return;
      }

      const monocle::RecordingJobAdded* recordingjobadded = flatbuffers::GetRoot<monocle::RecordingJobAdded>(data);
      if (!recordingjobadded)
      {

        return;
      }

      std::string name;
      if (recordingjobadded->name())
      {
        name = recordingjobadded->name()->str();

      }

      RecordingJobAdded(recordingjobadded->recordingtoken(), recordingjobadded->token(), name, recordingjobadded->enabled(), recordingjobadded->priority(), recordingjobadded->state());
      break;
    }
    case Message::RECORDINGJOBCHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingJobChanged>(nullptr))
      {

        return;
      }

      const monocle::RecordingJobChanged* recordingjobchanged = flatbuffers::GetRoot<monocle::RecordingJobChanged>(data);
      if (!recordingjobchanged)
      {

        return;
      }

      std::string name;
      if (recordingjobchanged->name())
      {
        name = recordingjobchanged->name()->str();

      }

      RecordingJobChanged(recordingjobchanged->recordingtoken(), recordingjobchanged->token(), name, recordingjobchanged->enabled(), recordingjobchanged->priority());
      break;
    }
    case Message::RECORDINGJOBSOURCETRACKACTIVEPARAMETERSCHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingJobSourceTrackActiveParametersChanged>(nullptr))
      {

        return;
      }

      const monocle::RecordingJobSourceTrackActiveParametersChanged* recordingjobsourcetrackactiveparameterschanged = flatbuffers::GetRoot<monocle::RecordingJobSourceTrackActiveParametersChanged>(data);
      if (!recordingjobsourcetrackactiveparameterschanged)
      {

        return;
      }

      std::vector<std::string> activeparameters;
      if (recordingjobsourcetrackactiveparameterschanged->activeparameters())
      {
        activeparameters.reserve(recordingjobsourcetrackactiveparameterschanged->activeparameters()->size());
        for (const flatbuffers::String* activeparameter : *recordingjobsourcetrackactiveparameterschanged->activeparameters())
        {
          activeparameters.push_back(activeparameter->str());

        }
      }

      RecordingJobSourceTrackActiveParametersChanged(recordingjobsourcetrackactiveparameterschanged->recording(), recordingjobsourcetrackactiveparameterschanged->recordingjob(), recordingjobsourcetrackactiveparameterschanged->recordingjobsource(), recordingjobsourcetrackactiveparameterschanged->recordingjobsourcetrack(), activeparameters);
      break;
    }
    case Message::RECORDINGJOBSOURCETRACKSTATECHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingJobSourceTrackStateChanged>(nullptr))
      {

        return;
      }

      const monocle::RecordingJobSourceTrackStateChanged* recordingjobsourcetrackstatechanged = flatbuffers::GetRoot<monocle::RecordingJobSourceTrackStateChanged>(data);
      if (!recordingjobsourcetrackstatechanged)
      {

        return;
      }

      std::string error;
      if (recordingjobsourcetrackstatechanged->error())
      {
        error = recordingjobsourcetrackstatechanged->error()->str();

      }

      RecordingJobSourceTrackStateChanged(recordingjobsourcetrackstatechanged->recording(), recordingjobsourcetrackstatechanged->recordingjob(), recordingjobsourcetrackstatechanged->recordingjobsource(), recordingjobsourcetrackstatechanged->recordingjobsourcetrack(), recordingjobsourcetrackstatechanged->time(), recordingjobsourcetrackstatechanged->state(), error);
      break;
    }
    case Message::RECORDINGJOBLOGMESSAGE:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingJobLogMessage>(nullptr))
      {

        return;
      }

      const monocle::RecordingJobLogMessage* recordingjoblogmessage = flatbuffers::GetRoot<monocle::RecordingJobLogMessage>(data);
      if (!recordingjoblogmessage || (recordingjoblogmessage->message() == nullptr) || (recordingjoblogmessage->message()->message() == nullptr))
      {

        return;
      }

      RecordingJobLogMessage(recordingjoblogmessage->recordingtoken(), recordingjoblogmessage->token(), recordingjoblogmessage->message()->time(), recordingjoblogmessage->message()->severity(), recordingjoblogmessage->message()->message()->str());
      break;
    }
    case Message::RECORDINGLOGMESSAGE:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingLogMessage>(nullptr))
      {

        return;
      }

      const monocle::RecordingLogMessage* recordinglogmessage = flatbuffers::GetRoot<monocle::RecordingLogMessage>(data);
      if (!recordinglogmessage || (recordinglogmessage->message() == nullptr) || (recordinglogmessage->message()->message() == nullptr))
      {

        return;
      }

      RecordingLogMessage(recordinglogmessage->token(), recordinglogmessage->message()->time(), recordinglogmessage->message()->severity(), recordinglogmessage->message()->message()->str());
      break;
    }
    case Message::RECORDINGTRACKCODECADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingTrackCodecAdded>(nullptr))
      {

        return;
      }

      const monocle::RecordingTrackCodecAdded* recordingtrackcodecadded = flatbuffers::GetRoot<monocle::RecordingTrackCodecAdded>(data);
      if (!recordingtrackcodecadded)
      {

        return;
      }

      RecordingTrackCodecAdded(recordingtrackcodecadded->recordingtoken(), recordingtrackcodecadded->trackid(), recordingtrackcodecadded->id(), recordingtrackcodecadded->codec(), recordingtrackcodecadded->parameters() ? recordingtrackcodecadded->parameters()->str() : std::string(), recordingtrackcodecadded->timestamp());
      break;
    }
    case Message::RECORDINGTRACKCODECREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingTrackCodecRemoved>(nullptr))
      {

        return;
      }

      const monocle::RecordingTrackCodecRemoved* recordingtrackcodecremoved = flatbuffers::GetRoot<monocle::RecordingTrackCodecRemoved>(data);
      if (!recordingtrackcodecremoved)
      {

        return;
      }

      RecordingTrackCodecRemoved(recordingtrackcodecremoved->recordingtoken(), recordingtrackcodecremoved->trackid(), recordingtrackcodecremoved->id());
      break;
    }
    case Message::RECORDINGSTATISTICS:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingsStatistics>(nullptr))
      {

        return;
      }

      const monocle::RecordingsStatistics* recordingsstatistics = flatbuffers::GetRoot<monocle::RecordingsStatistics>(data);
      if (!recordingsstatistics)
      {

        return;
      }

      std::vector<RECORDINGSTATISTICS> recordingsstatisticstmp;
      if (recordingsstatistics->recordingsstatistics())
      {
        recordingsstatisticstmp.reserve(recordingsstatistics->recordingsstatistics()->size());
        for (const RecordingStatistics* recordingstatistics : *recordingsstatistics->recordingsstatistics())
        {
          std::vector<TRACKSTATISTICS> trackstatisticstmp;
          if (recordingstatistics->trackstatistics())
          {
            trackstatisticstmp.reserve(recordingstatistics->trackstatistics()->size());
            for (const TrackStatistics* trackstatistics : *recordingstatistics->trackstatistics())
            {
              trackstatisticstmp.push_back(TRACKSTATISTICS(trackstatistics->id(), trackstatistics->totaldatareceived()));

            }
          }
          recordingsstatisticstmp.push_back(RECORDINGSTATISTICS(recordingstatistics->recordingtoken(), trackstatisticstmp));
        }
      }

      RecordingsStatistics(recordingsstatistics->time(), recordingsstatisticstmp);
      break;
    }
    case Message::RECORDINGTRACKLOGMESSAGE:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::RecordingTrackLogMessage>(nullptr))
      {

        return;
      }

      const monocle::RecordingTrackLogMessage* recordingtracklogmessage = flatbuffers::GetRoot<monocle::RecordingTrackLogMessage>(data);
      if (!recordingtracklogmessage || (recordingtracklogmessage->message() == nullptr) || (recordingtracklogmessage->message()->message() == nullptr))
      {

        return;
      }

      RecordingTrackLogMessage(recordingtracklogmessage->recordingtoken(), recordingtracklogmessage->id(), recordingtracklogmessage->message()->time(), recordingtracklogmessage->message()->severity(), recordingtracklogmessage->message()->message()->str());
      break;
    }
    case Message::REMOVEFILE:
    {
      if (error)
      {
        HandleError(removefile_, sequence, data, datasize);
        return;
      }
      removefile_.Response(sequence, REMOVEFILERESPONSE());
      break;
    }
    case Message::REMOVEGROUP:
    {
      if (error)
      {
        HandleError(removegroup_, sequence, data, datasize);
        return;
      }
      removegroup_.Response(sequence, REMOVEGROUPRESPONSE());
      break;
    }
    case Message::REMOVEMAP:
    {
      if (error)
      {
        HandleError(removemap_, sequence, data, datasize);
        return;
      }
      removemap_.Response(sequence, REMOVEMAPRESPONSE());
      break;
    }
    case Message::REMOVEONVIFUSER:
    {
      if (error)
      {
        HandleError(removeonvifuser_, sequence, data, datasize);
        return;
      }
      removeonvifuser_.Response(sequence, REMOVEONVIFUSERRESPONSE());
      break;
    }
    case Message::REMOVERECEIVER:
    {
      if (error)
      {
        HandleError(removereceiver_, sequence, data, datasize);
        return;
      }
      removereceiver_.Response(sequence, REMOVERECEIVERRESPONSE());
      break;
    }
    case Message::REMOVERECORDING:
    {
      if (error)
      {
        HandleError(removerecording_, sequence, data, datasize);
        return;
      }
      removerecording_.Response(sequence, REMOVERECORDINGRESPONSE());
      break;
    }
    case Message::REMOVERECORDINGJOB:
    {
      if (error)
      {
        HandleError(removerecordingjob_, sequence, data, datasize);
        return;
      }
      removerecordingjob_.Response(sequence, REMOVERECORDINGJOBRESPONSE());
      break;
    }
    case Message::REMOVERECORDINGJOBSOURCE:
    {
      if (error)
      {
        HandleError(removerecordingjobsource_, sequence, data, datasize);
        return;
      }
      removerecordingjobsource_.Response(sequence, REMOVERECORDINGJOBSOURCERESPONSE());
      break;
    }
    case Message::REMOVETRACK:
    {
      if (error)
      {
        HandleError(removetrack_, sequence, data, datasize);
        return;
      }
      removetrack_.Response(sequence, REMOVETRACKRESPONSE());
      break;
    }
    case Message::REMOVETRACKS:
    {
      if (error)
      {
        HandleError(removetracks_, sequence, data, datasize);
        return;
      }
      removetracks_.Response(sequence, REMOVETRACKSRESPONSE());
      break;
    }
    case Message::REMOVEUSER:
    {
      if (error)
      {
        HandleError(removeuser_, sequence, data, datasize);
        return;
      }
      removeuser_.Response(sequence, REMOVEUSERRESPONSE());
      break;
    }
    case Message::SETLOCATION:
    {
      if (error)
      {
        HandleError(setlocation_, sequence, data, datasize);
        return;
      }
      setlocation_.Response(sequence, SETLOCATIONRESPONSE());
      break;
    }
    case Message::SETNAME:
    {
      if (error)
      {
        HandleError(setname_, sequence, data, datasize);
        return;
      }
      setname_.Response(sequence, SETNAMERESPONSE());
      break;
    }
    case Message::SERVERLOGMESSAGE:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::LogMessage>(nullptr))
      {

        return;
      }

      const monocle::LogMessage* logmessage = flatbuffers::GetRoot<monocle::LogMessage>(data);
      if (!logmessage || (logmessage->message() == nullptr))
      {

        return;
      }

      ServerLogMessage(logmessage->time(), logmessage->severity(), logmessage->message()->str());
      break;
    }
    case Message::SUBSCRIBE:
    {
      if (error)
      {
        HandleError(subscribe_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SubscribeResponse>(nullptr))
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(Error(ErrorCode::InvalidMessage, "SubscribeResponse verification failed")));
        return;
      }

      const SubscribeResponse* subscriberesponse = flatbuffers::GetRoot<SubscribeResponse>(data);
      if (!subscriberesponse || (subscriberesponse->name() == nullptr) || (subscriberesponse->publickey() == nullptr) || (subscriberesponse->architecture() == nullptr) || (subscriberesponse->compiler() == nullptr) || (subscriberesponse->version() == nullptr) || (subscriberesponse->environmentvariables() == nullptr) || (subscriberesponse->commandlinevariables() == nullptr))
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing Parameters")));
        return;
      }

      // Name
      std::string name;
      if (subscriberesponse->name())
      {
        name = subscriberesponse->name()->str();

      }

      // Public Key
      std::string publickey;
      if (subscriberesponse->publickey())
      {
        publickey = subscriberesponse->publickey()->str();

      }

      // Architecture
      std::string architecture;
      if (subscriberesponse->architecture())
      {
        architecture = subscriberesponse->architecture()->str();

      }

      // Compiler
      std::string compiler;
      if (subscriberesponse->compiler())
      {
        compiler = subscriberesponse->compiler()->str();

      }

      // Database Path
      std::string databasepath;
      if (subscriberesponse->databasepath())
      {
        databasepath = subscriberesponse->databasepath()->str();

      }

      // Environmental variables
      std::vector<std::string> environmentvariables;
      for (const flatbuffers::String* environmentvariable : *subscriberesponse->environmentvariables())
      {
        if (environmentvariable == nullptr)
        {

          continue;
        }
        environmentvariables.push_back(environmentvariable->str());
      }

      // Command line variables
      std::vector<std::string> commandlinevariables;
      for (const flatbuffers::String* commandlinevariable : *subscriberesponse->commandlinevariables())
      {
        if (commandlinevariable == nullptr)
        {

          continue;
        }
        commandlinevariables.push_back(commandlinevariable->str());
      }

      // Version
      const utility::Version version(subscriberesponse->version()->major(), subscriberesponse->version()->minor(), subscriberesponse->version()->build());

      // ONVIF users
      if (subscriberesponse->onvifusers() == nullptr)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing ONVIF users")));
        return;
      }

      const std::pair< Error, std::vector<ONVIFUSER> > onvifusers = GetONVIFUsersBuffer(*subscriberesponse->onvifusers());
      if (onvifusers.first.code_ != ErrorCode::Success)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(onvifusers.first));
        return;
      }

      // Groups
      if (subscriberesponse->groups() == nullptr)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing Groups")));
        return;
      }

      const std::pair< Error, std::vector<GROUP> > groups = GetGroupsBuffer(*subscriberesponse->groups());
      if (groups.first.code_ != ErrorCode::Success)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(groups.first));
        return;
      }

      // Users
      if (subscriberesponse->users() == nullptr)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing Users")));
        return;
      }

      const std::pair< Error, std::vector<USER> > users = GetUsersBuffer(*subscriberesponse->users());
      if (users.first.code_ != ErrorCode::Success)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(users.first));
        return;
      }

      // Files
      if (subscriberesponse->files() == nullptr)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing Files")));
        return;
      }

      const std::pair< Error, std::vector<FILE> > files = GetFilesBuffer(*subscriberesponse->files());
      if (files.first.code_ != ErrorCode::Success)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(files.first));
        return;
      }

      // Receivers
      if (subscriberesponse->receivers() == nullptr)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing Receivers")));
        return;
      }

      const std::pair< Error, std::vector<RECEIVER> > receivers = GetReceiversBuffer(*subscriberesponse->receivers());
      if (receivers.first.code_ != ErrorCode::Success)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(receivers.first));
        return;
      }

      // Recordings
      if (subscriberesponse->recordings() == nullptr)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing Recordings")));
        return;
      }

      const std::pair< Error, std::vector<RECORDING> > recordings = GetRecordingsBuffer(*subscriberesponse->recordings());
      if (recordings.first.code_ != ErrorCode::Success)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(recordings.first));
        return;
      }

      // Server log
      if (subscriberesponse->serverlogmessages() == nullptr)
      {
        subscribe_.Response(sequence, SUBSCRIBERESPONSE(Error(ErrorCode::MissingParameter, "SubscribeResponse missing server log messages")));
        return;
      }

      std::vector<LOGMESSAGE> serverlogmessages;
      serverlogmessages.reserve(subscriberesponse->serverlogmessages()->size());
      for (const monocle::LogMessage* logmessage : *subscriberesponse->serverlogmessages())
      {
        if (logmessage && logmessage->message())
        {
          serverlogmessages.push_back(LOGMESSAGE(logmessage->time(), logmessage->severity(), logmessage->message()->str()));

        }
      }

      // Maps
      std::vector<MAP> maps;
      if (subscriberesponse->maps())
      {
        maps.reserve(subscriberesponse->maps()->size());
        for (const monocle::Map* map : *subscriberesponse->maps())
        {
          if (map && map->name() && map->location() && map->md5sum())
          {
            maps.push_back(MAP(map->token(), map->name()->str(), map->location()->str(), map->md5sum()->str()));

          }
        }
      }

      // Mount points
      std::vector<MOUNTPOINT> mountpoints;
      if (subscriberesponse->mountpoints())
      {
        mountpoints.reserve(subscriberesponse->mountpoints()->size());
        for (const monocle::MountPoint* mountpoint : *subscriberesponse->mountpoints())
        {
          std::string path;
          if (mountpoint->path())
          {
            path = mountpoint->path()->str();

          }

          std::string type;
          if (mountpoint->type())
          {
            type = mountpoint->type()->str();

          }

          std::string source;
          if (mountpoint->source())
          {
            source = mountpoint->source()->str();

          }

          mountpoints.push_back(MOUNTPOINT(mountpoint->id(), mountpoint->parentid(), mountpoint->majorstdev(), mountpoint->minorstdev(), path, type, source));
        }
      }

      std::string latitude;
      std::string longitude;
      if (subscriberesponse->latitude() && subscriberesponse->longitude())
      {
        latitude = subscriberesponse->latitude()->str();
        longitude = subscriberesponse->longitude()->str();
      }

      subscribe_.Response(sequence, SUBSCRIBERESPONSE(name, publickey, architecture, subscriberesponse->operatingsystem(), compiler, databasepath, version, subscriberesponse->identifier(), environmentvariables, commandlinevariables, onvifusers.second, groups.second, users.second, files.second, receivers.second, recordings.second, serverlogmessages, subscriberesponse->maxrecordings(), maps, mountpoints, latitude, longitude, subscriberesponse->numcudadevices(), subscriberesponse->numcldevices(), subscriberesponse->maxobjectdetectors()));
      break;
    }
    case Message::SUBSCRIBEDISCOVERY:
    {
      if (error)
      {
        HandleError(subscribediscovery_, sequence, data, datasize);
        return;
      }
      subscribediscovery_.Response(sequence, SUBSCRIBEDISCOVERYRESPONSE());
      break;
    }
    case Message::SUBSCRIBEHARDWARESTATS:
    {
      if (error)
      {
        HandleError(subscribehardwarestats_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SubscribeHardwareStatsResponse>(nullptr))
      {
        subscribehardwarestats_.Response(sequence, SUBSCRIBEHARDWARESTATSRESPONSE(Error(ErrorCode::InvalidMessage, "SubscribeHardwareStatsResponse verification failed")));
        return;
      }

      const SubscribeHardwareStatsResponse* subscribehardwarestatsresponse = flatbuffers::GetRoot<SubscribeHardwareStatsResponse>(data);
      if (!subscribehardwarestatsresponse || (subscribehardwarestatsresponse->currenthardwarestats() == nullptr))
      {
        subscribehardwarestats_.Response(sequence, SUBSCRIBEHARDWARESTATSRESPONSE(Error(ErrorCode::MissingParameter, "SubscribeHardwareStatsResponse missing message")));
        return;
      }

      std::vector<monocle::DISKSTAT> diskstats;
      if (subscribehardwarestatsresponse->currenthardwarestats()->diskstats())
      {
        diskstats.reserve(subscribehardwarestatsresponse->currenthardwarestats()->diskstats()->size());
        for (const monocle::DiskStat* diskstat : *subscribehardwarestatsresponse->currenthardwarestats()->diskstats())
        {
          if (!diskstat->device())
          {
            // Something is very wrong, just ignore this message
            return;
          }
          diskstats.push_back(monocle::DISKSTAT(diskstat->device()->str(), diskstat->utility(), diskstat->readspeed(), diskstat->writespeed()));
        }
      }

      std::vector<monocle::GPUSTAT> gpustats;
      if (subscribehardwarestatsresponse->currenthardwarestats()->gpustats())
      {
        gpustats.reserve(subscribehardwarestatsresponse->currenthardwarestats()->diskstats()->size());
        for (const monocle::GPUStat* gpustat : *subscribehardwarestatsresponse->currenthardwarestats()->gpustats())
        {
          if (!gpustat->uuid())
          {
            // Something is very wrong, just ignore this message
            return;
          }
          gpustats.push_back(monocle::GPUSTAT(gpustat->uuid()->str(), gpustat->name()->str(), gpustat->gpuusage(), gpustat->memoryusage(), gpustat->totalmemory(), gpustat->usedmemory()));
        }
      }

      subscribehardwarestats_.Response(sequence, SUBSCRIBEHARDWARESTATSRESPONSE(HARDWARESTATS(subscribehardwarestatsresponse->currenthardwarestats()->time(), diskstats, subscribehardwarestatsresponse->currenthardwarestats()->cpuusage(), subscribehardwarestatsresponse->currenthardwarestats()->totalmemory(), subscribehardwarestatsresponse->currenthardwarestats()->availablememory(), gpustats)));
      break;
    }
    case Message::SUBSCRIBERECORDINGJOBLOG:
    {
      if (error)
      {
        HandleError(subscriberecordingjoblog_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SubscribeRecordingJobLogResponse>(nullptr))
      {
        subscriberecordingjoblog_.Response(sequence, SUBSCRIBERECORDINGJOBLOGRESPONSE(Error(ErrorCode::InvalidMessage, "SubscribeRecordingJobLogResponse verification failed")));
        return;
      }

      // Messages
      const SubscribeRecordingJobLogResponse* subscriberecordingjoblogresponse = flatbuffers::GetRoot<SubscribeRecordingJobLogResponse>(data);
      if (!subscriberecordingjoblogresponse || (subscriberecordingjoblogresponse->recordingjoblogmessages() == nullptr))
      {
        subscriberecordingjoblog_.Response(sequence, SUBSCRIBERECORDINGJOBLOGRESPONSE(Error(ErrorCode::MissingParameter, "SubscribeRecordingJobLogResponse missing message")));
        return;
      }

      std::vector< std::pair< uint64_t, std::vector<LOGMESSAGE> > > recordingjobslogmessages;
      recordingjobslogmessages.reserve(subscriberecordingjoblogresponse->recordingjoblogmessages()->size());
      for (const monocle::RecordingJobLogMessages* recordingjoblogmessages : *subscriberecordingjoblogresponse->recordingjoblogmessages())
      {
        std::vector<LOGMESSAGE> messages;
        if (recordingjoblogmessages->messages())
        {
          messages.reserve(recordingjoblogmessages->messages()->size());
          for (const monocle::LogMessage* logmessage : *recordingjoblogmessages->messages())
          {
            messages.push_back(LOGMESSAGE(logmessage->time(), logmessage->severity(), logmessage->message()->str()));

          }
        }
        recordingjobslogmessages.push_back(std::make_pair(recordingjoblogmessages->token(), messages));
      }

      subscriberecordingjoblog_.Response(sequence, SUBSCRIBERECORDINGJOBLOGRESPONSE(recordingjobslogmessages));
      break;
    }
    case Message::SUBSCRIBERECORDINGJOBSOURCETRACKLOG:
    {
      if (error)
      {
        HandleError(subscriberecordingjobsourcetracklog_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SubscribeRecordingJobSourceTrackLogResponse>(nullptr))
      {
        subscriberecordingjobsourcetracklog_.Response(sequence, SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(Error(ErrorCode::InvalidMessage, "SubscribeRecordingJobSourceTrackLogResponse verification failed")));
        return;
      }

      // Messages
      const SubscribeRecordingJobSourceTrackLogResponse* subscriberecordingjobsourcetracklogresponse = flatbuffers::GetRoot<SubscribeRecordingJobSourceTrackLogResponse>(data);
      if (!subscriberecordingjobsourcetracklogresponse || (subscriberecordingjobsourcetracklogresponse->recordingjobsourcetracklogmessages() == nullptr))
      {
        subscriberecordingjobsourcetracklog_.Response(sequence, SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(Error(ErrorCode::MissingParameter, "SubscribeRecordingJobSourceTrackLogResponse missing message")));
        return;
      }

      std::vector< std::pair< SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE::RECORDINGJOBSOURCETRACKTOKEN, std::vector<LOGMESSAGE> > > recordingjobslogmessages;
      recordingjobslogmessages.reserve(subscriberecordingjobsourcetracklogresponse->recordingjobsourcetracklogmessages()->size());
      for (const monocle::RecordingJobSourceTrackLogMessages* recordingjobsourcetracklogmessages : *subscriberecordingjobsourcetracklogresponse->recordingjobsourcetracklogmessages())
      {
        std::vector<LOGMESSAGE> messages;
        if (recordingjobsourcetracklogmessages->messages())
        {
          messages.reserve(recordingjobsourcetracklogmessages->messages()->size());
          for (const monocle::LogMessage* logmessage : *recordingjobsourcetracklogmessages->messages())
          {
            messages.push_back(LOGMESSAGE(logmessage->time(), logmessage->severity(), logmessage->message()->str()));

          }
        }
        recordingjobslogmessages.push_back(std::make_pair(SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE::RECORDINGJOBSOURCETRACKTOKEN(recordingjobsourcetracklogmessages->recordingjobtoken(), recordingjobsourcetracklogmessages->recordingjobsourcetoken(), recordingjobsourcetracklogmessages->token()), messages));
      }

      subscriberecordingjobsourcetracklog_.Response(sequence, SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(recordingjobslogmessages));
      break;
    }
    case Message::SUBSCRIBERECORDINGLOG:
    {
      if (error)
      {
        HandleError(subscriberecordinglog_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SubscribeRecordingLogResponse>(nullptr))
      {
        subscriberecordinglog_.Response(sequence, SUBSCRIBERECORDINGLOGRESPONSE(Error(ErrorCode::InvalidMessage, "SubscribeRecordingLogResponse verification failed")));
        return;
      }

      // Messages
      const SubscribeRecordingLogResponse* subscribeserverlogresponse = flatbuffers::GetRoot<SubscribeRecordingLogResponse>(data);
      if (!subscribeserverlogresponse || (subscribeserverlogresponse->messages() == nullptr))
      {
        subscriberecordinglog_.Response(sequence, SUBSCRIBERECORDINGLOGRESPONSE(Error(ErrorCode::MissingParameter, "SubscribeRecordingLogResponse missing message")));
        return;
      }

      std::vector<LOGMESSAGE> messages;
      messages.reserve(subscribeserverlogresponse->messages()->size());
      for (const monocle::LogMessage* logmessage : *subscribeserverlogresponse->messages())
      {
        if (logmessage->message())
        {
          messages.push_back(LOGMESSAGE(logmessage->time(), logmessage->severity(), logmessage->message()->str()));

        }
      }

      subscriberecordinglog_.Response(sequence, SUBSCRIBERECORDINGLOGRESPONSE(messages));
      break;
    }
    case Message::SUBSCRIBERECORDINGTRACKLOG:
    {
      if (error)
      {
        HandleError(subscriberecordingtracklog_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SubscribeRecordingTrackLogResponse>(nullptr))
      {
        subscriberecordingtracklog_.Response(sequence, SUBSCRIBERECORDINGTRACKLOGRESPONSE(Error(ErrorCode::InvalidMessage, "SubscribeRecordingTrackLogResponse verification failed")));
        return;
      }

      // Messages
      const SubscribeRecordingTrackLogResponse* subscriberecordingtracklogresponse = flatbuffers::GetRoot<SubscribeRecordingTrackLogResponse>(data);
      if (!subscriberecordingtracklogresponse || (subscriberecordingtracklogresponse->recordingtracklogmessages() == nullptr))
      {
        subscriberecordingtracklog_.Response(sequence, SUBSCRIBERECORDINGTRACKLOGRESPONSE(Error(ErrorCode::MissingParameter, "SubscribeRecordingTrackLogResponse missing message")));
        return;
      }

      std::vector< std::pair< uint32_t, std::vector<LOGMESSAGE> > > recordingtrackslogmessages;
      recordingtrackslogmessages.reserve(subscriberecordingtracklogresponse->recordingtracklogmessages()->size());
      for (const monocle::RecordingTrackLogMessages* recordingtracklogmessages : *subscriberecordingtracklogresponse->recordingtracklogmessages())
      {
        std::vector<LOGMESSAGE> messages;
        if (recordingtracklogmessages->messages())
        {
          messages.reserve(recordingtracklogmessages->messages()->size());
          for (const monocle::LogMessage* logmessage : *recordingtracklogmessages->messages())
          {
            messages.push_back(LOGMESSAGE(logmessage->time(), logmessage->severity(), logmessage->message()->str()));

          }
        }
        recordingtrackslogmessages.push_back(std::make_pair(recordingtracklogmessages->token(), messages));
      }

      subscriberecordingtracklog_.Response(sequence, SUBSCRIBERECORDINGTRACKLOGRESPONSE(recordingtrackslogmessages));
      break;
    }
    case Message::TRACKADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::TrackAdded>(nullptr))
      {

        return;
      }

      const monocle::TrackAdded* trackadded = flatbuffers::GetRoot<monocle::TrackAdded>(data);
      if (!trackadded)
      {

        return;
      }

      std::string token;
      if (trackadded->token())
      {
        token = trackadded->token()->str();

      }

      std::string description;
      if (trackadded->description())
      {
        description = trackadded->description()->str();

      }

      std::vector<uint64_t> files;
      if (trackadded->files())
      {
        files.reserve(trackadded->files()->size());
        for (const uint64_t file : *trackadded->files())
        {
          files.push_back(file);

        }
      }

      std::vector<monocle::CODECINDEX> codecindices;
      if (trackadded->codecindices())
      {
        codecindices.reserve(trackadded->codecindices()->size());
        for (const CodecIndex* codecindex : *trackadded->codecindices())
        {
          codecindices.push_back(monocle::CODECINDEX(codecindex->id(), codecindex->codec(), codecindex->parameters() ? codecindex->parameters()->str() : std::string(), codecindex->timestamp()));

        }
      }

      TrackAdded(trackadded->recordingtoken(), trackadded->id(), token, trackadded->tracktype(), description, trackadded->fixedfiles(), trackadded->digitalsigning(), trackadded->encrypt(), trackadded->flushfrequency(), files, codecindices, std::make_pair(trackadded->totaltrackdatatime(), trackadded->totaltrackdata()));
      break;
    }
    case Message::TRACKCHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::TrackChanged>(nullptr))
      {

        return;
      }

      const monocle::TrackChanged* trackchanged = flatbuffers::GetRoot<monocle::TrackChanged>(data);
      if (!trackchanged)
      {

        return;
      }

      std::string token;
      if (trackchanged->token())
      {
        token = trackchanged->token()->str();

      }

      std::string description;
      if (trackchanged->description())
      {
        description = trackchanged->description()->str();

      }

      std::vector<uint64_t> files;
      if (trackchanged->files())
      {
        files.reserve(trackchanged->files()->size());
        for (const uint64_t file : *trackchanged->files())
        {
          files.push_back(file);

        }
      }

      std::vector<monocle::CODECINDEX> codecindices;
      if (trackchanged->codecindices())
      {
        codecindices.reserve(trackchanged->codecindices()->size());
        for (const CodecIndex* codecindex : *trackchanged->codecindices())
        {
          codecindices.push_back(monocle::CODECINDEX(codecindex->id(), codecindex->codec(), codecindex->parameters() ? codecindex->parameters()->str() : std::string(), codecindex->timestamp()));

        }
      }

      TrackChanged(trackchanged->recordingtoken(), trackchanged->id(), token, trackchanged->tracktype(), description, trackchanged->fixedfiles(), trackchanged->digitalsigning(), trackchanged->encrypt(), trackchanged->flushfrequency(), files, codecindices, std::make_pair(trackchanged->totaltrackdatatime(), trackchanged->totaltrackdata()));
      break;
    }
    case Message::TRACKREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::TrackRemoved>(nullptr))
      {

        return;
      }

      const monocle::TrackRemoved* trackremoved = flatbuffers::GetRoot<monocle::TrackRemoved>(data);
      if (!trackremoved)
      {

        return;
      }

      TrackRemoved(trackremoved->recordingtoken(), trackremoved->id());
      break;
    }
    case Message::UNMOUNTFILE:
    {
      if (error)
      {
        HandleError(unmountfile_, sequence, data, datasize);
        return;
      }

      unmountfile_.Response(sequence, UNMOUNTFILERESPONSE());
      break;
    }
    case Message::UNSUBSCRIBEDISCOVERY:
    {
      if (error)
      {
        HandleError(unsubscribediscovery_, sequence, data, datasize);
        return;
      }

      unsubscribediscovery_.Response(sequence, UNSUBSCRIBEDISCOVERYRESPONSE());
    }
    case Message::UNSUBSCRIBEHARDWARESTATS:
    {
      if (error)
      {
        HandleError(unsubscribehardwarestats_, sequence, data, datasize);
        return;
      }

      unsubscribehardwarestats_.Response(sequence, UNSUBSCRIBEHARDWARESTATSRESPONSE());
    }
    case Message::UNSUBSCRIBERECORDINGJOBLOG:
    {
      if (error)
      {
        HandleError(unsubscriberecordingjoblog_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<UnsubscribeRecordingJobLogResponse>(nullptr))
      {
        unsubscriberecordingjoblog_.Response(sequence, UNSUBSCRIBERECORDINGJOBLOGRESPONSE(Error(ErrorCode::InvalidMessage, "UnsubscribeRecordingJobLogResponse verification failed")));
        return;
      }

      const UnsubscribeRecordingJobLogResponse* unsubscriberecordingjoblogresponse = flatbuffers::GetRoot<UnsubscribeRecordingJobLogResponse>(data);
      if (!unsubscriberecordingjoblogresponse)
      {
        unsubscriberecordingjoblog_.Response(sequence, UNSUBSCRIBERECORDINGJOBLOGRESPONSE(Error(ErrorCode::MissingParameter, "UnsubscribeRecordingJobLogResponse missing message")));
        return;
      }

      unsubscriberecordingjoblog_.Response(sequence, UNSUBSCRIBERECORDINGJOBLOGRESPONSE());
    }
    case Message::UNSUBSCRIBERECORDINGJOBSOURCETRACKLOG:
    {
      if (error)
      {
        HandleError(unsubscriberecordingjobsourcetracklog_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<UnsubscribeRecordingJobLogResponse>(nullptr))
      {
        unsubscriberecordingjobsourcetracklog_.Response(sequence, UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(Error(ErrorCode::InvalidMessage, "UnsubscribeRecordingJobSourceTrackLogResponse verification failed")));
        return;
      }

      const UnsubscribeRecordingJobLogResponse* unsubscriberecordingjoblogresponse = flatbuffers::GetRoot<UnsubscribeRecordingJobLogResponse>(data);
      if (!unsubscriberecordingjoblogresponse)
      {
        unsubscriberecordingjobsourcetracklog_.Response(sequence, UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(Error(ErrorCode::MissingParameter, "UnsubscribeRecordingJobSourceTrackLogResponse missing message")));
        return;
      }

      unsubscriberecordingjobsourcetracklog_.Response(sequence, UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE());
    }
    case Message::UNSUBSCRIBERECORDINGLOG:
    {
      if (error)
      {
        HandleError(unsubscriberecordinglog_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<UnsubscribeRecordingLogResponse>(nullptr))
      {
        unsubscriberecordinglog_.Response(sequence, UNSUBSCRIBERECORDINGLOGRESPONSE(Error(ErrorCode::InvalidMessage, "UnsubscribeRecordingLogResponse verification failed")));
        return;
      }

      const UnsubscribeRecordingLogResponse* unsubscribeserverlogresponse = flatbuffers::GetRoot<UnsubscribeRecordingLogResponse>(data);
      if (!unsubscribeserverlogresponse)
      {
        unsubscriberecordinglog_.Response(sequence, UNSUBSCRIBERECORDINGLOGRESPONSE(Error(ErrorCode::MissingParameter, "UnsubscribeRecordingLogResponse missing message")));
        return;
      }

      unsubscriberecordinglog_.Response(sequence, UNSUBSCRIBERECORDINGLOGRESPONSE());
    }
    case Message::UNSUBSCRIBERECORDINGTRACKLOG:
    {
      if (error)
      {
        HandleError(unsubscriberecordingtracklog_, sequence, data, datasize);
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<UnsubscribeRecordingTrackLogResponse>(nullptr))
      {
        unsubscriberecordingtracklog_.Response(sequence, UNSUBSCRIBERECORDINGTRACKLOGRESPONSE(Error(ErrorCode::InvalidMessage, "UnsubscribeRecordingTrackLogResponse verification failed")));
        return;
      }

      const UnsubscribeRecordingTrackLogResponse* unsubscriberecordingtracklogresponse = flatbuffers::GetRoot<UnsubscribeRecordingTrackLogResponse>(data);
      if (!unsubscriberecordingtracklogresponse)
      {
        unsubscriberecordingtracklog_.Response(sequence, UNSUBSCRIBERECORDINGTRACKLOGRESPONSE(Error(ErrorCode::MissingParameter, "UnsubscribeRecordingTrackLogResponse  missing message")));
        return;
      }

      unsubscriberecordingtracklog_.Response(sequence, UNSUBSCRIBERECORDINGTRACKLOGRESPONSE());
    }
    case Message::TRACKSETDATA:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::TrackSetData>(nullptr))
      {

        return;
      }

      const monocle::TrackSetData* tracksetdata = flatbuffers::GetRoot<monocle::TrackSetData>(data);
      if (!tracksetdata || (tracksetdata->indices() == nullptr))
      {

        return;
      }

      std::vector<monocle::INDEX> indices;
      indices.reserve(tracksetdata->indices()->size());
      for (const monocle::INDEX* index : *tracksetdata->indices())
      {
        indices.push_back(*index);

      }

      TrackSetData(tracksetdata->recording(), tracksetdata->track(), indices);
      break;
    }
    case Message::TRACKDELETEDATA:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::TrackDeleteData>(nullptr))
      {

        return;
      }

      const monocle::TrackDeleteData* trackdeletedata = flatbuffers::GetRoot<monocle::TrackDeleteData>(data);
      if (!trackdeletedata)
      {

        return;
      }

      TrackDeleteData(trackdeletedata->recording(), trackdeletedata->track(), trackdeletedata->start() == 0 ? boost::none : boost::optional<uint64_t>(trackdeletedata->start()), trackdeletedata->end() == 0 ? boost::none : boost::optional<uint64_t>(trackdeletedata->end()));
      break;
    }
    case Message::USERADDED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::UserAdded>(nullptr))
      {

        return;
      }

      const monocle::UserAdded* useradded = flatbuffers::GetRoot<monocle::UserAdded>(data);
      if (!useradded || (useradded->username() == nullptr))
      {

        return;
      }

      UserAdded(useradded->token(), useradded->username()->str(), useradded->group());
      break;
    }
    case Message::USERCHANGED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::UserChanged>(nullptr))
      {

        return;
      }

      const monocle::UserChanged* userchanged = flatbuffers::GetRoot<monocle::UserChanged>(data);
      if (!userchanged)
      {

        return;
      }

      UserChanged(userchanged->token(), userchanged->group());
      break;
    }
    case Message::USERREMOVED:
    {
      if (error)
      {
        // Ignore this because it can't really happen...
        return;
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<monocle::UserRemoved>(nullptr))
      {

        return;
      }

      const monocle::UserRemoved* userremoved = flatbuffers::GetRoot<monocle::UserRemoved>(data);
      if (!userremoved)
      {

        return;
      }

      UserRemoved(userremoved->token());
      break;
    }
    default:
    {
      // Just ignore it

    }
  }
}

std::pair< Error, std::vector<ONVIFUSER> > Client::GetONVIFUsersBuffer(const flatbuffers::Vector< flatbuffers::Offset<ONVIFUser> >& onvifuserbuffers) const
{
  std::vector<ONVIFUSER> onvifusers;
  onvifusers.reserve(onvifuserbuffers.size());
  for (const auto& onvifuser : onvifuserbuffers)
  {
    if (!onvifuser->token() || !onvifuser->username())
    {

      return std::make_pair(Error(ErrorCode::InvalidMessage, "GetONVIFUsersResponse invalid user"), std::vector<ONVIFUSER>());
    }

    onvifusers.push_back(ONVIFUSER(onvifuser->token(), onvifuser->username()->str(), onvifuser->userlevel()));
  }
  return std::make_pair(Error(), onvifusers);
}

std::pair< Error, std::vector<GROUP> > Client::GetGroupsBuffer(const flatbuffers::Vector< flatbuffers::Offset<Group> >& groupbuffers) const
{
  std::vector<GROUP> groups;
  groups.reserve(groupbuffers.size());
  for (const auto& group : groupbuffers)
  {
    if ((group->name() == nullptr) || !group->recordings())
    {

      return std::make_pair(Error(ErrorCode::InvalidMessage, "GetGroupsResponse invalid group"), std::vector<GROUP>());
    }

    std::vector<uint64_t> recordings;
    for (const uint64_t recording : *group->recordings())
    {
      recordings.push_back(recording);

    }

    groups.push_back(GROUP(group->token(), group->name()->str(), group->manageusers(), group->managerecordings(), group->managemaps(), group->managedevice(), group->allrecordings(), recordings));
  }
  return std::make_pair(Error(), groups);
}

std::pair< Error, std::vector<USER> > Client::GetUsersBuffer(const flatbuffers::Vector< flatbuffers::Offset<User> >& userbuffers) const
{
  std::vector<USER> users;
  users.reserve(userbuffers.size());
  for (const auto& user : userbuffers)
  {
    if (!user->username())
    {

      return std::make_pair(Error(ErrorCode::InvalidMessage, "GetUsersResponse invalid user"), std::vector<USER>());
    }

    users.push_back(USER(user->token(), user->username()->str(), user->grouptoken()));
  }
  return std::make_pair(Error(), users);
}

std::pair< Error, std::vector<FILE> > Client::GetFilesBuffer(const flatbuffers::Vector< flatbuffers::Offset<File> >& filebuffers) const
{
  std::vector<FILE> files;
  files.reserve(filebuffers.size());
  for (const auto& file : filebuffers)
  {
    if (!file->path() || !file->mountpoint())
    {

      return std::make_pair(Error(ErrorCode::InvalidMessage, "GetFilesResponse invalid file"), std::vector<FILE>());
    }
    files.push_back(FILE(file->token(), file->path()->str(), file->mountpoint()->str(), file->numchunks(), file->chunksize(), file->automount(), file->state(), file->monitorstate()));
  }
  return std::make_pair(Error(), files);
}

std::pair< Error, std::vector<RECEIVER> > Client::GetReceiversBuffer(const flatbuffers::Vector< flatbuffers::Offset<Receiver> >& receiverbuffers) const
{
  std::vector<RECEIVER> receivers;
  receivers.reserve(receiverbuffers.size());
  for (const Receiver* receiver : receiverbuffers)
  {
    if (!receiver->mediauri())
    {

      return std::make_pair(Error(ErrorCode::InvalidMessage, "GetReceiversResponse invalid receiver"), std::vector<RECEIVER>());
    }

    std::string username;
    if (receiver->username())
    {
      username = receiver->username()->str();

    }

    std::string password;
    if (receiver->password())
    {
      password = receiver->password()->str();

    }

    std::vector<std::string> parameters;
    if (receiver->parameters())
    {
      parameters.reserve(receiver->parameters()->size());
      for (const flatbuffers::String* parameter : *receiver->parameters())
      {
        parameters.push_back(parameter->str());

      }
    }

    receivers.push_back(RECEIVER(receiver->token(), receiver->mode(), receiver->mediauri()->str(), receiver->autocreated(), username, password, parameters, receiver->state()));
  }
  return std::make_pair(Error(), receivers);
}

std::pair< Error, std::vector<RECORDING> > Client::GetRecordingsBuffer(const flatbuffers::Vector< flatbuffers::Offset<Recording> >& recordingbuffers) const
{
  std::vector<RECORDING> recordings;
  recordings.reserve(recordingbuffers.size());
  for (const Recording* recording : recordingbuffers)
  {
    if (!recording->sourceid() || !recording->name() || !recording->location() || !recording->description() || !recording->content() || !recording->recordingjobs() || !recording->tracks())
    {

      return std::make_pair(Error(ErrorCode::InvalidMessage, "GetRecordingsResponse invalid Recording"), std::vector<RECORDING>());
    }

    std::vector<RECORDINGJOB> jobs;
    jobs.reserve(recording->recordingjobs()->size());
    for (const auto& job : *recording->recordingjobs())
    {
      std::string jobname;
      if (job->name())
      {
        jobname = job->name()->str();

      }

      std::vector<RECORDINGJOBSOURCE> sources;
      if (job->recordingjobsources())
      {
        sources.reserve(job->recordingjobsources()->size());
        for (const auto& source : *job->recordingjobsources())
        {
          std::vector<RECORDINGJOBSOURCETRACK> tracks;
          if (source->recordingjobsourcetracks())
          {
            tracks.reserve(source->recordingjobsourcetracks()->size());
            for (const auto& track : *source->recordingjobsourcetracks())
            {
              std::vector<std::string> parameters;
              if (track->parameters())
              {
                parameters.reserve(track->parameters()->size());
                for (const flatbuffers::String* parameter : *track->parameters())
                {
                  parameters.push_back(parameter->str());

                }
              }

              std::string error;
              if (track->error())
              {
                error = track->error()->str();

              }

              std::vector<std::string> activeparameters;
              if (track->activeparameters())
              {
                activeparameters.reserve(track->activeparameters()->size());
                for (const flatbuffers::String* activeparameter : *track->activeparameters())
                {
                  activeparameters.push_back(activeparameter->str());

                }
              }

              tracks.push_back(RECORDINGJOBSOURCETRACK(track->token(), track->recordingtrack(), parameters, track->state(), error, activeparameters));
            }
          }
          sources.push_back(RECORDINGJOBSOURCE(source->token(), source->type(), source->receiver(), tracks));
        }
      }
      jobs.push_back(RECORDINGJOB(job->token(), jobname, job->enabled(), job->priority(), sources));
    }
    
    std::vector<RECORDINGTRACK> tracks;
    tracks.reserve(recording->tracks()->size());
    for (const monocle::RecordingTrack* track : *recording->tracks())
    {
      if (!track->description() || !track->files() || !track->indices())
      {

        return std::make_pair(Error(ErrorCode::InvalidMessage, "GetRecordingsResponse invalid RecordingTrack(video)"), std::vector<RECORDING>());
      }
      tracks.push_back(RECORDINGTRACK(track->id(), track->token()->str(), track->tracktype(), track->description()->str(), track->fixedfiles(), track->digitalsigning(), track->encrypt(), track->flushfrequency(), ToVector(*track->files()), ToVector(*track->indices()), ToVector(track->codecindices()), std::make_pair(track->totaltrackdatatime(), track->totaltrackdata())));
    }

    recordings.push_back(RECORDING(recording->token(), recording->sourceid()->str(), recording->name()->str(), recording->location()->str(), recording->description()->str(), recording->address()->str(), recording->content()->str(), recording->retentiontime(), jobs, tracks, recording->activejob() ? boost::optional<uint64_t>(recording->activejob()->token()) : boost::none));
  }
  return std::make_pair(Error(), recordings);
}

std::vector<monocle::INDEX> Client::ToVector(const flatbuffers::Vector<const monocle::INDEX*>& indices) const
{
  std::vector<INDEX> result;
  result.reserve(indices.size());
  for (const monocle::INDEX* index : indices)
  {
    result.push_back(*index);

  }
  return result;
}

std::vector<monocle::CODECINDEX> Client::ToVector(const flatbuffers::Vector< flatbuffers::Offset<monocle::CodecIndex> >* codecindices) const
{
  std::vector<CODECINDEX> result;
  if (codecindices)
  {
    result.reserve(codecindices->size());
    for (const monocle::CodecIndex* codecindex : *codecindices)
    {
      result.push_back(CODECINDEX(codecindex->id(), codecindex->codec(), codecindex->parameters() ? codecindex->parameters()->str() : std::string(), codecindex->timestamp()));

    }
  }
  return result;
}

}

}
