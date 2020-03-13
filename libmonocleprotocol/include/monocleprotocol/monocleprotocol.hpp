// monocleprotocol.hpp
//

#ifndef IDF69P3OAV5ULY5O12ABC6VTGJT0STS5UA
#define IDF69P3OAV5ULY5O12ABC6VTGJT0STS5UA

///// Includes /////

#include <array>
#include <boost/optional.hpp>
#include <chrono>
#include <stdint.h>
#include <utility/version.hpp>
#include <zlib.h>

#include "monocleprotocol/codec_generated.h"
#include "monocleprotocol/diskstat_generated.h"
#include "monocleprotocol/errorcode_generated.h"
#include "monocleprotocol/file_generated.h"
#include "monocleprotocol/filestate_generated.h"
#include "monocleprotocol/filemonitorstate_generated.h"
#include "monocleprotocol/group_generated.h"
#include "monocleprotocol/header_generated.h"
#include "monocleprotocol/logmessage_generated.h"
#include "monocleprotocol/onvifuser_generated.h"
#include "monocleprotocol/map_generated.h"
#include "monocleprotocol/message_generated.h"
#include "monocleprotocol/mountpoint_generated.h"
#include "monocleprotocol/receiver_generated.h"
#include "monocleprotocol/receivermode_generated.h"
#include "monocleprotocol/receiverstate_generated.h"
#include "monocleprotocol/recording_generated.h"
#include "monocleprotocol/recordingjobstate_generated.h"
#include "monocleprotocol/recordingjobsourcetrack_generated.h"
#include "monocleprotocol/recordingjobsourcetype_generated.h"
#include "monocleprotocol/severity_generated.h"
#include "monocleprotocol/tracktype_generated.h"
#include "monocleprotocol/user_generated.h"
#include "monocleprotocol/onvifuserlevel_generated.h"

///// Namespaces /////

namespace monocle
{

///// Structures /////

struct AUTHENTICATERESPONSE
{
  AUTHENTICATERESPONSE();

};

struct DISKSTAT
{
  DISKSTAT(const std::string& device, const double utility, const double readspeed, const double writespeed);

  std::string device_;
  double utility_;
  double readspeed_;
  double writespeed_;

};

struct Error
{
  Error();
  Error(const ErrorCode code, const std::string& text);

  ErrorCode code_;
  std::string text_;

};

struct GPUSTAT
{
  GPUSTAT(const std::string& uuid, const std::string& name, const unsigned int gpuusage, const unsigned int memoryusage, const unsigned long long freememory, const unsigned long long totalmemory, const unsigned long long usedmemory);

  std::string uuid_;
  std::string name_;
  unsigned int gpuusage_;
  unsigned int memoryusage_;
  unsigned long long freememory_;
  unsigned long long totalmemory_;
  unsigned long long usedmemory_;

};

struct HARDWARESTATS
{
  HARDWARESTATS(const uint64_t time, const std::vector<DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory,  const uint64_t availablememory, const std::vector<GPUSTAT>& gpustats);
  HARDWARESTATS();

  uint64_t time_;
  std::vector<DISKSTAT> diskstats_;
  double cpuusage_;
  uint64_t totalmemory_;
  uint64_t availablememory_;
  std::vector<GPUSTAT> gpustats_;

};

struct LOGMESSAGE
{
  LOGMESSAGE(const uint64_t time, const monocle::Severity severity, const std::string& message);

  bool operator==(const LOGMESSAGE& rhs) const;

  uint64_t time_;
  monocle::Severity severity_;
  std::string message_;

};

struct GROUP
{
  GROUP(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings);

  bool operator==(const GROUP& rhs) const;

  uint64_t token_;
  std::string name_;
  bool manageusers_;
  bool managerecordings_;
  bool managemaps_;
  bool managedevice_;
  bool allrecordings_;
  std::vector<uint64_t> recordings_;

};

struct MAP
{
  MAP(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5);

  bool operator==(const MAP& rhs) const;

  uint64_t token_;
  std::string name_;
  std::string location_;
  std::string imagemd5_;

};

struct ONVIFUSER
{
  ONVIFUSER(const uint64_t token, const std::string& username, const monocle::ONVIFUserlevel userlevel);

  bool operator==(const ONVIFUSER& rhs) const;

  uint64_t token_;
  std::string username_;
  monocle::ONVIFUserlevel userlevel_;

};

struct TRACKSTATISTICS
{
  TRACKSTATISTICS(const uint32_t trackid, const uint64_t trackdatareceived);

  bool operator==(const TRACKSTATISTICS& rhs) const;

  uint32_t trackid_;
  uint64_t trackdatareceived_;

};

struct RECORDINGSTATISTICS
{
  RECORDINGSTATISTICS();
  RECORDINGSTATISTICS(const uint64_t token, const std::vector<TRACKSTATISTICS>& tracksstatistics);

  bool operator==(const RECORDINGSTATISTICS& rhs) const;

  uint64_t token_;
  std::vector<TRACKSTATISTICS> tracksstatistics_;

};

struct USER
{
  USER(const uint64_t token, const std::string& username, const uint64_t grouptoken);

  bool operator==(const USER& rhs) const;

  uint64_t token_;
  std::string username_;
  uint64_t grouptoken_;

};

struct FILE
{
  FILE(const uint64_t token, const std::string& path, const std::string& mountpoint, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate);

  bool operator==(const FILE& rhs) const;

  uint64_t token_;
  std::string path_;
  std::string mountpoint_;
  uint64_t numchunks_;
  uint64_t chunksize_;
  bool automount_;
  monocle::FileState state_;
  monocle::FileMonitorState monitorstate_;

};

#pragma pack(push, 1)
struct HEADER
{
  explicit HEADER(const uint32_t size, const bool error, const bool compressed, const Message message, const uint16_t sequence);

  inline bool IsError() const { return (flags_ & static_cast<uint32_t>(monocle::HeaderFlags::FLAG_ERROR)); }
  inline bool IsCompressed() const { return (flags_ & static_cast<uint32_t>(monocle::HeaderFlags::FLAG_COMPRESSED)); }

  uint32_t size_; // The size of the message, excluding the size of this HEADER
  uint32_t flags_;
  Message message_;
  uint16_t sequence_;

};
#pragma pack(pop)

struct ADDRECORDINGJOBSOURCE
{
  ADDRECORDINGJOBSOURCE(const uint32_t recordingtrackid, const monocle::ReceiverMode receivermode, const std::vector<std::string>& sourceparameters, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& recordingjobsourcetrackparameters);

  bool operator==(const ADDRECORDINGJOBSOURCE& rhs) const;

  uint32_t recordingtrackid_;
  monocle::ReceiverMode receivermode_;
  std::vector<std::string> sourceparameters_;
  std::string mediauri_;
  std::string username_;
  std::string password_;
  std::vector<std::string> recordingjobsourcetrackparameters_;

};

struct CHANGERECORDINGJOBSOURCE
{
  CHANGERECORDINGJOBSOURCE(const uint32_t recordingtrackid, const monocle::ReceiverMode receivermode, const std::vector<std::string>& sourceparameters, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& recordingjobsourcetrackparameters);

  bool operator==(const CHANGERECORDINGJOBSOURCE& rhs) const;

  uint32_t recordingtrackid_;
  monocle::ReceiverMode receivermode_;
  std::vector<std::string> sourceparameters_;
  std::string mediauri_;
  std::string username_;
  std::string password_;
  std::vector<std::string> recordingjobsourcetrackparameters_;

};

struct RECEIVER
{
  RECEIVER(const uint64_t token, const ReceiverMode mode, const std::string& mediauri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, const ReceiverState state);

  bool operator==(const RECEIVER& rhs) const;

  uint64_t token_;
  ReceiverMode mode_;
  std::string mediauri_;
  bool autocreated_;
  std::string username_;
  std::string password_;
  std::vector<std::string> parameters_;
  ReceiverState state_;

};

struct RECORDINGJOBSOURCETRACK
{
  RECORDINGJOBSOURCETRACK(const uint64_t token, const uint64_t trackid, const std::vector<std::string>& parameters, const RecordingJobState state, const std::string& error, const std::vector<std::string>& activeparameters);

  bool operator==(const RECORDINGJOBSOURCETRACK& rhs) const;

  uint64_t token_;
  uint64_t trackid_;
  std::vector<std::string> parameters_;
  RecordingJobState state_;
  std::string error_;
  std::vector<std::string> activeparameters_;

};

struct RECORDINGJOBSOURCE
{
  RECORDINGJOBSOURCE(const uint64_t token, const RecordingJobSourceType type, const uint64_t receivertoken, const std::vector<RECORDINGJOBSOURCETRACK>& tracks);

  bool operator==(const RECORDINGJOBSOURCE& rhs) const;

  uint64_t token_;
  RecordingJobSourceType type_;
  uint64_t receivertoken_; // Points to a Media or Receiver token depending on type
  std::vector<RECORDINGJOBSOURCETRACK> tracks_;

};

struct RECORDINGJOB
{
  RECORDINGJOB(const uint64_t token, const std::string& name, const bool enable, const uint64_t priority, const std::vector<RECORDINGJOBSOURCE> sources);

  bool operator==(const RECORDINGJOB& rhs) const;

  uint64_t token_;
  std::string name_;
  bool enabled_;
  uint64_t priority_;
  std::vector<RECORDINGJOBSOURCE> sources_;

};

struct CODECINDEX
{
  CODECINDEX(const uint64_t id, const Codec codec, const std::string& parameters, const uint64_t timestamp);

  std::vector<std::string> GetParameters() const;

  bool operator==(const CODECINDEX& rhs) const;

  uint64_t id_;
  Codec codec_;
  std::string parameters_;
  uint64_t timestamp_;

};

struct RECORDINGTRACK
{
  RECORDINGTRACK();
  RECORDINGTRACK(const uint32_t id, const std::string& token, const TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsignature, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<INDEX>& indices, const std::vector<CODECINDEX>& codecindices, const std::pair<uint64_t, uint64_t>& totaltrackdata);

  bool operator==(const RECORDINGTRACK& rhs) const;

  uint32_t id_;
  std::string token_;
  TrackType tracktype_;
  std::string description_;
  bool fixedfiles_;
  bool digitalsignature_;
  bool encrypt_;
  uint32_t flushfrequency_;
  std::vector<uint64_t> files_;
  std::vector<INDEX> indices_; // <starttime, endtime>
  std::vector<CODECINDEX> codecindices_;
  std::pair<uint64_t, uint64_t> totaltrackdata_;

};

struct RECORDING
{
  RECORDING();
  RECORDING(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const std::vector<RECORDINGJOB>& jobs, const std::vector<RECORDINGTRACK>& tracks, const boost::optional<uint64_t>& activejob);

  bool operator==(const RECORDING& rhs) const;

  uint64_t token_;
  std::string sourceid_;
  std::string name_;
  std::string location_;
  std::string description_;
  std::string address_;
  std::string content_;
  uint64_t retentiontime_;
  std::vector<RECORDINGJOB> jobs_;
  std::vector<RECORDINGTRACK> tracks_;
  boost::optional<uint64_t> activejob_;

};

struct RECORDINGLOGMESSAGE
{
  RECORDINGLOGMESSAGE(const uint64_t token, const LOGMESSAGE& logmessage);

  uint64_t token_;
  LOGMESSAGE logmessage_;

};

struct MOUNTPOINT
{
  MOUNTPOINT();
  MOUNTPOINT(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source);

  bool operator==(const MOUNTPOINT& rhs) const;

  uint64_t id_;
  uint64_t parentid_;
  uint64_t majorstdev_;
  uint64_t minorstdev_;
  std::string path_;
  std::string type_;
  std::string source_;

};

struct GETSTATE
{
  GETSTATE();
  GETSTATE(const std::string& name, const std::string& publickey, const std::string& architecture, const int operatingsystem, const std::string& compiler, const std::string& databasepath, const utility::Version& version, const uint64_t identifier, const std::vector<std::string>& environmentvariables, const std::vector<std::string>& commandlinevariables, const std::vector<ONVIFUSER>& onvifusers, const std::vector<GROUP>& groups, const std::vector<USER>& users, const std::vector<FILE>& files, const std::vector<RECEIVER>& receivers, const std::vector<RECORDING>& recordings, const std::vector<LOGMESSAGE>& serverlogmessages, const uint32_t maxrecordings, const std::vector<MAP>& maps, const std::vector<MOUNTPOINT>& mountpoints, const std::string& latitude, const std::string& longitude);

  std::string name_;
  std::string publickey_;
  std::string architecture_;
  int operatingsystem_;
  std::string compiler_;
  std::string databasepath_;
  utility::Version version_;
  uint64_t identifier_;
  std::vector<std::string> environmentvariables_;
  std::vector<std::string> commandlinevariables_;
  std::vector<ONVIFUSER> onvifusers_;
  std::vector<GROUP> groups_;
  std::vector<USER> users_;
  std::vector<FILE> files_;
  std::vector<RECEIVER> receivers_;
  std::vector<RECORDING> recordings_;
  std::vector<LOGMESSAGE> serverlogmessages_;
  uint32_t maxrecordings_;
  std::vector<MAP> maps_;
  std::vector<MOUNTPOINT> mountpoints_;
  std::string latitude_;
  std::string longitude_;

};

struct SUBSCRIBE
{
  SUBSCRIBE();
  SUBSCRIBE(const std::string& name, const std::string& publickey, const std::string& architecture, const int operatingsystem, const std::string& compiler, const std::string& databasepath, const utility::Version& version, const uint64_t identifier, const std::vector<std::string>& environmentvariables, const std::vector<std::string>& commandlinevariables, const std::vector<ONVIFUSER>& onvifusers, const std::vector<GROUP>& groups, const std::vector<USER>& users, const std::vector<FILE>& files, const std::vector<RECEIVER>& receivers, const std::vector<RECORDING>& recordings, const std::vector<LOGMESSAGE>& serverlogmessages, const uint32_t maxrecordings, const std::vector<MAP>& maps, const std::vector<MOUNTPOINT>& mountpoints, const std::string& latitude, const std::string& longitude, const unsigned int numcudadevices, const unsigned int numcldevices, const uint32_t maxobjectdetectors);

  std::string name_;
  std::string publickey_;
  std::string architecture_;
  int operatingsystem_;
  std::string compiler_;
  std::string databasepath_;
  utility::Version version_;
  uint64_t identifier_;
  std::vector<std::string> environmentvariables_;
  std::vector<std::string> commandlinevariables_;
  std::vector<ONVIFUSER> onvifusers_;
  std::vector<GROUP> groups_;
  std::vector<USER> users_;
  std::vector<FILE> files_;
  std::vector<RECEIVER> receivers_;
  std::vector<RECORDING> recordings_;
  std::vector<LOGMESSAGE> serverlogmessages_;
  uint32_t maxrecordings_;
  std::vector<MAP> maps_;
  std::vector<MOUNTPOINT> mountpoints_;
  std::string latitude_;
  std::string longitude_;
  unsigned int numcudadevices_;
  unsigned int numcldevices_;
  uint32_t maxobjectdetectors_;

};

struct SUBSCRIBERECORDINGJOBLOG
{
  SUBSCRIBERECORDINGJOBLOG();
  SUBSCRIBERECORDINGJOBLOG(const std::vector< std::pair<uint64_t, std::vector<LOGMESSAGE> > >& recordingjoblogmessages);

  std::vector< std::pair<uint64_t, std::vector<LOGMESSAGE> > > recordingjoblogmessages_; // <RecordingJobToken, Messages>

};

struct SUBSCRIBERECORDINGJOBSOURCETRACKLOG
{
  struct RECORDINGJOBSOURCETRACKTOKEN
  {
    RECORDINGJOBSOURCETRACKTOKEN(const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken);

    uint64_t recordingjobtoken_;
    uint64_t recordingjobsourcetoken_;
    uint64_t recordingjobsourcetracktoken_;
  };

  SUBSCRIBERECORDINGJOBSOURCETRACKLOG();
  SUBSCRIBERECORDINGJOBSOURCETRACKLOG(const std::vector< std::pair<RECORDINGJOBSOURCETRACKTOKEN, std::vector<LOGMESSAGE> > >& recordingjobsourcetracklogmessages);

  std::vector< std::pair<RECORDINGJOBSOURCETRACKTOKEN, std::vector<LOGMESSAGE> > > recordingjobsourcetracklogmessages_; // <RecordingJobSourceTrackToken, Messages>

};

struct SUBSCRIBERECORDINGLOG
{
  SUBSCRIBERECORDINGLOG();
  SUBSCRIBERECORDINGLOG(const std::vector<LOGMESSAGE>& messages);

  std::vector<LOGMESSAGE> messages_;

};

struct SUBSCRIBERECORDINGTRACKLOG
{
  SUBSCRIBERECORDINGTRACKLOG();
  SUBSCRIBERECORDINGTRACKLOG(const std::vector< std::pair<uint32_t, std::vector<LOGMESSAGE> > >& recordingtracklogmessages);

  std::vector< std::pair<uint32_t, std::vector<LOGMESSAGE> > > recordingtracklogmessages_; // <RecordingTrackId, Messages>

};

///// Prototypes /////

boost::optional<TrackType> GetTrackType(const std::string& tracktype);
std::string AuthenticateDigest(const std::string& username, const std::string& password);
std::string AuthenticateDigest(const std::string& username, const std::string& password, const std::string& nonce, const std::string& clientnonce);
std::string AuthenticateDigest(const std::string& digest, const std::string& nonce, const std::string& clientnonce);
std::vector< flatbuffers::Offset<DiskStat> > GetDiskStatsBuffer(const std::vector<DISKSTAT>& diskstats, flatbuffers::FlatBufferBuilder& fbb);
std::vector< flatbuffers::Offset<ONVIFUser> > GetONVIFUserBuffers(const std::vector<ONVIFUSER>& onvifusers, flatbuffers::FlatBufferBuilder& fbb);
std::vector< flatbuffers::Offset<User> > GetUserBuffers(const std::vector<USER>& users, flatbuffers::FlatBufferBuilder& fbb);
std::vector< flatbuffers::Offset<Group> > GetGroupBuffers(const std::vector<GROUP>& groups, flatbuffers::FlatBufferBuilder& fbb);
std::vector< flatbuffers::Offset<File> > GetFileBuffers(const std::vector<FILE>& files, flatbuffers::FlatBufferBuilder& fbb);
std::vector< flatbuffers::Offset<Receiver> > GetReceiverBuffers(const std::vector<RECEIVER>& receivers, flatbuffers::FlatBufferBuilder& fbb);
std::vector< flatbuffers::Offset<Recording> > GetRecordingBuffers(const std::vector<RECORDING>& recordings, flatbuffers::FlatBufferBuilder& fbb);
std::vector< flatbuffers::Offset<RecordingJobSource> > GetRecordingJobSourceBuffers(const std::vector<RECORDINGJOBSOURCE>& recordingjobsources, flatbuffers::FlatBufferBuilder& fbb);
std::vector< flatbuffers::Offset<LogMessage> > GetLogMessagesBuffer(const std::vector<LOGMESSAGE>& logmessages, flatbuffers::FlatBufferBuilder& fbb);
std::vector< flatbuffers::Offset<Map> > GetMapBuffers(const std::vector<MAP>& maps, flatbuffers::FlatBufferBuilder& fbb);
std::vector< flatbuffers::Offset<MountPoint> > GetMountPointBuffers(const std::vector<MOUNTPOINT>& mountpoints, flatbuffers::FlatBufferBuilder& fbb);

}

#endif
