// getimagingsettingsresponse.cpp
//

///// Includes /////

#include "onvifclient/imagingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace imaging
{

///// Methods /////

GetImagingSettingsResponse::GetImagingSettingsResponse(boost::shared_ptr<ImagingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& videosourcetoken) :
  Response(client, localendpoint, latency, message),
  videosourcetoken_(videosourcetoken)
{

}

GetImagingSettingsResponse::GetImagingSettingsResponse(boost::shared_ptr<ImagingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& videosourcetoken, const boost::optional<ImagingSettings20>& imagingsettings) :
  Response(client, localendpoint, latency, message),
  videosourcetoken_(videosourcetoken),
  imagingsettings_(imagingsettings)
{

}

GetImagingSettingsResponse::~GetImagingSettingsResponse()
{

}

}

}
