// recordingserviceinterface.hpp
//

///// Includes /////

#include "onvifserver/recordingserviceinterface.hpp"

#include "onvifserver/soapfault.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace recording
{

///// Globals /////

const std::string RECORDINGNAMESPACEURI("http://www.onvif.org/ver10/recording/wsdl");

const std::string CREATERECORDING("CreateRecording");
const std::string CREATERECORDINGJOB("CreateRecordingJob");
const std::string CREATETRACK("CreateTrack");
const std::string DELETERECORDING("DeleteRecording");
const std::string DELETERECORDINGJOB("DeleteRecordingJob");
const std::string DELETETRACK("DeleteTrack");
const std::string GETRECORDINGCONFIGURATION("GetRecordingConfiguration");
const std::string GETRECORDINGJOBCONFIGURATION("GetRecordingJobConfiguration");
const std::string GETRECORDINGS("GetRecordings");
const std::string GETRECORDINGJOBS("GetRecordingJobs");
const std::string GETRECORDINGJOBSTATE("GetRecordingJobState");
const std::string GETRECORDINGOPTIONS("GetRecordingOptions");
const std::string GETSERVICECAPABILITIES("GetServiceCapabilities");
const std::string GETTRACKCONFIGURATION("GetTrackConfiguration");
const std::string SETRECORDINGCONFIGURATION("SetRecordingConfiguration");
const std::string SETRECORDINGJOBCONFIGURATION("SetRecordingJobConfiguration");
const std::string SETRECORDINGJOBMODE("SetRecordingJobMode");
const std::string SETTRACKCONFIGURATION("SetTrackConfiguration");

///// Declarations /////

const std::string RecordingServiceInterface::XADDR_("/onvif/recording");

///// Methods /////

RecordingServiceInterface::RecordingServiceInterface(const int onvifauthenticationtimediff) :
  OnvifService(2, 50, onvifauthenticationtimediff, { SOAP_NAMESPACE_ATTRIBUTE, SCHEMA_NAMESPACE_ATTRIBUTE, SCHEMA_INSTANCE_NAMESPACE_ATTRIBUTE, TYPES_NAMESPACE_ATTRIBUTE, RECORDING_NAMESPACE_ATTRIBUTE })
{

}

RecordingServiceInterface::~RecordingServiceInterface()
{
  Destroy();

}

int RecordingServiceInterface::Init()
{
  Destroy();
  return 0;
}

void RecordingServiceInterface::Destroy()
{

}

boost::shared_future<ServerResponse> RecordingServiceInterface::OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata)
{
  if (namespaceuri != RECORDINGNAMESPACEURI)
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NAMESPACE, std::string("Error parsing device namespace uri")).ToString()));
  }
  
  if (action == CREATERECORDING)
  {
    const auto recordingconfigurationnode = operationnode.select_node("*[local-name()='RecordingConfiguration']");
    if (!recordingconfigurationnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingConfiguration argument")).ToString()));
    }

    return boost::make_ready_future(CreateRecording(onvif::recording::RecordingConfiguration(recordingconfigurationnode.node())));
  }
  else if (action == CREATERECORDINGJOB)
  {
    const auto jobconfigurationnode = operationnode.select_node("*[local-name()='JobConfiguration']");
    if (!jobconfigurationnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing JobConfiguration argument")).ToString()));
    }

    return boost::make_ready_future(CreateRecordingJob(onvif::recording::RecordingJobConfiguration(jobconfigurationnode.node())));
  }
  else if (action == CREATETRACK)
  {
    const auto recordingtokennode = operationnode.select_node("*[local-name()='RecordingToken']");
    if (!recordingtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingToken")).ToString()));
    }

    uint64_t recordingtoken = 0;
    try
    {
      recordingtoken = boost::lexical_cast<uint64_t>(recordingtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDING, std::string("The RecordingToken does not reference an existing recording")).ToString()));
    }

    const auto trackconfigurationnode = operationnode.select_node("*[local-name()='TrackConfiguration']");
    if (!trackconfigurationnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing TrackConfiguration argument")).ToString()));
    }

    return boost::make_ready_future(CreateTrack(recordingtoken, onvif::TrackConfiguration(trackconfigurationnode.node())));
  }
  else if (action == DELETERECORDING)
  {
    const auto recordingtokennode = operationnode.select_node("*[local-name()='RecordingToken']");
    if (!recordingtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingToken")).ToString()));
    }

    uint64_t recordingtoken = 0;
    try
    {
      recordingtoken = boost::lexical_cast<uint64_t>(recordingtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDING, std::string("The RecordingToken does not reference an existing recording")).ToString()));
    }

    return boost::make_ready_future(DeleteRecording(recordingtoken));
  }
  else if (action == DELETERECORDINGJOB)
  {
    const auto jobtokennode = operationnode.select_node("*[local-name()='JobToken']");
    if (!jobtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing JobToken")).ToString()));
    }

    uint64_t jobtoken = 0;
    try
    {
      jobtoken = boost::lexical_cast<uint64_t>(jobtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDINGJOB, std::string("The JobToken does not reference an existing job")).ToString()));
    }

    return boost::make_ready_future(DeleteRecordingJob(jobtoken));
  }
  else if (action == DELETETRACK)
  {
    const auto recordingtokennode = operationnode.select_node("*[local-name()='RecordingToken']");
    if (!recordingtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingToken")).ToString()));
    }

    uint64_t recordingtoken = 0;
    try
    {
      recordingtoken = boost::lexical_cast<uint64_t>(recordingtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDING, std::string("The RecordingToken does not reference an existing recording")).ToString()));
    }

    const auto tracktokennode = operationnode.select_node("*[local-name()='TrackToken']");
    if (!tracktokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing TrackToken")).ToString()));
    }

    return boost::make_ready_future(DeleteTrack(recordingtoken, tracktokennode.node().text().get()));
  }
  else if (action == GETRECORDINGCONFIGURATION)
  {
    const auto recordingtokennode = operationnode.select_node("*[local-name()='RecordingToken']");
    if (!recordingtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingToken")).ToString()));
    }

    uint64_t recordingtoken = 0;
    try
    {
      recordingtoken = boost::lexical_cast<uint64_t>(recordingtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDING, std::string("The RecordingToken does not reference an existing recording")).ToString()));
    }

    return boost::make_ready_future(GetRecordingConfiguration(recordingtoken));
  }
  else if (action == GETRECORDINGJOBCONFIGURATION)
  {
    const auto jobtokennode = operationnode.select_node("*[local-name()='JobToken']");
    if (!jobtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing JobToken")).ToString()));
    }

    uint64_t jobtoken = 0;
    try
    {
      jobtoken = boost::lexical_cast<uint64_t>(jobtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDINGJOB, std::string("The JobToken does not reference an existing recording")).ToString()));
    }

    return boost::make_ready_future(GetRecordingJobConfiguration(jobtoken));
  }
  else if (action == GETRECORDINGJOBS)
  {

    return boost::make_ready_future(GetRecordingJobs());
  }
  else if (action == GETRECORDINGJOBSTATE)
  {
    const auto jobtokennode = operationnode.select_node("*[local-name()='JobToken']");
    if (!jobtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing JobToken")).ToString()));
    }

    uint64_t jobtoken = 0;
    try
    {
      jobtoken = boost::lexical_cast<uint64_t>(jobtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDINGJOB, std::string("The JobToken does not reference an existing job")).ToString()));
    }

    return boost::make_ready_future(GetRecordingJobState(jobtoken));
  }
  else if (action == GETRECORDINGS)
  {

    return boost::make_ready_future(GetRecordings());
  }
  else if (action == GETRECORDINGOPTIONS)
  {
    const auto recordingtokennode = operationnode.select_node("*[local-name()='RecordingToken']");
    if (!recordingtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingToken")).ToString()));
    }

    uint64_t recordingtoken = 0;
    try
    {
      recordingtoken = boost::lexical_cast<uint64_t>(recordingtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDING, std::string("The RecordingToken does not reference an existing recording")).ToString()));
    }

    return boost::make_ready_future(GetRecordingOptions(recordingtoken));
  }
  else if (action == GETSERVICECAPABILITIES)
  {

    return boost::make_ready_future(GetServiceCapabilities());
  }
  else if (action == GETTRACKCONFIGURATION)
  {
    const auto recordingtokennode = operationnode.select_node("*[local-name()='RecordingToken']");
    if (!recordingtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingToken")).ToString()));
    }

    uint64_t recordingtoken = 0;
    try
    {
      recordingtoken = boost::lexical_cast<uint64_t>(recordingtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDING, std::string("The RecordingToken does not reference an existing recording")).ToString()));
    }

    const auto tracktokennode = operationnode.select_node("*[local-name()='TrackToken']");
    if (!tracktokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing TrackToken")).ToString()));
    }

    return boost::make_ready_future(GetTrackConfiguration(recordingtoken, tracktokennode.node().text().get()));
  }
  else if (action == SETRECORDINGCONFIGURATION)
  {
    const auto recordingtokennode = operationnode.select_node("*[local-name()='RecordingToken']");
    if (!recordingtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingToken")).ToString()));
    }

    uint64_t recordingtoken = 0;
    try
    {
      recordingtoken = boost::lexical_cast<uint64_t>(recordingtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDING, std::string("The RecordingToken does not reference an existing recording")).ToString()));
    }

    const auto recordingconfigurationnode = operationnode.select_node("*[local-name()='RecordingConfiguration']");
    if (!recordingconfigurationnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingConfiguration argument")).ToString()));
    }

    return boost::make_ready_future(SetRecordingConfiguration(recordingtoken, onvif::recording::RecordingConfiguration(recordingconfigurationnode.node())));
  }
  else if (action == SETRECORDINGJOBCONFIGURATION)
  {
    const auto recordingjobtokennode = operationnode.select_node("*[local-name()='JobToken']");
    if (!recordingjobtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing JobToken")).ToString()));
    }

    uint64_t recordingjobtoken = 0;
    try
    {
      recordingjobtoken = boost::lexical_cast<uint64_t>(recordingjobtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDING, std::string("The JobToken does not reference an existing recording")).ToString()));
    }

    const auto jobconfigurationnode = operationnode.select_node("*[local-name()='JobConfiguration']");
    if (!jobconfigurationnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing JobConfiguration argument")).ToString()));
    }

    return boost::make_ready_future(SetRecordingJobConfiguration(recordingjobtoken, onvif::recording::RecordingJobConfiguration(jobconfigurationnode.node())));
  }
  else if (action == SETRECORDINGJOBMODE)
  {
    const auto jobtokennode = operationnode.select_node("*[local-name()='JobToken']");
    if (!jobtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing JobToken")).ToString()));
    }

    uint64_t jobtoken = 0;
    try
    {
      jobtoken = boost::lexical_cast<uint64_t>(jobtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDINGJOB, std::string("The JobToken does not reference an existing job")).ToString()));
    }

    const auto modenode = operationnode.select_node("*[local-name()='Mode']");
    if (!modenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Mode")).ToString()));
    }

    const boost::optional<onvif::recording::RECORDINGJOBMODE> mode = onvif::recording::GetRecordingJobMode(modenode.node().text().get());
    if (!mode.is_initialized())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid Mode")).ToString()));
    }

    return boost::make_ready_future(SetRecordingJobMode(jobtoken, *mode));
  }
  else if (action == SETTRACKCONFIGURATION)
  {
    const auto recordingtokennode = operationnode.select_node("*[local-name()='RecordingToken']");
    if (!recordingtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingToken")).ToString()));
    }

    uint64_t recordingtoken = 0;
    try
    {
      recordingtoken = boost::lexical_cast<uint64_t>(recordingtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDING, std::string("The RecordingToken does not reference an existing recording")).ToString()));
    }

    const auto tracktokennode = operationnode.select_node("*[local-name()='TrackToken']");
    if (!tracktokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingToken")).ToString()));
    }

    const auto treackconfigurationnode = operationnode.select_node("*[local-name()='TrackConfiguration']");
    if (!treackconfigurationnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing TrackConfiguration argument")).ToString()));
    }

    return boost::make_ready_future(SetTrackConfiguration(recordingtoken, tracktokennode.node().text().get(), onvif::TrackConfiguration(treackconfigurationnode.node())));
  }
  else
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNACTION, std::string("Unknown Action: ") + action).ToString()));
  }
}

}

}

}
