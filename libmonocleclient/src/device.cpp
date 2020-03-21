// device.cpp
//

///// Includes /////

#include "monocleclient/device.h"

#include <boost/range/combine.hpp>
#include <QMessageBox>
#include <QStandardPaths>
#include <QtGlobal>
#include <QTimer>
#include <utility/utility.hpp>

#include "monocleclient/file.h"
#include "monocleclient/group.h"
#include "monocleclient/layout.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/managefilewindow.h"
#include "monocleclient/onvifuser.h"
#include "monocleclient/receiver.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"
#include "monocleclient/user.h"

///// Namespaces /////

namespace client
{

///// Globals /////

const std::chrono::milliseconds RECONNECT_DELAY(10000);

///// Functions /////

bool operator!=(const std::vector< std::pair<uint64_t, uint64_t> >& lhs, const std::vector<monocle::INDEX>& rhs)
{
  if (lhs.size() != rhs.size())
  {

    return true;
  }

  for (auto index : boost::combine(lhs, rhs))
  {
    if ((index.get<0>().first != index.get<1>().starttime()) || (index.get<0>().second != index.get<1>().endtime()))
    {

      return true;
    }
  }
  return false;
}

///// Methods /////

MOUNTPOINT::MOUNTPOINT(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const QString& path, const QString& type, const QString& source) :
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

Device::Device(const sock::ProxyParams& proxyparams, const QString& address, const uint16_t port, const QString& username, const QString& password, const uint64_t identifier) :
  Connection(MainWindow::Instance()->GetGUIIOService(), proxyparams, address, port),
  username_(username),
  password_(password),
  state_(DEVICESTATE::DISCONNECTED),
  keepalivetimer_(-1),
  name_(address), // Temporary until we grab it from the server!
  utctimeoffset_(0),
  numcldevices_(0),
  numcudadevices_(0),
  identifier_(identifier),
  logmessages_(1000),
  maxobjectdetectors_(0),
  maxrecordings_(0)
{
  connect(this, &Connection::SignalDiscoveryHello, this, QOverload<const std::vector<std::string>&, const std::vector<std::string>&>::of(&Device::SlotDiscoveryHello), Qt::QueuedConnection);
  connect(this, &Connection::SignalFileAdded, this, QOverload<const uint64_t, const std::string&, const std::string&, const uint64_t, const uint64_t, const bool, const monocle::FileState, const monocle::FileMonitorState>::of(&Device::SlotFileAdded), Qt::QueuedConnection);
  connect(this, &Connection::SignalFileRemoved, this, QOverload<const uint64_t>::of(&Device::SlotFileRemoved), Qt::QueuedConnection);
  connect(this, &Connection::SignalFileMonitorStateChanged, this, QOverload<const uint64_t, const monocle::FileMonitorState>::of(&Device::SlotFileMonitorStateChanged), Qt::QueuedConnection);
  connect(this, &Connection::SignalFileStateChanged, this, QOverload<const uint64_t, const monocle::FileState>::of(&Device::SlotFileStateChanged), Qt::QueuedConnection);
  connect(this, &Connection::SignalGroupAdded, this, QOverload<const uint64_t, const QString&, const bool, const bool, const bool, const bool, const bool, const std::vector<uint64_t>&>::of(&Device::SlotGroupAdded), Qt::QueuedConnection);
  connect(this, &Connection::SignalGroupChanged, this, QOverload<const uint64_t, const QString&, const bool, const bool, const bool, const bool, const bool, const std::vector<uint64_t>&>::of(&Device::SlotGroupChanged), Qt::QueuedConnection);
  connect(this, &Connection::SignalGroupRemoved, this, QOverload<const uint64_t>::of(&Device::SlotGroupRemoved), Qt::QueuedConnection);
  connect(this, &Connection::SignalGuiOrderChanged, this, &Device::SlotGuiOrderChanged, Qt::QueuedConnection);
  connect(this, &Connection::SignalLayoutAdded, this, &Device::SlotLayoutAdded, Qt::QueuedConnection);
  connect(this, &Connection::SignalLayoutChanged, this, &Device::SlotLayoutChanged, Qt::QueuedConnection);
  connect(this, &Connection::SignalLayoutNameChanged, this, &Device::SlotLayoutNameChanged, Qt::QueuedConnection);
  connect(this, &Connection::SignalLayoutRemoved, this, &Device::SlotLayoutRemoved, Qt::QueuedConnection);
  connect(this, &Connection::SignalMapAdded, this, QOverload<const uint64_t, const QString&, const QString&, const QString&>::of(&Device::SlotMapAdded), Qt::QueuedConnection);
  connect(this, &Connection::SignalMapChanged, this, QOverload<const uint64_t, const QString&, const QString&, const QString&>::of(&Device::SlotMapChanged), Qt::QueuedConnection);
  connect(this, &Connection::SignalMapRemoved, this, QOverload<const uint64_t>::of(&Device::SlotMapRemoved), Qt::QueuedConnection);
  connect(this, &Connection::SignalMountPointAdded, this, QOverload<const uint64_t, const uint64_t, const uint64_t, const uint64_t, const QString&, const QString&, const QString&>::of(&Device::SlotMountPointAdded), Qt::QueuedConnection);
  connect(this, &Connection::SignalMountPointRemoved, this, QOverload<const uint64_t, const uint64_t, const uint64_t, const uint64_t, const QString&, const QString&, const QString&>::of(&Device::SlotMountPointRemoved), Qt::QueuedConnection);
  connect(this, &Connection::SignalLocationChanged, this, QOverload<const QString&, const QString&>::of(&Device::SlotLocationChanged), Qt::QueuedConnection);
  connect(this, &Connection::SignalNameChanged, this, QOverload<const QString&>::of(&Device::SlotNameChanged), Qt::QueuedConnection);
  connect(this, &Connection::SignalONVIFUserAdded, this, QOverload<const uint64_t, const QString&, const monocle::ONVIFUserlevel>::of(&Device::SlotONVIFUserAdded), Qt::QueuedConnection);
  connect(this, &Connection::SignalONVIFUserChanged, this, QOverload<const uint64_t, const boost::optional<QString>&, const monocle::ONVIFUserlevel>::of(&Device::SlotONVIFUserChanged), Qt::QueuedConnection);
  connect(this, &Connection::SignalONVIFUserRemoved, this, QOverload<const uint64_t>::of(&Device::SlotONVIFUserRemoved), Qt::QueuedConnection);
  connect(this, &Connection::SignalReceiverAdded, this, &Device::SlotReceiverAdded, Qt::QueuedConnection);
  connect(this, &Connection::SignalReceiverChanged, this, &Device::SlotReceiverChanged, Qt::QueuedConnection);
  connect(this, &Connection::SignalReceiverRemoved, this, QOverload<const uint64_t>::of(&Device::SlotReceiverRemoved), Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingActiveJobChanged, this, &Device::SlotRecordingActiveJobChanged, Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingAdded, this, &Device::SlotRecordingAdded, Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingChanged, this, &Device::SlotRecordingChanged, Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingJobAdded, this, &Device::SlotRecordingJobAdded, Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingJobChanged, this, &Device::SlotRecordingJobChanged, Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingJobLogMessage, this, QOverload<const uint64_t, const uint64_t, const uint64_t, const monocle::Severity, const QString&>::of(&Device::SlotRecordingJobLogMessage), Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingJobRemoved, this, &Device::SlotRecordingJobRemoved, Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingJobSourceAdded, this, &Device::SlotRecordingJobSourceAdded, Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingJobSourceRemoved, this, &Device::SlotRecordingJobSourceRemoved, Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingJobSourceTrackActiveParametersChanged, this, QOverload<const uint64_t, const uint64_t, const uint64_t, const uint64_t, const std::vector<QString>&>::of(&Device::SlotRecordingJobSourceTrackActiveParametersChanged), Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingJobSourceTrackAdded, this, QOverload<const uint64_t, const uint64_t, const uint64_t, const uint64_t, const uint32_t, const std::vector<QString>&, const monocle::RecordingJobState, const QString&, const std::vector<QString>&>::of(&Device::SlotRecordingJobSourceTrackAdded), Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingJobSourceTrackLogMessage, this, QOverload<const uint64_t, const uint64_t, const uint64_t, const uint64_t, const uint64_t, const monocle::Severity, const QString&>::of(&Device::SlotRecordingJobSourceTrackLogMessage), Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingJobSourceTrackRemoved, this, &Device::SlotRecordingJobSourceTrackRemoved, Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingJobSourceTrackStateChanged, this, QOverload<const uint64_t, const uint64_t, const uint64_t, const uint64_t, const uint64_t, const monocle::RecordingJobState, const QString&>::of(&Device::SlotRecordingJobSourceTrackStateChanged), Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingRemoved, this, QOverload<const uint64_t>::of(&Device::SlotRecordingRemoved), Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingLogMessage, this, QOverload<const uint64_t, const uint64_t, const monocle::Severity, const QString&>::of(&Device::SlotRecordingLogMessage), Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingStatistics, this, &Device::SlotRecordingStatistics, Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingTrackCodecAdded, this, &Device::SlotRecordingTrackCodecAdded, Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingTrackCodecRemoved, this, &Device::SlotRecordingTrackCodecRemoved, Qt::QueuedConnection);
  connect(this, &Connection::SignalRecordingTrackLogMessage, this, QOverload<const uint64_t, const uint32_t, const uint64_t, const monocle::Severity, const QString&>::of(&Device::SlotRecordingTrackLogMessage), Qt::QueuedConnection);
  connect(this, &Connection::SignalServerLogMessage, this, QOverload<const uint64_t, const monocle::Severity, const QString&>::of(&Device::SlotServerLogMessage), Qt::QueuedConnection);
  connect(this, &Connection::SignalTrackAdded, this, &Device::SlotTrackAdded, Qt::QueuedConnection);
  connect(this, &Connection::SignalTrackChanged, this, &Device::SlotTrackChanged, Qt::QueuedConnection);
  connect(this, &Connection::SignalTrackDeleteData, this, QOverload<const uint64_t, const uint32_t, const boost::optional<uint64_t>&, const boost::optional<uint64_t>&>::of(&Device::SlotTrackDeleteData), Qt::QueuedConnection);
  connect(this, &Connection::SignalTrackRemoved, this, QOverload<const uint64_t, const uint32_t>::of(&Device::SlotTrackRemoved), Qt::QueuedConnection);
  connect(this, &Connection::SignalTrackSetData, this, QOverload<const uint64_t, const uint32_t, const std::vector<monocle::INDEX>&>::of(&Device::SlotTrackSetData), Qt::QueuedConnection);
  connect(this, &Connection::SignalUserAdded, this, QOverload<const uint64_t, const QString&, const uint64_t>::of(&Device::SlotUserAdded), Qt::QueuedConnection);
  connect(this, &Connection::SignalUserChanged, this, QOverload<const uint64_t, const uint64_t>::of(&Device::SlotUserChanged), Qt::QueuedConnection);
  connect(this, &Connection::SignalUserRemoved, this, QOverload<const uint64_t>::of(&Device::SlotUserRemoved), Qt::QueuedConnection);
  connect(this, &Device::SignalStateChanged, this, &Device::SlotStateChanged, Qt::QueuedConnection);
}

Device::~Device()
{
  Destroy();
  DestroyData();

  username_.clear();
  password_.clear();

  connect_.Close();
  getauthenticatenonce_.Close();
  authenticate_.Close();
  gettime_.Close();
  subscribe_.Close();
  keepaliveconnection_.Close();

  SetState(DEVICESTATE::DISCONNECTED, "Disconnected");
  if (keepalivetimer_ != -1)
  {
    killTimer(keepalivetimer_);
    keepalivetimer_ = -1;
  }
}

void Device::DestroyData()
{
  environmentvariables_.clear();
  commandlinevariables_.clear();

  std::vector<uint64_t> recordings;
  recordings.reserve(recordings_.size());
  for (const QSharedPointer<client::Recording>& recording : recordings_)
  {
    recordings.push_back(recording->GetToken());

    std::vector<uint64_t> recordingjobs;
    recordingjobs.reserve(recording->GetJobs().size());
    for (const QSharedPointer<client::RecordingJob>& recordingjob : recording->GetRecordingJobs())
    {
      recordingjobs.push_back(recordingjob->GetToken());

      std::vector<uint64_t> recordingjobsources;
      recordingjobsources.reserve(recordingjob->GetSources().size());
      for (const QSharedPointer<RecordingJobSource>& recordingjobsource : recordingjob->GetSources())
      {
        recordingjobsources.push_back(recordingjobsource->GetToken());

        std::vector<uint64_t> recordingjobsourcetracks;
        recordingjobsourcetracks.reserve(recordingjobsource->GetTracks().size());
        for (const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack : recordingjobsource->GetTracks())
        {
          recordingjobsourcetracks.push_back(recordingjobsourcetrack->GetToken());

        }

        for (const uint64_t recordingjobsourcetrack : recordingjobsourcetracks)
        {
          recordingjobsource->RemoveTrack(recordingjobsourcetrack);
          emit recording->JobSourceTrackRemoved(recordingjob, recordingjobsource, recordingjobsourcetrack);
          emit recordingjob->SourceTrackRemoved(recordingjobsource, recordingjobsourcetrack);
          emit SignalRecordingJobSourceTrackRemoved(recording, recordingjob, recordingjobsource, recordingjobsourcetrack);
        }
      }

      for (const uint64_t recordingjobsource : recordingjobsources)
      {
        recordingjob->RemoveSource(recordingjobsource);
        emit recording->JobSourceRemoved(recordingjob, recordingjobsource);
        emit SignalRecordingJobSourceRemoved(recording, recordingjob, recordingjobsource);
      }
    }

    for (const uint64_t recordingjob : recordingjobs)
    {
      recording->RemoveJob(recordingjob);
      emit SignalRecordingJobRemoved(recording, recordingjob);
    }

    std::vector<uint32_t> recordingtracks;
    recordingtracks.reserve(recording->GetTracks().size());
    for (const QSharedPointer<client::RecordingTrack>& recordingtrack : recording->GetTracks())
    {
      recordingtracks.push_back(recordingtrack->GetId());

    }

    for (const uint32_t recordingtrack : recordingtracks)
    {
      recording->RemoveTrack(recordingtrack);
      emit SignalRecordingTrackRemoved(recording, recordingtrack);
    }
  }

  while (!recordings_.empty())
  {
    const uint64_t token = recordings_.front()->GetToken();
    recordings_.erase(recordings_.begin());
    emit SignalRecordingRemoved(token);
  }

  while (!files_.empty())
  {
    const uint64_t token = files_.front()->GetToken();
    files_.erase(files_.begin());
    emit SignalFileRemoved(token);
  }

  while (!receivers_.empty())
  {
    const uint64_t token = receivers_.front()->GetToken();
    receivers_.erase(receivers_.begin());
    emit SignalReceiverRemoved(token);
  }

  while (!layouts_.empty())
  {
    const uint64_t token = layouts_.front()->GetToken();
    layouts_.erase(layouts_.begin());
    emit SignalLayoutRemoved(token);
    emit MainWindow::Instance()->GetDeviceMgr().LayoutRemoved(token);
  }

  while (!onvifusers_.empty())
  {
    const uint64_t token = onvifusers_.front()->GetToken();
    onvifusers_.erase(onvifusers_.begin());
    emit SignalONVIFUserRemoved(token);
  }

  while (!groups_.empty())
  {
    const uint64_t token = groups_.front()->GetToken();
    groups_.erase(groups_.begin());
    emit SignalGroupRemoved(token);
  }

  while (!users_.empty())
  {
    const uint64_t token = users_.front()->GetToken();
    users_.erase(users_.begin());
    emit SignalUserRemoved(token);
  }

  logmessages_.clear();
}

void Device::Set(const sock::ProxyParams& proxyparams, const QString& address, const uint16_t port, const QString& username, const QString& password, const uint64_t identifier)
{
  proxyparams_ = proxyparams;
  address_ = address;
  port_ = port;
  username_ = username;
  password_ = password;
  identifier_ = identifier;
  name_ = address;
  SignalNameChanged(name_);
}

void Device::Disconnected()
{
  const DEVICESTATE prevstate = state_;
  SetState(DEVICESTATE::DISCONNECTED, "Disconnected");
  if (prevstate == DEVICESTATE::SUBSCRIBED)
  {
    emit SignalDisconnected();
    QTimer::singleShot(RECONNECT_DELAY, this, &Device::Reconnect);
  }
  Connection::Disconnected();
}

void Device::Subscribe()
{
  Destroy();

  SetState(DEVICESTATE::CONNECTING, "Connecting");
  connect_ = Connection::Connect([this](const boost::system::error_code& err)
  {
    if (err)
    {
      SetState(DEVICESTATE::DISCONNECTED, "Failed to connect");
      QTimer::singleShot(RECONNECT_DELAY, this, &Device::Reconnect);
      return;
    }

    SetState(DEVICESTATE::CONNECTED, "Authorising");
    getauthenticatenonce_ = GetAuthenticationNonce([this](const std::chrono::steady_clock::duration latency, const monocle::client::GETAUTHENTICATIONNONCERESPONSE& getauthenticationnonceresponse)
    {
      emit SignalLatency(latency);
      if (getauthenticationnonceresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        SetState(DEVICESTATE::UNAUTHORIZED, "Unauthorised");
        QTimer::singleShot(RECONNECT_DELAY, this, &Device::Reconnect);
        return;
      }

      const std::string clientnonce = utility::GenerateRandomString(32);
      authenticate_ = Authenticate(username_.toStdString(), clientnonce, monocle::AuthenticateDigest(username_.toStdString(), password_.toStdString(), getauthenticationnonceresponse.authenticatenonce_, clientnonce), [this](const std::chrono::steady_clock::duration latency, const monocle::client::AUTHENTICATERESPONSE& authenticateresponse)
      {
        emit SignalLatency(latency);
        if (authenticateresponse.GetErrorCode() != monocle::ErrorCode::Success)
        {
          SetState(DEVICESTATE::UNAUTHORIZED, "Unauthorised");
          QTimer::singleShot(RECONNECT_DELAY, this, &Device::Reconnect);
          return;
        }

        gettime_ = GetTime([this](const std::chrono::steady_clock::duration latency, const monocle::client::GETTIMERESPONSE& gettimeresponse)
        {
          if (gettimeresponse.GetErrorCode() != monocle::ErrorCode::Success)
          {
            LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Failed to retrieve device time"));
            return;
          }

          utctimeoffset_ = static_cast<int64_t>(gettimeresponse.utctime_) - static_cast<int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
          emit MainWindow::Instance()->GetDeviceMgr().TimeOffsetChanged(boost::static_pointer_cast<Device>(shared_from_this()));
        });

        SetState(DEVICESTATE::AUTHORIZED, "Subscribing");
        subscribe_ = Connection::Subscribe([this](const std::chrono::steady_clock::duration latency, const monocle::client::SUBSCRIBERESPONSE& subscriberesponse)
        {
          emit SignalLatency(latency);
          if (subscriberesponse.GetErrorCode() != monocle::ErrorCode::Success)
          {
            SetState(DEVICESTATE::DISCONNECTED, "Failed to subscribe to server");
            QTimer::singleShot(RECONNECT_DELAY, this, &Device::Reconnect);
            return;
          }

          if (identifier_ != subscriberesponse.identifier_)
          {
            // This can happen if the user has edited the device and we have connected to another, or if the device itself at the ip has changed
            DestroyData();
          }

          if ((latitude_.toStdString() != subscriberesponse.latitude_) || (longitude_.toStdString() != subscriberesponse.longitude_))
          {
            latitude_ = QString::fromStdString(subscriberesponse.latitude_);
            longitude_ = QString::fromStdString(subscriberesponse.longitude_);
            SignalLocationChanged(latitude_, longitude_);
          }

          if (name_.toStdString() != subscriberesponse.name_)
          {
            name_ = QString::fromStdString(subscriberesponse.name_);
            emit SignalNameChanged(name_);
          }

          numcldevices_ = subscriberesponse.numcldevices_;
          numcudadevices_ = subscriberesponse.numcudadevices_;
          architecture_ = QString::fromStdString(subscriberesponse.architecture_);
          operatingsystem_ = utility::GetOperatingSystem(subscriberesponse.operatingsystem_);
          compiler_ = QString::fromStdString(subscriberesponse.compiler_);
          databasepath_ = QString::fromStdString(subscriberesponse.databasepath_);
          version_ = subscriberesponse.version_;
          environmentvariables_ = ToStrings(subscriberesponse.environmentalvariables_);
          commandlinevariables_ = ToStrings(subscriberesponse.commandlinevariables_);

          if (identifier_ != subscriberesponse.identifier_)
          {
            identifier_ = subscriberesponse.identifier_;
            MainWindow::Instance()->GetDeviceMgr().Save();
          }

          if (publickey_.Init(subscriberesponse.publickey_))
          {
            LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Failed to initialise server public key"));

          }

          // Add and update ONVIF users
          for (const monocle::ONVIFUSER& ou : subscriberesponse.onvifusers_)
          {
            std::vector< QSharedPointer<ONVIFUser> >::iterator i = std::find_if(onvifusers_.begin(), onvifusers_.end(), [&ou](const QSharedPointer<ONVIFUser>& onvifuser) { return (onvifuser->GetToken() == ou.token_); });
            if (i == onvifusers_.end())
            {
              QSharedPointer<ONVIFUser> onvifuser = QSharedPointer<ONVIFUser>::create(ou.token_, QString::fromStdString(ou.username_), ou.userlevel_);
              onvifusers_.push_back(onvifuser);
              emit SignalONVIFUserAdded(onvifuser);
            }
            else
            {
              if (((*i)->GetUsername().toStdString() != ou.username_) || ((*i)->GetUserlevel() != ou.userlevel_))
              {
                (*i)->SetUsername(QString::fromStdString(ou.username_));
                (*i)->SetUserlevel(ou.userlevel_);
                emit SignalONVIFUserChanged(*i);
              }
            }
          }

          // Remove any ONVIF users which do not appear
          for (std::vector< QSharedPointer<ONVIFUser> >::iterator i = onvifusers_.begin(); i != onvifusers_.end();)
          {
            if (std::find_if(subscriberesponse.onvifusers_.cbegin(), subscriberesponse.onvifusers_.cend(), [i](const monocle::ONVIFUSER& ou) { return (ou.token_ == (*i)->GetToken()); }) == subscriberesponse.onvifusers_.cend())
            {
              const uint64_t token = (*i)->GetToken();
              i = onvifusers_.erase(i);
              emit SignalONVIFUserRemoved(token);
            }
            else
            {
              ++i;

            }
          }

          // Add and update groups
          for (const monocle::GROUP& g : subscriberesponse.groups_)
          {
            std::vector< QSharedPointer<Group> >::iterator i = std::find_if(groups_.begin(), groups_.end(), [&g](const QSharedPointer<Group>& group) { return (group->GetToken() == g.token_); });
            if (i == groups_.end())
            {
              QSharedPointer<Group> group = QSharedPointer<Group>::create(g.token_, QString::fromStdString(g.name_), g.manageusers_, g.managerecordings_, g.managemaps_, g.managedevice_, g.allrecordings_, g.recordings_);
              groups_.push_back(group);
              emit SignalGroupAdded(group);
            }
            else
            {
              if (((*i)->GetName().toStdString() != g.name_) || ((*i)->GetManageUsers() != g.manageusers_) || ((*i)->GetManageRecordings() != g.managerecordings_) || ((*i)->GetManageMaps() != g.managemaps_) || ((*i)->GetManageDevice() != g.managedevice_) || ((*i)->GetAllRecordings() != g.allrecordings_) || !std::is_permutation((*i)->GetRecordings().cbegin(), (*i)->GetRecordings().cend(), g.recordings_.cbegin(), g.recordings_.cend()))
              {
                (*i)->SetName(QString::fromStdString(g.name_));
                (*i)->SetManageUsers(g.manageusers_);
                (*i)->SetManageRecordings(g.managerecordings_);
                (*i)->SetManageMaps(g.managemaps_);
                (*i)->SetManageDevice(g.managedevice_);
                (*i)->SetAllRecordings(g.allrecordings_);
                (*i)->SetRecordings(g.recordings_);
                emit SignalGroupChanged(*i);
              }
            }
          }

          // Remove any groups which do not appear
          for (std::vector< QSharedPointer<Group> >::iterator i = groups_.begin(); i != groups_.end();)
          {
            if (std::find_if(subscriberesponse.groups_.cbegin(), subscriberesponse.groups_.cend(), [i](const monocle::GROUP& g) { return (g.token_ == (*i)->GetToken()); }) == subscriberesponse.groups_.cend())
            {
              const uint64_t token = (*i)->GetToken();
              i = groups_.erase(i);
              emit SignalGroupRemoved(token);
            }
            else
            {
              ++i;

            }
          }

          // Add and update users
          for (const monocle::USER& u : subscriberesponse.users_)
          {
            std::vector< QSharedPointer<User> >::iterator i = std::find_if(users_.begin(), users_.end(), [&u](const QSharedPointer<User>& user) { return (user->GetToken() == u.token_); });
            if (i == users_.end())
            {
              QSharedPointer<User> user = QSharedPointer<User>::create(u.token_, QString::fromStdString(u.username_), u.grouptoken_);
              users_.push_back(user);
              emit SignalUserAdded(user);
            }
            else
            {
              if (((*i)->GetUsername().toStdString() != u.username_) || ((*i)->GetGroupToken() != u.grouptoken_))
              {
                (*i)->SetUsername(QString::fromStdString(u.username_));
                (*i)->SetGroupToken(u.grouptoken_);
                emit SignalUserChanged(*i);
              }
            }
          }

          // Remove any users which do not appear
          for (std::vector< QSharedPointer<User> >::iterator i = users_.begin(); i != users_.end();)
          {
            if (std::find_if(subscriberesponse.users_.cbegin(), subscriberesponse.users_.cend(), [i](const monocle::USER& u) { return (u.token_ == (*i)->GetToken()); }) == subscriberesponse.users_.cend())
            {
              const uint64_t token = (*i)->GetToken();
              i = users_.erase(i);
              emit SignalUserRemoved(token);
            }
            else
            {
              ++i;

            }
          }

          // Add and update files
          for (const monocle::FILE& f : subscriberesponse.files_)
          {
            std::vector< QSharedPointer<client::File> >::iterator i = std::find_if(files_.begin(), files_.end(), [&f](const QSharedPointer<client::File>& file) { return (file->GetToken() == f.token_); });
            if (i == files_.end())
            {
              QSharedPointer<client::File> file = QSharedPointer<client::File>::create(f.token_, QString::fromStdString(f.path_), QString::fromStdString(f.mountpoint_), f.numchunks_, f.chunksize_, f.automount_, f.state_, f.monitorstate_);
              files_.push_back(file);
              emit SignalFileAdded(file);
            }
            else
            {
              if (((*i)->GetPath().toStdString() != f.path_) || ((*i)->GetMountPoint().toStdString() != f.mountpoint_) || ((*i)->GetNumChunks() != f.numchunks_) || ((*i)->GetChunkSize() != f.chunksize_) || ((*i)->GetAutomount() != f.automount_) || ((*i)->GetState() != f.state_) || ((*i)->GetMonitorState() != f.monitorstate_))
              {
                // We are currently not sending signals for these changes in a file because they can not happen(at the moment)
                (*i)->SetPath(QString::fromStdString(f.path_));
                (*i)->SetMountPoint(QString::fromStdString(f.mountpoint_));
                (*i)->SetNumChunks(f.numchunks_);
                (*i)->SetChunkSize(f.chunksize_);
                (*i)->SetAutomount(f.automount_);

                (*i)->StateChanged(f.state_);
                (*i)->MonitorStateChanged(f.monitorstate_);
                emit SignalFileStateChanged(*i, f.state_);
                emit SignalFileMonitorStateChanged(*i, f.monitorstate_);
              }
            }
          }

          // Remove any files which do not appear
          for (std::vector< QSharedPointer<client::File> >::iterator i = files_.begin(); i != files_.end();)
          {
            if (std::find_if(subscriberesponse.files_.cbegin(), subscriberesponse.files_.cend(), [i](const monocle::FILE& f) { return (f.token_ == (*i)->GetToken()); }) == subscriberesponse.files_.cend())
            {
              const uint64_t token = (*i)->GetToken();
              i = files_.erase(i);
              emit SignalFileRemoved(token);
            }
            else
            {
              ++i;

            }
          }

          // Add and update receivers
          for (const monocle::RECEIVER& r : subscriberesponse.receivers_)
          {
            std::vector< QSharedPointer<client::Receiver> >::iterator i = std::find_if(receivers_.begin(), receivers_.end(), [&r](const QSharedPointer<client::Receiver>& receiver) { return (receiver->GetToken() == r.token_); });
            if (i == receivers_.end())
            {
              QSharedPointer<client::Receiver> receiver = QSharedPointer<client::Receiver>::create(r.token_, r.mode_, QString::fromStdString(r.mediauri_), r.autocreated_, QString::fromStdString(r.username_), QString::fromStdString(r.password_), ConvertStrings(r.parameters_), r.state_);
              receivers_.push_back(receiver);
              emit SignalReceiverAdded(receiver);
            }
            else
            {
              if (((*i)->GetReceiverMode() != r.mode_) || ((*i)->GetMediaUri().toStdString() != r.mediauri_) || ((*i)->GetAutoCreated() != r.autocreated_) || ((*i)->GetUsername().toStdString() != r.username_) || ((*i)->GetPassword().toStdString() != r.password_) || ((*i)->GetReceiverState() != r.state_) || !IsPermutation((*i)->GetParameters(), r.parameters_))
              {
                std::vector<QString> parameters;
                parameters.reserve(r.parameters_.size());
                for (const std::string& parameter : r.parameters_)
                {
                  parameters.push_back(QString::fromStdString(parameter));

                }

                (*i)->SetMode(r.mode_);
                (*i)->SetMediaUri(QString::fromStdString(r.mediauri_));
                (*i)->SetAutoCreated(r.autocreated_);
                (*i)->SetUsername(QString::fromStdString(r.username_));
                (*i)->SetPassword(QString::fromStdString(r.password_));
                (*i)->SetReceiverState(r.state_);
                (*i)->SetParameters(parameters);
                emit SignalReceiverChanged(*i);
              }
            }
          }

          // Add and update recordings
          for (const monocle::RECORDING& r : subscriberesponse.recordings_)
          {
            QSharedPointer<client::Recording> recording;
            std::vector< QSharedPointer<client::Recording> >::iterator i = std::find_if(recordings_.begin(), recordings_.end(), [&r](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == r.token_); });
            if (i == recordings_.end())
            {
              recording = QSharedPointer<client::Recording>::create(boost::static_pointer_cast<Device>(shared_from_this()), r.token_, QString::fromStdString(r.sourceid_), QString::fromStdString(r.name_), QString::fromStdString(r.location_), QString::fromStdString(r.description_), QString::fromStdString(r.address_), QString::fromStdString(r.content_), r.retentiontime_, r.activejob_, r.guiorder_);
              recordings_.push_back(recording);
              emit SignalRecordingAdded(recording);
            }
            else
            {
              recording = *i;
              if (((*i)->GetSourceId().toStdString() != r.sourceid_) || ((*i)->GetName().toStdString() != r.name_) || ((*i)->GetLocation().toStdString() != r.location_) || ((*i)->GetDescription().toStdString() != r.description_) || ((*i)->GetAddress().toStdString() != r.address_) || ((*i)->GetContent().toStdString() != r.content_))
              {
                (*i)->SetSourceId(QString::fromStdString(r.sourceid_));
                (*i)->SetName(QString::fromStdString(r.name_));
                (*i)->SetLocation(QString::fromStdString(r.location_));
                (*i)->SetDescription(QString::fromStdString(r.description_));
                (*i)->SetAddress(QString::fromStdString(r.address_));
                (*i)->SetContent(QString::fromStdString(r.content_));
                emit SignalRecordingChanged(recording);
              }
            }

            // Add or update recording tracks
            for (const monocle::RECORDINGTRACK& t : r.tracks_)
            {
              std::vector< QSharedPointer<client::RecordingTrack> >::const_iterator i = std::find_if(recording->GetTracks().cbegin(), recording->GetTracks().cend(), [&t](const QSharedPointer<client::RecordingTrack>& track) { return (track->GetId() == t.id_); });
              if (i == recording->GetTracks().cend())
              {
                emit SignalRecordingTrackAdded(recording, recording->AddTrack(t));

              }
              else
              {
                if (((*i)->GetToken().toStdString() != t.token_) || ((*i)->GetTrackType() != t.tracktype_) || ((*i)->GetDescription().toStdString() != t.description_) || ((*i)->GetFixedFiles() != t.fixedfiles_) || ((*i)->GetDigitalSigning() != t.digitalsignature_) || ((*i)->GetEncrypt() != t.encrypt_) || ((*i)->GetFlushFrequency() != t.flushfrequency_) || ((*i)->GetIndices() != t.indices_))
                {
                  (*i)->SetToken(QString::fromStdString(t.token_));
                  (*i)->SetTrackType(t.tracktype_);
                  (*i)->SetDescription(QString::fromStdString(t.description_));
                  (*i)->SetFixedFiles(t.fixedfiles_);
                  (*i)->SetDigitalSigning(t.digitalsignature_);
                  (*i)->SetEncrypt(t.encrypt_);
                  (*i)->SetFlushFrequency(t.flushfrequency_);
                  emit SignalRecordingTrackChanged(recording, *i);

                  (*i)->SetData(t.indices_);
                  emit recording->TrackSetData(*i, t.indices_);
                }
              }
            }

            // Remove any recording tracks which do not appear
            for (std::vector< QSharedPointer<client::RecordingTrack> >::iterator i = recording->GetTracks().begin(); i != recording->GetTracks().end();)
            {
              if (std::find_if(r.tracks_.cbegin(), r.tracks_.cend(), [i](const monocle::RECORDINGTRACK& t) { return (t.id_ == (*i)->GetId()); }) == r.tracks_.cend())
              {
                const uint64_t token = (*i)->GetId();
                i = recording->GetTracks().erase(i);
                emit SignalRecordingTrackRemoved(recording, token);
              }
              else
              {
                ++i;

              }
            }

            // Add or update recording jobs
            for (const monocle::RECORDINGJOB& rj : r.jobs_)
            {
              QSharedPointer<client::RecordingJob> recordingjob;
              std::vector< QSharedPointer<client::RecordingJob> >::const_iterator i = std::find_if(recording->GetRecordingJobs().cbegin(), recording->GetRecordingJobs().cend(), [&rj](const QSharedPointer<client::RecordingJob>& recordingjob) { return (recordingjob->GetToken() == rj.token_); });
              if (i == recording->GetRecordingJobs().cend())
              {
                recordingjob = QSharedPointer<client::RecordingJob>::create(boost::static_pointer_cast<Device>(shared_from_this()), rj.token_, QString::fromStdString(rj.name_), rj.enabled_, rj.priority_);
                recording->AddJob(recordingjob);
                emit SignalRecordingJobAdded(recording, recordingjob);
              }
              else
              {
                recordingjob = *i;
                if (((*i)->GetName().toStdString() != rj.name_) || ((*i)->GetEnabled() != rj.enabled_) || ((*i)->GetPriority() == rj.priority_))
                {
                  (*i)->SetName(QString::fromStdString(rj.name_));
                  (*i)->SetEnabled(rj.enabled_);
                  (*i)->SetPriority(rj.priority_);
                  emit SignalRecordingJobChanged(recording, recordingjob);
                }
              }

              // Add or update recording job sources
              for (const monocle::RECORDINGJOBSOURCE& source : rj.sources_)
              {
                QSharedPointer<RecordingJobSource> recordingjobsource;
                std::vector< QSharedPointer<RecordingJobSource> >::const_iterator i = std::find_if(recordingjob->GetSources().cbegin(), recordingjob->GetSources().cend(), [&source](const QSharedPointer<client::RecordingJobSource>& recordingjobsource) { return (recordingjobsource->GetToken() == source.token_); });
                if (i == recordingjob->GetSources().cend())
                {
                  recordingjobsource = QSharedPointer<RecordingJobSource>::create(source.token_, source.receivertoken_);
                  recordingjob->AddSource(recordingjobsource);
                  emit recording->JobSourceAdded(recordingjob, recordingjobsource);
                  emit SignalRecordingJobSourceAdded(recording, recordingjob, recordingjobsource);
                }
                else
                {
                  recordingjobsource = *i;
                  if (((*i)->GetReceiverToken() != source.receivertoken_))
                  {
                    (*i)->SetReceiverToken(source.receivertoken_);
                    emit SignalRecordingJobSourceChanged(recording, recordingjob, *i);
                  }
                }

                // Add or update recording job source tracks
                for (const monocle::RECORDINGJOBSOURCETRACK& track : source.tracks_)
                {
                  std::vector<QString> parameters;
                  parameters.reserve(track.parameters_.size());
                  for (const std::string& parameter : track.parameters_)
                  {
                    parameters.push_back(QString::fromStdString(parameter));

                  }

                  std::vector<QString> activeparameters;
                  activeparameters.reserve(track.activeparameters_.size());
                  for (const std::string& activeparameter : track.activeparameters_)
                  {
                    activeparameters.push_back(QString::fromStdString(activeparameter));

                  }
                  
                  std::vector< QSharedPointer<client::RecordingJobSourceTrack> >::const_iterator i = std::find_if(recordingjobsource->GetTracks().cbegin(), recordingjobsource->GetTracks().cend(), [&track](const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack) { return (recordingjobsourcetrack->GetToken() == track.token_); });
                  if (i == recordingjobsource->GetTracks().cend())
                  {
                    QSharedPointer<client::RecordingTrack> recordingtrack = recording->GetTrack(track.trackid_);
                    if (recordingtrack == nullptr)
                    {
                      LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find track: ") + QString::number(track.trackid_));
                      continue;
                    }

                    QSharedPointer<client::RecordingJobSourceTrack> recordingjobsourcetrack = QSharedPointer<client::RecordingJobSourceTrack>::create(track.token_, parameters, track.state_, QString::fromStdString(track.error_), activeparameters, recordingtrack);
                    recordingjobsource->AddTrack(recordingjobsourcetrack);
                    emit recording->JobSourceTrackAdded(recordingjob, recordingjobsource, recordingjobsourcetrack);
                    emit recordingjob->SourceTrackAdded(recordingjobsource, recordingjobsourcetrack);
                    emit SignalRecordingJobSourceTrackAdded(recording, recordingjob, recordingjobsource, recordingjobsourcetrack);
                  }
                  else
                  {
                    const std::vector<QString> tmpparameters = (*i)->GetParameters();
                    const std::vector<QString> tmpactiveparameters = (*i)->GetActiveParameters();
                    if (!std::is_permutation(tmpparameters.cbegin(), tmpparameters.cend(), parameters.cbegin(), parameters.cend()) || !std::is_permutation(tmpactiveparameters.cbegin(), tmpactiveparameters.cend(), activeparameters.cbegin(), activeparameters.cend()))
                    {
                      emit recording->JobSourceTrackChanged(recordingjob, recordingjobsource, *i);
                      emit SignalRecordingJobSourceTrackChanged(recording, recordingjob, recordingjobsource, *i);
                    }

                    if (((*i)->GetState() != track.state_) || ((*i)->GetError().toStdString() != track.error_))
                    {
                      emit SignalRecordingJobSourceTrackStateChanged(recording->GetToken(), recordingjob->GetToken(), recordingjobsource->GetToken(), (*i)->GetToken(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + utctimeoffset_, track.state_, QString::fromStdString(track.error_));

                    }
                  }
                }

                // Remove any recording job source tracks which do not appear
                for (std::vector< QSharedPointer<client::RecordingJobSourceTrack> >::iterator i = recordingjobsource->GetTracks().begin(); i != recordingjobsource->GetTracks().end();)
                {
                  if (std::find_if(source.tracks_.cbegin(), source.tracks_.cend(), [i](const monocle::RECORDINGJOBSOURCETRACK& t) { return (t.token_ == (*i)->GetToken()); }) == source.tracks_.cend())
                  {
                    const uint64_t token = (*i)->GetToken();
                    i = recordingjobsource->GetTracks().erase(i);
                    emit recording->JobSourceTrackRemoved(recordingjob, recordingjobsource, token);
                    emit recordingjob->SourceTrackRemoved(recordingjobsource, token);
                    emit SignalRecordingJobSourceTrackRemoved(recording, recordingjob, recordingjobsource, token);
                  }
                  else
                  {
                    ++i;

                  }
                }
              }

              // Remove any recording job sources which do not appear
              for (std::vector< QSharedPointer<RecordingJobSource> >::iterator i = recordingjob->GetSources().begin(); i != recordingjob->GetSources().end();)
              {
                if (std::find_if(rj.sources_.cbegin(), rj.sources_.cend(), [i](const monocle::RECORDINGJOBSOURCE& s) { return (s.token_ == (*i)->GetToken()); }) == rj.sources_.cend())
                {
                  const uint64_t token = (*i)->GetToken();
                  i = recordingjob->GetSources().erase(i);
                  emit SignalRecordingJobSourceRemoved(recording, recordingjob, token);
                }
                else
                {
                  ++i;

                }
              }
            }

            // Remove any recording jobs which do not appear
            for (std::vector< QSharedPointer<client::RecordingJob> >::iterator i = recording->GetRecordingJobs().begin(); i != recording->GetRecordingJobs().end();)
            {
              if (std::find_if(r.jobs_.cbegin(), r.jobs_.cend(), [i](const monocle::RECORDINGJOB& rj) { return (rj.token_ == (*i)->GetToken()); }) == r.jobs_.cend())
              {
                const uint64_t token = (*i)->GetToken();
                i = recording->GetRecordingJobs().erase(i);
                emit SignalRecordingJobRemoved(recording, token);
              }
              else
              {
                ++i;

              }
            }

            recording->SetActiveJob(r.activejob_);
          }

          // Remove any recordings which do not appear
          for (std::vector< QSharedPointer<client::Recording> >::iterator i = recordings_.begin(); i != recordings_.end();)
          {
            if (std::find_if(subscriberesponse.recordings_.cbegin(), subscriberesponse.recordings_.cend(), [i](const monocle::RECORDING& r) { return (r.token_ == (*i)->GetToken()); }) == subscriberesponse.recordings_.cend())
            {
              const uint64_t token = (*i)->GetToken();
              i = recordings_.erase(i);
              emit SignalRecordingRemoved(token);
            }
            else
            {
              ++i;

            }
          }

          // Remove any files which do not appear
          for (std::vector< QSharedPointer<client::Receiver> >::iterator i = receivers_.begin(); i != receivers_.end();)
          {
            if (std::find_if(subscriberesponse.receivers_.cbegin(), subscriberesponse.receivers_.cend(), [i](const monocle::RECEIVER& r) { return (r.token_ == (*i)->GetToken()); }) == subscriberesponse.receivers_.cend())
            {
              const uint64_t token = (*i)->GetToken();
              i = receivers_.erase(i);
              emit SignalReceiverRemoved(token);
            }
            else
            {
              ++i;

            }
          }

          logmessages_.insert(logmessages_.end(), subscriberesponse.serverlogmessages_.cbegin(), subscriberesponse.serverlogmessages_.cend());

          maxobjectdetectors_ = subscriberesponse.maxobjectdetectors_;
          maxrecordings_ = subscriberesponse.maxrecordings_;

          // Add and update maps
          for (const monocle::MAP& m : subscriberesponse.maps_)
          {
            std::vector< QSharedPointer<Map> >::iterator i = std::find_if(maps_.begin(), maps_.end(), [&m](const QSharedPointer<Map>& map) { return (map->GetToken() == m.token_); });
            if (i == maps_.end())
            {
              QSharedPointer<Map> map = QSharedPointer<Map>::create(boost::static_pointer_cast<Device>(shared_from_this()), m.token_, QString::fromStdString(m.name_), QString::fromStdString(m.location_), QString::fromStdString(m.imagemd5_), m.guiorder_);
              maps_.push_back(map);
              emit SignalMapAdded(map);
            }
            else
            {
              if (((*i)->GetName().toStdString() != m.name_) || ((*i)->GetLocation().toStdString() != m.location_) || ((*i)->GetImageMD5().toStdString() != m.imagemd5_))
              {
                (*i)->SetName(QString::fromStdString(m.name_));
                (*i)->SetLocation(QString::fromStdString(m.location_));
                (*i)->SetImageMD5(QString::fromStdString(m.imagemd5_));
                emit SignalMapChanged(*i);
              }
            }
          }

          // Remove any maps which do not appear
          for (std::vector< QSharedPointer<Map> >::iterator i = maps_.begin(); i != maps_.end();)
          {
            if (std::find_if(subscriberesponse.maps_.cbegin(), subscriberesponse.maps_.cend(), [i](const monocle::MAP& g) { return (g.token_ == (*i)->GetToken()); }) == subscriberesponse.maps_.cend())
            {
              const uint64_t token = (*i)->GetToken();
              i = maps_.erase(i);
              emit SignalMapRemoved(token);
            }
            else
            {
              ++i;

            }
          }

          // Remove any maps from the filesystem which don't appear(but only from this device!)
          QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
          dir.setNameFilters(QStringList("*.map"));
          dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
          const QString identifier = QString::number(identifier_);
          for (const QString& filename : dir.entryList())
          {
            if (!filename.startsWith(identifier)) // Ignore other devices that we don't know about
            {

              continue;
            }

            if (std::find_if(maps_.cbegin(), maps_.cend(), [&filename](const QSharedPointer<Map>& map) { return (filename.endsWith(QString::number(map->GetToken()) + ".map")); }) == maps_.cend())
            {
              if (!QFile::remove(dir.filePath(filename)))
              {
                LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Failed to remove file: ") + filename);

              }
            }
          }

          // Add and update mount points
          for (const monocle::MOUNTPOINT& m : subscriberesponse.mountpoints_)
          {
            const MOUNTPOINT mountpoint = MOUNTPOINT(m.id_, m.parentid_, m.majorstdev_, m.minorstdev_, QString::fromStdString(m.path_), QString::fromStdString(m.type_), QString::fromStdString(m.source_));
            std::vector<MOUNTPOINT>::const_iterator i = std::find(mountpoints_.cbegin(), mountpoints_.cend(), mountpoint);
            if (i == mountpoints_.cend())
            {
              mountpoints_.push_back(mountpoint);
              emit SignalMountPointAdded(mountpoint);
            }
          }

          // Remove any mount points which do not appear
          for (std::vector<MOUNTPOINT>::iterator i = mountpoints_.begin(); i != mountpoints_.end();)
          {
            const monocle::MOUNTPOINT mountpoint = monocle::MOUNTPOINT(i->id_, i->parentid_, i->majorstdev_, i->minorstdev_, i->path_.toStdString(), i->type_.toStdString(), i->source_.toStdString());
            if (std::find(subscriberesponse.mountpoints_.cbegin(), subscriberesponse.mountpoints_.cend(), mountpoint) == subscriberesponse.mountpoints_.cend())
            {
              const MOUNTPOINT m = *i;
              i = mountpoints_.erase(i);
              emit SignalMountPointRemoved(m);
            }
            else
            {
              ++i;

            }
          }

          // Add and update layouts
          for (const monocle::LAYOUT& l : subscriberesponse.layouts_)
          {
            std::vector< QSharedPointer<Layout> >::iterator i = std::find_if(layouts_.begin(), layouts_.end(), [&l](const QSharedPointer<Layout>& layout) { return (layout->GetToken() == l.token_); });
            if (i == layouts_.end())
            {
              QSharedPointer<Layout> layout = QSharedPointer<Layout>::create(boost::static_pointer_cast<Device>(shared_from_this()), l);
              layouts_.push_back(layout);
              emit SignalLayoutAdded(layout);
              emit MainWindow::Instance()->GetDeviceMgr().LayoutAdded(layout);
            }
            else
            {
              (*i)->SetConfiguration(l);
              emit SignalLayoutChanged(*i);
              emit MainWindow::Instance()->GetDeviceMgr().LayoutChanged(*i);
            }
          }

          // Remove any layouts which do not appear
          for (std::vector< QSharedPointer<Layout> >::iterator i = layouts_.begin(); i != layouts_.end();)
          {
            if (std::find_if(subscriberesponse.layouts_.cbegin(), subscriberesponse.layouts_.cend(), [i](const monocle::LAYOUT& l) { return (l.token_ == (*i)->GetToken()); }) == subscriberesponse.layouts_.cend())
            {
              const uint64_t token = (*i)->GetToken();
              i = layouts_.erase(i);
              emit SignalLayoutRemoved(token);
            }
            else
            {
              ++i;

            }
          }

          SetState(DEVICESTATE::SUBSCRIBED, QString());
          emit SignalLicenseStateChanged(IsValidLicense());
        });
      });
    });
  });
}

bool Device::IsLinux() const
{
  if (!operatingsystem_.is_initialized())
  {

    return false;
  }
  return utility::IsLinux(*operatingsystem_);
}

bool Device::IsWindows() const
{
  if (!operatingsystem_.is_initialized())
  {

    return false;
  }
  return utility::IsWindows(*operatingsystem_);
}

const QSharedPointer<client::File> Device::GetFile(const uint64_t token) const
{
  std::vector< QSharedPointer<client::File> >::const_iterator file = std::find_if(files_.cbegin(), files_.cend(), [token](const QSharedPointer<client::File>& file) { return (file->GetToken() == token); });
  if (file == files_.cend())
  {

    return nullptr;
  }
  return *file;
}

const QSharedPointer<client::Recording> Device::GetRecording(const uint64_t token) const
{
  std::vector< QSharedPointer<client::Recording> >::const_iterator recording = std::find_if(recordings_.cbegin(), recordings_.cend(), [token](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == token); });
  if (recording == recordings_.cend())
  {

    return nullptr;
  }
  return *recording;
}

QSharedPointer<Group> Device::GetGroup(const uint64_t token) const
{
  std::vector< QSharedPointer<Group> >::const_iterator group = std::find_if(groups_.cbegin(), groups_.cend(), [token](const QSharedPointer<Group>& group) { return (group->GetToken() == token); });
  if (group == groups_.cend())
  {

    return nullptr;
  }
  return *group;
}

QSharedPointer<ONVIFUser> Device::GetONVIFUser(const uint64_t token) const
{
  std::vector< QSharedPointer<ONVIFUser> >::const_iterator onvifuser = std::find_if(onvifusers_.cbegin(), onvifusers_.cend(), [token](const QSharedPointer<ONVIFUser>& onvifuser) { return (onvifuser->GetToken() == token); });
  if (onvifuser == onvifusers_.cend())
  {

    return nullptr;
  }
  return *onvifuser;
}

QSharedPointer<client::Receiver> Device::GetReceiver(const uint64_t token) const
{
  std::vector< QSharedPointer<client::Receiver> >::const_iterator receiver = std::find_if(receivers_.cbegin(), receivers_.cend(), [token](const QSharedPointer<client::Receiver>& receiver) { return (receiver->GetToken() == token); });
  if (receiver == receivers_.cend())
  {

    return nullptr;
  }
  return *receiver;
}

QSharedPointer<User> Device::GetUser(const uint64_t token) const
{
  std::vector< QSharedPointer<User> >::const_iterator user = std::find_if(users_.cbegin(), users_.cend(), [token](const QSharedPointer<User>& user) { return (user->GetToken() == token); });
  if (user == users_.cend())
  {

    return nullptr;
  }
  return *user;
}

QSharedPointer<Map> Device::GetMap(const uint64_t token) const
{
  std::vector< QSharedPointer<Map> >::const_iterator map = std::find_if(maps_.cbegin(), maps_.cend(), [token](const QSharedPointer<Map>& map) { return (map->GetToken() == token); });
  if (map == maps_.cend())
  {

    return nullptr;
  }
  return *map;
}

QSharedPointer<Layout> Device::GetLayout(const uint64_t token) const
{
  std::vector< QSharedPointer<Layout> >::const_iterator layout = std::find_if(layouts_.cbegin(), layouts_.cend(), [token](const QSharedPointer<Layout>& layout) { return (layout->GetToken() == token); });
  if (layout == layouts_.cend())
  {

    return nullptr;
  }
  return *layout;
}

QStringList Device::GetLocations() const
{
  QStringList locations;

  for (const QSharedPointer<Recording>& recording : recordings_)
  {
    locations += recording->GetLocation();

  }

  for (const QSharedPointer<Map>& map : maps_)
  {
    locations += map->GetLocation();

  }

  return locations;
}

void Device::Reconnect()
{
  if (state_ == DEVICESTATE::SUBSCRIBED) // This shouldn't happen, but just in case
  {

    return;
  }

  Subscribe();
}

void Device::timerEvent(QTimerEvent* event)
{
  if (event->timerId() == keepalivetimer_)
  {
    keepaliveconnection_ = Keepalive([this](const std::chrono::steady_clock::duration latency, const monocle::client::KEEPALIVERESPONSE& keepaliveresponse)
    {
      if (keepaliveresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        SetState(DEVICESTATE::DISCONNECTED, "Disconnected");
        QTimer::singleShot(RECONNECT_DELAY, this, &Device::Reconnect);
      }
      else
      {
        emit SignalLatency(latency);

      }
    });
  }
}

bool Device::SupportsMaps() const
{
  if (version_ < utility::Version(1, 1, 0))
  {

    return false;
  }
  return true;
}

bool Device::SupportsRecordingLocation() const
{
  if (version_ < utility::Version(1, 1, 0))
  {

    return false;
  }
  return true;
}

bool Device::SupportsGetChildFolders() const
{
  if (version_ < utility::Version(1, 2, 0))
  {

    return false;
  }
  return true;
}

bool Device::SupportsManageDevice() const
{
  if (version_ < utility::Version(1, 4, 0))
  {

    return false;
  }
  return true;
}

bool Device::SupportsDeviceLocation() const
{
  if (version_ < utility::Version(1, 5, 0))
  {

    return false;
  }
  return true;
}

bool Device::SupportsFindMotion() const
{
  if (version_ < utility::Version(1, 9, 0))
  {

    return false;
  }
  return true;
}

bool Device::SupportsFindObject() const
{
  if (version_ < utility::Version(1, 11, 0))
  {

    return false;
  }
  return true;
}

bool Device::SupportsObjectDetection() const
{
  if (version_ < utility::Version(1, 10, 0))
  {

    return false;
  }
  return true;
}

bool Device::SupportsCreateDefaultJob() const
{
  if (version_ < utility::Version(1, 10, 0))
  {

    return false;
  }
  return true;
}

bool Device::SupportsTrackCodec() const
{
  if (version_ < utility::Version(1, 11, 0))
  {

    return false;
  }
  return true;
}

bool Device::SupportsGetChildFoldersFilter() const
{
  if (version_ < utility::Version(1, 11, 0))
  {

    return false;
  }
  return true;
}

bool Device::SupportsLayouts() const
{
  if (version_ < utility::Version(1, 12, 0))
  {

    return false;
  }
  return true;
}

bool Device::CanManageUsers()
{
  std::vector< QSharedPointer<User> >::const_iterator user = std::find_if(users_.cbegin(), users_.cend(), [this](const QSharedPointer<User>& user) { return (user->GetUsername() == username_); });
  if (user == users_.cend())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find user: ") + username_);
    return true;
  }

  std::vector< QSharedPointer<Group> >::const_iterator group = std::find_if(groups_.cbegin(), groups_.cend(), [&user](const QSharedPointer<Group>& group) { return (group->GetToken() == (*user)->GetGroupToken()); });
  if (group == groups_.cend())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find user: ") + QString::number((*user)->GetGroupToken()));
    return true;
  }

  return (*group)->GetManageUsers();
}

bool Device::CanManageRecordings()
{
  std::vector< QSharedPointer<User> >::const_iterator user = std::find_if(users_.cbegin(), users_.cend(), [this](const QSharedPointer<User>& user) { return (user->GetUsername() == username_); });
  if (user == users_.cend())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find user: ") + username_);
    return true;
  }

  std::vector< QSharedPointer<Group> >::const_iterator group = std::find_if(groups_.cbegin(), groups_.cend(), [&user](const QSharedPointer<Group>& group) { return (group->GetToken() == (*user)->GetGroupToken()); });
  if (group == groups_.cend())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find user: ") + QString::number((*user)->GetGroupToken()));
    return true;
  }

  return (*group)->GetManageRecordings();
}

bool Device::CanManageMaps()
{
  if (!SupportsMaps())
  {

    return false;
  }

  std::vector< QSharedPointer<User> >::const_iterator user = std::find_if(users_.cbegin(), users_.cend(), [this](const QSharedPointer<User>& user) { return (user->GetUsername() == username_); });
  if (user == users_.cend())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find user: ") + username_);
    return true;
  }

  std::vector< QSharedPointer<Group> >::const_iterator group = std::find_if(groups_.cbegin(), groups_.cend(), [&user](const QSharedPointer<Group>& group) { return (group->GetToken() == (*user)->GetGroupToken()); });
  if (group == groups_.cend())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find user: ") + QString::number((*user)->GetGroupToken()));
    return true;
  }

  return (*group)->GetManageMaps();
}

bool Device::CanManageDevice()
{
  if (!SupportsManageDevice())
  {

    return false;
  }

  std::vector< QSharedPointer<User> >::const_iterator user = std::find_if(users_.cbegin(), users_.cend(), [this](const QSharedPointer<User>& user) { return (user->GetUsername() == username_); });
  if (user == users_.cend())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find user: ") + username_);
    return true;
  }

  std::vector< QSharedPointer<Group> >::const_iterator group = std::find_if(groups_.cbegin(), groups_.cend(), [&user](const QSharedPointer<Group>& group) { return (group->GetToken() == (*user)->GetGroupToken()); });
  if (group == groups_.cend())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find user: ") + QString::number((*user)->GetGroupToken()));
    return true;
  }

  return (*group)->GetManageDevice();
}

size_t Device::GetNumObjectDetectors() const
{
  size_t totalobjectdetectors = 0;
  for (const QSharedPointer<Recording>& recording : recordings_)
  {
    totalobjectdetectors += recording->GetNumObjectDetectors();

  }
  return totalobjectdetectors;
}

bool Device::IsValidLicense() const
{
  if (GetNumObjectDetectors() > maxobjectdetectors_)
  {

    return false;
  }

  if (recordings_.size() > maxrecordings_)
  {

    return false;
  }
  return true;
}

void Device::SetState(const DEVICESTATE state, const QString& message)
{
  if (state_ == state)
  {

    return;
  }

  if (state == DEVICESTATE::SUBSCRIBED)
  {
    keepalivetimer_ = startTimer(std::chrono::seconds(10));

  }
  else
  {
    if (keepalivetimer_ != -1)
    {
      killTimer(keepalivetimer_);
      keepalivetimer_ = -1;
    }
  }

  // Mark all the recording job source tracks as idle or error if we were previously subscribed
  if (state_ == DEVICESTATE::SUBSCRIBED)
  {
    for (const QSharedPointer<Recording>& recording : recordings_)
    {
      for (const QSharedPointer<RecordingJob>& recordingjob : recording->GetJobs())
      {
        for (const QSharedPointer<RecordingJobSource>& recordingjobsource : recordingjob->GetSources())
        {
          for (const QSharedPointer<RecordingJobSourceTrack>& recordingjobsourcetrack : recordingjobsource->GetTracks())
          {
            const monocle::RecordingJobState prevstate = recordingjobsourcetrack->GetState();
            const QString error("Disconnected");
            if ((prevstate == monocle::RecordingJobState::Error) && (error == recordingjobsourcetrack->GetError()))
            {
              // Ignore this, nothing is really changing
              continue;
            }

            const uint64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + utctimeoffset_;
            recordingjobsourcetrack->GetTrack()->SetState(time, monocle::RecordingJobState::Error, prevstate);
            recordingjobsourcetrack->SetState(monocle::RecordingJobState::Error, error);
            emit recording->JobSourceTrackStateChanged(recordingjob, recordingjobsource, recordingjobsourcetrack, time, monocle::RecordingJobState::Error, error, prevstate);
          }
        }
      }
    }
  }

  state_ = state;
  emit SignalStateChanged(state_, message);
}

std::vector<QString> Device::ToStrings(const std::vector<std::string>& strings) const
{
  std::vector<QString> result;
  result.reserve(strings.size());
  for (const std::string& string : strings)
  {
    result.push_back(QString::fromStdString(string));
  }
  return result;
}

bool Device::IsPermutation(const std::vector<QString>& lhs, const std::vector<std::string>& rhs) const
{
  std::vector<std::string> tmp;
  tmp.reserve(lhs.size());
  for (const QString& s : lhs)
  {
    tmp.push_back(s.toStdString());

  }

  return std::is_permutation(tmp.cbegin(), tmp.cend(), rhs.cbegin(), rhs.cend());
}

void Device::SlotDiscoveryHello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector<QString> tmpaddresses;
  tmpaddresses.reserve(addresses.size());
  for (const std::string& address : addresses)
  {
    tmpaddresses.push_back(QString::fromStdString(address));

  }

  std::vector<QString> tmpscopes;
  tmpscopes.reserve(scopes.size());
  for (const std::string& scope : scopes)
  {
    tmpscopes.push_back(QString::fromStdString(scope));

  }

  emit SignalDiscoveryHello(tmpaddresses, tmpscopes);
}

void Device::SlotFileAdded(const uint64_t filetoken, const std::string& path, const std::string& mountpoint, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::File> >::iterator f = std::find_if(files_.begin(), files_.end(), [filetoken](const QSharedPointer<client::File>& file) { return (file->GetToken() == filetoken); });
  if (f == files_.end())
  {
    QSharedPointer<client::File> file = QSharedPointer<client::File>::create(filetoken, QString::fromStdString(path), QString::fromStdString(mountpoint), numchunks, chunksize, automount, state, monitorstate);
    files_.push_back(file);
    emit SignalFileAdded(file);
  }
  else
  {
    if (((*f)->GetMountPoint().toStdString() != mountpoint) || ((*f)->GetPath().toStdString() != path) || ((*f)->GetNumChunks() != numchunks) || ((*f)->GetChunkSize() != chunksize) || ((*f)->GetAutomount() != automount) || ((*f)->GetState() != state) || ((*f)->GetMonitorState() != monitorstate))
    {
      emit SignalFileChanged(*f);

    }
  }
}

void Device::SlotFileRemoved(const uint64_t token)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::File> >::iterator f = std::find_if(files_.begin(), files_.end(), [token](const QSharedPointer<client::File>& file) { return (file->GetToken() == token); });
  if (f == files_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find file: ") + QString::number(token));
    return;
  }
  files_.erase(f);
  emit SignalFileRemoved(token);
}

void Device::SlotFileMonitorStateChanged(const uint64_t token, const monocle::FileMonitorState monitorstate)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::File> >::iterator f = std::find_if(files_.begin(), files_.end(), [token](const QSharedPointer<client::File>& file) { return (file->GetToken() == token); });
  if (f == files_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find file: ") + QString::number(token));
    return;
  }
  (*f)->MonitorStateChanged(monitorstate);
  emit SignalFileMonitorStateChanged(*f, monitorstate);
}

void Device::SlotFileStateChanged(const uint64_t token, const monocle::FileState state)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::File> >::iterator f = std::find_if(files_.begin(), files_.end(), [token](const QSharedPointer<client::File>& file) { return (file->GetToken() == token); });
  if (f == files_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find file: ") + QString::number(token));
    return;
  }
  (*f)->StateChanged(state);
  emit SignalFileStateChanged(*f, state);
}

void Device::SlotGroupAdded(const uint64_t token, const QString& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<Group> >::iterator g = std::find_if(groups_.begin(), groups_.end(), [token](const QSharedPointer<Group>& group) { return (group->GetToken() == token); });
  if (g == groups_.end())
  {
    QSharedPointer<Group> group = QSharedPointer<Group>::create(token, name, manageusers, managerecordings, managemaps, managedevice, allrecordings, recordings);
    groups_.push_back(group);
    emit SignalGroupAdded(group);
  }
  else
  {
    if (((*g)->GetName() != name) || ((*g)->GetManageUsers() != manageusers) || ((*g)->GetManageRecordings() != managerecordings) || ((*g)->GetManageMaps() != managemaps) || ((*g)->GetManageDevice() != managedevice) || ((*g)->GetAllRecordings() != allrecordings) || !std::is_permutation((*g)->GetRecordings().cbegin(), (*g)->GetRecordings().cend(), recordings.cbegin(), recordings.cend()))
    {
      (*g)->SetName(name);
      (*g)->SetManageUsers(manageusers);
      (*g)->SetManageRecordings(managerecordings);
      (*g)->SetManageMaps(managemaps);
      (*g)->SetManageDevice(managedevice);
      (*g)->SetAllRecordings(allrecordings);
      (*g)->SetRecordings(recordings);
      emit SignalGroupChanged(*g);
    }
  }
}

void Device::SlotGroupChanged(const uint64_t token, const QString& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<Group> >::iterator g = std::find_if(groups_.begin(), groups_.end(), [token](const QSharedPointer<Group>& group) { return (group->GetToken() == token); });
  if (g == groups_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find group: ") + QString::number(token));
    return;
  }

  (*g)->SetName(name);
  (*g)->SetManageUsers(manageusers);
  (*g)->SetManageRecordings(managerecordings);
  (*g)->SetManageMaps(managemaps);
  (*g)->SetManageDevice(managedevice);
  (*g)->SetAllRecordings(allrecordings);
  (*g)->SetRecordings(recordings);
  emit SignalGroupChanged(*g);
}

void Device::SlotGroupRemoved(const uint64_t token)
{
  std::vector< QSharedPointer<Group> >::iterator g = std::find_if(groups_.begin(), groups_.end(), [token](const QSharedPointer<Group>& group) { return (group->GetToken() == token); });
  if (g == groups_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find group: ") + QString::number(token));
    return;
  }
  groups_.erase(g);
  emit SignalGroupRemoved(token);
}

void Device::SlotONVIFUserAdded(const uint64_t token, const QString& username, const monocle::ONVIFUserlevel onvifuserlevel)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<ONVIFUser> >::iterator u = std::find_if(onvifusers_.begin(), onvifusers_.end(), [token](const QSharedPointer<ONVIFUser>& onvifuser) { return (onvifuser->GetToken() == token); });
  if (u == onvifusers_.end())
  {
    QSharedPointer<ONVIFUser> onvifuser = QSharedPointer<ONVIFUser>::create(token, username, onvifuserlevel);
    onvifusers_.push_back(onvifuser);
    emit SignalONVIFUserAdded(onvifuser);
  }
  else
  {
    if (((*u)->GetUsername() != username) || ((*u)->GetUserlevel() != onvifuserlevel))
    {
      (*u)->SetUsername(username);
      (*u)->SetUserlevel(onvifuserlevel);
      emit SignalONVIFUserChanged(*u);
    }
  }
}

void Device::SlotONVIFUserChanged(const uint64_t token, const boost::optional<QString>& username, const monocle::ONVIFUserlevel onvifuserlevel)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<ONVIFUser> >::iterator u = std::find_if(onvifusers_.begin(), onvifusers_.end(), [token](const QSharedPointer<ONVIFUser>& onvifuser) { return (onvifuser->GetToken() == token); });
  if (u == onvifusers_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find ONVIF user: ") + QString::number(token));
    return;
  }
  if (username.is_initialized())
  {
    (*u)->SetUsername(*username);

  }
  (*u)->SetUserlevel(onvifuserlevel);
  emit SignalONVIFUserChanged(*u);
}

void Device::SlotONVIFUserRemoved(const uint64_t token)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<ONVIFUser> >::iterator u = std::find_if(onvifusers_.begin(), onvifusers_.end(), [token](const QSharedPointer<ONVIFUser>& onvifuser) { return (onvifuser->GetToken() == token); });
  if (u == onvifusers_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find ONVIF user: ") + QString::number(token));
    return;
  }
  onvifusers_.erase(u);
  emit SignalONVIFUserRemoved(token);
}

void Device::SlotGuiOrderChanged(const std::vector< std::pair<uint64_t, uint64_t> >& recordingsorder, const std::vector< std::pair<uint64_t, uint64_t> >& mapsorder)
{
  for (const std::pair<uint64_t, uint64_t>& recordingorder : recordingsorder)
  {
    std::vector< QSharedPointer<client::Recording> >::iterator r = std::find_if(recordings_.begin(), recordings_.end(), [&recordingorder](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingorder.first); });
    if (r == recordings_.end())
    {
      LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingorder.first));
      continue;
    }
    (*r)->SetGuiOrder(recordingorder.second);
  }

  for (const std::pair<uint64_t, uint64_t>& maporder : mapsorder)
  {
    std::vector< QSharedPointer<client::Map> >::iterator m = std::find_if(maps_.begin(), maps_.end(), [&maporder](const QSharedPointer<client::Map>& map) { return (map->GetToken() == maporder.first); });
    if (m == maps_.end())
    {
      LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find map: ") + QString::number(maporder.first));
      continue;
    }
    (*m)->SetGuiOrder(maporder.second);
  }
  emit MainWindow::Instance()->GetDeviceMgr().GuiOrderChanged(boost::static_pointer_cast<Device>(shared_from_this()), recordingsorder, mapsorder);
}

void Device::SlotLayoutAdded(const monocle::LAYOUT& layout)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<Layout> >::iterator i = std::find_if(layouts_.begin(), layouts_.end(), [&layout](const QSharedPointer<Layout>& l) { return (l->GetToken() == layout.token_); });
  if (i == layouts_.end())
  {
    QSharedPointer<Layout> l = QSharedPointer<Layout>::create(boost::static_pointer_cast<Device>(shared_from_this()), layout);
    layouts_.push_back(l);
    emit SignalLayoutAdded(l);
    emit MainWindow::Instance()->GetDeviceMgr().LayoutAdded(l);
  }
  else
  {
    (*i)->SetConfiguration(layout);
    emit SignalLayoutChanged(*i);
    emit MainWindow::Instance()->GetDeviceMgr().LayoutChanged(*i);
  }
}

void Device::SlotLayoutChanged(const monocle::LAYOUT& layout)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<Layout> >::iterator i = std::find_if(layouts_.begin(), layouts_.end(), [&layout](const QSharedPointer<Layout>& l) { return (l->GetToken() == layout.token_); });
  if (i == layouts_.end())
  {
    QSharedPointer<Layout> l = QSharedPointer<Layout>::create(boost::static_pointer_cast<Device>(shared_from_this()), layout);
    layouts_.push_back(l);
    emit SignalLayoutAdded(l);
    emit MainWindow::Instance()->GetDeviceMgr().LayoutAdded(l);
  }
  else
  {
    (*i)->SetConfiguration(layout);
    emit SignalLayoutChanged(*i);
    emit MainWindow::Instance()->GetDeviceMgr().LayoutChanged(*i);
  }
}

void Device::SlotLayoutNameChanged(const uint64_t token, const QString& name)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<Layout> >::iterator i = std::find_if(layouts_.begin(), layouts_.end(), [token](const QSharedPointer<Layout>& l) { return (l->GetToken() == token); });
  if (i != layouts_.end())
  {
    (*i)->SetName(name);
    emit SignalLayoutChanged(*i);
    emit MainWindow::Instance()->GetDeviceMgr().LayoutChanged(*i);
  }
}

void Device::SlotLayoutRemoved(const uint64_t token)
{
  std::vector< QSharedPointer<Layout> >::iterator l = std::find_if(layouts_.begin(), layouts_.end(), [token](const QSharedPointer<Layout>& layout) { return (layout->GetToken() == token); });
  if (l == layouts_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find layout: ") + QString::number(token));
    return;
  }
  layouts_.erase(l);
  emit SignalLayoutRemoved(token);
  emit MainWindow::Instance()->GetDeviceMgr().LayoutRemoved(token);
}

void Device::SlotMapAdded(const uint64_t token, const QString& name, const QString& location, const QString& imagemd5)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<Map> >::iterator m = std::find_if(maps_.begin(), maps_.end(), [token](const QSharedPointer<Map>& map) { return (map->GetToken() == token); });
  if (m == maps_.end())
  {
    QSharedPointer<Map> map = QSharedPointer<Map>::create(boost::static_pointer_cast<Device>(shared_from_this()), token, name, location, imagemd5, 0);
    maps_.push_back(map);
    emit SignalMapAdded(map);
  }
  else
  {
    if (((*m)->GetName() != name) || ((*m)->GetLocation() != location) || ((*m)->GetImageMD5() != imagemd5))
    {
      (*m)->SetName(name);
      (*m)->SetLocation(location);
      (*m)->SetImageMD5(imagemd5);
      emit SignalMapChanged(*m);
    }
  }
}

void Device::SlotMapChanged(const uint64_t token, const QString& name, const QString& location, const QString& imagemd5)
{
  std::vector< QSharedPointer<Map> >::iterator m = std::find_if(maps_.begin(), maps_.end(), [token](const QSharedPointer<Map>& map) { return (map->GetToken() == token); });
  if (m == maps_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find map: ") + QString::number(token));
    return;
  }

  (*m)->SetName(name);
  (*m)->SetLocation(location);
  (*m)->SetImageMD5(imagemd5);
  emit SignalMapChanged(*m);
}

void Device::SlotMapRemoved(const uint64_t token)
{
  std::vector< QSharedPointer<Map> >::iterator m = std::find_if(maps_.begin(), maps_.end(), [token](const QSharedPointer<Map>& map) { return (map->GetToken() == token); });
  if (m == maps_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find map: ") + QString::number(token));
    return;
  }
  maps_.erase(m);
  emit SignalMapRemoved(token);
}

void Device::SlotMountPointAdded(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const QString& path, const QString& type, const QString& source)
{
  const MOUNTPOINT mountpoint(id, parentid, majorstdev, minorstdev, path, type, source);
  if (std::find(mountpoints_.cbegin(), mountpoints_.cend(), mountpoint) != mountpoints_.cend())
  {

    return;
  }
  mountpoints_.push_back(mountpoint);
}

void Device::SlotMountPointRemoved(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const QString& path, const QString& type, const QString& source)
{
  const MOUNTPOINT mountpoint(id, parentid, majorstdev, minorstdev, path, type, source);
  std::vector<MOUNTPOINT>::iterator i = std::find(mountpoints_.begin(), mountpoints_.end(), mountpoint);
  if (i == mountpoints_.end())
  {

    return;
  }
  mountpoints_.erase(i);
}

void Device::SlotLocationChanged(const QString& latitude, const QString& longitude)
{
  latitude_ = latitude;
  longitude_ = longitude;
}

void Device::SlotNameChanged(const QString& name)
{
  name_ = name;

}

void Device::SlotReceiverAdded(const uint64_t token, const monocle::ReceiverMode mode, const QString& mediauri, const bool autocreated, const QString& username, const QString& password, const std::vector<QString>& parameters, const monocle::ReceiverState state)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Receiver> >::iterator r = std::find_if(receivers_.begin(), receivers_.end(), [token](const QSharedPointer<client::Receiver>& receiver) { return (receiver->GetToken() == token); });
  if (r == receivers_.end())
  {
    QSharedPointer<client::Receiver> receiver = QSharedPointer<client::Receiver>::create(token, mode, mediauri, autocreated, username, password, parameters, state);
    receivers_.push_back(receiver);
    emit SignalReceiverAdded(receiver);
  }
  else
  {
    const std::vector<QString> tmp = (*r)->GetParameters();
    if (((*r)->GetReceiverMode() != mode) || ((*r)->GetMediaUri() != mediauri) || ((*r)->GetAutoCreated() != autocreated) || ((*r)->GetUsername() != username) || ((*r)->GetPassword() != password) || ((*r)->GetReceiverState() != state) || !std::is_permutation(tmp.cbegin(), tmp.cend(), parameters.cbegin(), parameters.cend()))
    {
      (*r)->SetMode(mode);
      (*r)->SetMediaUri(mediauri);
      (*r)->SetAutoCreated(autocreated);
      (*r)->SetUsername(username);
      (*r)->SetPassword(password);
      (*r)->SetReceiverState(state);
      (*r)->SetParameters(parameters);
      emit SignalReceiverChanged(*r);
    }
  }
}

void Device::SlotReceiverChanged(const uint64_t token, const monocle::ReceiverMode mode, const QString& mediauri, const bool autocreated, const QString& username, const QString& password, const std::vector<QString>& parameters)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Receiver> >::iterator r = std::find_if(receivers_.begin(), receivers_.end(), [token](const QSharedPointer<client::Receiver>& receiver) { return (receiver->GetToken() == token); });
  if (r == receivers_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find receiver: ") + QString::number(token));
    return;
  }

  (*r)->SetConfiguration(mode, mediauri, autocreated, username, password, parameters);
  emit SignalReceiverChanged(*r);
}

void Device::SlotReceiverRemoved(const uint64_t token)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Receiver> >::iterator r = std::find_if(receivers_.begin(), receivers_.end(), [token](const QSharedPointer<client::Receiver>& receiver) { return (receiver->GetToken() == token); });
  if (r == receivers_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find receiver: ") + QString::number(token));
    return;
  }
  receivers_.erase(r);
  emit SignalReceiverRemoved(token);
}

void Device::SlotRecordingActiveJobChanged(const uint64_t token, const boost::optional<uint64_t>& activejob)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator r = std::find_if(recordings_.begin(), recordings_.end(), [token](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == token); });
  if (r == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(token));
    return;
  }

  (*r)->SetActiveJob(activejob);
  emit SignalRecordingChanged(*r);
}

void Device::SlotRecordingAdded(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator r = std::find_if(recordings_.begin(), recordings_.end(), [token](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == token); });
  if (r == recordings_.end())
  {
    QSharedPointer<client::Recording> recording = QSharedPointer<client::Recording>::create(boost::static_pointer_cast<Device>(shared_from_this()), token, QString::fromStdString(sourceid), QString::fromStdString(name), QString::fromStdString(location), QString::fromStdString(description), QString::fromStdString(address), QString::fromStdString(content), retentiontime, activejob, 0);
    recordings_.push_back(recording);
    emit SignalRecordingAdded(recording);
  }
  else
  {
    if (((*r)->GetSourceId().toStdString() != sourceid) || ((*r)->GetName().toStdString() != name) || ((*r)->GetLocation().toStdString() != location) || ((*r)->GetDescription().toStdString() != description) || ((*r)->GetAddress().toStdString() != address) || ((*r)->GetContent().toStdString() != content))
    {
      (*r)->SetSourceId(QString::fromStdString(sourceid));
      (*r)->SetName(QString::fromStdString(name));
      (*r)->SetLocation(QString::fromStdString(location));
      (*r)->SetDescription(QString::fromStdString(description));
      (*r)->SetAddress(QString::fromStdString(address));
      (*r)->SetContent(QString::fromStdString(content));
      emit SignalRecordingChanged(*r);
    }
  }
}

void Device::SlotRecordingChanged(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator r = std::find_if(recordings_.begin(), recordings_.end(), [token](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == token); });
  if (r == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(token));
    return;
  }

  (*r)->Set(QString::fromStdString(sourceid), QString::fromStdString(name), QString::fromStdString(location), QString::fromStdString(description), QString::fromStdString(address), QString::fromStdString(content), retentiontime, activejob);
  emit SignalRecordingChanged(*r);
}

void Device::SlotRecordingRemoved(const uint64_t token)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator r = std::find_if(recordings_.begin(), recordings_.end(), [token](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == token); });
  if (r == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(token));
    return;
  }

  const bool licensevalid = IsValidLicense();
  recordings_.erase(r);
  emit MainWindow::Instance()->GetDeviceMgr().RecordingRemoved(boost::static_pointer_cast<Device>(shared_from_this()), token);
  emit SignalRecordingRemoved(token);
  if (licensevalid != IsValidLicense())
  {
    emit SignalLicenseStateChanged(!licensevalid);

  }
}

void Device::SlotRecordingJobAdded(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const monocle::RecordingJobState state)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator r = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (r == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  const QSharedPointer<RecordingJob> recordingjob = (*r)->GetJob(token);
  if (recordingjob)
  {
    const QSharedPointer<client::RecordingJob> rj = (*r)->ChangeJob(token, QString::fromStdString(name), enabled, priority);
    if (rj)
    {
      LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job: ") + QString::number(token));
      return;
    }
    emit SignalRecordingJobChanged(*r, rj);
  }
  else
  {
    QSharedPointer<client::RecordingJob> rj = QSharedPointer<client::RecordingJob>::create(boost::static_pointer_cast<Device>(shared_from_this()), token, QString::fromStdString(name), enabled, priority);
    (*r)->AddJob(rj);
    emit SignalRecordingJobAdded(*r, rj);
  }
}

void Device::SlotRecordingJobChanged(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator r = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (r == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  const QSharedPointer<client::RecordingJob> rj = (*r)->ChangeJob(token, QString::fromStdString(name), enabled, priority);
  if (rj)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job: ") + QString::number(token));
    return;
  }
  emit SignalRecordingJobChanged(*r, rj);
}

void Device::SlotRecordingJobLogMessage(const uint64_t recordingtoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const QString& message)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  QSharedPointer<client::RecordingJob> recordingjob = (*recording)->GetJob(token);
  if (!recordingjob)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job: ") + QString::number(token));
    return;
  }

  emit (*recording)->JobLogMessage(recordingjob, time, severity, message);
  emit SignalRecordingJobLogMessage(*recording, recordingjob, time, severity, message);
}

void Device::SlotRecordingJobRemoved(const uint64_t recordingtoken, const uint64_t token)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  (*recording)->RemoveJob(token);
  emit SignalRecordingJobRemoved(*recording, token);
}

void Device::SlotRecordingJobSourceAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token, const uint64_t receivertoken)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  QSharedPointer<client::RecordingJob> recordingjob = (*recording)->GetJob(recordingjobtoken);
  if (!recordingjob)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job: ") + QString::number(recordingjobtoken));
    return;
  }

  const QSharedPointer<RecordingJobSource> recordingjobsource = recordingjob->GetSource(token);
  if (recordingjobsource)
  {
    recordingjobsource->SetReceiverToken(receivertoken);
    //TODO emit something when available
  }
  else
  {
    QSharedPointer<RecordingJobSource> source = QSharedPointer<RecordingJobSource>::create(token, receivertoken);
    recordingjob->AddSource(source);
    emit (*recording)->JobSourceAdded(recordingjob, source);
    emit SignalRecordingJobSourceAdded(*recording, recordingjob, source);
  }
}

void Device::SlotRecordingJobSourceRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  QSharedPointer<client::RecordingJob> recordingjob = (*recording)->GetJob(recordingjobtoken);
  if (!recordingjob)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job: ") + QString::number(recordingjobtoken));
    return;
  }

  recordingjob->RemoveSource(token);
  emit (*recording)->JobSourceRemoved(recordingjob, token);
  emit SignalRecordingJobSourceRemoved(*recording, recordingjob, token);
}

void Device::SlotRecordingJobSourceTrackActiveParametersChanged(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken, const std::vector<QString>& activeparameters)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  QSharedPointer<client::RecordingJob> recordingjob = (*recording)->GetJob(recordingjobtoken);
  if (!recordingjob)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job: ") + QString::number(recordingjobtoken));
    return;
  }

  QSharedPointer<RecordingJobSource> recordingjobsource = recordingjob->GetSource(recordingjobsourcetoken);
  if (!recordingjobsource)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job source: ") + QString::number(recordingjobsourcetoken));
    return;
  }

  QSharedPointer<client::RecordingJobSourceTrack> recordingjobsourcetrack = recordingjobsource->GetTrack(recordingjobsourcetracktoken);
  if (!recordingjobsourcetrack)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job source track: ") + QString::number(recordingjobsourcetracktoken));
    return;
  }

  recordingjobsourcetrack->SetActiveParameters(activeparameters);
  emit (*recording)->JobSourceTrackActiveParametersChanged(recordingjob, recordingjobsource, recordingjobsourcetrack, activeparameters);
}

void Device::SlotRecordingJobSourceTrackAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint32_t recordingtrackid, const std::vector<QString>& parameters, const monocle::RecordingJobState state, const QString& error, const std::vector<QString>& activeparameters)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  QSharedPointer<client::RecordingJob> recordingjob = (*recording)->GetJob(recordingjobtoken);
  if (!recordingjob)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job: ") + QString::number(recordingjobtoken));
    return;
  }

  QSharedPointer<RecordingJobSource> recordingjobsource = recordingjob->GetSource(recordingjobsourcetoken);
  if (!recordingjobsource)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job source: ") + QString::number(recordingjobsourcetoken));
    return;
  }

  const QSharedPointer<RecordingJobSourceTrack> recordingjobsourcetrack = recordingjobsource->GetTrack(token);
  if (recordingjobsourcetrack)
  {
    const std::vector<QString> tmpparameters = recordingjobsourcetrack->GetParameters();
    const std::vector<QString> tmpactiveparameters = recordingjobsourcetrack->GetActiveParameters();
    if (!std::is_permutation(tmpparameters.cbegin(), tmpparameters.cend(), parameters.cbegin(), parameters.cend()) || !std::is_permutation(tmpactiveparameters.cbegin(), tmpactiveparameters.cend(), activeparameters.cbegin(), activeparameters.cend()))
    {
      emit (*recording)->JobSourceTrackChanged(recordingjob, recordingjobsource, recordingjobsourcetrack);
      emit SignalRecordingJobSourceTrackChanged(*recording, recordingjob, recordingjobsource, recordingjobsourcetrack);
    }

    if ((recordingjobsourcetrack->GetState() != state) || (recordingjobsourcetrack->GetError() != error))
    {
      emit SignalRecordingJobSourceTrackStateChanged((*recording)->GetToken(), recordingjob->GetToken(), recordingjobsource->GetToken(), recordingjobsourcetrack->GetToken(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + utctimeoffset_, state, error);

    }
  }
  else
  {
    QSharedPointer<client::RecordingJobSourceTrack> recordingjobsourcetrack = QSharedPointer<client::RecordingJobSourceTrack>::create(token, parameters, state, error, activeparameters, (*recording)->GetTrack(recordingtrackid));
    recordingjobsource->AddTrack(recordingjobsourcetrack);
    emit (*recording)->JobSourceTrackAdded(recordingjob, recordingjobsource, recordingjobsourcetrack);
    emit recordingjob->SourceTrackAdded(recordingjobsource, recordingjobsourcetrack);
    emit SignalRecordingJobSourceTrackAdded(*recording, recordingjob, recordingjobsource, recordingjobsourcetrack);
  }
}

void Device::SlotRecordingJobSourceTrackLogMessage(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const QString& message)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  QSharedPointer<client::RecordingJob> recordingjob = (*recording)->GetJob(recordingjobtoken);
  if (!recordingjob)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job: ") + QString::number(recordingjobtoken));
    return;
  }

  QSharedPointer<RecordingJobSource> recordingjobsource = recordingjob->GetSource(recordingjobsourcetoken);
  if (!recordingjobsource)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job source: ") + QString::number(recordingjobsourcetoken));
    return;
  }

  QSharedPointer<client::RecordingJobSourceTrack> recordingjobsourcetrack = recordingjobsource->GetTrack(token);
  if (!recordingjobsourcetrack)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job source track: ") + QString::number(token));
    return;
  }

  emit (*recording)->JobSourceTrackLogMessage(recordingjob, recordingjobsource, recordingjobsourcetrack, time, severity, message);
  emit SignalRecordingJobSourceTrackLogMessage(*recording, recordingjob, recordingjobsource, recordingjobsourcetrack, time, severity, message);
}

void Device::SlotRecordingJobSourceTrackRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  QSharedPointer<client::RecordingJob> recordingjob = (*recording)->GetJob(recordingjobtoken);
  if (!recordingjob)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job: ") + QString::number(recordingjobtoken));
    return;
  }

  QSharedPointer<RecordingJobSource> recordingjobsource = recordingjob->GetSource(recordingjobsourcetoken);
  if (!recordingjobsource)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job source: ") + QString::number(recordingjobsourcetoken));
    return;
  }

  recordingjobsource->RemoveTrack(token);
  emit (*recording)->JobSourceTrackRemoved(recordingjob, recordingjobsource, token);
  emit recordingjob->SourceTrackRemoved(recordingjobsource, token);
  emit SignalRecordingJobSourceTrackRemoved(*recording, recordingjob, recordingjobsource, token);
}

void Device::SlotRecordingJobSourceTrackStateChanged(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken, const uint64_t time, const monocle::RecordingJobState state, const QString& error)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  QSharedPointer<client::RecordingJob> recordingjob = (*recording)->GetJob(recordingjobtoken);
  if (!recordingjob)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job: ") + QString::number(recordingjobtoken));
    return;
  }

  QSharedPointer<RecordingJobSource> recordingjobsource = recordingjob->GetSource(recordingjobsourcetoken);
  if (!recordingjobsource)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job source: ") + QString::number(recordingjobsourcetoken));
    return;
  }

  QSharedPointer<client::RecordingJobSourceTrack> recordingjobsourcetrack = recordingjobsource->GetTrack(recordingjobsourcetracktoken);
  if (!recordingjobsourcetrack)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording job source track: ") + QString::number(recordingjobsourcetracktoken));
    return;
  }

  const monocle::RecordingJobState prevstate = recordingjobsourcetrack->GetState();
  if ((prevstate == state) && (error == recordingjobsourcetrack->GetError()))
  {
    // Ignore this, nothing is really changing
    return;
  }
  
  recordingjobsourcetrack->GetTrack()->SetState(time, state, prevstate);
  recordingjobsourcetrack->SetState(state, error);
  emit (*recording)->JobSourceTrackStateChanged(recordingjob, recordingjobsource, recordingjobsourcetrack, time, state, error, prevstate);
}

void Device::SlotRecordingTrackCodecAdded(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp)
{
  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  const QSharedPointer<RecordingTrack> track = (*recording)->GetTrack(recordingtrackid);
  if (!track)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording track: ") + QString::number(recordingtrackid));
    return;
  }
  
  track->AddCodec(id, codec, parameters, timestamp);
}

void Device::SlotRecordingTrackCodecRemoved(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id)
{
  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  const QSharedPointer<RecordingTrack> track = (*recording)->GetTrack(recordingtrackid);
  if (!track)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording track: ") + QString::number(recordingtrackid));
    return;
  }

  track->RemoveCodec(id);
}

void Device::SlotRecordingTrackLogMessage(const uint64_t recordingtoken, const uint32_t id, const uint64_t time, const monocle::Severity severity, const QString& message)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  const QSharedPointer<client::RecordingTrack> track = (*recording)->GetTrack(id);
  if (!track)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording track: ") + QString::number(id));
    return;
  }

  emit (*recording)->TrackLogMessage(track, time, severity, message);
}

void Device::SlotRecordingLogMessage(const uint64_t token, const uint64_t time, const monocle::Severity severity, const QString& message)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [token](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == token); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(token));
    return;
  }
  emit (*recording)->LogMessage(time, severity, message);
  emit SignalRecordingLogMessage(*recording, time, severity, message);
}

void Device::SlotRecordingStatistics(const uint64_t time, const std::vector<monocle::RECORDINGSTATISTICS>& recordingsstatistics)
{
  for (const monocle::RECORDINGSTATISTICS& recordingstatistics : recordingsstatistics)
  {
    std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [&recordingstatistics](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingstatistics.token_); });
    if (recording == recordings_.end())
    {
      
      continue;
    }

    for (const monocle::TRACKSTATISTICS& trackstatistics : recordingstatistics.tracksstatistics_)
    {
      QSharedPointer<RecordingTrack> track = (*recording)->GetTrack(trackstatistics.trackid_);
      if (!track)
      {

        continue;
      }
      track->SetTotalTrackData(std::make_pair(time, trackstatistics.trackdatareceived_));
    }
    emit (*recording)->DataRate();
  }
}

void Device::SlotServerLogMessage(const uint64_t time, const monocle::Severity severity, const QString& message)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  emit SignalServerLogMessage(time, severity, message);

}

void Device::SlotTrackAdded(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& filetokens, const std::vector<monocle::CODECINDEX>& codecindices, const std::pair<uint64_t, uint64_t>& totaltrackdata)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  const QSharedPointer<RecordingTrack> recordingtrack = (*recording)->GetTrack(id);
  if (recordingtrack)
  {
    const QSharedPointer<client::RecordingTrack> track = (*recording)->ChangeTrack(id, QString::fromStdString(token), tracktype, QString::fromStdString(description), fixedfiles, digitalsigning, encrypt, flushfrequency, filetokens, codecindices, totaltrackdata);
    if (!track)
    {
      LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording track: ") + QString::number(id));
      return;
    }
  }
  else
  {
    const QSharedPointer<client::RecordingTrack> track = (*recording)->AddTrack(monocle::RECORDINGTRACK(id, token, tracktype, description, fixedfiles, digitalsigning, encrypt, flushfrequency, filetokens, std::vector<monocle::INDEX>(), codecindices, totaltrackdata));
    emit SignalRecordingTrackAdded(*recording, track);
  }
}

void Device::SlotTrackChanged(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& filetokens, const std::vector<monocle::CODECINDEX>& codecindices, const std::pair<uint64_t, uint64_t>& totaltrackdata)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  const QSharedPointer<client::RecordingTrack> track = (*recording)->ChangeTrack(id, QString::fromStdString(token), tracktype, QString::fromStdString(description), fixedfiles, digitalsigning, encrypt, flushfrequency, filetokens, codecindices, totaltrackdata);
  if (!track)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording track: ") + QString::number(id));
    return;
  }

  emit SignalRecordingTrackChanged(*recording, track);
}

void Device::SlotTrackDeleteData(const uint64_t recordingtoken, const uint32_t trackid, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  QSharedPointer<client::RecordingTrack> track = (*recording)->GetTrack(trackid);
  if (!track)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording track: ") + QString::number(trackid));
    return;
  }

  const monocle::RecordingJobState state = (*recording)->GetState(track);
  track->DeleteData(state, start, end);
  emit(*recording)->TrackDeleteData(track, state, start, end);
}

void Device::SlotTrackRemoved(const uint64_t recordingtoken, const uint32_t id)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }
  (*recording)->RemoveTrack(id);
  emit SignalRecordingTrackRemoved(*recording, id);
}

void Device::SlotTrackSetData(const uint64_t recordingtoken, const uint32_t trackid, const std::vector<monocle::INDEX>& indices)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<client::Recording> >::iterator recording = std::find_if(recordings_.begin(), recordings_.end(), [recordingtoken](const QSharedPointer<client::Recording>& recording) { return (recording->GetToken() == recordingtoken); });
  if (recording == recordings_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording: ") + QString::number(recordingtoken));
    return;
  }

  QSharedPointer<client::RecordingTrack> track = (*recording)->GetTrack(trackid);
  if (!track)
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find recording track: ") + QString::number(trackid));
    return;
  }

  track->SetData(indices);
  emit (*recording)->TrackSetData(track, indices);
}

void Device::SlotUserAdded(const uint64_t token, const QString& username, const uint64_t group)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<User> >::iterator u = std::find_if(users_.begin(), users_.end(), [token](const QSharedPointer<User>& user) { return (user->GetToken() == token); });
  if (u == users_.end())
  {
    QSharedPointer<User> user = QSharedPointer<User>::create(token, username, group);
    users_.push_back(user);
    emit SignalUserAdded(user);
  }
  else
  {
    if (((*u)->GetUsername() != username) || ((*u)->GetGroupToken() != group))
    {
      (*u)->SetUsername(username);
      (*u)->SetGroupToken(group);
      emit SignalUserChanged(*u);
    }
  }
}

void Device::SlotUserChanged(const uint64_t token, const uint64_t group)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<User> >::iterator u = std::find_if(users_.begin(), users_.end(), [token](const QSharedPointer<User>& user) { return (user->GetToken() == token); });
  if (u == users_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find user: ") + QString::number(token));
    return;
  }
  (*u)->SetGroupToken(group);
  emit SignalUserChanged(*u);
}

void Device::SlotUserRemoved(const uint64_t token)
{
  if (state_ != DEVICESTATE::SUBSCRIBED)
  {

    return;
  }

  std::vector< QSharedPointer<User> >::iterator u = std::find_if(users_.begin(), users_.end(), [token](const QSharedPointer<User>& user) { return (user->GetToken() == token); });
  if (u == users_.end())
  {
    LOG_GUI_WARNING_SOURCE(boost::static_pointer_cast<Device>(shared_from_this()), QString("Unable to find user: ") + QString::number(token));
    return;
  }
  users_.erase(u);
  emit SignalUserRemoved(token);
}

void Device::SlotStateChanged(const DEVICESTATE state, const QString& message)
{
  if ((state == DEVICESTATE::SUBSCRIBED) && IsValidLicense() && files_.empty() && recordings_.empty()) // If a device looks like it hasn't been setup before, we can help alert the user to setup a location to store data
  {
    if (Options::Instance().GetHideNewDeviceDialog() == false)
    {
      QCheckBox* checkbox = new QCheckBox("Do not show this again");
      QMessageBox messagebox;
      messagebox.setWindowTitle(tr("New Device Found: ") + address_);
      messagebox.setText("Would you like to setup a location to store video data?");
      messagebox.setIcon(QMessageBox::Icon::Question);
      messagebox.addButton(QMessageBox::Yes);
      messagebox.addButton(QMessageBox::No);
      messagebox.setDefaultButton(QMessageBox::Yes);
      messagebox.setCheckBox(checkbox);
      bool donotshowagain = false;
      QObject::connect(checkbox, &QCheckBox::stateChanged, [&donotshowagain](int state)
      {
        if (static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked)
        {
          donotshowagain = true;

        }
        else
        {
          donotshowagain = false;

        }
      });
      const int ret = messagebox.exec();
      if (donotshowagain)
      {
        Options::Instance().SetHideNewDeviceDialog(true);

      }
      if (ret != QMessageBox::Yes)
      {

        return;
      }

      ManageFileWindow(MainWindow::Instance(), boost::static_pointer_cast<Device>(shared_from_this())).exec();
    }
  }
}

}
