// mediaclient.hpp
//

#ifndef ID9A19VMFNEY3D9MQQCOYCTWX8WRL21NBT
#define ID9A19VMFNEY3D9MQQCOYCTWX8WRL21NBT

///// Includes /////

#include <curl/curl.h>
#include <onviftypes/onviftypes.hpp>

#include "client.hpp"
#include "response.hpp"
#include "signal.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Enumerations /////

enum MEDIAOPERATION
{
  MEDIAOPERATION_ADDAUDIODECODERCONFIGURATION,
  MEDIAOPERATION_ADDAUDIOENCODERCONFIGURATION,
  MEDIAOPERATION_ADDAUDIOOUTPUTCONFIGURATION,
  MEDIAOPERATION_ADDAUDIOSOURCECONFIGURATION,
  MEDIAOPERATION_ADDMETADATACONFIGURATION,
  MEDIAOPERATION_ADDPTZCONFIGURATION,
  MEDIAOPERATION_ADDVIDEOANALYTICSCONFIGURATION,
  MEDIAOPERATION_ADDVIDEOENCODERCONFIGURATION,
  MEDIAOPERATION_ADDVIDEOSOURCECONFIGURATION,
  MEDIAOPERATION_CREATEPROFILE,
  MEDIAOPERATION_DELETEPROFILE,
  MEDIAOPERATION_GETAUDIODECODERCONFIGURATION,
  MEDIAOPERATION_GETAUDIODECODERCONFIGURATIONOPTIONS,
  MEDIAOPERATION_GETAUDIODECODERCONFIGURATIONS,
  MEDIAOPERATION_GETAUDIOENCODERCONFIGURATION,
  MEDIAOPERATION_GETAUDIOENCODERCONFIGURATIONOPTIONS,
  MEDIAOPERATION_GETAUDIOENCODERCONFIGURATIONS,
  MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATION,
  MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATIONOPTIONS,
  MEDIAOPERATION_GETAUDIOOUTPUTCONFIGURATIONS,
  MEDIAOPERATION_GETAUDIOSOURCECONFIGURATION,
  MEDIAOPERATION_GETAUDIOSOURCECONFIGURATIONOPTIONS,
  MEDIAOPERATION_GETAUDIOSOURCECONFIGURATIONS,
  MEDIAOPERATION_GETAUDIOSOURCES,
  MEDIAOPERATION_GETCOMPATIBLEAUDIODECODERCONFIGURATIONS,
  MEDIAOPERATION_GETCOMPATIBLEAUDIOENCODERCONFIGURATIONS,
  MEDIAOPERATION_GETCOMPATIBLEAUDIOOUTPUTCONFIGURATIONS,
  MEDIAOPERATION_GETCOMPATIBLEAUDIOSOURCECONFIGURATIONS,
  MEDIAOPERATION_GETCOMPATIBLEMETADATACONFIGURATIONS,
  MEDIAOPERATION_GETCOMPATIBLEVIDEOANALYTICSCONFIGURATIONS,
  MEDIAOPERATION_GETCOMPATIBLEVIDEOENCODERCONFIGURATIONS,
  MEDIAOPERATION_GETCOMPATIBLEVIDEOSOURCECONFIGURATIONS,
  MEDIAOPERATION_GETMETADATACONFIGURATION,
  MEDIAOPERATION_GETMETADATACONFIGURATIONOPTIONS,
  MEDIAOPERATION_GETMETADATACONFIGURATIONS,
  MEDIAOPERATION_GETPROFILE,
  MEDIAOPERATION_GETPROFILES,
  MEDIAOPERATION_GETSERVICECAPABILITIES,
  MEDIAOPERATION_GETSNAPSHOTURI,
  MEDIAOPERATION_GETSTREAMURI,
  MEDIAOPERATION_GETVIDEOANALYTICSCONFIGURATIONS,
  MEDIAOPERATION_GETVIDEOENCODERCONFIGURATION,
  MEDIAOPERATION_GETVIDEOENCODERCONFIGURATIONOPTIONS,
  MEDIAOPERATION_GETVIDEOENCODERCONFIGURATIONS,
  MEDIAOPERATION_GETVIDEOSOURCECONFIGURATION,
  MEDIAOPERATION_GETVIDEOSOURCECONFIGURATIONOPTIONS,
  MEDIAOPERATION_GETVIDEOSOURCECONFIGURATIONS,
  MEDIAOPERATION_GETVIDEOSOURCEMODES,
  MEDIAOPERATION_GETVIDEOSOURCES,
  MEDIAOPERATION_REMOVEAUDIODECODERCONFIGURATION,
  MEDIAOPERATION_REMOVEAUDIOENCODERCONFIGURATION,
  MEDIAOPERATION_REMOVEAUDIOOUTPUTCONFIGURATION,
  MEDIAOPERATION_REMOVEAUDIOSOURCECONFIGURATION,
  MEDIAOPERATION_REMOVEMETADATACONFIGURATION,
  MEDIAOPERATION_REMOVEPTZCONFIGURATION,
  MEDIAOPERATION_REMOVEVIDEOANALYTICSCONFIGURATION,
  MEDIAOPERATION_REMOVEVIDEOENCODERCONFIGURATION,
  MEDIAOPERATION_REMOVEVIDEOSOURCECONFIGURATION,
  MEDIAOPERATION_SETAUDIODECODERCONFIGURATION,
  MEDIAOPERATION_SETAUDIOENCODERCONFIGURATION,
  MEDIAOPERATION_SETAUDIOOUTPUTCONFIGURATION,
  MEDIAOPERATION_SETAUDIOSOURCECONFIGURATION,
  MEDIAOPERATION_SETMETADATACONFIGURATION,
  MEDIAOPERATION_SETSYNCHRONIZATIONPOINT,
  MEDIAOPERATION_SETVIDEOENCODERCONFIGURATION,
  MEDIAOPERATION_SETVIDEOSOURCECONFIGURATION,
  MEDIAOPERATION_STARTMULTICASTSTREAMING,
  MEDIAOPERATION_STOPMULTICASTSTREAMING
};

///// Declarations /////

class MediaClient;
class MediaSignals;

///// Classes /////

class AddAudioDecoderConfigurationResponse : public Response<MediaClient>
{
 public:

  AddAudioDecoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::string& configurationtoken);
  virtual ~AddAudioDecoderConfigurationResponse();

  std::string profiletoken_;
  std::string configurationtoken_;

};

class AddAudioEncoderConfigurationResponse : public Response<MediaClient>
{
 public:

  AddAudioEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::string& configurationtoken);
  virtual ~AddAudioEncoderConfigurationResponse();

  std::string profiletoken_;
  std::string configurationtoken_;

};

class AddAudioOutputConfigurationResponse : public Response<MediaClient>
{
 public:

  AddAudioOutputConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::string& configurationtoken);
  virtual ~AddAudioOutputConfigurationResponse();

  std::string profiletoken_;
  std::string configurationtoken_;

};

class AddAudioSourceConfigurationResponse : public Response<MediaClient>
{
 public:

  AddAudioSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::string& configurationtoken);
  virtual ~AddAudioSourceConfigurationResponse();

  std::string profiletoken_;
  std::string configurationtoken_;

};

class AddMetadataConfigurationResponse : public Response<MediaClient>
{
 public:

  AddMetadataConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::string& configurationtoken);
  virtual ~AddMetadataConfigurationResponse();

  std::string profiletoken_;
  std::string configurationtoken_;

};

class AddPTZConfigurationResponse : public Response<MediaClient>
{
 public:

  AddPTZConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::string& configurationtoken);
  virtual ~AddPTZConfigurationResponse();

  std::string profiletoken_;
  std::string configurationtoken_;

};

class AddVideoAnalyticsConfigurationResponse : public Response<MediaClient>
{
 public:

  AddVideoAnalyticsConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::string& configurationtoken);
  virtual ~AddVideoAnalyticsConfigurationResponse();

  std::string profiletoken_;
  std::string configurationtoken_;

};

class AddVideoEncoderConfigurationResponse : public Response<MediaClient>
{
 public:

  AddVideoEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::string& configurationtoken);
  virtual ~AddVideoEncoderConfigurationResponse();

  std::string profiletoken_;
  std::string configurationtoken_;

};

class AddVideoSourceConfigurationResponse : public Response<MediaClient>
{
 public:

  AddVideoSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::string& configurationtoken);
  virtual ~AddVideoSourceConfigurationResponse();

  std::string profiletoken_;
  std::string configurationtoken_;

};

class CreateProfileResponse : public Response<MediaClient>
{
 public:

  CreateProfileResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& name, const boost::optional<std::string>& token);
  CreateProfileResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& name, const boost::optional<std::string>& token, const boost::optional<Profile>& profile);
  virtual ~CreateProfileResponse();

  std::string name_;
  boost::optional<std::string> token_;
  boost::optional<Profile> profile_;

};

class DeleteProfileResponse : public Response<MediaClient>
{
 public:

  DeleteProfileResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token);
  virtual ~DeleteProfileResponse();

  std::string token_;

};

class GetAudioDecoderConfigurationOptionsResponse : public Response<MediaClient>
{
 public:

  GetAudioDecoderConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  GetAudioDecoderConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, const boost::optional<AudioDecoderConfigurationOptions>& options);
  virtual ~GetAudioDecoderConfigurationOptionsResponse();

  boost::optional<std::string> configurationtoken_;
  boost::optional<std::string> profiletoken_;
  boost::optional<AudioDecoderConfigurationOptions> options_;

};

class GetAudioDecoderConfigurationResponse : public Response<MediaClient>
{
 public:

  GetAudioDecoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken);
  GetAudioDecoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken, const boost::optional<AudioDecoderConfiguration>& configuration);
  virtual ~GetAudioDecoderConfigurationResponse();

  std::string configurationtoken_;

  boost::optional<AudioDecoderConfiguration> configuration_;

};

class GetAudioDecoderConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetAudioDecoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetAudioDecoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<AudioDecoderConfiguration>& configurations);
  virtual ~GetAudioDecoderConfigurationsResponse();

  std::vector<AudioDecoderConfiguration> configurations_;

};

class GetAudioEncoderConfigurationOptionsResponse : public Response<MediaClient>
{
 public:

  GetAudioEncoderConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  GetAudioEncoderConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, const boost::optional<AudioEncoderConfigurationOptions>& options);
  virtual ~GetAudioEncoderConfigurationOptionsResponse();

  boost::optional<std::string> configurationtoken_;
  boost::optional<std::string> profiletoken_;

  boost::optional<AudioEncoderConfigurationOptions> options_;

};

class GetAudioEncoderConfigurationResponse : public Response<MediaClient>
{
 public:

  GetAudioEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken);
  GetAudioEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken, const boost::optional<AudioEncoderConfiguration>& configuration);
  virtual ~GetAudioEncoderConfigurationResponse();

  std::string configurationtoken_;

  boost::optional<AudioEncoderConfiguration> configuration_;

};

class GetAudioEncoderConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetAudioEncoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetAudioEncoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<AudioEncoderConfiguration>& configurations);
  virtual ~GetAudioEncoderConfigurationsResponse();

  std::vector<AudioEncoderConfiguration> configurations_;

};

class GetAudioSourceConfigurationOptionsResponse : public Response<MediaClient>
{
 public:

  GetAudioSourceConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  GetAudioSourceConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, const boost::optional<AudioSourceConfigurationOptions>& options);
  virtual ~GetAudioSourceConfigurationOptionsResponse();

  boost::optional<std::string> configurationtoken_;
  boost::optional<std::string> profiletoken_;

  boost::optional<AudioSourceConfigurationOptions> options_;

};

class GetAudioSourceConfigurationResponse : public Response<MediaClient>
{
 public:

  GetAudioSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken);
  GetAudioSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken, const boost::optional<AudioSourceConfiguration>& configuration);
  virtual ~GetAudioSourceConfigurationResponse();

  std::string configurationtoken_;

  boost::optional<AudioSourceConfiguration> configuration_;

};

class GetAudioOutputConfigurationOptionsResponse : public Response<MediaClient>
{
public:

  GetAudioOutputConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  GetAudioOutputConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, const boost::optional<AudioOutputConfigurationOptions>& options);
  virtual ~GetAudioOutputConfigurationOptionsResponse();

  boost::optional<std::string> configurationtoken_;
  boost::optional<std::string> profiletoken_;

  boost::optional<AudioOutputConfigurationOptions> options_;

};

class GetAudioOutputConfigurationResponse : public Response<MediaClient>
{
 public:

  GetAudioOutputConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken);
  GetAudioOutputConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken, const boost::optional<AudioOutputConfiguration>& configuration);
  virtual ~GetAudioOutputConfigurationResponse();

  std::string configurationtoken_;

  boost::optional<AudioOutputConfiguration> configuration_;

};

class GetAudioOutputConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetAudioOutputConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetAudioOutputConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<AudioOutputConfiguration>& configurations);
  virtual ~GetAudioOutputConfigurationsResponse();

  std::vector<AudioOutputConfiguration> configurations_;

};

class GetAudioSourceConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetAudioSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetAudioSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<AudioSourceConfiguration>& configurations);
  virtual ~GetAudioSourceConfigurationsResponse();

  std::vector<AudioSourceConfiguration> configurations_;

};

class GetAudioSourcesResponse : public Response<MediaClient>
{
 public:

  GetAudioSourcesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetAudioSourcesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<AudioSource>& audiosources);
  virtual ~GetAudioSourcesResponse();

  std::vector<AudioSource> audiosources_;

};

class GetCompatibleAudioDecoderConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetCompatibleAudioDecoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  GetCompatibleAudioDecoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<AudioDecoderConfiguration>& configurations);
  virtual ~GetCompatibleAudioDecoderConfigurationsResponse();

  std::string profiletoken_;
  std::vector<AudioDecoderConfiguration> configurations_;

};

class GetCompatibleAudioEncoderConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetCompatibleAudioEncoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  GetCompatibleAudioEncoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<AudioEncoderConfiguration>& configurations);
  virtual ~GetCompatibleAudioEncoderConfigurationsResponse();

  std::string profiletoken_;
  std::vector<AudioEncoderConfiguration> configurations_;

};

class GetCompatibleAudioOutputConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetCompatibleAudioOutputConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  GetCompatibleAudioOutputConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<AudioOutputConfiguration>& configurations);
  virtual ~GetCompatibleAudioOutputConfigurationsResponse();

  std::string profiletoken_;
  std::vector<AudioOutputConfiguration> configurations_;

};

class GetCompatibleAudioSourceConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetCompatibleAudioSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  GetCompatibleAudioSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<AudioSourceConfiguration>& configurations);
  virtual ~GetCompatibleAudioSourceConfigurationsResponse();

  std::string profiletoken_;
  std::vector<AudioSourceConfiguration> configurations_;

};

class GetCompatibleMetadataConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetCompatibleMetadataConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  GetCompatibleMetadataConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<MetadataConfiguration>& configurations);
  virtual ~GetCompatibleMetadataConfigurationsResponse();

  std::string profiletoken_;
  std::vector<MetadataConfiguration> configurations_;

};

class GetCompatibleVideoAnalyticsConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetCompatibleVideoAnalyticsConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  GetCompatibleVideoAnalyticsConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<VideoAnalyticsConfiguration>& configurations);
  virtual ~GetCompatibleVideoAnalyticsConfigurationsResponse();

  std::string profiletoken_;
  std::vector<VideoAnalyticsConfiguration> configurations_;

};

class GetCompatibleVideoEncoderConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetCompatibleVideoEncoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  GetCompatibleVideoEncoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<VideoEncoderConfiguration>& configurations);
  virtual ~GetCompatibleVideoEncoderConfigurationsResponse();

  std::string profiletoken_;
  std::vector<VideoEncoderConfiguration> configurations_;

};

class GetCompatibleVideoSourceConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetCompatibleVideoSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  GetCompatibleVideoSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<VideoSourceConfiguration>& compatiblevideosourceconfigurations);
  virtual ~GetCompatibleVideoSourceConfigurationsResponse();

  std::string profiletoken_;
  std::vector<VideoSourceConfiguration> configurations_;

};

class GetMetadataConfigurationOptionsResponse : public Response<MediaClient>
{
 public:

  GetMetadataConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  GetMetadataConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, const boost::optional<MetadataConfigurationOptions>& options);
  virtual ~GetMetadataConfigurationOptionsResponse();

  boost::optional<std::string> configurationtoken_;
  boost::optional<std::string> profiletoken_;

  boost::optional<MetadataConfigurationOptions> options_;

};

class GetMetadataConfigurationResponse : public Response<MediaClient>
{
 public:

  GetMetadataConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken);
  GetMetadataConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken, const boost::optional<MetadataConfiguration>& configuration);
  virtual ~GetMetadataConfigurationResponse();

  std::string configurationtoken_;

  boost::optional<MetadataConfiguration> configuration_;

};

class GetMetadataConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetMetadataConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetMetadataConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<MetadataConfiguration>& configurations);
  virtual ~GetMetadataConfigurationsResponse();

  std::vector<MetadataConfiguration> configurations_;

};

class GetProfileResponse : public Response<MediaClient>
{
 public:

  GetProfileResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token);
  GetProfileResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const boost::optional<Profile>& profile);
  virtual ~GetProfileResponse();

  std::string token_;
  boost::optional<Profile> profile_;

};

class GetProfilesResponse : public Response<MediaClient>
{
 public:

  GetProfilesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetProfilesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Profile>& profiles);
  virtual ~GetProfilesResponse();

  std::vector<Profile> profiles_;

};

class SetAudioDecoderConfigurationResponse : public Response<MediaClient>
{
 public:

  SetAudioDecoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const AudioDecoderConfiguration& configuration);
  virtual ~SetAudioDecoderConfigurationResponse();

  AudioDecoderConfiguration configuration_;

};

class SetAudioEncoderConfigurationResponse : public Response<MediaClient>
{
 public:

  SetAudioEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const AudioEncoderConfiguration& configuration);
  virtual ~SetAudioEncoderConfigurationResponse();

  AudioEncoderConfiguration configuration_;

};

class SetAudioOutputConfigurationResponse : public Response<MediaClient>
{
public:

  SetAudioOutputConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const AudioOutputConfiguration& configuration);
  virtual ~SetAudioOutputConfigurationResponse();

  AudioOutputConfiguration configuration_;

};

class SetAudioSourceConfigurationResponse : public Response<MediaClient>
{
 public:

  SetAudioSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const AudioSourceConfiguration& configuration);
  virtual ~SetAudioSourceConfigurationResponse();

  AudioSourceConfiguration configuration_;

};

class StartMulticastStreamingResponse : public Response<MediaClient>
{
 public:

  StartMulticastStreamingResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token);
  virtual ~StartMulticastStreamingResponse();

  std::string token_;

};

class StopMulticastStreamingResponse : public Response<MediaClient>
{
 public:

  StopMulticastStreamingResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token);
  virtual ~StopMulticastStreamingResponse();

  std::string token_;

};

class SetVideoSourceConfigurationResponse : public Response<MediaClient>
{
 public:

  SetVideoSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const VideoSourceConfiguration& videosourceconfiguration);
  virtual ~SetVideoSourceConfigurationResponse();

  VideoSourceConfiguration videosourceconfiguration_;

};
class SetVideoEncoderConfigurationResponse : public Response<MediaClient>
{
 public:

  SetVideoEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const VideoEncoderConfiguration& videoencoderconfiguration);
  virtual ~SetVideoEncoderConfigurationResponse();

  VideoEncoderConfiguration videoencoderconfiguration_;

};

class SetSynchronizationPointResponse : public Response<MediaClient>
{
 public:

  SetSynchronizationPointResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token);
  virtual ~SetSynchronizationPointResponse();

  std::string token_;

};

class SetMetadataConfigurationResponse : public Response<MediaClient>
{
 public:

  SetMetadataConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const MetadataConfiguration& configuration);
  virtual ~SetMetadataConfigurationResponse();

  MetadataConfiguration configuration_;

};

class RemoveVideoSourceConfigurationResponse : public Response<MediaClient>
{
 public:

  RemoveVideoSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  virtual ~RemoveVideoSourceConfigurationResponse();

  std::string profiletoken_;

};

class RemoveVideoEncoderConfigurationResponse : public Response<MediaClient>
{
 public:

  RemoveVideoEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  virtual ~RemoveVideoEncoderConfigurationResponse();

  std::string profiletoken_;

};

class RemoveVideoAnalyticsConfigurationResponse : public Response<MediaClient>
{
 public:

  RemoveVideoAnalyticsConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  virtual ~RemoveVideoAnalyticsConfigurationResponse();

  std::string profiletoken_;

};

class RemovePTZConfigurationResponse : public Response<MediaClient>
{
 public:

  RemovePTZConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  virtual ~RemovePTZConfigurationResponse();

  std::string profiletoken_;

};

class RemoveMetadataConfigurationResponse : public Response<MediaClient>
{
 public:

  RemoveMetadataConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  virtual ~RemoveMetadataConfigurationResponse();

  std::string profiletoken_;

};

class RemoveAudioSourceConfigurationResponse : public Response<MediaClient>
{
 public:

  RemoveAudioSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  virtual ~RemoveAudioSourceConfigurationResponse();

  std::string profiletoken_;
  std::string configurationtoken_;

};

class RemoveAudioOutputConfigurationResponse : public Response<MediaClient>
{
 public:

  RemoveAudioOutputConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  virtual ~RemoveAudioOutputConfigurationResponse();

  std::string profiletoken_;

};

class RemoveAudioEncoderConfigurationResponse : public Response<MediaClient>
{
 public:

  RemoveAudioEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  virtual ~RemoveAudioEncoderConfigurationResponse();

  std::string profiletoken_;

};

class RemoveAudioDecoderConfigurationResponse : public Response<MediaClient>
{
 public:

  RemoveAudioDecoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken);
  virtual ~RemoveAudioDecoderConfigurationResponse();

  std::string profiletoken_;

};

class GetVideoSourcesResponse : public Response<MediaClient>
{
 public:

  GetVideoSourcesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetVideoSourcesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<VideoSource>& videosources);
  virtual ~GetVideoSourcesResponse();

  std::vector<VideoSource> videosources_;

};

class GetVideoSourceModesResponse : public Response<MediaClient>
{
 public:

  GetVideoSourceModesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token);
  GetVideoSourceModesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const std::vector<media::VideoSourceMode>& videosourcemodes_);
  virtual ~GetVideoSourceModesResponse();

  std::string token_;
  std::vector<media::VideoSourceMode> videosourcemodes_;

};

class GetVideoSourceConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetVideoSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetVideoSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<VideoSourceConfiguration>& videosourceconfigurations);
  virtual ~GetVideoSourceConfigurationsResponse();

  std::vector<VideoSourceConfiguration> configurations_;

};

class GetVideoSourceConfigurationResponse : public Response<MediaClient>
{
 public:

  GetVideoSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token);
  GetVideoSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const boost::optional<VideoSourceConfiguration>& videosourceconfiguration);
  virtual ~GetVideoSourceConfigurationResponse();

  std::string token_;
  boost::optional<VideoSourceConfiguration> videosourceconfiguration_;

};

class GetVideoSourceConfigurationOptionsResponse : public Response<MediaClient>
{
 public:

  GetVideoSourceConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  GetVideoSourceConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, const boost::optional<VideoSourceConfigurationOptions>& videosourceconfigurationoptions);
  virtual ~GetVideoSourceConfigurationOptionsResponse();

  boost::optional<std::string> configurationtoken_;
  boost::optional<std::string> profiletoken_;
  boost::optional<VideoSourceConfigurationOptions> videosourceconfigurationoptions_;

};

class GetVideoEncoderConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetVideoEncoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetVideoEncoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<VideoEncoderConfiguration>& videoencoderconfigurations);
  virtual ~GetVideoEncoderConfigurationsResponse();

  std::vector<VideoEncoderConfiguration> configurations_;

};

class GetVideoEncoderConfigurationResponse : public Response<MediaClient>
{
 public:

  GetVideoEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token);
  GetVideoEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const boost::optional<VideoEncoderConfiguration>& videoencoderconfiguration);
  virtual ~GetVideoEncoderConfigurationResponse();

  std::string token_;
  boost::optional<VideoEncoderConfiguration> videoencoderconfiguration_;

};

class GetVideoEncoderConfigurationOptionsResponse : public Response<MediaClient>
{
 public:

  GetVideoEncoderConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  GetVideoEncoderConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, const boost::optional<VideoEncoderConfigurationOptions>& videoencoderconfigurationoptions);
  virtual ~GetVideoEncoderConfigurationOptionsResponse();

  boost::optional<std::string> configurationtoken_;
  boost::optional<std::string> profiletoken_;
  boost::optional<VideoEncoderConfigurationOptions> videoencoderconfigurationoptions_;

};

class GetVideoAnalyticsConfigurationsResponse : public Response<MediaClient>
{
 public:

  GetVideoAnalyticsConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetVideoAnalyticsConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<VideoAnalyticsConfiguration>& videoanalyticsconfigurations);
  virtual ~GetVideoAnalyticsConfigurationsResponse();

  std::vector<VideoAnalyticsConfiguration> configurations_;

};

class GetStreamUriResponse : public Response<MediaClient>
{
 public:

  GetStreamUriResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const StreamSetup& streamsetup, const std::string& token);
  GetStreamUriResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const StreamSetup& streamsetup, const std::string& token, const boost::optional<MediaUri>& mediauri);
  virtual ~GetStreamUriResponse();

  StreamSetup streamsetup_;
  std::string token_;
  boost::optional<MediaUri> mediauri_;

};

class GetSnapshotUriResponse : public Response<MediaClient>
{
 public:

  GetSnapshotUriResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token);
  GetSnapshotUriResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const boost::optional<MediaUri>& mediauri);
  virtual ~GetSnapshotUriResponse();

  std::string token_;
  boost::optional<MediaUri> mediauri_;

};

class GetServiceCapabilitiesResponse : public Response<MediaClient>
{
 public:

  GetServiceCapabilitiesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetServiceCapabilitiesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities);
  virtual ~GetServiceCapabilitiesResponse();

  boost::optional<Capabilities> capabilities_;

};

class MediaClient : public Client<MEDIAOPERATION>, public boost::enable_shared_from_this<MediaClient>
{
  friend Signal< MEDIAOPERATION, MediaClient, AddAudioDecoderConfigurationResponse, std::string, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, AddAudioEncoderConfigurationResponse, std::string, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, AddAudioOutputConfigurationResponse, std::string, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, AddAudioSourceConfigurationResponse, std::string, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, AddMetadataConfigurationResponse, std::string, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, AddPTZConfigurationResponse, std::string, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, AddVideoAnalyticsConfigurationResponse, std::string, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, AddVideoEncoderConfigurationResponse, std::string, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, AddVideoSourceConfigurationResponse, std::string, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, CreateProfileResponse, std::string, boost::optional<std::string> >;
  friend Signal< MEDIAOPERATION, MediaClient, DeleteProfileResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioDecoderConfigurationResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioDecoderConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> >;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioDecoderConfigurationsResponse>;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioEncoderConfigurationResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioEncoderConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> >;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioEncoderConfigurationsResponse>;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioOutputConfigurationResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioOutputConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> >;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioOutputConfigurationsResponse>;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioSourceConfigurationResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioSourceConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> >;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioSourceConfigurationsResponse>;
  friend Signal< MEDIAOPERATION, MediaClient, GetAudioSourcesResponse>;
  friend Signal< MEDIAOPERATION, MediaClient, GetCompatibleAudioDecoderConfigurationsResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetCompatibleAudioEncoderConfigurationsResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetCompatibleAudioOutputConfigurationsResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetCompatibleAudioSourceConfigurationsResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetCompatibleMetadataConfigurationsResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetCompatibleVideoAnalyticsConfigurationsResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetCompatibleVideoEncoderConfigurationsResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetCompatibleVideoSourceConfigurationsResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetMetadataConfigurationResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetMetadataConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> >;
  friend Signal< MEDIAOPERATION, MediaClient, GetMetadataConfigurationsResponse>;
  friend Signal< MEDIAOPERATION, MediaClient, GetProfileResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetProfilesResponse>;
  friend Signal< MEDIAOPERATION, MediaClient, GetServiceCapabilitiesResponse>;
  friend Signal< MEDIAOPERATION, MediaClient, GetSnapshotUriResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetStreamUriResponse, StreamSetup, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetVideoAnalyticsConfigurationsResponse>;
  friend Signal< MEDIAOPERATION, MediaClient, GetVideoEncoderConfigurationResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetVideoEncoderConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> >;
  friend Signal< MEDIAOPERATION, MediaClient, GetVideoEncoderConfigurationsResponse>;
  friend Signal< MEDIAOPERATION, MediaClient, GetVideoSourceConfigurationResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetVideoSourceConfigurationOptionsResponse, boost::optional<std::string>, boost::optional<std::string> >;
  friend Signal< MEDIAOPERATION, MediaClient, GetVideoSourceConfigurationsResponse>;
  friend Signal< MEDIAOPERATION, MediaClient, GetVideoSourceModesResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, GetVideoSourcesResponse>;
  friend Signal< MEDIAOPERATION, MediaClient, RemoveAudioDecoderConfigurationResponse, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, RemoveAudioEncoderConfigurationResponse, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, RemoveAudioOutputConfigurationResponse, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, RemoveAudioSourceConfigurationResponse, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, RemoveMetadataConfigurationResponse, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, RemovePTZConfigurationResponse, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, RemoveVideoAnalyticsConfigurationResponse, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, RemoveVideoEncoderConfigurationResponse, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, RemoveVideoSourceConfigurationResponse, std::string >;
  friend Signal< MEDIAOPERATION, MediaClient, SetAudioDecoderConfigurationResponse, AudioDecoderConfiguration>;
  friend Signal< MEDIAOPERATION, MediaClient, SetAudioEncoderConfigurationResponse, AudioEncoderConfiguration>;
  friend Signal< MEDIAOPERATION, MediaClient, SetAudioOutputConfigurationResponse, AudioOutputConfiguration>;
  friend Signal< MEDIAOPERATION, MediaClient, SetAudioSourceConfigurationResponse, AudioSourceConfiguration>;
  friend Signal< MEDIAOPERATION, MediaClient, SetMetadataConfigurationResponse, MetadataConfiguration>;
  friend Signal< MEDIAOPERATION, MediaClient, SetSynchronizationPointResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, SetVideoEncoderConfigurationResponse, VideoEncoderConfiguration>;
  friend Signal< MEDIAOPERATION, MediaClient, SetVideoSourceConfigurationResponse, VideoSourceConfiguration>;
  friend Signal< MEDIAOPERATION, MediaClient, StartMulticastStreamingResponse, std::string>;
  friend Signal< MEDIAOPERATION, MediaClient, StopMulticastStreamingResponse, std::string>;

 public:
   
  using Client::Update;

  MediaClient(const boost::shared_ptr<std::recursive_mutex>& mutex);
  virtual ~MediaClient();

  virtual int Init(const sock::ProxyParams& proxyparams, const std::string& address, const std::string& username, const std::string& password, const unsigned int maxconcurrentrequests, const bool forcehttpauthentication, const bool forbidreuse) override;
  virtual void Destroy() override;

  // Requests
  void AddAudioDecoderConfiguration(const std::string& profiletoken, const std::string& configurationtoken);
  void AddAudioEncoderConfiguration(const std::string& profiletoken, const std::string& configurationtoken);
  void AddAudioOutputConfiguration(const std::string& profiletoken, const std::string& configurationtoken);
  void AddAudioSourceConfiguration(const std::string& profiletoken, const std::string& configurationtoken);
  void AddMetadataConfiguration(const std::string& profiletoken, const std::string& configurationtoken);
  void AddPTZConfiguration(const std::string& profiletoken, const std::string& configurationtoken);
  void AddVideoAnalyticsConfiguration(const std::string& profiletoken, const std::string& configurationtoken);
  void AddVideoEncoderConfiguration(const std::string& profiletoken, const std::string& configurationtoken);
  void AddVideoSourceConfiguration(const std::string& profiletoken, const std::string& configurationtoken);
  void CreateProfile(const std::string& name, const boost::optional<std::string>& token);
  void DeleteProfile(const std::string& token);
  void GetAudioDecoderConfiguration(const std::string& configurationtoken);
  void GetAudioDecoderConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  void GetAudioDecoderConfigurations();
  void GetAudioEncoderConfiguration(const std::string& configurationtoken);
  void GetAudioEncoderConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  void GetAudioEncoderConfigurations();
  void GetAudioOutputConfiguration(const std::string& configurationtoken);
  void GetAudioOutputConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  void GetAudioOutputConfigurations();
  void GetAudioSourceConfiguration(const std::string& configurationtoken);
  void GetAudioSourceConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  void GetAudioSourceConfigurations();
  void GetAudioSources();
  void GetCompatibleAudioDecoderConfigurations(const std::string& profiletoken);
  void GetCompatibleAudioEncoderConfigurations(const std::string& profiletoken);
  void GetCompatibleAudioOutputConfigurations(const std::string& profiletoken);
  void GetCompatibleAudioSourceConfigurations(const std::string& profiletoken);
  void GetCompatibleMetadataConfigurations(const std::string& profiletoken);
  void GetCompatibleVideoAnalyticsConfigurations(const std::string& profiletoken);
  void GetCompatibleVideoEncoderConfigurations(const std::string& profiletoken);
  void GetCompatibleVideoSourceConfigurations(const std::string& profiletoken);
  void GetMetadataConfiguration(const std::string& configurationtoken);
  void GetMetadataConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  void GetMetadataConfigurations();
  void GetProfile(const std::string& token);
  void GetProfiles();
  void GetServiceCapabilities();
  void GetSnapshotUri(const std::string& token);
  void GetStreamUri(const StreamSetup& streamsetup, const std::string& token);
  void GetVideoAnalyticsConfigurations();
  void GetVideoEncoderConfiguration(const std::string& token);
  void GetVideoEncoderConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  void GetVideoEncoderConfigurations();
  void GetVideoSourceConfiguration(const std::string& token);
  void GetVideoSourceConfigurationOptions(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  void GetVideoSourceConfigurations();
  void GetVideoSourceModes(const std::string& token);
  void GetVideoSources();
  void RemoveAudioDecoderConfiguration(const std::string& profiletoken);
  void RemoveAudioEncoderConfiguration(const std::string& profiletoken);
  void RemoveAudioOutputConfiguration(const std::string& profiletoken);
  void RemoveAudioSourceConfiguration(const std::string& profiletoken);
  void RemoveMetadataConfiguration(const std::string& profiletoken);
  void RemovePTZConfiguration(const std::string& profiletoken);
  void RemoveVideoAnalyticsConfiguration(const std::string& profiletoken);
  void RemoveVideoEncoderConfiguration(const std::string& profiletoken);
  void RemoveVideoSourceConfiguration(const std::string& profiletoken);
  void SetAudioDecoderConfiguration(const AudioDecoderConfiguration& configuration);
  void SetAudioEncoderConfiguration(const AudioEncoderConfiguration& configuration);
  void SetAudioOutputConfiguration(const AudioOutputConfiguration& configuration);
  void SetAudioSourceConfiguration(const AudioSourceConfiguration& configuration);
  void SetMetadataConfiguration(const MetadataConfiguration& configuration);
  void SetSynchronizationPoint(const std::string& profiletoken);
  void SetVideoEncoderConfiguration(const VideoEncoderConfiguration& videoencoderconfiguration);
  void SetVideoSourceConfiguration(const VideoSourceConfiguration& videosourceconfiguration);
  void StartMulticastStreaming(const std::string& profiletoken);
  void StopMulticastStreaming(const std::string& profiletoken);
  
  // Callback requests
  Connection AddAudioDecoderConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddAudioDecoderConfigurationResponse&)> callback);
  Connection AddAudioEncoderConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddAudioEncoderConfigurationResponse&)> callback);
  Connection AddAudioOutputConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddAudioOutputConfigurationResponse&)> callback);
  Connection AddAudioSourceConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddAudioSourceConfigurationResponse&)> callback);
  Connection AddMetadataConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddMetadataConfigurationResponse&)> callback);
  Connection AddPTZConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddPTZConfigurationResponse&)> callback);
  Connection AddVideoAnalyticsConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddVideoAnalyticsConfigurationResponse&)> callback);
  Connection AddVideoEncoderConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddVideoEncoderConfigurationResponse&)> callback);
  Connection AddVideoSourceConfigurationCallback(const std::string& profiletoken, const std::string& configurationtoken, boost::function<void(const AddVideoSourceConfigurationResponse&)> callback);
  Connection CreateProfileCallback(const std::string& name, const boost::optional<std::string>& token, boost::function<void(const CreateProfileResponse&)> callback);
  Connection DeleteProfileCallback(const std::string& token, boost::function<void(const DeleteProfileResponse&)> callback);
  Connection GetAudioDecoderConfigurationCallback(const std::string& configurationtoken, boost::function<void(const GetAudioDecoderConfigurationResponse&)> callback);
  Connection GetAudioDecoderConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetAudioDecoderConfigurationOptionsResponse&)> callback);
  Connection GetAudioDecoderConfigurationsCallback(boost::function<void(const GetAudioDecoderConfigurationsResponse&)> callback);
  Connection GetAudioEncoderConfigurationCallback(const std::string& configurationtoken, boost::function<void(const GetAudioEncoderConfigurationResponse&)> callback);
  Connection GetAudioEncoderConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetAudioEncoderConfigurationOptionsResponse&)> callback);
  Connection GetAudioEncoderConfigurationsCallback(boost::function<void(const GetAudioEncoderConfigurationsResponse&)> callback);
  Connection GetAudioOutputConfigurationCallback(const std::string& configurationtoken, boost::function<void(const GetAudioOutputConfigurationResponse&)> callback);
  Connection GetAudioOutputConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetAudioOutputConfigurationOptionsResponse&)> callback);
  Connection GetAudioOutputConfigurationsCallback(boost::function<void(const GetAudioOutputConfigurationsResponse&)> callback);
  Connection GetAudioSourceConfigurationCallback(const std::string& configurationtoken, boost::function<void(const GetAudioSourceConfigurationResponse&)> callback);
  Connection GetAudioSourceConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetAudioSourceConfigurationOptionsResponse&)> callback);
  Connection GetAudioSourceConfigurationsCallback(boost::function<void(const GetAudioSourceConfigurationsResponse&)> callback);
  Connection GetAudioSourcesCallback(boost::function<void(const GetAudioSourcesResponse&)> callback);
  Connection GetCompatibleAudioDecoderConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleAudioDecoderConfigurationsResponse&)> callback);
  Connection GetCompatibleAudioEncoderConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleAudioEncoderConfigurationsResponse&)> callback);
  Connection GetCompatibleAudioOutputConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleAudioOutputConfigurationsResponse&)> callback);
  Connection GetCompatibleAudioSourceConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleAudioSourceConfigurationsResponse&)> callback);
  Connection GetCompatibleMetadataConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleMetadataConfigurationsResponse&)> callback);
  Connection GetCompatibleVideoAnalyticsConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleVideoAnalyticsConfigurationsResponse&)> callback);
  Connection GetCompatibleVideoEncoderConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleVideoEncoderConfigurationsResponse&)> callback);
  Connection GetCompatibleVideoSourceConfigurationsCallback(const std::string& profiletoken, boost::function<void(const GetCompatibleVideoSourceConfigurationsResponse&)> callback);
  Connection GetMetadataConfigurationCallback(const std::string& configurationtoken, boost::function<void(const GetMetadataConfigurationResponse&)> callback);
  Connection GetMetadataConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetMetadataConfigurationOptionsResponse&)> callback);
  Connection GetMetadataConfigurationsCallback(boost::function<void(const GetMetadataConfigurationsResponse&)> callback);
  Connection GetProfileCallback(const std::string& token, boost::function<void(const GetProfileResponse&)> callback);
  Connection GetProfilesCallback(boost::function<void(const GetProfilesResponse&)> callback);
  Connection GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback);
  Connection GetSnapshotUriCallback(const std::string& token, boost::function<void(const GetSnapshotUriResponse&)> callback);
  Connection GetStreamUriCallback(const StreamSetup& streamsetup, const std::string& token, boost::function<void(const GetStreamUriResponse&)> callback);
  Connection GetVideoAnalyticsConfigurationsCallback(boost::function<void(const GetVideoAnalyticsConfigurationsResponse&)> callback);
  Connection GetVideoEncoderConfigurationCallback(const std::string& token, boost::function<void(const GetVideoEncoderConfigurationResponse&)> callback);
  Connection GetVideoEncoderConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetVideoEncoderConfigurationOptionsResponse&)> callback);
  Connection GetVideoEncoderConfigurationsCallback(boost::function<void(const GetVideoEncoderConfigurationsResponse&)> callback);
  Connection GetVideoSourceConfigurationCallback(const std::string& token, boost::function<void(const GetVideoSourceConfigurationResponse&)> callback);
  Connection GetVideoSourceConfigurationOptionsCallback(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, boost::function<void(const GetVideoSourceConfigurationOptionsResponse&)> callback);
  Connection GetVideoSourceConfigurationsCallback(boost::function<void(const GetVideoSourceConfigurationsResponse&)> callback);
  Connection GetVideoSourceModesCallback(const std::string& token, boost::function<void(const GetVideoSourceModesResponse&)> callback);
  Connection GetVideoSourcesCallback(boost::function<void(const GetVideoSourcesResponse&)> callback);
  Connection RemoveAudioDecoderConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveAudioDecoderConfigurationResponse&)> callback);
  Connection RemoveAudioEncoderConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveAudioEncoderConfigurationResponse&)> callback);
  Connection RemoveAudioOutputConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveAudioOutputConfigurationResponse&)> callback);
  Connection RemoveAudioSourceConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveAudioSourceConfigurationResponse&)> callback);
  Connection RemoveMetadataConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveMetadataConfigurationResponse&)> callback);
  Connection RemovePTZConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemovePTZConfigurationResponse&)> callback);
  Connection RemoveVideoAnalyticsConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveVideoAnalyticsConfigurationResponse&)> callback);
  Connection RemoveVideoEncoderConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveVideoEncoderConfigurationResponse&)> callback);
  Connection RemoveVideoSourceConfigurationCallback(const std::string& profiletoken, boost::function<void(const RemoveVideoSourceConfigurationResponse&)> callback);
  Connection SetAudioDecoderConfigurationCallback(const AudioDecoderConfiguration& configuration, boost::function<void(const SetAudioDecoderConfigurationResponse&)> callback);
  Connection SetAudioEncoderConfigurationCallback(const AudioEncoderConfiguration& configuration, boost::function<void(const SetAudioEncoderConfigurationResponse&)> callback);
  Connection SetAudioOutputConfigurationCallback(const AudioOutputConfiguration& configuration, boost::function<void(const SetAudioOutputConfigurationResponse&)> callback);
  Connection SetAudioSourceConfigurationCallback(const AudioSourceConfiguration& configuration, boost::function<void(const SetAudioSourceConfigurationResponse&)> callback);
  Connection SetMetadataConfigurationCallback(const MetadataConfiguration& configuration, boost::function<void(const SetMetadataConfigurationResponse&)> callback);
  Connection SetSynchronizationPointCallback(const std::string& profiletoken, boost::function<void(const SetSynchronizationPointResponse&)> callback);
  Connection SetVideoEncoderConfigurationCallback(const VideoEncoderConfiguration& videoencoderconfiguration, boost::function<void(const SetVideoEncoderConfigurationResponse&)> callback);
  Connection SetVideoSourceConfigurationCallback(const VideoSourceConfiguration& videosourceconfiguration, boost::function<void(const SetVideoSourceConfigurationResponse&)> callback);
  Connection StartMulticastStreamingCallback(const std::string& profiletoken, boost::function<void(const StartMulticastStreamingResponse&)> callback);
  Connection StopMulticastStreamingCallback(const std::string& profiletoken, boost::function<void(const StopMulticastStreamingResponse&)> callback);

  // Future requests
  boost::unique_future<AddAudioDecoderConfigurationResponse> AddAudioDecoderConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken);
  boost::unique_future<AddAudioEncoderConfigurationResponse> AddAudioEncoderConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken);
  boost::unique_future<AddAudioOutputConfigurationResponse> AddAudioOutputConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken);
  boost::unique_future<AddAudioSourceConfigurationResponse> AddAudioSourceConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken);
  boost::unique_future<AddMetadataConfigurationResponse> AddMetadataConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken);
  boost::unique_future<AddPTZConfigurationResponse> AddPTZConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken);
  boost::unique_future<AddVideoAnalyticsConfigurationResponse> AddVideoAnalyticsConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken);
  boost::unique_future<AddVideoEncoderConfigurationResponse> AddVideoEncoderConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken);
  boost::unique_future<AddVideoSourceConfigurationResponse> AddVideoSourceConfigurationFuture(const std::string& profiletoken, const std::string& configurationtoken);
  boost::unique_future<CreateProfileResponse> CreateProfileFuture(const std::string& name, const boost::optional<std::string>& token);
  boost::unique_future<DeleteProfileResponse> DeleteProfileFuture(const std::string& token);
  boost::unique_future<GetAudioDecoderConfigurationResponse> GetAudioDecoderConfigurationFuture(const std::string& configurationtoken);
  boost::unique_future<GetAudioDecoderConfigurationOptionsResponse> GetAudioDecoderConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  boost::unique_future<GetAudioDecoderConfigurationsResponse> GetAudioDecoderConfigurationsFuture();
  boost::unique_future<GetAudioEncoderConfigurationResponse> GetAudioEncoderConfigurationFuture(const std::string& configurationtoken);
  boost::unique_future<GetAudioEncoderConfigurationOptionsResponse> GetAudioEncoderConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  boost::unique_future<GetAudioEncoderConfigurationsResponse> GetAudioEncoderConfigurationsFuture();
  boost::unique_future<GetAudioOutputConfigurationResponse> GetAudioOutputConfigurationFuture(const std::string& configurationtoken);
  boost::unique_future<GetAudioOutputConfigurationOptionsResponse> GetAudioOutputConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  boost::unique_future<GetAudioOutputConfigurationsResponse> GetAudioOutputConfigurationsFuture();
  boost::unique_future<GetAudioSourceConfigurationResponse> GetAudioSourceConfigurationFuture(const std::string& configurationtoken);
  boost::unique_future<GetAudioSourceConfigurationOptionsResponse> GetAudioSourceConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  boost::unique_future<GetAudioSourceConfigurationsResponse> GetAudioSourceConfigurationsFuture();
  boost::unique_future<GetAudioSourcesResponse> GetAudioSourcesFuture();
  boost::unique_future<GetCompatibleAudioDecoderConfigurationsResponse> GetCompatibleAudioDecoderConfigurationsFuture(const std::string& profiletoken);
  boost::unique_future<GetCompatibleAudioEncoderConfigurationsResponse> GetCompatibleAudioEncoderConfigurationsFuture(const std::string& profiletoken);
  boost::unique_future<GetCompatibleAudioOutputConfigurationsResponse> GetCompatibleAudioOutputConfigurationsFuture(const std::string& profiletoken);
  boost::unique_future<GetCompatibleAudioSourceConfigurationsResponse> GetCompatibleAudioSourceConfigurationsFuture(const std::string& profiletoken);
  boost::unique_future<GetCompatibleMetadataConfigurationsResponse> GetCompatibleMetadataConfigurationsFuture(const std::string& profiletoken);
  boost::unique_future<GetCompatibleVideoAnalyticsConfigurationsResponse> GetCompatibleVideoAnalyticsConfigurationsFuture(const std::string& profiletoken);
  boost::unique_future<GetCompatibleVideoEncoderConfigurationsResponse> GetCompatibleVideoEncoderConfigurationsFuture(const std::string& profiletoken);
  boost::unique_future<GetCompatibleVideoSourceConfigurationsResponse> GetCompatibleVideoSourceConfigurationsFuture(const std::string& profiletoken);
  boost::unique_future<GetMetadataConfigurationResponse> GetMetadataConfigurationFuture(const std::string& configurationtoken);
  boost::unique_future<GetMetadataConfigurationOptionsResponse> GetMetadataConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  boost::unique_future<GetMetadataConfigurationsResponse> GetMetadataConfigurationsFuture();
  boost::unique_future<GetProfileResponse> GetProfileFuture(const std::string& token);
  boost::unique_future<GetProfilesResponse> GetProfilesFuture();
  boost::unique_future<GetServiceCapabilitiesResponse> GetServiceCapabilitiesFuture();
  boost::unique_future<GetSnapshotUriResponse> GetSnapshotUriFuture(const std::string& token);
  boost::unique_future<GetStreamUriResponse> GetStreamUriFuture(const StreamSetup& streamsetup, const std::string& token);
  boost::unique_future<GetVideoAnalyticsConfigurationsResponse> GetVideoAnalyticsConfigurationsFuture();
  boost::unique_future<GetVideoEncoderConfigurationResponse> GetVideoEncoderConfigurationFuture(const std::string& token);
  boost::unique_future<GetVideoEncoderConfigurationOptionsResponse> GetVideoEncoderConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  boost::unique_future<GetVideoEncoderConfigurationsResponse> GetVideoEncoderConfigurationsFuture();
  boost::unique_future<GetVideoSourceConfigurationResponse> GetVideoSourceConfigurationFuture(const std::string& token);
  boost::unique_future<GetVideoSourceConfigurationOptionsResponse> GetVideoSourceConfigurationOptionsFuture(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  boost::unique_future<GetVideoSourceConfigurationsResponse> GetVideoSourceConfigurationsFuture();
  boost::unique_future<GetVideoSourceModesResponse> GetVideoSourceModesFuture(const std::string& token);
  boost::unique_future<GetVideoSourcesResponse> GetVideoSourcesFuture();
  boost::unique_future<RemoveAudioDecoderConfigurationResponse> RemoveAudioDecoderConfigurationFuture(const std::string& profiletoken);
  boost::unique_future<RemoveAudioEncoderConfigurationResponse> RemoveAudioEncoderConfigurationFuture(const std::string& profiletoken);
  boost::unique_future<RemoveAudioOutputConfigurationResponse> RemoveAudioOutputConfigurationFuture(const std::string& profiletoken);
  boost::unique_future<RemoveAudioSourceConfigurationResponse> RemoveAudioSourceConfigurationFuture(const std::string& profiletoken);
  boost::unique_future<RemoveMetadataConfigurationResponse> RemoveMetadataConfigurationFuture(const std::string& profiletoken);
  boost::unique_future<RemovePTZConfigurationResponse> RemovePTZConfigurationFuture(const std::string& profiletoken);
  boost::unique_future<RemoveVideoAnalyticsConfigurationResponse> RemoveVideoAnalyticsConfigurationFuture(const std::string& profiletoken);
  boost::unique_future<RemoveVideoEncoderConfigurationResponse> RemoveVideoEncoderConfigurationFuture(const std::string& profiletoken);
  boost::unique_future<RemoveVideoSourceConfigurationResponse> RemoveVideoSourceConfigurationFuture(const std::string& profiletoken);
  boost::unique_future<SetAudioDecoderConfigurationResponse> SetAudioDecoderConfigurationFuture(const AudioDecoderConfiguration& configuration);
  boost::unique_future<SetAudioEncoderConfigurationResponse> SetAudioEncoderConfigurationFuture(const AudioEncoderConfiguration& configuration);
  boost::unique_future<SetAudioOutputConfigurationResponse> SetAudioOutputConfigurationFuture(const AudioOutputConfiguration& configuration);
  boost::unique_future<SetAudioSourceConfigurationResponse> SetAudioSourceConfigurationFuture(const AudioSourceConfiguration& configuration);
  boost::unique_future<SetMetadataConfigurationResponse> SetMetadataConfigurationFuture(const MetadataConfiguration& configuration);
  boost::unique_future<SetSynchronizationPointResponse> SetSynchronizationPointFuture(const std::string& profiletoken);
  boost::unique_future<SetVideoEncoderConfigurationResponse> SetVideoEncoderConfigurationFuture(const VideoEncoderConfiguration& videoencoderconfiguration);
  boost::unique_future<SetVideoSourceConfigurationResponse> SetVideoSourceConfigurationFuture(const VideoSourceConfiguration& videosourceconfiguration);
  boost::unique_future<StartMulticastStreamingResponse> StartMulticastStreamingFuture(const std::string& profiletoken);
  boost::unique_future<StopMulticastStreamingResponse> StopMulticastStreamingFuture(const std::string& profiletoken);

  // Boost Signals2
  boost::signals2::signal<void(const AddAudioDecoderConfigurationResponse&)>& AddAudioDecoderConfigurationSignal();
  boost::signals2::signal<void(const AddAudioEncoderConfigurationResponse&)>& AddAudioEncoderConfigurationSignal();
  boost::signals2::signal<void(const AddAudioOutputConfigurationResponse&)>& AddAudioOutputConfigurationSignal();
  boost::signals2::signal<void(const AddAudioSourceConfigurationResponse&)>& AddAudioSourceConfigurationSignal();
  boost::signals2::signal<void(const AddMetadataConfigurationResponse&)>& AddMetadataConfigurationSignal();
  boost::signals2::signal<void(const AddPTZConfigurationResponse&)>& AddPTZConfigurationSignal();
  boost::signals2::signal<void(const AddVideoAnalyticsConfigurationResponse&)>& AddVideoAnalyticsConfigurationSignal();
  boost::signals2::signal<void(const AddVideoEncoderConfigurationResponse&)>& AddVideoEncoderConfigurationSignal();
  boost::signals2::signal<void(const AddVideoSourceConfigurationResponse&)>& AddVideoSourceConfigurationSignal();
  boost::signals2::signal<void(const CreateProfileResponse&)>& CreateProfileSignal();
  boost::signals2::signal<void(const DeleteProfileResponse&)>& DeleteProfileSignal();
  boost::signals2::signal<void(const GetAudioDecoderConfigurationResponse&)>& GetAudioDecoderConfigurationSignal();
  boost::signals2::signal<void(const GetAudioDecoderConfigurationOptionsResponse&)>& GetAudioDecoderConfigurationOptionsSignal();
  boost::signals2::signal<void(const GetAudioDecoderConfigurationsResponse&)>& GetAudioDecoderConfigurationsSignal();
  boost::signals2::signal<void(const GetAudioEncoderConfigurationResponse&)>& GetAudioEncoderConfigurationSignal();
  boost::signals2::signal<void(const GetAudioEncoderConfigurationOptionsResponse&)>& GetAudioEncoderConfigurationOptionsSignal();
  boost::signals2::signal<void(const GetAudioEncoderConfigurationsResponse&)>& GetAudioEncoderConfigurationsSignal();
  boost::signals2::signal<void(const GetAudioOutputConfigurationResponse&)>& GetAudioOutputConfigurationSignal();
  boost::signals2::signal<void(const GetAudioOutputConfigurationOptionsResponse&)>& GetAudioOutputConfigurationOptionsSignal();
  boost::signals2::signal<void(const GetAudioOutputConfigurationsResponse&)>& GetAudioOutputConfigurationsSignal();
  boost::signals2::signal<void(const GetAudioSourceConfigurationResponse&)>& GetAudioSourceConfigurationSignal();
  boost::signals2::signal<void(const GetAudioSourceConfigurationOptionsResponse&)>& GetAudioSourceConfigurationOptionsSignal();
  boost::signals2::signal<void(const GetAudioSourceConfigurationsResponse&)>& GetAudioSourceConfigurationsSignal();
  boost::signals2::signal<void(const GetAudioSourcesResponse&)>& GetAudioSourcesSignal();
  boost::signals2::signal<void(const GetCompatibleAudioDecoderConfigurationsResponse&)>& GetCompatibleAudioDecoderConfigurations();
  boost::signals2::signal<void(const GetCompatibleAudioEncoderConfigurationsResponse&)>& GetCompatibleAudioEncoderConfigurations();
  boost::signals2::signal<void(const GetCompatibleAudioOutputConfigurationsResponse&)>& GetCompatibleAudioOutputConfigurations();
  boost::signals2::signal<void(const GetCompatibleAudioSourceConfigurationsResponse&)>& GetCompatibleAudioSourceConfigurations();
  boost::signals2::signal<void(const GetCompatibleMetadataConfigurationsResponse&)>& GetCompatibleMetadataConfigurations();
  boost::signals2::signal<void(const GetCompatibleVideoAnalyticsConfigurationsResponse&)>& GetCompatibleVideoAnalyticsConfigurations();
  boost::signals2::signal<void(const GetCompatibleVideoEncoderConfigurationsResponse&)>& GetCompatibleVideoEncoderConfigurations();
  boost::signals2::signal<void(const GetCompatibleVideoSourceConfigurationsResponse&)>& GetCompatibleVideoSourceConfigurations();
  boost::signals2::signal<void(const GetMetadataConfigurationResponse&)>& GetMetadataConfigurationSignal();
  boost::signals2::signal<void(const GetMetadataConfigurationOptionsResponse&)>& GetMetadataConfigurationOptionsSignal();
  boost::signals2::signal<void(const GetMetadataConfigurationsResponse&)>& GetMetadataConfigurationsSignal();
  boost::signals2::signal<void(const GetProfileResponse&)>& GetProfileSignal();
  boost::signals2::signal<void(const GetProfilesResponse&)>& GetProfilesSignal();
  boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& GetServiceCapabilitiesSignal();
  boost::signals2::signal<void(const GetSnapshotUriResponse&)>& GetSnapshotUriSignal();
  boost::signals2::signal<void(const GetStreamUriResponse&)>& GetStreamUriSignal();
  boost::signals2::signal<void(const GetVideoAnalyticsConfigurationsResponse&)>& GetVideoAnalyticsConfigurationsSignal();
  boost::signals2::signal<void(const GetVideoEncoderConfigurationResponse&)>& GetVideoEncoderConfigurationSignal();
  boost::signals2::signal<void(const GetVideoEncoderConfigurationOptionsResponse&)>& GetVideoEncoderConfiguratioOptionsSignal();
  boost::signals2::signal<void(const GetVideoEncoderConfigurationsResponse&)>& GetVideoEncoderConfigurationsSignal();
  boost::signals2::signal<void(const GetVideoSourceConfigurationResponse&)>& GetVideoSourceConfigurationSignal();
  boost::signals2::signal<void(const GetVideoSourceConfigurationOptionsResponse&)>& GetVideoSourceConfiguratioOptionsSignal();
  boost::signals2::signal<void(const GetVideoSourceConfigurationsResponse&)>& GetVideoSourceConfigurationsSignal();
  boost::signals2::signal<void(const GetVideoSourceModesResponse&)>& GetVideoSourceModesSignal();
  boost::signals2::signal<void(const GetVideoSourcesResponse&)>& GetVideoSourcesSignal();
  boost::signals2::signal<void(const RemoveAudioDecoderConfigurationResponse&)>& RemoveAudioDecoderConfigurationSignal();
  boost::signals2::signal<void(const RemoveAudioEncoderConfigurationResponse&)>& RemoveAudioEncoderConfigurationSignal();
  boost::signals2::signal<void(const RemoveAudioOutputConfigurationResponse&)>& RemoveAudioOutputConfigurationSignal();
  boost::signals2::signal<void(const RemoveAudioSourceConfigurationResponse&)>& RemoveAudioSourceConfigurationSignal();
  boost::signals2::signal<void(const RemoveMetadataConfigurationResponse&)>& RemoveMetadataConfigurationSignal();
  boost::signals2::signal<void(const RemovePTZConfigurationResponse&)>& RemovePTZConfigurationSignal();
  boost::signals2::signal<void(const RemoveVideoAnalyticsConfigurationResponse&)>& RemoveVideoAnalyticsConfigurationSignal();
  boost::signals2::signal<void(const RemoveVideoEncoderConfigurationResponse&)>& RemoveVideoEncoderConfigurationSignal();
  boost::signals2::signal<void(const RemoveVideoSourceConfigurationResponse&)>& RemoveVideoSourceConfigurationSignal();
  boost::signals2::signal<void(const SetAudioDecoderConfigurationResponse&)>& SetAudioDecoderConfigurationSignal();
  boost::signals2::signal<void(const SetAudioEncoderConfigurationResponse&)>& SetAudioEncoderConfigurationSignal();
  boost::signals2::signal<void(const SetAudioOutputConfigurationResponse&)>& SetAudioOutputConfigurationSignal();
  boost::signals2::signal<void(const SetAudioSourceConfigurationResponse&)>& SetAudioSourceConfigurationSignal();
  boost::signals2::signal<void(const SetMetadataConfigurationResponse&)>& SetMetadataConfigurationSignal();
  boost::signals2::signal<void(const SetSynchronizationPointResponse&)>& SetSynchronizationPointSignal();
  boost::signals2::signal<void(const SetVideoEncoderConfigurationResponse&)>& SetVideoEncoderConfigurationSignal();
  boost::signals2::signal<void(const SetVideoSourceConfigurationResponse&)>& SetVideoSourceConfigurationSignal();
  boost::signals2::signal<void(const StartMulticastStreamingResponse&)>& GetStartMulticastStreamingSignal();
  boost::signals2::signal<void(const StopMulticastStreamingResponse&)>& GetStopMulticastStreamingSignal();

 private:

  virtual void Update(MEDIAOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata) override;
  virtual void SignalError(MEDIAOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) override;

  // Get the body of the request string
  std::string AddAudioDecoderConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken);
  std::string AddAudioEncoderConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken);
  std::string AddAudioOutputConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken);
  std::string AddAudioSourceConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken);
  std::string AddMetadataConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken);
  std::string AddPTZConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken);
  std::string AddVideoAnalyticsConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken);
  std::string AddVideoEncoderConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken);
  std::string AddVideoSourceConfigurationBody(const std::string& profiletoken, const std::string& configurationtoken);
  std::string CreateProfileBody(const std::string& name, const boost::optional<std::string>& token);
  std::string DeleteProfileBody(const std::string& name);
  std::string GetAudioDecoderConfigurationBody(const std::string& configurationtoken);
  std::string GetAudioDecoderConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  std::string GetAudioDecoderConfigurationsBody();
  std::string GetAudioEncoderConfigurationBody(const std::string& configurationtoken);
  std::string GetAudioEncoderConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  std::string GetAudioEncoderConfigurationsBody();
  std::string GetAudioOutputConfigurationBody(const std::string& configurationtoken);
  std::string GetAudioOutputConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  std::string GetAudioOutputConfigurationsBody();
  std::string GetAudioSourceConfigurationBody(const std::string& configurationtoken);
  std::string GetAudioSourceConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  std::string GetAudioSourceConfigurationsBody();
  std::string GetAudioSourcesBody();
  std::string GetCompatibleAudioDecoderConfigurationsBody(const std::string& profiletoken);
  std::string GetCompatibleAudioEncoderConfigurationsBody(const std::string& profiletoken);
  std::string GetCompatibleAudioOutputConfigurationsBody(const std::string& profiletoken);
  std::string GetCompatibleAudioSourceConfigurationsBody(const std::string& profiletoken);
  std::string GetCompatibleMetadataConfigurationsBody(const std::string& profiletoken);
  std::string GetCompatibleVideoAnalyticsConfigurationsBody(const std::string& profiletoken);
  std::string GetCompatibleVideoEncoderConfigurationsBody(const std::string& profiletoken);
  std::string GetCompatibleVideoSourceConfigurationsBody(const std::string& profiletoken);
  std::string GetMetadataConfigurationBody(const std::string& configurationtoken);
  std::string GetMetadataConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  std::string GetMetadataConfigurationsBody();
  std::string GetProfileBody(const std::string& token);
  std::string GetProfilesBody();
  std::string GetServiceCapabilitiesBody();
  std::string GetSnapshotUriBody(const std::string& token);
  std::string GetStreamUriBody(const StreamSetup& streamsetup, const std::string& token);
  std::string GetVideoAnalyticsConfigurationsBody();
  std::string GetVideoEncoderConfigurationBody(const std::string& token);
  std::string GetVideoEncoderConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  std::string GetVideoEncoderConfigurationsBody();
  std::string GetVideoSourceConfigurationBody(const std::string& token);
  std::string GetVideoSourceConfigurationOptionsBody(const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken);
  std::string GetVideoSourceConfigurationsBody();
  std::string GetVideoSourceModesBody(const std::string& token);
  std::string GetVideoSourcesBody();
  std::string RemoveAudioDecoderConfigurationBody(const std::string& profiletoken);
  std::string RemoveAudioEncoderConfigurationBody(const std::string& profiletoken);
  std::string RemoveAudioOutputConfigurationBody(const std::string& profiletoken);
  std::string RemoveAudioSourceConfigurationBody(const std::string& profiletoken);
  std::string RemoveMetadataConfigurationBody(const std::string& profiletoken);
  std::string RemovePTZConfigurationBody(const std::string& profiletoken);
  std::string RemoveVideoAnalyticsConfigurationBody(const std::string& profiletoken);
  std::string RemoveVideoEncoderConfigurationBody(const std::string& profiletoken);
  std::string RemoveVideoSourceConfigurationBody(const std::string& profiletoken);
  std::string SetAudioDecoderConfigurationBody(const AudioDecoderConfiguration& configuration);
  std::string SetAudioEncoderConfigurationBody(const AudioEncoderConfiguration& configuration);
  std::string SetAudioOutputConfigurationBody(const AudioOutputConfiguration& configuration);
  std::string SetAudioSourceConfigurationBody(const AudioSourceConfiguration& configuration);
  std::string SetMetadataConfigurationBody(const MetadataConfiguration& configuration);
  std::string SetSynchronizationPointBody(const std::string& profiletoken);
  std::string SetVideoEncoderConfigurationBody(const VideoEncoderConfiguration& videoencoderconfiguration);
  std::string SetVideoSourceConfigurationBody(const VideoSourceConfiguration& videosourceconfiguration);
  std::string StartMulticastStreamingBody(const std::string& profiletoken);
  std::string StopMulticastStreamingBody(const std::string& profiletoken);

  // Signals
  MediaSignals* signals_;

};

}

}

#endif
