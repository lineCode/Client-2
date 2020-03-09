// device.h
//

#ifndef IDFMKGWV9THPSCNDCDDE52HSJ9366BC609
#define IDFMKGWV9THPSCNDCDDE52HSJ9366BC609

///// Includes /////

#include <boost/circular_buffer.hpp>
#include <monocleprotocol/client/client.hpp>
#include <mutex>
#include <socket/proxyparams.hpp>
#include <QEnableSharedFromThis>
#include <QSharedPointer>
#include <QString>
#include <utility/utility.hpp>
#include <utility/publickey.hpp>
#include <vector>

#include "monocleclient/connection.h"
#include "monocleclient/map.h"

///// Namespaces /////

namespace client
{

///// Declarations /////

class File;
class Group;
class ONVIFUser;
class Receiver;
class Recording;
class RecordingJob;
class RecordingJobSource;
class RecordingJobSourceTrack;
class RecordingTrack;
class User;

///// Enumerations /////

enum class DEVICESTATE : int
{
  DISCONNECTED,
  UNAUTHORIZED,
  CONNECTING,
  CONNECTED,
  AUTHORIZED,
  SUBSCRIBED
};

///// Structures /////

struct MOUNTPOINT
{
  MOUNTPOINT(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const QString& path, const QString& type, const QString& source);

  bool operator==(const MOUNTPOINT& rhs) const;

  uint64_t id_;
  uint64_t parentid_;
  uint64_t majorstdev_;
  uint64_t minorstdev_;
  QString path_;
  QString type_;
  QString source_;

};

///// Classes /////

class Device : public Connection
{
 Q_OBJECT

 public:

  Device(const sock::ProxyParams& proxyparams, const QString& address, const uint16_t port, const QString& username, const QString& password, const uint64_t identifier);
  ~Device();

  void DestroyData();

  void Set(const sock::ProxyParams& proxyparams, const QString& address, const uint16_t port, const QString& username, const QString& password, const uint64_t identifier);

  virtual void Disconnected() override;

  void Subscribe();

  inline QString GetUsername() const { return username_; }
  inline QString GetPassword() const { return password_; }
  inline DEVICESTATE GetState() const { return state_; }
  inline int64_t GetTimeOffset() const { return utctimeoffset_; }
  inline unsigned int GetNumClDevices() const { return numcldevices_; }
  inline unsigned int GetNumCudaDevices() const { return numcudadevices_; }
  inline std::pair<QString, QString> GetLocation() const { return std::make_pair(latitude_, longitude_); }
  inline const QString& GetName() const { return name_; }
  inline utility::PublicKey& GetPublicKey() { return publickey_; }
  inline const QString& GetArchitecture() const { return architecture_; }
  inline const boost::optional<utility::OPERATINGSYSTEM>& GetOperatingSystem() const { return operatingsystem_; }
  bool IsLinux() const;
  bool IsWindows() const;
  inline const QString& GetCompiler() const { return compiler_; }
  inline const QString GetDatabasePath() const { return databasepath_; }
  inline const utility::Version& GetVersion() const { return version_; }
  inline uint64_t GetIdentifier() const { return identifier_; }
  inline const std::vector<QString> GetEnvironmentVariables() const { return environmentvariables_; }
  inline const std::vector<QString> GetCommandLineVariables() const { return commandlinevariables_; }
  inline const std::vector< QSharedPointer<ONVIFUser> >& GetONVIFUsers() const { return onvifusers_; }
  inline const std::vector< QSharedPointer<Group> >& GetGroups() const { return groups_; }
  inline const std::vector< QSharedPointer<User> >& GetUsers() const { return users_; }
  inline const std::vector< QSharedPointer<client::File> >& GetFiles() const { return files_; }
  const QSharedPointer<client::File> GetFile(const uint64_t token) const;
  inline const std::vector< QSharedPointer<client::Receiver> > GetReceivers() const { return receivers_; }
  inline const std::vector< QSharedPointer<client::Recording> > GetRecordings() const { return recordings_; }
  const QSharedPointer<client::Recording> GetRecording(const uint64_t token) const;
  inline const boost::circular_buffer<monocle::LOGMESSAGE>& GetLogMessages() const { return logmessages_; }
  inline uint32_t GetMaxObjectDetectors() const { return maxobjectdetectors_; }
  inline uint32_t GetMaxRecordings() const { return maxrecordings_; }
  inline const std::vector< QSharedPointer<Map> >& GetMaps() const { return maps_; }
  inline const std::vector<MOUNTPOINT> GetMountPoints() const { return mountpoints_; }

  QSharedPointer<Group> GetGroup(const uint64_t token) const;
  QSharedPointer<ONVIFUser> GetONVIFUser(const uint64_t token) const;
  QSharedPointer<client::Receiver> GetReceiver(const uint64_t token) const;
  QSharedPointer<User> GetUser(const uint64_t token) const;
  QSharedPointer<Map> GetMap(const uint64_t token) const;

  QStringList GetLocations() const;

  bool SupportsMaps() const;
  bool SupportsRecordingLocation() const;
  bool SupportsGetChildFolders() const;
  bool SupportsManageDevice() const;
  bool SupportsDeviceLocation() const;
  bool SupportsFindMotion() const;
  bool SupportsFindObject() const;
  bool SupportsObjectDetection() const;
  bool SupportsCreateDefaultJob() const;
  bool SupportsTrackCodec() const; // Whether the codecs are put into the Track structure
  bool SupportsGetChildFoldersFilter() const;

  // These methods look for the current user in the users_, find the group and figure out whether the current user has permission to do the operation
  bool CanManageUsers();
  bool CanManageRecordings();
  bool CanManageMaps();
  bool CanManageDevice();

  size_t GetNumObjectDetectors() const;
  bool IsValidLicense() const;

 protected:

  Q_INVOKABLE void Reconnect();
  virtual void timerEvent(QTimerEvent* event) override;
  
 private:

  void SetState(const DEVICESTATE state, const QString& message);
  std::vector<QString> ToStrings(const std::vector<std::string>& strings) const;
  bool IsPermutation(const std::vector<QString>& lhs, const std::vector<std::string>& rhs) const;

  QString username_;
  QString password_;

  sock::Connection connect_;
  monocle::client::Connection getauthenticatenonce_;
  monocle::client::Connection authenticate_;
  monocle::client::Connection gettime_;
  monocle::client::Connection subscribe_;
  monocle::client::Connection keepaliveconnection_;

  DEVICESTATE state_;
  int keepalivetimer_;

  int64_t utctimeoffset_;

  unsigned int numcldevices_;
  unsigned int numcudadevices_;
  QString latitude_;
  QString longitude_;
  QString name_;
  utility::PublicKey publickey_;
  QString architecture_;
  boost::optional<utility::OPERATINGSYSTEM> operatingsystem_;
  QString compiler_;
  QString databasepath_;
  utility::Version version_;
  uint64_t identifier_;
  std::vector<QString> environmentvariables_;
  std::vector<QString> commandlinevariables_;
  std::vector< QSharedPointer<ONVIFUser> > onvifusers_;
  std::vector< QSharedPointer<Group> > groups_;
  std::vector< QSharedPointer<User> > users_;
  std::vector< QSharedPointer<client::File> > files_;
  std::vector< QSharedPointer<client::Receiver> > receivers_;
  std::vector< QSharedPointer<client::Recording> > recordings_;
  boost::circular_buffer<monocle::LOGMESSAGE> logmessages_;
  uint32_t maxobjectdetectors_;
  uint32_t maxrecordings_;
  std::vector< QSharedPointer<Map> > maps_;
  std::vector<MOUNTPOINT> mountpoints_;

 signals:

  void SignalDisconnected();
  void SignalDiscoveryHello(const std::vector<QString>& addresses, const std::vector<QString>& scopes);
  void SignalFileAdded(QSharedPointer<client::File>& file);
  void SignalFileChanged(QSharedPointer<client::File>& file);
  void SignalFileRemoved(const uint64_t token);
  void SignalFileMonitorStateChanged(QSharedPointer<client::File>& file, const monocle::FileMonitorState monitorstate);
  void SignalFileStateChanged(QSharedPointer<client::File>& file, const monocle::FileState state);
  void SignalGroupAdded(QSharedPointer<Group>& group);
  void SignalGroupChanged(QSharedPointer<Group>& group);
  void SignalGroupRemoved(const uint64_t token);
  void SignalLatency(const std::chrono::steady_clock::duration latency);
  void SignalMapAdded(const QSharedPointer<Map>& map);
  void SignalMapChanged(const QSharedPointer<Map>& map);
  void SignalMapRemoved(const uint64_t token);
  void SignalMountPointAdded(const MOUNTPOINT& mountpoint);
  void SignalMountPointRemoved(const MOUNTPOINT& mountpoint);
  void SignalONVIFUserAdded(QSharedPointer<ONVIFUser>& onvifuser);
  void SignalONVIFUserChanged(QSharedPointer<ONVIFUser>& onvifuser);
  void SignalONVIFUserRemoved(const uint64_t token);
  void SignalReceiverAdded(QSharedPointer<client::Receiver>& receiver);
  void SignalReceiverChanged(QSharedPointer<client::Receiver>& receiver);
  void SignalReceiverRemoved(const uint64_t token);
  void SignalRecordingAdded(QSharedPointer<client::Recording>& recording);
  void SignalRecordingChanged(QSharedPointer<client::Recording>& recording);
  void SignalRecordingRemoved(const uint64_t token);
  void SignalRecordingJobAdded(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob);
  void SignalRecordingJobChanged(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob);
  void SignalRecordingJobLogMessage(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const uint64_t time, const monocle::Severity severity, const QString& message);
  void SignalRecordingJobRemoved(const QSharedPointer<client::Recording>& recording, const uint64_t token);
  void SignalRecordingJobSourceAdded(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource);
  void SignalRecordingJobSourceChanged(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource);
  void SignalRecordingJobSourceRemoved(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const uint64_t token);
  void SignalRecordingJobSourceTrackAdded(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack);
  void SignalRecordingJobSourceTrackChanged(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack);
  void SignalRecordingJobSourceTrackLogMessage(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsourcetoken, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack, const uint64_t time, const monocle::Severity severity, const QString& message);
  void SignalRecordingJobSourceTrackRemoved(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const uint64_t token);
  void SignalRecordingLogMessage(const QSharedPointer<client::Recording>& recording, const uint64_t time, const monocle::Severity severity, const QString& message);
  void SignalRecordingTrackLogMessage(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingTrack>& recordingtrack, const uint64_t time, const monocle::Severity severity, const QString& message);
  void SignalServerLogMessage(const uint64_t time, const monocle::Severity severity, const QString& message);
  void SignalRecordingTrackAdded(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingTrack>& track);
  void SignalRecordingTrackChanged(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingTrack>& track);
  void SignalRecordingTrackRemoved(const QSharedPointer<client::Recording>& recording, const uint32_t token);
  void SignalUserAdded(QSharedPointer<User>& user);
  void SignalUserChanged(QSharedPointer<User>& user);
  void SignalUserRemoved(const uint64_t token);
  void SignalStateChanged(const client::DEVICESTATE state, const QString& message);
  void SignalLicenseStateChanged(const bool valid);

 private slots:

  void SlotDiscoveryHello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes);
  void SlotFileAdded(const uint64_t filetoken, const std::string& path, const std::string& mountpoint, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate);
  void SlotFileRemoved(const uint64_t token);
  void SlotFileMonitorStateChanged(const uint64_t token, const monocle::FileMonitorState monitorstate);
  void SlotFileStateChanged(const uint64_t token, const monocle::FileState state);
  void SlotGroupAdded(const uint64_t token, const QString& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings);
  void SlotGroupChanged(const uint64_t token, const QString& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings);
  void SlotGroupRemoved(const uint64_t token);
  void SlotMapAdded(const uint64_t token, const QString& name, const QString& location, const QString& imagemd5);
  void SlotMapChanged(const uint64_t token, const QString& name, const QString& location, const QString& imagemd5);
  void SlotMapRemoved(const uint64_t token);
  void SlotMountPointAdded(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const QString& path, const QString& type, const QString& source);
  void SlotMountPointRemoved(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const QString& path, const QString& type, const QString& source);
  void SlotLocationChanged(const QString& latitude, const QString& longitude);
  void SlotNameChanged(const QString& name);
  void SlotONVIFUserAdded(const uint64_t token, const QString& username, const monocle::ONVIFUserlevel onvifuserlevel);
  void SlotONVIFUserChanged(const uint64_t token, const boost::optional<QString>& username, const monocle::ONVIFUserlevel onvifuserlevel);
  void SlotONVIFUserRemoved(const uint64_t token);
  void SlotReceiverAdded(const uint64_t token, const monocle::ReceiverMode mode, const QString& mediauri, const bool autocreated, const QString& username, const QString& password, const std::vector<QString>& parameters, const monocle::ReceiverState state);
  void SlotReceiverChanged(const uint64_t token, const monocle::ReceiverMode mode, const QString& mediauri, const bool autocreated, const QString& username, const QString& password, const std::vector<QString>& parameters);
  void SlotReceiverRemoved(const uint64_t token);
  void SlotRecordingActiveJobChanged(const uint64_t token, const boost::optional<uint64_t>& activejob);
  void SlotRecordingAdded(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob);
  void SlotRecordingChanged(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob);
  void SlotRecordingRemoved(const uint64_t token);
  void SlotRecordingJobAdded(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const monocle::RecordingJobState state);
  void SlotRecordingJobChanged(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority);
  void SlotRecordingJobLogMessage(const uint64_t recordingtoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const QString& message);
  void SlotRecordingJobRemoved(const uint64_t recordingtoken, const uint64_t token);
  void SlotRecordingJobSourceAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token, const uint64_t receivertoken);
  void SlotRecordingJobSourceRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token);
  void SlotRecordingJobSourceTrackActiveParametersChanged(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken, const std::vector<QString>& activeparameters);
  void SlotRecordingJobSourceTrackAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint32_t recordingtrackid, const std::vector<QString>& parameters, const monocle::RecordingJobState state, const QString& error, const std::vector<QString>& activeparameters);
  void SlotRecordingJobSourceTrackLogMessage(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const QString& message);
  void SlotRecordingJobSourceTrackRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token);
  void SlotRecordingJobSourceTrackStateChanged(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken, const uint64_t time, const monocle::RecordingJobState state, const QString& error);
  void SlotRecordingLogMessage(const uint64_t token, const uint64_t time, const monocle::Severity severity, const QString& message);
  void SlotRecordingTrackCodecAdded(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp);
  void SlotRecordingTrackCodecRemoved(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id);
  void SlotRecordingTrackLogMessage(const uint64_t recordingtoken, const uint32_t id, const uint64_t time, const monocle::Severity severity, const QString& message);
  void SlotServerLogMessage(const uint64_t time, const monocle::Severity severity, const QString& message);
  void SlotTrackAdded(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& filetokens, const std::vector<monocle::CODECINDEX>& codecindices);
  void SlotTrackChanged(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& filetokens, const std::vector<monocle::CODECINDEX>& codecindices);
  void SlotTrackDeleteData(const uint64_t recordingtoken, const uint32_t trackid, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end);
  void SlotTrackRemoved(const uint64_t recordingtoken, const uint32_t id);
  void SlotTrackSetData(const uint64_t recordingtoken, const uint32_t trackid, const std::vector<monocle::INDEX>& indices);
  void SlotUserAdded(const uint64_t token, const QString& username, const uint64_t group);
  void SlotUserChanged(const uint64_t token, const uint64_t group);
  void SlotUserRemoved(const uint64_t token);
  void SlotStateChanged(const client::DEVICESTATE state, const QString& message);

};

}

#endif
