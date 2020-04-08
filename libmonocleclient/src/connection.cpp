// connection.cpp
//

///// Includes /////

#include "monocleclient/connection.h"

///// Namespaces /////

namespace client
{

///// Methods /////

GUIORDER::GUIORDER() :
  token_(0),
  order_(0)
{

}

GUIORDER::GUIORDER(const uint64_t token, const uint64_t order) :
  token_(token),
  order_(order)
{

}

DATASNAPSHOT::DATASNAPSHOT() :
  time_(0),
  totaldata_(0)
{

}

DATASNAPSHOT::DATASNAPSHOT(const uint64_t time, const uint64_t totaldata) :
  time_(time),
  totaldata_(totaldata)
{

}

Connection::Connection(boost::asio::io_service& io, const sock::ProxyParams& proxyparams, const QString& address, const uint16_t port) :
  Client(io),
  proxyparams_(proxyparams),
  address_(address),
  port_(port)
{
  
}

Connection::~Connection()
{
  Destroy();

}

void Connection::Destroy()
{
  streams_.clear();
  Client::Destroy();
}

void Connection::FileAdded(const uint64_t token, const std::string& mountpoint, const std::string& path, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate)
{
  emit SignalFileAdded(token, mountpoint, path, numchunks, chunksize, automount, state, monitorstate);

}

void Connection::FileRemoved(const uint64_t token)
{
  emit SignalFileRemoved(token);

}

void Connection::FileMonitorStateChanged(const uint64_t token, const monocle::FileMonitorState monitorstate)
{
  emit SignalFileMonitorStateChanged(token, monitorstate);

}

void Connection::FileStateChanged(const uint64_t token, const monocle::FileState state)
{
  emit SignalFileStateChanged(token, state);

}

void Connection::FindMotionEnd(const uint64_t token, const uint64_t ret)
{
  emit SignalFindMotionEnd(token, ret);

}

void Connection::FindMotionProgress(const uint64_t token, const float progress)
{
  emit SignalFindMotionProgress(token, progress);

}

void Connection::FindMotionResult(const uint64_t token, const uint64_t start, const uint64_t end)
{
  emit SignalFindMotionResult(token, start, end);

}

void Connection::FindObjectEnd(const uint64_t token, const uint64_t ret)
{
  emit SignalFindObjectEnd(token, ret);

}

void Connection::FindObjectProgress(const uint64_t token, const float progress)
{
  emit SignalFindObjectProgress(token, progress);

}

void Connection::FindObjectResult(const uint64_t token, const uint64_t start, const uint64_t end, const monocle::ObjectClass objectclass, const uint64_t id, const uint64_t largesttime, const float largestx, const float largesty, const float largestwidth, const float largestheight)
{
  emit SignalFindObjectResult(token, start, end, objectclass, id, largesttime, largestx, largesty, largestwidth, largestheight);

}

void Connection::Goodbye()
{
  Disconnected();

}

void Connection::GroupAdded(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{
  emit SignalGroupAdded(token, QString::fromStdString(name), manageusers, managerecordings, managemaps, managedevice, allrecordings, recordings);

}

void Connection::GroupChanged(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{
  emit SignalGroupChanged(token, QString::fromStdString(name), manageusers, managerecordings, managemaps, managedevice, allrecordings, recordings);

}

void Connection::GroupRemoved(const uint64_t token)
{
  emit SignalGroupRemoved(token);

}

void Connection::GuiOrderChanged(const std::vector< std::pair<uint64_t, uint64_t> >& recordingsorder, const std::vector< std::pair<uint64_t, uint64_t> >& mapsorder)
{
  std::vector<GUIORDER> guiorderrecordings;
  guiorderrecordings.reserve(recordingsorder.size());
  for (const std::pair<uint64_t, uint64_t>& recordingorder : recordingsorder)
  {
    guiorderrecordings.push_back(GUIORDER(recordingorder.first, recordingorder.second));

  }

  std::vector<GUIORDER> guiordermaps;
  guiordermaps.reserve(mapsorder.size());
  for (const std::pair<uint64_t, uint64_t>& maporder : mapsorder)
  {
    guiordermaps.push_back(GUIORDER(maporder.first, maporder.second));

  }

  emit SignalGuiOrderChanged(guiorderrecordings, guiordermaps);
}

void Connection::ControlStreamEnd(const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error)
{
  std::vector<Stream>::iterator stream = std::find_if(streams_.begin(), streams_.end(), [streamtoken](const Stream& stream) { return (stream.GetToken() == streamtoken); });
  if (stream == streams_.end())
  {
    // Just ignore
    return;
  }
  stream->ControlStreamEnd(playrequestindex, error);
}

void Connection::Disconnected()
{
  streams_.clear();

}

void Connection::DiscoveryHello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes)
{
  emit SignalDiscoveryHello(addresses, scopes);

}

void Connection::H265Frame(const uint64_t token, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size)
{
  std::vector<Stream>::iterator stream = std::find_if(streams_.begin(), streams_.end(), [token](const Stream& stream) { return (stream.GetToken() == token); });
  if (stream == streams_.end())
  {
    // Just ignore
    return;
  }
  stream->H265Frame(playrequestindex, codecindex, marker, timestamp, sequencenum, progress, signature, signaturesize, donlfield, offsets, numoffsets, data, size);
}

void Connection::H264Frame(const uint64_t token, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size)
{
  std::vector<Stream>::iterator stream = std::find_if(streams_.begin(), streams_.end(), [token](const Stream& stream) { return (stream.GetToken() == token); });
  if (stream == streams_.end())
  {
    // Just ignore
    return;
  }
  stream->H264Frame(playrequestindex, codecindex, marker, timestamp, sequencenum, progress, signature, signaturesize, offsets, numoffsets, data, size);
}

void Connection::HardwareStatsMessage(const uint64_t time, const std::vector<monocle::DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory, const uint64_t availablememory, const std::vector<monocle::GPUSTAT>& gpustats)
{
  emit SignalHardwareStats(time, diskstats, cpuusage, totalmemory, availablememory, gpustats);

}

void Connection::JPEGFrame(const uint64_t token, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* data, const size_t size)
{
  std::vector<Stream>::iterator stream = std::find_if(streams_.begin(), streams_.end(), [token](const Stream& stream) { return (stream.GetToken() == token); });
  if (stream == streams_.end())
  {
    // Just ignore
    return;
  }
  stream->JPEGFrame(playrequestindex, codecindex, timestamp, sequencenum, progress, signature, signaturesize, restartinterval, typespecificfragmentoffset, type, q, width, height, lqt, cqt, data, size);
}

void Connection::LayoutAdded(const monocle::LAYOUT& layout)
{
  emit SignalLayoutAdded(layout);

}

void Connection::LayoutChanged(const monocle::LAYOUT& layout)
{
  emit SignalLayoutChanged(layout);

}

void Connection::LayoutNameChanged(const uint64_t token, const std::string& name)
{
  emit SignalLayoutNameChanged(token, QString::fromStdString(name));

}

void Connection::LayoutRemoved(const uint64_t token)
{
  emit SignalLayoutRemoved(token);

}

void Connection::MapAdded(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5)
{
  emit SignalMapAdded(token, QString::fromStdString(name), QString::fromStdString(location), QString::fromStdString(imagemd5));

}

void Connection::MapChanged(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5)
{
  emit SignalMapChanged(token, QString::fromStdString(name), QString::fromStdString(location), QString::fromStdString(imagemd5));

}

void Connection::MapRemoved(const uint64_t token)
{
  emit SignalMapRemoved(token);

}

void Connection::MetadataFrame(const uint64_t token, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* data, const size_t size)
{
  std::vector<Stream>::iterator stream = std::find_if(streams_.begin(), streams_.end(), [token](const Stream& stream) { return (stream.GetToken() == token); });
  if (stream == streams_.end())
  {
    // Just ignore
    return;
  }
  stream->MetadataFrame(playrequestindex, codecindex, timestamp, sequencenum, progress, signature, signaturesize, metadataframetype, data, size);
}

void Connection::MountPointAdded(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source)
{
  emit SignalMountPointAdded(id, parentid, majorstdev, minorstdev, QString::fromStdString(path), QString::fromStdString(type), QString::fromStdString(source));

}

void Connection::MountPointRemoved(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source)
{
  emit SignalMountPointRemoved(id, parentid, majorstdev, minorstdev, QString::fromStdString(path), QString::fromStdString(type), QString::fromStdString(source));

}

void Connection::MPEG4Frame(const uint64_t token, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* data, const size_t size)
{
  std::vector<Stream>::iterator stream = std::find_if(streams_.begin(), streams_.end(), [token](const Stream& stream) { return (stream.GetToken() == token); });
  if (stream == streams_.end())
  {
    // Just ignore
    return;
  }
  stream->MPEG4Frame(playrequestindex, codecindex, marker, timestamp, sequencenum, progress, signature, signaturesize, data, size);
}

void Connection::LocationChanged(const std::string& latitude, const std::string& longitude)
{
  emit SignalLocationChanged(QString::fromStdString(latitude), QString::fromStdString(longitude));

}

void Connection::NameChanged(const std::string& name)
{
  emit SignalNameChanged(QString::fromStdString(name));

}

void Connection::NewCodecIndex(const uint64_t token, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp)
{
  std::vector<Stream>::iterator stream = std::find_if(streams_.begin(), streams_.end(), [token](const Stream& stream) { return (stream.GetToken() == token); });
  if (stream == streams_.end())
  {
    // Just ignore
    return;
  }
  stream->NewCodecIndex(id, codec, parameters, timestamp);
}

void Connection::ObjectDetectorFrame(const uint64_t token, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::ObjectDetectorFrameType objectdetectorframetype, const char* data, const size_t size)
{
  std::vector<Stream>::iterator stream = std::find_if(streams_.begin(), streams_.end(), [token](const Stream& stream) { return (stream.GetToken() == token); });
  if (stream == streams_.end())
  {
    // Just ignore
    return;
  }
  stream->ObjectDetectorFrame(playrequestindex, codecindex, timestamp, sequencenum, progress, signature, signaturesize, objectdetectorframetype, data, size);
}

void Connection::ONVIFUserAdded(const uint64_t token, const std::string& username, const monocle::ONVIFUserlevel onvifuserlevel)
{
  emit SignalONVIFUserAdded(token, QString::fromStdString(username), onvifuserlevel);

}

void Connection::ONVIFUserChanged(const uint64_t token, const boost::optional<std::string>& username, const monocle::ONVIFUserlevel onvifuserlevel)
{
  boost::optional<QString> un;
  if (username.is_initialized())
  {
    un = QString::fromStdString(*username);

  }

  emit SignalONVIFUserChanged(token, un, onvifuserlevel);
}

void Connection::ONVIFUserRemoved(const uint64_t token)
{
  emit SignalONVIFUserRemoved(token);

}

void Connection::ReceiverAdded(const uint64_t token, const monocle::ReceiverMode mode, const std::string& mediauri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, const monocle::ReceiverState state)
{
  emit SignalReceiverAdded(token, mode, QString::fromStdString(mediauri), autocreated, QString::fromStdString(username), QString::fromStdString(password), ConvertStrings(parameters), state);

}

void Connection::ReceiverChanged(const uint64_t token, const monocle::ReceiverMode mode, const std::string& mediauri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters)
{
  emit SignalReceiverChanged(token, mode, QString::fromStdString(mediauri), autocreated, QString::fromStdString(username), QString::fromStdString(password), ConvertStrings(parameters));

}

void Connection::ReceiverRemoved(const uint64_t token)
{
  emit SignalReceiverRemoved(token);

}

void Connection::RecordingActiveJobChanged(const uint64_t recordingtoken, const boost::optional<uint64_t>& activejob)
{
  emit SignalRecordingActiveJobChanged(recordingtoken, activejob);

}

void Connection::RecordingAdded(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob)
{
  emit SignalRecordingAdded(token, sourceid, name, location, description, address, content, retentiontime, activejob);

}

void Connection::RecordingChanged(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob)
{
  emit SignalRecordingChanged(token, sourceid, name, location, description, address, content, retentiontime, activejob);

}

void Connection::RecordingJobAdded(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const monocle::RecordingJobState state)
{
  emit SignalRecordingJobAdded(recordingtoken, token, name, enabled, priority, state);

}

void Connection::RecordingJobChanged(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority)
{
  emit SignalRecordingJobChanged(recordingtoken, token, name, enabled, priority);

}

void Connection::RecordingJobLogMessage(const uint64_t recordingtoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message)
{
  emit SignalRecordingJobLogMessage(recordingtoken, token, time, severity, QString::fromStdString(message));

}

void Connection::RecordingJobSourceTrackActiveParametersChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const std::vector<std::string>& activeparameters)
{
  emit SignalRecordingJobSourceTrackActiveParametersChanged(recording, recordingjob, recordingjobsource, recordingjobsourcetrack, ConvertStrings(activeparameters));

}

void Connection::RecordingJobSourceTrackStateChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const uint64_t time, const monocle::RecordingJobState state, const std::string& error)
{
  emit SignalRecordingJobSourceTrackStateChanged(recording, recordingjob, recordingjobsource, recordingjobsourcetrack, time, state, QString::fromStdString(error));

}

void Connection::RecordingJobRemoved(const uint64_t recordingtoken, const uint64_t token)
{
  emit SignalRecordingJobRemoved(recordingtoken, token);

}

void Connection::RecordingJobSourceAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token, const uint64_t receivertoken)
{
  emit SignalRecordingJobSourceAdded(recordingtoken, recordingjobtoken, token, receivertoken);
  
}

void Connection::RecordingJobSourceRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token)
{
  emit SignalRecordingJobSourceRemoved(recordingtoken, recordingjobtoken, token);

}

void Connection::RecordingJobSourceTrackAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint32_t recordingtrackid, const std::vector<std::string>& parameters, const monocle::RecordingJobState state, const std::string& error, const std::vector<std::string>& activeparameters)
{
  emit SignalRecordingJobSourceTrackAdded(recordingtoken, recordingjobtoken, recordingjobsourcetoken, token, recordingtrackid, ConvertStrings(parameters), state, QString::fromStdString(error), ConvertStrings(activeparameters));

}

void Connection::RecordingJobSourceTrackLogMessage(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message)
{
  emit SignalRecordingJobSourceTrackLogMessage(recordingtoken, recordingjobtoken, recordingjobsourcetoken, token, time, severity, QString::fromStdString(message));

}

void Connection::RecordingJobSourceTrackRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token)
{
  emit SignalRecordingJobSourceTrackRemoved(recordingtoken, recordingjobtoken, recordingjobsourcetoken, token);

}

void Connection::RecordingRemoved(const uint64_t token)
{
  emit SignalRecordingRemoved(token);

}

void Connection::RecordingsStatistics(const uint64_t time, const std::vector<monocle::RECORDINGSTATISTICS>& recordingsstatistics)
{
  emit SignalRecordingStatistics(time, recordingsstatistics);

}

void Connection::RecordingLogMessage(const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message)
{
  emit SignalRecordingLogMessage(token, time, severity, QString::fromStdString(message));

}

void Connection::RecordingTrackCodecAdded(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp)
{
  emit SignalRecordingTrackCodecAdded(recordingtoken, recordingtrackid, id, codec, parameters, timestamp);

}

void Connection::RecordingTrackCodecRemoved(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id)
{
  emit SignalRecordingTrackCodecRemoved(recordingtoken, recordingtrackid, id);

}

void Connection::RecordingTrackLogMessage(const uint64_t recordingtoken, const uint32_t id, const uint64_t time, const monocle::Severity severity, const std::string& message)
{
  emit SignalRecordingTrackLogMessage(recordingtoken, id, time, severity, QString::fromStdString(message));

}

void Connection::ServerLogMessage(const uint64_t time, const monocle::Severity severity, const std::string& message)
{
  emit SignalServerLogMessage(time, severity, QString::fromStdString(message));

}

void Connection::TrackAdded(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<monocle::CODECINDEX>& codecindices, const std::vector< std::pair<uint64_t, uint64_t> >& totaltrackdata)
{
  std::vector<client::DATASNAPSHOT> totaltrackdatas;
  totaltrackdatas.reserve(totaltrackdata.size());
  for (const std::pair<uint64_t, uint64_t>& i : totaltrackdata)
  {
    totaltrackdatas.push_back(client::DATASNAPSHOT(i.first, i.second));

  }
  emit SignalTrackAdded(recordingtoken, id, token, tracktype, description, fixedfiles, digitalsigning, encrypt, flushfrequency, files, codecindices, totaltrackdatas);
}

void Connection::TrackChanged(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<monocle::CODECINDEX>& codecindices, const std::vector< std::pair<uint64_t, uint64_t> >& totaltrackdata)
{
  std::vector<client::DATASNAPSHOT> totaltrackdatas;
  totaltrackdatas.reserve(totaltrackdata.size());
  for (const std::pair<uint64_t, uint64_t>& i : totaltrackdata)
  {
    totaltrackdatas.push_back(client::DATASNAPSHOT(i.first, i.second));

  }
  emit SignalTrackChanged(recordingtoken, id, token, tracktype, description, fixedfiles, digitalsigning, encrypt, flushfrequency, files, codecindices, totaltrackdatas);
}

void Connection::TrackDeleteData(const uint64_t recording, const uint32_t trackid, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end)
{
  emit SignalTrackDeleteData(recording, trackid, start, end);

}

void Connection::TrackSetData(const uint64_t recording, const uint32_t trackid, const std::vector<monocle::INDEX>& indices)
{
  emit SignalTrackSetData(recording, trackid, indices);

}

void Connection::TrackRemoved(const uint64_t recordingtoken, const uint32_t token)
{
  emit SignalTrackRemoved(recordingtoken, token);

}

void Connection::UserAdded(const uint64_t token, const std::string& username, const uint64_t group)
{
  emit SignalUserAdded(token, QString::fromStdString(username), group);

}

void Connection::UserChanged(const uint64_t token, const uint64_t group)
{
  emit SignalUserChanged(token, group);

}

void Connection::UserRemoved(const uint64_t token)
{
  emit SignalUserRemoved(token);

}

sock::Connection Connection::Connect(const boost::function<void(const boost::system::error_code&)> callback)
{

  return Client::Init(proxyparams_, address_.toStdString(), port_, true, callback);
}

monocle::client::Connection Connection::Authenticate(const std::string& username, const std::string& clientnonce, const std::string& authdigest, boost::function<void(const std::chrono::steady_clock::duration, const monocle::client::AUTHENTICATERESPONSE&)> callback)
{

  return Client::Authenticate(username, clientnonce, authdigest, callback);
}

monocle::client::Connection Connection::CreateStream(const uint64_t recordingtoken, const uint32_t trackid, boost::function<void(const std::chrono::steady_clock::duration, const monocle::client::CREATESTREAMRESPONSE&)> callback, const CONTROLSTREAMEND controlstreamendcallback, const H265CALLBACK h265callback, const H264CALLBACK h264callback, const METADATACALLBACK metadatacallback, const JPEGCALLBACK jpegcallback, const MPEG4CALLBACK mpeg4callback, const OBJECTDETECTORCALLBACK objectdetectorcallback, const NEWCODECINDEX newcodecindexcallback, void* callbackdata)
{
  return Client::CreateStream(recordingtoken, trackid, [this, callback, controlstreamendcallback, h265callback, h264callback, metadatacallback, jpegcallback, mpeg4callback, objectdetectorcallback, newcodecindexcallback, callbackdata](const std::chrono::steady_clock::duration latency, const monocle::client::CREATESTREAMRESPONSE& createstreamresponse)
  {
    if (createstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      callback(latency, createstreamresponse);
      return;
    }

    // Intercept the response and create our own stream
    Stream stream(createstreamresponse.token_, controlstreamendcallback, h265callback, h264callback, metadatacallback, jpegcallback, mpeg4callback, objectdetectorcallback, newcodecindexcallback, callbackdata);
    streams_.push_back(stream);
    callback(latency, createstreamresponse);
  });
}

monocle::client::Connection Connection::DestroyStream(const uint64_t streamtoken, boost::function<void(const std::chrono::steady_clock::duration, const monocle::client::DESTROYSTREAMRESPONSE&)> callback)
{
  std::vector<Stream>::iterator i = std::find_if(streams_.begin(), streams_.end(), [streamtoken](const Stream& stream) { return (stream.GetToken() == streamtoken); });
  if (i != streams_.end())
  {
    streams_.erase(i);

  }

  return Client::DestroyStream(streamtoken, [this, callback](const std::chrono::steady_clock::duration latency, const monocle::client::DESTROYSTREAMRESPONSE& destroystreamresponse)
  {
    if (destroystreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      callback(latency, destroystreamresponse);
      return;
    }

    callback(latency, destroystreamresponse);
  });
}

monocle::client::Connection Connection::GetAuthenticationNonce(boost::function<void(const std::chrono::steady_clock::duration latency, const monocle::client::GETAUTHENTICATIONNONCERESPONSE&)> callback)
{
  return Client::GetAuthenticationNonce(callback);
}

monocle::client::Connection Connection::RemoveFile(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const monocle::client::REMOVEFILERESPONSE&)> callback)
{
  return Client::RemoveFile(token, callback);
}

monocle::client::Connection Connection::Subscribe(boost::function<void(const std::chrono::steady_clock::duration latency, const monocle::client::SUBSCRIBERESPONSE&)> callback)
{
  return Client::Subscribe(callback);
}


std::vector<QString> Connection::ConvertStrings(const std::vector<std::string>& strings) const
{
  std::vector<QString> result;
  result.reserve(static_cast<int>(strings.size()));
  for (const std::string& s : strings)
  {
    result.push_back(QString::fromStdString(s));

  }
  return result;
}

}
