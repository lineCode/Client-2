// dummymediaservice.hpp
//

#ifndef IDA7PIJT5GRWI5HHQ74QM8K22WKUB6IW91
#define IDA7PIJT5GRWI5HHQ74QM8K22WKUB6IW91

///// Includes /////

#include <onvifserver/onvifserver.hpp>
#include <onviftypes/onviftypes.hpp>
#include <vector>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class DummyMediaService : public server::Service
{
 public:

   DummyMediaService();
  virtual ~DummyMediaService();

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const override { return false; }
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& path, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientcnonce, const std::string& clientresponse, const std::string& clientqop) const override { return false; }

  virtual boost::shared_future<server::ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) override;
  
  inline void SetCreateProfile(const Profile& createprofile) { createprofile_ = createprofile; }
  inline void SetGetAudioDecoderConfiguration(const AudioDecoderConfiguration& getaudiodecoderconfiguration) { getaudiodecoderconfiguration_ = getaudiodecoderconfiguration; }
  inline void SetGetAudioDecoderConfigurationOptions(const AudioDecoderConfigurationOptions& getaudiodecoderconfigurationoptions) { getaudiodecoderconfigurationoptions_ = getaudiodecoderconfigurationoptions; }
  inline void SetGetAudioDecoderConfigurations(const std::vector<AudioDecoderConfiguration>& getaudiodecoderconfigurations) { getaudiodecoderconfigurations_ = getaudiodecoderconfigurations; }
  inline void SetGetAudioEncoderConfiguration(const AudioEncoderConfiguration& getaudioencoderconfiguration) { getaudioencoderconfiguration_ = getaudioencoderconfiguration; }
  inline void SetGetAudioEncoderConfigurationOptions(const AudioEncoderConfigurationOptions& getaudioencoderconfigurationoptions) { getaudioencoderconfigurationoptions_ = getaudioencoderconfigurationoptions; }
  inline void SetGetAudioEncoderConfigurations(const std::vector<AudioEncoderConfiguration>& getaudioencoderconfigurations) { getaudioencoderconfigurations_ = getaudioencoderconfigurations; }
  inline void SetGetAudioOutputConfiguration(const AudioOutputConfiguration& getaudiooutputconfiguration) { getaudiooutputconfiguration_ = getaudiooutputconfiguration; }
  inline void SetGetAudioOutputConfigurationOptions(const AudioOutputConfigurationOptions& getaudiooutputconfigurationoptions) { getaudiooutputconfigurationoptions_ = getaudiooutputconfigurationoptions; }
  inline void SetGetAudioOutputConfigurations(const std::vector<AudioOutputConfiguration>& getaudiooutputconfigurations) { getaudiooutputconfigurations_ = getaudiooutputconfigurations; }
  inline void SetGetAudioSourceConfiguration(const AudioSourceConfiguration& getaudiosourceconfiguration) { getaudiosourceconfiguration_ = getaudiosourceconfiguration; }
  inline void SetGetAudioSourceConfigurationOptions(const AudioSourceConfigurationOptions& getaudiosourceconfigurationoptions) { getaudiosourceconfigurationoptions_ = getaudiosourceconfigurationoptions; }
  inline void SetGetAudioSourceConfigurations(const std::vector<AudioSourceConfiguration>& getaudiosourceconfigurations) { getaudiosourceconfigurations_ = getaudiosourceconfigurations; }
  inline void SetGetAudioSources(const std::vector<AudioSource>& getaudiosources) { getaudiosources_ = getaudiosources; }
  inline void SetGetCompatibleAudioDecoderConfigurations(const std::vector<AudioDecoderConfiguration>& getcompatibleaudiodecoderconfigurations) { getcompatibleaudiodecoderconfigurations_ = getcompatibleaudiodecoderconfigurations; }
  inline void SetGetCompatibleAudioEncoderConfigurations(const std::vector<AudioEncoderConfiguration>& getcompatibleaudioencoderconfigurations) { getcompatibleaudioencoderconfigurations_ = getcompatibleaudioencoderconfigurations; }
  inline void SetGetCompatibleAudioOutputConfigurations(const std::vector<AudioOutputConfiguration>& getcompatibleaudiooutputconfigurations) { getcompatibleaudiooutputconfigurations_ = getcompatibleaudiooutputconfigurations; }
  inline void SetGetCompatibleAudioSourceConfigurations(const std::vector<AudioSourceConfiguration>& getcompatibleaudiosourceconfigurations) { getcompatibleaudiosourceconfigurations_ = getcompatibleaudiosourceconfigurations; }
  inline void SetGetCompatibleMetadataConfigurations(const std::vector<MetadataConfiguration>& getcompatiblemetadataconfigurations) { getcompatiblemetadataconfigurations_ = getcompatiblemetadataconfigurations; }
  inline void SetGetCompatibleVideoAnalyticsConfigurations(const std::vector<VideoAnalyticsConfiguration>& getcompatiblevideoanalyticsconfigurations) { getcompatiblevideoanalyticsconfigurations_ = getcompatiblevideoanalyticsconfigurations; }
  inline void SetGetCompatibleVideoEncoderConfigurations(const std::vector<VideoEncoderConfiguration>& getcompatiblevideoencoderconfigurations) { getcompatiblevideoencoderconfigurations_ = getcompatiblevideoencoderconfigurations; }
  inline void SetGetCompatibleVideoSourceConfigurations(const std::vector<VideoSourceConfiguration>& getcompatiblevideosourceconfigurations) { getcompatiblevideosourceconfigurations_ = getcompatiblevideosourceconfigurations; }
  inline void SetGetMetadataConfiguration(const MetadataConfiguration& getmetadataconfiguration) { getmetadataconfiguration_ = getmetadataconfiguration; }
  inline void SetGetMetadataConfigurationOptions(const MetadataConfigurationOptions& getmetadataconfigurationoptions) { getmetadataconfigurationoptions_ = getmetadataconfigurationoptions; }
  inline void SetGetMetadataConfigurations(const std::vector<MetadataConfiguration>& getmetadataconfigurations) { getmetadataconfigurations_ = getmetadataconfigurations; }
  inline void SetGetProfile(const Profile& getprofile) { getprofile_ = getprofile; }
  inline void SetGetProfiles(const std::vector<Profile>& getprofiles) { getprofiles_ = getprofiles; }
  inline void SetGetServiceCapabilities(const media::Capabilities& getservicecapabilities) { getservicecapabilities_ = getservicecapabilities; }
  inline void SetGetSnapshotUri(const MediaUri& getsnapshoturi) { getsnapshoturi_ = getsnapshoturi; }
  inline void SetGetStreamUri(const MediaUri& getstreamuri) { getstreamuri_ = getstreamuri; }
  inline void SetGetVideoAnalyticsConfigurations(const std::vector<VideoAnalyticsConfiguration>& getvideoanalyticsconfigurations) { getvideoanalyticsconfigurations_ = getvideoanalyticsconfigurations; }
  inline void SetGetVideoEncoderConfiguration(const VideoEncoderConfiguration& getvideoencoderconfiguration) { getvideoencoderconfiguration_ = getvideoencoderconfiguration; }
  inline void SetGetVideoEncoderConfigurationOptions(const VideoEncoderConfigurationOptions& getvideoencoderconfigurationoptions) { getvideoencoderconfigurationoptions_ = getvideoencoderconfigurationoptions; }
  inline void SetGetVideoEncoderConfigurations(const std::vector<VideoEncoderConfiguration>& getvideoencoderconfigurations) { getvideoencoderconfigurations_ = getvideoencoderconfigurations; }
  inline void SetGetVideoSourceConfiguration(const VideoSourceConfiguration& getvideosourceconfiguration) { getvideosourceconfiguration_ = getvideosourceconfiguration; }
  inline void SetGetVideoSourceConfigurationOptions(const VideoSourceConfigurationOptions& getvideosourceconfigurationoptions) { getvideosourceconfigurationoptions_ = getvideosourceconfigurationoptions; }
  inline void SetGetVideoSourceConfigurations(const std::vector<VideoSourceConfiguration>& getvideosourceconfigurations) { getvideosourceconfigurations_ = getvideosourceconfigurations; }
  inline void SetGetVideoSourceModes(const std::vector<media::VideoSourceMode>& getvideosourcemodes) { getvideosourcemodes_ = getvideosourcemodes; }
  inline void SetGetVideoSources(const std::vector<VideoSource>& getvideosources) { getvideosources_ = getvideosources; }

 private:

  Profile createprofile_;
  AudioDecoderConfiguration getaudiodecoderconfiguration_;
  AudioDecoderConfigurationOptions getaudiodecoderconfigurationoptions_;
  std::vector<AudioDecoderConfiguration> getaudiodecoderconfigurations_;
  AudioEncoderConfiguration getaudioencoderconfiguration_;
  AudioEncoderConfigurationOptions getaudioencoderconfigurationoptions_;
  std::vector<AudioEncoderConfiguration> getaudioencoderconfigurations_;
  AudioOutputConfiguration getaudiooutputconfiguration_;
  AudioOutputConfigurationOptions getaudiooutputconfigurationoptions_;
  std::vector<AudioOutputConfiguration> getaudiooutputconfigurations_;
  AudioSourceConfiguration getaudiosourceconfiguration_;
  AudioSourceConfigurationOptions getaudiosourceconfigurationoptions_;
  std::vector<AudioSourceConfiguration> getaudiosourceconfigurations_;
  std::vector<AudioSource> getaudiosources_;
  std::vector<AudioDecoderConfiguration> getcompatibleaudiodecoderconfigurations_;
  std::vector<AudioEncoderConfiguration> getcompatibleaudioencoderconfigurations_;
  std::vector<AudioOutputConfiguration> getcompatibleaudiooutputconfigurations_;
  std::vector<AudioSourceConfiguration> getcompatibleaudiosourceconfigurations_;
  std::vector<MetadataConfiguration> getcompatiblemetadataconfigurations_;
  std::vector<VideoAnalyticsConfiguration> getcompatiblevideoanalyticsconfigurations_;
  std::vector<VideoEncoderConfiguration> getcompatiblevideoencoderconfigurations_;
  std::vector<VideoSourceConfiguration> getcompatiblevideosourceconfigurations_;
  MetadataConfiguration getmetadataconfiguration_;
  MetadataConfigurationOptions getmetadataconfigurationoptions_;
  std::vector<MetadataConfiguration> getmetadataconfigurations_;
  Profile getprofile_;
  std::vector<Profile> getprofiles_;
  MediaUri getstreamuri_;
  media::Capabilities getservicecapabilities_;
  MediaUri getsnapshoturi_;
  std::vector<VideoAnalyticsConfiguration> getvideoanalyticsconfigurations_;
  VideoEncoderConfiguration getvideoencoderconfiguration_;
  VideoEncoderConfigurationOptions getvideoencoderconfigurationoptions_;
  std::vector<VideoEncoderConfiguration> getvideoencoderconfigurations_;
  VideoSourceConfiguration getvideosourceconfiguration_;
  VideoSourceConfigurationOptions getvideosourceconfigurationoptions_;
  std::vector<VideoSourceConfiguration> getvideosourceconfigurations_;
  std::vector<media::VideoSourceMode> getvideosourcemodes_;
  std::vector<VideoSource> getvideosources_;

};

}

}

#endif
