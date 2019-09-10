// createtrackresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

CreateTrackResponse::CreateTrackResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const TrackConfiguration& trackconfiguration) :
  Response(client, localendpoint, latency, message),
  recordingtoken_(recordingtoken),
  trackconfiguration_(trackconfiguration)
{

}

CreateTrackResponse::CreateTrackResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const TrackConfiguration& trackconfiguration, const boost::optional<std::string>& tracktoken) :
  Response(client, localendpoint, latency, message),
  recordingtoken_(recordingtoken),
  trackconfiguration_(trackconfiguration),
  tracktoken_(tracktoken)
{
  
}

CreateTrackResponse::~CreateTrackResponse()
{
  
}

}

}
