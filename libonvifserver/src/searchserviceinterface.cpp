// searchserviceinterface.hpp
//

///// Includes /////

#include "onvifserver/searchserviceinterface.hpp"

#include "onvifserver/soapfault.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace search
{

///// Globals /////

const std::string SEARCHNAMESPACEURI("http://www.onvif.org/ver10/search/wsdl");

const std::string ENDSEARCH("EndSearch");
const std::string FINDEVENTS("FindEvents");
const std::string FINDRECORDINGS("FindRecordings");
const std::string GETEVENTSEARCHRESULTS("GetEventSearchResults");
const std::string GETRECORDINGINFORMATION("GetRecordingInformation");
const std::string GETRECORDINGSEARCHRESULTS("GetRecordingSearchResults");
const std::string GETRECORDINGSUMMARY("GetRecordingSummary");
const std::string GETSERVICECAPABILITIES("GetServiceCapabilities");

///// Declarations /////

const std::string SearchServiceInterface::XADDR_("/onvif/search");

///// Methods /////

SearchServiceInterface::SearchServiceInterface(const int onvifauthenticationtimediff) :
  OnvifService(2, 42, onvifauthenticationtimediff, { SOAP_NAMESPACE_ATTRIBUTE, SCHEMA_NAMESPACE_ATTRIBUTE, SCHEMA_INSTANCE_NAMESPACE_ATTRIBUTE, TYPES_NAMESPACE_ATTRIBUTE, SEARCH_NAMESPACE_ATTRIBUTE, XS_NAMESPACE_ATTRIBUTE, WSNT_NAMESPACE_ATTRIBUTE, TOPICS_NAMESPACE_ATTRIBUTE })
{

}

SearchServiceInterface::~SearchServiceInterface()
{
  Destroy();

}

int SearchServiceInterface::Init()
{
  Destroy();
  return 0;
}

void SearchServiceInterface::Destroy()
{

}

boost::shared_future<ServerResponse> SearchServiceInterface::OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata)
{
  if (namespaceuri != SEARCHNAMESPACEURI)
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NAMESPACE, std::string("Error parsing device namespace uri")).ToString()));
  }

  if (action == ENDSEARCH)
  {
    const auto searchtokennode = operationnode.select_node("*[local-name()='SearchToken']");
    if (!searchtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing SearchToken")).ToString()));
    }

    int64_t searchtoken = 0;
    try
    {
      searchtoken = boost::lexical_cast<int64_t>(searchtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("The search token is invalid")).ToString()));
    }

    return boost::make_ready_future(EndSearch(searchtoken));
  }
  else if (action == FINDEVENTS)
  {
    // StartPoint
    const auto startpointnode = operationnode.select_node("*[local-name()='StartPoint']");
    if (!startpointnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing StartPoint argument")).ToString()));
    }

    // EndPoint
    boost::optional<onvif::ws::DateTime> endpoint;
    const auto endpointnode = operationnode.select_node("*[local-name()='EndPoint']");
    if (endpointnode)
    {
      endpoint = onvif::ws::DateTime(endpointnode.node());

    }

    // Scope
    const auto scopenode = operationnode.select_node("*[local-name()='Scope']");
    if (!scopenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Scope argument")).ToString()));
    }

    // SearchFilter
    const auto searchfilternode = operationnode.select_node("*[local-name()='SearchFilter']");
    if (!searchfilternode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing SearchFilter argument")).ToString()));
    }

    // IncludeStartState
    const auto includestartstatenode = operationnode.select_node("*[local-name()='IncludeStartState']");
    if (!includestartstatenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing IncludeStartState argument")).ToString()));
    }

    // MaxMatches
    const auto maxmatchesnode = operationnode.select_node("*[local-name()='MaxMatches']");
    boost::optional<int> maxmatches;
    if (maxmatchesnode)
    {
      maxmatches = maxmatchesnode.node().text().as_int();

    }

    // KeepAliveTime
    const auto keepalivetimenode = operationnode.select_node("*[local-name()='KeepAliveTime']");
    if (!keepalivetimenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing KeepAliveTime argument")).ToString()));
    }

    const boost::optional<onvif::Duration> duration = onvif::GetDuration(keepalivetimenode.node().text().get());
    if (!duration.is_initialized())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid KeepAliveTime argument")).ToString()));
    }

    return boost::make_ready_future(FindEvents(onvif::ws::DateTime(startpointnode.node()), endpoint, onvif::SearchScope(scopenode.node()), searchfilternode.node().text().get(), includestartstatenode.node().text().as_bool(), maxmatches, *duration));
  }
  else if (action == FINDRECORDINGS)
  {
    const auto scopenode = operationnode.select_node("*[local-name()='Scope']");
    if (!scopenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Scope argument")).ToString()));
    }

    const auto maxmatchesnode = operationnode.select_node("*[local-name()='MaxMatches']");
    boost::optional<int> maxmatches;
    if (maxmatchesnode)
    {
      maxmatches = maxmatchesnode.node().text().as_int();

    }

    const auto keepalivetimenode = operationnode.select_node("*[local-name()='KeepAliveTime']");
    if (!keepalivetimenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing KeepAliveTime argument")).ToString()));
    }

    const boost::optional<onvif::Duration> duration = onvif::GetDuration(keepalivetimenode.node().text().get());
    if (!duration.is_initialized())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid KeepAliveTime argument")).ToString()));
    }

    return boost::make_ready_future(FindRecordings(onvif::SearchScope(scopenode.node()), maxmatches, *duration));
  }
  else if (action == GETEVENTSEARCHRESULTS)
  {
    // SearchToken
    const auto searchtokennode = operationnode.select_node("*[local-name()='SearchToken']");
    if (!searchtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing SearchToken")).ToString()));
    }

    int64_t searchtoken = 0;
    try
    {
      searchtoken = boost::lexical_cast<int64_t>(searchtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("The search token is invalid")).ToString()));
    }

    // MinResults
    boost::optional<int> minresults;
    const auto minresultsnode = operationnode.select_node("*[local-name()='MinResults']");
    if (minresultsnode)
    {
      try
      {
        minresults = boost::lexical_cast<int>(minresultsnode.node().text().get());

      }
      catch (const boost::bad_lexical_cast&)
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("MinResults is invalid")).ToString()));
      }
    }

    // MaxResults
    boost::optional<int> maxresults;
    const auto maxresultsnode = operationnode.select_node("*[local-name()='MaxResults']");
    if (maxresultsnode)
    {
      try
      {
        maxresults = boost::lexical_cast<int>(maxresultsnode.node().text().get());

      }
      catch (const boost::bad_lexical_cast&)
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("MaxResults is invalid")).ToString()));
      }
    }

    // WaitTime
    boost::optional<onvif::Duration> waittime;
    const auto waittimenode = operationnode.select_node("*[local-name()='WaitTime']");
    if (waittimenode)
    {
      waittime = onvif::GetDuration(waittimenode.node().text().get());
      if (!waittime.is_initialized())
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("WaitTime is invalid")).ToString()));
      }
    }

    return GetEventSearchResults(searchtoken, minresults, maxresults, waittime);
  }
  else if (action == GETRECORDINGINFORMATION)
  {
    const auto recordingtokennode = operationnode.select_node("*[local-name()='RecordingToken']");
    if (!recordingtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing SearchToken")).ToString()));
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

    return boost::make_ready_future(GetRecordingInformation(recordingtoken));
  }
  else if (action == GETRECORDINGSEARCHRESULTS)
  {
    // SearchToken
    const auto searchtokennode = operationnode.select_node("*[local-name()='SearchToken']");
    if (!searchtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingToken")).ToString()));
    }

    int64_t searchtoken = 0;
    try
    {
      searchtoken = boost::lexical_cast<int64_t>(searchtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("The search token is invalid")).ToString()));
    }

    // MinResults
    boost::optional<int> minresults;
    const auto minresultsnode = operationnode.select_node("*[local-name()='MinResults']");
    if (minresultsnode)
    {
      try
      {
        minresults = boost::lexical_cast<int>(minresultsnode.node().text().get());

      }
      catch (const boost::bad_lexical_cast&)
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("MinResults is invalid")).ToString()));
      }
    }

    // MaxResults
    boost::optional<int> maxresults;
    const auto maxresultsnode = operationnode.select_node("*[local-name()='MaxResults']");
    if (maxresultsnode)
    {
      try
      {
        maxresults = boost::lexical_cast<int>(maxresultsnode.node().text().get());

      }
      catch (const boost::bad_lexical_cast&)
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("MaxResults is invalid")).ToString()));
      }
    }

    // WaitTime
    boost::optional<onvif::Duration> waittime;
    const auto waittimenode = operationnode.select_node("*[local-name()='WaitTime']");
    if (waittimenode)
    {
      waittime = onvif::GetDuration(waittimenode.node().text().get());
      if (!waittime.is_initialized())
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("WaitTime is invalid")).ToString()));
      }
    }

    return boost::make_ready_future(GetRecordingSearchResults(searchtoken, minresults, maxresults, waittime));
  }
  else if (action == GETRECORDINGSUMMARY)
  {

    return boost::make_ready_future(GetRecordingSummary());
  }
  else if (action == GETSERVICECAPABILITIES)
  {

    return boost::make_ready_future(GetServiceCapabilities());
  }
  else
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNACTION, std::string("Unknown Action: ") + action).ToString()));
  }
}

}

}

}
