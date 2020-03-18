// response.hpp
//

#ifndef IDF69P3OMCNFH9877254Z6ETGJT0STS5UA
#define IDF69P3OMCNFH9877254Z6ETGJT0STS5UA

///// Includes /////

#include <stdint.h>
#include <string>
#include <utility/version.hpp>
#include <vector>

#include "monocleprotocol/monocleprotocol.hpp"

///// Namespaces /////

namespace monocle
{

namespace client
{

///// Structures /////

struct RESPONSE
{
  RESPONSE(const Error& error);

  inline ErrorCode GetErrorCode() const { return error_.code_; }
  inline const std::string& GetErrorText() const { return error_.text_; }

  Error error_;

};

struct ADDFILERESPONSE : public RESPONSE
{
  ADDFILERESPONSE();
  ADDFILERESPONSE(const Error& error);

};

struct ADDGROUPRESPONSE : public RESPONSE
{
  ADDGROUPRESPONSE();
  ADDGROUPRESPONSE(const Error& error);

};

struct ADDLAYOUTRESPONSE : public RESPONSE
{
  ADDLAYOUTRESPONSE();
  ADDLAYOUTRESPONSE(const Error& error);

};

struct ADDMAPRESPONSE : public RESPONSE
{
  ADDMAPRESPONSE();
  ADDMAPRESPONSE(const Error& error);

};

struct ADDONVIFUSERRESPONSE : public RESPONSE
{
  ADDONVIFUSERRESPONSE();
  ADDONVIFUSERRESPONSE(const Error& error);

};

struct ADDRECEIVERRESPONSE : public RESPONSE
{
  ADDRECEIVERRESPONSE();
  ADDRECEIVERRESPONSE(const Error& error);

};

struct ADDRECORDINGRESPONSE : public RESPONSE
{
  ADDRECORDINGRESPONSE(const uint64_t token);
  ADDRECORDINGRESPONSE(const Error& error);

  uint64_t token_;

};

struct ADDRECORDINGJOBRESPONSE : public RESPONSE
{
  ADDRECORDINGJOBRESPONSE(const uint64_t recordingjobtoken);
  ADDRECORDINGJOBRESPONSE(const Error& error);

  uint64_t recordingjobtoken_;

};

struct ADDTRACKRESPONSE : public RESPONSE
{
  ADDTRACKRESPONSE(const uint32_t token);
  ADDTRACKRESPONSE(const Error& error);

  uint32_t token_;

};

struct ADDTRACK2RESPONSE : public RESPONSE
{
  ADDTRACK2RESPONSE();
  ADDTRACK2RESPONSE(const Error& error);

};

struct ADDUSERRESPONSE : public RESPONSE
{
  ADDUSERRESPONSE();
  ADDUSERRESPONSE(const Error& error);

};

struct AUTHENTICATERESPONSE : public RESPONSE
{
  AUTHENTICATERESPONSE();
  AUTHENTICATERESPONSE(const Error& error);

};

struct CHANGEGROUPRESPONSE : public RESPONSE
{
  CHANGEGROUPRESPONSE();
  CHANGEGROUPRESPONSE(const Error& error);

};

struct CHANGELAYOUTRESPONSE : public RESPONSE
{
  CHANGELAYOUTRESPONSE();
  CHANGELAYOUTRESPONSE(const Error& error);

};

struct CHANGEMAPRESPONSE : public RESPONSE
{
  CHANGEMAPRESPONSE();
  CHANGEMAPRESPONSE(const Error& error);

};

struct CHANGERECORDINGJOBRESPONSE : public RESPONSE
{
  CHANGERECORDINGJOBRESPONSE();
  CHANGERECORDINGJOBRESPONSE(const Error& error);

};

struct CHANGEONVIFUSERRESPONSE : public RESPONSE
{
  CHANGEONVIFUSERRESPONSE();
  CHANGEONVIFUSERRESPONSE(const Error& error);

};

struct CHANGERECEIVERRESPONSE : public RESPONSE
{
  CHANGERECEIVERRESPONSE();
  CHANGERECEIVERRESPONSE(const Error& error);

};

struct CHANGERECORDINGRESPONSE : public RESPONSE
{
  CHANGERECORDINGRESPONSE();
  CHANGERECORDINGRESPONSE(const Error& error);

};

struct CHANGETRACKRESPONSE : public RESPONSE
{
  CHANGETRACKRESPONSE();
  CHANGETRACKRESPONSE(const Error& error);

};

struct CHANGETRACK2RESPONSE : public RESPONSE
{
  CHANGETRACK2RESPONSE();
  CHANGETRACK2RESPONSE(const Error& error);

};

struct CHANGEUSERRESPONSE : public RESPONSE
{
  CHANGEUSERRESPONSE();
  CHANGEUSERRESPONSE(const Error& error);

};

struct CONTROLSTREAMRESPONSE : public RESPONSE
{
  CONTROLSTREAMRESPONSE();
  CONTROLSTREAMRESPONSE(const Error& error);

};

struct CREATEFINDMOTIONRESPONSE : public RESPONSE
{
  CREATEFINDMOTIONRESPONSE(const uint64_t token);
  CREATEFINDMOTIONRESPONSE(const Error& error);

  uint64_t token_;

};

struct CREATEFINDOBJECTRESPONSE : public RESPONSE
{
  CREATEFINDOBJECTRESPONSE(const uint64_t token);
  CREATEFINDOBJECTRESPONSE(const Error& error);

  uint64_t token_;

};

struct CREATESTREAMRESPONSE : public RESPONSE
{
  CREATESTREAMRESPONSE(const uint64_t token, const std::vector<CODECINDEX>& codecindices);
  CREATESTREAMRESPONSE(const Error& error);

  uint64_t token_;
  std::vector<CODECINDEX> codecindices_;

};

struct DESTROYFINDMOTIONRESPONSE : public RESPONSE
{
  DESTROYFINDMOTIONRESPONSE();
  DESTROYFINDMOTIONRESPONSE(const Error& error);

};

struct DESTROYFINDOBJECTRESPONSE : public RESPONSE
{
  DESTROYFINDOBJECTRESPONSE();
  DESTROYFINDOBJECTRESPONSE(const Error& error);

};

struct DESTROYSTREAMRESPONSE : public RESPONSE
{
  DESTROYSTREAMRESPONSE();
  DESTROYSTREAMRESPONSE(const Error& error);

};

struct DISCOVERYBROADCASTRESPONSE : public RESPONSE
{
  DISCOVERYBROADCASTRESPONSE();
  DISCOVERYBROADCASTRESPONSE(const Error& error);

};

struct GETAUTHENTICATIONNONCERESPONSE : public RESPONSE
{
  GETAUTHENTICATIONNONCERESPONSE(const std::string& authenticatenonce);
  GETAUTHENTICATIONNONCERESPONSE(const Error& error);

  std::string authenticatenonce_;

};

struct GETCHILDFOLDERSRESPONSE : public RESPONSE
{
  GETCHILDFOLDERSRESPONSE(const std::vector<std::string>& childfolders);
  GETCHILDFOLDERSRESPONSE(const Error& error);

  std::vector<std::string> childfolders_;

};

struct GETFILESRESPONSE : public RESPONSE
{
  GETFILESRESPONSE(const std::vector<FILE>& files);
  GETFILESRESPONSE(const Error& error);

  std::vector<FILE> files_;

};

struct GETRECEIVERSRESPONSE : public RESPONSE
{
  GETRECEIVERSRESPONSE(const std::vector<RECEIVER>& receivers);
  GETRECEIVERSRESPONSE(const Error& error);

  std::vector<RECEIVER> receivers_;

};

struct GETRECORDINGSRESPONSE : public RESPONSE
{
  GETRECORDINGSRESPONSE(const std::vector<RECORDING>& recordings);
  GETRECORDINGSRESPONSE(const Error& error);

  std::vector<RECORDING> recordings_;

};

struct GETSNAPSHOTRESPONSE : public RESPONSE
{
  GETSNAPSHOTRESPONSE(const std::vector<uint8_t>& data);
  GETSNAPSHOTRESPONSE(const Error& error);

  std::vector<uint8_t> data_;

};

struct GETSTATERESPONSE : public RESPONSE
{
  GETSTATERESPONSE(const std::string& name, const std::string& publickey, const std::string& architecture, const int operatingsystem, const std::string& compiler, const std::string& databasepath, const utility::Version& version, const uint64_t identifier, const std::vector<std::string>& environmentalvariables, const std::vector<std::string>& commandlinevariables, const std::vector<ONVIFUSER>& onvifusers, const std::vector<GROUP>& groups, const std::vector<USER>& users, const std::vector<FILE>& files, const std::vector<RECEIVER>& receivers, const std::vector<RECORDING>& recordings, const std::vector<LOGMESSAGE>& serverlogmessages, const uint32_t maxrecordings, const std::vector<LAYOUT>& layouts, const std::vector<MAP>& maps, const std::vector<MOUNTPOINT>& mountpoints, const std::string& latitude, const std::string& longitude);
  GETSTATERESPONSE(const Error& error);

  std::string name_;
  std::string publickey_;
  std::string architecture_;
  int operatingsystem_;
  std::string compiler_;
  std::string databasepath_;
  utility::Version version_;
  uint64_t identifier_;
  std::vector<std::string> environmentalvariables_;
  std::vector<std::string> commandlinevariables_;
  std::vector<ONVIFUSER> onvifusers_;
  std::vector<GROUP> groups_;
  std::vector<USER> users_;
  std::vector<FILE> files_;
  std::vector<RECEIVER> receivers_;
  std::vector<RECORDING> recordings_;
  std::vector<LOGMESSAGE> serverlogmessages_;
  uint32_t maxrecordings_;
  std::vector<LAYOUT> layouts_;
  std::vector<MAP> maps_;
  std::vector<MOUNTPOINT> mountpoints_;
  std::string latitude_;
  std::string longitude_;

};

struct GETTIMERESPONSE : public RESPONSE
{
  GETTIMERESPONSE(const uint64_t utctime);
  GETTIMERESPONSE(const Error& error);

  uint64_t utctime_;

};

struct KEEPALIVERESPONSE : public RESPONSE
{
  KEEPALIVERESPONSE();
  KEEPALIVERESPONSE(const Error& error);

};

struct MOUNTFILERESPONSE : public RESPONSE
{
  MOUNTFILERESPONSE();
  MOUNTFILERESPONSE(const Error& error);

};

struct REMOVEFILERESPONSE : public RESPONSE
{
  REMOVEFILERESPONSE();
  REMOVEFILERESPONSE(const Error& error);

};

struct REMOVEGROUPRESPONSE : public RESPONSE
{
  REMOVEGROUPRESPONSE();
  REMOVEGROUPRESPONSE(const Error& error);

};

struct REMOVELAYOUTRESPONSE : public RESPONSE
{
  REMOVELAYOUTRESPONSE();
  REMOVELAYOUTRESPONSE(const Error& error);

};

struct REMOVEMAPRESPONSE : public RESPONSE
{
  REMOVEMAPRESPONSE();
  REMOVEMAPRESPONSE(const Error& error);

};

struct REMOVEONVIFUSERRESPONSE : public RESPONSE
{
  REMOVEONVIFUSERRESPONSE();
  REMOVEONVIFUSERRESPONSE(const Error& error);

};

struct REMOVERECEIVERRESPONSE : public RESPONSE
{
  REMOVERECEIVERRESPONSE();
  REMOVERECEIVERRESPONSE(const Error& error);

};

struct REMOVERECORDINGRESPONSE : public RESPONSE
{
  REMOVERECORDINGRESPONSE();
  REMOVERECORDINGRESPONSE(const Error& error);

};

struct REMOVERECORDINGJOBRESPONSE : public RESPONSE
{
  REMOVERECORDINGJOBRESPONSE();
  REMOVERECORDINGJOBRESPONSE(const Error& error);

};

struct REMOVERECORDINGJOBSOURCERESPONSE : public RESPONSE
{
  REMOVERECORDINGJOBSOURCERESPONSE();
  REMOVERECORDINGJOBSOURCERESPONSE(const Error& error);

};

struct REMOVETRACKRESPONSE : public RESPONSE
{
  REMOVETRACKRESPONSE();
  REMOVETRACKRESPONSE(const Error& error);

};

struct REMOVETRACKSRESPONSE : public RESPONSE
{
  REMOVETRACKSRESPONSE();
  REMOVETRACKSRESPONSE(const Error& error);

};

struct REMOVEUSERRESPONSE : public RESPONSE
{
  REMOVEUSERRESPONSE();
  REMOVEUSERRESPONSE(const Error& error);

};

struct SETLOCATIONRESPONSE : public RESPONSE
{
  SETLOCATIONRESPONSE();
  SETLOCATIONRESPONSE(const Error& error);

};

struct SETNAMERESPONSE : public RESPONSE
{
  SETNAMERESPONSE();
  SETNAMERESPONSE(const Error& error);

};

struct SUBSCRIBERESPONSE : public RESPONSE
{
  SUBSCRIBERESPONSE(const std::string& name, const std::string& publickey, const std::string& architecture, const int operatingsystem, const std::string& compiler, const std::string& databasepath, const utility::Version& version, const uint64_t identifier, const std::vector<std::string>& environmentalvariables, const std::vector<std::string>& commandlinevariables, const std::vector<ONVIFUSER>& onvifusers, const std::vector<GROUP>& groups, const std::vector<USER>& users, const std::vector<FILE>& files, const std::vector<RECEIVER>& receivers, const std::vector<RECORDING>& recordings, const std::vector<LOGMESSAGE>& serverlogmessages, const uint32_t maxrecordings, const std::vector<LAYOUT>& layouts, const std::vector<MAP>& maps, const std::vector<MOUNTPOINT>& mountpoints, const std::string& latitude, const std::string& longitude, const unsigned int numcudadevices, const unsigned int numcldevices, const uint32_t maxobjectdetectors);
  SUBSCRIBERESPONSE(const Error& error);

  std::string name_;
  std::string publickey_;
  std::string architecture_;
  int operatingsystem_;
  std::string compiler_;
  std::string databasepath_;
  utility::Version version_;
  uint64_t identifier_;
  std::vector<std::string> environmentalvariables_;
  std::vector<std::string> commandlinevariables_;
  std::vector<ONVIFUSER> onvifusers_;
  std::vector<GROUP> groups_;
  std::vector<USER> users_;
  std::vector<FILE> files_;
  std::vector<RECEIVER> receivers_;
  std::vector<RECORDING> recordings_;
  std::vector<LOGMESSAGE> serverlogmessages_;
  uint32_t maxrecordings_;
  std::vector<LAYOUT> layouts_;
  std::vector<MAP> maps_;
  std::vector<MOUNTPOINT> mountpoints_;
  std::string latitude_;
  std::string longitude_;
  unsigned int numcudadevices_;
  unsigned int numcldevices_;
  uint32_t maxobjectdetectors_;

};

struct SUBSCRIBEDISCOVERYRESPONSE : public RESPONSE
{
  SUBSCRIBEDISCOVERYRESPONSE();
  SUBSCRIBEDISCOVERYRESPONSE(const Error& error);

};

struct SUBSCRIBEHARDWARESTATSRESPONSE : public RESPONSE
{
  SUBSCRIBEHARDWARESTATSRESPONSE(const HARDWARESTATS& currnethardwarestats);
  SUBSCRIBEHARDWARESTATSRESPONSE(const Error& error);

  HARDWARESTATS currenthardwarestats_;

};

struct SUBSCRIBERECORDINGJOBLOGRESPONSE : public RESPONSE
{
  SUBSCRIBERECORDINGJOBLOGRESPONSE(const std::vector< std::pair< uint64_t, std::vector<LOGMESSAGE> > >& recordingjoblogmessages);
  SUBSCRIBERECORDINGJOBLOGRESPONSE(const Error& error);

  std::vector< std::pair< uint64_t, std::vector<LOGMESSAGE> > > recordingjoblogmessages_; // <JobToken, LogMessage>

};

struct SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE : public RESPONSE
{
  struct RECORDINGJOBSOURCETRACKTOKEN
  {
    RECORDINGJOBSOURCETRACKTOKEN(const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken);

    uint64_t recordingjobtoken_;
    uint64_t recordingjobsourcetoken_;
    uint64_t recordingjobsourcetracktoken_;
  };

  SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(const std::vector< std::pair< RECORDINGJOBSOURCETRACKTOKEN, std::vector<LOGMESSAGE> > >& recordingjobsourcetracklogmessages);
  SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(const Error& error);

  std::vector< std::pair< RECORDINGJOBSOURCETRACKTOKEN, std::vector<LOGMESSAGE> > > recordingjobsourcetracklogmessages_;

};

struct SUBSCRIBERECORDINGLOGRESPONSE : public RESPONSE
{
  SUBSCRIBERECORDINGLOGRESPONSE(const std::vector<LOGMESSAGE>& messages);
  SUBSCRIBERECORDINGLOGRESPONSE(const Error& error);

  std::vector<LOGMESSAGE> messages_;

};

struct SUBSCRIBERECORDINGTRACKLOGRESPONSE : public RESPONSE
{
  SUBSCRIBERECORDINGTRACKLOGRESPONSE(const std::vector< std::pair< uint32_t, std::vector<LOGMESSAGE> > >& recordingtracklogmessages);
  SUBSCRIBERECORDINGTRACKLOGRESPONSE(const Error& error);

  std::vector< std::pair< uint32_t, std::vector<LOGMESSAGE> > > recordingtracklogmessages_;

};

struct UNMOUNTFILERESPONSE : public RESPONSE
{
  UNMOUNTFILERESPONSE();
  UNMOUNTFILERESPONSE(const Error& error);

};

struct UNSUBSCRIBEDISCOVERYRESPONSE : public RESPONSE
{
  UNSUBSCRIBEDISCOVERYRESPONSE();
  UNSUBSCRIBEDISCOVERYRESPONSE(const Error& error);

};

struct UNSUBSCRIBEHARDWARESTATSRESPONSE : public RESPONSE
{
  UNSUBSCRIBEHARDWARESTATSRESPONSE();
  UNSUBSCRIBEHARDWARESTATSRESPONSE(const Error& error);

};

struct UNSUBSCRIBERECORDINGJOBLOGRESPONSE : public RESPONSE
{
  UNSUBSCRIBERECORDINGJOBLOGRESPONSE();
  UNSUBSCRIBERECORDINGJOBLOGRESPONSE(const Error& error);

};

struct UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE : public RESPONSE
{
  UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE();
  UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE(const Error& error);

};

struct UNSUBSCRIBERECORDINGLOGRESPONSE : public RESPONSE
{
  UNSUBSCRIBERECORDINGLOGRESPONSE();
  UNSUBSCRIBERECORDINGLOGRESPONSE(const Error& error);

};

struct UNSUBSCRIBERECORDINGTRACKLOGRESPONSE : public RESPONSE
{
  UNSUBSCRIBERECORDINGTRACKLOGRESPONSE();
  UNSUBSCRIBERECORDINGTRACKLOGRESPONSE(const Error& error);

};

}

}

#endif
