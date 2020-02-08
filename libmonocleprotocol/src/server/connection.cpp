// connection.hpp
//

///// Includes /////

#include "monocleprotocol/server/connection.hpp"

#include <boost/bind.hpp>
#include <network/uri.hpp>
#include <utility/utility.hpp>

#include "monocleprotocol/addfilerequest_generated.h"
#include "monocleprotocol/addgrouprequest_generated.h"
#include "monocleprotocol/addmaprequest_generated.h"
#include "monocleprotocol/addonvifuserrequest_generated.h"
#include "monocleprotocol/addreceiverrequest_generated.h"
#include "monocleprotocol/addrecordingjobrequest_generated.h"
#include "monocleprotocol/addrecordingrequest_generated.h"
#include "monocleprotocol/addrecordingresponse_generated.h"
#include "monocleprotocol/addtrackrequest_generated.h"
#include "monocleprotocol/addtrackrequest2_generated.h"
#include "monocleprotocol/addtrackresponse_generated.h"
#include "monocleprotocol/adduserrequest_generated.h"
#include "monocleprotocol/authenticaterequest_generated.h"
#include "monocleprotocol/authenticateresponse_generated.h"
#include "monocleprotocol/changegrouprequest_generated.h"
#include "monocleprotocol/changemaprequest_generated.h"
#include "monocleprotocol/changeonvifuserrequest_generated.h"
#include "monocleprotocol/changereceiverrequest_generated.h"
#include "monocleprotocol/changerecordingjobrequest_generated.h"
#include "monocleprotocol/changerecordingrequest_generated.h"
#include "monocleprotocol/changetrackrequest_generated.h"
#include "monocleprotocol/changetrackrequest2_generated.h"
#include "monocleprotocol/changeuserrequest_generated.h"
#include "monocleprotocol/controlstreamend_generated.h"
#include "monocleprotocol/controlstreamframesteprequest_generated.h"
#include "monocleprotocol/controlstreamliverequest_generated.h"
#include "monocleprotocol/controlstreampauserequest_generated.h"
#include "monocleprotocol/controlstreamplayrequest_generated.h"
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
#include "monocleprotocol/errorresponse_generated.h"
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
#include "monocleprotocol/getrecordingrequest_generated.h"
#include "monocleprotocol/getrecordingsresponse_generated.h"
#include "monocleprotocol/getsnapshotrequest_generated.h"
#include "monocleprotocol/getsnapshotresponse_generated.h"
#include "monocleprotocol/getstateresponse_generated.h"
#include "monocleprotocol/gettimeresponse_generated.h"
#include "monocleprotocol/groupadded_generated.h"
#include "monocleprotocol/groupchanged_generated.h"
#include "monocleprotocol/groupremoved_generated.h"
#include "monocleprotocol/h265frameheader_generated.h"
#include "monocleprotocol/h264frameheader_generated.h"
#include "monocleprotocol/hardwarestats_generated.h"
#include "monocleprotocol/jpegframeheader_generated.h"
#include "monocleprotocol/locationchanged_generated.h"
#include "monocleprotocol/mapadded_generated.h"
#include "monocleprotocol/mapchanged_generated.h"
#include "monocleprotocol/mapremoved_generated.h"
#include "monocleprotocol/metadataframeheader_generated.h"
#include "monocleprotocol/monocleprotocol.hpp"
#include "monocleprotocol/mountpointadded_generated.h"
#include "monocleprotocol/mountpointremoved_generated.h"
#include "monocleprotocol/mountfilerequest_generated.h"
#include "monocleprotocol/mpeg4frameheader_generated.h"
#include "monocleprotocol/namechanged_generated.h"
#include "monocleprotocol/newcodecindex_generated.h"
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
#include "monocleprotocol/recordingjobsourceadded_generated.h"
#include "monocleprotocol/recordingjobsourceremoved_generated.h"
#include "monocleprotocol/recordingjobsourcetrackactiveparameterschanged_generated.h"
#include "monocleprotocol/recordingjobsourcetrackadded_generated.h"
#include "monocleprotocol/recordingjobsourcetracklogmessage_generated.h"
#include "monocleprotocol/recordingjobsourcetrackremoved_generated.h"
#include "monocleprotocol/recordingjobsourcetrackstatechanged_generated.h"
#include "monocleprotocol/recordingjobremoved_generated.h"
#include "monocleprotocol/recordingjoblogmessage_generated.h"
#include "monocleprotocol/recordinglogmessage_generated.h"
#include "monocleprotocol/recordingremoved_generated.h"
#include "monocleprotocol/recordingtracklogmessage_generated.h"
#include "monocleprotocol/recordingtrackcodecadded_generated.h"
#include "monocleprotocol/recordingtrackcodecremoved_generated.h"
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
#include "monocleprotocol/server/connection.hpp"
#include "monocleprotocol/server/server.hpp"
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
#include "monocleprotocol/trackadded_generated.h"
#include "monocleprotocol/trackchanged_generated.h"
#include "monocleprotocol/trackdeletedata_generated.h"
#include "monocleprotocol/trackremoved_generated.h"
#include "monocleprotocol/tracksetdata_generated.h"
#include "monocleprotocol/unmountfilerequest_generated.h"
#include "monocleprotocol/unsubscriberecordingjoblogrequest_generated.h"
#include "monocleprotocol/unsubscriberecordingjobsourcetracklogrequest_generated.h"
#include "monocleprotocol/unsubscriberecordinglogrequest_generated.h"
#include "monocleprotocol/useradded_generated.h"
#include "monocleprotocol/userchanged_generated.h"
#include "monocleprotocol/userremoved_generated.h"

///// Namespaces /////

namespace monocle
{

namespace server
{

///// Methods /////
  
SNAPSHOT::SNAPSHOT() :
  time_(0)
{

}

SNAPSHOT::SNAPSHOT(const std::vector<uint8_t>& data, const uint64_t time) :
  data_(data),
  time_(time)
{

}

STREAM::STREAM() :
  token_(0)
{

}

STREAM::STREAM(const uint64_t token, const std::vector<CODECINDEX>& codecindices) :
  token_(token),
  codecindices_(codecindices)
{

}

Connection::Connection(boost::asio::ip::tcp::socket&& socket, const boost::shared_ptr<Server>& server) :
  server_(server),
  socket_(std::move(socket)),
  sequence_(0)
{
  InitCompression();

}

Connection::Connection(boost::asio::io_service& io, const boost::shared_ptr<Server>& server) :
  server_(server),
  socket_(io),
  sequence_(0)
{
  InitCompression();

}

Connection::~Connection()
{
  if (inflateEnd(&inflate_) != Z_OK)
  {
    // We make a fairly safe assumption that this will not fail

  }

  if (deflateEnd(&deflate_) != Z_OK)
  {
    // We make a fairly safe assumption that this will not fail

  }

  Destroy();

  server_.reset();
}

void Connection::Destroy()
{
  boost::system::error_code err;
  socket_.close(err);
}

boost::system::error_code Connection::Connected()
{
  boost::asio::async_read(socket_, buffer_, boost::asio::transfer_at_least(sizeof(HEADER)), boost::bind(&Connection::HandleReadHeader, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  return boost::system::error_code();
}

boost::system::error_code Connection::SendControlStreamEnd(const uint64_t stream, const uint64_t playrequest, const monocle::ErrorCode error)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateControlStreamEnd(fbb_, stream, playrequest, error));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::CONTROLSTREAMEND, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendDiscoveryHello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateDiscoveryHello(fbb_, fbb_.CreateVectorOfStrings(addresses), fbb_.CreateVectorOfStrings(scopes)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::DISCOVERYHELLO, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendFileAdded(const uint64_t token, const boost::filesystem::path& path, const boost::filesystem::path& mountpoint, const uint64_t numchunks, const uint64_t chunksize, const bool automount)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateFile(fbb_, token, fbb_.CreateString(path.string()), fbb_.CreateString(mountpoint.string()), numchunks, chunksize, automount));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::FILEADDED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendFileRemoved(const uint64_t token)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateFileRemoved(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::FILEREMOVED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendFileMonitorStateChanged(const uint64_t token, const monocle::FileMonitorState monitorstate)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateFileMonitorStateChanged(fbb_, token, monitorstate));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::FILEMONITORSTATECHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendFileStateChanged(const uint64_t token, const monocle::FileState state)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateFileStateChanged(fbb_, token, state));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::FILESTATECHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendFindMotionEnd(const uint64_t token, const uint64_t ret)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateFindMotionEnd(fbb_, token, ret));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::FINDMOTIONEND, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendFindMotionProgress(const uint64_t token, const float progress)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateFindMotionProgress(fbb_, token, progress));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::FINDMOTIONPROGRESS, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendFindMotionResult(const uint64_t token, const uint64_t start, const uint64_t end)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateFindMotionResult(fbb_, token, start, end));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::FINDMOTIONRESULT, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendFindObjectEnd(const uint64_t token, const uint64_t ret)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateFindObjectEnd(fbb_, token, ret));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::FINDOBJECTEND, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendFindObjectProgress(const uint64_t token, const float progress)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateFindObjectProgress(fbb_, token, progress));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::FINDOBJECTPROGRESS, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendFindObjectResult(const uint64_t token, const uint64_t start, const uint64_t end, const monocle::ObjectClass objectclass, const uint64_t id, const uint64_t largesttime, const float largestx, const float largesty, const float largestwidth, const float largestheight)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateFindObjectResult(fbb_, token, start, end, objectclass, id, largesttime, largestx, largesty, largestwidth, largestheight));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::FINDOBJECTRESULT, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendGoodbye()
{
  std::lock_guard<std::mutex> lock(writemutex_);
  const HEADER header(0, false, false, Message::GOODBYE, ++sequence_);
  boost::system::error_code err;
  boost::asio::write(socket_, boost::asio::buffer(&header, sizeof(header)), boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendGroupAdded(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateGroupAdded(fbb_, token, fbb_.CreateString(name), manageusers, managerecordings, allrecordings, fbb_.CreateVector(recordings), managemaps, managedevice));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::GROUPADDED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendGroupChanged(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateGroupChanged(fbb_, token, fbb_.CreateString(name), manageusers, managerecordings, allrecordings, fbb_.CreateVector(recordings), managemaps, managedevice));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::GROUPCHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendGroupRemoved(const uint64_t token)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateGroupRemoved(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::GROUPREMOVED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendH265Frame(const uint64_t stream, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const boost::optional<uint64_t>& sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateH265FrameHeader(fbb_, stream, playrequest, codecindex, marker, timestamp, sequencenum.is_initialized() ? *sequencenum : 0, progress, signature ? fbb_.CreateVector(signature, signaturesize) : flatbuffers::Offset< flatbuffers::Vector<uint8_t> >(), donlfield, fbb_.CreateVector(offsets, numoffsets)));
  const uint32_t frameheadersize = static_cast<uint32_t>(fbb_.GetSize());
  const uint32_t messagesize = sizeof(frameheadersize) + frameheadersize + static_cast<uint32_t>(size);
  const HEADER header(messagesize, false, false, Message::H265FRAME, ++sequence_);

  boost::system::error_code err;
  const std::array<boost::asio::const_buffer, 4> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(&frameheadersize, sizeof(frameheadersize)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), frameheadersize),
    boost::asio::const_buffer(data, size)
  };
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendH264Frame(const uint64_t stream, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const boost::optional<uint64_t>& sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateH264FrameHeader(fbb_, stream, playrequest, codecindex, marker, timestamp, sequencenum.is_initialized() ? *sequencenum : 0, progress, signature ? fbb_.CreateVector(signature, signaturesize) : flatbuffers::Offset< flatbuffers::Vector<uint8_t> >(), fbb_.CreateVector(offsets, numoffsets)));
  const uint32_t frameheadersize = static_cast<uint32_t>(fbb_.GetSize());
  const uint32_t messagesize = sizeof(frameheadersize) + frameheadersize + static_cast<uint32_t>(size);
  const HEADER header(messagesize, false, false, Message::H264FRAME, ++sequence_);

  boost::system::error_code err;
  const std::array<boost::asio::const_buffer, 4> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(&frameheadersize, sizeof(frameheadersize)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), frameheadersize),
    boost::asio::const_buffer(data, size)
  };
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendHardwareStats(const uint64_t timestamp, const std::vector<monocle::DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory, const uint64_t availablememory)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  std::vector< flatbuffers::Offset<monocle::DiskStat> > diskstatsbuffer;
  fbb_.Clear();
  for (const DISKSTAT& diskstat : diskstats)
  {
    diskstatsbuffer.push_back(CreateDiskStat(fbb_, fbb_.CreateString(diskstat.device_), diskstat.utility_, diskstat.readspeed_, diskstat.writespeed_));

  }

  fbb_.Finish(CreateHardwareStats(fbb_, timestamp, fbb_.CreateVector(diskstatsbuffer), cpuusage, totalmemory, availablememory));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::HARDWARESTATS, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendJPEGFrame(const uint64_t stream, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const boost::optional<uint64_t>& sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* data, const size_t size)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateJPEGFrameHeader(fbb_, stream, playrequest, codecindex, timestamp, sequencenum.is_initialized() ? *sequencenum : 0, progress, signature ? fbb_.CreateVector(signature, signaturesize) : flatbuffers::Offset< flatbuffers::Vector<uint8_t> >(), restartinterval, typespecificfragmentoffset, type, q, width, height, lqt ? fbb_.CreateVector<uint8_t>(lqt, 64) : 0, cqt ? fbb_.CreateVector<uint8_t>(cqt, 64) : 0));
  const uint32_t frameheadersize = static_cast<uint32_t>(fbb_.GetSize());
  const uint32_t messagesize = sizeof(frameheadersize) + frameheadersize + static_cast<uint32_t>(size);
  const HEADER header(messagesize, false, false, Message::JPEGFRAME, ++sequence_);

  boost::system::error_code err;
  const std::array<boost::asio::const_buffer, 4> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(&frameheadersize, sizeof(frameheadersize)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), frameheadersize),
    boost::asio::const_buffer(data, size)
  };
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendLocationChanged(const std::string& latitude, const std::string& location)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateLocationChanged(fbb_, fbb_.CreateString(latitude), fbb_.CreateString(location)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::LOCATIONCHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendMapAdded(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateMapAdded(fbb_, token, fbb_.CreateString(name), fbb_.CreateString(location), fbb_.CreateString(imagemd5)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::MAPADDED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendMapChanged(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateMapChanged(fbb_, token, fbb_.CreateString(name), fbb_.CreateString(location), fbb_.CreateString(imagemd5)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::MAPCHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendMapRemoved(const uint64_t token)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateMapRemoved(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::MAPREMOVED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendMetadataFrame(const uint64_t stream, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const boost::optional<uint64_t>& sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* data, const size_t size)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateMetadataFrameHeader(fbb_, stream, playrequest, codecindex, true, timestamp, sequencenum.is_initialized() ? *sequencenum : 0, progress, signature ? fbb_.CreateVector(signature, signaturesize) : flatbuffers::Offset< flatbuffers::Vector<uint8_t> >(), metadataframetype));
  const uint32_t frameheadersize = static_cast<uint32_t>(fbb_.GetSize());
  const uint32_t messagesize = sizeof(frameheadersize) + frameheadersize + static_cast<uint32_t>(size);
  const HEADER header(messagesize, false, false, Message::METADATAFRAME, ++sequence_);

  boost::system::error_code err;
  const std::array<boost::asio::const_buffer, 4> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(&frameheadersize, sizeof(frameheadersize)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), frameheadersize),
    boost::asio::const_buffer(data, size)
  };
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendMountAdded(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateMountPointAdded(fbb_, id, parentid, majorstdev, minorstdev, fbb_.CreateString(path), fbb_.CreateString(type), fbb_.CreateString(source)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::MOUNTPOINTADDED, ++sequence_);

  boost::system::error_code err;
  const std::array<boost::asio::const_buffer, 4> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendMountRemoved(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateMountPointRemoved(fbb_, id, parentid, majorstdev, minorstdev, fbb_.CreateString(path), fbb_.CreateString(type), fbb_.CreateString(source)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::MOUNTPOINTREMOVED, ++sequence_);

  boost::system::error_code err;
  const std::array<boost::asio::const_buffer, 4> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendMPEG4Frame(const uint64_t stream, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const boost::optional<uint64_t>& sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* data, const size_t size)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateMPEG4FrameHeader(fbb_, stream, playrequest, codecindex, marker, timestamp, sequencenum.is_initialized() ? *sequencenum : 0, progress, signature ? fbb_.CreateVector(signature, signaturesize) : flatbuffers::Offset< flatbuffers::Vector<uint8_t> >()));
  const uint32_t frameheadersize = static_cast<uint32_t>(fbb_.GetSize());
  const uint32_t messagesize = sizeof(frameheadersize) + frameheadersize + static_cast<uint32_t>(size);
  const HEADER header(messagesize, false, false, Message::MPEG4FRAME, ++sequence_);

  boost::system::error_code err;
  const std::array<boost::asio::const_buffer, 4> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(&frameheadersize, sizeof(frameheadersize)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), frameheadersize),
    boost::asio::const_buffer(data, size)
  };
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendNameChanged(const std::string& name)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateNameChanged(fbb_, fbb_.CreateString(name)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::NAMECHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendNewCodecIndex(const uint64_t stream, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateNewCodecIndex(fbb_, stream, CreateCodecIndex(fbb_, id, codec, fbb_.CreateString(parameters), timestamp)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::NEWCODECINDEX, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendONVIFUserAdded(const uint64_t token, const std::string& username, const ONVIFUserlevel onvifuserlevel)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateONVIFUserAdded(fbb_, token, fbb_.CreateString(username), onvifuserlevel));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ONVIFUSERADDED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendONVIFUserChanged(const uint64_t token, const boost::optional<std::string>& username, const ONVIFUserlevel onvifuserlevel)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  flatbuffers::Offset<flatbuffers::String> usernamebuffer = 0;
  if (username.is_initialized())
  {
    usernamebuffer = fbb_.CreateString(*username);

  }
  fbb_.Finish(CreateONVIFUserChanged(fbb_, token, usernamebuffer, onvifuserlevel));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ONVIFUSERCHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendONVIFUserRemoved(const uint64_t token)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateONVIFUserRemoved(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::ONVIFUSERREMOVED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendReceiverAdded(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, const monocle::ReceiverState state)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateReceiver(fbb_, token, mode, fbb_.CreateString(uri), autocreated, fbb_.CreateString(username), fbb_.CreateString(password), fbb_.CreateVectorOfStrings(parameters), state));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECEIVERADDED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendReceiverChanged(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateReceiverChanged(fbb_, token, mode, fbb_.CreateString(uri), autocreated, fbb_.CreateString(username), fbb_.CreateString(password), fbb_.CreateVectorOfStrings(parameters)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECEIVERCHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendReceiverRemoved(const uint64_t token)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateReceiverRemoved(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECEIVERREMOVED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingActiveJobChanged(const uint64_t token, const boost::optional<uint64_t>& activejob)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  std::unique_ptr<monocle::TOKEN> activejobtoken;
  if (activejob.is_initialized())
  {
    activejobtoken = std::make_unique<monocle::TOKEN>(*activejob);
  }
  fbb_.Clear();
  fbb_.Finish(CreateRecordingActiveJobChanged(fbb_, token, activejobtoken.get()));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGACTIVEJOBCHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingAdded(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  std::unique_ptr<monocle::TOKEN> activejobtoken;
  if (activejob.is_initialized())
  {
    activejobtoken = std::make_unique<monocle::TOKEN>(*activejob);
  }
  fbb_.Clear();
  fbb_.Finish(CreateRecordingAdded(fbb_, token, fbb_.CreateString(sourceid), fbb_.CreateString(name), fbb_.CreateString(location), fbb_.CreateString(description), fbb_.CreateString(address), fbb_.CreateString(content), retentiontime, activejobtoken.get()));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGADDED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingChanged(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  std::unique_ptr<monocle::TOKEN> activejobtoken;
  if (activejob.is_initialized())
  {
    activejobtoken = std::make_unique<monocle::TOKEN>(*activejob);
  }
  fbb_.Finish(CreateRecordingChanged(fbb_, token, fbb_.CreateString(sourceid), fbb_.CreateString(name), fbb_.CreateString(location), fbb_.CreateString(description), fbb_.CreateString(address), fbb_.CreateString(content), retentiontime, activejobtoken.get()));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGCHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingJobLogMessage(const uint64_t recordingtoken, const uint64_t token, const std::chrono::system_clock::time_point time, const monocle::Severity severity, const std::string& message)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingJobLogMessage(fbb_, recordingtoken, token, CreateLogMessage(fbb_, std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count(), severity, fbb_.CreateString(message))));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGJOBLOGMESSAGE, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingJobAdded(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const monocle::RecordingJobState state)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingJobAdded(fbb_, recordingtoken, token, fbb_.CreateString(name), enabled, priority, state));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGJOBADDED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingJobChanged(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingJobChanged(fbb_, recordingtoken, token, fbb_.CreateString(name), enabled, priority));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGJOBCHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingJobRemoved(const uint64_t recordingtoken, const uint64_t token)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingJobRemoved(fbb_, recordingtoken, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGJOBREMOVED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingJobSourceAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token, const uint64_t receivertoken)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingJobSourceAdded(fbb_, recordingtoken, recordingjobtoken, token, receivertoken));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGJOBSOURCEADDED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingJobSourceRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingJobSourceRemoved(fbb_, recordingtoken, recordingjobtoken, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGJOBSOURCEREMOVED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingJobSourceTrackActiveParametersChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const std::vector<std::string>& activeparameters)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingJobSourceTrackActiveParametersChanged(fbb_, recording, recordingjob, recordingjobsource, recordingjobsourcetrack, fbb_.CreateVectorOfStrings(activeparameters)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGJOBSOURCETRACKACTIVEPARAMETERSCHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingJobSourceTrackAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint32_t recordingtrackid, const std::vector<std::string>& parameters, const RecordingJobState state, const std::string& error, const std::vector<std::string>& activeparameters)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingJobSourceTrackAdded(fbb_, recordingtoken, recordingjobtoken, recordingjobsourcetoken, token, recordingtrackid, fbb_.CreateVectorOfStrings(parameters), state, fbb_.CreateString(error), fbb_.CreateVectorOfStrings(activeparameters)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGJOBSOURCETRACKADDED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingTrackCodecAdded(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingTrackCodecAdded(fbb_, recordingtoken, recordingtrackid, id, codec, fbb_.CreateString(parameters), timestamp));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGTRACKCODECADDED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingTrackCodecRemoved(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingTrackCodecRemoved(fbb_, recordingtoken, recordingtrackid, id));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGTRACKCODECREMOVED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingJobSourceTrackLogMessage(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const std::chrono::system_clock::time_point time, const monocle::Severity severity, const std::string& message)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingJobSourceTrackLogMessage(fbb_, recordingtoken, recordingjobtoken, recordingjobsourcetoken, token, CreateLogMessage(fbb_, std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count(), severity, fbb_.CreateString(message))));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGJOBSOURCETRACKLOGMESSAGE, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingJobSourceTrackRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingJobSourceTrackRemoved(fbb_, recordingtoken, recordingjobtoken, recordingjobsourcetoken, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGJOBSOURCETRACKREMOVED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingJobSourceTrackStateChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const uint64_t time, const RecordingJobState state, const std::string& error)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingJobSourceTrackStateChanged(fbb_, recording, recordingjob, recordingjobsource, recordingjobsourcetrack, time, state, fbb_.CreateString(error)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGJOBSOURCETRACKSTATECHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingLogMessage(const uint64_t token, const std::chrono::system_clock::time_point time, const monocle::Severity severity, const std::string& message)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingLogMessage(fbb_, token, CreateLogMessage(fbb_, std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count(), severity, fbb_.CreateString(message))));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGLOGMESSAGE, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingRemoved(const uint64_t token)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingRemoved(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGREMOVED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendRecordingTrackLogMessage(const uint64_t recordingtoken, const uint32_t id, const std::chrono::system_clock::time_point time, const monocle::Severity severity, const std::string& message)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateRecordingTrackLogMessage(fbb_, recordingtoken, id, CreateLogMessage(fbb_, std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count(), severity, fbb_.CreateString(message))));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::RECORDINGTRACKLOGMESSAGE, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendServerLogMessage(const std::chrono::system_clock::time_point time, const monocle::Severity severity, const std::string& message)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateLogMessage(fbb_, std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count(), severity, fbb_.CreateString(message)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::SERVERLOGMESSAGE, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendTrackAdded(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateTrackAdded(fbb_, recordingtoken, id, fbb_.CreateString(token), tracktype, fbb_.CreateString(description), fixedfiles, digitalsigning, encrypt, flushfrequency));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::TRACKADDED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendTrackChanged(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateTrackChanged(fbb_, recordingtoken, id, fbb_.CreateString(token), tracktype, fbb_.CreateString(description), fixedfiles, digitalsigning, encrypt, flushfrequency));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::TRACKCHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendTrackRemoved(const uint64_t recordingtoken, const uint32_t id)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateTrackRemoved(fbb_, recordingtoken, id));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::TRACKREMOVED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendTrackSetData(const uint64_t recording, const uint32_t trackid, const std::vector<monocle::INDEX>& indices)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateTrackSetData(fbb_, recording, trackid, fbb_.CreateVectorOfStructs<monocle::INDEX>(indices)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::TRACKSETDATA, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendTrackDeleteData(const uint64_t recording, const uint32_t trackid, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateTrackDeleteData(fbb_, recording, trackid, start.is_initialized() ? *start : 0, end.is_initialized() ? *end : 0));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::TRACKDELETEDATA, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendUserAdded(const uint64_t token, const std::string& username, const std::string& digest, const uint64_t group)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateUserAdded(fbb_, token, fbb_.CreateString(username), fbb_.CreateString(digest), group));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::USERADDED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendUserChanged(const uint64_t token, const uint64_t group)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateUserChanged(fbb_, token, group));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::USERCHANGED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendUserRemoved(const uint64_t token)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Clear();
  fbb_.Finish(CreateUserRemoved(fbb_, token));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, false, false, Message::USERREMOVED, ++sequence_);
  const std::array<boost::asio::const_buffer, 2> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

void Connection::HandleReadHeader(const boost::system::error_code& err, std::size_t size)
{
  if (err)
  {
    DestroyConnection();
    return;
  }

  const HEADER header = *boost::asio::buffer_cast<const HEADER*>(buffer_.data());
  buffer_.consume(sizeof(HEADER));
  if (header.size_ == 0)
  {
    if (HandleMessage(header.IsError(), header.IsCompressed(), header.message_, header.sequence_, nullptr, 0))
    {
      DestroyConnection();
      return;
    }
    boost::asio::async_read(socket_, buffer_, boost::asio::transfer_exactly(sizeof(HEADER)), boost::bind(&Connection::HandleReadHeader, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }
  else if (buffer_.in_avail() >= header.size_)
  {
    if (HandleMessage(header.IsError(), header.IsCompressed(), header.message_, header.sequence_, boost::asio::buffer_cast<const char*>(buffer_.data()), header.size_))
    {
      DestroyConnection();
      return;
    }
    buffer_.consume(header.size_);
    boost::asio::async_read(socket_, buffer_, boost::asio::transfer_exactly(sizeof(HEADER)), boost::bind(&Connection::HandleReadHeader, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }
  else
  {
    boost::asio::async_read(socket_, buffer_, boost::asio::transfer_exactly(header.size_ - buffer_.in_avail()), boost::bind(&Connection::HandleReadMessage, shared_from_this(), boost::asio::placeholders::error, header.size_, header.IsError(), header.IsCompressed(), header.message_, header.sequence_));

  }
}

void Connection::HandleReadMessage(const boost::system::error_code& err, const uint32_t size, const bool error, const bool compressed, const Message message, const uint16_t sequence)
{
  if (err)
  {
    DestroyConnection();
    return;
  }

  if (HandleMessage(error, compressed, message, sequence, boost::asio::buffer_cast<const char*>(buffer_.data()), size))
  {
    DestroyConnection();
    return;
  }
  buffer_.consume(size);
  boost::asio::async_read(socket_, buffer_, boost::asio::transfer_exactly(sizeof(HEADER)), boost::bind(&Connection::HandleReadHeader, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

boost::system::error_code Connection::HandleMessage(const bool error, const bool compressed, const Message message, const uint16_t sequence, const char* data, size_t datasize)
{
  if (compressed)
  {
    if (utility::Decompress(inflate_, data, datasize, zbuffer_))
    {

      return SendErrorResponse(message, sequence, monocle::Error(monocle::ErrorCode::InternalError, "Decompression failed"));
    }
    data = zbuffer_.data();
    datasize = zbuffer_.size();
  }

  if (!Permission(message))
  {

    return SendErrorResponse(message, sequence, Error(ErrorCode::Permission, "Insufficient permissions"));
  }

  switch (message)
  {
    case Message::ADDFILE:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::ADDFILE, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddFileRequest>(nullptr))
      {

        return SendErrorResponse(Message::ADDFILE, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const AddFileRequest* addfilerequest = flatbuffers::GetRoot<AddFileRequest>(data);
      if (addfilerequest == nullptr)
      {

        return SendErrorResponse(Message::ADDFILE, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      std::string mountpoint;
      if (addfilerequest->mountpoint())
      {
        mountpoint = addfilerequest->mountpoint()->str();

      }

      if (addfilerequest->path() == nullptr)
      {

        return SendErrorResponse(Message::ADDFILE, sequence, Error(ErrorCode::MissingParameter, "Missing parameter path"));
      }

      if (addfilerequest->numchunks() == 0)
      {

        return SendErrorResponse(Message::ADDFILE, sequence, Error(ErrorCode::MissingParameter, "Missing parameter numchunks"));
      }

      if (addfilerequest->chunksize() == 0)
      {

        return SendErrorResponse(Message::ADDFILE, sequence, Error(ErrorCode::MissingParameter, "Missing parameter chunksize"));
      }

      const Error error = AddFile(mountpoint, addfilerequest->path()->str(), addfilerequest->filldisk(), addfilerequest->numchunks(), addfilerequest->chunksize(), addfilerequest->automount());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::ADDFILE, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::ADDFILE, sequence));
    }
    case Message::ADDGROUP:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::ADDGROUP, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddGroupRequest>(nullptr))
      {

        return SendErrorResponse(Message::ADDGROUP, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const AddGroupRequest* addgrouprequest = flatbuffers::GetRoot<AddGroupRequest>(data);
      if (addgrouprequest == nullptr)
      {

        return SendErrorResponse(Message::ADDGROUP, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      if (addgrouprequest->name() == nullptr)
      {

        return SendErrorResponse(Message::ADDGROUP, sequence, Error(ErrorCode::InvalidMessage, "Missing name"));
      }

      std::vector<uint64_t> recordings;
      if (addgrouprequest->recordings())
      {
        recordings.reserve(addgrouprequest->recordings()->size());
        for (const uint64_t recording : *addgrouprequest->recordings())
        {
          recordings.push_back(recording);

        }
      }

      const Error error = AddGroup(addgrouprequest->name()->str(), addgrouprequest->manageusers(), addgrouprequest->managerecordings(), addgrouprequest->managemaps(), addgrouprequest->managedevice(), addgrouprequest->allrecordings(), recordings);
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::ADDGROUP, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::ADDGROUP, sequence));
    }
    case Message::ADDMAP:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::ADDMAP, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddMapRequest>(nullptr))
      {

        return SendErrorResponse(Message::ADDMAP, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const AddMapRequest* addmaprequest = flatbuffers::GetRoot<AddMapRequest>(data);
      if (addmaprequest == nullptr)
      {

        return SendErrorResponse(Message::ADDMAP, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      if (addmaprequest->name() == nullptr)
      {

        return SendErrorResponse(Message::ADDMAP, sequence, Error(ErrorCode::InvalidMessage, "Missing name"));
      }

      if (addmaprequest->location() == nullptr)
      {

        return SendErrorResponse(Message::ADDMAP, sequence, Error(ErrorCode::InvalidMessage, "Missing location"));
      }

      if (addmaprequest->image() == nullptr)
      {

        return SendErrorResponse(Message::ADDMAP, sequence, Error(ErrorCode::InvalidMessage, "Missing image"));
      }

      const Error error = AddMap(addmaprequest->name()->str(), addmaprequest->location()->str(), std::vector<int8_t>(addmaprequest->image()->cbegin(), addmaprequest->image()->cend()));
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::ADDMAP, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::ADDMAP, sequence));
    }
    case Message::ADDONVIFUSER:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::ADDONVIFUSER, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddONVIFUserRequest>(nullptr))
      {

        return SendErrorResponse(Message::ADDONVIFUSER, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const AddONVIFUserRequest* addonvifuserrequest = flatbuffers::GetRoot<AddONVIFUserRequest>(data);
      if (addonvifuserrequest == nullptr)
      {

        return SendErrorResponse(Message::ADDONVIFUSER, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      if ((addonvifuserrequest->username() == nullptr) || (addonvifuserrequest->password() == nullptr))
      {

        return SendErrorResponse(Message::ADDONVIFUSER, sequence, Error(ErrorCode::MissingParameter, "Missing parameter"));
      }

      const Error error = AddONVIFUser(addonvifuserrequest->username()->str(), addonvifuserrequest->password()->str(), addonvifuserrequest->onvifuserlevel());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::ADDONVIFUSER, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::ADDONVIFUSER, sequence));
    }
    case Message::ADDRECEIVER:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::ADDRECEIVER, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddReceiverRequest>(nullptr))
      {

        return SendErrorResponse(Message::ADDRECEIVER, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const AddReceiverRequest* addreceiverrequest = flatbuffers::GetRoot<AddReceiverRequest>(data);
      if (addreceiverrequest == nullptr)
      {

        return SendErrorResponse(Message::ADDRECEIVER, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      if (addreceiverrequest->mediauri() == nullptr)
      {

        return SendErrorResponse(Message::ADDRECEIVER, sequence, Error(ErrorCode::MissingParameter, "Missing parameter path"));
      }

      try
      {
        const network::uri uri(addreceiverrequest->mediauri()->str());
        if (!uri.has_host())
        {

          return SendErrorResponse(Message::ADDRECEIVER, sequence, monocle::Error(ErrorCode::InvalidMessage, std::string("Invalid receiver uri(no host): ") + addreceiverrequest->mediauri()->str()));
        }
      }
      catch (...)
      {

        return SendErrorResponse(Message::ADDRECEIVER, sequence, monocle::Error(ErrorCode::InvalidMessage, std::string("Invalid receiver uri: ") + addreceiverrequest->mediauri()->str()));
      }

      const std::string username = addreceiverrequest->username() ? addreceiverrequest->username()->str() : std::string();
      const std::string password = addreceiverrequest->password() ? addreceiverrequest->password()->str() : std::string();

      std::vector<std::string> parameters;
      if (addreceiverrequest->parameters())
      {
        parameters.reserve(addreceiverrequest->parameters()->size());
        for (const flatbuffers::String* parameter : *addreceiverrequest->parameters())
        {
          parameters.push_back(parameter->str());

        }
      }

      const Error error = AddReceiver(addreceiverrequest->mode(), addreceiverrequest->mediauri()->str(), username, password, parameters);
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::ADDRECEIVER, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::ADDRECEIVER, sequence));
    }
    case Message::ADDRECORDING:
    {
      if (data == nullptr)
      {
       
        return SendErrorResponse(Message::ADDRECORDING, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddRecordingRequest>(nullptr))
      {

        return SendErrorResponse(Message::ADDRECORDING, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const AddRecordingRequest* addrecordingrequest = flatbuffers::GetRoot<AddRecordingRequest>(data);
      if (addrecordingrequest == nullptr)
      {

        return SendErrorResponse(Message::ADDRECORDING, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      std::string sourceid;
      if (addrecordingrequest->sourceid())
      {
        sourceid = addrecordingrequest->sourceid()->str();

      }

      std::string name;
      if (addrecordingrequest->name())
      {
        name = addrecordingrequest->name()->str();

      }

      std::string location;
      if (addrecordingrequest->location())
      {
        location = addrecordingrequest->location()->str();

      }

      std::string description;
      if (addrecordingrequest->description())
      {
        description = addrecordingrequest->description()->str();

      }

      std::string address;
      if (addrecordingrequest->address())
      {
        address = addrecordingrequest->address()->str();

      }

      std::string content;
      if (addrecordingrequest->content())
      {
        content = addrecordingrequest->content()->str();

      }

      const std::pair<Error, uint64_t> error = AddRecording(sourceid, name, location, description, address, content, addrecordingrequest->retentiontime(), addrecordingrequest->createdefaulttracks(), addrecordingrequest->createdefaultjob());
      if (error.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::ADDRECORDING, sequence, error.first);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateAddRecordingResponse(fbb_, error.second));
      return SendResponse(true, Message::ADDRECORDING, sequence);
    }
    case Message::ADDRECORDINGJOB:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::ADDRECORDINGJOB, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddRecordingJobRequest>(nullptr))
      {

        return SendErrorResponse(Message::ADDRECORDINGJOB, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const AddRecordingJobRequest* addrecordingjobrequest = flatbuffers::GetRoot<AddRecordingJobRequest>(data);
      if (addrecordingjobrequest == nullptr)
      {

        return SendErrorResponse(Message::ADDRECORDINGJOB, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      std::string name;
      if (addrecordingjobrequest->name())
      {
        name = addrecordingjobrequest->name()->str();

      }

      std::vector<ADDRECORDINGJOBSOURCE> sources;
      if (addrecordingjobrequest->sources())
      {
        sources.reserve(addrecordingjobrequest->sources()->size());
        for (const monocle::AddRecordingJobSource* source : *addrecordingjobrequest->sources())
        {
          std::vector<std::string> sourceparameters;
          if (source->sourceparameters())
          {
            sourceparameters.reserve(source->sourceparameters()->size());
            for (const flatbuffers::String* sourceparameter : *source->sourceparameters())
            {
              sourceparameters.push_back(sourceparameter->str());

            }
          }

          std::vector<std::string> recordingjobsourcetrackparameters;
          if (source->recordingjobsourcetrackparameters())
          {
            recordingjobsourcetrackparameters.reserve(source->recordingjobsourcetrackparameters()->size());
            for (const flatbuffers::String* recordingjobsourcetrackparameter : *source->recordingjobsourcetrackparameters())
            {
              recordingjobsourcetrackparameters.push_back(recordingjobsourcetrackparameter->str());

            }
          }

          std::string mediauri;
          if (source->mediauri())
          {
            mediauri = source->mediauri()->str();

          }

          std::string username;
          if (source->username())
          {
            username = source->username()->str();

          }

          std::string password;
          if (source->password())
          {
            password = source->password()->str();

          }

          sources.emplace_back(source->recordingtrackid(), source->receivermode(), sourceparameters, mediauri, username, password, recordingjobsourcetrackparameters);
        }
      }

      const Error error = AddRecordingJob(addrecordingjobrequest->recordingtoken(), name, addrecordingjobrequest->enabled(), addrecordingjobrequest->priority(), sources);
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::ADDRECORDINGJOB, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::ADDRECORDINGJOB, sequence));
    }
    case Message::ADDTRACK:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::ADDTRACK, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddTrackRequest>(nullptr))
      {

        return SendErrorResponse(Message::ADDTRACK, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const AddTrackRequest* addtrackrequest = flatbuffers::GetRoot<AddTrackRequest>(data);
      if (addtrackrequest == nullptr)
      {

        return SendErrorResponse(Message::ADDTRACK, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      std::string description;
      if (addtrackrequest->description())
      {
        description = addtrackrequest->description()->str();

      }

      std::vector<uint64_t> files;
      files.reserve(addtrackrequest->files()->size());
      if (addtrackrequest->files())
      {
        for (const uint64_t file : *addtrackrequest->files())
        {
          files.push_back(file);

        }
      }

      const std::pair<Error, uint32_t> error = AddTrack(addtrackrequest->recordingtoken(), addtrackrequest->tracktype(), description, addtrackrequest->fixedfiles(), addtrackrequest->digitalsigning(), addtrackrequest->encrypt(), addtrackrequest->flushfrequency(), files);
      if (error.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::ADDTRACK, sequence, error.first);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateAddTrackResponse(fbb_, error.second));
      return SendResponse(true, Message::ADDTRACK, sequence);
    }
    case Message::ADDTRACK2:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::ADDTRACK2, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddTrackRequest2>(nullptr))
      {

        return SendErrorResponse(Message::ADDTRACK2, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const AddTrackRequest2* addtrackrequest2 = flatbuffers::GetRoot<AddTrackRequest2>(data);
      if (addtrackrequest2 == nullptr)
      {

        return SendErrorResponse(Message::ADDTRACK2, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      std::string description;
      if (addtrackrequest2->description())
      {
        description = addtrackrequest2->description()->str();

      }

      std::vector<uint64_t> files;
      files.reserve(addtrackrequest2->files()->size());
      if (addtrackrequest2->files())
      {
        for (const uint64_t file : *addtrackrequest2->files())
        {
          files.push_back(file);

        }
      }

      std::string mediauri;
      if (addtrackrequest2->mediauri())
      {
        mediauri = addtrackrequest2->mediauri()->str();

      }

      std::string username;
      if (addtrackrequest2->username())
      {
        username = addtrackrequest2->username()->str();

      }

      std::string password;
      if (addtrackrequest2->password())
      {
        password = addtrackrequest2->password()->str();

      }

      std::vector<std::string> receiverparameters;
      receiverparameters.reserve(addtrackrequest2->receiverparameters()->size());
      if (addtrackrequest2->receiverparameters())
      {
        for (const flatbuffers::String* receiverparameter : *addtrackrequest2->receiverparameters())
        {
          receiverparameters.push_back(receiverparameter->str());

        }
      }

      std::vector<std::string> sourceparameters;
      if (addtrackrequest2->receiverparameters())
      {
        sourceparameters.reserve(addtrackrequest2->sourceparameters()->size());
        for (const flatbuffers::String* sourceparameter : *addtrackrequest2->sourceparameters())
        {
          sourceparameters.push_back(sourceparameter->str());

        }
      }

      std::vector<std::string> objectdetectorsourceparameters;
      if (addtrackrequest2->objectdetectorsourceparameters())
      {
        objectdetectorsourceparameters.reserve(addtrackrequest2->objectdetectorsourceparameters()->size());
        for (const flatbuffers::String* objectdetectorsourceparameter : *addtrackrequest2->objectdetectorsourceparameters())
        {
          objectdetectorsourceparameters.push_back(objectdetectorsourceparameter->str());

        }
      }

      const Error error = AddTrack2(addtrackrequest2->recordingtoken(), addtrackrequest2->recordingjobtoken(), addtrackrequest2->tracktype(), description, addtrackrequest2->fixedfiles(), addtrackrequest2->digitalsigning(), addtrackrequest2->encrypt(), addtrackrequest2->flushfrequency(), files, mediauri, username, password, receiverparameters, sourceparameters, objectdetectorsourceparameters);
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::ADDTRACK2, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::ADDTRACK2, sequence));
    }
    case Message::ADDUSER:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::ADDUSER, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AddUserRequest>(nullptr))
      {

        return SendErrorResponse(Message::ADDUSER, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const AddUserRequest* adduserrequest = flatbuffers::GetRoot<AddUserRequest>(data);
      if (adduserrequest == nullptr)
      {

        return SendErrorResponse(Message::ADDUSER, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      if ((adduserrequest->username() == nullptr) || (adduserrequest->digest() == nullptr))
      {

        return SendErrorResponse(Message::ADDUSER, sequence, Error(ErrorCode::MissingParameter, "Missing parameter"));
      }

      const Error error = AddUser(adduserrequest->username()->str(), adduserrequest->digest()->str(), adduserrequest->group());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::ADDUSER, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::ADDUSER, sequence));
    }
    case Message::CHANGEGROUP:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::CHANGEGROUP, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ChangeGroupRequest>(nullptr))
      {

        return SendErrorResponse(Message::CHANGEGROUP, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const ChangeGroupRequest* changegrouprequest = flatbuffers::GetRoot<ChangeGroupRequest>(data);
      if (changegrouprequest == nullptr)
      {

        return SendErrorResponse(Message::CHANGEGROUP, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      if (changegrouprequest->name() == nullptr)
      {

        return SendErrorResponse(Message::CHANGEGROUP, sequence, Error(ErrorCode::InvalidMessage, "Missing name"));
      }

      std::vector<uint64_t> recordings;
      if (changegrouprequest->recordings())
      {
        recordings.reserve(changegrouprequest->recordings()->size());
        for (const uint64_t recording : *changegrouprequest->recordings())
        {
          recordings.push_back(recording);

        }
      }

      const Error error = ChangeGroup(changegrouprequest->token(), changegrouprequest->name()->str(), changegrouprequest->manageusers(), changegrouprequest->managerecordings(), changegrouprequest->managemaps(), changegrouprequest->managedevice(), changegrouprequest->allrecordings(), recordings);
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CHANGEGROUP, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CHANGEGROUP, sequence));
    }
    case Message::CHANGEMAP:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::CHANGEMAP, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ChangeMapRequest>(nullptr))
      {

        return SendErrorResponse(Message::CHANGEMAP, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const ChangeMapRequest* changemaprequest = flatbuffers::GetRoot<ChangeMapRequest>(data);
      if (changemaprequest == nullptr)
      {

        return SendErrorResponse(Message::CHANGEMAP, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      std::string name;
      if (changemaprequest->name())
      {
        name = changemaprequest->name()->str();

      }

      std::string location;
      if (changemaprequest->location())
      {
        location = changemaprequest->location()->str();

      }

      std::vector<int8_t> image;
      if (changemaprequest->image())
      {
        image.reserve(changemaprequest->image()->size());
        image = std::vector<int8_t>(changemaprequest->image()->cbegin(), changemaprequest->image()->cend());
      }

      const Error error = ChangeMap(changemaprequest->token(), name, location, image);
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CHANGEMAP, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CHANGEMAP, sequence));
    }
    case Message::CHANGEONVIFUSER:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::CHANGEONVIFUSER, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ChangeONVIFUserRequest>(nullptr))
      {

        return SendErrorResponse(Message::CHANGEONVIFUSER, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const ChangeONVIFUserRequest* changeonvifuserrequest = flatbuffers::GetRoot<ChangeONVIFUserRequest>(data);
      if (changeonvifuserrequest == nullptr)
      {

        return SendErrorResponse(Message::CHANGEONVIFUSER, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      boost::optional<std::string> username;
      if (changeonvifuserrequest->username())
      {
        username = changeonvifuserrequest->username()->str();

      }

      boost::optional<std::string> password;
      if (changeonvifuserrequest->password())
      {
        password = changeonvifuserrequest->password()->str();

      }

      const Error error = ChangeONVIFUser(changeonvifuserrequest->token(), username, password, changeonvifuserrequest->onvifuserlevel());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CHANGEONVIFUSER, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CHANGEONVIFUSER, sequence));
    }
    case Message::CHANGERECEIVER:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::CHANGERECEIVER, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ChangeReceiverRequest>(nullptr))
      {

        return SendErrorResponse(Message::CHANGERECEIVER, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const ChangeReceiverRequest* changereceiverrequest = flatbuffers::GetRoot<ChangeReceiverRequest>(data);
      if (changereceiverrequest == nullptr)
      {

        return SendErrorResponse(Message::CHANGERECEIVER, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      if (changereceiverrequest->mediauri() == nullptr)
      {

        return SendErrorResponse(Message::CHANGERECEIVER, sequence, Error(ErrorCode::MissingParameter, "Missing mediauri"));
      }

      try
      {
        const network::uri uri(changereceiverrequest->mediauri()->str());
        if (!uri.has_host())
        {

          return SendErrorResponse(Message::CHANGERECEIVER, sequence, monocle::Error(ErrorCode::InvalidMessage, std::string("Invalid receiver uri(no host): ") + changereceiverrequest->mediauri()->str()));
        }
      }
      catch (...)
      {

        return SendErrorResponse(Message::CHANGERECEIVER, sequence, monocle::Error(ErrorCode::InvalidMessage, std::string("Invalid receiver uri: ") + changereceiverrequest->mediauri()->str()));
      }

      const std::string username = changereceiverrequest->username() ? changereceiverrequest->username()->str() : std::string();
      const std::string password = changereceiverrequest->password() ? changereceiverrequest->password()->str() : std::string();

      std::vector<std::string> parameters;
      if (changereceiverrequest->parameters())
      {
        parameters.reserve(changereceiverrequest->parameters()->size());
        for (const flatbuffers::String* parameter : *changereceiverrequest->parameters())
        {
          parameters.push_back(parameter->str());

        }
      }

      const Error error = ChangeReceiver(changereceiverrequest->token(), changereceiverrequest->mode(), changereceiverrequest->mediauri()->str(), username, password, parameters);
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CHANGERECEIVER, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CHANGERECEIVER, sequence));
    }
    case Message::CHANGERECORDING:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::CHANGERECORDING, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ChangeRecordingRequest>(nullptr))
      {

        return SendErrorResponse(Message::CHANGERECORDING, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const ChangeRecordingRequest* changerecordingrequest = flatbuffers::GetRoot<ChangeRecordingRequest>(data);
      if (changerecordingrequest == nullptr)
      {

        return SendErrorResponse(Message::CHANGERECORDING, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      std::string sourceid;
      if (changerecordingrequest->sourceid())
      {
        sourceid = changerecordingrequest->sourceid()->str();

      }

      std::string name;
      if (changerecordingrequest->name())
      {
        name = changerecordingrequest->name()->str();

      }

      std::string location;
      if (changerecordingrequest->location())
      {
        location = changerecordingrequest->location()->str();

      }

      std::string description;
      if (changerecordingrequest->description())
      {
        description = changerecordingrequest->description()->str();

      }

      std::string address;
      if (changerecordingrequest->address())
      {
        address = changerecordingrequest->address()->str();

      }

      std::string content;
      if (changerecordingrequest->content())
      {
        content = changerecordingrequest->content()->str();

      }

      const Error error = ChangeRecording(changerecordingrequest->token(), sourceid, name, location, description, address, content, changerecordingrequest->retentiontime());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CHANGERECORDING, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CHANGERECORDING, sequence));
    }
    case Message::CHANGERECORDINGJOB:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::CHANGERECORDINGJOB, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ChangeRecordingJobRequest>(nullptr))
      {

        return SendErrorResponse(Message::CHANGERECORDINGJOB, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const ChangeRecordingJobRequest* changerecordingjobrequest = flatbuffers::GetRoot<ChangeRecordingJobRequest>(data);
      if (changerecordingjobrequest == nullptr)
      {

        return SendErrorResponse(Message::CHANGERECORDINGJOB, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      std::string name;
      if (changerecordingjobrequest->name())
      {
        name = changerecordingjobrequest->name()->str();

      }

      std::vector<CHANGERECORDINGJOBSOURCE> sources;
      if (changerecordingjobrequest->sources())
      {
        sources.reserve(changerecordingjobrequest->sources()->size());
        for (const monocle::ChangeRecordingJobSource* source : *changerecordingjobrequest->sources())
        {
          std::vector<std::string> sourceparameters;
          if (source->sourceparameters())
          {
            sourceparameters.reserve(source->sourceparameters()->size());
            for (const flatbuffers::String* sourceparameter : *source->sourceparameters())
            {
              sourceparameters.push_back(sourceparameter->str());

            }
          }

          std::vector<std::string> recordingjobsourcetrackparameters;
          if (source->recordingjobsourcetrackparameters())
          {
            recordingjobsourcetrackparameters.reserve(source->recordingjobsourcetrackparameters()->size());
            for (const flatbuffers::String* recordingjobsourcetrackparameter : *source->recordingjobsourcetrackparameters())
            {
              recordingjobsourcetrackparameters.push_back(recordingjobsourcetrackparameter->str());

            }
          }

          std::string mediauri;
          if (source->mediauri())
          {
            mediauri = source->mediauri()->str();

          }

          std::string username;
          if (source->username())
          {
            username = source->username()->str();

          }

          std::string password;
          if (source->password())
          {
            password = source->password()->str();

          }

          sources.emplace_back(source->recordingtrackid(), source->receivermode(), sourceparameters, mediauri, username, password, recordingjobsourcetrackparameters);
        }
      }

      const Error error = ChangeRecordingJob(changerecordingjobrequest->recordingtoken(), changerecordingjobrequest->token(), name, changerecordingjobrequest->enabled(), changerecordingjobrequest->priority(), sources);
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CHANGERECORDINGJOB, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CHANGERECORDINGJOB, sequence));
    }
    case Message::CHANGETRACK:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::CHANGETRACK, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ChangeTrackRequest>(nullptr))
      {

        return SendErrorResponse(Message::CHANGETRACK, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const ChangeTrackRequest* changetrackrequest = flatbuffers::GetRoot<ChangeTrackRequest>(data);
      if (changetrackrequest == nullptr)
      {

        return SendErrorResponse(Message::CHANGETRACK, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      std::string description;
      if (changetrackrequest->description())
      {
        description = changetrackrequest->description()->str();

      }

      std::vector<uint64_t> files;
      files.reserve(changetrackrequest->files()->size());
      if (changetrackrequest->files())
      {
        for (const uint64_t file : *changetrackrequest->files())
        {
          files.push_back(file);

        }
      }
    
      const Error error = ChangeTrack(changetrackrequest->recordingtoken(), changetrackrequest->id(), changetrackrequest->tracktype(), description, changetrackrequest->fixedfiles(), changetrackrequest->digitalsigning(), changetrackrequest->encrypt(), changetrackrequest->flushfrequency(), files);
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CHANGETRACK, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CHANGETRACK, sequence));
    }
    case Message::CHANGETRACK2:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::CHANGETRACK2, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ChangeTrackRequest2>(nullptr))
      {

        return SendErrorResponse(Message::CHANGETRACK2, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const ChangeTrackRequest2* changetrackrequest2 = flatbuffers::GetRoot<ChangeTrackRequest2>(data);
      if (changetrackrequest2 == nullptr)
      {

        return SendErrorResponse(Message::CHANGETRACK2, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      std::string description;
      if (changetrackrequest2->description())
      {
        description = changetrackrequest2->description()->str();

      }

      std::vector<uint64_t> files;
      files.reserve(changetrackrequest2->files()->size());
      if (changetrackrequest2->files())
      {
        for (const uint64_t file : *changetrackrequest2->files())
        {
          files.push_back(file);

        }
      }

      std::string mediauri;
      if (changetrackrequest2->mediauri())
      {
        mediauri = changetrackrequest2->mediauri()->str();

      }

      std::string username;
      if (changetrackrequest2->username())
      {
        username = changetrackrequest2->username()->str();

      }

      std::string password;
      if (changetrackrequest2->password())
      {
        password = changetrackrequest2->password()->str();

      }

      std::vector<std::string> receiverparameters;
      receiverparameters.reserve(changetrackrequest2->receiverparameters()->size());
      if (changetrackrequest2->receiverparameters())
      {
        for (const flatbuffers::String* receiverparameter : *changetrackrequest2->receiverparameters())
        {
          receiverparameters.push_back(receiverparameter->str());

        }
      }

      std::vector<std::string> sourceparameters;
      if (changetrackrequest2->receiverparameters())
      {
        sourceparameters.reserve(changetrackrequest2->sourceparameters()->size());
        for (const flatbuffers::String* sourceparameter : *changetrackrequest2->sourceparameters())
        {
          sourceparameters.push_back(sourceparameter->str());

        }
      }

      std::vector<std::string> objectdetectorsourceparameters;
      if (changetrackrequest2->objectdetectorsourceparameters())
      {
        objectdetectorsourceparameters.reserve(changetrackrequest2->objectdetectorsourceparameters()->size());
        for (const flatbuffers::String* objectdetectorsourceparameter : *changetrackrequest2->objectdetectorsourceparameters())
        {
          objectdetectorsourceparameters.push_back(objectdetectorsourceparameter->str());

        }
      }

      const Error error = ChangeTrack2(changetrackrequest2->recordingtoken(), changetrackrequest2->recordingjobtoken(), changetrackrequest2->recordingjobsourcetoken(), changetrackrequest2->recordingjobsourcetracktoken(), description, changetrackrequest2->fixedfiles(), changetrackrequest2->digitalsigning(), changetrackrequest2->encrypt(), changetrackrequest2->flushfrequency(), files, mediauri, username, password, receiverparameters, sourceparameters, objectdetectorsourceparameters);
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CHANGETRACK2, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CHANGETRACK2, sequence));
    }
    case Message::CHANGEUSER:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::CHANGEUSER, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ChangeUserRequest>(nullptr))
      {

        return SendErrorResponse(Message::CHANGEUSER, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const ChangeUserRequest* changeuserrequest = flatbuffers::GetRoot<ChangeUserRequest>(data);
      if (changeuserrequest == nullptr)
      {

        return SendErrorResponse(Message::CHANGEUSER, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      boost::optional<std::string> digest;
      if (changeuserrequest->digest())
      {
        digest = changeuserrequest->digest()->str();

      }

      const Error error = ChangeUser(changeuserrequest->token(), digest, changeuserrequest->group());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CHANGEUSER, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CHANGEUSER, sequence));
    }
    case Message::AUTHENTICATE:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::AUTHENTICATE, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<AuthenticateRequest>(nullptr))
      {

        return SendErrorResponse(Message::AUTHENTICATE, sequence, Error(ErrorCode::InvalidMessage, "Invalid Authenticate message"));
      }

      const AuthenticateRequest* authenticaterequest = flatbuffers::GetRoot<AuthenticateRequest>(data);
      if (authenticaterequest == nullptr)
      {

        return SendErrorResponse(Message::AUTHENTICATE, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      if (authenticaterequest->username() == nullptr)
      {

        return SendErrorResponse(Message::AUTHENTICATE, sequence, Error(ErrorCode::MissingParameter, "Missing parameter username"));
      }

      if (authenticaterequest->clientnonce() == nullptr)
      {

        return SendErrorResponse(Message::AUTHENTICATE, sequence, Error(ErrorCode::MissingParameter, "Missing parameter clientnonce"));
      }

      if (authenticaterequest->authdigest() == nullptr)
      {

        return SendErrorResponse(Message::AUTHENTICATE, sequence, Error(ErrorCode::MissingParameter, "Missing parameter authdigest"));
      }

      const std::pair<Error, AUTHENTICATERESPONSE> error = Authenticate(authenticaterequest->username()->str(), authenticaterequest->clientnonce()->str(), authenticaterequest->authdigest()->str());
      if (error.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::AUTHENTICATE, sequence, error.first);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Finish(CreateAuthenticateResponse(fbb_));
      return SendResponse(false, Message::AUTHENTICATE, sequence);
    }
    case Message::CONTROLSTREAM:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ControlStreamPlayRequest>(nullptr))
      {

        return SendErrorResponse(Message::CONTROLSTREAM, sequence, Error(ErrorCode::InvalidMessage, "Invalid ControlStream message"));
      }

      const ControlStreamPlayRequest* controlstreamplayrequest = flatbuffers::GetRoot<ControlStreamPlayRequest>(data);
      if (controlstreamplayrequest == nullptr)
      {

        return SendErrorResponse(Message::CONTROLSTREAM, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = ControlStream(controlstreamplayrequest->token(), controlstreamplayrequest->playrequestindex(), controlstreamplayrequest->fetchmarker(), controlstreamplayrequest->ratecontrol(), controlstreamplayrequest->forwards(), controlstreamplayrequest->starttime() ? boost::optional<uint64_t>(controlstreamplayrequest->starttime()) : boost::none, controlstreamplayrequest->endtime() ? boost::optional<uint64_t>(controlstreamplayrequest->endtime()) : boost::none, controlstreamplayrequest->numframes() ? boost::optional<uint64_t>(controlstreamplayrequest->numframes()) : boost::none, controlstreamplayrequest->iframes());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CONTROLSTREAM, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CONTROLSTREAM, sequence));
    }
    case Message::CONTROLSTREAMFRAMESTEP:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ControlStreamFrameStepRequest>(nullptr))
      {

        return SendErrorResponse(Message::CONTROLSTREAMFRAMESTEP, sequence, Error(ErrorCode::InvalidMessage, "Invalid ControlStreamFrameStep message"));
      }

      const ControlStreamFrameStepRequest* controlstreamframesteprequest = flatbuffers::GetRoot<ControlStreamFrameStepRequest>(data);
      if (controlstreamframesteprequest == nullptr)
      {

        return SendErrorResponse(Message::CONTROLSTREAMFRAMESTEP, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = ControlStreamFrameStep(controlstreamframesteprequest->token(), controlstreamframesteprequest->playrequestindex(), controlstreamframesteprequest->forwards(), controlstreamframesteprequest->sequencenum());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CONTROLSTREAMFRAMESTEP, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CONTROLSTREAMFRAMESTEP, sequence));
    }
    case Message::CONTROLSTREAMLIVE:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ControlStreamLiveRequest>(nullptr))
      {

        return SendErrorResponse(Message::CONTROLSTREAMLIVE, sequence, Error(ErrorCode::InvalidMessage, "Invalid ControlStreamLive message"));
      }

      const ControlStreamLiveRequest* controlstreamliverequest = flatbuffers::GetRoot<ControlStreamLiveRequest>(data);
      if (controlstreamliverequest == nullptr)
      {

        return SendErrorResponse(Message::CONTROLSTREAMLIVE, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = ControlStreamLive(controlstreamliverequest->token(), controlstreamliverequest->playrequestindex());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CONTROLSTREAMLIVE, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CONTROLSTREAMLIVE, sequence));
    }
    case Message::CONTROLSTREAMPAUSE:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ControlStreamPauseRequest>(nullptr))
      {

        return SendErrorResponse(Message::CONTROLSTREAMPAUSE, sequence, Error(ErrorCode::InvalidMessage, "Invalid ControlStreamPause message"));
      }

      const ControlStreamPauseRequest* controlstreampauserequest = flatbuffers::GetRoot<ControlStreamPauseRequest>(data);
      if (controlstreampauserequest == nullptr)
      {

        return SendErrorResponse(Message::CONTROLSTREAMPAUSE, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = ControlStreamPause(controlstreampauserequest->token(), controlstreampauserequest->time() ? boost::optional<uint64_t>(controlstreampauserequest->time()) : boost::none);
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CONTROLSTREAMPAUSE, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::CONTROLSTREAMPAUSE, sequence));
    }
    case Message::CREATEFINDMOTION:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<CreateFindMotionRequest>(nullptr))
      {

        return SendErrorResponse(Message::CREATEFINDMOTION, sequence, Error(ErrorCode::InvalidMessage, "Invalid CreateFindMotionRequest message"));
      }

      const CreateFindMotionRequest* createfindmotionrequest = flatbuffers::GetRoot<CreateFindMotionRequest>(data);
      if (createfindmotionrequest == nullptr)
      {

        return SendErrorResponse(Message::CREATEFINDMOTION, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const std::pair<Error, uint64_t> findmotion = CreateFindMotion(createfindmotionrequest->recordingtoken(), createfindmotionrequest->trackid(), createfindmotionrequest->starttime(), createfindmotionrequest->endtime(), createfindmotionrequest->x(), createfindmotionrequest->y(), createfindmotionrequest->width(), createfindmotionrequest->height(), createfindmotionrequest->sensitivity(), createfindmotionrequest->fast());
      if (findmotion.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CREATEFINDMOTION, sequence, findmotion.first);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateCreateFindMotionResponse(fbb_, findmotion.second));
      return SendResponse(true, Message::CREATEFINDMOTION, sequence);
    }
    case Message::CREATEFINDOBJECT:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<CreateFindObjectRequest>(nullptr))
      {

        return SendErrorResponse(Message::CREATEFINDOBJECT, sequence, Error(ErrorCode::InvalidMessage, "Invalid CreateFindObjectRequest message"));
      }

      const CreateFindObjectRequest* createfindobjectrequest = flatbuffers::GetRoot<CreateFindObjectRequest>(data);
      if (createfindobjectrequest == nullptr)
      {

        return SendErrorResponse(Message::CREATEFINDOBJECT, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const std::pair<Error, uint64_t> findobject = CreateFindObject(createfindobjectrequest->recordingtoken(), createfindobjectrequest->trackid(), createfindobjectrequest->starttime(), createfindobjectrequest->endtime(), createfindobjectrequest->x(), createfindobjectrequest->y(), createfindobjectrequest->width(), createfindobjectrequest->height());
      if (findobject.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CREATEFINDOBJECT, sequence, findobject.first);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateCreateFindObjectResponse(fbb_, findobject.second));
      return SendResponse(true, Message::CREATEFINDOBJECT, sequence);
    }
    case Message::CREATESTREAM:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<CreateStreamRequest>(nullptr))
      {

        return SendErrorResponse(Message::CREATESTREAM, sequence, Error(ErrorCode::InvalidMessage, "Invalid CreateStream message"));
      }

      const CreateStreamRequest* createstreamrequest = flatbuffers::GetRoot<CreateStreamRequest>(data);
      if (createstreamrequest == nullptr)
      {

        return SendErrorResponse(Message::CREATESTREAM, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const std::pair<Error, STREAM> stream = CreateStream(createstreamrequest->recordingtoken(), createstreamrequest->tracktoken());
      if (stream.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::CREATESTREAM, sequence, stream.first);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      std::vector< flatbuffers::Offset<CodecIndex> > codecindices;
      for (const CODECINDEX& codecindex : stream.second.codecindices_)
      {
        codecindices.push_back(CreateCodecIndex(fbb_, codecindex.id_, codecindex.codec_, fbb_.CreateString(codecindex.parameters_), codecindex.timestamp_));

      }
      fbb_.Finish(CreateCreateStreamResponse(fbb_, stream.second.token_, fbb_.CreateVector(codecindices)));
      return SendResponse(true, Message::CREATESTREAM, sequence);
    }
    case Message::DESTROYFINDMOTION:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<DestroyFindMotionRequest>(nullptr))
      {

        return SendErrorResponse(Message::DESTROYFINDMOTION, sequence, Error(ErrorCode::InvalidMessage, "Invalid DestroyFindMotionRequest message"));
      }

      const DestroyFindMotionRequest* destroyfindmotionrequest = flatbuffers::GetRoot<DestroyFindMotionRequest>(data);
      if (destroyfindmotionrequest == nullptr)
      {

        return SendErrorResponse(Message::DESTROYFINDMOTION, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = DestroyFindMotion(destroyfindmotionrequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::DESTROYFINDMOTION, sequence, error);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      return SendResponse(true, Message::DESTROYFINDMOTION, sequence);
    }
    case Message::DESTROYFINDOBJECT:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<DestroyFindObjectRequest>(nullptr))
      {

        return SendErrorResponse(Message::DESTROYFINDOBJECT, sequence, Error(ErrorCode::InvalidMessage, "Invalid DestroyFindObjectRequest message"));
      }

      const DestroyFindObjectRequest* destroyfindobjectrequest = flatbuffers::GetRoot<DestroyFindObjectRequest>(data);
      if (destroyfindobjectrequest == nullptr)
      {

        return SendErrorResponse(Message::DESTROYFINDOBJECT, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = DestroyFindObject(destroyfindobjectrequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::DESTROYFINDOBJECT, sequence, error);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      return SendResponse(true, Message::DESTROYFINDOBJECT, sequence);
    }
    case Message::DESTROYSTREAM:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<DestroyStreamRequest>(nullptr))
      {

        return SendErrorResponse(Message::DESTROYSTREAM, sequence, Error(ErrorCode::InvalidMessage, "Invalid DestroyStreamRequest message"));
      }

      const DestroyStreamRequest* destroystreamrequest = flatbuffers::GetRoot<DestroyStreamRequest>(data);
      if (destroystreamrequest == nullptr)
      {

        return SendErrorResponse(Message::DESTROYSTREAM, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = DestroyStream(destroystreamrequest->streamtoken());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::DESTROYSTREAM, sequence, error);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      return SendResponse(true, Message::DESTROYSTREAM, sequence);
    }
    case Message::DISCOVERYBROADCAST:
    {
      const monocle::Error error = DiscoveryBroadcast();
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::DISCOVERYBROADCAST, sequence, error);
      }
      return SendResponse(true, Message::DISCOVERYBROADCAST, sequence);
    }
    case Message::GETAUTHNETICATIONNONCE:
    {
      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateGetAuthenticationNonceResponse(fbb_, fbb_.CreateString(GetAuthenticationNonce())));
      return SendResponse(false, Message::GETAUTHNETICATIONNONCE, sequence);
    }
    case Message::GETCHILDFOLDERS:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<GetChildFoldersRequest>(nullptr))
      {

        return SendErrorResponse(Message::GETCHILDFOLDERS, sequence, Error(ErrorCode::InvalidMessage, "Invalid GetChildFolders message"));
      }

      const GetChildFoldersRequest* getchildfoldersrequest = flatbuffers::GetRoot<GetChildFoldersRequest>(data);
      if (getchildfoldersrequest == nullptr)
      {

        return SendErrorResponse(Message::GETCHILDFOLDERS, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      std::string path;
      if (getchildfoldersrequest->path())
      {
        path = getchildfoldersrequest->path()->str();

      }

      const std::pair<Error, std::vector<std::string> > files = GetChildFolders(path, getchildfoldersrequest->parentpaths());
      if (files.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::GETCHILDFOLDERS, sequence, files.first);
      }
      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateGetChildFoldersResponse(fbb_, fbb_.CreateVectorOfStrings(files.second)));
      return SendResponse(true, Message::GETCHILDFOLDERS, sequence);
    }
    case Message::GETFILES:
    {
      const std::pair<Error, std::vector<FILE> > files = GetFiles();
      if (files.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::GETFILES, sequence, files.first);
      }
      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateGetFilesResponse(fbb_, fbb_.CreateVector(GetFileBuffers(files.second, fbb_))));
      return SendResponse( true, Message::GETFILES, sequence);
    }
    case Message::GETRECEIVERS:
    {
      const std::pair<Error, std::vector<RECEIVER> > receivers = GetReceivers();
      if (receivers.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::GETRECEIVERS, sequence, receivers.first);
      }
      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateGetReceiversResponse(fbb_, fbb_.CreateVector(GetReceiverBuffers(receivers.second, fbb_))));
      return SendResponse(true, Message::GETRECEIVERS, sequence);
    }
    case Message::GETRECORDING:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<GetRecordingRequest>(nullptr))
      {

        return SendErrorResponse(Message::GETRECORDING, sequence, Error(ErrorCode::InvalidMessage, "Invalid GetRecordingRequest message"));
      }

      const GetRecordingRequest* getrecordingrequest = flatbuffers::GetRoot<GetRecordingRequest>(data);
      if (getrecordingrequest == nullptr)
      {

        return SendErrorResponse(Message::GETRECORDING, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const std::pair<Error, RECORDING> recording = GetRecording(getrecordingrequest->recordingtoken());
      if (recording.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::GETRECORDING, sequence, recording.first);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      std::vector< flatbuffers::Offset<RecordingJob> > recordingjobbuffers;
      recordingjobbuffers.reserve(recording.second.jobs_.size());
      for (const RECORDINGJOB& recordingjob : recording.second.jobs_)
      {
        recordingjobbuffers.push_back(CreateRecordingJob(fbb_, recordingjob.token_, fbb_.CreateString(recordingjob.name_), recordingjob.enabled_, recordingjob.priority_, fbb_.CreateVector(GetRecordingJobSourceBuffers(recordingjob.sources_, fbb_))));

      }

      std::vector< flatbuffers::Offset<RecordingTrack> > tracks;
      tracks.reserve(recording.second.tracks_.size());
      for (const monocle::RECORDINGTRACK& track : recording.second.tracks_)
      {
        std::vector< flatbuffers::Offset<monocle::CodecIndex> > codecindices;
        codecindices.reserve(track.codecindices_.size());
        for (const monocle::CODECINDEX& codecindex : track.codecindices_)
        {
          codecindices.push_back(CreateCodecIndex(fbb_, codecindex.id_, codecindex.codec_, fbb_.CreateString(codecindex.parameters_), codecindex.timestamp_));

        }
        tracks.push_back(CreateRecordingTrack(fbb_, track.id_, fbb_.CreateString(track.token_), track.tracktype_, fbb_.CreateString(track.description_), track.fixedfiles_, track.digitalsignature_, track.encrypt_, track.flushfrequency_, fbb_.CreateVector(track.files_), fbb_.CreateVectorOfStructs(track.indices_), fbb_.CreateVector(codecindices)));
      }

      std::unique_ptr<monocle::TOKEN> activejobtoken;
      if (recording.second.activejob_.is_initialized())
      {
        activejobtoken = std::make_unique<monocle::TOKEN>(*recording.second.activejob_);
      }
      fbb_.Finish(CreateRecording(fbb_, recording.second.token_, fbb_.CreateString(recording.second.sourceid_), fbb_.CreateString(recording.second.name_), fbb_.CreateString(recording.second.location_), fbb_.CreateString(recording.second.description_), fbb_.CreateString(recording.second.address_), fbb_.CreateString(recording.second.content_), recording.second.retentiontime_, fbb_.CreateVector(recordingjobbuffers), fbb_.CreateVector(tracks), activejobtoken.get()));
      return SendResponse(true, Message::GETRECORDING, sequence);
    }
    case Message::GETRECORDINGS:
    {
      const std::pair<Error, std::vector<RECORDING> > recordings = GetRecordings();
      if (recordings.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::GETRECORDINGS, sequence, recordings.first);
      }
      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateGetRecordingsResponse(fbb_, fbb_.CreateVector(GetRecordingBuffers(recordings.second, fbb_))));
      return SendResponse(true, Message::GETRECORDINGS, sequence);
    }
    case Message::GETSNAPSHOT:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<GetSnapshotRequest>(nullptr))
      {

        return SendErrorResponse(Message::GETSNAPSHOT, sequence, Error(ErrorCode::InvalidMessage, "Invalid GetSnapshotRequest message"));
      }

      const GetSnapshotRequest* getsnapshotrequest = flatbuffers::GetRoot<GetSnapshotRequest>(data);
      if (getsnapshotrequest == nullptr)
      {

        return SendErrorResponse(Message::GETSNAPSHOT, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const std::pair<Error, SNAPSHOT> getsnapshot = GetSnapshot(getsnapshotrequest->recordingtoken(), getsnapshotrequest->recordingtrackid(), getsnapshotrequest->time(), getsnapshotrequest->x(), getsnapshotrequest->y(), getsnapshotrequest->width(), getsnapshotrequest->height());
      if (getsnapshot.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::GETSNAPSHOT, sequence, getsnapshot.first);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateGetSnapshotResponse(fbb_, fbb_.CreateVector(getsnapshot.second.data_), getsnapshot.second.time_));
      return SendResponse(true, Message::GETSNAPSHOT, sequence);
    }
    case Message::GETSTATE:
    {
      const std::pair<Error, GETSTATE> getstate = GetState();
      if (getstate.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::GETSTATE, sequence, getstate.first);
      }

      const monocle::VERSION version(getstate.second.version_.GetMajor(), getstate.second.version_.GetMinor(), getstate.second.version_.GetBuild());
      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateGetStateResponse(fbb_, fbb_.CreateString(getstate.second.name_), fbb_.CreateString(getstate.second.publickey_), fbb_.CreateString(getstate.second.architecture_), getstate.second.operatingsystem_, fbb_.CreateString(getstate.second.compiler_), fbb_.CreateString(getstate.second.databasepath_), &version, getstate.second.identifier_, fbb_.CreateVectorOfStrings(getstate.second.environmentvariables_), fbb_.CreateVectorOfStrings(getstate.second.commandlinevariables_), fbb_.CreateVector(GetONVIFUserBuffers(getstate.second.onvifusers_, fbb_)), fbb_.CreateVector(GetGroupBuffers(getstate.second.groups_, fbb_)), fbb_.CreateVector(GetUserBuffers(getstate.second.users_, fbb_)), fbb_.CreateVector(GetFileBuffers(getstate.second.files_, fbb_)), fbb_.CreateVector(GetReceiverBuffers(getstate.second.receivers_, fbb_)), fbb_.CreateVector(GetRecordingBuffers(getstate.second.recordings_, fbb_)), fbb_.CreateVector(GetLogMessagesBuffer(getstate.second.serverlogmessages_, fbb_)), getstate.second.maxrecordings_, fbb_.CreateVector(GetMapBuffers(getstate.second.maps_, fbb_)), fbb_.CreateVector(GetMountPointBuffers(getstate.second.mountpoints_, fbb_)), fbb_.CreateString(getstate.second.latitude_), fbb_.CreateString(getstate.second.longitude_)));
      return SendResponse(true, Message::GETSTATE, sequence);
    }
    case Message::GETTIME:
    {
      const uint64_t time = GetTime();
      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateGetTimeResponse(fbb_, time));
      return SendResponse(false, Message::GETTIME, sequence);
    }
    case Message::KEEPALIVE:
    {

      return SendHeaderResponse(HEADER(0, false, false, Message::KEEPALIVE, sequence));
    }
    case Message::MOUNTFILE:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<MountFileRequest>(nullptr))
      {

        return SendErrorResponse(Message::MOUNTFILE, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const MountFileRequest* mountfilerequest = flatbuffers::GetRoot<MountFileRequest>(data);
      if (mountfilerequest == nullptr)
      {

        return SendErrorResponse(Message::MOUNTFILE, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = MountFile(mountfilerequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::MOUNTFILE, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::MOUNTFILE, sequence));
    }
    case Message::REMOVEFILE:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<RemoveFileRequest>(nullptr))
      {

        return SendErrorResponse(Message::REMOVEFILE, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const RemoveFileRequest* removefilerequest = flatbuffers::GetRoot<RemoveFileRequest>(data);
      if (removefilerequest == nullptr)
      {

        return SendErrorResponse(Message::REMOVEFILE, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = RemoveFile(removefilerequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::REMOVEFILE, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::REMOVEFILE, sequence));
    }
    case Message::REMOVEGROUP:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<RemoveGroupRequest>(nullptr))
      {

        return SendErrorResponse(Message::REMOVEGROUP, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const RemoveGroupRequest* removegrouprequest = flatbuffers::GetRoot<RemoveGroupRequest>(data);
      if (removegrouprequest == nullptr)
      {

        return SendErrorResponse(Message::REMOVEGROUP, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = RemoveGroup(removegrouprequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::REMOVEGROUP, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::REMOVEGROUP, sequence));
    }
    case Message::REMOVEMAP:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<RemoveMapRequest>(nullptr))
      {

        return SendErrorResponse(Message::REMOVEMAP, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const RemoveMapRequest* removemaprequest = flatbuffers::GetRoot<RemoveMapRequest>(data);
      if (removemaprequest == nullptr)
      {

        return SendErrorResponse(Message::REMOVEMAP, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = RemoveMap(removemaprequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::REMOVEMAP, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::REMOVEMAP, sequence));
    }
    case Message::REMOVEONVIFUSER:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<RemoveONVIFUserRequest>(nullptr))
      {

        return SendErrorResponse(Message::REMOVEONVIFUSER, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const RemoveONVIFUserRequest* removeonvifuserrequest = flatbuffers::GetRoot<RemoveONVIFUserRequest>(data);
      if (removeonvifuserrequest == nullptr)
      {

        return SendErrorResponse(Message::REMOVEONVIFUSER, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = RemoveONVIFUser(removeonvifuserrequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::REMOVEONVIFUSER, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::REMOVEONVIFUSER, sequence));
    }
    case Message::REMOVERECEIVER:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<RemoveReceiverRequest>(nullptr))
      {

        return SendErrorResponse(Message::REMOVERECEIVER, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const RemoveReceiverRequest* removereceiverrequest = flatbuffers::GetRoot<RemoveReceiverRequest>(data);
      if (removereceiverrequest == nullptr)
      {

        return SendErrorResponse(Message::REMOVERECEIVER, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = RemoveReceiver(removereceiverrequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::REMOVERECEIVER, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::REMOVERECEIVER, sequence));
    }
    case Message::REMOVERECORDING:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<RemoveRecordingRequest>(nullptr))
      {

        return SendErrorResponse(Message::REMOVERECORDING, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const RemoveRecordingRequest* removerecordingrequest = flatbuffers::GetRoot<RemoveRecordingRequest>(data);
      if (removerecordingrequest == nullptr)
      {

        return SendErrorResponse(Message::REMOVERECORDING, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = RemoveRecording(removerecordingrequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::REMOVERECORDING, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::REMOVERECORDING, sequence));
    }
    case Message::REMOVERECORDINGJOB:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<RemoveRecordingJobRequest>(nullptr))
      {

        return SendErrorResponse(Message::REMOVERECORDINGJOB, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const RemoveRecordingJobRequest* removerecordingjobrequest = flatbuffers::GetRoot<RemoveRecordingJobRequest>(data);
      if (removerecordingjobrequest == nullptr)
      {

        return SendErrorResponse(Message::REMOVERECORDINGJOB, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = RemoveRecordingJob(removerecordingjobrequest->recordingtoken(), removerecordingjobrequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::REMOVERECORDINGJOB, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::REMOVERECORDINGJOB, sequence));
    }
    case Message::REMOVERECORDINGJOBSOURCE:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<RemoveRecordingJobSourceRequest>(nullptr))
      {

        return SendErrorResponse(Message::REMOVERECORDINGJOBSOURCE, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const RemoveRecordingJobSourceRequest* removerecordingjobsourcerequest = flatbuffers::GetRoot<RemoveRecordingJobSourceRequest>(data);
      if (removerecordingjobsourcerequest == nullptr)
      {

        return SendErrorResponse(Message::REMOVERECORDINGJOBSOURCE, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = RemoveRecordingJobSource(removerecordingjobsourcerequest->recordingtoken(), removerecordingjobsourcerequest->recordingjobtoken(), removerecordingjobsourcerequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::REMOVERECORDINGJOBSOURCE, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::REMOVERECORDINGJOBSOURCE, sequence));
    }
    case Message::REMOVETRACK:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<RemoveTrackRequest>(nullptr))
      {

        return SendErrorResponse(Message::REMOVETRACK, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const RemoveTrackRequest* removetrackrequest = flatbuffers::GetRoot<RemoveTrackRequest>(data);
      if (removetrackrequest == nullptr)
      {

        return SendErrorResponse(Message::REMOVETRACK, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = RemoveTrack(removetrackrequest->recordingtoken(), removetrackrequest->id());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::REMOVETRACK, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::REMOVETRACK, sequence));
    }
    case Message::REMOVETRACKS:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<RemoveTracksRequest>(nullptr))
      {

        return SendErrorResponse(Message::REMOVETRACKS, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const RemoveTracksRequest* removetracksrequest = flatbuffers::GetRoot<RemoveTracksRequest>(data);
      if (removetracksrequest == nullptr)
      {

        return SendErrorResponse(Message::REMOVETRACKS, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      std::vector<uint32_t> tracks;
      if (removetracksrequest->ids())
      {
        tracks.reserve(removetracksrequest->ids()->size());
        for (uint32_t track : *removetracksrequest->ids())
        {
          tracks.push_back(track);

        }
      }

      const Error error = RemoveTracks(removetracksrequest->recordingtoken(), tracks);
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::REMOVETRACKS, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::REMOVETRACKS, sequence));
    }
    case Message::REMOVEUSER:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<RemoveUserRequest>(nullptr))
      {

        return SendErrorResponse(Message::REMOVEUSER, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const RemoveUserRequest* removeuserrequest = flatbuffers::GetRoot<RemoveUserRequest>(data);
      if (removeuserrequest == nullptr)
      {

        return SendErrorResponse(Message::REMOVEUSER, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = RemoveUser(removeuserrequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::REMOVEUSER, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::REMOVEUSER, sequence));
    }
    case Message::SETLOCATION:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SetLocationRequest>(nullptr))
      {

        return SendErrorResponse(Message::SETLOCATION, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const SetLocationRequest* setlocationrequest = flatbuffers::GetRoot<SetLocationRequest>(data);
      if (setlocationrequest == nullptr)
      {

        return SendErrorResponse(Message::SETLOCATION, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = SetLocation(setlocationrequest->latitude() ? setlocationrequest->latitude()->str() : std::string(), setlocationrequest->longitude() ? setlocationrequest->longitude()->str() : std::string());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::SETLOCATION, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::SETLOCATION, sequence));
    }
    case Message::SETNAME:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SetNameRequest>(nullptr))
      {

        return SendErrorResponse(Message::SETNAME, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const SetNameRequest* setnamerequest = flatbuffers::GetRoot<SetNameRequest>(data);
      if (setnamerequest == nullptr)
      {

        return SendErrorResponse(Message::SETNAME, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = SetName(setnamerequest->name() ? setnamerequest->name()->str() : std::string());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::SETNAME, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::SETNAME, sequence));
    }
    case Message::SUBSCRIBE:
    {
      const std::pair<Error, SUBSCRIBE> subscribe = Subscribe();
      if (subscribe.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::SUBSCRIBE, sequence, subscribe.first);
      }

      const monocle::VERSION version(subscribe.second.version_.GetMajor(), subscribe.second.version_.GetMinor(), subscribe.second.version_.GetBuild());
      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateSubscribeResponse(fbb_, fbb_.CreateString(subscribe.second.name_), fbb_.CreateString(subscribe.second.publickey_), fbb_.CreateString(subscribe.second.architecture_), subscribe.second.operatingsystem_, fbb_.CreateString(subscribe.second.compiler_), fbb_.CreateString(subscribe.second.databasepath_), &version, subscribe.second.identifier_, fbb_.CreateVectorOfStrings(subscribe.second.environmentvariables_), fbb_.CreateVectorOfStrings(subscribe.second.commandlinevariables_), fbb_.CreateVector(GetONVIFUserBuffers(subscribe.second.onvifusers_, fbb_)), fbb_.CreateVector(GetGroupBuffers(subscribe.second.groups_, fbb_)), fbb_.CreateVector(GetUserBuffers(subscribe.second.users_, fbb_)), fbb_.CreateVector(GetFileBuffers(subscribe.second.files_, fbb_)), fbb_.CreateVector(GetReceiverBuffers(subscribe.second.receivers_, fbb_)), fbb_.CreateVector(GetRecordingBuffers(subscribe.second.recordings_, fbb_)), fbb_.CreateVector(GetLogMessagesBuffer(subscribe.second.serverlogmessages_, fbb_)), subscribe.second.maxrecordings_, fbb_.CreateVector(GetMapBuffers(subscribe.second.maps_, fbb_)), fbb_.CreateVector(GetMountPointBuffers(subscribe.second.mountpoints_, fbb_)), fbb_.CreateString(subscribe.second.latitude_), fbb_.CreateString(subscribe.second.longitude_), subscribe.second.numcudadevices_, subscribe.second.numcldevices_, subscribe.second.maxobjectdetectors_));
      return SendResponse(true, Message::SUBSCRIBE, sequence);
    }
    case Message::SUBSCRIBEDISCOVERY:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::SUBSCRIBEDISCOVERY, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SubscribeDiscoveryRequest>(nullptr))
      {

        return SendErrorResponse(Message::SUBSCRIBEDISCOVERY, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const SubscribeDiscoveryRequest* subscribediscoveryrequest = flatbuffers::GetRoot<SubscribeDiscoveryRequest>(data);
      if (subscribediscoveryrequest == nullptr)
      {

        return SendErrorResponse(Message::SUBSCRIBEDISCOVERY, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error subscribediscovery = SubscribeDiscovery(subscribediscoveryrequest->broadcast());
      if (subscribediscovery.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::SUBSCRIBEDISCOVERY, sequence, subscribediscovery);
      }
      return SendResponse(true, Message::SUBSCRIBEDISCOVERY, sequence);
    }
    case Message::SUBSCRIBEHARDWARESTATS:
    {
      const std::pair<Error, SUBSCRIBE> subscribe = Subscribe();
      if (subscribe.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::SUBSCRIBEHARDWARESTATS, sequence, subscribe.first);
      }

      const std::pair<Error, HARDWARESTATS> hardwarestats = SubscribeHardwareStats();
      if (hardwarestats.first.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::SUBSCRIBEHARDWARESTATS, sequence, hardwarestats.first);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateSubscribeHardwareStatsResponse(fbb_, CreateHardwareStats(fbb_, hardwarestats.second.time_, fbb_.CreateVector(GetDiskStatsBuffer(hardwarestats.second.diskstats_, fbb_)), hardwarestats.second.cpuusage_, hardwarestats.second.totalmemory_, hardwarestats.second.availablememory_)));
      return SendResponse(false, Message::SUBSCRIBEHARDWARESTATS, sequence);
    }
    case Message::SUBSCRIBERECORDINGJOBLOG:
    {
      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SubscribeRecordingJobLogRequest>(nullptr))
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGJOBLOG, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const SubscribeRecordingJobLogRequest* subscriberecordingjoblogrequest = flatbuffers::GetRoot<SubscribeRecordingJobLogRequest>(data);
      if (subscriberecordingjoblogrequest == nullptr)
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGJOBLOG, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const std::pair<Error, SUBSCRIBERECORDINGJOBLOG> subscribesereverjoblog = SubscribeRecordingJobLog(subscriberecordingjoblogrequest->recordingtoken());
      if (subscribesereverjoblog.first.code_ != monocle::ErrorCode::Success)
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGJOBLOG, sequence, subscribesereverjoblog.first);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();

      std::vector< flatbuffers::Offset<monocle::RecordingJobLogMessages> > recordingjobslogmessages;
      recordingjobslogmessages.reserve(subscribesereverjoblog.second.recordingjoblogmessages_.size());
      for (const std::pair<uint64_t, std::vector<monocle::LOGMESSAGE> >& recordingjoblogmessage : subscribesereverjoblog.second.recordingjoblogmessages_)
      {
        std::vector< flatbuffers::Offset<monocle::LogMessage> > logmessages;
        logmessages.reserve(recordingjoblogmessage.second.size());
        for (const monocle::LOGMESSAGE& recordingjoblogmessage : recordingjoblogmessage.second)
        {
          logmessages.push_back(CreateLogMessage(fbb_, recordingjoblogmessage.time_, recordingjoblogmessage.severity_, fbb_.CreateString(recordingjoblogmessage.message_)));

        }
        recordingjobslogmessages.push_back(CreateRecordingJobLogMessages(fbb_, recordingjoblogmessage.first, fbb_.CreateVector(logmessages)));
      }
      fbb_.Finish(CreateSubscribeRecordingJobLogResponse(fbb_, fbb_.CreateVector(recordingjobslogmessages)));
      return SendResponse(true, Message::SUBSCRIBERECORDINGJOBLOG, sequence);
    }
    case Message::SUBSCRIBERECORDINGJOBSOURCETRACKLOG:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGJOBSOURCETRACKLOG, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SubscribeRecordingJobSourceTrackLogRequest>(nullptr))
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGJOBSOURCETRACKLOG, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const SubscribeRecordingJobSourceTrackLogRequest* subscriberecordingjobsourcetracklogrequest = flatbuffers::GetRoot<SubscribeRecordingJobSourceTrackLogRequest>(data);
      if (subscriberecordingjobsourcetracklogrequest == nullptr)
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGJOBSOURCETRACKLOG, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const std::pair<monocle::Error, SUBSCRIBERECORDINGJOBSOURCETRACKLOG> subscribesereverjoblog = SubscribeRecordingJobSourceTrackLog(subscriberecordingjobsourcetracklogrequest->recordingtoken());
      if (subscribesereverjoblog.first.code_ != monocle::ErrorCode::Success)
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGJOBSOURCETRACKLOG, sequence, subscribesereverjoblog.first);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();

      std::vector< flatbuffers::Offset<monocle::RecordingJobSourceTrackLogMessages> > recordingjobsourcetrackslogmessages;
      recordingjobsourcetrackslogmessages.reserve(subscribesereverjoblog.second.recordingjobsourcetracklogmessages_.size());
      for (const std::pair<SUBSCRIBERECORDINGJOBSOURCETRACKLOG::RECORDINGJOBSOURCETRACKTOKEN, std::vector<monocle::LOGMESSAGE> >& recordingjobsourcetracklogmessage : subscribesereverjoblog.second.recordingjobsourcetracklogmessages_)
      {
        std::vector< flatbuffers::Offset<monocle::LogMessage> > logmessages;
        logmessages.reserve(recordingjobsourcetracklogmessage.second.size());
        for (const monocle::LOGMESSAGE& recordingjobsourcetracklogmessage : recordingjobsourcetracklogmessage.second)
        {
          logmessages.push_back(CreateLogMessage(fbb_, recordingjobsourcetracklogmessage.time_, recordingjobsourcetracklogmessage.severity_, fbb_.CreateString(recordingjobsourcetracklogmessage.message_)));

        }
        recordingjobsourcetrackslogmessages.push_back(CreateRecordingJobSourceTrackLogMessages(fbb_, recordingjobsourcetracklogmessage.first.recordingjobtoken_, recordingjobsourcetracklogmessage.first.recordingjobsourcetoken_, recordingjobsourcetracklogmessage.first.recordingjobsourcetracktoken_, fbb_.CreateVector(logmessages)));
      }
      fbb_.Finish(CreateSubscribeRecordingJobSourceTrackLogResponse(fbb_, fbb_.CreateVector(recordingjobsourcetrackslogmessages)));
      return SendResponse(true, Message::SUBSCRIBERECORDINGJOBSOURCETRACKLOG, sequence);
    }
    case Message::SUBSCRIBERECORDINGLOG:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGLOG, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SubscribeRecordingLogRequest>(nullptr))
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGLOG, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const SubscribeRecordingLogRequest* subscriberecordinglogrequest = flatbuffers::GetRoot<SubscribeRecordingLogRequest>(data);
      if (subscriberecordinglogrequest == nullptr)
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGLOG, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const std::pair<monocle::Error, monocle::SUBSCRIBERECORDINGLOG> subscriberecordinglog = SubscribeRecordingLog(subscriberecordinglogrequest->token());
      if (subscriberecordinglog.first.code_ != monocle::ErrorCode::Success)
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGLOG, sequence, Error(ErrorCode::InvalidToken, "Invalid token"));
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();
      fbb_.Finish(CreateSubscribeRecordingLogResponse(fbb_, fbb_.CreateVector(GetLogMessagesBuffer(subscriberecordinglog.second.messages_, fbb_))));
      return SendResponse(true, Message::SUBSCRIBERECORDINGLOG, sequence);
    }
    case Message::SUBSCRIBERECORDINGTRACKLOG:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGTRACKLOG, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<SubscribeRecordingLogRequest>(nullptr))
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGTRACKLOG, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const SubscribeRecordingTrackLogRequest* subscriberecordingtracklogrequest = flatbuffers::GetRoot<SubscribeRecordingTrackLogRequest>(data);
      if (subscriberecordingtracklogrequest == nullptr)
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGTRACKLOG, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const std::pair<Error, SUBSCRIBERECORDINGTRACKLOG> subscribeserevertracklog = SubscribeRecordingTrackLog(subscriberecordingtracklogrequest->recordingtoken());
      if (subscribeserevertracklog.first.code_ != monocle::ErrorCode::Success)
      {

        return SendErrorResponse(Message::SUBSCRIBERECORDINGTRACKLOG, sequence, subscribeserevertracklog.first);
      }

      std::lock_guard<std::mutex> lock(writemutex_);
      fbb_.Clear();

      std::vector< flatbuffers::Offset<monocle::RecordingTrackLogMessages> > recordingtrackslogmessages;
      recordingtrackslogmessages.reserve(subscribeserevertracklog.second.recordingtracklogmessages_.size());
      for (const std::pair<uint32_t, std::vector<monocle::LOGMESSAGE> >& recordingtracklogmessage : subscribeserevertracklog.second.recordingtracklogmessages_)
      {
        std::vector< flatbuffers::Offset<monocle::LogMessage> > logmessages;
        logmessages.reserve(recordingtracklogmessage.second.size());
        for (const monocle::LOGMESSAGE& recordingtracklogmessage : recordingtracklogmessage.second)
        {
          logmessages.push_back(CreateLogMessage(fbb_, recordingtracklogmessage.time_, recordingtracklogmessage.severity_, fbb_.CreateString(recordingtracklogmessage.message_)));

        }
        recordingtrackslogmessages.push_back(CreateRecordingTrackLogMessages(fbb_, recordingtracklogmessage.first, fbb_.CreateVector(logmessages)));
      }
      fbb_.Finish(CreateSubscribeRecordingTrackLogResponse(fbb_, fbb_.CreateVector(recordingtrackslogmessages)));
      return SendResponse(true, Message::SUBSCRIBERECORDINGTRACKLOG, sequence);
    }
    case Message::UNMOUNTFILE:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::UNMOUNTFILE, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<UnmountFileRequest>(nullptr))
      {

        return SendErrorResponse(Message::UNMOUNTFILE, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const UnmountFileRequest* unmountfilerequest = flatbuffers::GetRoot<UnmountFileRequest>(data);
      if (unmountfilerequest == nullptr)
      {

        return SendErrorResponse(Message::UNMOUNTFILE, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      const Error error = UnmountFile(unmountfilerequest->token());
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::UNMOUNTFILE, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::UNMOUNTFILE, sequence));
    }
    case Message::UNSUBSCRIBEDISCOVERY:
    {
      const Error unsubscribediscovery = UnsubscribeDiscovery();
      if (unsubscribediscovery.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::UNSUBSCRIBEDISCOVERY, sequence, unsubscribediscovery);
      }
      return SendResponse(true, Message::UNSUBSCRIBEDISCOVERY, sequence);
    }
    case Message::UNSUBSCRIBEHARDWARESTATS:
    {
      const Error error = UnsubscribeHardwareStats();
      if (error.code_ != ErrorCode::Success)
      {

        return SendErrorResponse(Message::UNSUBSCRIBEHARDWARESTATS, sequence, error);
      }

      return SendHeaderResponse(HEADER(0, false, false, Message::UNSUBSCRIBEHARDWARESTATS, sequence));
    }
    case Message::UNSUBSCRIBERECORDINGJOBLOG:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::UNSUBSCRIBERECORDINGJOBLOG, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<UnsubscribeRecordingJobLogRequest>(nullptr))
      {

        return SendErrorResponse(Message::UNSUBSCRIBERECORDINGJOBLOG, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const UnsubscribeRecordingJobLogRequest* unsubscriberecordingjoblogrequest = flatbuffers::GetRoot<UnsubscribeRecordingJobLogRequest>(data);
      if (unsubscriberecordingjoblogrequest == nullptr)
      {

        return SendErrorResponse(Message::UNSUBSCRIBERECORDINGJOBLOG, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      UnsubscribeRecordingJobLog(unsubscriberecordingjoblogrequest->token());
      return SendHeaderResponse(HEADER(0, false, false, Message::UNSUBSCRIBERECORDINGJOBLOG, sequence));
    }
    case Message::UNSUBSCRIBERECORDINGJOBSOURCETRACKLOG:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::UNSUBSCRIBERECORDINGJOBSOURCETRACKLOG, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<UnsubscribeRecordingJobSourceTrackLogRequest>(nullptr))
      {

        return SendErrorResponse(Message::UNSUBSCRIBERECORDINGJOBSOURCETRACKLOG, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const UnsubscribeRecordingJobSourceTrackLogRequest* unsubscriberecordingjobsourcetracklogrequest = flatbuffers::GetRoot<UnsubscribeRecordingJobSourceTrackLogRequest>(data);
      if (unsubscriberecordingjobsourcetracklogrequest == nullptr)
      {

        return SendErrorResponse(Message::UNSUBSCRIBERECORDINGJOBSOURCETRACKLOG, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      UnsubscribeRecordingJobSourceTrackLog(unsubscriberecordingjobsourcetracklogrequest->recordingtoken());
      return SendHeaderResponse(HEADER(0, false, false, Message::UNSUBSCRIBERECORDINGJOBSOURCETRACKLOG, sequence));
    }
    case Message::UNSUBSCRIBERECORDINGLOG:
    {
      if (data == nullptr)
      {

        return SendErrorResponse(Message::UNSUBSCRIBERECORDINGLOG, sequence, Error(ErrorCode::MissingMessage, "Missing message"));
      }

      if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<UnsubscribeRecordingJobSourceTrackLogRequest>(nullptr))
      {

        return SendErrorResponse(Message::UNSUBSCRIBERECORDINGLOG, sequence, Error(ErrorCode::InvalidMessage, "Invalid message"));
      }

      const UnsubscribeRecordingLogRequest* unsubscriberecordinglogrequest = flatbuffers::GetRoot<UnsubscribeRecordingLogRequest>(data);
      if (unsubscriberecordinglogrequest == nullptr)
      {

        return SendErrorResponse(Message::UNSUBSCRIBERECORDINGLOG, sequence, Error(ErrorCode::MissingParameter, "Invalid message"));
      }

      UnsubscribeRecordingLog(unsubscriberecordinglogrequest->token());
      return SendHeaderResponse(HEADER(0, false, false, Message::UNSUBSCRIBERECORDINGLOG, sequence));
    }
    default:
    {

      return SendErrorResponse(message, sequence, Error(ErrorCode::UnrecognisedMessageID, "Unrecognised MessageID: " + std::to_string(static_cast<int>(message))));
    }
  }
}

boost::system::error_code Connection::SendHeaderResponse(const HEADER& header)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  boost::system::error_code err;
  boost::asio::write(socket_, boost::asio::buffer(&header, sizeof(HEADER)), boost::asio::transfer_all(), err);
  if (err)
  {
    DestroyConnection();
    return err;
  }
  return err;
}

boost::system::error_code Connection::SendErrorResponse(const Message message, const uint16_t sequence, const Error& error)
{
  std::lock_guard<std::mutex> lock(writemutex_);
  fbb_.Finish(CreateErrorResponse(fbb_, error.code_, fbb_.CreateString(error.text_)));
  const uint32_t messagesize = static_cast<uint32_t>(fbb_.GetSize());
  const HEADER header(messagesize, true, false, message, sequence);
  const std::array<boost::asio::const_buffer, 4> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(fbb_.GetBufferPointer(), messagesize)
  };
  boost::system::error_code err;
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

boost::system::error_code Connection::SendResponse(const bool compress, const monocle::Message message, const uint16_t sequence)
{
  const char* buffer = reinterpret_cast<const char*>(fbb_.GetBufferPointer());
  size_t size = fbb_.GetSize();
  if (compress)
  {
    if (fbb_.GetSize())
    {
      if (utility::Compress(deflate_, reinterpret_cast<const char*>(fbb_.GetBufferPointer()), fbb_.GetSize(), zbuffer_))
      {

        return SendErrorResponse(message, sequence, monocle::Error(monocle::ErrorCode::InternalError, "Compression failed"));
      }
      buffer = zbuffer_.data();
      size = zbuffer_.size();
    }
    else
    {

      return SendHeaderResponse(HEADER(0, false, false, message, sequence)); // No additional data, so we're ok
    }
  }

  const HEADER header(static_cast<uint32_t>(size), false, compress, message, sequence);
  boost::system::error_code err;
  const std::array<boost::asio::const_buffer, 4> buffers =
  {
    boost::asio::const_buffer(&header, sizeof(HEADER)),
    boost::asio::const_buffer(buffer, size)
  };
  boost::asio::write(socket_, buffers, boost::asio::transfer_all(), err);
  return err;
}

void Connection::DestroyConnection()
{
  if (!server_)
  {

    return;
  }
  server_->DestroyConnection(shared_from_this());
}

void Connection::InitCompression()
{
  deflate_.zalloc = Z_NULL;
  deflate_.zfree = Z_NULL;
  deflate_.opaque = Z_NULL;
  if (deflateInit(&deflate_, Z_DEFAULT_COMPRESSION) != Z_OK)
  {
    // We make a fairly safe assumption that this will not fail

  }

  inflate_.zalloc = Z_NULL;
  inflate_.zfree = Z_NULL;
  inflate_.opaque = Z_NULL;
  inflate_.avail_in = 0;
  inflate_.next_in = Z_NULL;
  if (inflateInit(&inflate_) != Z_OK)
  {
    // We make a fairly safe assumption that this will not fail

  }
}

}

}
