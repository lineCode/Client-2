// recordingserviceinterface.hpp
//

#ifndef IDUIWOX4SDL4NVPW6RI428TIIY84RML2IW
#define IDUIWOX4SDL4NVPW6RI428TIIY84RML2IW

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <memory>
#include <string>

#include "onvifservice.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace recording
{

///// Globals /////

extern const std::string RECORDINGNAMESPACEURI;

extern const std::string CREATERECORDING;
extern const std::string CREATERECORDINGJOB;
extern const std::string CREATETRACK;
extern const std::string DELETERECORDING;
extern const std::string DELETERECORDINGJOB;
extern const std::string DELETETRACK;
extern const std::string GETRECORDINGCONFIGURATION;
extern const std::string GETRECORDINGJOBCONFIGURATION;
extern const std::string GETRECORDINGJOBS;
extern const std::string GETRECORDINGJOBSTATE;
extern const std::string GETRECORDINGS;
extern const std::string GETRECORDINGOPTIONS;
extern const std::string GETSERVICECAPABILITIES;
extern const std::string GETTRACKCONFIGURATION;
extern const std::string SETRECORDINGCONFIGURATION;
extern const std::string SETRECORDINGJOBCONFIGURATION;
extern const std::string SETRECORDINGJOBMODE;
extern const std::string SETTRACKCONFIGURATION;

///// Classes ////

class RecordingServiceInterface : public OnvifService
{
public:

  RecordingServiceInterface(const int onvifauthenticationtimediff);
  virtual ~RecordingServiceInterface();

  int Init();
  void Destroy();

  inline virtual const std::string& GetXAddr() const override { return XADDR_; }

  virtual onvif::RecordingCapabilities Capabilities(const std::string& localaddress, const uint16_t port) const = 0;
  virtual onvif::recording::Capabilities ServiceCapabilities() const = 0;

protected:

  virtual boost::shared_future<ServerResponse> OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata) override;

  virtual ServerResponse CreateRecording(const onvif::recording::RecordingConfiguration& recordingconfiguration) = 0;
  virtual ServerResponse CreateRecordingJob(const onvif::recording::RecordingJobConfiguration& jobconfiguration) = 0;
  virtual ServerResponse DeleteRecording(const uint64_t recordingtoken) = 0;
  virtual ServerResponse DeleteRecordingJob(const uint64_t recordingtoken) = 0;
  virtual ServerResponse DeleteTrack(const uint64_t recordingtoken, const std::string& tracktoken) = 0;
  virtual ServerResponse CreateTrack(const uint64_t recordingtoken, const onvif::TrackConfiguration& trackconfiguration) = 0;
  virtual ServerResponse GetRecordingConfiguration(const uint64_t recordingtoken) = 0;
  virtual ServerResponse GetRecordingJobConfiguration(const uint64_t jobtoken) = 0;
  virtual ServerResponse GetRecordingJobs() = 0;
  virtual ServerResponse GetRecordingJobState(const uint64_t jobtoken) = 0;
  virtual ServerResponse GetRecordings() = 0;
  virtual ServerResponse GetRecordingOptions(const uint64_t recordingtoken) = 0;
  virtual ServerResponse GetServiceCapabilities() = 0;
  virtual ServerResponse GetTrackConfiguration(const uint64_t recordingtoken, const std::string& tracktoken) = 0;
  virtual ServerResponse SetRecordingConfiguration(const uint64_t recordingtoken, const onvif::recording::RecordingConfiguration& recordingconfiguration) = 0;
  virtual ServerResponse SetRecordingJobConfiguration(const uint64_t jobtoken, const onvif::recording::RecordingJobConfiguration& recordingjobconfiguration) = 0;
  virtual ServerResponse SetRecordingJobMode(const uint64_t jobtoken, const onvif::recording::RECORDINGJOBMODE mode) = 0;
  virtual ServerResponse SetTrackConfiguration(const uint64_t recordingtoken, const std::string& tracktoken, const onvif::TrackConfiguration& trackconfiguration) = 0;

  static const std::string XADDR_;

private:


};

}

}

}

#endif
