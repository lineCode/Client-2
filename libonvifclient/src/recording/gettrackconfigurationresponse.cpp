// gettrackconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

GetTrackConfigurationResponse::GetTrackConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const std::string& tracktoken) :
  Response(client, localendpoint, latency, message),
  recordingtoken_(recordingtoken),
  tracktoken_(tracktoken)
{

}

GetTrackConfigurationResponse::GetTrackConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const std::string& tracktoken, const boost::optional<TrackConfiguration>& trackconfiguration) :
  Response(client, localendpoint, latency, message),
  recordingtoken_(recordingtoken),
  tracktoken_(tracktoken),
  trackconfiguration_(trackconfiguration)
{
  
}

GetTrackConfigurationResponse::~GetTrackConfigurationResponse()
{
  
}

}

}
