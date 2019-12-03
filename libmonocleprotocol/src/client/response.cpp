// response.hpp
//

///// Includes /////

#include "monocleprotocol/client/response.hpp"

///// Namespaces /////

namespace monocle
{

namespace client
{

///// Methods /////

RESPONSE::RESPONSE(const Error& error) :
  error_(error)
{

}

ADDFILERESPONSE::ADDFILERESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

ADDFILERESPONSE::ADDFILERESPONSE(const Error& error) :
  RESPONSE(error)
{

}

ADDGROUPRESPONSE::ADDGROUPRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

ADDGROUPRESPONSE::ADDGROUPRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

ADDMAPRESPONSE::ADDMAPRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

ADDMAPRESPONSE::ADDMAPRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

ADDONVIFUSERRESPONSE::ADDONVIFUSERRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

ADDONVIFUSERRESPONSE::ADDONVIFUSERRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

ADDRECEIVERRESPONSE::ADDRECEIVERRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

ADDRECEIVERRESPONSE::ADDRECEIVERRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

ADDRECORDINGRESPONSE::ADDRECORDINGRESPONSE(const uint64_t token) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  token_(token)
{

}

ADDRECORDINGRESPONSE::ADDRECORDINGRESPONSE(const Error& error) :
  RESPONSE(error),
  token_(0)
{

}

ADDRECORDINGJOBRESPONSE::ADDRECORDINGJOBRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

ADDRECORDINGJOBRESPONSE::ADDRECORDINGJOBRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

ADDTRACKRESPONSE::ADDTRACKRESPONSE(const uint32_t token) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  token_(token)
{

}

ADDTRACKRESPONSE::ADDTRACKRESPONSE(const Error& error) :
  RESPONSE(error),
  token_(0)
{

}

ADDTRACK2RESPONSE::ADDTRACK2RESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

ADDTRACK2RESPONSE::ADDTRACK2RESPONSE(const Error& error) :
  RESPONSE(error)
{

}

ADDUSERRESPONSE::ADDUSERRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

ADDUSERRESPONSE::ADDUSERRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

AUTHENTICATERESPONSE::AUTHENTICATERESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

AUTHENTICATERESPONSE::AUTHENTICATERESPONSE(const Error& error) :
  RESPONSE(error)
{

}

CHANGEGROUPRESPONSE::CHANGEGROUPRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

CHANGEGROUPRESPONSE::CHANGEGROUPRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

CHANGEMAPRESPONSE::CHANGEMAPRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

CHANGEMAPRESPONSE::CHANGEMAPRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

CHANGERECORDINGJOBRESPONSE::CHANGERECORDINGJOBRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

CHANGERECORDINGJOBRESPONSE::CHANGERECORDINGJOBRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

CHANGEONVIFUSERRESPONSE::CHANGEONVIFUSERRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

CHANGEONVIFUSERRESPONSE::CHANGEONVIFUSERRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

CHANGERECEIVERRESPONSE::CHANGERECEIVERRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

CHANGERECEIVERRESPONSE::CHANGERECEIVERRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

CHANGERECORDINGRESPONSE::CHANGERECORDINGRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

CHANGERECORDINGRESPONSE::CHANGERECORDINGRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

CHANGETRACKRESPONSE::CHANGETRACKRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

CHANGETRACKRESPONSE::CHANGETRACKRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

CHANGEUSERRESPONSE::CHANGEUSERRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

CHANGEUSERRESPONSE::CHANGEUSERRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

CONTROLSTREAMRESPONSE::CONTROLSTREAMRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

CONTROLSTREAMRESPONSE::CONTROLSTREAMRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

CREATEFINDMOTIONRESPONSE::CREATEFINDMOTIONRESPONSE(const uint64_t token) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  token_(token)
{

}

CREATEFINDMOTIONRESPONSE::CREATEFINDMOTIONRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

CREATEFINDOBJECTRESPONSE::CREATEFINDOBJECTRESPONSE(const uint64_t token) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  token_(token)
{

}

CREATEFINDOBJECTRESPONSE::CREATEFINDOBJECTRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

CREATESTREAMRESPONSE::CREATESTREAMRESPONSE(const uint64_t token, const std::vector<CODECINDEX>& codecindices) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  token_(token),
  codecindices_(codecindices)
{

}

CREATESTREAMRESPONSE::CREATESTREAMRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

DESTROYFINDMOTIONRESPONSE::DESTROYFINDMOTIONRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

DESTROYFINDMOTIONRESPONSE::DESTROYFINDMOTIONRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

DESTROYFINDOBJECTRESPONSE::DESTROYFINDOBJECTRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

DESTROYFINDOBJECTRESPONSE::DESTROYFINDOBJECTRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

DESTROYSTREAMRESPONSE::DESTROYSTREAMRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

DESTROYSTREAMRESPONSE::DESTROYSTREAMRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

DISCOVERYBROADCASTRESPONSE::DISCOVERYBROADCASTRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

DISCOVERYBROADCASTRESPONSE::DISCOVERYBROADCASTRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

GETAUTHENTICATIONNONCERESPONSE::GETAUTHENTICATIONNONCERESPONSE(const std::string& authenticatenonce) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  authenticatenonce_(authenticatenonce)
{

}

GETAUTHENTICATIONNONCERESPONSE::GETAUTHENTICATIONNONCERESPONSE(const Error& error) :
  RESPONSE(error)
{

}

GETCHILDFOLDERSRESPONSE::GETCHILDFOLDERSRESPONSE(const std::vector<std::string>& childfolders) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  childfolders_(childfolders)
{

}

GETCHILDFOLDERSRESPONSE::GETCHILDFOLDERSRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

GETFILESRESPONSE::GETFILESRESPONSE(const std::vector<FILE>& files) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  files_(files)
{

}

GETFILESRESPONSE::GETFILESRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

GETRECEIVERSRESPONSE::GETRECEIVERSRESPONSE(const std::vector<RECEIVER>& receivers) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  receivers_(receivers)
{

}

GETRECEIVERSRESPONSE::GETRECEIVERSRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

GETRECORDINGSRESPONSE::GETRECORDINGSRESPONSE(const std::vector<RECORDING>& recordings) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  recordings_(recordings)
{

}

GETRECORDINGSRESPONSE::GETRECORDINGSRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

GETSNAPSHOTRESPONSE::GETSNAPSHOTRESPONSE(const std::vector<uint8_t>& data) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  data_(data)
{

}

GETSNAPSHOTRESPONSE::GETSNAPSHOTRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

GETSTATERESPONSE::GETSTATERESPONSE(const std::string& name, const std::string& publickey, const std::string& architecture, const int operatingsystem, const std::string& compiler, const std::string& databasepath, const utility::Version& version, const uint64_t identifier, const std::vector<std::string>& environmentalvariables, const std::vector<std::string>& commandlinevariables, const std::vector<ONVIFUSER>& onvifusers, const std::vector<GROUP>& groups, const std::vector<USER>& users, const std::vector<FILE>& files, const std::vector<RECEIVER>& receivers, const std::vector<RECORDING>& recordings, const std::vector<LOGMESSAGE>& serverlogmessages, const uint32_t maxrecordings, const std::vector<MAP>& maps, const std::vector<MOUNTPOINT>& mountpoints, const std::string& latitude, const std::string& longitude) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  name_(name),
  publickey_(publickey),
  architecture_(architecture),
  operatingsystem_(operatingsystem),
  compiler_(compiler),
  databasepath_(databasepath),
  version_(version),
  identifier_(identifier),
  environmentalvariables_(environmentalvariables),
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

GETSTATERESPONSE::GETSTATERESPONSE(const Error& error) :
  RESPONSE(error)
{

}

GETTIMERESPONSE::GETTIMERESPONSE(const uint64_t utctime) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  utctime_(utctime)
{

}

GETTIMERESPONSE::GETTIMERESPONSE(const Error& error) :
  RESPONSE(error)
{

}

KEEPALIVERESPONSE::KEEPALIVERESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

KEEPALIVERESPONSE::KEEPALIVERESPONSE(const Error& error) :
  RESPONSE(error)
{

}

MOUNTFILERESPONSE::MOUNTFILERESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

MOUNTFILERESPONSE::MOUNTFILERESPONSE(const Error& error) :
  RESPONSE(error)
{

}

REMOVEFILERESPONSE::REMOVEFILERESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

REMOVEFILERESPONSE::REMOVEFILERESPONSE(const Error& error) :
  RESPONSE(error)
{

}

REMOVEGROUPRESPONSE::REMOVEGROUPRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

REMOVEGROUPRESPONSE::REMOVEGROUPRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

REMOVEMAPRESPONSE::REMOVEMAPRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

REMOVEMAPRESPONSE::REMOVEMAPRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

REMOVEONVIFUSERRESPONSE::REMOVEONVIFUSERRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

REMOVEONVIFUSERRESPONSE::REMOVEONVIFUSERRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

REMOVERECEIVERRESPONSE::REMOVERECEIVERRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

REMOVERECEIVERRESPONSE::REMOVERECEIVERRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

REMOVERECORDINGRESPONSE::REMOVERECORDINGRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

REMOVERECORDINGRESPONSE::REMOVERECORDINGRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

REMOVERECORDINGJOBRESPONSE::REMOVERECORDINGJOBRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

REMOVERECORDINGJOBRESPONSE::REMOVERECORDINGJOBRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

REMOVERECORDINGJOBSOURCERESPONSE::REMOVERECORDINGJOBSOURCERESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

REMOVERECORDINGJOBSOURCERESPONSE::REMOVERECORDINGJOBSOURCERESPONSE(const Error& error) :
  RESPONSE(error)
{

}

REMOVETRACKRESPONSE::REMOVETRACKRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

REMOVETRACKRESPONSE::REMOVETRACKRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

REMOVEUSERRESPONSE::REMOVEUSERRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

REMOVEUSERRESPONSE::REMOVEUSERRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

SETLOCATIONRESPONSE::SETLOCATIONRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

SETLOCATIONRESPONSE::SETLOCATIONRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

SETNAMERESPONSE::SETNAMERESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

SETNAMERESPONSE::SETNAMERESPONSE(const Error& error) :
  RESPONSE(error)
{

}

SUBSCRIBERESPONSE::SUBSCRIBERESPONSE(const std::string& name, const std::string& publickey, const std::string& architecture, const int operatingsystem, const std::string& compiler, const std::string& databasepath, const utility::Version& version, const uint64_t identifier, const std::vector<std::string>& environmentalvariables, const std::vector<std::string>& commandlinevariables, const std::vector<ONVIFUSER>& onvifusers, const std::vector<GROUP>& groups, const std::vector<USER>& users, const std::vector<FILE>& files, const std::vector<RECEIVER>& receivers, const std::vector<RECORDING>& recordings, const std::vector<LOGMESSAGE>& serverlogmessages, const uint32_t maxrecordings, const std::vector<MAP>& maps, const std::vector<MOUNTPOINT>& mountpoints, const std::string& latitude, const std::string& longitude, const unsigned int numcudadevices, const unsigned int numcldevices, const uint32_t maxobjectdetectors) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  name_(name),
  publickey_(publickey),
  architecture_(architecture),
  operatingsystem_(operatingsystem),
  compiler_(compiler),
  databasepath_(databasepath),
  version_(version),
  identifier_(identifier),
  environmentalvariables_(environmentalvariables),
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

SUBSCRIBERESPONSE::SUBSCRIBERESPONSE(const Error& error) :
  RESPONSE(error),
  operatingsystem_(0),
  numcudadevices_(0)
{

}

SUBSCRIBEDISCOVERYRESPONSE::SUBSCRIBEDISCOVERYRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

SUBSCRIBEDISCOVERYRESPONSE::SUBSCRIBEDISCOVERYRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

SUBSCRIBEHARDWARESTATSRESPONSE::SUBSCRIBEHARDWARESTATSRESPONSE(const HARDWARESTATS& currnethardwarestats) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  currenthardwarestats_(currnethardwarestats)
{

}

SUBSCRIBEHARDWARESTATSRESPONSE::SUBSCRIBEHARDWARESTATSRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

SUBSCRIBERECORDINGJOBLOGRESPONSE::SUBSCRIBERECORDINGJOBLOGRESPONSE(const std::vector< std::pair< uint64_t, std::vector<LOGMESSAGE> > >& recordingjoblogmessages) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  recordingjoblogmessages_(recordingjoblogmessages)
{

}

SUBSCRIBERECORDINGJOBLOGRESPONSE::SUBSCRIBERECORDINGJOBLOGRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE::RECORDINGJOBSOURCETRACKTOKEN::RECORDINGJOBSOURCETRACKTOKEN(const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken) :
  recordingjobtoken_(recordingjobtoken),
  recordingjobsourcetoken_(recordingjobsourcetoken),
  recordingjobsourcetracktoken_(recordingjobsourcetracktoken)
{

}

SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE::SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(const std::vector< std::pair< RECORDINGJOBSOURCETRACKTOKEN, std::vector<LOGMESSAGE> > >& recordingjobsourcetracklogmessages_) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  recordingjobsourcetracklogmessages_(recordingjobsourcetracklogmessages_)
{

}

SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE::SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

SUBSCRIBERECORDINGLOGRESPONSE::SUBSCRIBERECORDINGLOGRESPONSE(const std::vector<LOGMESSAGE>& messages) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  messages_(messages)
{

}

SUBSCRIBERECORDINGLOGRESPONSE::SUBSCRIBERECORDINGLOGRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

SUBSCRIBERECORDINGTRACKLOGRESPONSE::SUBSCRIBERECORDINGTRACKLOGRESPONSE(const std::vector< std::pair< uint32_t, std::vector<LOGMESSAGE> > >& recordingtracklogmessages) :
  RESPONSE(Error(ErrorCode::Success, std::string())),
  recordingtracklogmessages_(recordingtracklogmessages)
{

}

SUBSCRIBERECORDINGTRACKLOGRESPONSE::SUBSCRIBERECORDINGTRACKLOGRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

UNMOUNTFILERESPONSE::UNMOUNTFILERESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

UNMOUNTFILERESPONSE::UNMOUNTFILERESPONSE(const Error& error) :
  RESPONSE(error)
{

}

UNSUBSCRIBEDISCOVERYRESPONSE::UNSUBSCRIBEDISCOVERYRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

UNSUBSCRIBEDISCOVERYRESPONSE::UNSUBSCRIBEDISCOVERYRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

UNSUBSCRIBEHARDWARESTATSRESPONSE::UNSUBSCRIBEHARDWARESTATSRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

UNSUBSCRIBEHARDWARESTATSRESPONSE::UNSUBSCRIBEHARDWARESTATSRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

UNSUBSCRIBERECORDINGJOBLOGRESPONSE::UNSUBSCRIBERECORDINGJOBLOGRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

UNSUBSCRIBERECORDINGJOBLOGRESPONSE::UNSUBSCRIBERECORDINGJOBLOGRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE::UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE::UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

UNSUBSCRIBERECORDINGLOGRESPONSE::UNSUBSCRIBERECORDINGLOGRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

UNSUBSCRIBERECORDINGLOGRESPONSE::UNSUBSCRIBERECORDINGLOGRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

UNSUBSCRIBERECORDINGTRACKLOGRESPONSE::UNSUBSCRIBERECORDINGTRACKLOGRESPONSE() :
  RESPONSE(Error(ErrorCode::Success, std::string()))
{

}

UNSUBSCRIBERECORDINGTRACKLOGRESPONSE::UNSUBSCRIBERECORDINGTRACKLOGRESPONSE(const Error& error) :
  RESPONSE(error)
{

}

}

}
