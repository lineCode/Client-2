// monocleprotocol.hpp
//

///// Includes /////

#include "monocleprotocol/monocleprotocol.hpp"

#include <boost/algorithm/string.hpp>
#include <utility/utility.hpp>

///// Defines /////

#undef CreateFile

///// Namespaces /////

namespace monocle
{

///// Functions /////
  
boost::optional<TrackType> GetTrackType(const std::string& tracktype)
{
  if (boost::iequals(tracktype, "Video"))
  {

    return TrackType::Video;
  }
  else if (boost::iequals(tracktype, "Audio"))
  {

    return TrackType::Audio;
  }
  else if (boost::iequals(tracktype, "Metadata"))
  {

    return TrackType::Metadata;
  }
  else if (boost::iequals(tracktype, "ObjectDetector"))
  {

    return TrackType::ObjectDetector;
  }
  else
  {
  
    return boost::none;
  }
}

std::string AuthenticateDigest(const std::string& username, const std::string& password)
{
  return utility::Md5(username + std::string(":monocle:") + password);
}

std::string AuthenticateDigest(const std::string& username, const std::string& password, const std::string& nonce, const std::string& clientnonce)
{
  return utility::Md5(AuthenticateDigest(username, password) + std::string(":") + nonce + std::string(":") + clientnonce + std::string(":"));
}

std::string AuthenticateDigest(const std::string& digest, const std::string& nonce, const std::string& clientnonce)
{
  return utility::Md5(digest + std::string(":") + nonce + std::string(":") + clientnonce + std::string(":"));
}

std::vector< flatbuffers::Offset<ONVIFUser> > GetONVIFUserBuffers(const std::vector<ONVIFUSER>& onvifusers, flatbuffers::FlatBufferBuilder& fbb)
{
  std::vector< flatbuffers::Offset<ONVIFUser> > userbuffers;
  userbuffers.reserve(onvifusers.size());
  for (const ONVIFUSER& onvifuser : onvifusers)
  {
    userbuffers.push_back(CreateONVIFUser(fbb, onvifuser.token_, fbb.CreateString(onvifuser.username_), onvifuser.userlevel_));

  }
  return userbuffers;
}

std::vector< flatbuffers::Offset<DiskStat> > GetDiskStatsBuffer(const std::vector<DISKSTAT>& diskstats, flatbuffers::FlatBufferBuilder& fbb)
{
  std::vector< flatbuffers::Offset<DiskStat> > diskstatsbuffer;
  diskstatsbuffer.reserve(diskstats.size());
  for (const DISKSTAT& diskstat : diskstats)
  {
    diskstatsbuffer.push_back(CreateDiskStat(fbb, fbb.CreateString(diskstat.device_), diskstat.utility_, diskstat.readspeed_, diskstat.writespeed_));

  }
  return diskstatsbuffer;
}

std::vector< flatbuffers::Offset<Group> > GetGroupBuffers(const std::vector<GROUP>& groups, flatbuffers::FlatBufferBuilder& fbb)
{
  std::vector< flatbuffers::Offset<Group> > groupbuffers;
  groupbuffers.reserve(groups.size());
  for (const GROUP& group : groups)
  {
    groupbuffers.push_back(CreateGroup(fbb, group.token_, fbb.CreateString(group.name_), group.manageusers_, group.managerecordings_, group.allrecordings_, fbb.CreateVector(group.recordings_), group.managemaps_, group.managedevice_));

  }
  return groupbuffers;
}

std::vector< flatbuffers::Offset<User> > GetUserBuffers(const std::vector<USER>& users, flatbuffers::FlatBufferBuilder& fbb)
{
  std::vector< flatbuffers::Offset<User> > userbuffers;
  userbuffers.reserve(users.size());
  for (const USER& user : users)
  {
    userbuffers.push_back(CreateUser(fbb, user.token_, fbb.CreateString(user.username_), user.grouptoken_));

  }
  return userbuffers;
}

std::vector< flatbuffers::Offset<File> > GetFileBuffers(const std::vector<FILE>& files, flatbuffers::FlatBufferBuilder& fbb)
{
  std::vector< flatbuffers::Offset<File> > filebuffers;
  filebuffers.reserve(files.size());
  for (const FILE& file : files)
  {
    filebuffers.push_back(CreateFile(fbb, file.token_, fbb.CreateString(file.path_), fbb.CreateString(file.mountpoint_), file.numchunks_, file.chunksize_, file.automount_, file.state_, file.monitorstate_));

  }
  return filebuffers;
}

std::vector< flatbuffers::Offset<Receiver> > GetReceiverBuffers(const std::vector<RECEIVER>& receivers, flatbuffers::FlatBufferBuilder& fbb)
{
  std::vector< flatbuffers::Offset<Receiver> > receiverbuffers;
  receiverbuffers.reserve(receivers.size());
  for (const RECEIVER& receiver : receivers)
  {
    receiverbuffers.push_back(CreateReceiver(fbb, receiver.token_, receiver.mode_, fbb.CreateString(receiver.mediauri_), receiver.autocreated_, fbb.CreateString(receiver.username_), fbb.CreateString(receiver.password_), fbb.CreateVectorOfStrings(receiver.parameters_), receiver.state_));

  }
  return receiverbuffers;
}

std::vector< flatbuffers::Offset<Recording> > GetRecordingBuffers(const std::vector<RECORDING>& recordings, flatbuffers::FlatBufferBuilder& fbb)
{
  std::vector< flatbuffers::Offset<Recording> > recordingbuffers;
  recordingbuffers.reserve(recordings.size());
  for (const RECORDING& recording : recordings)
  {
    std::vector< flatbuffers::Offset<RecordingJob> > recordingjobbuffers;
    recordingjobbuffers.reserve(recording.jobs_.size());
    for (const RECORDINGJOB& recordingjob : recording.jobs_)
    {
      recordingjobbuffers.push_back(CreateRecordingJob(fbb, recordingjob.token_, fbb.CreateString(recordingjob.name_), recordingjob.enabled_, recordingjob.priority_, fbb.CreateVector(GetRecordingJobSourceBuffers(recordingjob.sources_, fbb))));

    }

    std::vector< flatbuffers::Offset<RecordingTrack> > tracks;
    tracks.reserve(recording.tracks_.size());
    for (const monocle::RECORDINGTRACK& track : recording.tracks_)
    {
      std::vector< flatbuffers::Offset<monocle::CodecIndex> > codecindices;
      codecindices.reserve(track.codecindices_.size());
      for (const monocle::CODECINDEX& codecindex : track.codecindices_)
      {
        codecindices.push_back(CreateCodecIndex(fbb, codecindex.id_, codecindex.codec_, fbb.CreateString(codecindex.parameters_), codecindex.timestamp_));

      }
      tracks.push_back(CreateRecordingTrack(fbb, track.id_, fbb.CreateString(track.token_), track.tracktype_, fbb.CreateString(track.description_), track.fixedfiles_, track.digitalsignature_, track.encrypt_, track.flushfrequency_, fbb.CreateVector(track.files_), fbb.CreateVectorOfStructs(track.indices_), fbb.CreateVector(codecindices), track.totaltrackdata_.first, track.totaltrackdata_.second));
    }

    std::unique_ptr<monocle::TOKEN> activejob;
    if (recording.activejob_.is_initialized())
    {
      activejob = std::make_unique<monocle::TOKEN>(*recording.activejob_);

    }

    recordingbuffers.push_back(CreateRecording(fbb, recording.token_, fbb.CreateString(recording.sourceid_), fbb.CreateString(recording.name_), fbb.CreateString(recording.location_), fbb.CreateString(recording.description_), fbb.CreateString(recording.address_), fbb.CreateString(recording.content_), recording.retentiontime_, fbb.CreateVector(recordingjobbuffers), fbb.CreateVector(tracks), activejob.get()));
  }
  return recordingbuffers;
}

std::vector< flatbuffers::Offset<RecordingJobSource> > GetRecordingJobSourceBuffers(const std::vector<RECORDINGJOBSOURCE>& recordingjobsources, flatbuffers::FlatBufferBuilder& fbb)
{
  std::vector<flatbuffers::Offset<RecordingJobSource>> recordingjobsourcebuffers;
  recordingjobsourcebuffers.reserve(recordingjobsources.size());
  for (const RECORDINGJOBSOURCE& recordingjobsource : recordingjobsources)
  {
    std::vector< flatbuffers::Offset<RecordingJobSourceTrack> > recordingjobsourcetrackbuffers;
    recordingjobsourcetrackbuffers.reserve(recordingjobsource.tracks_.size());
    for (const RECORDINGJOBSOURCETRACK& recordingjobsourcetrack : recordingjobsource.tracks_)
    {
      recordingjobsourcetrackbuffers.push_back(CreateRecordingJobSourceTrack(fbb, recordingjobsourcetrack.token_, recordingjobsourcetrack.trackid_, fbb.CreateVectorOfStrings(recordingjobsourcetrack.parameters_), recordingjobsourcetrack.state_, fbb.CreateString(recordingjobsourcetrack.error_), fbb.CreateVectorOfStrings(recordingjobsourcetrack.activeparameters_)));

    }

    recordingjobsourcebuffers.push_back(CreateRecordingJobSource(fbb, recordingjobsource.token_, recordingjobsource.type_, recordingjobsource.receivertoken_, fbb.CreateVector(recordingjobsourcetrackbuffers)));
  }
  return recordingjobsourcebuffers;
}

std::vector < flatbuffers::Offset<LogMessage> > GetLogMessagesBuffer(const std::vector<LOGMESSAGE>& logmessages, flatbuffers::FlatBufferBuilder& fbb)
{
  std::vector < flatbuffers::Offset<LogMessage> > logmessagesbuffer;
  logmessagesbuffer.reserve(logmessages.size());
  for (const LOGMESSAGE& logmessage : logmessages)
  {
    logmessagesbuffer.push_back(CreateLogMessage(fbb, logmessage.time_, logmessage.severity_, fbb.CreateString(logmessage.message_)));

  }
  return logmessagesbuffer;
}

std::vector< flatbuffers::Offset<Map> > GetMapBuffers(const std::vector<MAP>& maps, flatbuffers::FlatBufferBuilder& fbb)
{
  std::vector < flatbuffers::Offset<Map> > mapsbuffer;
  mapsbuffer.reserve(maps.size());
  for (const MAP& map : maps)
  {
    mapsbuffer.push_back(CreateMap(fbb, map.token_, fbb.CreateString(map.name_), fbb.CreateString(map.location_), fbb.CreateString(map.imagemd5_)));

  }
  return mapsbuffer;
}

std::vector< flatbuffers::Offset<MountPoint> > GetMountPointBuffers(const std::vector<MOUNTPOINT>& mountpoints, flatbuffers::FlatBufferBuilder& fbb)
{
  std::vector < flatbuffers::Offset<MountPoint> > mountpointsbuffer;
  mountpointsbuffer.reserve(mountpoints.size());
  for (const MOUNTPOINT& mountpoint : mountpoints)
  {
    mountpointsbuffer.push_back(CreateMountPoint(fbb, mountpoint.id_, mountpoint.parentid_, mountpoint.majorstdev_, mountpoint.minorstdev_, fbb.CreateString(mountpoint.path_), fbb.CreateString(mountpoint.type_), fbb.CreateString(mountpoint.source_)));

  }
  return mountpointsbuffer;
}

///// Methods /////

AUTHENTICATERESPONSE::AUTHENTICATERESPONSE()
{

}

DISKSTAT::DISKSTAT(const std::string& device, const double utility, const double readspeed, const double writespeed) :
  device_(device),
  utility_(utility),
  readspeed_(readspeed),
  writespeed_(writespeed)
{

}

Error::Error() :
  code_(ErrorCode::Success)
{

}

Error::Error(const ErrorCode code, const std::string& text) :
  code_(code),
  text_(text)
{

}

GPUSTAT::GPUSTAT(const std::string& uuid, const std::string& name, const unsigned int gpuusage, const unsigned int memoryusage, const unsigned long long freememory, const unsigned long long totalmemory, const unsigned long long usedmemory) :
  uuid_(uuid),
  name_(name),
  gpuusage_(gpuusage),
  memoryusage_(memoryusage),
  freememory_(freememory),
  totalmemory_(totalmemory),
  usedmemory_(usedmemory)
{

}


GETSTATE::GETSTATE()
{

}

GETSTATE::GETSTATE(const std::string& name, const std::string& publickey, const std::string& architecture, const int operatingsystem, const std::string& compiler, const std::string& databasepath, const utility::Version& version, const uint64_t identifier, const std::vector<std::string>& environmentvariables, const std::vector<std::string>& commandlinevariables, const std::vector<ONVIFUSER>& onvifusers, const std::vector<GROUP>& groups, const std::vector<USER>& users, const std::vector<FILE>& files, const std::vector<RECEIVER>& receivers, const std::vector<RECORDING>& recordings, const std::vector<LOGMESSAGE>& serverlogmessages, const uint32_t maxrecordings, const std::vector<MAP>& maps, const std::vector<MOUNTPOINT>& mountpoints, const std::string& latitude, const std::string& longitude) :
  name_(name),
  publickey_(publickey),
  architecture_(architecture),
  operatingsystem_(operatingsystem),
  compiler_(compiler),
  databasepath_(databasepath),
  version_(version),
  identifier_(identifier),
  environmentvariables_(environmentvariables),
  commandlinevariables_(commandlinevariables),
  onvifusers_(onvifusers),
  groups_(groups),
  users_(users),
  files_(files),
  receivers_(receivers),
  recordings_(recordings),
  serverlogmessages_(serverlogmessages),
  maxrecordings_(maxrecordings),
  maps_(maps),
  mountpoints_(mountpoints),
  latitude_(latitude),
  longitude_(longitude)
{

}

HARDWARESTATS::HARDWARESTATS(const uint64_t time, const std::vector<DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory, const uint64_t availablememory, const std::vector<GPUSTAT>& gpustats) :
  time_(time),
  diskstats_(diskstats),
  cpuusage_(cpuusage),
  totalmemory_(totalmemory),
  availablememory_(availablememory),
  gpustats_(gpustats)
{

}

HARDWARESTATS::HARDWARESTATS() :
  time_(0),
  cpuusage_(0.0),
  totalmemory_(0),
  availablememory_(0)
{

}

LOGMESSAGE::LOGMESSAGE(const uint64_t time, const monocle::Severity severity, const std::string& message) :
  time_(time),
  severity_(severity),
  message_(message)
{

}

bool LOGMESSAGE::operator==(const LOGMESSAGE& rhs) const
{
  return ((time_ == rhs.time_) && (severity_ == rhs.severity_) && (message_ == rhs.message_));
}

ONVIFUSER::ONVIFUSER(const uint64_t token, const std::string& username, const ONVIFUserlevel userlevel) :
  token_(token),
  username_(username),
  userlevel_(userlevel)
{

}

bool ONVIFUSER::operator==(const ONVIFUSER& rhs) const
{
  return ((token_ == rhs.token_) && (username_ == rhs.username_) && (userlevel_ == rhs.userlevel_));
}

GROUP::GROUP(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings) :
  token_(token),
  name_(name),
  manageusers_(manageusers),
  managerecordings_(managerecordings),
  managemaps_(managemaps),
  managedevice_(managedevice),
  allrecordings_(allrecordings),
  recordings_(recordings)
{

}

bool GROUP::operator==(const GROUP& rhs) const
{
  return ((token_ == rhs.token_) && (name_ == rhs.name_) && (manageusers_ == rhs.manageusers_) && (managerecordings_ == rhs.managerecordings_) && (managemaps_ == rhs.managemaps_) && (managedevice_ == rhs.managedevice_) && (allrecordings_ == rhs.allrecordings_) && std::is_permutation(recordings_.cbegin(), recordings_.cend(), rhs.recordings_.cbegin(), rhs.recordings_.cend()));
}

MAP::MAP(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5) :
  token_(token),
  name_(name),
  location_(location),
  imagemd5_(imagemd5)
{

}

bool MAP::operator==(const MAP& rhs) const
{
  return ((token_ == rhs.token_) && (name_ == rhs.name_) && (location_ == rhs.location_) && (imagemd5_ == rhs.imagemd5_));
}

TRACKSTATISTICS::TRACKSTATISTICS(const uint32_t trackid, const uint64_t trackdatareceived) :
  trackid_(trackid),
  trackdatareceived_(trackdatareceived)
{

}

bool TRACKSTATISTICS::operator==(const TRACKSTATISTICS& rhs) const
{
  return ((trackid_ == rhs.trackid_) && (trackdatareceived_ == rhs.trackdatareceived_));
}

RECORDINGSTATISTICS::RECORDINGSTATISTICS(const uint64_t token, const std::vector<TRACKSTATISTICS>& tracksstatistics) :
  token_(token),
  tracksstatistics_(tracksstatistics)
{

}

RECORDINGSTATISTICS::RECORDINGSTATISTICS() :
  token_(0)
{

}

bool RECORDINGSTATISTICS::operator==(const RECORDINGSTATISTICS& rhs) const
{
  return ((token_ == rhs.token_) && (tracksstatistics_ == rhs.tracksstatistics_));
}


USER::USER(const uint64_t token, const std::string& username, const uint64_t grouptoken) :
  token_(token),
  username_(username),
  grouptoken_(grouptoken)
{

}

bool USER::operator==(const USER& rhs) const
{
  return ((token_ == rhs.token_) && (username_ == rhs.username_) && (grouptoken_ == rhs.grouptoken_));
}

FILE::FILE(const uint64_t token, const std::string& path, const std::string& mountpoint, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate) :
  token_(token),
  path_(path),
  mountpoint_(mountpoint),
  numchunks_(numchunks),
  chunksize_(chunksize),
  automount_(automount),
  state_(state),
  monitorstate_(monitorstate)
{

}

bool FILE::operator==(const FILE& rhs) const
{
  return ((token_ == rhs.token_) && (path_ == rhs.path_) && (mountpoint_ == rhs.mountpoint_) && (numchunks_ == rhs.numchunks_) && (chunksize_ == rhs.chunksize_) && (automount_ == rhs.automount_) && (state_ == rhs.state_) && (monitorstate_ == rhs.monitorstate_));
}

HEADER::HEADER(const uint32_t size, const bool error, const bool compressed, const Message message, const uint16_t sequence) :
  size_(size),
  flags_((error ? static_cast<uint32_t>(monocle::HeaderFlags::FLAG_ERROR) : 0) | (compressed ? static_cast<uint32_t>(monocle::HeaderFlags::FLAG_COMPRESSED) : 0)),
  message_(message),
  sequence_(sequence)
{

}

ADDRECORDINGJOBSOURCE::ADDRECORDINGJOBSOURCE(const uint32_t recordingtrackid, const monocle::ReceiverMode receivermode, const std::vector<std::string>& sourceparameters, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& recordingjobsourcetrackparameters) :
  recordingtrackid_(recordingtrackid),
  receivermode_(receivermode),
  sourceparameters_(sourceparameters),
  mediauri_(mediauri),
  username_(username),
  password_(password),
  recordingjobsourcetrackparameters_(recordingjobsourcetrackparameters)
{

}

bool ADDRECORDINGJOBSOURCE::operator==(const ADDRECORDINGJOBSOURCE& rhs) const
{
  return ((recordingtrackid_ == rhs.recordingtrackid_) && (receivermode_ == rhs.receivermode_) && std::is_permutation(sourceparameters_.cbegin(), sourceparameters_.cend(), rhs.sourceparameters_.cbegin(), rhs.sourceparameters_.cend()) && (mediauri_ == rhs.mediauri_) && (username_ == rhs.username_) && (password_ == rhs.password_) && std::is_permutation(recordingjobsourcetrackparameters_.cbegin(), recordingjobsourcetrackparameters_.cend(), rhs.recordingjobsourcetrackparameters_.cbegin(), rhs.recordingjobsourcetrackparameters_.cend()));
}

CHANGERECORDINGJOBSOURCE::CHANGERECORDINGJOBSOURCE(const uint32_t recordingtrackid, const monocle::ReceiverMode receivermode, const std::vector<std::string>& sourceparameters, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& recordingjobsourcetrackparameters) :
  recordingtrackid_(recordingtrackid),
  receivermode_(receivermode),
  sourceparameters_(sourceparameters),
  mediauri_(mediauri),
  username_(username),
  password_(password),
  recordingjobsourcetrackparameters_(recordingjobsourcetrackparameters)
{

}

bool CHANGERECORDINGJOBSOURCE::operator==(const CHANGERECORDINGJOBSOURCE& rhs) const
{
  return ((recordingtrackid_ == rhs.recordingtrackid_) && (receivermode_ == rhs.receivermode_) && std::is_permutation(sourceparameters_.cbegin(), sourceparameters_.cend(), rhs.sourceparameters_.cbegin(), rhs.sourceparameters_.cend()) && (mediauri_ == rhs.mediauri_) && (username_ == rhs.username_) && (password_ == rhs.password_) && std::is_permutation(recordingjobsourcetrackparameters_.cbegin(), recordingjobsourcetrackparameters_.cend(), rhs.recordingjobsourcetrackparameters_.cbegin(), rhs.recordingjobsourcetrackparameters_.cend()));
}

RECEIVER::RECEIVER(const uint64_t token, const ReceiverMode mode, const std::string& mediauri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, const ReceiverState state) :
  token_(token),
  mode_(mode),
  mediauri_(mediauri),
  autocreated_(autocreated),
  username_(username),
  password_(password),
  parameters_(parameters),
  state_(state)
{

}

bool RECEIVER::operator==(const RECEIVER& rhs) const
{
  return ((token_ == rhs.token_) && (mode_ == rhs.mode_) && (mediauri_ == rhs.mediauri_) && (autocreated_ == rhs.autocreated_) && (username_ == rhs.username_) && (password_ == rhs.password_) && (state_ == rhs.state_));
}

RECORDINGJOBSOURCETRACK::RECORDINGJOBSOURCETRACK(const uint64_t token, const uint64_t trackid, const std::vector<std::string>& parameters, const RecordingJobState state, const std::string& error, const std::vector<std::string>& activeparameters) :
  token_(token),
  trackid_(trackid),
  parameters_(parameters),
  state_(state),
  error_(error),
  activeparameters_(activeparameters)
{

}

bool RECORDINGJOBSOURCETRACK::operator==(const RECORDINGJOBSOURCETRACK& rhs) const
{
  return ((token_ == rhs.token_) && (trackid_ == rhs.trackid_) && std::is_permutation(parameters_.cbegin(), parameters_.cend(), rhs.parameters_.cbegin(), rhs.parameters_.cend()) && (state_ == rhs.state_) && (error_ == rhs.error_) && std::is_permutation(activeparameters_.cbegin(), activeparameters_.cend(), rhs.activeparameters_.cbegin(), rhs.activeparameters_.cend()));
}

RECORDINGJOBSOURCE::RECORDINGJOBSOURCE(const uint64_t token, const RecordingJobSourceType type, const uint64_t receivertoken, const std::vector<RECORDINGJOBSOURCETRACK>& tracks) :
  token_(token),
  type_(type),
  receivertoken_(receivertoken),
  tracks_(tracks)
{

}

bool RECORDINGJOBSOURCE::operator==(const RECORDINGJOBSOURCE& rhs) const
{
  return ((token_ == rhs.token_) && (type_ == rhs.type_) && (receivertoken_ == rhs.receivertoken_) && std::is_permutation(tracks_.cbegin(), tracks_.cend(), rhs.tracks_.cbegin(), rhs.tracks_.cend()));
}

RECORDINGJOB::RECORDINGJOB(const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<RECORDINGJOBSOURCE> sources) :
  token_(token),
  name_(name),
  enabled_(enabled),
  priority_(priority),
  sources_(sources)
{

}

bool RECORDINGJOB::operator==(const RECORDINGJOB& rhs) const
{
  return ((token_ == rhs.token_) && (enabled_ == rhs.enabled_) && (priority_ == rhs.priority_) && std::is_permutation(sources_.cbegin(), sources_.cend(), rhs.sources_.cbegin(), rhs.sources_.cend()));
}

CODECINDEX::CODECINDEX(const uint64_t id, const Codec codec, const std::string& parameters, const uint64_t timestamp) :
  id_(id),
  codec_(codec),
  parameters_(parameters),
  timestamp_(timestamp)
{

}

std::vector<std::string> CODECINDEX::GetParameters() const
{
  std::vector<std::string> parameters;
  boost::split(parameters, parameters_, boost::is_any_of(";"), boost::algorithm::token_compress_on);
  return parameters;
}

bool CODECINDEX::operator==(const CODECINDEX& rhs) const
{
  return ((id_ == rhs.id_) && (codec_ == rhs.codec_) && (parameters_ == rhs.parameters_) && (timestamp_ == rhs.timestamp_));
}

RECORDINGTRACK::RECORDINGTRACK() :
  id_(-1),
  tracktype_(TrackType::Video),
  fixedfiles_(false),
  digitalsignature_(false),
  encrypt_(false),
  flushfrequency_(0)
{

}

RECORDINGTRACK::RECORDINGTRACK(const uint32_t id, const std::string& token, const TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsignature, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<INDEX>& indices, const std::vector<CODECINDEX>& codecindices, const std::pair<uint64_t, uint64_t>& totaltrackdata) :
  id_(id),
  token_(token),
  tracktype_(tracktype),
  description_(description),
  fixedfiles_(fixedfiles),
  digitalsignature_(digitalsignature),
  encrypt_(encrypt),
  flushfrequency_(flushfrequency),
  files_(files),
  indices_(indices),
  codecindices_(codecindices),
  totaltrackdata_(totaltrackdata)
{

}

bool RECORDINGTRACK::operator==(const RECORDINGTRACK& rhs) const
{
  return ((id_ == rhs.id_) && (token_ == rhs.token_) && (tracktype_ == rhs.tracktype_) && (description_ == rhs.description_) && (fixedfiles_ == rhs.fixedfiles_) && (digitalsignature_ == rhs.digitalsignature_) && (encrypt_ == rhs.encrypt_) && (flushfrequency_ == rhs.flushfrequency_) && std::is_permutation(files_.cbegin(), files_.cend(), rhs.files_.cbegin(), rhs.files_.cend()) && std::is_permutation(codecindices_.cbegin(), codecindices_.cend(), rhs.codecindices_.cbegin(), rhs.codecindices_.cend()) && (totaltrackdata_ == rhs.totaltrackdata_));
}

RECORDING::RECORDING() :
  token_(-1),
  retentiontime_(0)
{

}

RECORDING::RECORDING(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const std::vector<RECORDINGJOB>& jobs, const std::vector<RECORDINGTRACK>& tracks, const boost::optional<uint64_t>& activejob) :
  token_(token),
  sourceid_(sourceid),
  name_(name),
  location_(location),
  description_(description),
  address_(address),
  content_(content),
  retentiontime_(retentiontime),
  jobs_(jobs),
  tracks_(tracks),
  activejob_(activejob)
{

}

bool RECORDING::operator==(const RECORDING& rhs) const
{
  return ((token_ == rhs.token_) && (sourceid_ == rhs.sourceid_) && (name_ == rhs.name_) && (location_ == rhs.location_) && (description_ == rhs.description_) && (address_ == rhs.address_) && (content_ == rhs.content_) && (retentiontime_ == rhs.retentiontime_) && std::is_permutation(jobs_.cbegin(), jobs_.cend(), rhs.jobs_.cbegin(), rhs.jobs_.cend()) && (tracks_ == rhs.tracks_));
}

RECORDINGLOGMESSAGE::RECORDINGLOGMESSAGE(const uint64_t token, const LOGMESSAGE& logmessage) :
  token_(token),
  logmessage_(logmessage)
{

}

MOUNTPOINT::MOUNTPOINT() :
  id_(0),
  parentid_(0),
  majorstdev_(0),
  minorstdev_(0)
{

}

MOUNTPOINT::MOUNTPOINT(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source) :
  id_(id),
  parentid_(parentid),
  majorstdev_(majorstdev),
  minorstdev_(minorstdev),
  path_(path),
  type_(type),
  source_(source)
{

}

bool MOUNTPOINT::operator==(const MOUNTPOINT& rhs) const
{
  return ((id_ == rhs.id_) && (parentid_ == rhs.parentid_) && (majorstdev_ == rhs.majorstdev_) && (minorstdev_ == rhs.minorstdev_) && (path_ == rhs.path_) && (type_ == rhs.type_) && (source_ == rhs.source_));
}

SUBSCRIBE::SUBSCRIBE()
{

}

SUBSCRIBE::SUBSCRIBE(const std::string& name, const std::string& publickey, const std::string& architecture, const int operatingsystem, const std::string& compiler, const std::string& databasepath, const utility::Version& version, const uint64_t identifier, const std::vector<std::string>& environmentvariables, const std::vector<std::string>& commandlinevariables, const std::vector<ONVIFUSER>& onvifusers, const std::vector<GROUP>& groups, const std::vector<USER>& users, const std::vector<FILE>& files, const std::vector<RECEIVER>& receivers, const std::vector<RECORDING>& recordings, const std::vector<LOGMESSAGE>& serverlogmessages, const uint32_t maxrecordings, const std::vector<MAP>& maps, const std::vector<MOUNTPOINT>& mountpoints, const std::string& latitude, const std::string& longitude, const unsigned int numcudadevices, const unsigned int numcldevices, const uint32_t maxobjectdetectors) :
  name_(name),
  publickey_(publickey),
  architecture_(architecture),
  operatingsystem_(operatingsystem),
  compiler_(compiler),
  databasepath_(databasepath),
  version_(version),
  identifier_(identifier),
  environmentvariables_(environmentvariables),
  commandlinevariables_(commandlinevariables),
  onvifusers_(onvifusers),
  groups_(groups),
  users_(users),
  files_(files),
  receivers_(receivers),
  recordings_(recordings),
  serverlogmessages_(serverlogmessages),
  maxrecordings_(maxrecordings),
  maps_(maps),
  mountpoints_(mountpoints),
  latitude_(latitude),
  longitude_(longitude),
  numcudadevices_(numcudadevices),
  numcldevices_(numcldevices),
  maxobjectdetectors_(maxobjectdetectors)
{

}

SUBSCRIBERECORDINGJOBLOG::SUBSCRIBERECORDINGJOBLOG()
{

}

SUBSCRIBERECORDINGJOBLOG::SUBSCRIBERECORDINGJOBLOG(const std::vector< std::pair<uint64_t, std::vector<LOGMESSAGE> > >& recordingjoblogmessages) :
  recordingjoblogmessages_(recordingjoblogmessages)
{

}


SUBSCRIBERECORDINGJOBSOURCETRACKLOG::RECORDINGJOBSOURCETRACKTOKEN::RECORDINGJOBSOURCETRACKTOKEN(const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken) :
  recordingjobtoken_(recordingjobtoken),
  recordingjobsourcetoken_(recordingjobsourcetoken),
  recordingjobsourcetracktoken_(recordingjobsourcetracktoken)
{

}

SUBSCRIBERECORDINGJOBSOURCETRACKLOG::SUBSCRIBERECORDINGJOBSOURCETRACKLOG()
{

}

SUBSCRIBERECORDINGJOBSOURCETRACKLOG::SUBSCRIBERECORDINGJOBSOURCETRACKLOG(const std::vector< std::pair<RECORDINGJOBSOURCETRACKTOKEN, std::vector<LOGMESSAGE> > >& recordingjobsourcetracklogmessages) :
  recordingjobsourcetracklogmessages_(recordingjobsourcetracklogmessages)
{

}

SUBSCRIBERECORDINGLOG::SUBSCRIBERECORDINGLOG()
{

}

SUBSCRIBERECORDINGLOG::SUBSCRIBERECORDINGLOG(const std::vector<LOGMESSAGE>& messages) :
  messages_(messages)
{

}

SUBSCRIBERECORDINGTRACKLOG::SUBSCRIBERECORDINGTRACKLOG()
{

}

SUBSCRIBERECORDINGTRACKLOG::SUBSCRIBERECORDINGTRACKLOG(const std::vector< std::pair<uint32_t, std::vector<LOGMESSAGE> > >& recordingtracklogmessages) :
  recordingtracklogmessages_(recordingtracklogmessages)
{

}

}
