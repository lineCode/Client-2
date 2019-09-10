// dummymediaservice.hpp
//

///// Includes /////

#include "dummymediaservice.hpp"

#include <algorithm>
#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Methods /////

DummyMediaService::DummyMediaService()
{
  
}

DummyMediaService::~DummyMediaService()
{

}

boost::shared_future<server::ServerResponse> DummyMediaService::Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata)
{
  pugi::xml_document doc;
  if (!doc.load_buffer(content.data(), content.size()))
  {

    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  std::string response;
  if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddAudioDecoderConfiguration']"))
  {
    response = std::string("<media:AddAudioDecoderConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddAudioEncoderConfiguration']"))
  {
    response = std::string("<media:AddAudioEncoderConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddAudioOutputConfiguration']"))
  {
    response = std::string("<media:AddAudioOutputConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddAudioSourceConfiguration']"))
  {
    response = std::string("<media:AddAudioSourceConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddMetadataConfiguration']"))
  {
    response = std::string("<media:AddMetadataConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddPTZConfiguration']"))
  {
    response = std::string("<media:AddPTZConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddVideoAnalyticsConfiguration']"))
  {
    response = std::string("<media:AddVideoAnalyticsConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddVideoEncoderConfiguration']"))
  {
    response = std::string("<media:AddVideoEncoderConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddVideoSourceConfiguration']"))
  {
    response = std::string("<media:AddVideoSourceConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateProfile']"))
  {
    response = std::string("<media:CreateProfileResponse>") + createprofile_.ToXml("tt:Profile") + std::string("</media:CreateProfileResponse>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteProfile']"))
  {
    response = std::string("<media:DeleteProfileResponse/>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioDecoderConfiguration']"))
  {
    response = std::string("<media:GetAudioDecoderConfigurationResponse>") + getaudiodecoderconfiguration_.ToXml("tt:Configuration") + std::string("</media:GetAudioDecoderConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioDecoderConfigurationOptions']"))
  {
    response = std::string("<media:GetAudioDecoderConfigurationOptionsResponse>") + getaudiodecoderconfigurationoptions_.ToXml("tt:Options") + std::string("</media:GetAudioDecoderConfigurationOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioDecoderConfigurations']"))
  {
    std::string getaudiodecoderconfigurations;
    for (const auto& audiodecoderconfiguration : getaudiodecoderconfigurations_)
    {
      getaudiodecoderconfigurations += audiodecoderconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetAudioDecoderConfigurationsResponse>") + getaudiodecoderconfigurations + std::string("</media:GetAudioDecoderConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioEncoderConfiguration']"))
  {
    response = std::string("<media:GetAudioEncoderConfigurationResponse>") + getaudioencoderconfiguration_.ToXml("tt:Configuration") + std::string("</media:GetAudioEncoderConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioEncoderConfigurationOptions']"))
  {
    response = std::string("<media:GetAudioEncoderConfigurationOptionsResponse>") + getaudioencoderconfigurationoptions_.ToXml("tt:Options") + std::string("</media:GetAudioEncoderConfigurationOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioEncoderConfigurations']"))
  {
    std::string getaudioencoderconfigurations;
    for (const auto& audiodecoderconfiguration : getaudioencoderconfigurations_)
    {
      getaudioencoderconfigurations += audiodecoderconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetAudioEncoderConfigurationsResponse>") + getaudioencoderconfigurations + std::string("</media:GetAudioEncoderConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioOutputConfiguration']"))
  {
    response = std::string("<media:GetAudioOutputConfigurationResponse>") + getaudiooutputconfiguration_.ToXml("tt:Configuration") + std::string("</media:GetAudioOutputConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioOutputConfigurationOptions']"))
  {
    response = std::string("<media:GetAudioOutputConfigurationOptionsResponse>") + getaudiooutputconfigurationoptions_.ToXml("tt:Options") + std::string("</media:GetAudioOutputConfigurationOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioOutputConfigurations']"))
  {
    std::string getaudiooutputconfigurations;
    for (const auto& getaudiooutputconfiguration : getaudiooutputconfigurations_)
    {
      getaudiooutputconfigurations += getaudiooutputconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetAudioOutputConfigurationsResponse>") + getaudiooutputconfigurations + std::string("</media:GetAudioOutputConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSourceConfiguration']"))
  {
    response = std::string("<media:GetAudioSourceConfigurationResponse>") + getaudiosourceconfiguration_.ToXml("tt:Configuration") + std::string("</media:GetAudioSourceConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSourceConfigurationOptions']"))
  {
    response = std::string("<media:GetAudioSourceConfigurationOptionsResponse>") + getaudiosourceconfigurationoptions_.ToXml("tt:Options") + std::string("</media:GetAudioSourceConfigurationOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSourceConfigurations']"))
  {
    std::string getaudiosourceconfigurations;
    for (const auto& audiosourceconfiguration : getaudiosourceconfigurations_)
    {
      getaudiosourceconfigurations += audiosourceconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetAudioSourceConfigurationsResponse>") + getaudiosourceconfigurations + std::string("</media:GetAudioSourceConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSources']"))
  {
    std::string getaudiosources;
    for (const auto& audiosource : getaudiosources_)
    {
      getaudiosources += audiosource.ToXml("media:AudioSources");

    }

    response = std::string("<media:GetAudioSourcesResponse>") + getaudiosources + std::string("</media:GetAudioSourcesResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleAudioDecoderConfigurations']"))
  {
    std::string compatibleaudiodecoderconfigurations;
    for (const auto& compatibleaudiodecoderconfiguration : getcompatibleaudiodecoderconfigurations_)
    {
      compatibleaudiodecoderconfigurations += compatibleaudiodecoderconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetCompatibleAudioDecoderConfigurationsResponse>") + compatibleaudiodecoderconfigurations + std::string("</media:GetCompatibleAudioDecoderConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleAudioEncoderConfigurations']"))
  {
    std::string compatibleaudioencoderconfigurations;
    for (const auto& compatibleaudioencoderconfiguration : getcompatibleaudioencoderconfigurations_)
    {
      compatibleaudioencoderconfigurations += compatibleaudioencoderconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetCompatibleAudioEncoderConfigurationsResponse>") + compatibleaudioencoderconfigurations + std::string("</media:GetCompatibleAudioEncoderConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleAudioOutputConfigurations']"))
  {
    std::string compatibleaudiooutputconfigurations;
    for (const auto& compatibleaudiooutputconfiguration : getcompatibleaudiooutputconfigurations_)
    {
      compatibleaudiooutputconfigurations += compatibleaudiooutputconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetCompatibleAudioOutputConfigurationsResponse>") + compatibleaudiooutputconfigurations + std::string("</media:GetCompatibleAudioOutputConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleAudioSourceConfigurations']"))
  {
    std::string compatibleaudiosourceconfigurations;
    for (const auto& compatibleaudiosourceconfiguration : getcompatibleaudiosourceconfigurations_)
    {
      compatibleaudiosourceconfigurations += compatibleaudiosourceconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetCompatibleAudioSourceConfigurationsResponse>") + compatibleaudiosourceconfigurations + std::string("</media:GetCompatibleAudioSourceConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleMetadataConfigurations']"))
  {
    std::string compatiblemetadataconfigurations;
    for (const auto& compatiblemetadataconfiguration : getcompatiblemetadataconfigurations_)
    {
      compatiblemetadataconfigurations += compatiblemetadataconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetCompatibleMetadataConfigurationsResponse>") + compatiblemetadataconfigurations + std::string("</media:GetCompatibleMetadataConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleVideoAnalyticsConfigurations']"))
  {
    std::string compatiblevideoanalyticsconfigurations;
    for (const auto& compatiblevideoanalyticsconfiguration : getcompatiblevideoanalyticsconfigurations_)
    {
      compatiblevideoanalyticsconfigurations += compatiblevideoanalyticsconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetCompatibleVideoAnalyticsConfigurationsResponse>") + compatiblevideoanalyticsconfigurations + std::string("</media:GetCompatibleVideoAnalyticsConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleVideoEncoderConfigurations']"))
  {
    std::string compatiblevideoencoderconfigurations;
    for (const auto& compatiblevideoencoderconfiguration : getcompatiblevideoencoderconfigurations_)
    {
      compatiblevideoencoderconfigurations += compatiblevideoencoderconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetCompatibleVideoEncoderConfigurationsResponse>") + compatiblevideoencoderconfigurations + std::string("</media:GetCompatibleVideoEncoderConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleVideoSourceConfigurations']"))
  {
    std::string compatiblevideosourceconfigurations;
    for (const auto& compatiblevideosourceconfiguration : getcompatiblevideosourceconfigurations_)
    {
      compatiblevideosourceconfigurations += compatiblevideosourceconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetCompatibleVideoSourceConfigurationsResponse>") + compatiblevideosourceconfigurations + std::string("</media:GetCompatibleVideoSourceConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetMetadataConfiguration']"))
  {
    response = std::string("<media:GetMetadataConfigurationResponse>") + getmetadataconfiguration_.ToXml("tt:Configuration") + std::string("</media:GetMetadataConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetMetadataConfigurationOptions']"))
  {
    response = std::string("<media:GetMetadataConfigurationOptionsResponse>") + getmetadataconfigurationoptions_.ToXml("tt:Options") + std::string("</media:GetMetadataConfigurationOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetMetadataConfigurations']"))
  {
    std::string configurations;
    for (const auto& configuration : getmetadataconfigurations_)
    {
      configurations += configuration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetMetadataConfigurationsResponse>") + configurations + std::string("</media:GetMetadataConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetProfile']"))
  {
    response = std::string("<media:GetProfileResponse>") + getprofile_.ToXml("tt:Profile") + std::string("</media:GetProfileResponse>");
    
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetProfiles']"))
  {
    std::string profiles;
    for (const auto& profile : getprofiles_)
    {
      profiles += profile.ToXml("media:Profiles");
  
    }
  
    response = std::string("<media:GetProfilesResponse>") + profiles + std::string("</media:GetProfilesResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilities']"))
  {
    response = std::string("<media:GetServiceCapabilitiesResponse>") + getservicecapabilities_.ToXml("tt:Capabilities") + std::string("</media:GetServiceCapabilitiesResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSnapshotUri']"))
  {
    response = std::string("<media:GetSnapshotUriResponse>") + getsnapshoturi_.ToXml("tt:MediaUri") + std::string("</media:GetSnapshotUriResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetStreamUri']"))
  {
    response = std::string("<media:GetStreamUriResponse>") + getstreamuri_.ToXml("tt:MediaUri") + std::string("</media:GetStreamUriResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoAnalyticsConfigurations']"))
  {
    std::string getvideoanalyticsconfigurations;
    for (const auto& getvideoanalyticsconfiguration : getvideoanalyticsconfigurations_)
    {
      getvideoanalyticsconfigurations += getvideoanalyticsconfiguration.ToXml("media:Configurations");
  
    }
  
    response = std::string("<media:GetVideoAnalyticsConfigurationsResponse>") + getvideoanalyticsconfigurations + std::string("</media:GetVideoAnalyticsConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoEncoderConfiguration']"))
  {
    response = std::string("<media:GetVideoEncoderConfigurationResponse>") + getvideoencoderconfiguration_.ToXml("tt:Configuration") + std::string("</media:GetVideoEncoderConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoEncoderConfigurationOptions']"))
  {
    response = std::string("<media:GetVideoEncoderConfigurationOptionsResponse>") + getvideoencoderconfigurationoptions_.ToXml("tt:Options") + std::string("</media:GetVideoEncoderConfigurationOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoEncoderConfigurations']"))
  {
    std::string videoencoderconfigurations;
    for (const auto& videoencoderconfiguration : getvideoencoderconfigurations_)
    {
      videoencoderconfigurations += videoencoderconfiguration.ToXml("media:Configurations");

    }

    response = std::string("<media:GetVideoEncoderConfigurationsResponse>") + videoencoderconfigurations + std::string("</media:GetVideoEncoderConfigurationsResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoSourceConfiguration']"))
  {
    response = std::string("<media:GetVideoSourceConfigurationResponse>") + getvideosourceconfiguration_.ToXml("tt:Configuration") + std::string("</media:GetVideoSourceConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoSourceConfigurationOptions']"))
  {
    response = std::string("<media:GetVideoSourceConfigurationOptionsResponse>") + getvideosourceconfigurationoptions_.ToXml("tt:Options") + std::string("</media:GetVideoSourceConfigurationOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoSourceConfigurations']"))
  {
    std::string videosourceconfigurations;
    for (const auto& videosourceconfiguration : getvideosourceconfigurations_)
    {
      videosourceconfigurations += videosourceconfiguration.ToXml("media:Configurations");

    }

    response = "<media:GetVideoSourceConfigurationsResponse>" + videosourceconfigurations + "</media:GetVideoSourceConfigurationsResponse>";
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoSourceModes']"))
  {
    std::string videosourcemodes;
    for (const auto& getvideosourcemode : getvideosourcemodes_)
    {
      videosourcemodes += getvideosourcemode.ToXml("media:VideoSourceModes");

    }

    response = std::string("<media:GetVideoSourceModesResponse>") + videosourcemodes + std::string("</media:GetVideoSourceModesResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoSources']"))
  {
    std::string videosources;
    for (const auto& videosource : getvideosources_)
    {
      videosources += videosource.ToXml("media:VideoSources");
  
    }

    response = std::string("<media:GetVideoSourcesResponse>") + videosources + std::string("</media:GetVideoSourcesResponse>");
  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveAudioDecoderConfiguration']"))
  {
    response = std::string("<media:RemoveAudioDecoderConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveAudioEncoderConfiguration']"))
  {
    response = std::string("<media:RemoveAudioEncoderConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveAudioOutputConfiguration']"))
  {
    response = std::string("<media:RemoveAudioOutputConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveAudioSourceConfiguration']"))
  {
    response = std::string("<media:RemoveAudioSourceConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveMetadataConfiguration']"))
  {
    response = std::string("<media:RemoveMetadataConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemovePTZConfiguration']"))
  {
    response = std::string("<media:RemovePTZConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveVideoAnalyticsConfiguration']"))
  {
    response = std::string("<media:RemoveVideoAnalyticsConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveVideoEncoderConfiguration']"))
  {
    response = std::string("<media:RemoveVideoEncoderConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveVideoSourceConfiguration']"))
  {
    response = std::string("<media:RemoveVideoSourceConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAudioDecoderConfiguration']"))
  {
    response = std::string("<media:SetAudioDecoderConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAudioOutputConfiguration']"))
  {
    response = std::string("<media:SetAudioOutputConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAudioSourceConfiguration']"))
  {
    response = std::string("<media:SetAudioSourceConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetMetadataConfiguration']"))
  {
    response = std::string("<media:SetMetadataConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetSynchronizationPoint']"))
  {
    response = std::string("<media:SetSynchronizationPointResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetVideoEncoderConfiguration']"))
  {
    response = std::string("<media:SetVideoEncoderConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetVideoSourceConfiguration']"))
  {
    response = std::string("<media:SetVideoSourceConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='StartMulticastStreaming']"))
  {
    response = std::string("<media:StartMulticastStreamingResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='StopMulticastStreaming']"))
  {
    response = std::string("<media:StopMulticastStreamingResponse/>");

  }
  else
  {
  
    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::OK, std::string("<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:media=\"http://www.onvif.org/ver10/media/wsdl\"><SOAP-ENV:Body>") + response + std::string("</SOAP-ENV:Body></SOAP-ENV:Envelope>")));
}

}

}
