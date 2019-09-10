// deletetrackresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

DeleteTrackResponse::DeleteTrackResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const std::string& tracktoken) :
  Response(client, localendpoint, latency, message),
  recordingtoken_(recordingtoken),
  tracktoken_(tracktoken)
{

}

DeleteTrackResponse::~DeleteTrackResponse()
{
  
}

}

}
