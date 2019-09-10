// settrackconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////
  
SetTrackConfigurationResponse::SetTrackConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const std::string& tracktoken, const TrackConfiguration& trackconfiguration) :
  Response(client, localendpoint, latency, message),
  recordingtoken_(recordingtoken),
  tracktoken_(tracktoken),
  trackconfiguration_(trackconfiguration)
{
  
}

SetTrackConfigurationResponse::~SetTrackConfigurationResponse()
{
  
}

}

}
