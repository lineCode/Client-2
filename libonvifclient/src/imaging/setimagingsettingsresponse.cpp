// setimagingsettingsresponse.cpp
//

///// Includes /////

#include "onvifclient/imagingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace imaging
{

///// Methods /////

SetImagingSettingsResponse::SetImagingSettingsResponse(boost::shared_ptr<ImagingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& videosourcetoken, const ImagingSettings20& imagingsettings, bool forcepersistence) :
  Response(client, localendpoint, latency, message),
  videosourcetoken_(videosourcetoken),
  imagingsettings_(imagingsettings),
  forcepersistence_(forcepersistence)
{
  
}

SetImagingSettingsResponse::~SetImagingSettingsResponse()
{
  
}

}

}
