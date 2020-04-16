// mediaclient.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Classes /////

class MediaSignals
{
 public:

  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, AddAudioDecoderConfigurationResponse, std::string, std::string > > addaudiodecoderconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, AddAudioEncoderConfigurationResponse, std::string, std::string > > addaudioencoderconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, AddAudioOutputConfigurationResponse, std::string, std::string > > addaudiooutputconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, AddAudioSourceConfigurationResponse, std::string, std::string > > addaudiosourceconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, AddMetadataConfigurationResponse, std::string, std::string > > addmetadataconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, AddPTZConfigurationResponse, std::string, std::string > > addptzconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, AddVideoAnalyticsConfigurationResponse, std::string, std::string > > addvideoanalyticsconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, AddVideoEncoderConfigurationResponse, std::string, std::string > > addvideoencoderconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, AddVideoSourceConfigurationResponse, std::string, std::string > > addvideosourceconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, CreateProfileResponse, std::string, boost::optional<std::string> > > createprofile_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, DeleteProfileResponse, std::string> > deleteprofile_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioDecoderConfigurationResponse, std::string> > getaudiodecoderconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioDecoderConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > > getaudiodecoderconfigurationoptions_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioDecoderConfigurationsResponse >> getaudiodecoderconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioEncoderConfigurationResponse, std::string> > getaudioencoderconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioEncoderConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > > getaudioencoderconfigurationoptions_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioEncoderConfigurationsResponse> > getaudioencoderconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioOutputConfigurationResponse, std::string> > getaudiooutputconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioOutputConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > > getaudiooutputconfigurationoptions_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioOutputConfigurationsResponse> > getaudiooutputconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioSourceConfigurationResponse, std::string> > getaudiosourceconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioSourceConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > > getaudiosourceconfigurationoptions_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioSourceConfigurationsResponse> > getaudiosourceconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetAudioSourcesResponse> > getaudiosources_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetCompatibleAudioDecoderConfigurationsResponse, std::string> > getcompatibleaudiodecoderconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetCompatibleAudioEncoderConfigurationsResponse, std::string> > getcompatibleaudioencoderconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetCompatibleAudioOutputConfigurationsResponse, std::string> > getcompatibleaudiooutputconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetCompatibleAudioSourceConfigurationsResponse, std::string> > getcompatibleaudiosourceconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetCompatibleMetadataConfigurationsResponse, std::string> > getcompatiblemetadataconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetCompatibleVideoAnalyticsConfigurationsResponse, std::string> > getcompatiblevideoanalyticsconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetCompatibleVideoEncoderConfigurationsResponse, std::string> > getcompatiblevideoencoderconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetCompatibleVideoSourceConfigurationsResponse, std::string> > getcompatiblevideosourceconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetMetadataConfigurationResponse, std::string> > getmetadataconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetMetadataConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > > getmetadataconfigurationoptions_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetMetadataConfigurationsResponse> > getmetadataconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetProfileResponse, std::string> > getprofile_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetProfilesResponse> > getprofiles_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetServiceCapabilitiesResponse> > getservicecapabilities_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetSnapshotUriResponse, std::string> > getsnapshoturi_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetStreamUriResponse, StreamSetup, std::string> > getstreamuri_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetVideoAnalyticsConfigurationsResponse> > getvideoanalyticsconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetVideoEncoderConfigurationResponse, std::string> > getvideoencoderconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetVideoEncoderConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > > getvideoencoderconfigurationoptions_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetVideoEncoderConfigurationsResponse> > getvideoencoderconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetVideoSourceConfigurationResponse, std::string> > getvideosourceconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetVideoSourceConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > > getvideosourceconfigurationoptions_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetVideoSourceConfigurationsResponse> > getvideosourceconfigurations_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetVideoSourceModesResponse, std::string> > getvideosourcemodes_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, GetVideoSourcesResponse> > getvideosources_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, RemoveAudioDecoderConfigurationResponse, std::string > > removeaudiodecoderconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, RemoveAudioEncoderConfigurationResponse, std::string > > removeaudioencoderconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, RemoveAudioOutputConfigurationResponse, std::string > > removeaudiooutputconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, RemoveAudioSourceConfigurationResponse, std::string > > removeaudiosourceconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, RemoveMetadataConfigurationResponse, std::string > > removemetadataconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, RemovePTZConfigurationResponse, std::string > > removeptzconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, RemoveVideoAnalyticsConfigurationResponse, std::string > > removevideoanalyticsconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, RemoveVideoEncoderConfigurationResponse, std::string > > removevideoencoderconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, RemoveVideoSourceConfigurationResponse, std::string > > removevideosourceconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, SetAudioDecoderConfigurationResponse, AudioDecoderConfiguration> > setaudiodecoderconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, SetAudioEncoderConfigurationResponse, AudioEncoderConfiguration> > setaudioencoderconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, SetAudioOutputConfigurationResponse, AudioOutputConfiguration> > setaudiooutputconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, SetAudioSourceConfigurationResponse, AudioSourceConfiguration> > setaudiosourceconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, SetMetadataConfigurationResponse, MetadataConfiguration> > setmetadataconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, SetSynchronizationPointResponse, std::string> > setsynchronizationpoint_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, SetVideoEncoderConfigurationResponse, VideoEncoderConfiguration> > setvideoencoderconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, SetVideoSourceConfigurationResponse, VideoSourceConfiguration> > setvideosourceconfiguration_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, StartMulticastStreamingResponse, std::string> > startmulticaststreaming_;
  std::unique_ptr< Signal< MEDIAOPERATION, MediaClient, StopMulticastStreamingResponse, std::string> > stopmulticaststreaming_;

};

///// Methods /////

MediaClient::MediaClient(const boost::shared_ptr<std::recursive_mutex>& mutex) :
  Client(mutex),
  signals_(new MediaSignals(
  {
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, AddAudioDecoderConfigurationResponse, std::string, std::string > >(this, MEDIAOPERATION_ADDAUDIODECODERCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/AddAudioDecoderConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, AddAudioEncoderConfigurationResponse, std::string, std::string > >(this, MEDIAOPERATION_ADDAUDIOENCODERCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/AddAudioEncoderConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, AddAudioOutputConfigurationResponse, std::string, std::string > >(this, MEDIAOPERATION_ADDAUDIOOUTPUTCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/AddAudioOutputConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, AddAudioSourceConfigurationResponse, std::string, std::string > >(this, MEDIAOPERATION_ADDAUDIOSOURCECONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/AddAudioSourceConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, AddMetadataConfigurationResponse, std::string, std::string > >(this, MEDIAOPERATION_ADDMETADATACONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/AddMetadataConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, AddPTZConfigurationResponse, std::string, std::string > >(this, MEDIAOPERATION_ADDPTZCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/AddPTZConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, AddVideoAnalyticsConfigurationResponse, std::string, std::string > >(this, MEDIAOPERATION_ADDVIDEOANALYTICSCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/AddVideoAnalyticsConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, AddVideoEncoderConfigurationResponse, std::string, std::string > >(this, MEDIAOPERATION_ADDVIDEOENCODERCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/AddVideoEncoderConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, AddVideoSourceConfigurationResponse, std::string, std::string > >(this, MEDIAOPERATION_ADDVIDEOSOURCECONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/AddVideoSourceConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, CreateProfileResponse, std::string, boost::optional<std::string> > >(this, MEDIAOPERATION_CREATEPROFILE, true, std::string("http://www.onvif.org/ver10/media/wsdl/CreateProfile"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, DeleteProfileResponse, std::string> >(this, MEDIAOPERATION_DELETEPROFILE, true, std::string("http://www.onvif.org/ver10/media/wsdl/DeleteProfile"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioDecoderConfigurationResponse, std::string> >(this, MEDIAOPERATION_GETAUDIODECODERCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioDecoderConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioDecoderConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > >(this, MEDIAOPERATION_GETAUDIODECODERCONFIGURATIONOPTIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioDecoderConfigurationOptions"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioDecoderConfigurationsResponse> >(this, MEDIAOPERATION_GETAUDIODECODERCONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioDecoderConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioEncoderConfigurationResponse, std::string> >(this, MEDIAOPERATION_GETAUDIOENCODERCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioEncoderConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioEncoderConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > >(this, MEDIAOPERATION_GETAUDIOENCODERCONFIGURATIONOPTIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioEncoderConfigurationOptions"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioEncoderConfigurationsResponse> >(this, MEDIAOPERATION_GETAUDIOENCODERCONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioEncoderConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioOutputConfigurationResponse, std::string> >(this, MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioOutputConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioOutputConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > >(this, MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATIONOPTIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioOutputConfigurationOptions"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioOutputConfigurationsResponse> >(this, MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioOutputConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioSourceConfigurationResponse, std::string> >(this, MEDIAOPERATION_GETAUDIOSOURCECONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioSourceConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioSourceConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > >(this, MEDIAOPERATION_GETAUDIOSOURCECONFIGURATIONOPTIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioSourceConfigurationOptions"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioSourceConfigurationsResponse> >(this, MEDIAOPERATION_GETAUDIOSOURCECONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioSourceConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetAudioSourcesResponse> >(this, MEDIAOPERATION_GETAUDIOSOURCES, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetAudioSources"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetCompatibleAudioDecoderConfigurationsResponse, std::string> >(this, MEDIAOPERATION_GETCOMPATIBLEAUDIODECODERCONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetCompatibleAudioDecoderConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetCompatibleAudioEncoderConfigurationsResponse, std::string> >(this, MEDIAOPERATION_GETCOMPATIBLEAUDIOENCODERCONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetCompatibleAudioEncoderConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetCompatibleAudioOutputConfigurationsResponse, std::string> >(this, MEDIAOPERATION_GETCOMPATIBLEAUDIOOUTPUTCONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetCompatibleAudioOutputConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetCompatibleAudioSourceConfigurationsResponse, std::string> >(this, MEDIAOPERATION_GETCOMPATIBLEAUDIOSOURCECONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetCompatibleAudioSourceConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetCompatibleMetadataConfigurationsResponse, std::string> >(this, MEDIAOPERATION_GETCOMPATIBLEMETADATACONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetCompatibleMetadataConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetCompatibleVideoAnalyticsConfigurationsResponse, std::string> >(this, MEDIAOPERATION_GETCOMPATIBLEVIDEOANALYTICSCONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetCompatibleVideoAnalyticsConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetCompatibleVideoEncoderConfigurationsResponse, std::string> >(this, MEDIAOPERATION_GETCOMPATIBLEVIDEOENCODERCONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetCompatibleVideoEncoderConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetCompatibleVideoSourceConfigurationsResponse, std::string> >(this, MEDIAOPERATION_GETCOMPATIBLEVIDEOSOURCECONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetCompatibleVideoSourceConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetMetadataConfigurationResponse, std::string> >(this, MEDIAOPERATION_GETMETADATACONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetMetadataConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetMetadataConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > >(this, MEDIAOPERATION_GETMETADATACONFIGURATIONOPTIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetMetadataConfigurationOptions"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetMetadataConfigurationsResponse> >(this, MEDIAOPERATION_GETMETADATACONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetMetadataConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetProfileResponse, std::string> >(this, MEDIAOPERATION_GETPROFILE, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetProfile"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetProfilesResponse> >(this, MEDIAOPERATION_GETPROFILES, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetProfiles"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetServiceCapabilitiesResponse> >(this, MEDIAOPERATION_GETSERVICECAPABILITIES, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetServiceCapabilities"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetSnapshotUriResponse, std::string> >(this, MEDIAOPERATION_GETSNAPSHOTURI, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetSnapshotUri"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetStreamUriResponse, StreamSetup, std::string> >(this, MEDIAOPERATION_GETSTREAMURI, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetStreamUri"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetVideoAnalyticsConfigurationsResponse> >(this, MEDIAOPERATION_GETVIDEOANALYTICSCONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetVideoAnalyticsConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetVideoEncoderConfigurationResponse, std::string> >(this, MEDIAOPERATION_GETVIDEOENCODERCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetVideoEncoderConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetVideoEncoderConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > >(this, MEDIAOPERATION_GETVIDEOENCODERCONFIGURATIONOPTIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetVideoEncoderConfigurationOptions"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetVideoEncoderConfigurationsResponse> >(this, MEDIAOPERATION_GETVIDEOENCODERCONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetVideoEncoderConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetVideoSourceConfigurationResponse, std::string> >(this, MEDIAOPERATION_GETVIDEOSOURCECONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetVideoSourceConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetVideoSourceConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> > >(this, MEDIAOPERATION_GETVIDEOSOURCECONFIGURATIONOPTIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetVideoSourceConfigurationOptions"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetVideoSourceConfigurationsResponse> >(this, MEDIAOPERATION_GETVIDEOSOURCECONFIGURATIONS, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetVideoSourceConfigurations"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetVideoSourceModesResponse, std::string> >(this, MEDIAOPERATION_GETVIDEOSOURCEMODES, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetVideoSourceModes"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, GetVideoSourcesResponse> >(this, MEDIAOPERATION_GETVIDEOSOURCES, true, std::string("http://www.onvif.org/ver10/media/wsdl/GetVideoSources"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, RemoveAudioDecoderConfigurationResponse, std::string > >(this, MEDIAOPERATION_REMOVEAUDIODECODERCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/RemoveAudioDecoderConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, RemoveAudioEncoderConfigurationResponse, std::string > >(this, MEDIAOPERATION_REMOVEAUDIOENCODERCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/RemoveAudioEncoderConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, RemoveAudioOutputConfigurationResponse, std::string > >(this, MEDIAOPERATION_REMOVEAUDIOOUTPUTCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/RemoveAudioOutputConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, RemoveAudioSourceConfigurationResponse, std::string > >(this, MEDIAOPERATION_REMOVEAUDIOSOURCECONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/RemoveAudioSourceConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, RemoveMetadataConfigurationResponse, std::string > >(this, MEDIAOPERATION_REMOVEMETADATACONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/RemoveMetadataConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, RemovePTZConfigurationResponse, std::string > >(this, MEDIAOPERATION_REMOVEPTZCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/RemovePTZConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, RemoveVideoAnalyticsConfigurationResponse, std::string > >(this, MEDIAOPERATION_REMOVEVIDEOANALYTICSCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/RemoveVideoAnalyticsConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, RemoveVideoEncoderConfigurationResponse, std::string > >(this, MEDIAOPERATION_REMOVEVIDEOENCODERCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/RemoveVideoEncoderConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, RemoveVideoSourceConfigurationResponse, std::string > >(this, MEDIAOPERATION_REMOVEVIDEOSOURCECONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/RemoveVideoSourceConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, SetAudioDecoderConfigurationResponse, AudioDecoderConfiguration> >(this, MEDIAOPERATION_SETAUDIODECODERCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/SetAudioDecoderConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, SetAudioEncoderConfigurationResponse, AudioEncoderConfiguration> >(this, MEDIAOPERATION_SETAUDIOENCODERCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/SetAudioEncoderConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, SetAudioOutputConfigurationResponse, AudioOutputConfiguration> >(this, MEDIAOPERATION_SETAUDIOOUTPUTCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/SetAudioOutputConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, SetAudioSourceConfigurationResponse, AudioSourceConfiguration> >(this, MEDIAOPERATION_SETAUDIOSOURCECONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/SetAudioSourceConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, SetMetadataConfigurationResponse, MetadataConfiguration> >(this, MEDIAOPERATION_SETMETADATACONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/SetMetadataConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, SetSynchronizationPointResponse, std::string> >(this, MEDIAOPERATION_SETSYNCHRONIZATIONPOINT, true, std::string("http://www.onvif.org/ver10/media/wsdl/SetSynchronizationPoint"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, SetVideoEncoderConfigurationResponse, VideoEncoderConfiguration> >(this, MEDIAOPERATION_SETVIDEOENCODERCONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/SetVideoEncoderConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, SetVideoSourceConfigurationResponse, VideoSourceConfiguration> >(this, MEDIAOPERATION_SETVIDEOSOURCECONFIGURATION, true, std::string("http://www.onvif.org/ver10/media/wsdl/SetVideoSourceConfiguration"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, StartMulticastStreamingResponse, std::string> >(this, MEDIAOPERATION_STARTMULTICASTSTREAMING, true, std::string("http://www.onvif.org/ver10/media/wsdl/StartMulticastStreaming"), false),
    std::make_unique< Signal< MEDIAOPERATION, MediaClient, StopMulticastStreamingResponse, std::string> >(this, MEDIAOPERATION_STOPMULTICASTSTREAMING, true, std::string("http://www.onvif.org/ver10/media/wsdl/StopMulticastStreaming"), false)
  }))
{
  
}

MediaClient::~MediaClient()
{
  if (signals_)
  {
    delete signals_;
    signals_ = nullptr;
  }
}

void MediaClient::Destroy()
{
  Client::Destroy();

  signals_->addaudiodecoderconfiguration_->Destroy();
  signals_->addaudioencoderconfiguration_->Destroy();
  signals_->addaudiooutputconfiguration_->Destroy();
  signals_->addaudiosourceconfiguration_->Destroy();
  signals_->addmetadataconfiguration_->Destroy();
  signals_->addptzconfiguration_->Destroy();
  signals_->addvideoanalyticsconfiguration_->Destroy();
  signals_->addvideoencoderconfiguration_->Destroy();
  signals_->addvideosourceconfiguration_->Destroy();
  signals_->createprofile_->Destroy();
  signals_->deleteprofile_->Destroy();
  signals_->getaudiodecoderconfiguration_->Destroy();
  signals_->getaudiodecoderconfigurationoptions_->Destroy();
  signals_->getaudiodecoderconfigurations_->Destroy();
  signals_->getaudioencoderconfiguration_->Destroy();
  signals_->getaudioencoderconfigurationoptions_->Destroy();
  signals_->getaudioencoderconfigurations_->Destroy();
  signals_->getaudiooutputconfiguration_->Destroy();
  signals_->getaudiooutputconfigurationoptions_->Destroy();
  signals_->getaudiooutputconfigurations_->Destroy();
  signals_->getaudiosourceconfiguration_->Destroy();
  signals_->getaudiosourceconfigurationoptions_->Destroy();
  signals_->getaudiosourceconfigurations_->Destroy();
  signals_->getaudiosources_->Destroy();
  signals_->getcompatibleaudiodecoderconfigurations_->Destroy();
  signals_->getcompatibleaudioencoderconfigurations_->Destroy();
  signals_->getcompatibleaudiooutputconfigurations_->Destroy();
  signals_->getcompatibleaudiosourceconfigurations_->Destroy();
  signals_->getcompatiblemetadataconfigurations_->Destroy();
  signals_->getcompatiblevideoanalyticsconfigurations_->Destroy();
  signals_->getcompatiblevideoencoderconfigurations_->Destroy();
  signals_->getcompatiblevideosourceconfigurations_->Destroy();
  signals_->getmetadataconfiguration_->Destroy();
  signals_->getmetadataconfigurationoptions_->Destroy();
  signals_->getmetadataconfigurations_->Destroy();
  signals_->getprofile_->Destroy();
  signals_->getprofiles_->Destroy();
  signals_->getservicecapabilities_->Destroy();
  signals_->getsnapshoturi_->Destroy();
  signals_->getstreamuri_->Destroy();
  signals_->getvideoanalyticsconfigurations_->Destroy();
  signals_->getvideoencoderconfiguration_->Destroy();
  signals_->getvideoencoderconfigurationoptions_->Destroy();
  signals_->getvideoencoderconfigurations_->Destroy();
  signals_->getvideosourceconfiguration_->Destroy();
  signals_->getvideosourceconfigurationoptions_->Destroy();
  signals_->getvideosourceconfigurations_->Destroy();
  signals_->getvideosourcemodes_->Destroy();
  signals_->getvideosources_->Destroy();
  signals_->removeaudiodecoderconfiguration_->Destroy();
  signals_->removeaudioencoderconfiguration_->Destroy();
  signals_->removeaudiooutputconfiguration_->Destroy();
  signals_->removeaudiosourceconfiguration_->Destroy();
  signals_->removemetadataconfiguration_->Destroy();
  signals_->removeptzconfiguration_->Destroy();
  signals_->removevideoanalyticsconfiguration_->Destroy();
  signals_->removevideoencoderconfiguration_->Destroy();
  signals_->removevideosourceconfiguration_->Destroy();
  signals_->setaudiodecoderconfiguration_->Destroy();
  signals_->setaudioencoderconfiguration_->Destroy();
  signals_->setaudiooutputconfiguration_->Destroy();
  signals_->setaudiosourceconfiguration_->Destroy();
  signals_->setmetadataconfiguration_->Destroy();
  signals_->setsynchronizationpoint_->Destroy();
  signals_->setvideoencoderconfiguration_->Destroy();
  signals_->setvideosourceconfiguration_->Destroy();
  signals_->startmulticaststreaming_->Destroy();
  signals_->stopmulticaststreaming_->Destroy();
}

// Requests
void MediaClient::AddAudioDecoderConfiguration(const std::string& profiletoken, const std::string& configurationtoken)
{
  signals_->addaudiodecoderconfiguration_->Create(AddAudioDecoderConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

void MediaClient::AddAudioEncoderConfiguration(const std::string& profiletoken, const std::string& configurationtoken)
{
  signals_->addaudioencoderconfiguration_->Create(AddAudioEncoderConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

void MediaClient::AddAudioOutputConfiguration(const std::string& profiletoken, const std::string& configurationtoken)
{
  signals_->addaudiooutputconfiguration_->Create(AddAudioOutputConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

void MediaClient::AddAudioSourceConfiguration(const std::string& profiletoken, const std::string& configurationtoken)
{
  signals_->addaudiosourceconfiguration_->Create(AddAudioSourceConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

void MediaClient::AddMetadataConfiguration(const std::string& profiletoken, const std::string& configurationtoken)
{
  signals_->addmetadataconfiguration_->Create(AddMetadataConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

void MediaClient::AddPTZConfiguration(const std::string& profiletoken, const std::string& configurationtoken)
{
  signals_->addptzconfiguration_->Create(AddPTZConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

void MediaClient::AddVideoAnalyticsConfiguration(const std::string& profiletoken, const std::string& configurationtoken)
{
  signals_->addvideoanalyticsconfiguration_->Create(AddVideoAnalyticsConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

void MediaClient::AddVideoEncoderConfiguration(const std::string& profiletoken, const std::string& configurationtoken)
{
  signals_->addvideoencoderconfiguration_->Create(AddVideoEncoderConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

void MediaClient::AddVideoSourceConfiguration(const std::string& profiletoken, const std::string& configurationtoken)
{
  signals_->addvideosourceconfiguration_->Create(AddVideoSourceConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

void MediaClient::CreateProfile(const std::string& name, const boost::optional<std::string>& token)
{
  signals_->createprofile_->Create(CreateProfileBody(name, token), name, token);
}

void MediaClient::DeleteProfile(const std::string& token)
{
  signals_->deleteprofile_->Create(DeleteProfileBody(token), token);
}

void MediaClient::GetAudioDecoderConfiguration(const std::string& configurationtoken)
{
  signals_->getaudiodecoderconfiguration_->Create(GetAudioDecoderConfigurationBody(configurationtoken), configurationtoken);
}

void MediaClient::GetAudioDecoderConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  signals_->getaudiodecoderconfigurationoptions_->Create(GetAudioDecoderConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

void MediaClient::GetAudioDecoderConfigurations()
{
  signals_->getaudiodecoderconfigurations_->Create(GetAudioDecoderConfigurationsBody());
}

void MediaClient::GetAudioEncoderConfiguration(const std::string& configurationtoken)
{
  signals_->getaudioencoderconfiguration_->Create(GetAudioEncoderConfigurationBody(configurationtoken), configurationtoken);
}

void MediaClient::GetAudioEncoderConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  signals_->getaudioencoderconfigurationoptions_->Create(GetAudioEncoderConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

void MediaClient::GetAudioEncoderConfigurations()
{
  signals_->getaudioencoderconfigurations_->Create(GetAudioEncoderConfigurationsBody());
}

void MediaClient::GetAudioOutputConfiguration(const std::string& configurationtoken)
{
  signals_->getaudiooutputconfiguration_->Create(GetAudioOutputConfigurationBody(configurationtoken), configurationtoken);
}

void MediaClient::GetAudioOutputConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  signals_->getaudiooutputconfigurationoptions_->Create(GetAudioOutputConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

void MediaClient::GetAudioOutputConfigurations()
{
  signals_->getaudiooutputconfigurations_->Create(GetAudioOutputConfigurationsBody());
}

void MediaClient::GetAudioSourceConfiguration(const std::string& configurationtoken)
{
  signals_->getaudiosourceconfiguration_->Create(GetAudioSourceConfigurationBody(configurationtoken), configurationtoken);
}

void MediaClient::GetAudioSourceConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  signals_->getaudiosourceconfigurationoptions_->Create(GetAudioSourceConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

void MediaClient::GetAudioSourceConfigurations()
{
  signals_->getaudiosourceconfigurations_->Create(GetAudioSourceConfigurationsBody());
}

void MediaClient::GetAudioSources()
{
  signals_->getaudiosources_->Create(GetAudioSourcesBody());
}

void MediaClient::GetCompatibleAudioDecoderConfigurations(const std::string& profiletoken)
{
  signals_->getcompatibleaudiodecoderconfigurations_->Create(GetCompatibleAudioDecoderConfigurationsBody(profiletoken), profiletoken);
}

void MediaClient::GetCompatibleAudioEncoderConfigurations(const std::string& profiletoken)
{
  signals_->getcompatibleaudioencoderconfigurations_->Create(GetCompatibleAudioEncoderConfigurationsBody(profiletoken), profiletoken);
}

void MediaClient::GetCompatibleAudioOutputConfigurations(const std::string& profiletoken)
{
  signals_->getcompatibleaudiooutputconfigurations_->Create(GetCompatibleAudioOutputConfigurationsBody(profiletoken), profiletoken);
}

void MediaClient::GetCompatibleAudioSourceConfigurations(const std::string& profiletoken)
{
  signals_->getcompatibleaudiosourceconfigurations_->Create(GetCompatibleAudioSourceConfigurationsBody(profiletoken), profiletoken);
}

void MediaClient::GetCompatibleMetadataConfigurations(const std::string& profiletoken)
{
  signals_->getcompatiblemetadataconfigurations_->Create(GetCompatibleMetadataConfigurationsBody(profiletoken), profiletoken);
}

void MediaClient::GetCompatibleVideoAnalyticsConfigurations(const std::string& profiletoken)
{
  signals_->getcompatiblevideoanalyticsconfigurations_->Create(GetCompatibleVideoAnalyticsConfigurationsBody(profiletoken), profiletoken);
}

void MediaClient::GetCompatibleVideoEncoderConfigurations(const std::string& profiletoken)
{
  signals_->getcompatiblevideoencoderconfigurations_->Create(GetCompatibleVideoEncoderConfigurationsBody(profiletoken), profiletoken);
}

void MediaClient::GetCompatibleVideoSourceConfigurations(const std::string& profiletoken)
{
  signals_->getcompatiblevideosourceconfigurations_->Create(GetCompatibleVideoSourceConfigurationsBody(profiletoken), profiletoken);
}

void MediaClient::GetMetadataConfiguration(const std::string& configurationtoken)
{
  signals_->getmetadataconfiguration_->Create(GetMetadataConfigurationBody(configurationtoken), configurationtoken);
}

void MediaClient::GetMetadataConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  signals_->getmetadataconfigurationoptions_->Create(GetMetadataConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

void MediaClient::GetMetadataConfigurations()
{
  signals_->getmetadataconfigurations_->Create(GetMetadataConfigurationsBody());
}

void MediaClient::GetProfile(const std::string& token)
{
  signals_->getprofile_->Create(GetProfileBody(token), token);
}

void MediaClient::GetProfiles()
{
  signals_->getprofiles_->Create(GetProfilesBody());
}

void MediaClient::GetServiceCapabilities()
{
  signals_->getservicecapabilities_->Create(GetServiceCapabilitiesBody());
}

void MediaClient::GetSnapshotUri(const std::string& token)
{
  signals_->getsnapshoturi_->Create(GetSnapshotUriBody(token), token);
}

void MediaClient::GetStreamUri(const StreamSetup& streamsetup, const std::string& token)
{
  signals_->getstreamuri_->Create(GetStreamUriBody(streamsetup, token), streamsetup, token);
}

void MediaClient::GetVideoAnalyticsConfigurations()
{
  signals_->getvideoanalyticsconfigurations_->Create(GetVideoAnalyticsConfigurationsBody());
}

void MediaClient::GetVideoEncoderConfiguration(const std::string& token)
{
  signals_->getvideoencoderconfiguration_->Create(GetVideoEncoderConfigurationBody(token), token);
}

void MediaClient::GetVideoEncoderConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  signals_->getvideoencoderconfigurationoptions_->Create(GetVideoEncoderConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

void MediaClient::GetVideoEncoderConfigurations()
{
  signals_->getvideoencoderconfigurations_->Create(GetVideoEncoderConfigurationsBody());
}

void MediaClient::GetVideoSourceConfiguration(const std::string& token)
{
  signals_->getvideosourceconfiguration_->Create(GetVideoSourceConfigurationBody(token), token);
}
void MediaClient::GetVideoSourceConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  signals_->getvideosourceconfigurationoptions_->Create(GetVideoSourceConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

void MediaClient::GetVideoSourceConfigurations()
{
  signals_->getvideosourceconfigurations_->Create(GetVideoSourceConfigurationsBody());
}

void MediaClient::GetVideoSourceModes(const std::string& token)
{
  signals_->getvideosourcemodes_->Create(GetVideoSourceModesBody(token), token);
}

void MediaClient::GetVideoSources()
{
  signals_->getvideosources_->Create(GetVideoSourcesBody());
}

void MediaClient::RemoveAudioDecoderConfiguration(const std::string& profiletoken)
{
  signals_->removeaudiodecoderconfiguration_->Create(RemoveAudioDecoderConfigurationBody(profiletoken), profiletoken);
}

void MediaClient::RemoveAudioEncoderConfiguration(const std::string& profiletoken)
{
  signals_->removeaudioencoderconfiguration_->Create(RemoveAudioEncoderConfigurationBody(profiletoken), profiletoken);
}

void MediaClient::RemoveAudioOutputConfiguration(const std::string& profiletoken)
{
  signals_->removeaudiooutputconfiguration_->Create(RemoveAudioOutputConfigurationBody(profiletoken), profiletoken);
}

void MediaClient::RemoveAudioSourceConfiguration(const std::string& profiletoken)
{
  signals_->removeaudiosourceconfiguration_->Create(RemoveAudioSourceConfigurationBody(profiletoken), profiletoken);
}

void MediaClient::RemoveMetadataConfiguration(const std::string& profiletoken)
{
  signals_->removemetadataconfiguration_->Create(RemoveMetadataConfigurationBody(profiletoken), profiletoken);
}

void MediaClient::RemovePTZConfiguration(const std::string& profiletoken)
{
  signals_->removeptzconfiguration_->Create(RemovePTZConfigurationBody(profiletoken), profiletoken);
}

void MediaClient::RemoveVideoAnalyticsConfiguration(const std::string& profiletoken)
{
  signals_->removevideoanalyticsconfiguration_->Create(RemoveVideoAnalyticsConfigurationBody(profiletoken), profiletoken);
}

void MediaClient::RemoveVideoEncoderConfiguration(const std::string& profiletoken)
{
  signals_->removevideoencoderconfiguration_->Create(RemoveVideoEncoderConfigurationBody(profiletoken), profiletoken);
}

void MediaClient::RemoveVideoSourceConfiguration(const std::string& profiletoken)
{
  signals_->removevideosourceconfiguration_->Create(RemoveVideoSourceConfigurationBody(profiletoken), profiletoken);
}

void MediaClient::SetAudioDecoderConfiguration(const AudioDecoderConfiguration& configuration)
{
  signals_->setaudiodecoderconfiguration_->Create(SetAudioDecoderConfigurationBody(configuration), configuration);
}

void MediaClient::SetAudioEncoderConfiguration(const AudioEncoderConfiguration& configuration)
{
  signals_->setaudioencoderconfiguration_->Create(SetAudioEncoderConfigurationBody(configuration), configuration);
}

void MediaClient::SetAudioOutputConfiguration(const AudioOutputConfiguration& configuration)
{
  signals_->setaudiooutputconfiguration_->Create(SetAudioOutputConfigurationBody(configuration), configuration);
}

void MediaClient::SetAudioSourceConfiguration(const AudioSourceConfiguration& configuration)
{
  signals_->setaudiosourceconfiguration_->Create(SetAudioSourceConfigurationBody(configuration), configuration);
}

void MediaClient::SetMetadataConfiguration(const MetadataConfiguration& configuration)
{
  signals_->setmetadataconfiguration_->Create(SetMetadataConfigurationBody(configuration), configuration);
}

void MediaClient::SetSynchronizationPoint(const std::string& profiletoken)
{
  signals_->setsynchronizationpoint_->Create(SetSynchronizationPointBody(profiletoken), profiletoken);
}

void MediaClient::SetVideoEncoderConfiguration(const VideoEncoderConfiguration& videoencoderconfiguration)
{
  signals_->setvideoencoderconfiguration_->Create(SetVideoEncoderConfigurationBody(videoencoderconfiguration), videoencoderconfiguration);
}

void MediaClient::SetVideoSourceConfiguration(const VideoSourceConfiguration& videosourceconfiguration)
{
  signals_->setvideosourceconfiguration_->Create(SetVideoSourceConfigurationBody(videosourceconfiguration), videosourceconfiguration);
}

void MediaClient::StartMulticastStreaming(const std::string& profiletoken)
{
  signals_->startmulticaststreaming_->Create(StartMulticastStreamingBody(profiletoken), profiletoken);
}

void MediaClient::StopMulticastStreaming(const std::string& profiletoken)
{
  signals_->stopmulticaststreaming_->Create(StopMulticastStreamingBody(profiletoken), profiletoken);
}

// Callbacks
Connection MediaClient::AddAudioDecoderConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddAudioDecoderConfigurationResponse&)> callback)
{
  return signals_->addaudiodecoderconfiguration_->CreateCallback(AddAudioDecoderConfigurationBody(profiletoken, configurationtoken), callback, profiletoken, configurationtoken);
}

Connection MediaClient::AddAudioEncoderConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddAudioEncoderConfigurationResponse&)> callback)
{
  return signals_->addaudioencoderconfiguration_->CreateCallback(AddAudioEncoderConfigurationBody(profiletoken, configurationtoken), callback, profiletoken, configurationtoken);
}

Connection MediaClient::AddAudioOutputConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddAudioOutputConfigurationResponse&)> callback)
{
  return signals_->addaudiooutputconfiguration_->CreateCallback(AddAudioOutputConfigurationBody(profiletoken, configurationtoken), callback, profiletoken, configurationtoken);
}

Connection MediaClient::AddAudioSourceConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddAudioSourceConfigurationResponse&)> callback)
{
  return signals_->addaudiosourceconfiguration_->CreateCallback(AddAudioSourceConfigurationBody(profiletoken, configurationtoken), callback, profiletoken, configurationtoken);
}

Connection MediaClient::AddMetadataConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddMetadataConfigurationResponse&)> callback)
{
  return signals_->addmetadataconfiguration_->CreateCallback(AddMetadataConfigurationBody(profiletoken, configurationtoken), callback, profiletoken, configurationtoken);
}

Connection MediaClient::AddPTZConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddPTZConfigurationResponse&)> callback)
{
  return signals_->addptzconfiguration_->CreateCallback(AddPTZConfigurationBody(profiletoken, configurationtoken), callback, profiletoken, configurationtoken);
}

Connection MediaClient::AddVideoAnalyticsConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddVideoAnalyticsConfigurationResponse&)> callback)
{
  return signals_->addvideoanalyticsconfiguration_->CreateCallback(AddVideoAnalyticsConfigurationBody(profiletoken, configurationtoken), callback, profiletoken, configurationtoken);
}

Connection MediaClient::AddVideoEncoderConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddVideoEncoderConfigurationResponse&)> callback)
{
  return signals_->addvideoencoderconfiguration_->CreateCallback(AddVideoEncoderConfigurationBody(profiletoken, configurationtoken), callback, profiletoken, configurationtoken);
}

Connection MediaClient::AddVideoSourceConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddVideoSourceConfigurationResponse&)> callback)
{
  return signals_->addvideosourceconfiguration_->CreateCallback(AddVideoSourceConfigurationBody(profiletoken, configurationtoken), callback, profiletoken, configurationtoken);
}

Connection MediaClient::CreateProfileCallback(const std::string& name, const boost::optional<std::string>& token, boost::function<void(const CreateProfileResponse&)> callback)
{
  return signals_->createprofile_->CreateCallback(CreateProfileBody(name, token), callback, name, token);
}

Connection MediaClient::DeleteProfileCallback(const std::string& token, boost::function<void(const DeleteProfileResponse&)> callback)
{
  return signals_->deleteprofile_->CreateCallback(DeleteProfileBody(token), callback, token);
}

Connection MediaClient::GetAudioDecoderConfigurationCallback(const std::string& configurationtoken, boost::function<void(const GetAudioDecoderConfigurationResponse&)> callback)
{
  return signals_->getaudiodecoderconfiguration_->CreateCallback(GetAudioDecoderConfigurationBody(configurationtoken), callback, configurationtoken);
}

Connection MediaClient::GetAudioDecoderConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetAudioDecoderConfigurationOptionsResponse&)> callback)
{
  return signals_->getaudiodecoderconfigurationoptions_->CreateCallback(GetAudioDecoderConfigurationOptionsBody(configurationtoken, profiletoken), callback, configurationtoken, profiletoken);
}

Connection MediaClient::GetAudioDecoderConfigurationsCallback(boost::function<void(const GetAudioDecoderConfigurationsResponse&)> callback)
{
  return signals_->getaudiodecoderconfigurations_->CreateCallback(GetAudioDecoderConfigurationsBody(), callback);
}

Connection MediaClient::GetAudioEncoderConfigurationCallback(const std::string& configurationtoken, boost::function<void(const GetAudioEncoderConfigurationResponse&)> callback)
{
  return signals_->getaudioencoderconfiguration_->CreateCallback(GetAudioEncoderConfigurationBody(configurationtoken), callback, configurationtoken);
}

Connection MediaClient::GetAudioEncoderConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetAudioEncoderConfigurationOptionsResponse&)> callback)
{
  return signals_->getaudioencoderconfigurationoptions_->CreateCallback(GetAudioEncoderConfigurationOptionsBody(configurationtoken, profiletoken), callback, configurationtoken, profiletoken);
}

Connection MediaClient::GetAudioEncoderConfigurationsCallback(boost::function<void(const GetAudioEncoderConfigurationsResponse&)> callback)
{
  return signals_->getaudioencoderconfigurations_->CreateCallback(GetAudioEncoderConfigurationsBody(), callback);
}

Connection MediaClient::GetAudioOutputConfigurationCallback(const std::string& configurationtoken, boost::function<void(const GetAudioOutputConfigurationResponse&)> callback)
{
  return signals_->getaudiooutputconfiguration_->CreateCallback(GetAudioOutputConfigurationBody(configurationtoken), callback, configurationtoken);
}

Connection MediaClient::GetAudioOutputConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetAudioOutputConfigurationOptionsResponse&)> callback)
{
  return signals_->getaudiooutputconfigurationoptions_->CreateCallback(GetAudioOutputConfigurationOptionsBody(configurationtoken, profiletoken), callback, configurationtoken, profiletoken);
}

Connection MediaClient::GetAudioOutputConfigurationsCallback(boost::function<void(const GetAudioOutputConfigurationsResponse&)> callback)
{
  return signals_->getaudiooutputconfigurations_->CreateCallback(GetAudioOutputConfigurationsBody(), callback);
}

Connection MediaClient::GetAudioSourceConfigurationCallback(const std::string& configurationtoken, boost::function<void(const GetAudioSourceConfigurationResponse&)> callback)
{
  return signals_->getaudiosourceconfiguration_->CreateCallback(GetAudioSourceConfigurationBody(configurationtoken), callback, configurationtoken);
}

Connection MediaClient::GetAudioSourceConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetAudioSourceConfigurationOptionsResponse&)> callback)
{
  return signals_->getaudiosourceconfigurationoptions_->CreateCallback(GetAudioSourceConfigurationOptionsBody(configurationtoken, profiletoken), callback, configurationtoken, profiletoken);
}

Connection MediaClient::GetAudioSourceConfigurationsCallback(boost::function<void(const GetAudioSourceConfigurationsResponse&)> callback)
{
  return signals_->getaudiosourceconfigurations_->CreateCallback(GetAudioSourceConfigurationsBody(), callback);
}

Connection MediaClient::GetAudioSourcesCallback(boost::function<void(const GetAudioSourcesResponse&)> callback)
{
  return signals_->getaudiosources_->CreateCallback(GetAudioSourcesBody(), callback);
}

Connection MediaClient::GetCompatibleAudioDecoderConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleAudioDecoderConfigurationsResponse&)> callback)
{
  return signals_->getcompatibleaudiodecoderconfigurations_->CreateCallback(GetCompatibleAudioDecoderConfigurationsBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::GetCompatibleAudioEncoderConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleAudioEncoderConfigurationsResponse&)> callback)
{
  return signals_->getcompatibleaudioencoderconfigurations_->CreateCallback(GetCompatibleAudioEncoderConfigurationsBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::GetCompatibleAudioOutputConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleAudioOutputConfigurationsResponse&)> callback)
{
  return signals_->getcompatibleaudiooutputconfigurations_->CreateCallback(GetCompatibleAudioOutputConfigurationsBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::GetCompatibleAudioSourceConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleAudioSourceConfigurationsResponse&)> callback)
{
  return signals_->getcompatibleaudiosourceconfigurations_->CreateCallback(GetCompatibleAudioSourceConfigurationsBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::GetCompatibleMetadataConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleMetadataConfigurationsResponse&)> callback)
{
  return signals_->getcompatiblemetadataconfigurations_->CreateCallback(GetCompatibleMetadataConfigurationsBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::GetCompatibleVideoAnalyticsConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleVideoAnalyticsConfigurationsResponse&)> callback)
{
  return signals_->getcompatiblevideoanalyticsconfigurations_->CreateCallback(GetCompatibleVideoAnalyticsConfigurationsBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::GetCompatibleVideoEncoderConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleVideoEncoderConfigurationsResponse&)> callback)
{
  return signals_->getcompatiblevideoencoderconfigurations_->CreateCallback(GetCompatibleVideoEncoderConfigurationsBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::GetCompatibleVideoSourceConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleVideoSourceConfigurationsResponse&)> callback)
{
  return signals_->getcompatiblevideosourceconfigurations_->CreateCallback(GetCompatibleVideoSourceConfigurationsBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::GetMetadataConfigurationCallback(const std::string& configurationtoken, boost::function<void(const GetMetadataConfigurationResponse&)> callback)
{
  return signals_->getmetadataconfiguration_->CreateCallback(GetMetadataConfigurationBody(configurationtoken), callback, configurationtoken);
}

Connection MediaClient::GetMetadataConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetMetadataConfigurationOptionsResponse&)> callback)
{
  return signals_->getmetadataconfigurationoptions_->CreateCallback(GetMetadataConfigurationOptionsBody(configurationtoken, profiletoken), callback, configurationtoken, profiletoken);
}

Connection MediaClient::GetMetadataConfigurationsCallback(boost::function<void(const GetMetadataConfigurationsResponse&)> callback)
{
  return signals_->getmetadataconfigurations_->CreateCallback(GetMetadataConfigurationsBody(), callback);
}

Connection MediaClient::GetProfileCallback(const std::string& token, boost::function<void(const GetProfileResponse&)> callback)
{
  return signals_->getprofile_->CreateCallback(GetProfileBody(token), callback, token);
}

Connection MediaClient::GetProfilesCallback(boost::function<void(const GetProfilesResponse&)> callback)
{
  return signals_->getprofiles_->CreateCallback(GetProfilesBody(), callback);
}

Connection MediaClient::GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback)
{
  return signals_->getservicecapabilities_->CreateCallback(GetServiceCapabilitiesBody(), callback);
}

Connection MediaClient::GetSnapshotUriCallback(const std::string& token, boost::function<void(const GetSnapshotUriResponse&)> callback)
{
  return signals_->getsnapshoturi_->CreateCallback(GetSnapshotUriBody(token), callback, token);
}

Connection MediaClient::GetStreamUriCallback(const StreamSetup& streamsetup, const std::string& token, boost::function<void(const GetStreamUriResponse&)> callback)
{
  return signals_->getstreamuri_->CreateCallback(GetStreamUriBody(streamsetup, token), callback, streamsetup, token);
}

Connection MediaClient::GetVideoAnalyticsConfigurationsCallback(boost::function<void(const GetVideoAnalyticsConfigurationsResponse&)> callback)
{
  return signals_->getvideoanalyticsconfigurations_->CreateCallback(GetVideoAnalyticsConfigurationsBody(), callback);
}

Connection MediaClient::GetVideoEncoderConfigurationCallback(const std::string& token, boost::function<void(const GetVideoEncoderConfigurationResponse&)> callback)
{
  return signals_->getvideoencoderconfiguration_->CreateCallback(GetVideoEncoderConfigurationBody(token), callback, token);
}

Connection MediaClient::GetVideoEncoderConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetVideoEncoderConfigurationOptionsResponse&)> callback)
{
  return signals_->getvideoencoderconfigurationoptions_->CreateCallback(GetVideoEncoderConfigurationOptionsBody(configurationtoken, profiletoken), callback, configurationtoken, profiletoken);
}

Connection MediaClient::GetVideoEncoderConfigurationsCallback(boost::function<void(const GetVideoEncoderConfigurationsResponse&)> callback)
{
  return signals_->getvideoencoderconfigurations_->CreateCallback(GetVideoEncoderConfigurationsBody(), callback);
}

Connection MediaClient::GetVideoSourceConfigurationCallback(const std::string& token, boost::function<void(const GetVideoSourceConfigurationResponse&)> callback)
{
  return signals_->getvideosourceconfiguration_->CreateCallback(GetVideoSourceConfigurationBody(token), callback, token);
}

Connection MediaClient::GetVideoSourceConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetVideoSourceConfigurationOptionsResponse&)> callback)
{
  return signals_->getvideosourceconfigurationoptions_->CreateCallback(GetVideoSourceConfigurationOptionsBody(configurationtoken, profiletoken), callback, configurationtoken, profiletoken);
}

Connection MediaClient::GetVideoSourceConfigurationsCallback(boost::function<void(const GetVideoSourceConfigurationsResponse&)> callback)
{
  return signals_->getvideosourceconfigurations_->CreateCallback(GetVideoSourceConfigurationsBody(), callback);
}

Connection MediaClient::GetVideoSourceModesCallback(const std::string& token, boost::function<void(const GetVideoSourceModesResponse&)> callback)
{
  return signals_->getvideosourcemodes_->CreateCallback(GetVideoSourceModesBody(token), callback, token);
}

Connection MediaClient::GetVideoSourcesCallback(boost::function<void(const GetVideoSourcesResponse&)> callback)
{
  return signals_->getvideosources_->CreateCallback(GetVideoSourcesBody(), callback);
}

Connection MediaClient::RemoveAudioDecoderConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveAudioDecoderConfigurationResponse&)> callback)
{
  return signals_->removeaudiodecoderconfiguration_->CreateCallback(RemoveAudioDecoderConfigurationBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::RemoveAudioEncoderConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveAudioEncoderConfigurationResponse&)> callback)
{
  return signals_->removeaudioencoderconfiguration_->CreateCallback(RemoveAudioEncoderConfigurationBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::RemoveAudioOutputConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveAudioOutputConfigurationResponse&)> callback)
{
  return signals_->removeaudiooutputconfiguration_->CreateCallback(RemoveAudioOutputConfigurationBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::RemoveAudioSourceConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveAudioSourceConfigurationResponse&)> callback)
{
  return signals_->removeaudiosourceconfiguration_->CreateCallback(RemoveAudioSourceConfigurationBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::RemoveMetadataConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveMetadataConfigurationResponse&)> callback)
{
  return signals_->removemetadataconfiguration_->CreateCallback(RemoveMetadataConfigurationBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::RemovePTZConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemovePTZConfigurationResponse&)> callback)
{
  return signals_->removeptzconfiguration_->CreateCallback(RemovePTZConfigurationBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::RemoveVideoAnalyticsConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveVideoAnalyticsConfigurationResponse&)> callback)
{
  return signals_->removevideoanalyticsconfiguration_->CreateCallback(RemoveVideoAnalyticsConfigurationBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::RemoveVideoEncoderConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveVideoEncoderConfigurationResponse&)> callback)
{
  return signals_->removevideoencoderconfiguration_->CreateCallback(RemoveVideoEncoderConfigurationBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::RemoveVideoSourceConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveVideoSourceConfigurationResponse&)> callback)
{
  return signals_->removevideosourceconfiguration_->CreateCallback(RemoveVideoSourceConfigurationBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::SetAudioDecoderConfigurationCallback(const AudioDecoderConfiguration& configuration, boost::function<void(const SetAudioDecoderConfigurationResponse&)> callback)
{
  return signals_->setaudiodecoderconfiguration_->CreateCallback(SetAudioDecoderConfigurationBody(configuration), callback, configuration);
}

Connection MediaClient::SetAudioEncoderConfigurationCallback(const AudioEncoderConfiguration& configuration, boost::function<void(const SetAudioEncoderConfigurationResponse&)> callback)
{
  return signals_->setaudioencoderconfiguration_->CreateCallback(SetAudioEncoderConfigurationBody(configuration), callback, configuration);
}

Connection MediaClient::SetAudioOutputConfigurationCallback(const AudioOutputConfiguration& configuration, boost::function<void(const SetAudioOutputConfigurationResponse&)> callback)
{
  return signals_->setaudiooutputconfiguration_->CreateCallback(SetAudioOutputConfigurationBody(configuration), callback, configuration);
}

Connection MediaClient::SetAudioSourceConfigurationCallback(const AudioSourceConfiguration& configuration, boost::function<void(const SetAudioSourceConfigurationResponse&)> callback)
{
  return signals_->setaudiosourceconfiguration_->CreateCallback(SetAudioSourceConfigurationBody(configuration), callback, configuration);
}

Connection MediaClient::SetMetadataConfigurationCallback(const MetadataConfiguration& configuration, boost::function<void(const SetMetadataConfigurationResponse&)> callback)
{
  return signals_->setmetadataconfiguration_->CreateCallback(SetMetadataConfigurationBody(configuration), callback, configuration);
}

Connection MediaClient::SetSynchronizationPointCallback(const std::string& profiletoken, boost::function<void(const SetSynchronizationPointResponse&)> callback)
{
  return signals_->setsynchronizationpoint_->CreateCallback(SetSynchronizationPointBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::SetVideoEncoderConfigurationCallback(const VideoEncoderConfiguration& videoencoderconfiguration, boost::function<void(const SetVideoEncoderConfigurationResponse&)> callback)
{
  return signals_->setvideoencoderconfiguration_->CreateCallback(SetVideoEncoderConfigurationBody(videoencoderconfiguration), callback, videoencoderconfiguration);
}

Connection MediaClient::SetVideoSourceConfigurationCallback(const VideoSourceConfiguration& videosourceconfiguration, boost::function<void(const SetVideoSourceConfigurationResponse&)> callback)
{
  return signals_->setvideosourceconfiguration_->CreateCallback(SetVideoSourceConfigurationBody(videosourceconfiguration), callback, videosourceconfiguration);
}

Connection MediaClient::StartMulticastStreamingCallback(const std::string& profiletoken, boost::function<void(const StartMulticastStreamingResponse&)> callback)
{
  return signals_->startmulticaststreaming_->CreateCallback(StartMulticastStreamingBody(profiletoken), callback, profiletoken);
}

Connection MediaClient::StopMulticastStreamingCallback(const std::string& profiletoken, boost::function<void(const StopMulticastStreamingResponse&)> callback)
{
  return signals_->stopmulticaststreaming_->CreateCallback(StartMulticastStreamingBody(profiletoken), callback, profiletoken);
}

// Futures
boost::unique_future<AddAudioDecoderConfigurationResponse> MediaClient::AddAudioDecoderConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken)
{
  return signals_->addaudiodecoderconfiguration_->CreateFuture(AddAudioDecoderConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

boost::unique_future<AddAudioEncoderConfigurationResponse> MediaClient::AddAudioEncoderConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken)
{
  return signals_->addaudioencoderconfiguration_->CreateFuture(AddAudioEncoderConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

boost::unique_future<AddAudioOutputConfigurationResponse> MediaClient::AddAudioOutputConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken)
{
  return signals_->addaudiooutputconfiguration_->CreateFuture(AddAudioOutputConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

boost::unique_future<AddAudioSourceConfigurationResponse> MediaClient::AddAudioSourceConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken)
{
  return signals_->addaudiosourceconfiguration_->CreateFuture(AddAudioSourceConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

boost::unique_future<AddMetadataConfigurationResponse> MediaClient::AddMetadataConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken)
{
  return signals_->addmetadataconfiguration_->CreateFuture(AddMetadataConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

boost::unique_future<AddPTZConfigurationResponse> MediaClient::AddPTZConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken)
{
  return signals_->addptzconfiguration_->CreateFuture(AddPTZConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

boost::unique_future<AddVideoAnalyticsConfigurationResponse> MediaClient::AddVideoAnalyticsConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken)
{
  return signals_->addvideoanalyticsconfiguration_->CreateFuture(AddVideoAnalyticsConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

boost::unique_future<AddVideoEncoderConfigurationResponse> MediaClient::AddVideoEncoderConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken)
{
  return signals_->addvideoencoderconfiguration_->CreateFuture(AddVideoEncoderConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

boost::unique_future<AddVideoSourceConfigurationResponse> MediaClient::AddVideoSourceConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken)
{
  return signals_->addvideosourceconfiguration_->CreateFuture(AddVideoSourceConfigurationBody(profiletoken, configurationtoken), profiletoken, configurationtoken);
}

boost::unique_future<CreateProfileResponse> MediaClient::CreateProfileFuture(const std::string& name, const boost::optional<std::string>& token)
{
  return signals_->createprofile_->CreateFuture(CreateProfileBody(name, token), name, token);
}

boost::unique_future<DeleteProfileResponse> MediaClient::DeleteProfileFuture(const std::string& token)
{
  return signals_->deleteprofile_->CreateFuture(DeleteProfileBody(token), token);
}

boost::unique_future<GetAudioDecoderConfigurationResponse> MediaClient::GetAudioDecoderConfigurationFuture(const std::string& configurationtoken)
{
  return signals_->getaudiodecoderconfiguration_->CreateFuture(GetAudioDecoderConfigurationBody(configurationtoken), configurationtoken);
}

boost::unique_future<GetAudioDecoderConfigurationOptionsResponse> MediaClient::GetAudioDecoderConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return signals_->getaudiodecoderconfigurationoptions_->CreateFuture(GetAudioDecoderConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

boost::unique_future<GetAudioDecoderConfigurationsResponse> MediaClient::GetAudioDecoderConfigurationsFuture()
{
  return signals_->getaudiodecoderconfigurations_->CreateFuture(GetAudioDecoderConfigurationsBody());
}

boost::unique_future<GetAudioEncoderConfigurationResponse> MediaClient::GetAudioEncoderConfigurationFuture(const std::string& configurationtoken)
{
  return signals_->getaudioencoderconfiguration_->CreateFuture(GetAudioEncoderConfigurationBody(configurationtoken), configurationtoken);
}

boost::unique_future<GetAudioEncoderConfigurationOptionsResponse> MediaClient::GetAudioEncoderConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return signals_->getaudioencoderconfigurationoptions_->CreateFuture(GetAudioEncoderConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

boost::unique_future<GetAudioEncoderConfigurationsResponse> MediaClient::GetAudioEncoderConfigurationsFuture()
{
  return signals_->getaudioencoderconfigurations_->CreateFuture(GetAudioEncoderConfigurationsBody());
}

boost::unique_future<GetAudioOutputConfigurationResponse> MediaClient::GetAudioOutputConfigurationFuture(const std::string& configurationtoken)
{
  return signals_->getaudiooutputconfiguration_->CreateFuture(GetAudioOutputConfigurationBody(configurationtoken), configurationtoken);
}

boost::unique_future<GetAudioOutputConfigurationOptionsResponse> MediaClient::GetAudioOutputConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return signals_->getaudiooutputconfigurationoptions_->CreateFuture(GetAudioOutputConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

boost::unique_future<GetAudioOutputConfigurationsResponse> MediaClient::GetAudioOutputConfigurationsFuture()
{
  return signals_->getaudiooutputconfigurations_->CreateFuture(GetAudioOutputConfigurationsBody());
}

boost::unique_future<GetAudioSourceConfigurationResponse> MediaClient::GetAudioSourceConfigurationFuture(const std::string& configurationtoken)
{
  return signals_->getaudiosourceconfiguration_->CreateFuture(GetAudioSourceConfigurationBody(configurationtoken), configurationtoken);
}

boost::unique_future<GetAudioSourceConfigurationOptionsResponse> MediaClient::GetAudioSourceConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return signals_->getaudiosourceconfigurationoptions_->CreateFuture(GetAudioSourceConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

boost::unique_future<GetAudioSourceConfigurationsResponse> MediaClient::GetAudioSourceConfigurationsFuture()
{
  return signals_->getaudiosourceconfigurations_->CreateFuture(GetAudioSourceConfigurationsBody());
}

boost::unique_future<GetAudioSourcesResponse> MediaClient::GetAudioSourcesFuture()
{
  return signals_->getaudiosources_->CreateFuture(GetAudioSourcesBody());
}

boost::unique_future<GetCompatibleAudioDecoderConfigurationsResponse> MediaClient::GetCompatibleAudioDecoderConfigurationsFuture(const std::string& profiletoken)
{
  return signals_->getcompatibleaudiodecoderconfigurations_->CreateFuture(GetCompatibleAudioDecoderConfigurationsBody(profiletoken), profiletoken);
}

boost::unique_future<GetCompatibleAudioEncoderConfigurationsResponse> MediaClient::GetCompatibleAudioEncoderConfigurationsFuture(const std::string& profiletoken)
{
  return signals_->getcompatibleaudioencoderconfigurations_->CreateFuture(GetCompatibleAudioEncoderConfigurationsBody(profiletoken), profiletoken);
}

boost::unique_future<GetCompatibleAudioOutputConfigurationsResponse> MediaClient::GetCompatibleAudioOutputConfigurationsFuture(const std::string& profiletoken)
{
  return signals_->getcompatibleaudiooutputconfigurations_->CreateFuture(GetCompatibleAudioOutputConfigurationsBody(profiletoken), profiletoken);
}

boost::unique_future<GetCompatibleAudioSourceConfigurationsResponse> MediaClient::GetCompatibleAudioSourceConfigurationsFuture(const std::string& profiletoken)
{
  return signals_->getcompatibleaudiosourceconfigurations_->CreateFuture(GetCompatibleAudioSourceConfigurationsBody(profiletoken), profiletoken);
}

boost::unique_future<GetCompatibleMetadataConfigurationsResponse> MediaClient::GetCompatibleMetadataConfigurationsFuture(const std::string& profiletoken)
{
  return signals_->getcompatiblemetadataconfigurations_->CreateFuture(GetCompatibleMetadataConfigurationsBody(profiletoken), profiletoken);
}

boost::unique_future<GetCompatibleVideoAnalyticsConfigurationsResponse> MediaClient::GetCompatibleVideoAnalyticsConfigurationsFuture(const std::string& profiletoken)
{
  return signals_->getcompatiblevideoanalyticsconfigurations_->CreateFuture(GetCompatibleVideoAnalyticsConfigurationsBody(profiletoken), profiletoken);
}

boost::unique_future<GetCompatibleVideoEncoderConfigurationsResponse> MediaClient::GetCompatibleVideoEncoderConfigurationsFuture(const std::string& profiletoken)
{
  return signals_->getcompatiblevideoencoderconfigurations_->CreateFuture(GetCompatibleVideoEncoderConfigurationsBody(profiletoken), profiletoken);
}

boost::unique_future<GetCompatibleVideoSourceConfigurationsResponse> MediaClient::GetCompatibleVideoSourceConfigurationsFuture(const std::string& profiletoken)
{
  return signals_->getcompatiblevideosourceconfigurations_->CreateFuture(GetCompatibleVideoSourceConfigurationsBody(profiletoken), profiletoken);
}

boost::unique_future<GetMetadataConfigurationResponse> MediaClient::GetMetadataConfigurationFuture(const std::string& configurationtoken)
{
  return signals_->getmetadataconfiguration_->CreateFuture(GetMetadataConfigurationBody(configurationtoken), configurationtoken);
}

boost::unique_future<GetMetadataConfigurationOptionsResponse> MediaClient::GetMetadataConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return signals_->getmetadataconfigurationoptions_->CreateFuture(GetMetadataConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

boost::unique_future<GetMetadataConfigurationsResponse> MediaClient::GetMetadataConfigurationsFuture()
{
  return signals_->getmetadataconfigurations_->CreateFuture(GetMetadataConfigurationsBody());
}

boost::unique_future<GetProfileResponse> MediaClient::GetProfileFuture(const std::string& token)
{
  return signals_->getprofile_->CreateFuture(GetProfileBody(token), token);
}

boost::unique_future<GetProfilesResponse> MediaClient::GetProfilesFuture()
{
  return signals_->getprofiles_->CreateFuture(GetProfilesBody());
}

boost::unique_future<GetServiceCapabilitiesResponse> MediaClient::GetServiceCapabilitiesFuture()
{
  return signals_->getservicecapabilities_->CreateFuture(GetServiceCapabilitiesBody());
}

boost::unique_future<GetSnapshotUriResponse> MediaClient::GetSnapshotUriFuture(const std::string& token)
{
  return signals_->getsnapshoturi_->CreateFuture(GetSnapshotUriBody(token), token);
}

boost::unique_future<GetStreamUriResponse> MediaClient::GetStreamUriFuture(const StreamSetup& streamsetup, const std::string& token)
{
  return signals_->getstreamuri_->CreateFuture(GetStreamUriBody(streamsetup, token), streamsetup, token);
}

boost::unique_future<GetVideoAnalyticsConfigurationsResponse> MediaClient::GetVideoAnalyticsConfigurationsFuture()
{
  return signals_->getvideoanalyticsconfigurations_->CreateFuture(GetVideoAnalyticsConfigurationsBody());
}

boost::unique_future<GetVideoEncoderConfigurationResponse> MediaClient::GetVideoEncoderConfigurationFuture(const std::string& token)
{
  return signals_->getvideoencoderconfiguration_->CreateFuture(GetVideoEncoderConfigurationBody(token), token);
}

boost::unique_future<GetVideoEncoderConfigurationOptionsResponse> MediaClient::GetVideoEncoderConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return signals_->getvideoencoderconfigurationoptions_->CreateFuture(GetVideoEncoderConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

boost::unique_future<GetVideoEncoderConfigurationsResponse> MediaClient::GetVideoEncoderConfigurationsFuture()
{
  return signals_->getvideoencoderconfigurations_->CreateFuture(GetVideoEncoderConfigurationsBody());
}

boost::unique_future<GetVideoSourceConfigurationResponse> MediaClient::GetVideoSourceConfigurationFuture(const std::string& token)
{
  return signals_->getvideosourceconfiguration_->CreateFuture(GetVideoSourceConfigurationBody(token), token);
}

boost::unique_future<GetVideoSourceConfigurationOptionsResponse> MediaClient::GetVideoSourceConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return signals_->getvideosourceconfigurationoptions_->CreateFuture(GetVideoSourceConfigurationOptionsBody(configurationtoken, profiletoken), configurationtoken, profiletoken);
}

boost::unique_future<GetVideoSourceConfigurationsResponse> MediaClient::GetVideoSourceConfigurationsFuture()
{
  return signals_->getvideosourceconfigurations_->CreateFuture(GetVideoSourceConfigurationsBody());
}

boost::unique_future<GetVideoSourceModesResponse> MediaClient::GetVideoSourceModesFuture(const std::string& token)
{
  return signals_->getvideosourcemodes_->CreateFuture(GetVideoSourceModesBody(token), token);
}

boost::unique_future<GetVideoSourcesResponse> MediaClient::GetVideoSourcesFuture()
{
  return signals_->getvideosources_->CreateFuture(GetVideoSourcesBody());
}

boost::unique_future<RemoveAudioDecoderConfigurationResponse> MediaClient::RemoveAudioDecoderConfigurationFuture(const std::string& profiletoken)
{
  return signals_->removeaudiodecoderconfiguration_->CreateFuture(RemoveAudioDecoderConfigurationBody(profiletoken), profiletoken);
}

boost::unique_future<RemoveAudioEncoderConfigurationResponse> MediaClient::RemoveAudioEncoderConfigurationFuture(const std::string& profiletoken)
{
  return signals_->removeaudioencoderconfiguration_->CreateFuture(RemoveAudioEncoderConfigurationBody(profiletoken), profiletoken);
}

boost::unique_future<RemoveAudioOutputConfigurationResponse> MediaClient::RemoveAudioOutputConfigurationFuture(const std::string& profiletoken)
{
  return signals_->removeaudiooutputconfiguration_->CreateFuture(RemoveAudioOutputConfigurationBody(profiletoken), profiletoken);
}

boost::unique_future<RemoveAudioSourceConfigurationResponse> MediaClient::RemoveAudioSourceConfigurationFuture(const std::string& profiletoken)
{
  return signals_->removeaudiosourceconfiguration_->CreateFuture(RemoveAudioSourceConfigurationBody(profiletoken), profiletoken);
}

boost::unique_future<RemoveMetadataConfigurationResponse> MediaClient::RemoveMetadataConfigurationFuture(const std::string& profiletoken)
{
  return signals_->removemetadataconfiguration_->CreateFuture(RemoveMetadataConfigurationBody(profiletoken), profiletoken);
}

boost::unique_future<RemovePTZConfigurationResponse> MediaClient::RemovePTZConfigurationFuture(const std::string& profiletoken)
{
  return signals_->removeptzconfiguration_->CreateFuture(RemovePTZConfigurationBody(profiletoken), profiletoken);
}

boost::unique_future<RemoveVideoAnalyticsConfigurationResponse> MediaClient::RemoveVideoAnalyticsConfigurationFuture(const std::string& profiletoken)
{
  return signals_->removevideoanalyticsconfiguration_->CreateFuture(RemoveVideoAnalyticsConfigurationBody(profiletoken), profiletoken);
}

boost::unique_future<RemoveVideoEncoderConfigurationResponse> MediaClient::RemoveVideoEncoderConfigurationFuture(const std::string& profiletoken)
{
  return signals_->removevideoencoderconfiguration_->CreateFuture(RemoveVideoEncoderConfigurationBody(profiletoken), profiletoken);
}

boost::unique_future<RemoveVideoSourceConfigurationResponse> MediaClient::RemoveVideoSourceConfigurationFuture(const std::string& profiletoken)
{
  return signals_->removevideosourceconfiguration_->CreateFuture(RemoveVideoSourceConfigurationBody(profiletoken), profiletoken);
}

boost::unique_future<SetAudioDecoderConfigurationResponse> MediaClient::SetAudioDecoderConfigurationFuture(const AudioDecoderConfiguration& configuration)
{
  return signals_->setaudiodecoderconfiguration_->CreateFuture(SetAudioDecoderConfigurationBody(configuration), configuration);
}

boost::unique_future<SetAudioEncoderConfigurationResponse> MediaClient::SetAudioEncoderConfigurationFuture(const AudioEncoderConfiguration& configuration)
{
  return signals_->setaudioencoderconfiguration_->CreateFuture(SetAudioEncoderConfigurationBody(configuration), configuration);
}

boost::unique_future<SetAudioOutputConfigurationResponse> MediaClient::SetAudioOutputConfigurationFuture(const AudioOutputConfiguration& configuration)
{
  return signals_->setaudiooutputconfiguration_->CreateFuture(SetAudioOutputConfigurationBody(configuration), configuration);
}

boost::unique_future<SetAudioSourceConfigurationResponse> MediaClient::SetAudioSourceConfigurationFuture(const AudioSourceConfiguration& configuration)
{
  return signals_->setaudiosourceconfiguration_->CreateFuture(SetAudioSourceConfigurationBody(configuration), configuration);
}

boost::unique_future<SetMetadataConfigurationResponse> MediaClient::SetMetadataConfigurationFuture(const MetadataConfiguration& configuration)
{
  return signals_->setmetadataconfiguration_->CreateFuture(SetMetadataConfigurationBody(configuration), configuration);
}

boost::unique_future<SetSynchronizationPointResponse> MediaClient::SetSynchronizationPointFuture(const std::string& profiletoken)
{
  return signals_->setsynchronizationpoint_->CreateFuture(SetSynchronizationPointBody(profiletoken), profiletoken);
}

boost::unique_future<SetVideoEncoderConfigurationResponse> MediaClient::SetVideoEncoderConfigurationFuture(const VideoEncoderConfiguration& videoencoderconfiguration)
{
  return signals_->setvideoencoderconfiguration_->CreateFuture(SetVideoEncoderConfigurationBody(videoencoderconfiguration), videoencoderconfiguration);
}

boost::unique_future<SetVideoSourceConfigurationResponse> MediaClient::SetVideoSourceConfigurationFuture(const VideoSourceConfiguration& videosourceconfiguration)
{
  return signals_->setvideosourceconfiguration_->CreateFuture(SetVideoSourceConfigurationBody(videosourceconfiguration), videosourceconfiguration);
}

boost::unique_future<StartMulticastStreamingResponse> MediaClient::StartMulticastStreamingFuture(const std::string& profiletoken)
{
  return signals_->startmulticaststreaming_->CreateFuture(StartMulticastStreamingBody(profiletoken), profiletoken);
}

boost::unique_future<StopMulticastStreamingResponse> MediaClient::StopMulticastStreamingFuture(const std::string& profiletoken)
{
  return signals_->stopmulticaststreaming_->CreateFuture(StopMulticastStreamingBody(profiletoken), profiletoken);
}

// Signals
boost::signals2::signal<void(const AddAudioDecoderConfigurationResponse&)>& MediaClient::AddAudioDecoderConfigurationSignal()
{
  return signals_->addaudiodecoderconfiguration_->GetSignal();
}

boost::signals2::signal<void(const AddAudioEncoderConfigurationResponse&)>& MediaClient::AddAudioEncoderConfigurationSignal()
{
  return signals_->addaudioencoderconfiguration_->GetSignal();
}

boost::signals2::signal<void(const AddAudioOutputConfigurationResponse&)>& MediaClient::AddAudioOutputConfigurationSignal()
{
  return signals_->addaudiooutputconfiguration_->GetSignal();
}

boost::signals2::signal<void(const AddAudioSourceConfigurationResponse&)>& MediaClient::AddAudioSourceConfigurationSignal()
{
  return signals_->addaudiosourceconfiguration_->GetSignal();
}

boost::signals2::signal<void(const AddMetadataConfigurationResponse&)>& MediaClient::AddMetadataConfigurationSignal()
{
  return signals_->addmetadataconfiguration_->GetSignal();
}

boost::signals2::signal<void(const AddPTZConfigurationResponse&)>& MediaClient::AddPTZConfigurationSignal()
{
  return signals_->addptzconfiguration_->GetSignal();
}

boost::signals2::signal<void(const AddVideoAnalyticsConfigurationResponse&)>& MediaClient::AddVideoAnalyticsConfigurationSignal()
{
  return signals_->addvideoanalyticsconfiguration_->GetSignal();
}

boost::signals2::signal<void(const AddVideoEncoderConfigurationResponse&)>& MediaClient::AddVideoEncoderConfigurationSignal()
{
  return signals_->addvideoencoderconfiguration_->GetSignal();
}

boost::signals2::signal<void(const AddVideoSourceConfigurationResponse&)>& MediaClient::AddVideoSourceConfigurationSignal()
{
  return signals_->addvideosourceconfiguration_->GetSignal();
}

boost::signals2::signal<void(const CreateProfileResponse&)>& MediaClient::CreateProfileSignal()
{
  return signals_->createprofile_->GetSignal();
}

boost::signals2::signal<void(const DeleteProfileResponse&)>& MediaClient::DeleteProfileSignal()
{
  return signals_->deleteprofile_->GetSignal();
}

boost::signals2::signal<void(const GetAudioDecoderConfigurationResponse&)>& MediaClient::GetAudioDecoderConfigurationSignal()
{
  return signals_->getaudiodecoderconfiguration_->GetSignal();
}

boost::signals2::signal<void(const GetAudioDecoderConfigurationOptionsResponse&)>& MediaClient::GetAudioDecoderConfigurationOptionsSignal()
{
  return signals_->getaudiodecoderconfigurationoptions_->GetSignal();
}

boost::signals2::signal<void(const GetAudioDecoderConfigurationsResponse&)>& MediaClient::GetAudioDecoderConfigurationsSignal()
{
  return signals_->getaudiodecoderconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetAudioEncoderConfigurationResponse&)>& MediaClient::GetAudioEncoderConfigurationSignal()
{
  return signals_->getaudioencoderconfiguration_->GetSignal();
}

boost::signals2::signal<void(const GetAudioEncoderConfigurationOptionsResponse&)>& MediaClient::GetAudioEncoderConfigurationOptionsSignal()
{
  return signals_->getaudioencoderconfigurationoptions_->GetSignal();
}

boost::signals2::signal<void(const GetAudioEncoderConfigurationsResponse&)>& MediaClient::GetAudioEncoderConfigurationsSignal()
{
  return signals_->getaudioencoderconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetAudioOutputConfigurationResponse&)>& MediaClient::GetAudioOutputConfigurationSignal()
{
  return signals_->getaudiooutputconfiguration_->GetSignal();
}

boost::signals2::signal<void(const GetAudioOutputConfigurationOptionsResponse&)>& MediaClient::GetAudioOutputConfigurationOptionsSignal()
{
  return signals_->getaudiooutputconfigurationoptions_->GetSignal();
}

boost::signals2::signal<void(const GetAudioOutputConfigurationsResponse&)>& MediaClient::GetAudioOutputConfigurationsSignal()
{
  return signals_->getaudiooutputconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetAudioSourceConfigurationResponse&)>& MediaClient::GetAudioSourceConfigurationSignal()
{
  return signals_->getaudiosourceconfiguration_->GetSignal();
}

boost::signals2::signal<void(const GetAudioSourceConfigurationOptionsResponse&)>& MediaClient::GetAudioSourceConfigurationOptionsSignal()
{
  return signals_->getaudiosourceconfigurationoptions_->GetSignal();
}

boost::signals2::signal<void(const GetAudioSourceConfigurationsResponse&)>& MediaClient::GetAudioSourceConfigurationsSignal()
{
  return signals_->getaudiosourceconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetAudioSourcesResponse&)>& MediaClient::GetAudioSourcesSignal()
{
  return signals_->getaudiosources_->GetSignal();
}

boost::signals2::signal<void(const GetCompatibleAudioDecoderConfigurationsResponse&)>& MediaClient::GetCompatibleAudioDecoderConfigurations()
{
  return signals_->getcompatibleaudiodecoderconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetCompatibleAudioEncoderConfigurationsResponse&)>& MediaClient::GetCompatibleAudioEncoderConfigurations()
{
  return signals_->getcompatibleaudioencoderconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetCompatibleAudioOutputConfigurationsResponse&)>& MediaClient::GetCompatibleAudioOutputConfigurations()
{
  return signals_->getcompatibleaudiooutputconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetCompatibleAudioSourceConfigurationsResponse&)>& MediaClient::GetCompatibleAudioSourceConfigurations()
{
  return signals_->getcompatibleaudiosourceconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetCompatibleMetadataConfigurationsResponse&)>& MediaClient::GetCompatibleMetadataConfigurations()
{
  return signals_->getcompatiblemetadataconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetCompatibleVideoAnalyticsConfigurationsResponse&)>& MediaClient::GetCompatibleVideoAnalyticsConfigurations()
{
  return signals_->getcompatiblevideoanalyticsconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetCompatibleVideoEncoderConfigurationsResponse&)>& MediaClient::GetCompatibleVideoEncoderConfigurations()
{
  return signals_->getcompatiblevideoencoderconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetCompatibleVideoSourceConfigurationsResponse&)>& MediaClient::GetCompatibleVideoSourceConfigurations()
{
  return signals_->getcompatiblevideosourceconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetMetadataConfigurationResponse&)>& MediaClient::GetMetadataConfigurationSignal()
{
  return signals_->getmetadataconfiguration_->GetSignal();
}

boost::signals2::signal<void(const GetMetadataConfigurationOptionsResponse&)>& MediaClient::GetMetadataConfigurationOptionsSignal()
{
  return signals_->getmetadataconfigurationoptions_->GetSignal();
}

boost::signals2::signal<void(const GetMetadataConfigurationsResponse&)>& MediaClient::GetMetadataConfigurationsSignal()
{
  return signals_->getmetadataconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetProfileResponse&)>& MediaClient::GetProfileSignal()
{
  return signals_->getprofile_->GetSignal();
}

boost::signals2::signal<void(const GetProfilesResponse&)>& MediaClient::GetProfilesSignal()
{
  return signals_->getprofiles_->GetSignal();
}

boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& MediaClient::GetServiceCapabilitiesSignal()
{
  return signals_->getservicecapabilities_->GetSignal();
}

boost::signals2::signal<void(const GetSnapshotUriResponse&)>& MediaClient::GetSnapshotUriSignal()
{
  return signals_->getsnapshoturi_->GetSignal();
}

boost::signals2::signal<void(const GetStreamUriResponse&)>& MediaClient::GetStreamUriSignal()
{
  return signals_->getstreamuri_->GetSignal();
}

boost::signals2::signal<void(const GetVideoAnalyticsConfigurationsResponse&)>& MediaClient::GetVideoAnalyticsConfigurationsSignal()
{
  return signals_->getvideoanalyticsconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetVideoEncoderConfigurationResponse&)>& MediaClient::GetVideoEncoderConfigurationSignal()
{
  return signals_->getvideoencoderconfiguration_->GetSignal();
}

boost::signals2::signal<void(const GetVideoEncoderConfigurationOptionsResponse&)>& MediaClient::GetVideoEncoderConfiguratioOptionsSignal()
{
  return signals_->getvideoencoderconfigurationoptions_->GetSignal();
}

boost::signals2::signal<void(const GetVideoEncoderConfigurationsResponse&)>& MediaClient::GetVideoEncoderConfigurationsSignal()
{
  return signals_->getvideoencoderconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetVideoSourceConfigurationResponse&)>& MediaClient::GetVideoSourceConfigurationSignal()
{
  return signals_->getvideosourceconfiguration_->GetSignal();
}

boost::signals2::signal<void(const GetVideoSourceConfigurationOptionsResponse&)>& MediaClient::GetVideoSourceConfiguratioOptionsSignal()
{
  return signals_->getvideosourceconfigurationoptions_->GetSignal();
}

boost::signals2::signal<void(const GetVideoSourceConfigurationsResponse&)>& MediaClient::GetVideoSourceConfigurationsSignal()
{
  return signals_->getvideosourceconfigurations_->GetSignal();
}

boost::signals2::signal<void(const GetVideoSourceModesResponse&)>& MediaClient::GetVideoSourceModesSignal()
{
  return signals_->getvideosourcemodes_->GetSignal();
}

boost::signals2::signal<void(const GetVideoSourcesResponse&)>& MediaClient::GetVideoSourcesSignal()
{
  return signals_->getvideosources_->GetSignal();
}

boost::signals2::signal<void(const RemoveAudioDecoderConfigurationResponse&)>& MediaClient::RemoveAudioDecoderConfigurationSignal()
{
  return signals_->removeaudiodecoderconfiguration_->GetSignal();
}

boost::signals2::signal<void(const RemoveAudioEncoderConfigurationResponse&)>& MediaClient::RemoveAudioEncoderConfigurationSignal()
{
  return signals_->removeaudioencoderconfiguration_->GetSignal();
}

boost::signals2::signal<void(const RemoveAudioOutputConfigurationResponse&)>& MediaClient::RemoveAudioOutputConfigurationSignal()
{
  return signals_->removeaudiooutputconfiguration_->GetSignal();
}

boost::signals2::signal<void(const RemoveAudioSourceConfigurationResponse&)>& MediaClient::RemoveAudioSourceConfigurationSignal()
{
  return signals_->removeaudiosourceconfiguration_->GetSignal();
}

boost::signals2::signal<void(const RemoveMetadataConfigurationResponse&)>& MediaClient::RemoveMetadataConfigurationSignal()
{
  return signals_->removemetadataconfiguration_->GetSignal();
}

boost::signals2::signal<void(const RemovePTZConfigurationResponse&)>& MediaClient::RemovePTZConfigurationSignal()
{
  return signals_->removeptzconfiguration_->GetSignal();
}

boost::signals2::signal<void(const RemoveVideoAnalyticsConfigurationResponse&)>& MediaClient::RemoveVideoAnalyticsConfigurationSignal()
{
  return signals_->removevideoanalyticsconfiguration_->GetSignal();
}

boost::signals2::signal<void(const RemoveVideoEncoderConfigurationResponse&)>& MediaClient::RemoveVideoEncoderConfigurationSignal()
{
  return signals_->removevideoencoderconfiguration_->GetSignal();
}

boost::signals2::signal<void(const RemoveVideoSourceConfigurationResponse&)>& MediaClient::RemoveVideoSourceConfigurationSignal()
{
  return signals_->removevideosourceconfiguration_->GetSignal();
}

boost::signals2::signal<void(const SetAudioDecoderConfigurationResponse&)>& MediaClient::SetAudioDecoderConfigurationSignal()
{
  return signals_->setaudiodecoderconfiguration_->GetSignal();
}

boost::signals2::signal<void(const SetAudioEncoderConfigurationResponse&)>& MediaClient::SetAudioEncoderConfigurationSignal()
{
  return signals_->setaudioencoderconfiguration_->GetSignal();
}

boost::signals2::signal<void(const SetAudioOutputConfigurationResponse&)>& MediaClient::SetAudioOutputConfigurationSignal()
{
  return signals_->setaudiooutputconfiguration_->GetSignal();
}

boost::signals2::signal<void(const SetAudioSourceConfigurationResponse&)>& MediaClient::SetAudioSourceConfigurationSignal()
{
  return signals_->setaudiosourceconfiguration_->GetSignal();
}

boost::signals2::signal<void(const SetMetadataConfigurationResponse&)>& MediaClient::SetMetadataConfigurationSignal()
{
  return signals_->setmetadataconfiguration_->GetSignal();
}

boost::signals2::signal<void(const SetSynchronizationPointResponse&)>& MediaClient::SetSynchronizationPointSignal()
{
  return signals_->setsynchronizationpoint_->GetSignal();
}

boost::signals2::signal<void(const SetVideoEncoderConfigurationResponse&)>& MediaClient::SetVideoEncoderConfigurationSignal()
{
  return signals_->setvideoencoderconfiguration_->GetSignal();
}

boost::signals2::signal<void(const SetVideoSourceConfigurationResponse&)>& MediaClient::SetVideoSourceConfigurationSignal()
{
  return signals_->setvideosourceconfiguration_->GetSignal();
}

boost::signals2::signal<void(const StartMulticastStreamingResponse&)>& MediaClient::GetStartMulticastStreamingSignal()
{
  return signals_->startmulticaststreaming_->GetSignal();
}

boost::signals2::signal<void(const StopMulticastStreamingResponse&)>& MediaClient::GetStopMulticastStreamingSignal()
{
  return signals_->stopmulticaststreaming_->GetSignal();
}

void MediaClient::Update(MEDIAOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata)
{
  switch (operation)
  {
    case MEDIAOPERATION_ADDAUDIODECODERCONFIGURATION:
    {
      auto addaudiodecoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddAudioDecoderConfigurationResponse']");
      if (!addaudiodecoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_ADDAUDIODECODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/AddAudioDecoderConfigurationResponse element not found"));
        break;
      }

      signals_->addaudiodecoderconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_ADDAUDIOENCODERCONFIGURATION:
    {
      auto addaudiodecoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddAudioEncoderConfigurationResponse']");
      if (!addaudiodecoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_ADDAUDIOENCODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/AddAudioEncoderConfigurationResponse element not found"));
        break;
      }

      signals_->addaudioencoderconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_ADDAUDIOOUTPUTCONFIGURATION:
    {
      auto addaudiodecoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddAudioOutputConfigurationResponse']");
      if (!addaudiodecoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_ADDAUDIOOUTPUTCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/AddAudioOutputConfigurationResponse element not found"));
        break;
      }

      signals_->addaudiooutputconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_ADDAUDIOSOURCECONFIGURATION:
    {
      auto addaudiodecoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddAudioSourceConfigurationResponse']");
      if (!addaudiodecoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_ADDAUDIOSOURCECONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/AddAudioSourceConfigurationResponse element not found"));
        break;
      }

      signals_->addaudiosourceconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_ADDMETADATACONFIGURATION:
    {
      auto addaudiodecoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddMetadataConfigurationResponse']");
      if (!addaudiodecoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_ADDMETADATACONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/AddMetadataConfigurationResponse element not found"));
        break;
      }

      signals_->addmetadataconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_ADDPTZCONFIGURATION:
    {
      auto addaudiodecoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddPTZConfigurationResponse']");
      if (!addaudiodecoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_ADDPTZCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/AddPTZConfigurationResponse element not found"));
        break;
      }

      signals_->addptzconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_ADDVIDEOANALYTICSCONFIGURATION:
    {
      auto addaudiodecoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddVideoAnalyticsConfigurationResponse']");
      if (!addaudiodecoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_ADDVIDEOANALYTICSCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/AddVideoAnalyticsConfigurationResponse element not found"));
        break;
      }

      signals_->addvideoanalyticsconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_ADDVIDEOENCODERCONFIGURATION:
    {
      auto addaudiodecoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddVideoEncoderConfigurationResponse']");
      if (!addaudiodecoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_ADDVIDEOENCODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/AddVideoEncoderConfigurationResponse element not found"));
        break;
      }

      signals_->addvideoencoderconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_ADDVIDEOSOURCECONFIGURATION:
    {
      auto addaudiodecoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='AddVideoSourceConfigurationResponse']");
      if (!addaudiodecoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_ADDVIDEOSOURCECONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/AddVideoSourceConfigurationResponse element not found"));
        break;
      }

      signals_->addvideosourceconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_CREATEPROFILE:
    {
      auto createprofileresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateProfileResponse']");
      if (!createprofileresponse.node())
      {
        SignalError(MEDIAOPERATION_CREATEPROFILE, handle, localendpoint, latency, std::string("/Envelope/Body/CreateProfileResponse element not found"));
        break;
      }
      
      signals_->createprofile_->Emit(handle, localendpoint, latency, std::string(), GetClass<Profile>(createprofileresponse, "*[local-name()='Profile']"));
      break;
    }
    case MEDIAOPERATION_DELETEPROFILE:
    {
      auto profile = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteProfileResponse']");
      if (!profile.node())
      {
        SignalError(MEDIAOPERATION_CREATEPROFILE, handle, localendpoint, latency, std::string("/Envelope/Body/DeleteProfileResponse element not found"));
        break;
      }

      signals_->deleteprofile_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_GETAUDIODECODERCONFIGURATION:
    {
      auto getaudiodecoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioDecoderConfigurationResponse']");
      if (!getaudiodecoderconfigurationresponse)
      {
        SignalError(MEDIAOPERATION_GETAUDIODECODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioDecoderConfigurationResponse element not found"));
        break;
      }

      signals_->getaudiodecoderconfiguration_->Emit(handle, localendpoint, latency, std::string(), GetClass<AudioDecoderConfiguration>(getaudiodecoderconfigurationresponse, "*[local-name()='Configuration']"));
      break;
    }
    case MEDIAOPERATION_GETAUDIODECODERCONFIGURATIONOPTIONS:
    {
      auto getaudiodecoderconfigurationoptions = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioDecoderConfigurationOptionsResponse']");
      if (!getaudiodecoderconfigurationoptions)
      {
        SignalError(MEDIAOPERATION_GETAUDIODECODERCONFIGURATIONOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioDecoderConfigurationOptionsResponse element not found"));
        break;
      }
      
      signals_->getaudiodecoderconfigurationoptions_->Emit(handle, localendpoint, latency, std::string(), GetClass<AudioDecoderConfigurationOptions>(getaudiodecoderconfigurationoptions, "*[local-name()='Options']"));
      break;
    }
    case MEDIAOPERATION_GETAUDIODECODERCONFIGURATIONS:
    {
      auto getaudiodecoderconfigurations = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioDecoderConfigurationsResponse']");
      if (!getaudiodecoderconfigurations.node())
      {
        SignalError(MEDIAOPERATION_GETAUDIODECODERCONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioDecoderConfigurationsResponse element not found"));
        break;
      }

      std::vector<AudioDecoderConfiguration> audiodecoderconfigurations;
      for (const auto& audiodecoderconfiguration : getaudiodecoderconfigurations.node().select_nodes("*[local-name()='Configurations']"))
      {
        audiodecoderconfigurations.push_back(AudioDecoderConfiguration(audiodecoderconfiguration.node()));

      }

      signals_->getaudiodecoderconfigurations_->Emit(handle, localendpoint, latency, std::string(), audiodecoderconfigurations);
      break;
    }
    case MEDIAOPERATION_GETAUDIOENCODERCONFIGURATION:
    {
      auto getaudioencoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioEncoderConfigurationResponse']");
      if (!getaudioencoderconfigurationresponse)
      {
        SignalError(MEDIAOPERATION_GETAUDIOENCODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioEncoderConfigurationResponse element not found"));
        break;
      }

      signals_->getaudioencoderconfiguration_->Emit(handle, localendpoint, latency, std::string(), GetClass<AudioEncoderConfiguration>(getaudioencoderconfigurationresponse, "*[local-name()='Configuration']"));
      break;
    }
    case MEDIAOPERATION_GETAUDIOENCODERCONFIGURATIONOPTIONS:
    {
      auto getaudioencoderconfigurationoptions = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioEncoderConfigurationOptionsResponse']");
      if (!getaudioencoderconfigurationoptions)
      {
        SignalError(MEDIAOPERATION_GETAUDIOENCODERCONFIGURATIONOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioEncoderConfigurationOptionsResponse element not found"));
        break;
      }

      signals_->getaudioencoderconfigurationoptions_->Emit(handle, localendpoint, latency, std::string(), GetClass<AudioEncoderConfigurationOptions>(getaudioencoderconfigurationoptions, "*[local-name()='Options']"));
      break;
    }
    case MEDIAOPERATION_GETAUDIOENCODERCONFIGURATIONS:
    {
      auto getaudioencoderonfigurations = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioEncoderConfigurationsResponse']");
      if (!getaudioencoderonfigurations.node())
      {
        SignalError(MEDIAOPERATION_GETAUDIOENCODERCONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioEncoderConfigurationsResponse element not found"));
        break;
      }

      std::vector<AudioEncoderConfiguration> audioencoderconfigurations;
      for (const auto& audioencoderconfiguration : getaudioencoderonfigurations.node().select_nodes("*[local-name()='Configurations']"))
      {
        audioencoderconfigurations.push_back(AudioEncoderConfiguration(audioencoderconfiguration.node()));

      }

      signals_->getaudioencoderconfigurations_->Emit(handle, localendpoint, latency, std::string(), audioencoderconfigurations);
      break;
    }
    case MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATION:
    {
      auto getaudiooutputconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioOutputConfigurationResponse']");
      if (!getaudiooutputconfigurationresponse)
      {
        SignalError(MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioOutputConfigurationResponse element not found"));
        break;
      }

      signals_->getaudiooutputconfiguration_->Emit(handle, localendpoint, latency, std::string(), GetClass<AudioOutputConfiguration>(getaudiooutputconfigurationresponse, "*[local-name()='Configuration']"));
      break;
    }
    case MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATIONOPTIONS:
    {
      auto getaudiooutputconfigurationoptions = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioOutputConfigurationOptionsResponse']");
      if (!getaudiooutputconfigurationoptions)
      {
        SignalError(MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATIONOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioOutputConfigurationOptionsResponse element not found"));
        break;
      }

      signals_->getaudiooutputconfigurationoptions_->Emit(handle, localendpoint, latency, std::string(), GetClass<AudioOutputConfigurationOptions>(getaudiooutputconfigurationoptions, "*[local-name()='Options']"));
      break;
    }
    case MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATIONS:
    {
      auto getaudiooutputconfigurations = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioOutputConfigurationsResponse']");
      if (!getaudiooutputconfigurations.node())
      {
        SignalError(MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioOutputConfigurationsResponse element not found"));
        break;
      }

      std::vector<AudioOutputConfiguration> audiooutputconfigurations;
      for (const auto& audiooutputconfiguration : getaudiooutputconfigurations.node().select_nodes("*[local-name()='Configurations']"))
      {
        audiooutputconfigurations.push_back(AudioOutputConfiguration(audiooutputconfiguration.node()));

      }

      signals_->getaudiooutputconfigurations_->Emit(handle, localendpoint, latency, std::string(), audiooutputconfigurations);
      break;
    }
    case MEDIAOPERATION_GETAUDIOSOURCECONFIGURATION:
    {
      auto getaudiosourceconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSourceConfigurationResponse']");
      if (!getaudiosourceconfigurationresponse)
      {
        SignalError(MEDIAOPERATION_GETAUDIOSOURCECONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioSourceConfigurationResponse element not found"));
        break;
      }

      signals_->getaudiosourceconfiguration_->Emit(handle, localendpoint, latency, std::string(), GetClass<AudioSourceConfiguration>(getaudiosourceconfigurationresponse, "*[local-name()='Configuration']"));
      break;
    }
    case MEDIAOPERATION_GETAUDIOSOURCECONFIGURATIONOPTIONS:
    {
      auto getaudiosourceconfigurationoptions = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSourceConfigurationOptionsResponse']");
      if (!getaudiosourceconfigurationoptions)
      {
        SignalError(MEDIAOPERATION_GETAUDIOSOURCECONFIGURATIONOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioSourceConfigurationOptionsResponse element not found"));
        break;
      }

      signals_->getaudiosourceconfigurationoptions_->Emit(handle, localendpoint, latency, std::string(), GetClass<AudioSourceConfigurationOptions>(getaudiosourceconfigurationoptions, "*[local-name()='Options']"));
      break;
    }
    case MEDIAOPERATION_GETAUDIOSOURCECONFIGURATIONS:
    {
      auto getaudiosourceconfigurations = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSourceConfigurationsResponse']");
      if (!getaudiosourceconfigurations.node())
      {
        SignalError(MEDIAOPERATION_GETAUDIOSOURCECONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioSourceConfigurationsResponse element not found"));
        break;
      }

      std::vector<AudioSourceConfiguration> audiosourceconfigurations;
      for (const auto& audiosourceconfiguration : getaudiosourceconfigurations.node().select_nodes("*[local-name()='Configurations']"))
      {
        audiosourceconfigurations.push_back(AudioSourceConfiguration(audiosourceconfiguration.node()));

      }

      signals_->getaudiosourceconfigurations_->Emit(handle, localendpoint, latency, std::string(), audiosourceconfigurations);
      break;
    }
    case MEDIAOPERATION_GETAUDIOSOURCES:
    {
      auto getaudiosources = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetAudioSourcesResponse']");
      if (!getaudiosources.node())
      {
        SignalError(MEDIAOPERATION_GETAUDIOSOURCES, handle, localendpoint, latency, std::string("/Envelope/Body/GetAudioSourcesResponse element not found"));
        break;
      }

      std::vector<AudioSource> audiosources;
      for (const auto& audiosource : getaudiosources.node().select_nodes("*[local-name()='AudioSources']"))
      {
        audiosources.push_back(AudioSource(audiosource.node()));

      }

      signals_->getaudiosources_->Emit(handle, localendpoint, latency, std::string(), audiosources);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEAUDIODECODERCONFIGURATIONS:
    {
      auto getcompatibleaudiodecoderconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleAudioDecoderConfigurationsResponse']");
      if (!getcompatibleaudiodecoderconfigurationsresponse.node())
      {
        SignalError(MEDIAOPERATION_GETCOMPATIBLEAUDIODECODERCONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetCompatibleAudioDecoderConfigurationsResponse element not found"));
        break;
      }

      std::vector<AudioDecoderConfiguration> getcompatibleaudiodecoderconfigurations;
      for (const auto& compatibleaudiodecoderconfiguration : getcompatibleaudiodecoderconfigurationsresponse.node().select_nodes("*[local-name()='Configurations']"))
      {
        getcompatibleaudiodecoderconfigurations.push_back(AudioDecoderConfiguration(compatibleaudiodecoderconfiguration.node()));

      }

      signals_->getcompatibleaudiodecoderconfigurations_->Emit(handle, localendpoint, latency, std::string(), getcompatibleaudiodecoderconfigurations);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEAUDIOENCODERCONFIGURATIONS:
    {
      auto getcompatibleaudioencoderconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleAudioEncoderConfigurationsResponse']");
      if (!getcompatibleaudioencoderconfigurationsresponse.node())
      {
        SignalError(MEDIAOPERATION_GETCOMPATIBLEAUDIOENCODERCONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetCompatibleAudioEncoderConfigurationsResponse element not found"));
        break;
      }

      std::vector<AudioEncoderConfiguration> getcompatibleaudioencoderconfigurations;
      for (const auto& compatibleaudioencoderconfiguration : getcompatibleaudioencoderconfigurationsresponse.node().select_nodes("*[local-name()='Configurations']"))
      {
        getcompatibleaudioencoderconfigurations.push_back(AudioEncoderConfiguration(compatibleaudioencoderconfiguration.node()));

      }

      signals_->getcompatibleaudioencoderconfigurations_->Emit(handle, localendpoint, latency, std::string(), getcompatibleaudioencoderconfigurations);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEAUDIOOUTPUTCONFIGURATIONS:
    {
      auto getcompatibleaudiooutputconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleAudioOutputConfigurationsResponse']");
      if (!getcompatibleaudiooutputconfigurationsresponse.node())
      {
        SignalError(MEDIAOPERATION_GETCOMPATIBLEAUDIOOUTPUTCONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetCompatibleAudioOutputConfigurationsResponse element not found"));
        break;
      }

      std::vector<AudioOutputConfiguration> getcompatibleaudiooutputconfigurations;
      for (const auto& compatibleaudiooutputconfiguration : getcompatibleaudiooutputconfigurationsresponse.node().select_nodes("*[local-name()='Configurations']"))
      {
        getcompatibleaudiooutputconfigurations.push_back(AudioOutputConfiguration(compatibleaudiooutputconfiguration.node()));

      }

      signals_->getcompatibleaudiooutputconfigurations_->Emit(handle, localendpoint, latency, std::string(), getcompatibleaudiooutputconfigurations);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEAUDIOSOURCECONFIGURATIONS:
    {
      auto getcompatibleaudiosourceconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleAudioSourceConfigurationsResponse']");
      if (!getcompatibleaudiosourceconfigurationsresponse.node())
      {
        SignalError(MEDIAOPERATION_GETCOMPATIBLEAUDIOSOURCECONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetCompatibleAudioSourceConfigurationsResponse element not found"));
        break;
      }

      std::vector<AudioSourceConfiguration> getcompatibleaudiosourceconfigurations;
      for (const auto& compatibleaudiosourceconfiguration : getcompatibleaudiosourceconfigurationsresponse.node().select_nodes("*[local-name()='Configurations']"))
      {
        getcompatibleaudiosourceconfigurations.push_back(AudioSourceConfiguration(compatibleaudiosourceconfiguration.node()));

      }

      signals_->getcompatibleaudiosourceconfigurations_->Emit(handle, localendpoint, latency, std::string(), getcompatibleaudiosourceconfigurations);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEMETADATACONFIGURATIONS:
    {
      auto getcompatiblemetadataconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleMetadataConfigurationsResponse']");
      if (!getcompatiblemetadataconfigurationsresponse.node())
      {
        SignalError(MEDIAOPERATION_GETCOMPATIBLEMETADATACONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetCompatibleMetadataConfigurationsResponse element not found"));
        break;
      }

      std::vector<MetadataConfiguration> getcompatiblemetadataconfigurations;
      for (const auto& compatiblemetadataconfiguration : getcompatiblemetadataconfigurationsresponse.node().select_nodes("*[local-name()='Configurations']"))
      {
        getcompatiblemetadataconfigurations.push_back(MetadataConfiguration(compatiblemetadataconfiguration.node()));

      }

      signals_->getcompatiblemetadataconfigurations_->Emit(handle, localendpoint, latency, std::string(), getcompatiblemetadataconfigurations);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEVIDEOANALYTICSCONFIGURATIONS:
    {
      auto getcompatiblevideoanalyticsconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleVideoAnalyticsConfigurationsResponse']");
      if (!getcompatiblevideoanalyticsconfigurationsresponse.node())
      {
        SignalError(MEDIAOPERATION_GETCOMPATIBLEVIDEOANALYTICSCONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetCompatibleVideoAnalyticsConfigurationsResponse element not found"));
        break;
      }

      std::vector<VideoAnalyticsConfiguration> getcompatiblevideoanalyticsconfigurations;
      for (const auto& compatiblevideoanalyticsconfiguration : getcompatiblevideoanalyticsconfigurationsresponse.node().select_nodes("*[local-name()='Configurations']"))
      {
        getcompatiblevideoanalyticsconfigurations.push_back(VideoAnalyticsConfiguration(compatiblevideoanalyticsconfiguration.node()));

      }

      signals_->getcompatiblevideoanalyticsconfigurations_->Emit(handle, localendpoint, latency, std::string(), getcompatiblevideoanalyticsconfigurations);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEVIDEOENCODERCONFIGURATIONS:
    {
      auto getcompatiblevideoencoderconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleVideoEncoderConfigurationsResponse']");
      if (!getcompatiblevideoencoderconfigurationsresponse.node())
      {
        SignalError(MEDIAOPERATION_GETCOMPATIBLEVIDEOENCODERCONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetCompatibleVideoEncoderConfigurationsResponse element not found"));
        break;
      }

      std::vector<VideoEncoderConfiguration> getcompatiblevideoencoderconfigurations;
      for (const auto& compatiblevideosourceconfiguration : getcompatiblevideoencoderconfigurationsresponse.node().select_nodes("*[local-name()='Configurations']"))
      {
        getcompatiblevideoencoderconfigurations.push_back(VideoEncoderConfiguration(compatiblevideosourceconfiguration.node()));

      }

      signals_->getcompatiblevideoencoderconfigurations_->Emit(handle, localendpoint, latency, std::string(), getcompatiblevideoencoderconfigurations);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEVIDEOSOURCECONFIGURATIONS:
    {
      auto getcompatiblevideosourceconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetCompatibleVideoSourceConfigurationsResponse']");
      if (!getcompatiblevideosourceconfigurationsresponse.node())
      {
        SignalError(MEDIAOPERATION_GETCOMPATIBLEVIDEOSOURCECONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetCompatibleVideoSourceConfigurationsResponse element not found"));
        break;
      }

      std::vector<VideoSourceConfiguration> getcompatiblevideosourceconfigurations;
      for (const auto& compatiblevideosourceconfiguration : getcompatiblevideosourceconfigurationsresponse.node().select_nodes("*[local-name()='Configurations']"))
      {
        getcompatiblevideosourceconfigurations.push_back(VideoSourceConfiguration(compatiblevideosourceconfiguration.node()));

      }

      signals_->getcompatiblevideosourceconfigurations_->Emit(handle, localendpoint, latency, std::string(), getcompatiblevideosourceconfigurations);
      break;
    }
    case MEDIAOPERATION_GETMETADATACONFIGURATION:
    {
      auto getmetadataconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetMetadataConfigurationResponse']");
      if (!getmetadataconfigurationresponse)
      {
        SignalError(MEDIAOPERATION_GETMETADATACONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetMetadataConfigurationResponse element not found"));
        break;
      }

      signals_->getmetadataconfiguration_->Emit(handle, localendpoint, latency, std::string(), GetClass<MetadataConfiguration>(getmetadataconfigurationresponse, "*[local-name()='Configuration']"));
      break;
    }
    case MEDIAOPERATION_GETMETADATACONFIGURATIONOPTIONS:
    {
      auto getmetadataconfigurationoptionsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetMetadataConfigurationOptionsResponse']");
      if (!getmetadataconfigurationoptionsresponse)
      {
        SignalError(MEDIAOPERATION_GETMETADATACONFIGURATIONOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetMetadataConfigurationOptionsResponse element not found"));
        break;
      }

      signals_->getmetadataconfigurationoptions_->Emit(handle, localendpoint, latency, std::string(), GetClass<MetadataConfigurationOptions>(getmetadataconfigurationoptionsresponse, "*[local-name()='Options']"));
      break;
    }
    case MEDIAOPERATION_GETMETADATACONFIGURATIONS:
    {
      auto getmetadataconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetMetadataConfigurationsResponse']");
      if (!getmetadataconfigurationsresponse.node())
      {
        SignalError(MEDIAOPERATION_GETPROFILES, handle, localendpoint, latency, std::string("/Envelope/Body/GetMetadataConfigurationsResponse element not found"));
        break;
      }

      std::vector<MetadataConfiguration> configurations;
      for (const auto& configuration : getmetadataconfigurationsresponse.node().select_nodes("*[local-name()='Configurations']"))
      {
        configurations.push_back(MetadataConfiguration(configuration.node()));

      }

      signals_->getmetadataconfigurations_->Emit(handle, localendpoint, latency, std::string(), configurations);
      break;
    }
    case MEDIAOPERATION_GETPROFILE:
    {
      auto getprofileresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetProfileResponse']");
      if (!getprofileresponse)
      {
        SignalError(MEDIAOPERATION_GETPROFILE, handle, localendpoint, latency, std::string("/Envelope/Body/GetProfileResponse element not found"));
        break;
      }
      
      signals_->getprofile_->Emit(handle, localendpoint, latency, std::string(), GetClass<Profile>(getprofileresponse, "*[local-name()='Profile']"));
      break;
    }
    case MEDIAOPERATION_GETPROFILES:
    {
      auto getprofilesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetProfilesResponse']");
      if (!getprofilesresponse.node())
      {
        SignalError(MEDIAOPERATION_GETPROFILES, handle, localendpoint, latency, std::string("/Envelope/Body/GetProfilesResponse element not found"));
        break;
      }

      std::vector<Profile> profiles;
      for (const auto& profile : getprofilesresponse.node().select_nodes("*[local-name()='Profiles']"))
      {
        profiles.push_back(Profile(profile.node()));

      }

      signals_->getprofiles_->Emit(handle, localendpoint, latency, std::string(), profiles);
      break;
    }
    case MEDIAOPERATION_GETSERVICECAPABILITIES:
    {
      auto getservicecapabilitiesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilitiesResponse']");
      if (!getservicecapabilitiesresponse)
      {
        SignalError(MEDIAOPERATION_GETSERVICECAPABILITIES, handle, localendpoint, latency, std::string("/Envelope/Body/GetServiceCapabilitiesResponse element not found"));
        break;
      }
      
      signals_->getservicecapabilities_->Emit(handle, localendpoint, latency, std::string(), GetClass<Capabilities>(getservicecapabilitiesresponse, "*[local-name()='Capabilities']"));
      break;
    }
    case MEDIAOPERATION_GETSNAPSHOTURI:
    {
      auto getsnapshoturiresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetSnapshotUriResponse']");
      if (!getsnapshoturiresponse)
      {
        SignalError(MEDIAOPERATION_GETSNAPSHOTURI, handle, localendpoint, latency, std::string("/Envelope/Body/GetSnapshotUriResponse element not found"));
        break;
      }
      
      signals_->getsnapshoturi_->Emit(handle, localendpoint, latency, std::string(), GetClass<MediaUri>(getsnapshoturiresponse, "*[local-name()='MediaUri']"));
      break;
    }
    case MEDIAOPERATION_GETSTREAMURI:
    {
      auto getstreamuriresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetStreamUriResponse']");
      if (!getstreamuriresponse)
      {
        SignalError(MEDIAOPERATION_GETSTREAMURI, handle, localendpoint, latency, std::string("/Envelope/Body/GetStreamUriResponse element not found"));
        break;
      }
      
      signals_->getstreamuri_->Emit(handle, localendpoint, latency, std::string(), GetClass<MediaUri>(getstreamuriresponse, "*[local-name()='MediaUri']"));
      break;
    }
    case MEDIAOPERATION_GETVIDEOANALYTICSCONFIGURATIONS:
    {
      auto getvideoanalyticsconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoAnalyticsConfigurationsResponse']");
      if (!getvideoanalyticsconfigurationsresponse.node())
      {
        SignalError(MEDIAOPERATION_GETVIDEOANALYTICSCONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetVideoAnalyticsConfigurationsResponse element not found"));
        break;
      }

      std::vector<VideoAnalyticsConfiguration> videoanalyticsconfigurations;
      for (const auto& videoanalyticsconfiguration : getvideoanalyticsconfigurationsresponse.node().select_nodes("*[local-name()='Configurations']"))
      {
        videoanalyticsconfigurations.push_back(VideoAnalyticsConfiguration(videoanalyticsconfiguration.node()));

      }

      signals_->getvideoanalyticsconfigurations_->Emit(handle, localendpoint, latency, std::string(), videoanalyticsconfigurations);
      break;
    }
    case MEDIAOPERATION_GETVIDEOENCODERCONFIGURATION:
    {
      auto getvideoencoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoEncoderConfigurationResponse']");
      if (!getvideoencoderconfigurationresponse)
      {
        SignalError(MEDIAOPERATION_GETVIDEOENCODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetVideoEncoderConfigurationResponse element not found"));
        break;
      }
      
      signals_->getvideoencoderconfiguration_->Emit(handle, localendpoint, latency, std::string(), GetClass<VideoEncoderConfiguration>(getvideoencoderconfigurationresponse, "*[local-name()='Configuration']"));
      break;
    }
    case MEDIAOPERATION_GETVIDEOENCODERCONFIGURATIONOPTIONS:
    {
      auto getvideoencoderconfigurationoptionsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoEncoderConfigurationOptionsResponse']");
      if (!getvideoencoderconfigurationoptionsresponse)
      {
        SignalError(MEDIAOPERATION_GETVIDEOENCODERCONFIGURATIONOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetVideoEncoderConfigurationOptionsResponse element not found"));
        break;
      }
      
      signals_->getvideoencoderconfigurationoptions_->Emit(handle, localendpoint, latency, std::string(), GetClass<VideoEncoderConfigurationOptions>(getvideoencoderconfigurationoptionsresponse, "*[local-name()='Options']"));
      break;
    }
    case MEDIAOPERATION_GETVIDEOENCODERCONFIGURATIONS:
    {
      auto getvideoencoderconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoEncoderConfigurationsResponse']");
      if (!getvideoencoderconfigurationsresponse.node())
      {
        SignalError(MEDIAOPERATION_GETVIDEOENCODERCONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetVideoEncoderConfigurationsResponse element not found"));
        break;
      }

      std::vector<VideoEncoderConfiguration> videoencoderconfigurations;
      for (const auto& videoencoderconfiguration : getvideoencoderconfigurationsresponse.node().select_nodes("*[local-name()='Configurations']"))
      {
        videoencoderconfigurations.push_back(VideoEncoderConfiguration(videoencoderconfiguration.node()));

      }
      
      signals_->getvideoencoderconfigurations_->Emit(handle, localendpoint, latency, std::string(), videoencoderconfigurations);
      break;
    }
    case MEDIAOPERATION_GETVIDEOSOURCECONFIGURATION:
    {
      auto getvideosourceconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoSourceConfigurationResponse']");
      if (!getvideosourceconfigurationresponse)
      {
        SignalError(MEDIAOPERATION_GETVIDEOSOURCECONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetVideoSourceConfigurationResponse element not found"));
        break;
      }
      
      signals_->getvideosourceconfiguration_->Emit(handle, localendpoint, latency, std::string(), GetClass<VideoSourceConfiguration>(getvideosourceconfigurationresponse, "*[local-name()='Configuration']"));
      break;
    }
    case MEDIAOPERATION_GETVIDEOSOURCECONFIGURATIONOPTIONS:
    {
      auto getvideosourceconfigurationoptionsoptionsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoSourceConfigurationOptionsResponse']");
      if (!getvideosourceconfigurationoptionsoptionsresponse)
      {
        SignalError(MEDIAOPERATION_GETVIDEOSOURCECONFIGURATIONOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetVideoSourceConfigurationOptionsResponse element not found"));
        break;
      }
      
      signals_->getvideosourceconfigurationoptions_->Emit(handle, localendpoint, latency, std::string(), GetClass<VideoSourceConfigurationOptions>(getvideosourceconfigurationoptionsoptionsresponse, "*[local-name()='Options']"));
      break;
    }
    case MEDIAOPERATION_GETVIDEOSOURCECONFIGURATIONS:
    {
      auto getvideosourceconfigurationsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoSourceConfigurationsResponse']");
      if (!getvideosourceconfigurationsresponse.node())
      {
        SignalError(MEDIAOPERATION_GETVIDEOSOURCECONFIGURATIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetVideoSourceConfigurationsResponse element not found"));
        break;
      }

      std::vector<VideoSourceConfiguration> getvideosourceconfigurations;
      for (const auto& videosourceconfiguration : getvideosourceconfigurationsresponse.node().select_nodes("*[local-name()='Configurations']"))
      {
        getvideosourceconfigurations.push_back(VideoSourceConfiguration(videosourceconfiguration.node()));

      }

      signals_->getvideosourceconfigurations_->Emit(handle, localendpoint, latency, std::string(), getvideosourceconfigurations);
      break;
    }
    case MEDIAOPERATION_GETVIDEOSOURCEMODES:
    {
      auto videosourcemodesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoSourceModesResponse']");
      if (!videosourcemodesresponse)
      {
        SignalError(MEDIAOPERATION_GETVIDEOSOURCEMODES, handle, localendpoint, latency, std::string("/Envelope/Body/GetVideoSourceModesResponse element not found"));
        break;
      }
      
      std::vector<VideoSourceMode> getvideosourcemodes;
      for (const auto& videosourcemoderesponse : videosourcemodesresponse.node().select_nodes("*[local-name()='VideoSourceModes']"))
      {
        getvideosourcemodes.push_back(VideoSourceMode(videosourcemoderesponse.node()));

      }

      signals_->getvideosourcemodes_->Emit(handle, localendpoint, latency, std::string(), getvideosourcemodes);
      break;
    }
    case MEDIAOPERATION_GETVIDEOSOURCES:
    {
      auto getvideosourcesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetVideoSourcesResponse']");
      if (!getvideosourcesresponse.node())
      {
        SignalError(MEDIAOPERATION_GETVIDEOSOURCES, handle, localendpoint, latency, std::string("/Envelope/Body/GetVideoSourcesResponse element not found"));
        break;
      }

      std::vector<VideoSource> videosources;
      for (const auto& videosource : getvideosourcesresponse.node().select_nodes("*[local-name()='VideoSources']"))
      {
        videosources.push_back(VideoSource(videosource.node()));

      }

      signals_->getvideosources_->Emit(handle, localendpoint, latency, std::string(), videosources);
      break;
    }

    case MEDIAOPERATION_REMOVEAUDIODECODERCONFIGURATION:
    {
      auto removeaudiodecoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveAudioDecoderConfigurationResponse']");
      if (!removeaudiodecoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_REMOVEAUDIODECODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/RemoveAudioDecoderConfigurationResponse element not found"));
        break;
      }

      signals_->removeaudiodecoderconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_REMOVEAUDIOENCODERCONFIGURATION:
    {
      auto removeaudioencoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveAudioEncoderConfigurationResponse']");
      if (!removeaudioencoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_REMOVEAUDIOENCODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/RemoveAudioEncoderConfigurationResponse element not found"));
        break;
      }

      signals_->removeaudioencoderconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_REMOVEAUDIOOUTPUTCONFIGURATION:
    {
      auto removeaudiooutputconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveAudioOutputConfigurationResponse']");
      if (!removeaudiooutputconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_REMOVEAUDIOOUTPUTCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/RemoveAudioOutputConfigurationResponse element not found"));
        break;
      }

      signals_->removeaudiooutputconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_REMOVEAUDIOSOURCECONFIGURATION:
    {
      auto removeaudiosourceconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveAudioSourceConfigurationResponse']");
      if (!removeaudiosourceconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_REMOVEAUDIOSOURCECONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/RemoveAudioSourceConfigurationResponse element not found"));
        break;
      }

      signals_->removeaudiosourceconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_REMOVEMETADATACONFIGURATION:
    {
      auto removemetadataconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveMetadataConfigurationResponse']");
      if (!removemetadataconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_REMOVEMETADATACONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/RemoveMetadataConfigurationResponse element not found"));
        break;
      }

      signals_->removemetadataconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_REMOVEPTZCONFIGURATION:
    {
      auto removeptzconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemovePTZConfigurationResponse']");
      if (!removeptzconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_REMOVEPTZCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/RemovePTZConfigurationResponse element not found"));
        break;
      }

      signals_->removeptzconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_REMOVEVIDEOANALYTICSCONFIGURATION:
    {
      auto removevideoanalyticsconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveVideoAnalyticsConfigurationResponse']");
      if (!removevideoanalyticsconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_REMOVEVIDEOANALYTICSCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/RemoveVideoAnalyticsConfigurationResponse element not found"));
        break;
      }

      signals_->removevideoanalyticsconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_REMOVEVIDEOENCODERCONFIGURATION:
    {
      auto removevideoencoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveVideoEncoderConfigurationResponse']");
      if (!removevideoencoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_REMOVEVIDEOENCODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/RemoveVideoEncoderConfigurationResponse element not found"));
        break;
      }

      signals_->removevideoencoderconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_REMOVEVIDEOSOURCECONFIGURATION:
    {
      auto removevideosourceconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='RemoveVideoSourceConfigurationResponse']");
      if (!removevideosourceconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_REMOVEVIDEOSOURCECONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/RemoveVideoSourceConfigurationResponse element not found"));
        break;
      }

      signals_->removevideosourceconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }

    case MEDIAOPERATION_SETAUDIODECODERCONFIGURATION:
    {
      auto setaudiodecoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAudioDecoderConfigurationResponse']");
      if (!setaudiodecoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_SETAUDIODECODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetAudioDecoderConfigurationResponse element not found"));
        break;
      }

      signals_->setaudiodecoderconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_SETAUDIOENCODERCONFIGURATION:
    {
      auto setaudioencoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAudioEncoderConfigurationResponse']");
      if (!setaudioencoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_SETAUDIOENCODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetAudioEncoderConfigurationResponse element not found"));
        break;
      }

      signals_->setaudioencoderconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_SETAUDIOOUTPUTCONFIGURATION:
    {
      auto setaudiooutputconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAudioOutputConfigurationResponse']");
      if (!setaudiooutputconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_SETAUDIOOUTPUTCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetAudioOutputConfigurationResponse element not found"));
        break;
      }

      signals_->setaudiooutputconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_SETAUDIOSOURCECONFIGURATION:
    {
      auto setaudiosourceconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetAudioSourceConfigurationResponse']");
      if (!setaudiosourceconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_SETAUDIOSOURCECONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetAudioSourceConfigurationResponse element not found"));
        break;
      }

      signals_->setaudiosourceconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_SETMETADATACONFIGURATION:
    {
      auto setmetadataconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetMetadataConfigurationResponse']");
      if (!setmetadataconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_SETMETADATACONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetMetadataConfigurationResponse element not found"));
        break;
      }

      signals_->setmetadataconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_SETSYNCHRONIZATIONPOINT:
    {
      auto setsynchronizationpointresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetSynchronizationPointResponse']");
      if (!setsynchronizationpointresponse)
      {
        SignalError(MEDIAOPERATION_SETSYNCHRONIZATIONPOINT, handle, localendpoint, latency, std::string("/Envelope/Body/SetSynchronizationPointResponse element not found"));
        break;
      }

      signals_->setsynchronizationpoint_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_SETVIDEOENCODERCONFIGURATION:
    {
      auto setvideoencoderconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetVideoEncoderConfigurationResponse']");
      if (!setvideoencoderconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_SETVIDEOENCODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetVideoEncoderConfigurationResponse element not found"));
        break;
      }

      signals_->setvideoencoderconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_SETVIDEOSOURCECONFIGURATION:
    {
      auto setvideosourceconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetVideoSourceConfigurationResponse']");
      if (!setvideosourceconfigurationresponse.node())
      {
        SignalError(MEDIAOPERATION_SETVIDEOENCODERCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetVideoSourceConfigurationResponse element not found"));
        break;
      }

      signals_->setvideosourceconfiguration_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_STARTMULTICASTSTREAMING:
    {
      auto startmulticaststreamingresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='StartMulticastStreamingResponse']");
      if (!startmulticaststreamingresponse)
      {
        SignalError(MEDIAOPERATION_STARTMULTICASTSTREAMING, handle, localendpoint, latency, std::string("/Envelope/Body/StartMulticastStreamingResponse element not found"));
        break;
      }
      
      signals_->startmulticaststreaming_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case MEDIAOPERATION_STOPMULTICASTSTREAMING:
    {
      auto stopmulticaststreamingresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='StopMulticastStreamingResponse']");
      if (!stopmulticaststreamingresponse)
      {
        SignalError(MEDIAOPERATION_STOPMULTICASTSTREAMING, handle, localendpoint, latency, std::string("/Envelope/Body/StopMulticastStreamingResponse element not found"));
        break;
      }
      
      signals_->stopmulticaststreaming_->Emit(handle, localendpoint, latency, std::string());
      break;
    }
    default:
    {
      assert(false);
      break;
    }
  }
}

void MediaClient::SignalError(MEDIAOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message)
{
  switch (operation)
  {
    case MEDIAOPERATION_ADDAUDIODECODERCONFIGURATION:
    {
      signals_->addaudiodecoderconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_ADDAUDIOENCODERCONFIGURATION:
    {
      signals_->addaudioencoderconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_ADDAUDIOOUTPUTCONFIGURATION:
    {
      signals_->addaudiooutputconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_ADDAUDIOSOURCECONFIGURATION:
    {
      signals_->addaudiosourceconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_ADDMETADATACONFIGURATION:
    {
      signals_->addmetadataconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_ADDPTZCONFIGURATION:
    {
      signals_->addptzconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_ADDVIDEOANALYTICSCONFIGURATION:
    {
      signals_->addvideoanalyticsconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_ADDVIDEOENCODERCONFIGURATION:
    {
      signals_->addvideoencoderconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_ADDVIDEOSOURCECONFIGURATION:
    {
      signals_->addvideosourceconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_CREATEPROFILE:
    {
      signals_->createprofile_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_DELETEPROFILE:
    {
      signals_->deleteprofile_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIODECODERCONFIGURATION:
    {
      signals_->getaudiodecoderconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIODECODERCONFIGURATIONOPTIONS:
    {
      signals_->getaudiodecoderconfigurationoptions_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIODECODERCONFIGURATIONS:
    {
      signals_->getaudiodecoderconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIOENCODERCONFIGURATION:
    {
      signals_->getaudioencoderconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIOENCODERCONFIGURATIONOPTIONS:
    {
      signals_->getaudioencoderconfigurationoptions_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIOENCODERCONFIGURATIONS:
    {
      signals_->getaudioencoderconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATION:
    {
      signals_->getaudiooutputconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATIONOPTIONS:
    {
      signals_->getaudiooutputconfigurationoptions_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATIONS:
    {
      signals_->getaudiooutputconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIOSOURCECONFIGURATION:
    {
      signals_->getaudiosourceconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIOSOURCECONFIGURATIONOPTIONS:
    {
      signals_->getaudiosourceconfigurationoptions_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIOSOURCECONFIGURATIONS:
    {
      signals_->getaudiosourceconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETAUDIOSOURCES:
    {
      signals_->getaudiosources_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEAUDIODECODERCONFIGURATIONS:
    {
      signals_->getcompatibleaudiodecoderconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEAUDIOENCODERCONFIGURATIONS:
    {
      signals_->getcompatibleaudioencoderconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEAUDIOOUTPUTCONFIGURATIONS:
    {
      signals_->getcompatibleaudiooutputconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEAUDIOSOURCECONFIGURATIONS:
    {
      signals_->getcompatibleaudiosourceconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEMETADATACONFIGURATIONS:
    {
      signals_->getcompatiblemetadataconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEVIDEOANALYTICSCONFIGURATIONS:
    {
      signals_->getcompatiblevideoanalyticsconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEVIDEOENCODERCONFIGURATIONS:
    {
      signals_->getcompatiblevideoencoderconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETCOMPATIBLEVIDEOSOURCECONFIGURATIONS:
    {
      signals_->getcompatiblevideosourceconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETMETADATACONFIGURATION:
    {
      signals_->getmetadataconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETMETADATACONFIGURATIONOPTIONS:
    {
      signals_->getmetadataconfigurationoptions_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETMETADATACONFIGURATIONS:
    {
      signals_->getmetadataconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETPROFILE:
    {
      signals_->getprofile_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETPROFILES:
    {
      signals_->getprofiles_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETSERVICECAPABILITIES:
    {
      signals_->getservicecapabilities_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETSNAPSHOTURI:
    {
      signals_->getsnapshoturi_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETSTREAMURI:
    {
      signals_->getstreamuri_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETVIDEOANALYTICSCONFIGURATIONS:
    {
      signals_->getvideoanalyticsconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETVIDEOENCODERCONFIGURATION:
    {
      signals_->getvideoencoderconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETVIDEOENCODERCONFIGURATIONOPTIONS:
    {
      signals_->getvideoencoderconfigurationoptions_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETVIDEOENCODERCONFIGURATIONS:
    {
      signals_->getvideoencoderconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETVIDEOSOURCECONFIGURATION:
    {
      signals_->getvideosourceconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETVIDEOSOURCECONFIGURATIONOPTIONS:
    {
      signals_->getvideosourceconfigurationoptions_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETVIDEOSOURCECONFIGURATIONS:
    {
      signals_->getvideosourceconfigurations_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETVIDEOSOURCEMODES:
    {
      signals_->getvideosourcemodes_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_GETVIDEOSOURCES:
    {
      signals_->getvideosources_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_REMOVEAUDIODECODERCONFIGURATION:
    {
      signals_->removeaudiodecoderconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_REMOVEAUDIOENCODERCONFIGURATION:
    {
      signals_->removeaudioencoderconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_REMOVEAUDIOOUTPUTCONFIGURATION:
    {
      signals_->removeaudiooutputconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_REMOVEAUDIOSOURCECONFIGURATION:
    {
      signals_->removeaudiosourceconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_REMOVEMETADATACONFIGURATION:
    {
      signals_->removemetadataconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_REMOVEPTZCONFIGURATION:
    {
      signals_->removeptzconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_REMOVEVIDEOANALYTICSCONFIGURATION:
    {
      signals_->removevideoanalyticsconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_REMOVEVIDEOENCODERCONFIGURATION:
    {
      signals_->removevideoencoderconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_REMOVEVIDEOSOURCECONFIGURATION:
    {
      signals_->removevideosourceconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_SETAUDIODECODERCONFIGURATION:
    {
      signals_->setaudiodecoderconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_SETAUDIOENCODERCONFIGURATION:
    {
      signals_->setaudioencoderconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_SETAUDIOOUTPUTCONFIGURATION:
    {
      signals_->setaudiooutputconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_SETAUDIOSOURCECONFIGURATION:
    {
      signals_->setaudiosourceconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_SETMETADATACONFIGURATION:
    {
      signals_->setmetadataconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_SETSYNCHRONIZATIONPOINT:
    {
      signals_->setsynchronizationpoint_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_SETVIDEOENCODERCONFIGURATION:
    {
      signals_->setvideoencoderconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_SETVIDEOSOURCECONFIGURATION:
    {
      signals_->setvideosourceconfiguration_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_STARTMULTICASTSTREAMING:
    {
      signals_->startmulticaststreaming_->Emit(handle, localendpoint, latency, message);
      break;
    }
    case MEDIAOPERATION_STOPMULTICASTSTREAMING:
    {
      signals_->stopmulticaststreaming_->Emit(handle, localendpoint, latency, message);
      break;
    }
    default:
    {
       assert(false);
       break;
    }
  }
}

std::string MediaClient::AddAudioDecoderConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken)
{
  return std::string("<s:Body><media:AddAudioDecoderConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:AddAudioDecoderConfiguration></s:Body>");
}

std::string MediaClient::AddAudioEncoderConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken)
{
  return std::string("<s:Body><media:AddAudioEncoderConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:AddAudioEncoderConfiguration></s:Body>");
}

std::string MediaClient::AddAudioOutputConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken)
{
  return std::string("<s:Body><media:AddAudioOutputConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:AddAudioOutputConfiguration></s:Body>");
}

std::string MediaClient::AddAudioSourceConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken)
{
  return std::string("<s:Body><media:AddAudioSourceConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:AddAudioSourceConfiguration></s:Body>");
}

std::string MediaClient::AddMetadataConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken)
{
  return std::string("<s:Body><media:AddMetadataConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:AddMetadataConfiguration></s:Body>");
}

std::string MediaClient::AddPTZConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken)
{
  return std::string("<s:Body><media:AddPTZConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:AddPTZConfiguration></s:Body>");
}

std::string MediaClient::AddVideoAnalyticsConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken)
{
  return std::string("<s:Body><media:AddVideoAnalyticsConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:AddVideoAnalyticsConfiguration></s:Body>");
}

std::string MediaClient::AddVideoEncoderConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken)
{
  return std::string("<s:Body><media:AddVideoEncoderConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:AddVideoEncoderConfiguration></s:Body>");
}

std::string MediaClient::AddVideoSourceConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken)
{
  return std::string("<s:Body><media:AddVideoSourceConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:AddVideoSourceConfiguration></s:Body>");
}

std::string MediaClient::CreateProfileBody(const std::string& name, const boost::optional<std::string>& token)
{
  return std::string("<s:Body><media:CreateProfile><media:Name>" + name + "</media:Name>" + (token.is_initialized() ? (std::string("<media:Token>") + *token + std::string("</media:Token>")) : std::string()) + "</media:CreateProfile></s:Body>");
}

std::string MediaClient::DeleteProfileBody(const std::string& token)
{
  return std::string("<s:Body><media:DeleteProfile><media:ProfileToken>" + token + "</media:ProfileToken></media:DeleteProfile></s:Body>");
}

std::string MediaClient::GetAudioDecoderConfigurationBody(const std::string& configurationtoken)
{
  return std::string("<s:Body><media:GetAudioDecoderConfiguration><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:GetAudioDecoderConfiguration></s:Body>");
}

std::string MediaClient::GetAudioDecoderConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return std::string("<s:Body><media:GetAudioDecoderConfigurationOptions>" + ToXml("media:ConfigurationToken", configurationtoken) + ToXml("media:ProfileToken", profiletoken) + "</media:GetAudioDecoderConfigurationOptions></s:Body>");
}

std::string MediaClient::GetAudioDecoderConfigurationsBody()
{
  return std::string("<s:Body><media:GetAudioDecoderConfigurations/></s:Body>");
}

std::string MediaClient::GetAudioEncoderConfigurationBody(const std::string& configurationtoken)
{
  return std::string("<s:Body><media:GetAudioEncoderConfiguration><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:GetAudioEncoderConfiguration></s:Body>");
}

std::string MediaClient::GetAudioEncoderConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return std::string("<s:Body><media:GetAudioEncoderConfigurationOptions>" + ToXml("media:ConfigurationToken", configurationtoken) + ToXml("media:ProfileToken", profiletoken) + "</media:GetAudioEncoderConfigurationOptions></s:Body>");
}

std::string MediaClient::GetAudioEncoderConfigurationsBody()
{
  return std::string("<s:Body><media:GetAudioEncoderConfigurations/></s:Body>");
}

std::string MediaClient::GetAudioOutputConfigurationBody(const std::string& configurationtoken)
{
  return std::string("<s:Body><media:GetAudioOutputConfiguration><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:GetAudioOutputConfiguration></s:Body>");
}

std::string MediaClient::GetAudioOutputConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return std::string("<s:Body><media:GetAudioOutputConfigurationOptions>" + ToXml("media:ConfigurationToken", configurationtoken) + ToXml("media:ProfileToken", profiletoken) + "</media:GetAudioOutputConfigurationOptions></s:Body>");
}

std::string MediaClient::GetAudioOutputConfigurationsBody()
{
  return std::string("<s:Body><media:GetAudioOutputConfigurations/></s:Body>");
}

std::string MediaClient::GetAudioSourceConfigurationBody(const std::string& configurationtoken)
{
  return std::string("<s:Body><media:GetAudioSourceConfiguration><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:GetAudioSourceConfiguration></s:Body>");
}

std::string MediaClient::GetAudioSourceConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return std::string("<s:Body><media:GetAudioSourceConfigurationOptions>" + ToXml("media:ConfigurationToken", configurationtoken) + ToXml("media:ProfileToken", profiletoken) + "</media:GetAudioSourceConfigurationOptions></s:Body>");
}

std::string MediaClient::GetAudioSourceConfigurationsBody()
{
  return std::string("<s:Body><media:GetAudioSourceConfigurations/></s:Body>");
}

std::string MediaClient::GetAudioSourcesBody()
{
  return std::string("<s:Body><media:GetAudioSources/></s:Body>");
}

std::string MediaClient::GetCompatibleAudioDecoderConfigurationsBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:GetCompatibleAudioDecoderConfigurations><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:GetCompatibleAudioDecoderConfigurations></s:Body>");
}

std::string MediaClient::GetCompatibleAudioEncoderConfigurationsBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:GetCompatibleAudioEncoderConfigurations><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:GetCompatibleAudioEncoderConfigurations></s:Body>");
}

std::string MediaClient::GetCompatibleAudioOutputConfigurationsBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:GetCompatibleAudioOutputConfigurations><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:GetCompatibleAudioOutputConfigurations></s:Body>");
}

std::string MediaClient::GetCompatibleAudioSourceConfigurationsBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:GetCompatibleAudioSourceConfigurations><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:GetCompatibleAudioSourceConfigurations></s:Body>");
}

std::string MediaClient::GetCompatibleMetadataConfigurationsBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:GetCompatibleMetadataConfigurations><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:GetCompatibleMetadataConfigurations></s:Body>");
}

std::string MediaClient::GetCompatibleVideoAnalyticsConfigurationsBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:GetCompatibleVideoAnalyticsConfigurations><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:GetCompatibleVideoAnalyticsConfigurations></s:Body>");
}

std::string MediaClient::GetCompatibleVideoEncoderConfigurationsBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:GetCompatibleVideoEncoderConfigurations><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:GetCompatibleVideoEncoderConfigurations></s:Body>");
}

std::string MediaClient::GetCompatibleVideoSourceConfigurationsBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:GetCompatibleVideoSourceConfigurations><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:GetCompatibleVideoSourceConfigurations></s:Body>");
}

std::string MediaClient::GetMetadataConfigurationBody(const std::string& configurationtoken)
{
  return std::string("<s:Body><media:GetMetadataConfiguration><media:ConfigurationToken>" + configurationtoken + "</media:ConfigurationToken></media:GetMetadataConfiguration></s:Body>");
}

std::string MediaClient::GetMetadataConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return std::string("<s:Body><media:GetMetadataConfigurationOptions>" + ToXml("media:ConfigurationToken", configurationtoken) + ToXml("media:ProfileToken", profiletoken) + "</media:GetMetadataConfigurationOptions></s:Body>");
}

std::string MediaClient::GetMetadataConfigurationsBody()
{
  return std::string("<s:Body><media:GetMetadataConfigurations/></s:Body>");
}

std::string MediaClient::GetProfileBody(const std::string& token)
{
  return std::string("<s:Body><media:GetProfile><media:ProfileToken>" + token + "</media:ProfileToken></media:GetProfile></s:Body>");
}

std::string MediaClient::GetProfilesBody()
{
  return std::string("<s:Body><media:GetProfiles/></s:Body>");
}

std::string MediaClient::GetServiceCapabilitiesBody()
{
  return std::string("<s:Body><media:GetServiceCapabilities/></s:Body>");
}

std::string MediaClient::GetSnapshotUriBody(const std::string& token)
{
  return std::string("<s:Body><media:GetSnapshotUri><media:ProfileToken>" + token + "</media:ProfileToken></media:GetSnapshotUri></s:Body>");
}

std::string MediaClient::GetStreamUriBody(const StreamSetup& streamsetup, const std::string& token)
{
  return std::string("<s:Body><media:GetStreamUri>" + streamsetup.ToXml("media:StreamSetup") + "<media:ProfileToken>" + token + "</media:ProfileToken></media:GetStreamUri></s:Body>");
}

std::string MediaClient::GetVideoAnalyticsConfigurationsBody()
{
  return std::string("<s:Body><media:GetVideoAnalyticsConfigurations/></s:Body>");
}

std::string MediaClient::GetVideoEncoderConfigurationBody(const std::string& token)
{
  return std::string("<s:Body><media:GetVideoEncoderConfiguration><media:ConfigurationToken>" + token + "</media:ConfigurationToken></media:GetVideoEncoderConfiguration></s:Body>");
}

std::string MediaClient::GetVideoEncoderConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return std::string("<s:Body><media:GetVideoEncoderConfigurationOptions>" + ToXml("media:ConfigurationToken", configurationtoken) + ToXml("media:ProfileToken", profiletoken) + "</media:GetVideoEncoderConfigurationOptions></s:Body>");
}

std::string MediaClient::GetVideoEncoderConfigurationsBody()
{
  return std::string("<s:Body><media:GetVideoEncoderConfigurations/></s:Body>");
}

std::string MediaClient::GetVideoSourceConfigurationBody(const std::string& token)
{
  return std::string("<s:Body><media:GetVideoSourceConfiguration><media:ConfigurationToken>" + token + "</media:ConfigurationToken></media:GetVideoSourceConfiguration></s:Body>");
}

std::string MediaClient::GetVideoSourceConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken)
{
  return std::string("<s:Body><media:GetVideoSourceConfigurationOptions>" + ToXml("media:ConfigurationToken", configurationtoken) + ToXml("media:ProfileToken", profiletoken) + "</media:GetVideoSourceConfigurationOptions></s:Body>");
}

std::string MediaClient::GetVideoSourceConfigurationsBody()
{
  return std::string("<s:Body><media:GetVideoSourceConfigurations/></s:Body>");
}

std::string MediaClient::GetVideoSourceModesBody(const std::string& token)
{
  return std::string("<s:Body><media:GetVideoSourceModes><media:VideoSourceToken>" + token + "</media:VideoSourceToken></media:GetVideoSourceModes></s:Body>");
}

std::string MediaClient::GetVideoSourcesBody()
{
  return std::string("<s:Body><media:GetVideoSources/></s:Body>");
}

std::string MediaClient::RemoveAudioDecoderConfigurationBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:RemoveAudioDecoderConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:RemoveAudioDecoderConfiguration></s:Body>");
}

std::string MediaClient::RemoveAudioEncoderConfigurationBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:RemoveAudioEncoderConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:RemoveAudioEncoderConfiguration></s:Body>");
}

std::string MediaClient::RemoveAudioOutputConfigurationBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:RemoveAudioOutputConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:RemoveAudioOutputConfiguration></s:Body>");
}

std::string MediaClient::RemoveAudioSourceConfigurationBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:RemoveAudioSourceConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:RemoveAudioSourceConfiguration></s:Body>");
}

std::string MediaClient::RemoveMetadataConfigurationBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:RemoveMetadataConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:RemoveMetadataConfiguration></s:Body>");
}

std::string MediaClient::RemovePTZConfigurationBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:RemovePTZConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:RemovePTZConfiguration></s:Body>");
}

std::string MediaClient::RemoveVideoAnalyticsConfigurationBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:RemoveVideoAnalyticsConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:RemoveVideoAnalyticsConfiguration></s:Body>");
}

std::string MediaClient::RemoveVideoEncoderConfigurationBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:RemoveVideoEncoderConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:RemoveVideoEncoderConfiguration></s:Body>");
}

std::string MediaClient::RemoveVideoSourceConfigurationBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:RemoveVideoSourceConfiguration><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:RemoveVideoSourceConfiguration></s:Body>");
}

std::string MediaClient::SetAudioDecoderConfigurationBody(const AudioDecoderConfiguration& configuration)
{
  return std::string("<s:Body><media:SetAudioDecoderConfiguration>" + configuration.ToXml("media:Configuration") + "<media:ForcePersistence>true</media:ForcePersistence></media:SetAudioDecoderConfiguration></s:Body>");
}

std::string MediaClient::SetAudioEncoderConfigurationBody(const AudioEncoderConfiguration& configuration)
{
  return std::string("<s:Body><media:SetAudioEncoderConfiguration>" + configuration.ToXml("media:Configuration") + "<media:ForcePersistence>true</media:ForcePersistence></media:SetAudioEncoderConfiguration></s:Body>");
}

std::string MediaClient::SetAudioOutputConfigurationBody(const AudioOutputConfiguration& configuration)
{
  return std::string("<s:Body><media:SetAudioOutputConfiguration>" + configuration.ToXml("media:Configuration") + "<media:ForcePersistence>true</media:ForcePersistence></media:SetAudioOutputConfiguration></s:Body>");
}

std::string MediaClient::SetAudioSourceConfigurationBody(const AudioSourceConfiguration& configuration)
{
  return std::string("<s:Body><media:SetAudioSourceConfiguration>" + configuration.ToXml("media:Configuration") + "<media:ForcePersistence>true</media:ForcePersistence></media:SetAudioSourceConfiguration></s:Body>");
}

std::string MediaClient::SetMetadataConfigurationBody(const MetadataConfiguration& configuration)
{
  return std::string("<s:Body><media:SetMetadataConfiguration>" + configuration.ToXml("media:Configuration") + "<media:ForcePersistence>true</media:ForcePersistence></media:SetMetadataConfiguration></s:Body>");
}

std::string MediaClient::SetSynchronizationPointBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:SetSynchronizationPoint><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:SetSynchronizationPoint></s:Body>");
}

std::string MediaClient::SetVideoEncoderConfigurationBody(const VideoEncoderConfiguration& videoencoderconfiguration)
{
  return std::string("<s:Body><media:SetVideoEncoderConfiguration>" + videoencoderconfiguration.ToXml("media:Configuration") + "<media:ForcePersistence>true</media:ForcePersistence></media:SetVideoEncoderConfiguration></s:Body>");
}

std::string MediaClient::SetVideoSourceConfigurationBody(const VideoSourceConfiguration& videosourceconfiguration)
{
  return std::string("<s:Body><media:SetVideoSourceConfiguration>" + videosourceconfiguration.ToXml("media:Configuration") + "<media:ForcePersistence>true</media:ForcePersistence></media:SetVideoSourceConfiguration></s:Body>");
}

std::string MediaClient::StartMulticastStreamingBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:StartMulticastStreaming><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:StartMulticastStreaming></s:Body>");
}

std::string MediaClient::StopMulticastStreamingBody(const std::string& profiletoken)
{
  return std::string("<s:Body><media:StopMulticastStreaming><media:ProfileToken>" + profiletoken + "</media:ProfileToken></media:StopMulticastStreaming></s:Body>");
}

}

}
