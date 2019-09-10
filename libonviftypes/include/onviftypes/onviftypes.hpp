// types.hpp
//

#ifndef IDQ45S4BRMUR6H9DNFU1UFGP6AXB1FR3YC
#define IDQ45S4BRMUR6H9DNFU1UFGP6AXB1FR3YC

///// Includes /////

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <boost/date_time.hpp>
#include <pugixml.hpp>
#include <utility/utility.hpp>
#include <vector>

///// Defines /////

#ifdef _WIN32
  #undef DeviceCapabilities
#endif

///// Namespaces /////

namespace onvif
{

///// Globals /////

const std::string RECEIVERURI("http://www.onvif.org/ver10/schema/Receiver");

///// Enumerations /////

namespace deviceio
{

enum SERIALPORTTYPE
{
  SERIALPORTTYPE_RS232,
  SERIALPORTTYPE_RS422HALFDUPLEX,
  SERIALPORTTYPE_RS422FULLDUPLEX,
  SERIALPORTTYPE_RS485HALFDUPLEX,
  SERIALPORTTYPE_RS485FULLDUPLEX,
  SERIALPORTTYPE_GENERIC
};

enum PARITYBIT
{
  PARITYBIT_NONE,
  PARITYBIT_EVEN,
  PARITYBIT_ODD,
  PARITYBIT_MARK,
  PARITYBIT_SPACE,
  PARITYBIT_EXTENDED
};

}

enum RECORDINGSTATUS
{
  RECORDINGSTATUS_INITIATED,
  RECORDINGSTATUS_RECORDING,
  RECORDINGSTATUS_STOPPED,
  RECORDINGSTATUS_REMOVING,
  RECORDINGSTATUS_REMOVED,
  RECORDINGSTATUS_UNKNOWN
};

enum TONECOMPENSATIONMODE
{
  TONECOMPENSATIONMODE_OFF,
  TONECOMPENSATIONMODE_ON,
  TONECOMPENSATIONMODE_AUTO
};

enum SYSTEMLOGTYPE
{
  SYSTEMLOGLEVEL_SYSTEM,
  SYSTEMLOGLEVEL_ACCESS
};

enum STREAMTYPE
{
  STREAM_RTPUNICAST,
  STREAM_RTPMULTICAST
};

enum SEARCHSTATE
{
  SEARCHSTATE_QUEUED,
  SEARCHSTATE_SEARCHING,
  SEARCHSTATE_COMPLETED,
  SEARCHSTATE_UNKNOWN
};

enum SCOPEDEFINITION
{
  SCOPEDEFINITION_FIXED,
  SCOPEDEFINITION_CONFIGURABLE
};

enum ROTATEMODE
{
  ROTATE_OFF,
  ROTATE_ON,
  ROTATE_AUTO
};

enum REVERSEMODE
{
  REVERSEMODE_OFF,
  REVERSEMODE_ON,
  REVERSEMODE_AUTO,
  REVERSEMODE_EXTENDED
};

enum RELAYMODE
{
  RELAYMODE_BISTABLE,
  RELAYMODE_MONOSTABLE
};

enum RELAYLOGICALSTATE
{
  RELAYLOGICALSTATE_ACTIVE,
  RELAYLOGICALSTATE_INACTIVE
};

enum RELAYIDLESTATE
{
  RELAYIDLESTATE_CLOSED,
  RELAYIDLESTATE_OPEN
};

enum PTZPRESETTOUROPERATION
{
  PTZPRESETTOUROPERATION_START,
  PTZPRESETTOUROPERATION_STOP,
  PTZPRESETTOUROPERATION_PAUSE,
  PTZPRESETTOUROPERATION_EXTENDED
};

enum NETWORKPROTOCOLTYPE
{
  NETWORKPROTOCOLTYPE_HTTP,
  NETWORKPROTOCOLTYPE_HTTPS,
  NETWORKPROTOCOLTYPE_RTSP
};

enum NETWORKHOSTTYPE
{
  NETWORKHOSTTYPE_IPV4,
  NETWORKHOSTTYPE_IPV6,
  NETWORKHOSTTYPE_DNS
};

enum MPEG4PROFILE
{
  MPEG4PROFILE_SP,
  MPEG4PROFILE_ASP
};

enum MOVESTATUS
{
  MOVESTATUS_IDLE,
  MOVESTATUS_MOVING,
  MOVESTATUS_UNKNOWN
};

enum METADATACOMPRESSIONTYPE
{
  METADATACOMPRESSIONTYPE_NONE,
  METADATACOMPRESSIONTYPE_GZIP,
  METADATACOMPRESSIONTYPE_EXI
};

enum IRCUTFILTERMODE
{
  IRCUTFILTERMODE_ON,
  IRCUTFILTERMODE_OFF,
  IRCUTFILTERMODE_AUTO
};

enum IRCUTFILTERAUTOBOUNDARYTYPE
{
  IRCUTFILTERAUTOBOUNDARYTYPE_COMMON,
  IRCUTFILTERAUTOBOUNDARYTYPE_TOON,
  IRCUTFILTERAUTOBOUNDARYTYPE_TOOFF,
  IRCUTFILTERAUTOBOUNDARYTYPE_EXTENDED
};

enum IPV6DHCPCONFIGURATION
{
  IPV6DHCPCONFIGURATION_AUTO,
  IPV6DHCPCONFIGURATION_STATEFUL,
  IPV6DHCPCONFIGURATION_STATELESS,
  IPV6DHCPCONFIGURATION_OFF
};

enum IPTYPE
{
  IPTYPE_IPV4,
  IPTYPE_IPV6
};

enum IPADDRESSFILTERTYPE
{
  IPADDRESSFILTERTYPE_ALLOW,
  IPADDRESSFILTERTYPE_DENY
};

enum IMAGESTABILIZATIONMODE
{
  IMAGESTABILIZATIONMODE_OFF,
  IMAGESTABILIZATIONMODE_ON,
  IMAGESTABILIZATIONMODE_AUTO,
  IMAGESTABILIZATIONMODE_EXTENDED
};

enum H264PROFILE
{
  H264PROFILE_BASELINE,
  H264PROFILE_MAIN,
  H264PROFILE_EXTENDED,
  H264PROFILE_HIGH
};

enum FACTORYDEFAULTTYPE
{
  FACTORYDEFAULTTYPE_SOFT,
  FACTORYDEFAULTTYPE_HARD
};

enum EXPOSUREPRIORITY
{
  EXPOSUREPRIORITY_LOWNOISE,
  EXPOSUREPRIORITY_FRAMERATE
};

enum EXPOSUREMODE
{
  EXPOSUREMODE_AUTO,
  EXPOSUREMODE_MANUAL
};

enum EFLIPMODE
{
  EFLIPMODE_OFF,
  EFLIPMODE_ON,
  EFLIPMODE_EXTENDED
};

enum DYNAMICDNSTYPE
{
  DYNAMICDNSTYPE_NOUPDATE,
  DYNAMICDNSTYPE_CLIENTUPDATES,
  DYNAMICDNSTYPE_SERVERUPDATES
};

enum DUPLEX
{
  DUPLEX_FULL,
  DUPLEX_HALF
};

enum DOT11STATIONMODE
{
  DOT11STATIONMODE_ADHOC,
  DOT11STATIONMODE_INFRASTRUCTURE,
  DOT11STATIONMODE_EXTENDED
};

enum DOT11SIGNALSTRENGTH
{
  DOT11SIGNALSTRENGTH_NONE,
  DOT11SIGNALSTRENGTH_VERYBAD,
  DOT11SIGNALSTRENGTH_BAD,
  DOT11SIGNALSTRENGTH_GOOD,
  DOT11SIGNALSTRENGTH_VERYGOOD,
  DOT11SIGNALSTRENGTH_EXTENDED
};

enum DOT11SECURITYMODE
{
  DOT11SECURITYMODE_NONE,
  DOT11SECURITYMODE_WEP,
  DOT11SECURITYMODE_PSK,
  DOT11SECURITYMODE_DOT1X,
  DOT11SECURITYMODE_EXTENDED
};

enum DOT11CIPHER
{
  DOT11CIPHER_CCMP,
  DOT11CIPHER_TKIP,
  DOT11CIPHER_ANY,
  DOT11CIPHER_EXTENDED
};

enum DOT11AUTHANDMANAGEMENTSUITE
{
  DOT11AUTHANDMANAGEMENTSUITE_NONE,
  DOT11AUTHANDMANAGEMENTSUITE_DOT1X,
  DOT11AUTHANDMANAGEMENTSUITE_PSK,
  DOT11AUTHANDMANAGEMENTSUITE_EXTENDED
};

enum DISCOVERYMODE
{
  DISCOVERYMODE_DISCOVERABLE,
  DISCOVERYMODE_NONDISCOVERABLE
};

enum DIGITALIDLESTATE
{
  DIGITALIDLESTATE_CLOSED,
  DIGITALIDLESTATE_OPEN
};

enum DEFOGGINGMODE
{
  DEFOGGINGMODE_OFF,
  DEFOGGINGMODE_ON,
  DEFOGGINGMODE_AUTO
};

enum DATETIMETYPE
{
  DATETIME_MANUAL,
  DATETIME_NTP
};

enum CAPABILITYCATEGORY
{
  CAPABILITYCATEGORY_ALL,
  CAPABILITYCATEGORY_ANALYTICS,
  CAPABILITYCATEGORY_DEVICE,
  CAPABILITYCATEGORY_EVENTS,
  CAPABILITYCATEGORY_IMAGING,
  CAPABILITYCATEGORY_MEDIA,
  CAPABILITYCATEGORY_PTZ
};

enum BACKLIGHTCOMPENSATIONMODE
{
  BACKLIGHTCOMPENSATIONMODE_OFF,
  BACKLIGHTCOMPENSATIONMODE_ON
};

enum AUTOFOCUSMODE
{
  AUTOFOCUSMODE_AUTO,
  AUTOFOCUSMODE_MANUAL
};

enum AUDIOENCODING
{
  AUDIOENCODING_G711,
  AUDIOENCODING_G726,
  AUDIOENCODING_AAC
};

enum WIDEDYNAMICMODE
{
  WIDEDYNAMICMODE_OFF,
  WIDEDYNAMICMODE_ON
};

enum WHITEBALANCEMODE
{
  WHITEBALANCEMODE_AUTO,
  WHITEBALANCEMODE_MANUAL
};

enum VIDEOENCODING
{
  VIDEOENCODING_JPEG,
  VIDEOENCODING_MPEG4,
  VIDEOENCODING_H264
};

enum USERLEVEL
{
  USERLEVEL_EXTENDED,
  USERLEVEL_ANONYMOUS,
  USERLEVEL_USER,
  USERLEVEL_OPERATOR,
  USERLEVEL_ADMINISTRATOR,
};

enum TRANSPORTPROTOCOL
{
  TRANSPORTPROTOCOL_UDP,
  TRANSPORTPROTOCOL_TCP,
  TRANSPORTPROTOCOL_RTSP,
  TRANSPORTPROTOCOL_HTTP
};

enum TRACKTYPE
{
  TRACKTYPE_VIDEO,
  TRACKTYPE_AUDIO,
  TRACKTYPE_METADATA
};


namespace receiver
{
  
enum RECEIVERMODE
{
  RECEIVERMODE_AUTOCONNECT,
  RECEIVERMODE_ALWAYSCONNECT,
  RECEIVERMODE_NEVERCONNECT,
  RECEIVERMODE_UNKNOWN
};

enum RECEIVERSTATE
{
  RECEIVERSTATE_NOTCONNECTED,
  RECEIVERSTATE_CONNECTING,
  RECEIVERSTATE_CONNECTED,
  RECEIVERSTATE_UNKNOWN
};

}

namespace recording
{
  
enum RECORDINGJOBMODE
{
  RECORDINGJOBMODE_IDLE,
  RECORDINGJOBMODE_ACTIVE
};

enum RECORDINGJOBSTATE
{
  RECORDINGJOBSTATE_IDLE,
  RECORDINGJOBSTATE_ACTIVE,
  RECORDINGJOBSTATE_PARTIALLYACTIVE,
  RECORDINGJOBSTATE_ERROR
};

}

///// Classes /////

class Element
{
public:

  Element();
  Element(const std::string& name, const std::vector< std::pair<std::string, std::string> >& attributes, const std::string& text, const std::vector<Element>& elements);
  Element(const pugi::xml_node& node);

  std::string ToXml() const;

  bool operator==(const Element& rhs) const;

  std::string name_;
  std::vector< std::pair<std::string, std::string> > attributes_;
  std::string text_;
  std::vector<Element> elements_;

};

namespace analytics
{

class Capabilities
{
 public:

  Capabilities();
  Capabilities(const boost::optional<bool>& rulesupport, const boost::optional<bool>& analyticsmodulesupport, const boost::optional<bool>& cellbasedscenedescriptionsupported);
  Capabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Capabilities& rhs) const;

  boost::optional<bool> rulesupport_;
  boost::optional<bool> analyticsmodulesupport_;
  boost::optional<bool> cellbasedscenedescriptionsupported_;

};

}

namespace imaging
{

class Capabilities
{
 public:

  Capabilities();
  Capabilities(const boost::optional<bool>& imagestabilization);
  Capabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Capabilities& rhs) const;

  boost::optional<bool> imagestabilization_;

};

}

namespace ptz
{

class Capabilities
{
 public:

  Capabilities();
  Capabilities(const boost::optional<bool>& eflip, const boost::optional<bool>& reverse, const boost::optional<bool>& getcompatibleconfigurations, const boost::optional<bool>& movestatus, const boost::optional<bool>& statusposition);
  Capabilities(const pugi::xml_node& node);

  inline bool SupportsEFlip() const { return (eflip_.is_initialized() && *eflip_); }
  inline bool SupportsReverse() const { return (reverse_.is_initialized() && *reverse_); }
  inline bool SupportsGetCompatibleConfigurations() const { return (getcompatibleconfigurations_.is_initialized() && *getcompatibleconfigurations_); }
  inline bool SupportsMoveStatus() const { return (movestatus_.is_initialized() && *movestatus_); }
  inline bool SupportsStatusPosition() const { return (statusposition_.is_initialized() && *statusposition_); }

  std::string ToXml(const std::string& name) const;

  bool operator==(const Capabilities& rhs) const;

  boost::optional<bool> eflip_;
  boost::optional<bool> reverse_;
  boost::optional<bool> getcompatibleconfigurations_;
  boost::optional<bool> movestatus_;
  boost::optional<bool> statusposition_;

};

}

class ElementItemDescription
{
 public:

  ElementItemDescription();
  ElementItemDescription(const std::string& name, const std::string& type);
  ElementItemDescription(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ElementItemDescription& rhs) const;

  std::string name_;
  std::string type_;

};

class SimpleItemDescription
{
 public:

  SimpleItemDescription();
  SimpleItemDescription(const std::string& name, const std::string& type);
  SimpleItemDescription(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SimpleItemDescription& rhs) const;

  std::string name_;
  std::string type_;

};

class ItemListDescription
{
 public:

  ItemListDescription();
  ItemListDescription(const std::vector<SimpleItemDescription>& simpleitemdescription, const std::vector<ElementItemDescription>& elementitemdescription);
  ItemListDescription(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ItemListDescription& rhs) const;

  std::vector<SimpleItemDescription> simpleitemdescription_;
  std::vector<ElementItemDescription> elementitemdescription_;

};

class MessageDescription
{
 public:

  MessageDescription();
  MessageDescription(bool isproperty, const std::vector<ItemListDescription>& source, const std::vector<ItemListDescription>& key, const std::vector<ItemListDescription>& data);
  MessageDescription(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const MessageDescription& rhs) const;

  bool isproperty_;
  std::vector<ItemListDescription> source_;
  std::vector<ItemListDescription> key_;
  std::vector<ItemListDescription> data_;

};

namespace ws
{

class DateTime
{
 public:

  DateTime();
  DateTime(const boost::posix_time::ptime& ptime);
  DateTime(const std::string& datetime);
  DateTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second, unsigned int milliseconds);
  DateTime(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  boost::optional<boost::posix_time::ptime> GetPTime() const;

  bool operator==(const DateTime& rhs) const;

  std::string datetime_;

};

class DigitalInput
{
 public:

  DigitalInput();
  DigitalInput(const MessageDescription& messagedescription);
  DigitalInput(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const DigitalInput& rhs) const;

  MessageDescription messagedescription_;

};

class Trigger
{
 public:

  Trigger();
  Trigger(const DigitalInput& digitalinput);
  Trigger(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Trigger& rhs) const;

  DigitalInput digitalinput_;

};

class Device
{
 public:

  Device();
  Device(const Trigger& trigger);
  Device(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Device& rhs) const;

  Trigger trigger_;

};

class EndpointReferenceType
{
 public:

  EndpointReferenceType();
  EndpointReferenceType(const std::string& address, const Element& referenceparameters);
  EndpointReferenceType(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const EndpointReferenceType& rhs) const;

  std::string address_;
  Element referenceparameters_;

};

class MessageContent
{
 public:

  MessageContent();
  MessageContent(const boost::optional<std::string>& dialect, const std::string& expression);
  MessageContent(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const MessageContent& rhs) const;

  boost::optional<std::string> dialect_;
  std::string expression_;

};

class TopicExpression
{
 public:

  TopicExpression();
  TopicExpression(const boost::optional<std::string>& dialect, const std::string& expression);
  TopicExpression(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const TopicExpression& rhs) const;

  boost::optional<std::string> dialect_;
  std::string expression_;

};

class TopicSet
{
 public:

  TopicSet();
  TopicSet(const Device& device);
  TopicSet(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const TopicSet& rhs) const;

  Device device_;

};

class FilterType
{
 public:

  FilterType();
  FilterType(const std::vector<TopicExpression>& topicexpression, const std::vector<MessageContent>& messagecontent);
  FilterType(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const FilterType& rhs) const;

  std::vector<TopicExpression> topicexpression_;
  std::vector<MessageContent> messagecontent_;

};

}

class IntList
{
 public:

  IntList();
  IntList(const std::vector<int>& items);
  IntList(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IntList& rhs) const;

  std::vector<int> items_;

};

class AacDecOptions
{
 public:

  AacDecOptions();
  AacDecOptions(const boost::optional<IntList>& bitrate, const boost::optional<IntList>& sampleraterange);
  AacDecOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AacDecOptions& rhs) const;

  boost::optional<IntList> bitrate_;
  boost::optional<IntList> sampleraterange_;

};

class AbsoluteFocus
{
 public:

  AbsoluteFocus();
  AbsoluteFocus(const boost::optional<float>& position, const boost::optional<float>& speed);
  AbsoluteFocus(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AbsoluteFocus& rhs) const;

  boost::optional<float> position_;
  boost::optional<float> speed_;

};

class FloatRange
{
 public:

  FloatRange();
  FloatRange(const boost::optional<float>& min, const boost::optional<float>& max);
  FloatRange(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const FloatRange& rhs) const;

  boost::optional<float> min_;
  boost::optional<float> max_;

};

class AbsoluteFocusOptions
{
 public:

  AbsoluteFocusOptions();
  AbsoluteFocusOptions(const boost::optional<FloatRange>& position, const boost::optional<FloatRange>& speed);
  AbsoluteFocusOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AbsoluteFocusOptions& rhs) const;

  boost::optional<FloatRange> position_;
  boost::optional<FloatRange> speed_;

};

class AnalyticsCapabilities
{
 public:

  AnalyticsCapabilities();
  AnalyticsCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& rulesupport, const boost::optional<bool>& analyticsmodulesupport);
  AnalyticsCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AnalyticsCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;
  boost::optional<bool> rulesupport_;
  boost::optional<bool> analyticsmodulesupport_;

};

class AnalyticsDeviceCapabilities
{
 public:

  AnalyticsDeviceCapabilities();
  AnalyticsDeviceCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& rulesupport);
  AnalyticsDeviceCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AnalyticsDeviceCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;
  boost::optional<bool> rulesupport_;

};

class ElementItem
{
 public:

  ElementItem();
  ElementItem(const std::vector<Element>& elements);
  ElementItem(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ElementItem& rhs) const;

  std::vector<Element> elements_;

};

class SimpleItem
{
 public:

  SimpleItem();
  SimpleItem(const boost::optional<std::string>& name, const boost::optional<std::string>& value);
  SimpleItem(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SimpleItem& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<std::string> value_;

};

class BinaryData
{
 public:

  BinaryData();
  BinaryData(const std::vector<unsigned char>& data, const boost::optional<std::string>& contenttype);
  BinaryData(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const BinaryData& rhs) const;

  std::vector<unsigned char> data_;
  boost::optional<std::string> contenttype_;

};

class ItemList
{
 public:

  ItemList();
  ItemList(const std::vector<SimpleItem>& simpleitems, const std::vector<ElementItem>& elementitems);
  ItemList(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ItemList& rhs) const;

  std::vector<SimpleItem> simpleitems_;
  std::vector<ElementItem> elementitems_;

};

class Config
{
 public:

  Config();
  Config(const boost::optional<ItemList>& parameters, const boost::optional<std::string>& name, const boost::optional<std::string>& type);
  Config(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Config& rhs) const;

  boost::optional<ItemList> parameters_;
  boost::optional<std::string> name_;
  boost::optional<std::string> type_;

};

class IntRange
{
 public:

  IntRange();
  IntRange(const boost::optional<int>& min, const boost::optional<int>& max);
  IntRange(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IntRange& rhs) const;

  boost::optional<int> min_;
  boost::optional<int> max_;

};

class AnalyticsEngineConfiguration
{
 public:

  AnalyticsEngineConfiguration();
  AnalyticsEngineConfiguration(const std::vector<Config>& analyticsmodules);
  AnalyticsEngineConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AnalyticsEngineConfiguration& rhs) const;

  std::vector<Config> analyticsmodules_;

};

class Include
{
 public:

  Include();
  Include(const boost::optional<std::string>& href, const std::string& data);
  Include(const boost::optional<std::string>& href, const std::vector<char>& data);
  Include(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Include& rhs) const;

  boost::optional<std::string> href_;
  std::vector<char> data_;

};

class AttachmentData
{
 public:

  AttachmentData();
  AttachmentData(const boost::optional<Include>& include, const boost::optional<std::string>& contenttype);
  AttachmentData(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AttachmentData& rhs) const;

  boost::optional<Include> include_;
  boost::optional<std::string> contenttype_;

};

class Vector1d
{
 public:

  Vector1d();
  Vector1d(const boost::optional<float>& x, const boost::optional<std::string>& space);
  Vector1d(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Vector1d& rhs) const;

  boost::optional<float> x_;
  boost::optional<std::string> space_;

};

class Vector2d
{
 public:

  Vector2d();
  Vector2d(const boost::optional<float>& x, const boost::optional<float>& y, const boost::optional<std::string>& space);
  Vector2d(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Vector2d& rhs) const;

  boost::optional<float> x_;
  boost::optional<float> y_;
  boost::optional<std::string> space_;

};

class AudioDecoderConfiguration
{
 public:

  AudioDecoderConfiguration();
  AudioDecoderConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token);
  AudioDecoderConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AudioDecoderConfiguration& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<int> usecount_;
  boost::optional<std::string> token_;

};

class G711DecOptions
{
 public:

  G711DecOptions();
  G711DecOptions(const boost::optional<IntList>& bitrate, const boost::optional<IntList>& sampleraterange);
  G711DecOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const G711DecOptions& rhs) const;

  boost::optional<IntList> bitrate_;
  boost::optional<IntList> sampleraterange_;

};

class G726DecOptions
{
 public:

  G726DecOptions();
  G726DecOptions(const boost::optional<IntList>& bitrate, const boost::optional<IntList>& sampleraterange);
  G726DecOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const G726DecOptions& rhs) const;

  boost::optional<IntList> bitrate_;
  boost::optional<IntList> sampleraterange_;

};

class FloatList
{
 public:

  FloatList();
  FloatList(const std::vector<float>& items);
  FloatList(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const FloatList& rhs) const;

  std::vector<float> items_;

};

class FloatAttrList
{
 public:

  FloatAttrList();
  FloatAttrList(const std::vector<float>& items);
  FloatAttrList(const std::string& items);

  std::string ToString() const;

  bool operator==(const FloatAttrList& rhs) const;

  std::vector<float> items_;

};

class AudioDecoderConfigurationOptions
{
 public:

  AudioDecoderConfigurationOptions();
  AudioDecoderConfigurationOptions(const boost::optional<AacDecOptions>& aacdecoptions, const boost::optional<G711DecOptions>& g711decoptions, const boost::optional<G726DecOptions>& g726decoptions);
  AudioDecoderConfigurationOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AudioDecoderConfigurationOptions& rhs) const;

  boost::optional<AacDecOptions> aacdecoptions_;
  boost::optional<G711DecOptions> g711decoptions_;
  boost::optional<G726DecOptions> g726decoptions_;

};

class Duration
{
 public:

  Duration();
  Duration(const int64_t seconds);
  Duration(const int64_t years, const int64_t months, const int64_t days, const int64_t hours, const int64_t minutes, const int64_t seconds);
  Duration(const int64_t years, const int64_t months, const int64_t days, const int64_t hours, const int64_t minutes, const int64_t seconds, const int64_t milliseconds);

  int64_t ToMilliseconds() const;

  std::string ToString() const;

  bool operator==(const Duration& rhs) const;

  int64_t years_;
  int64_t months_;
  int64_t days_;
  
  int64_t hours_;
  int64_t minutes_;
  int64_t seconds_;
  int64_t milliseconds_;

};

class DurationRange
{
 public:

  DurationRange();
  DurationRange(const boost::optional<Duration>& min, const boost::optional<Duration>& max);
  DurationRange(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const DurationRange& rhs) const;

  boost::optional<Duration> min_;
  boost::optional<Duration> max_;

};

class IPAddress
{
 public:

  IPAddress();
  IPAddress(const boost::optional<IPTYPE>& type, const boost::optional<std::string>& ipv4address, const boost::optional<std::string>& ipv6address);
  IPAddress(const pugi::xml_node& node);

  std::string GetAddress() const;
  bool IsValid() const;

  std::string ToXml(const std::string& name) const;

  bool operator==(const IPAddress& rhs) const;

  boost::optional<IPTYPE> type_;
  boost::optional<std::string> ipv4address_;
  boost::optional<std::string> ipv6address_;

};

class MulticastConfiguration
{
 public:

  MulticastConfiguration();
  MulticastConfiguration(const boost::optional<IPAddress>& address, const boost::optional<int>& port, const boost::optional<int>& ttl, const boost::optional<bool>& autostart);
  MulticastConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const MulticastConfiguration& rhs) const;

  boost::optional<IPAddress> address_;
  boost::optional<int> port_;
  boost::optional<int> ttl_;
  boost::optional<bool> autostart_;

};

class AudioEncoderConfiguration
{
 public:

  AudioEncoderConfiguration();
  AudioEncoderConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<AUDIOENCODING>& encoding, const boost::optional<int>& bitrate, const boost::optional<int>& samplerate, const boost::optional<MulticastConfiguration>& multicast, const boost::optional<Duration>& sessiontimeout);
  AudioEncoderConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AudioEncoderConfiguration& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<int> usecount_;
  boost::optional<std::string> token_;
  boost::optional<AUDIOENCODING> encoding_;
  boost::optional<int> bitrate_;
  boost::optional<int> samplerate_;
  boost::optional<MulticastConfiguration> multicast_;
  boost::optional<Duration> sessiontimeout_;

};

class ToneCompensationOptions
{
 public:

  ToneCompensationOptions();
  ToneCompensationOptions(const std::vector<TONECOMPENSATIONMODE>& mode, const boost::optional<bool>& level);
  ToneCompensationOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ToneCompensationOptions& rhs) const;

  std::vector<TONECOMPENSATIONMODE> mode_;
  boost::optional<bool> level_;

};

class ToneCompensation
{
 public:

  ToneCompensation();
  ToneCompensation(const boost::optional<TONECOMPENSATIONMODE>& mode, const boost::optional<float>& level);
  ToneCompensation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ToneCompensation& rhs) const;

  boost::optional<TONECOMPENSATIONMODE> mode_;
  boost::optional<float> level_;

};

class TLSConfiguration
{
 public:

  TLSConfiguration();
  TLSConfiguration(const boost::optional<std::string>& certificateid);
  TLSConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const TLSConfiguration& rhs) const;

  boost::optional<std::string> certificateid_;

};

class TimeZone
{
 public:

  TimeZone();
  TimeZone(const boost::optional<std::string>& tz);
  TimeZone(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  boost::optional<boost::local_time::posix_time_zone> GetTimeZone() const;

  bool operator==(const TimeZone& rhs) const;

  boost::optional<std::string> tz_;

};

class Time
{
 public:

  Time();
  Time(const boost::optional<int64_t>& hour, const boost::optional<int64_t>& minute, const boost::optional<int64_t>& second);
  Time(const boost::posix_time::time_duration& time);
  Time(const pugi::xml_node& node);

  inline bool IsInitialized() const { return (hour_.is_initialized() && minute_.is_initialized() && second_.is_initialized()); }

  std::string ToXml(const std::string& name) const;

  boost::optional<boost::posix_time::time_duration> GetTime() const;

  bool operator==(const Time& rhs) const;

  boost::optional<int64_t> hour_;
  boost::optional<int64_t> minute_;
  boost::optional<int64_t> second_;

};

class SystemLogUri
{
 public:

  SystemLogUri();
  SystemLogUri(const boost::optional<SYSTEMLOGTYPE>& type, const boost::optional<std::string>& uri);
  SystemLogUri(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SystemLogUri& rhs) const;

  boost::optional<SYSTEMLOGTYPE> type_;
  boost::optional<std::string> uri_;

};

class SystemLogUriList
{
 public:

  SystemLogUriList();
  SystemLogUriList(const std::vector<SystemLogUri>& ntpfromdhcp);
  SystemLogUriList(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SystemLogUriList& rhs) const;

  std::vector<SystemLogUri> systemloguris_;

};

class BackupFile
{
 public:

  BackupFile();
  BackupFile(const boost::optional<std::string>& name, const boost::optional<AttachmentData>& data);
  BackupFile(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const BackupFile& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<AttachmentData> data_;

};

class SystemLog
{
 public:

  SystemLog();
  SystemLog(const boost::optional<AttachmentData>& binary, const boost::optional<std::string>& string);
  SystemLog(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SystemLog& rhs) const;

  boost::optional<AttachmentData> binary_;
  boost::optional<std::string> string_;

};

class Date
{
 public:

  Date();
  Date(const boost::optional<int>& year, const boost::optional<int>& month, const boost::optional<int>& day);
  Date(const boost::gregorian::date& date);
  Date(const pugi::xml_node& node);

  inline bool IsInitialized() const { return (year_.is_initialized() && month_.is_initialized() && day_.is_initialized()); }

  std::string ToXml(const std::string& name) const;

  boost::optional<boost::gregorian::date> GetDate() const;

  bool operator==(const Date& rhs) const;

  boost::optional<int> year_;
  boost::optional<int> month_;
  boost::optional<int> day_;

};

class DateTime
{
 public:

  DateTime();
  DateTime(const boost::optional<Time>& time, const boost::optional<Date>& date);
  DateTime(const boost::posix_time::ptime& time);
  DateTime(const pugi::xml_node& node);

  inline bool IsInitialized() const { return (date_.is_initialized() && time_.is_initialized() && date_->IsInitialized() && time_->IsInitialized()); }

  std::string ToXml(const std::string& name) const;

  boost::optional<boost::posix_time::ptime> GetDateTime() const;
  int64_t GetOffset() const; // Get the offset from local UTC time in milliseconds

  bool operator==(const DateTime& rhs) const;

  boost::optional<Time> time_;
  boost::optional<Date> date_;

};

class SystemDateTime
{
 public:

  SystemDateTime();
  SystemDateTime(const boost::optional<DATETIMETYPE>& datetimetype, const boost::optional<bool>& daylightsavings, const boost::optional<TimeZone>& timezone, const boost::optional<DateTime>& utctime, const boost::optional<DateTime>& localtime);
  SystemDateTime(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  boost::optional<boost::posix_time::ptime> GetUtcDateTime() const { return (utctime_.is_initialized() ? utctime_->GetDateTime() : boost::none); }
  boost::optional<boost::posix_time::ptime> GetLocalDateTime() const { return (localtime_.is_initialized() ? localtime_->GetDateTime() : boost::none); }

  int64_t GetUtcOffset() const { return (utctime_.is_initialized() ? utctime_->GetOffset() : 0); }

  bool operator==(const SystemDateTime& rhs) const;

  boost::optional<DATETIMETYPE> datetimetype_;
  boost::optional<bool> daylightsavings_;
  boost::optional<TimeZone> timezone_;
  boost::optional<DateTime> utctime_;
  boost::optional<DateTime> localtime_;

};

class OnvifVersion
{
 public:

  OnvifVersion();
  OnvifVersion(int major, int minor);
  OnvifVersion(const pugi::xml_node& node);

  utility::Version GetVersion() const;

  std::string ToXml(const std::string& name) const;

  bool operator==(const OnvifVersion& rhs) const;

  boost::optional<int> major_;
  boost::optional<int> minor_;

};

class SystemCapabilities
{
 public:

  SystemCapabilities();
  SystemCapabilities(const boost::optional<bool>& discoveryresolve, const boost::optional<bool>& discoverybye, const boost::optional<bool>& remotediscovery, const boost::optional<bool>& systembackup, const boost::optional<bool>& systemlogging, const boost::optional<bool>& firmwareupgrade, const std::vector<OnvifVersion>& supportedversions, const boost::optional<bool>& httpfirmwareupgrade, const boost::optional<bool>& httpsystembackup, const boost::optional<bool>& httpsystemlogging, const boost::optional<bool>& httpsupportinformation);
  SystemCapabilities(const pugi::xml_node& node);

  inline bool SupportsDiscoveryResolve() const { return (discoveryresolve_.is_initialized() && (*discoveryresolve_ == true)); }
  inline bool SupportsDiscoveryBye() const { return (discoverybye_.is_initialized() && (*discoverybye_ == true)); }
  inline bool SupportsRemoteDiscovery() const { return (remotediscovery_.is_initialized() && (*remotediscovery_ == true)); }
  inline bool SupportsSystemBackup() const { return (systembackup_.is_initialized() && (*systembackup_ == true)); }
  inline bool SupportsSystemLogging() const { return (systemlogging_.is_initialized() && (*systemlogging_ == true)); }
  inline bool SupportsFirmwareUpgrade() const { return (firmwareupgrade_.is_initialized() && (*firmwareupgrade_ == true)); }
  inline const std::vector<OnvifVersion>& SupportedVersions() const { return supportedversions_; }
  inline bool SupportsHTTPFirmwareUpgrade() const { return (httpfirmwareupgrade_.is_initialized() && (*httpfirmwareupgrade_ == true)); }
  inline bool SupportsHTTPSystemBackup() const { return (httpsystembackup_.is_initialized() && (*httpsystembackup_ == true)); }
  inline bool SupportsHTTPSystemLogging() const { return (httpsystemlogging_.is_initialized() && (*httpsystemlogging_ == true)); }
  inline bool SupportsHTTPSupportInformation() const { return (httpsupportinformation_.is_initialized() && (*httpsupportinformation_ == true)); }

  std::string ToXml(const std::string& name) const;

  bool operator==(const SystemCapabilities& rhs) const;

  boost::optional<bool> discoveryresolve_;
  boost::optional<bool> discoverybye_;
  boost::optional<bool> remotediscovery_;
  boost::optional<bool> systembackup_;
  boost::optional<bool> systemlogging_;
  boost::optional<bool> firmwareupgrade_;
  std::vector<OnvifVersion> supportedversions_;
  boost::optional<bool> httpfirmwareupgrade_;
  boost::optional<bool> httpsystembackup_;
  boost::optional<bool> httpsystemlogging_;
  boost::optional<bool> httpsupportinformation_;

};

class SupportInformation
{
 public:

  SupportInformation();
  SupportInformation(const boost::optional<BinaryData>& binary, const boost::optional<std::string>& string);
  SupportInformation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SupportInformation& rhs) const;

  boost::optional<BinaryData> binary_;
  boost::optional<std::string> string_;

};

class IntAttrList
{
 public:

  IntAttrList();
  IntAttrList(const std::vector<int>& items);
  IntAttrList(const std::string& items);

  std::string ToString() const;

  bool operator==(const IntAttrList& rhs) const;

  std::vector<int> items_;

};

class StringAttrList
{
 public:

  StringAttrList();
  StringAttrList(const std::vector<std::string>& items);
  StringAttrList(const std::string& items);

  std::string ToString() const;

  bool operator==(const StringAttrList& rhs) const;

  std::vector<std::string> items_;

};

class Transport
{
 public:

  Transport();
  Transport(const Transport& transport);
  Transport(TRANSPORTPROTOCOL transportprotocol);
  Transport(TRANSPORTPROTOCOL transportprotocol, const Transport& transport);
  Transport(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  Transport operator=(const Transport& transport);
  bool operator==(const Transport& rhs) const;

  boost::optional<TRANSPORTPROTOCOL> protocol_;
  std::unique_ptr<Transport> tunnel_; // This is a unique_ptr because you can not recursively nest boost::optional

};

class StreamSetup
{
 public:

  StreamSetup();
  StreamSetup(const boost::optional<STREAMTYPE>& stream, const boost::optional<Transport>& transport);
  StreamSetup(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const StreamSetup& rhs) const;

  boost::optional<STREAMTYPE> stream_;
  boost::optional<Transport> transport_;

};

class StreamingCapabilities
{
 public:

  StreamingCapabilities();
  StreamingCapabilities(const boost::optional<bool>& rtpmulticast, const boost::optional<bool>& rtptcp, const boost::optional<bool>& rtprtsptcp, const boost::optional<bool>& nonaggregatecontrol, const boost::optional<bool>& nortspstreaming);
  StreamingCapabilities(const pugi::xml_node& node);

  inline bool SupportsRTPMulticast() const { return (rtpmulticast_.is_initialized() && (*rtpmulticast_ == true)); }
  inline bool SupportsRTPTCP() const { return (rtptcp_.is_initialized() && (*rtptcp_ == true)); }
  inline bool SupportsRTPRTSPTCP() const { return (rtprtsptcp_.is_initialized() && (*rtprtsptcp_ == true)); }
  inline bool SupportsNonAggregateControl() const { return (nonaggregatecontrol_.is_initialized() && (*nonaggregatecontrol_ == true)); }
  inline bool NoRTSPStreaming() const { return (nortspstreaming_.is_initialized() && (*nortspstreaming_ == true)); }

  std::string ToXml(const std::string& name) const;

  bool operator==(const StreamingCapabilities& rhs) const;

  boost::optional<bool> rtpmulticast_;
  boost::optional<bool> rtptcp_;
  boost::optional<bool> rtprtsptcp_;
  boost::optional<bool> nonaggregatecontrol_;
  boost::optional<bool> nortspstreaming_;

};

class Space2dDescription
{
 public:

  Space2dDescription();
  Space2dDescription(const boost::optional<std::string>& uri, const boost::optional<FloatRange>& xrange, const boost::optional<FloatRange>& yrange);
  Space2dDescription(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Space2dDescription& rhs) const;

  boost::optional<std::string> uri_;
  boost::optional<FloatRange> xrange_;
  boost::optional<FloatRange> yrange_;

};

class Space1dDescription
{
 public:

  Space1dDescription();
  Space1dDescription(const boost::optional<std::string>& uri, const boost::optional<FloatRange>& xrange);
  Space1dDescription(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Space1dDescription& rhs) const;

  boost::optional<std::string> uri_;
  boost::optional<FloatRange> xrange_;

};

class SecurityCapabilities
{
 public:

  SecurityCapabilities();
  SecurityCapabilities(const boost::optional<bool>& tls11, const boost::optional<bool>& tls12, const boost::optional<bool>& onboardkeygeneration, const boost::optional<bool>& accesspolicyconfig, const boost::optional<bool>& x509token, const boost::optional<bool>& samltoken, const boost::optional<bool>& kerberostoken, const boost::optional<bool>& reltoken, const boost::optional<bool>& tls10, const boost::optional<bool>& dot1x, const std::vector<int>& supportedeapmethod, const boost::optional<bool>& remoteuserhandling);
  SecurityCapabilities(const pugi::xml_node& node);

  inline bool SupportsTLS() const { return (SupportsTLS10() || SupportsTLS11() || SupportsTLS12()); }
  inline bool SupportsTLS11() const { return (tls11_.is_initialized() && (*tls11_ == true)); }
  inline bool SupportsTLS12() const { return (tls12_.is_initialized() && (*tls12_ == true)); }
  inline bool SupportsOnboardKeyGeneration() const { return (onboardkeygeneration_.is_initialized() && (*onboardkeygeneration_ == true)); }
  inline bool SupportsAccessPolicyConfig() const { return (accesspolicyconfig_.is_initialized() && (*accesspolicyconfig_ == true)); }
  inline bool SupportsDefaultAccessPolicy() const { return (defaultaccesspolicy_.is_initialized() && (*defaultaccesspolicy_ == true)); }
  inline bool SupportsX509Token() const { return (x509token_.is_initialized() && (*x509token_ == true)); }
  inline bool SupportsSAMLToken() const { return (samltoken_.is_initialized() && (*samltoken_ == true)); }
  inline bool SupportsKerberosToken() const { return (kerberostoken_.is_initialized() && (*kerberostoken_ == true)); }
  inline bool SupportsRELToken() const { return (reltoken_.is_initialized() && (*reltoken_ == true)); }
  inline bool SupportsTLS10() const { return (tls10_.is_initialized() && (*tls10_ == true)); }
  inline bool SupportsDot1X() const { return (dot1x_.is_initialized() && (*dot1x_ == true)); }
  inline std::vector<int> SupportedEAPMethods() const { return supportedeapmethod_; }
  inline bool SupportsRemoteUserHandling() const { return (remoteuserhandling_.is_initialized() && (*remoteuserhandling_ == true)); }

  std::string ToXml(const std::string& name) const;

  bool operator==(const SecurityCapabilities& rhs) const;

  boost::optional<bool> tls11_;
  boost::optional<bool> tls12_;
  boost::optional<bool> onboardkeygeneration_;
  boost::optional<bool> accesspolicyconfig_;
  boost::optional<bool> defaultaccesspolicy_;
  boost::optional<bool> x509token_;
  boost::optional<bool> samltoken_;
  boost::optional<bool> kerberostoken_;
  boost::optional<bool> reltoken_;
  boost::optional<bool> tls10_;
  boost::optional<bool> dot1x_;
  std::vector<int> supportedeapmethod_;
  boost::optional<bool> remoteuserhandling_;

};

class SearchCapabilities
{
 public:

  SearchCapabilities();
  SearchCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& metadatasearch);
  SearchCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SearchCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;
  boost::optional<bool> metadatasearch_;

};

class Scope
{
 public:

  Scope();
  Scope(const boost::optional<SCOPEDEFINITION>& scopedef, const boost::optional<std::string>& scopeitem);
  Scope(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Scope& rhs) const;

  boost::optional<SCOPEDEFINITION> scopedef_;
  boost::optional<std::string> scopeitem_;

};

class RuleEngineConfiguration
{
 public:

  RuleEngineConfiguration();
  RuleEngineConfiguration(const std::vector<Config>& rules);
  RuleEngineConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RuleEngineConfiguration& rhs) const;

  std::vector<Config> rules_;

};

class RotateOptions
{
 public:

  RotateOptions();
  RotateOptions(const std::vector<ROTATEMODE>& mode, const boost::optional<IntList>& degreelist);
  RotateOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RotateOptions& rhs) const;

  std::vector<ROTATEMODE> mode_;
  boost::optional<IntList> degreelist_;

};

class Rotate
{
 public:

  Rotate();
  Rotate(const boost::optional<ROTATEMODE>& rotatemode, const boost::optional<int>& degree);
  Rotate(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Rotate& rhs) const;

  boost::optional<ROTATEMODE> rotatemode_;
  boost::optional<int> degree_;

};

class ReplayCapabilities
{
 public:

  ReplayCapabilities();
  ReplayCapabilities(const boost::optional<std::string>& xaddr);
  ReplayCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ReplayCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;

};

class RemoteUser
{
 public:

  RemoteUser();
  RemoteUser(const boost::optional<std::string>& username, const boost::optional<std::string>& password, const boost::optional<bool>& usederivedpassword);
  RemoteUser(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RemoteUser& rhs) const;

  boost::optional<std::string> username_;
  boost::optional<std::string> password_;
  boost::optional<bool> usederivedpassword_;

};

class RelayOutputSettings
{
 public:

  RelayOutputSettings();
  RelayOutputSettings(const boost::optional<RELAYMODE>& mode, const boost::optional<Duration>& delaytime, const boost::optional<RELAYIDLESTATE>& idlestate);
  RelayOutputSettings(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RelayOutputSettings& rhs) const;

  boost::optional<RELAYMODE> mode_;
  boost::optional<Duration> delaytime_;
  boost::optional<RELAYIDLESTATE> idlestate_;

};

class RelayOutput
{
 public:

  RelayOutput();
  RelayOutput(const boost::optional<std::string>& token, const boost::optional<RelayOutputSettings>& properties);
  RelayOutput(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RelayOutput& rhs) const;

  boost::optional<std::string> token_;
  boost::optional<RelayOutputSettings> properties_;

};

class RelativeFocusOptions20
{
 public:

  RelativeFocusOptions20();
  RelativeFocusOptions20(const boost::optional<FloatRange>& distance, const boost::optional<FloatRange>& speed);
  RelativeFocusOptions20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RelativeFocusOptions20& rhs) const;

  boost::optional<FloatRange> distance_;
  boost::optional<FloatRange> speed_;

};

class RelativeFocus
{
 public:

  RelativeFocus();
  RelativeFocus(const boost::optional<float>& distance, const boost::optional<float>& speed);
  RelativeFocus(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RelativeFocus& rhs) const;

  boost::optional<float> distance_;
  boost::optional<float> speed_;

};

class Rectangle
{
 public:

  Rectangle();
  Rectangle(const boost::optional<float>& bottom, const boost::optional<float>& top, const boost::optional<float>& right, const boost::optional<float>& left);
  Rectangle(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Rectangle& rhs) const;

  boost::optional<float> bottom_;
  boost::optional<float> top_;
  boost::optional<float> right_;
  boost::optional<float> left_;

};

class RecordingCapabilities
{
 public:

  RecordingCapabilities();
  RecordingCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& receiversource, const boost::optional<bool>& mediaprofilesource, const boost::optional<bool>& dynamicrecordings, const boost::optional<bool>& dynamictracks, const boost::optional<int>& maxstringlength);
  RecordingCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RecordingCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;
  boost::optional<bool> receiversource_;
  boost::optional<bool> mediaprofilesource_;
  boost::optional<bool> dynamicrecordings_;
  boost::optional<bool> dynamictracks_;
  boost::optional<int> maxstringlength_;

};

class ReceiverCapabilities
{
 public:

  ReceiverCapabilities();
  ReceiverCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& rtpmulticast, const boost::optional<bool>& rtptcp, const boost::optional<bool>& rtprtsptcp, const boost::optional<int>& supportedreceivers, const boost::optional<int>& maxrtspurilength);
  ReceiverCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ReceiverCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;
  boost::optional<bool> rtpmulticast_;
  boost::optional<bool> rtptcp_;
  boost::optional<bool> rtprtsptcp_;
  boost::optional<int> supportedreceivers_;
  boost::optional<int> maxrtspurilength_;

};

class RealTimeStreamingCapabilities
{
 public:

  RealTimeStreamingCapabilities();
  RealTimeStreamingCapabilities(const boost::optional<bool>& rtpmulticast, const boost::optional<bool>& rtptcp, const boost::optional<bool>& rtprtsptcp);
  RealTimeStreamingCapabilities(const pugi::xml_node& node);

  inline bool SupportsRTPMulticast() const { return (rtpmulticast_.is_initialized() && (*rtpmulticast_ == true)); }
  inline bool SupportsRTPTCP() const { return (rtptcp_.is_initialized() && (*rtptcp_ == true)); }
  inline bool SupportsRTPRTSPTCP() const { return (rtprtsptcp_.is_initialized() && (*rtprtsptcp_ == true)); }

  std::string ToXml(const std::string& name) const;

  bool operator==(const RealTimeStreamingCapabilities& rhs) const;

  boost::optional<bool> rtpmulticast_;
  boost::optional<bool> rtptcp_;
  boost::optional<bool> rtprtsptcp_;

};

class PTZStatusFilterOptions
{
 public:

  PTZStatusFilterOptions();
  PTZStatusFilterOptions(const boost::optional<bool>& pantiltstatussupported, const boost::optional<bool>& zoomstatussupported, const boost::optional<bool>& pantiltpositionsupported, const boost::optional<bool>& zoompositionsupported);
  PTZStatusFilterOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PTZStatusFilterOptions& rhs) const;

  boost::optional<bool> pantiltstatussupported_;
  boost::optional<bool> zoomstatussupported_;
  boost::optional<bool> pantiltpositionsupported_;
  boost::optional<bool> zoompositionsupported_;

};

class PTZSpeed
{
 public:

  PTZSpeed();
  PTZSpeed(const boost::optional<Vector2d>& pantilt, const boost::optional<Vector1d>& zoom);
  PTZSpeed(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PTZSpeed& rhs) const;

  boost::optional<Vector2d> pantilt_;
  boost::optional<Vector1d> zoom_;

};

class PTZSpaces
{
 public:

  PTZSpaces();
  PTZSpaces(const std::vector<Space2dDescription>& absolutepantiltpositionspace, const std::vector<Space1dDescription>& absolutezoompositionspace, const std::vector<Space2dDescription>& relativepantilttranslationspace, const std::vector<Space1dDescription>& relativezoomtranslationspace, const std::vector<Space2dDescription>& continuouspantiltvelocityspace, const std::vector<Space1dDescription>& continuouszoomvelocityspace, const std::vector<Space1dDescription>& pantiltspeedspace, const std::vector<Space1dDescription>& zoomspeedspace);
  PTZSpaces(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PTZSpaces& rhs) const;

  std::vector<Space2dDescription> absolutepantiltpositionspace_;
  std::vector<Space1dDescription> absolutezoompositionspace_;
  std::vector<Space2dDescription> relativepantilttranslationspace_;
  std::vector<Space1dDescription> relativezoomtranslationspace_;
  std::vector<Space2dDescription> continuouspantiltvelocityspace_;
  std::vector<Space1dDescription> continuouszoomvelocityspace_;
  std::vector<Space1dDescription> pantiltspeedspace_;
  std::vector<Space1dDescription> zoomspeedspace_;

};

class PTZPresetTourSupported
{
 public:

  PTZPresetTourSupported();
  PTZPresetTourSupported(const boost::optional<int>& maximumnumberofpresettours, const std::vector<PTZPRESETTOUROPERATION>& ptzpresettouroperation);
  PTZPresetTourSupported(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PTZPresetTourSupported& rhs) const;

  boost::optional<int> maximumnumberofpresettours_;
  std::vector<PTZPRESETTOUROPERATION> ptzpresettouroperation_;

};

class PTZConfigurationOptions
{
 public:

  PTZConfigurationOptions();
  PTZConfigurationOptions(const boost::optional<IntAttrList>& ptzramps_, const boost::optional<PTZSpaces>& spaces);
  PTZConfigurationOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PTZConfigurationOptions& rhs) const;

  boost::optional<IntAttrList> ptzramps_;
  boost::optional<PTZSpaces> spaces_;

};

class PTZNode
{
 public:

  PTZNode();
  PTZNode(const boost::optional<std::string>& token, const boost::optional<bool>& fixedhomeposition, const boost::optional<std::string>& name, const boost::optional<PTZSpaces>& supportedptzspaces, const boost::optional<int>& maximumnumberofpresets, const boost::optional<bool>& homesupported, const std::vector<std::string>& auxiliarycommands, const std::vector<PTZPresetTourSupported>& supportedpresettour);
  PTZNode(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PTZNode& rhs) const;

  boost::optional<std::string> token_;
  boost::optional<bool> fixedhomeposition_;
  boost::optional<std::string> name_;
  boost::optional<PTZSpaces> supportedptzspaces_;
  boost::optional<int> maximumnumberofpresets_;
  boost::optional<bool> homesupported_;
  std::vector<std::string> auxiliarycommands_;
  std::vector<PTZPresetTourSupported> supportedpresettour_;

};

class PTZFilter
{
 public:

  PTZFilter();
  PTZFilter(const boost::optional<bool>& status, const boost::optional<bool>& position);
  PTZFilter(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PTZFilter& rhs) const;

  boost::optional<bool> status_;
  boost::optional<bool> position_;

};

class PTZControlDirectionOptions
{
 public:

  PTZControlDirectionOptions();
  PTZControlDirectionOptions(const boost::optional<EFLIPMODE>& eflip, const boost::optional<REVERSEMODE>& reverse);
  PTZControlDirectionOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PTZControlDirectionOptions& rhs) const;

  boost::optional<EFLIPMODE> eflip_;
  boost::optional<REVERSEMODE> reverse_;

};

class DefaultPTZSpeed
{
 public:

  DefaultPTZSpeed();
  DefaultPTZSpeed(const boost::optional<Vector2d>& pantilt, const boost::optional<int>& zoom);
  DefaultPTZSpeed(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const DefaultPTZSpeed& rhs) const;

  boost::optional<Vector2d> pantilt_;
  boost::optional<int> zoom_;

};

class PanTiltLimits
{
 public:

  PanTiltLimits();
  PanTiltLimits(const boost::optional<Space2dDescription>& range);
  PanTiltLimits(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PanTiltLimits& rhs) const;

  boost::optional<Space2dDescription> range_;

};

class ZoomLimits
{
 public:

  ZoomLimits();
  ZoomLimits(const boost::optional<Space1dDescription>& range);
  ZoomLimits(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ZoomLimits& rhs) const;

  boost::optional<Space1dDescription> range_;

};

class PTZConfiguration
{
 public:

  PTZConfiguration();
  PTZConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<std::string>& nodetoken, const boost::optional<std::string>& defaultabsolutepantiltpositionspace, const boost::optional<std::string>& defaultabsolutezoompositionspace, const boost::optional<std::string>& defaultrelativepantilttranslationspace, const boost::optional<std::string>& defaultrelativezoomtranslationspace, const boost::optional<std::string>& defaultcontinuouspantiltvelocityspace, const boost::optional<std::string>& defaultcontinuouszoomvelocityspace, const boost::optional<DefaultPTZSpeed>& defaultptzspeed, const boost::optional<std::string>& defaultptztimeout, const boost::optional<PanTiltLimits> pantiltlimits, const boost::optional<ZoomLimits> zoomlimits);
  PTZConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PTZConfiguration& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<int> usecount_;
  boost::optional<std::string> token_;
  boost::optional<std::string> nodetoken_;
  boost::optional<std::string> defaultabsolutepantiltpositionspace_;
  boost::optional<std::string> defaultabsolutezoompositionspace_;
  boost::optional<std::string> defaultrelativepantilttranslationspace_;
  boost::optional<std::string> defaultrelativezoomtranslationspace_;
  boost::optional<std::string> defaultcontinuouspantiltvelocityspace_;
  boost::optional<std::string> defaultcontinuouszoomvelocityspace_;
  boost::optional<DefaultPTZSpeed> defaultptzspeed_;
  boost::optional<std::string> defaultptztimeout_;
  boost::optional<PanTiltLimits> pantiltlimits_;
  boost::optional<ZoomLimits> zoomlimits_;

};

class PTZCapabilities
{
 public:

  PTZCapabilities();
  PTZCapabilities(const boost::optional<std::string>& xaddr);
  PTZCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PTZCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;

};

class ProfileCapabilities
{
 public:

  ProfileCapabilities();
  ProfileCapabilities(const boost::optional<int>& maximumnumberofprofiles);
  ProfileCapabilities(const pugi::xml_node& node);

  inline int MaximumNumberOfProfiles() const { return (maximumnumberofprofiles_.is_initialized() ? *maximumnumberofprofiles_ : 0); }

  std::string ToXml(const std::string& name) const;

  bool operator==(const ProfileCapabilities& rhs) const;

  boost::optional<int> maximumnumberofprofiles_;

};

class IntRectangleRange
{
 public:

  IntRectangleRange();
  IntRectangleRange(const boost::optional<IntRange>& xrange, const boost::optional<IntRange>& yrange, const boost::optional<IntRange>& widthrange, const boost::optional<IntRange>& heightrange);
  IntRectangleRange(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IntRectangleRange& rhs) const;

  boost::optional<IntRange> xrange_;
  boost::optional<IntRange> yrange_;
  boost::optional<IntRange> widthrange_;
  boost::optional<IntRange> heightrange_;

};

class IntRectangle
{
 public:

  IntRectangle();
  IntRectangle(const boost::optional<int>& x, const boost::optional<int>& y, const boost::optional<int>& width, const boost::optional<int>& height);
  IntRectangle(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IntRectangle& rhs) const;

  boost::optional<int> x_;
  boost::optional<int> y_;
  boost::optional<int> width_;
  boost::optional<int> height_;

};

class VideoSourceConfigurationOptions
{
 public:

  VideoSourceConfigurationOptions();
  VideoSourceConfigurationOptions(const boost::optional<IntRectangleRange>& boundsrange, const std::vector<std::string>& videosourcetokensavailable, const boost::optional<RotateOptions>& rotate);
  VideoSourceConfigurationOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const VideoSourceConfigurationOptions& rhs) const;

  boost::optional<IntRectangleRange> boundsrange_;
  std::vector<std::string> videosourcetokensavailable_;
  boost::optional<RotateOptions> rotate_;

};

class VideoSourceConfiguration
{
 public:

  VideoSourceConfiguration();
  VideoSourceConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<std::string>& sourcetoken, const boost::optional<IntRectangle>& bounds, const boost::optional<Rotate>& rotate);
  VideoSourceConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const VideoSourceConfiguration& rhs) const;

  boost::optional<std::string> token_;
  boost::optional<std::string> name_;
  boost::optional<int> usecount_;
  boost::optional<std::string> sourcetoken_;
  boost::optional<IntRectangle> bounds_;
  boost::optional<Rotate> rotate_;

};

class AudioSourceConfigurationOptions
{
 public:

  AudioSourceConfigurationOptions();
  AudioSourceConfigurationOptions(const std::vector<std::string>& inputtokensavailable);
  AudioSourceConfigurationOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AudioSourceConfigurationOptions& rhs) const;

  std::vector<std::string> inputtokensavailable_;

};

class AudioSourceConfiguration
{
 public:

  AudioSourceConfiguration();
  AudioSourceConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<std::string>& sourcetoken);
  AudioSourceConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AudioSourceConfiguration& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<int> usecount_;
  boost::optional<std::string> token_;
  boost::optional<std::string> sourcetoken_;

};

class VideoAnalyticsConfiguration
{
 public:

  VideoAnalyticsConfiguration();
  VideoAnalyticsConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<AnalyticsEngineConfiguration>& analyticsengineconfiguration);
  VideoAnalyticsConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const VideoAnalyticsConfiguration& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<int> usecount_;
  boost::optional<std::string> token_;
  boost::optional<AnalyticsEngineConfiguration> analyticsengineconfiguration_;

};

class EventSubscription
{
 public:

  EventSubscription();
  EventSubscription(const boost::optional<ws::FilterType>& filter);
  EventSubscription(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const EventSubscription& rhs) const;

  boost::optional<ws::FilterType> filter_;

};

class MetadataConfiguration
{
 public:

  MetadataConfiguration();
  MetadataConfiguration(const boost::optional<std::string>& token, const boost::optional<METADATACOMPRESSIONTYPE>& compressiontype, const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<PTZFilter>& ptzstatus, const boost::optional<EventSubscription>& events, const boost::optional<bool>& analytics, const boost::optional<MulticastConfiguration>& multicast, const boost::optional<Duration>& sessiontimeout, const boost::optional<AnalyticsEngineConfiguration>& analyticsengineconfiguration);
  MetadataConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const MetadataConfiguration& rhs) const;

  boost::optional<std::string> token_;
  boost::optional<METADATACOMPRESSIONTYPE> compressiontype_;
  boost::optional<std::string> name_;
  boost::optional<int> usecount_;
  boost::optional<PTZFilter> ptzstatus_;
  boost::optional<EventSubscription> events_;
  boost::optional<bool> analytics_;
  boost::optional<MulticastConfiguration> multicast_;
  boost::optional<Duration> sessiontimeout_;
  boost::optional<AnalyticsEngineConfiguration> analyticsengineconfiguration_;

};

class VideoResolution
{
 public:

  VideoResolution();
  VideoResolution(const boost::optional<int>& width, const boost::optional<int>& height);
  VideoResolution(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const VideoResolution& rhs) const;

  boost::optional<int> width_;
  boost::optional<int> height_;

};

class JpegOptions2
{
 public:

  JpegOptions2();
  JpegOptions2(const std::vector<VideoResolution>& resolutionsavailable, const boost::optional<IntRange>& framerateaverage, const boost::optional<IntRange>& encodingintervalrange, const boost::optional<IntRange>& bitraterange);
  JpegOptions2(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const JpegOptions2& rhs) const;

  std::vector<VideoResolution> resolutionsavailable_;
  boost::optional<IntRange> framerateaverage_;
  boost::optional<IntRange> encodingintervalrange_;
  boost::optional<IntRange> bitraterange_;

};

class JpegOptions
{
 public:

  JpegOptions();
  JpegOptions(const std::vector<VideoResolution>& resolutionsavailable, const boost::optional<IntRange>& frameraterange, const boost::optional<IntRange>& encodingintervalrange);
  JpegOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const JpegOptions& rhs) const;

  std::vector<VideoResolution> resolutionsavailable_;
  boost::optional<IntRange> frameraterange_;
  boost::optional<IntRange> encodingintervalrange_;

};

class Mpeg4Options2
{
 public:

  Mpeg4Options2();
  Mpeg4Options2(const std::vector<VideoResolution>& resolutionsavailable, const boost::optional<IntRange>& govlengthrange, const boost::optional<IntRange>& framerateaverage, const boost::optional<IntRange>& encodingintervalrange, const std::vector<MPEG4PROFILE>& mpeg4profilessupported, const boost::optional<IntRange>& bitraterange);
  Mpeg4Options2(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Mpeg4Options2& rhs) const;

  std::vector<VideoResolution> resolutionsavailable_;
  boost::optional<IntRange> govlengthrange_;
  boost::optional<IntRange> framerateaverage_;
  boost::optional<IntRange> encodingintervalrange_;
  std::vector<MPEG4PROFILE> mpeg4profilessupported_;
  boost::optional<IntRange> bitraterange_;

};

class Mpeg4Options
{
 public:

  Mpeg4Options();
  Mpeg4Options(const std::vector<VideoResolution>& resolutionsavailable, const boost::optional<IntRange>& govlengthrange, const boost::optional<IntRange>& frameraterange, const boost::optional<IntRange>& encodingintervalrange, const std::vector<MPEG4PROFILE>& mpeg4profilessupported);
  Mpeg4Options(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Mpeg4Options& rhs) const;

  std::vector<VideoResolution> resolutionsavailable_;
  boost::optional<IntRange> govlengthrange_;
  boost::optional<IntRange> frameraterange_;
  boost::optional<IntRange> encodingintervalrange_;
  std::vector<MPEG4PROFILE> mpeg4profilessupported_;

};

class H264Options2
{
 public:

  H264Options2();
  H264Options2(const std::vector<VideoResolution>& resolutionsavailable, const boost::optional<IntRange>& govlengthrange, const boost::optional<IntRange>& framerateaverage, const boost::optional<IntRange>& encodingintervalrange, const std::vector<H264PROFILE>& h264profilessupported, const boost::optional<IntRange>& bitraterange);
  H264Options2(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const H264Options2& rhs) const;

  std::vector<VideoResolution> resolutionsavailable_;
  boost::optional<IntRange> govlengthrange_;
  boost::optional<IntRange> framerateaverage_;
  boost::optional<IntRange> encodingintervalrange_;
  std::vector<H264PROFILE> h264profilessupported_;
  boost::optional<IntRange> bitraterange_;

};

class H264Options
{
 public:

  H264Options();
  H264Options(const std::vector<VideoResolution>& resolutionsavailable, const boost::optional<IntRange>& govlengthrange, const boost::optional<IntRange>& frameraterange, const boost::optional<IntRange>& encodingintervalrange, const std::vector<H264PROFILE>& h264profilessupported);
  H264Options(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const H264Options& rhs) const;

  std::vector<VideoResolution> resolutionsavailable_;
  boost::optional<IntRange> govlengthrange_;
  boost::optional<IntRange> frameraterange_;
  boost::optional<IntRange> encodingintervalrange_;
  std::vector<H264PROFILE> h264profilessupported_;

};

class VideoEncoderConfigurationOptions
{
 public:

  VideoEncoderConfigurationOptions();
  VideoEncoderConfigurationOptions(const boost::optional<IntRange>& qualityrange, const boost::optional<JpegOptions>& jpeg, const boost::optional<Mpeg4Options>& mpeg4, const boost::optional<H264Options>& h264, const boost::optional<JpegOptions2>& extensionjpeg, const boost::optional<Mpeg4Options2>& extensionmpeg4, const boost::optional<H264Options2>& extensionh264);
  VideoEncoderConfigurationOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const VideoEncoderConfigurationOptions& rhs) const;

  boost::optional<IntRange> qualityrange_;
  boost::optional<JpegOptions> jpeg_;
  boost::optional<Mpeg4Options> mpeg4_;
  boost::optional<H264Options> h264_;
  boost::optional<JpegOptions2> extensionjpeg_;
  boost::optional<Mpeg4Options2> extensionmpeg4_;
  boost::optional<H264Options2> extensionh264_;

};

class VideoRateControl
{
 public:

  VideoRateControl();
  VideoRateControl(const boost::optional<int>& frameratelimit, const boost::optional<int>& encodinginterval, const boost::optional<int>& bitratelimit);
  VideoRateControl(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const VideoRateControl& rhs) const;

  boost::optional<int> frameratelimit_;
  boost::optional<int> encodinginterval_;
  boost::optional<int> bitratelimit_;

};

class Mpeg4Configuration
{
 public:

  Mpeg4Configuration();
  Mpeg4Configuration(const boost::optional<int>& govlength, const boost::optional<MPEG4PROFILE>& mpeg4profile);
  Mpeg4Configuration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Mpeg4Configuration& rhs) const;

  boost::optional<int> govlength_;
  boost::optional<MPEG4PROFILE> mpeg4profile_;

};

class H264Configuration
{
public:

  H264Configuration();
  H264Configuration(const boost::optional<int>& govlength, const boost::optional<H264PROFILE>& h264profile);
  H264Configuration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const H264Configuration& rhs) const;

  boost::optional<int> govlength_;
  boost::optional<H264PROFILE> h264profile_;

};

class VideoEncoderConfiguration
{
 public:

  VideoEncoderConfiguration();
  VideoEncoderConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<VIDEOENCODING>& encoding, const boost::optional<VideoResolution>& resolution, const boost::optional<float>& quality, const boost::optional<VideoRateControl>& ratecontrol, const boost::optional<Mpeg4Configuration>& mpeg4, const boost::optional<H264Configuration>& h264, const boost::optional<MulticastConfiguration>& multicast, const boost::optional<Duration>& sessiontimeout);
  VideoEncoderConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const VideoEncoderConfiguration& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<int> usecount_;
  boost::optional<std::string> token_;
  boost::optional<VIDEOENCODING> encoding_;
  boost::optional<VideoResolution> resolution_;
  boost::optional<float> quality_;
  boost::optional<VideoRateControl> ratecontrol_;
  boost::optional<Mpeg4Configuration> mpeg4_;
  boost::optional<H264Configuration> h264_;
  boost::optional<MulticastConfiguration> multicast_;
  boost::optional<Duration> sessiontimeout_;

};

class AudioOutputConfiguration
{
 public:

  AudioOutputConfiguration();
  AudioOutputConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<std::string>& outputtoken, const boost::optional<std::string>& sendprimacy, const boost::optional<int>& outputlevel);
  AudioOutputConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AudioOutputConfiguration& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<int> usecount_;
  boost::optional<std::string> token_;
  boost::optional<std::string> outputtoken_;
  boost::optional<std::string> sendprimacy_;
  boost::optional<int> outputlevel_;

};

class NetworkHost
{
 public:

  NetworkHost();
  NetworkHost(const boost::optional<NETWORKHOSTTYPE>& type, const boost::optional<std::string>& ipv4address, const boost::optional<std::string>& ipv6address, const boost::optional<std::string>& dnsname);
  NetworkHost(NETWORKHOSTTYPE type, const std::string& address);
  NetworkHost(const pugi::xml_node& node);

  std::string GetAddress() const;

  bool IsValid() const;

  std::string ToXml(const std::string& name) const;

  bool operator==(const NetworkHost& rhs) const;

  boost::optional<NETWORKHOSTTYPE> type_;
  boost::optional<std::string> ipv4address_;
  boost::optional<std::string> ipv6address_;
  boost::optional<std::string> dnsname_;

};

class NetworkGateway
{
 public:

  NetworkGateway();
  NetworkGateway(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address);
  NetworkGateway(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const NetworkGateway& rhs) const;

  std::vector<std::string> ipv4address_;
  std::vector<std::string> ipv6address_;

};

class Profile
{
 public:

  Profile();
  Profile(const boost::optional<std::string>& name, const boost::optional<VideoSourceConfiguration>& videosourceconfiguration, const boost::optional<AudioSourceConfiguration>& audiosourceconfiguration, const boost::optional<VideoEncoderConfiguration>& videoencoderconfiguration, const boost::optional<AudioEncoderConfiguration>& audioencoderconfiguration, const boost::optional<VideoAnalyticsConfiguration>& videoanalyticsconfiguration, const boost::optional<PTZConfiguration>& ptzconfiguration, const boost::optional<MetadataConfiguration>& metadataconfiguration, const boost::optional<AudioOutputConfiguration>& audiooutputconfiguration, const boost::optional<AudioDecoderConfiguration>& audiodecoderconfiguration, const boost::optional<std::string>& token, const boost::optional<bool>& fixed);
  Profile(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Profile& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<VideoSourceConfiguration> videosourceconfiguration_;
  boost::optional<AudioSourceConfiguration> audiosourceconfiguration_;
  boost::optional<VideoEncoderConfiguration> videoencoderconfiguration_;
  boost::optional<AudioEncoderConfiguration> audioencoderconfiguration_;
  boost::optional<VideoAnalyticsConfiguration> videoanalyticsconfiguration_;
  boost::optional<PTZConfiguration> ptzconfiguration_;
  boost::optional<MetadataConfiguration> metadataconfiguration_;
  boost::optional<AudioOutputConfiguration> audiooutputconfiguration_;
  boost::optional<AudioDecoderConfiguration> audiodecoderconfiguration_;
  boost::optional<std::string> token_;
  boost::optional<bool> fixed_;

};

class PrefixedIPv6Address
{
 public:

  PrefixedIPv6Address();
  PrefixedIPv6Address(const boost::optional<std::string>& address, const boost::optional<int>& prefixlength);
  PrefixedIPv6Address(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PrefixedIPv6Address& rhs) const;

  boost::optional<std::string> address_;
  boost::optional<int> prefixlength_;

};

class PrefixedIPv4Address
{
 public:

  PrefixedIPv4Address();
  PrefixedIPv4Address(const boost::optional<std::string>& address, const boost::optional<int>& prefixlength);
  PrefixedIPv4Address(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PrefixedIPv4Address& rhs) const;

  boost::optional<std::string> address_;
  boost::optional<int> prefixlength_;

};

class PaneLayout
{
 public:

  PaneLayout();
  PaneLayout(const boost::optional<std::string>& pane, const boost::optional<Rectangle>& area);
  PaneLayout(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const PaneLayout& rhs) const;

  boost::optional<std::string> pane_;
  boost::optional<Rectangle> area_;

};

class NTPInformation
{
 public:

  NTPInformation();
  NTPInformation(const boost::optional<bool>& fromdhcp, const std::vector<NetworkHost>& ntpfromdhcp, const std::vector<NetworkHost>& ntpmanual);
  NTPInformation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const NTPInformation& rhs) const;

  boost::optional<bool> fromdhcp_;
  std::vector<NetworkHost> ntpfromdhcp_;
  std::vector<NetworkHost> ntpmanual_;

};

class NoiseReductionOptions
{
 public:

  NoiseReductionOptions();
  NoiseReductionOptions(const boost::optional<bool>& level);
  NoiseReductionOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const NoiseReductionOptions& rhs) const;

  boost::optional<bool> level_;

};

class NoiseReduction
{
 public:

  NoiseReduction();
  NoiseReduction(const boost::optional<float>& level);
  NoiseReduction(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const NoiseReduction& rhs) const;

  boost::optional<float> level_;

};

class NetworkZeroConfiguration
{
 public:

  NetworkZeroConfiguration();
  NetworkZeroConfiguration(const boost::optional<std::string>& interfacetoken, const boost::optional<bool>& enabled, const std::vector<std::string>& addresses, const std::vector<NetworkZeroConfiguration>& additional);
  NetworkZeroConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  boost::optional<NetworkZeroConfiguration> GetNetworkZeroConfiguration(const std::string& interfacetoken) const;

  bool operator==(const NetworkZeroConfiguration& rhs) const;

  boost::optional<std::string> interfacetoken_;
  boost::optional<bool> enabled_;
  std::vector<std::string> addresses_;
  std::vector<NetworkZeroConfiguration> additional_;

};

class NetworkProtocol
{
 public:

  NetworkProtocol();
  NetworkProtocol(const boost::optional<NETWORKPROTOCOLTYPE>& name, const boost::optional<bool>& enabled, const std::vector<int>& port);
  NetworkProtocol(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const NetworkProtocol& rhs) const;

  boost::optional<NETWORKPROTOCOLTYPE> name_;
  boost::optional<bool> enabled_;
  std::vector<int> port_;

};

class NetworkInterfaceConnectionSetting
{
 public:

  NetworkInterfaceConnectionSetting();
  NetworkInterfaceConnectionSetting(const boost::optional<bool>& autonegotiation, const boost::optional<int>& speed, const boost::optional<DUPLEX>& duplex);
  NetworkInterfaceConnectionSetting(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const NetworkInterfaceConnectionSetting& rhs) const;

  boost::optional<bool> autonegotiation_;
  boost::optional<int> speed_;
  boost::optional<DUPLEX> duplex_;

};

class IPv4NetworkInterfaceSetConfiguration
{
 public:

  IPv4NetworkInterfaceSetConfiguration();
  IPv4NetworkInterfaceSetConfiguration(const boost::optional<bool>& enabled, const std::vector<PrefixedIPv4Address>& manual, const boost::optional<bool>& dhcp);
  IPv4NetworkInterfaceSetConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IPv4NetworkInterfaceSetConfiguration& rhs) const;

  boost::optional<bool> enabled_;
  std::vector<PrefixedIPv4Address> manual_;
  boost::optional<bool> dhcp_;

};

class IPv6NetworkInterfaceSetConfiguration
{
 public:

  IPv6NetworkInterfaceSetConfiguration();
  IPv6NetworkInterfaceSetConfiguration(const boost::optional<bool>& enabled, const boost::optional<bool>& acceptrouteradvert, const std::vector<PrefixedIPv6Address>& manual, const boost::optional<IPV6DHCPCONFIGURATION>& dhcp);
  IPv6NetworkInterfaceSetConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IPv6NetworkInterfaceSetConfiguration& rhs) const;

  boost::optional<bool> enabled_;
  boost::optional<bool> acceptrouteradvert_;
  std::vector<PrefixedIPv6Address> manual_;
  boost::optional<IPV6DHCPCONFIGURATION> dhcp_;

};

class IPv6Configuration
{
 public:

  IPv6Configuration();
  IPv6Configuration(const boost::optional<bool>& acceptrouteradvert, const boost::optional<IPV6DHCPCONFIGURATION>& dhcp, const std::vector<PrefixedIPv6Address>& manual, const std::vector<PrefixedIPv6Address>& linklocal, const std::vector<PrefixedIPv6Address>& fromdhcp, const std::vector<PrefixedIPv6Address>& fromra);
  IPv6Configuration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IPv6Configuration& rhs) const;

  boost::optional<bool> acceptrouteradvert_;
  boost::optional<IPV6DHCPCONFIGURATION> dhcp_;
  std::vector<PrefixedIPv6Address> manual_;
  std::vector<PrefixedIPv6Address> linklocal_;
  std::vector<PrefixedIPv6Address> fromdhcp_;
  std::vector<PrefixedIPv6Address> fromra_;

};

class IPv6NetworkInterface
{
 public:

  IPv6NetworkInterface();
  IPv6NetworkInterface(const boost::optional<bool>& enabled, const boost::optional<IPv6Configuration>& config);
  IPv6NetworkInterface(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IPv6NetworkInterface& rhs) const;

  boost::optional<bool> enabled_;
  boost::optional<IPv6Configuration> config_;

};

class IPv4Configuration
{
 public:

  IPv4Configuration();
  IPv4Configuration(const std::vector<PrefixedIPv4Address>& manual, const boost::optional<PrefixedIPv4Address>& linklocal, const boost::optional<PrefixedIPv4Address>& fromdhcp, const boost::optional<bool>& dhcp);
  IPv4Configuration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IPv4Configuration& rhs) const;

  std::vector<PrefixedIPv4Address> manual_;
  boost::optional<PrefixedIPv4Address> linklocal_;
  boost::optional<PrefixedIPv4Address> fromdhcp_;
  boost::optional<bool> dhcp_;

};

class IPv4NetworkInterface
{
 public:

  IPv4NetworkInterface();
  IPv4NetworkInterface(const boost::optional<bool>& enabled, const boost::optional<IPv4Configuration>& config);
  IPv4NetworkInterface(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IPv4NetworkInterface& rhs) const;

  boost::optional<bool> enabled_;
  boost::optional<IPv4Configuration> config_;

};

class IPAddressFilter
{
 public:

  IPAddressFilter();
  IPAddressFilter(const boost::optional<IPADDRESSFILTERTYPE>& type, const std::vector<PrefixedIPv4Address>& ipv4address, const std::vector<PrefixedIPv6Address>& ipv6address);
  IPAddressFilter(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IPAddressFilter& rhs) const;

  boost::optional<IPADDRESSFILTERTYPE> type_;
  std::vector<PrefixedIPv4Address> ipv4address_;
  std::vector<PrefixedIPv6Address> ipv6address_;

};

class Dot11Status
{
 public:

  Dot11Status();
  Dot11Status(const boost::optional<std::string>& ssid, const boost::optional<std::string>& bssid, const boost::optional<DOT11CIPHER>& paircipher, const boost::optional<DOT11CIPHER>& groupcipher, const boost::optional<DOT11SIGNALSTRENGTH>& signalstrength, const boost::optional<std::string>& activeconfigalias);
  Dot11Status(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Dot11Status& rhs) const;

  boost::optional<std::string> ssid_;
  boost::optional<std::string> bssid_;
  boost::optional<DOT11CIPHER> paircipher_;
  boost::optional<DOT11CIPHER> groupcipher_;
  boost::optional<DOT11SIGNALSTRENGTH> signalstrength_;
  boost::optional<std::string> activeconfigalias_;

};

class Dot11PSKSet
{
 public:

  Dot11PSKSet();
  Dot11PSKSet(const boost::optional<std::string>& key, const boost::optional<std::string>& passphrase);
  Dot11PSKSet(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Dot11PSKSet& rhs) const;

  boost::optional<std::string> key_;
  boost::optional<std::string> passphrase_;

};

class Dot11SecurityConfiguration
{
 public:

  Dot11SecurityConfiguration();
  Dot11SecurityConfiguration(const boost::optional<DOT11SECURITYMODE>& mode, const boost::optional<DOT11CIPHER>& algorithm, const boost::optional<Dot11PSKSet>& psk, const boost::optional<std::string>& dot1x);
  Dot11SecurityConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Dot11SecurityConfiguration& rhs) const;

  boost::optional<DOT11SECURITYMODE> mode_;
  boost::optional<DOT11CIPHER> algorithm_;
  boost::optional<Dot11PSKSet> psk_;
  boost::optional<std::string> dot1x_;

};

class Dot11Configuration
{
 public:

  Dot11Configuration();
  Dot11Configuration(const boost::optional<std::string> ssid, const boost::optional<DOT11STATIONMODE>& mode, const boost::optional<std::string>& alias, const boost::optional<unsigned int>& priority, const boost::optional<Dot11SecurityConfiguration>& security);
  Dot11Configuration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Dot11Configuration& rhs) const;

  boost::optional<std::string> ssid_;
  boost::optional<DOT11STATIONMODE> mode_;
  boost::optional<std::string> alias_;
  boost::optional<unsigned int> priority_;
  boost::optional<Dot11SecurityConfiguration> security_;

};

class Dot11Capabilities
{
 public:

  Dot11Capabilities();
  Dot11Capabilities(const boost::optional<bool>& tkip, const boost::optional<bool>& scanavailablenetworks, const boost::optional<bool>& multipleconfiguration, const boost::optional<bool>& adhocstationmode, const boost::optional<bool>& wep);
  Dot11Capabilities(const pugi::xml_node& node);

  inline bool SupportsTKIP() const { return (tkip_.is_initialized() && (*tkip_ == true)); }
  inline bool SupportsScanAvailableNetworks() const { return (scanavailablenetworks_.is_initialized() && (*scanavailablenetworks_ == true)); }
  inline bool SupportsMultipleConfiguration() const { return (multipleconfiguration_.is_initialized() && (*multipleconfiguration_ == true)); }
  inline bool SupportsAdHocStationMode() const { return (adhocstationmode_.is_initialized() && (*adhocstationmode_ == true)); }
  inline bool SupportsWEP() const { return (wep_.is_initialized() && (*wep_ == true)); }

  std::string ToXml(const std::string& name) const;

  bool operator==(const Dot11Capabilities& rhs) const;

  boost::optional<bool> tkip_;
  boost::optional<bool> scanavailablenetworks_;
  boost::optional<bool> multipleconfiguration_;
  boost::optional<bool> adhocstationmode_;
  boost::optional<bool> wep_;

};

class Dot11AvailableNetworks
{
 public:

  Dot11AvailableNetworks();
  Dot11AvailableNetworks(const boost::optional<std::string>& ssid, const boost::optional<std::string>& bssid, const std::vector<DOT11AUTHANDMANAGEMENTSUITE>& authandmanagementsuite, const boost::optional<DOT11CIPHER>& paircipher, const boost::optional<DOT11CIPHER>& groupcipher, const boost::optional<DOT11SIGNALSTRENGTH>& signalstrength);
  Dot11AvailableNetworks(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Dot11AvailableNetworks& rhs) const;

  boost::optional<std::string> ssid_;
  boost::optional<std::string> bssid_;
  std::vector<DOT11AUTHANDMANAGEMENTSUITE> authandmanagementsuite_;
  boost::optional<DOT11CIPHER> paircipher_;
  boost::optional<DOT11CIPHER> groupcipher_;
  boost::optional<DOT11SIGNALSTRENGTH> signalstrength_;

};

class NetworkInterfaceSetConfiguration
{
 public:

  NetworkInterfaceSetConfiguration();
  NetworkInterfaceSetConfiguration(const boost::optional<bool>& enabled, const boost::optional<NetworkInterfaceConnectionSetting>& link, const boost::optional<int>& mtu, const boost::optional<IPv4NetworkInterfaceSetConfiguration>& ipv4, const boost::optional<IPv6NetworkInterfaceSetConfiguration>& ipv6, const std::vector<Dot11Configuration>& dot11);
  NetworkInterfaceSetConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const NetworkInterfaceSetConfiguration& rhs) const;

  boost::optional<bool> enabled_;
  boost::optional<NetworkInterfaceConnectionSetting> link_;
  boost::optional<int> mtu_;
  boost::optional<IPv4NetworkInterfaceSetConfiguration> ipv4_;
  boost::optional<IPv6NetworkInterfaceSetConfiguration> ipv6_;
  std::vector<Dot11Configuration> dot11_;

};

class NetworkInterfaceLink
{
 public:

  NetworkInterfaceLink();
  NetworkInterfaceLink(const boost::optional<NetworkInterfaceConnectionSetting> adminsettings, const boost::optional<NetworkInterfaceConnectionSetting>& opersettings, const boost::optional<int>& interfacetype);
  NetworkInterfaceLink(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const NetworkInterfaceLink& rhs) const;

  boost::optional<NetworkInterfaceConnectionSetting> adminsettings_;
  boost::optional<NetworkInterfaceConnectionSetting> opersettings_;
  boost::optional<int> interfacetype_;

};

class NetworkInterfaceInfo
{
 public:

  NetworkInterfaceInfo();
  NetworkInterfaceInfo(const boost::optional<std::string>& name, const boost::optional<std::string>& hwaddress, const boost::optional<int>& mtu);
  NetworkInterfaceInfo(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const NetworkInterfaceInfo& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<std::string> hwaddress_;
  boost::optional<int> mtu_;

};

class NetworkInterface
{
 public:

  NetworkInterface();
  NetworkInterface(const boost::optional<std::string>& token, const boost::optional<bool>& enabled, const boost::optional<NetworkInterfaceInfo>& info, const boost::optional<NetworkInterfaceLink>& link, const boost::optional<IPv4NetworkInterface>& ipv4, const boost::optional<IPv6NetworkInterface>& ipv6, const boost::optional<Dot11Configuration>& dot11);
  NetworkInterface(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const NetworkInterface& rhs) const;

  boost::optional<std::string> token_;
  boost::optional<bool> enabled_;
  boost::optional<NetworkInterfaceInfo> info_;
  boost::optional<NetworkInterfaceLink> link_;
  boost::optional<IPv4NetworkInterface> ipv4_;
  boost::optional<IPv6NetworkInterface> ipv6_;
  boost::optional<Dot11Configuration> dot11_;

};

class NetworkCapabilities
{
 public:

  NetworkCapabilities();
  NetworkCapabilities(const boost::optional<bool>& ipfilter, const boost::optional<bool>& zeroconfiguration, const boost::optional<bool>& ipversion6, const boost::optional<bool>& dynamicdns, const boost::optional<bool>& dot11configuration);
  NetworkCapabilities(const pugi::xml_node& node);

  inline bool SupportsIPFilter() const { return (ipfilter_.is_initialized() && (*ipfilter_ == true)); }
  inline bool SupportsZeroConfiguration() const { return (zeroconfiguration_.is_initialized() && (*zeroconfiguration_ == true)); }
  inline bool SupportsIPv6() const { return (ipversion6_.is_initialized() && (*ipversion6_ == true)); }
  inline bool SupportsDynamicDNS() const { return (dynamicdns_.is_initialized() && (*dynamicdns_ == true)); }
  inline bool SupportsDot11Configuration() const { return (dot11configuration_.is_initialized() && (*dot11configuration_ == true)); }

  std::string ToXml(const std::string& name) const;

  bool operator==(const NetworkCapabilities& rhs) const;

  boost::optional<bool> ipfilter_;
  boost::optional<bool> zeroconfiguration_;
  boost::optional<bool> ipversion6_;
  boost::optional<bool> dynamicdns_;
  boost::optional<bool> dot11configuration_;

};

class ContinuousFocusOptions
{
 public:

  ContinuousFocusOptions();
  ContinuousFocusOptions(const boost::optional<FloatRange>& speed);
  ContinuousFocusOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ContinuousFocusOptions& rhs) const;

  boost::optional<FloatRange> speed_;

};

class ContinuousFocus
{
 public:

  ContinuousFocus();
  ContinuousFocus(const boost::optional<float>& speed);
  ContinuousFocus(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ContinuousFocus& rhs) const;

  boost::optional<float> speed_;

};

class MoveOptions20
{
 public:

  MoveOptions20();
  MoveOptions20(const boost::optional<AbsoluteFocusOptions>& absolute, const boost::optional<RelativeFocusOptions20>& relative, const boost::optional<ContinuousFocusOptions>& continuous);
  MoveOptions20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const MoveOptions20& rhs) const;

  boost::optional<AbsoluteFocusOptions> absolute_;
  boost::optional<RelativeFocusOptions20> relative_;
  boost::optional<ContinuousFocusOptions> continuous_;

};

class MetadataConfigurationOptions
{
 public:

  MetadataConfigurationOptions();
  MetadataConfigurationOptions(const boost::optional<PTZStatusFilterOptions>& ptzstatusfilteroptions, const std::vector<METADATACOMPRESSIONTYPE>& compressiontype);
  MetadataConfigurationOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const MetadataConfigurationOptions& rhs) const;

  boost::optional<PTZStatusFilterOptions> ptzstatusfilteroptions_;
  std::vector<METADATACOMPRESSIONTYPE> compressiontype_;

};

class MediaUri
{
 public:

  MediaUri();
  MediaUri(const boost::optional<std::string>& uri, const boost::optional<bool>& invalidateafterconnect, const boost::optional<bool>& invalidateafterreboot, const boost::optional<Duration>& timeout);
  MediaUri(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const MediaUri& rhs) const;

  boost::optional<std::string> uri_;
  boost::optional<bool> invalidateafterconnect_;
  boost::optional<bool> invalidateafterreboot_;
  boost::optional<Duration> timeout_;

};

class MediaCapabilities
{
 public:

  MediaCapabilities();
  MediaCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<RealTimeStreamingCapabilities>& streamingcapabilities, const boost::optional<ProfileCapabilities>& profilecapabilities);
  MediaCapabilities(const pugi::xml_node& node);

  inline std::string GetXAddr() const { return (xaddr_.is_initialized() ? *xaddr_ : std::string()); }
  inline bool SupportsRTPMulticast() const { return (streamingcapabilities_.is_initialized() && streamingcapabilities_->SupportsRTPMulticast()); }
  inline bool SupportsRTPTCP() const { return (streamingcapabilities_.is_initialized() && streamingcapabilities_->SupportsRTPTCP()); }
  inline bool SupportsRTPRTSPTCP() const { return (streamingcapabilities_.is_initialized() && streamingcapabilities_->SupportsRTPRTSPTCP()); }
  inline int MaximumNumberOfProfiles() const { return (profilecapabilities_.is_initialized() && profilecapabilities_->MaximumNumberOfProfiles()); }

  std::string ToXml(const std::string& name) const;

  bool operator==(const MediaCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;
  boost::optional<RealTimeStreamingCapabilities> streamingcapabilities_;
  boost::optional<ProfileCapabilities> profilecapabilities_;

};

class Layout
{
 public:

  Layout();
  Layout(const std::vector<PaneLayout>& panelayout);
  Layout(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Layout& rhs) const;

  std::vector<PaneLayout> panelayout_;

};

class IrCutFilterAutoAdjustmentOptions
{
 public:

  IrCutFilterAutoAdjustmentOptions();
  IrCutFilterAutoAdjustmentOptions(const std::vector<IRCUTFILTERAUTOBOUNDARYTYPE>& boundarytype, const boost::optional<bool>& boundaryoffset, const boost::optional<DurationRange>& responsetimerange);
  IrCutFilterAutoAdjustmentOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IrCutFilterAutoAdjustmentOptions& rhs) const;

  std::vector<IRCUTFILTERAUTOBOUNDARYTYPE> boundarytype_;
  boost::optional<bool> boundaryoffset_;
  boost::optional<DurationRange> responsetimerange_;

};

class IrCutFilterAutoAdjustment
{
 public:

  IrCutFilterAutoAdjustment();
  IrCutFilterAutoAdjustment(const boost::optional<std::string>& boundarytype, const boost::optional<float>& boundaryoffset, const boost::optional<Duration>& responsetime);
  IrCutFilterAutoAdjustment(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IrCutFilterAutoAdjustment& rhs) const;

  boost::optional<std::string> boundarytype_;
  boost::optional<float> boundaryoffset_;
  boost::optional<Duration> responsetime_;

};

class IOCapabilities
{
 public:

  IOCapabilities();
  IOCapabilities(const boost::optional<int>& inputconnectors, const boost::optional<int>& relayoutputs);
  IOCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const IOCapabilities& rhs) const;

  boost::optional<int> inputconnectors_;
  boost::optional<int> relayoutputs_;

};

class FocusStatus20
{
 public:

  FocusStatus20();
  FocusStatus20(const boost::optional<float>& xaddr, const boost::optional<MOVESTATUS>& movestatus, const boost::optional<std::string>& error);
  FocusStatus20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const FocusStatus20& rhs) const;

  boost::optional<float> xaddr_;
  boost::optional<MOVESTATUS> movestatus_;
  boost::optional<std::string> error_;

};

class FocusOptions20
{
 public:

  FocusOptions20();
  FocusOptions20(const std::vector<AUTOFOCUSMODE>& autofocusmodes, const boost::optional<FloatRange>& defaultspeed, const boost::optional<FloatRange>& nearlimit, const boost::optional<FloatRange>& farlimit);
  FocusOptions20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const FocusOptions20& rhs) const;

  std::vector<AUTOFOCUSMODE> autofocusmodes_;
  boost::optional<FloatRange> defaultspeed_;
  boost::optional<FloatRange> nearlimit_;
  boost::optional<FloatRange> farlimit_;

};

class ImagingStatus20
{
 public:

  ImagingStatus20();
  ImagingStatus20(const boost::optional<FocusStatus20>& focusstatus20);
  ImagingStatus20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ImagingStatus20& rhs) const;

  boost::optional<FocusStatus20> focusstatus20_;

};

class BacklightCompensation20
{
 public:

  BacklightCompensation20();
  BacklightCompensation20(const boost::optional<BACKLIGHTCOMPENSATIONMODE>& mode, const boost::optional<float>& level);
  BacklightCompensation20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const BacklightCompensation20& rhs) const;

  boost::optional<BACKLIGHTCOMPENSATIONMODE> mode_;
  boost::optional<float> level_;

};

class BacklightCompensation
{
 public:

  BacklightCompensation();
  BacklightCompensation(const boost::optional<BACKLIGHTCOMPENSATIONMODE>& mode, const boost::optional<float>& level);
  BacklightCompensation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const BacklightCompensation& rhs) const;

  boost::optional<BACKLIGHTCOMPENSATIONMODE> mode_;
  boost::optional<float> level_;

};

class Exposure20
{
 public:

  Exposure20();
  Exposure20(const boost::optional<EXPOSUREMODE>& mode_, const boost::optional<EXPOSUREPRIORITY>& priority_, const boost::optional<Rectangle>& window_, const boost::optional<float>& minexposuretime_, const boost::optional<float>& maxexposuretime_, const boost::optional<float>& mingain_, const boost::optional<float>& maxgain_, const boost::optional<float>& miniris_, const boost::optional<float>& maxiris_, const boost::optional<float>& exposuretime_, const boost::optional<float>& gain_, const boost::optional<float>& iris_);
  Exposure20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Exposure20& rhs) const;

  boost::optional<EXPOSUREMODE> mode_;
  boost::optional<EXPOSUREPRIORITY> priority_;
  boost::optional<Rectangle> window_;
  boost::optional<float> minexposuretime_;
  boost::optional<float> maxexposuretime_;
  boost::optional<float> mingain_;
  boost::optional<float> maxgain_;
  boost::optional<float> miniris_;
  boost::optional<float> maxiris_;
  boost::optional<float> exposuretime_;
  boost::optional<float> gain_;
  boost::optional<float> iris_;

};

class FocusConfiguration20
{
 public:

  FocusConfiguration20();
  FocusConfiguration20(const boost::optional<AUTOFOCUSMODE>& autofocusmode, const boost::optional<float>& defaultspeed, const boost::optional<float>& nearlimit, const boost::optional<float>& farlimit);
  FocusConfiguration20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const FocusConfiguration20& rhs) const;

  boost::optional<AUTOFOCUSMODE> autofocusmode_;
  boost::optional<float> defaultspeed_;
  boost::optional<float> nearlimit_;
  boost::optional<float> farlimit_;

};

class WideDynamicRange20
{
 public:

  WideDynamicRange20();
  WideDynamicRange20(const boost::optional<WIDEDYNAMICMODE>& mode, const boost::optional<float>& level);
  WideDynamicRange20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const WideDynamicRange20& rhs) const;

  boost::optional<WIDEDYNAMICMODE> mode_;
  boost::optional<float> level_;

};

class WhiteBalanceOptions20
{
 public:

  WhiteBalanceOptions20();
  WhiteBalanceOptions20(const std::vector<WHITEBALANCEMODE>& mode, const boost::optional<FloatRange>& yrgain, const boost::optional<FloatRange>& ybgain);
  WhiteBalanceOptions20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const WhiteBalanceOptions20& rhs) const;

  std::vector<WHITEBALANCEMODE> mode_;
  boost::optional<FloatRange> yrgain_;
  boost::optional<FloatRange> ybgain_;

};

class WhiteBalance20
{
 public:

  WhiteBalance20();
  WhiteBalance20(const boost::optional<WHITEBALANCEMODE>& mode, const boost::optional<float>& crgain_, const boost::optional<float>& cbgain_);
  WhiteBalance20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const WhiteBalance20& rhs) const;

  boost::optional<WHITEBALANCEMODE> mode_;
  boost::optional<float> crgain_;
  boost::optional<float> cbgain_;

};

class WhiteBalance
{
 public:

  WhiteBalance();
  WhiteBalance(const boost::optional<WHITEBALANCEMODE>& mode, const boost::optional<float>& crgain_, const boost::optional<float>& cbgain_);
  WhiteBalance(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const WhiteBalance& rhs) const;

  boost::optional<WHITEBALANCEMODE> mode_;
  boost::optional<float> crgain_;
  boost::optional<float> cbgain_;

};

class ImageStabilizationOptions
{
 public:

  ImageStabilizationOptions();
  ImageStabilizationOptions(const std::vector<IMAGESTABILIZATIONMODE>& mode, const boost::optional<FloatRange>& level);
  ImageStabilizationOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ImageStabilizationOptions& rhs) const;

  std::vector<IMAGESTABILIZATIONMODE> mode_;
  boost::optional<FloatRange> level_;

};

class ImageStabilization
{
 public:

  ImageStabilization();
  ImageStabilization(const boost::optional<IMAGESTABILIZATIONMODE>& mode, const boost::optional<float>& level);
  ImageStabilization(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ImageStabilization& rhs) const;

  boost::optional<IMAGESTABILIZATIONMODE> mode_;
  boost::optional<float> level_;

};

class DefoggingOptions
{
 public:

  DefoggingOptions();
  DefoggingOptions(const std::vector<DEFOGGINGMODE>& mode, const boost::optional<bool>& level);
  DefoggingOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const DefoggingOptions& rhs) const;

  std::vector<DEFOGGINGMODE> mode_;
  boost::optional<bool> level_;

};

class Defogging
{
 public:

  Defogging();
  Defogging(const boost::optional<DEFOGGINGMODE>& mode, const boost::optional<float>& level);
  Defogging(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Defogging& rhs) const;

  boost::optional<DEFOGGINGMODE> mode_;
  boost::optional<float> level_;

};

class ImagingSettings20
{
 public:

  ImagingSettings20();
  ImagingSettings20(const boost::optional<BacklightCompensation20>& backlightcompensation, const boost::optional<float>& brightness, const boost::optional<float>& colorsaturation, const boost::optional<float>& contrast, const boost::optional<Exposure20>& exposure, const boost::optional<FocusConfiguration20>& focus, const boost::optional<IRCUTFILTERMODE>& ircutfilter, const boost::optional<float>& sharpness, const boost::optional<WideDynamicRange20>& widedynamicrange, const boost::optional<WhiteBalance20>& whitebalance, const boost::optional<ImageStabilization>& imagestabilization, const boost::optional<IrCutFilterAutoAdjustment>& ircurfilterautoadjustment, const boost::optional<ToneCompensation>& tonecompensation, const boost::optional<Defogging>& defogging, const boost::optional<NoiseReduction>& noisereduction);
  ImagingSettings20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ImagingSettings20& rhs) const;

  boost::optional<BacklightCompensation20> backlightcompensation_;
  boost::optional<float> brightness_;
  boost::optional<float> colorsaturation_;
  boost::optional<float> contrast_;
  boost::optional<Exposure20> exposure_;
  boost::optional<FocusConfiguration20> focus_;
  boost::optional<IRCUTFILTERMODE> ircutfilter_;
  boost::optional<float> sharpness_;
  boost::optional<WideDynamicRange20> widedynamicrange_;
  boost::optional<WhiteBalance20> whitebalance_;
  boost::optional<ImageStabilization> imagestabilization_;
  boost::optional<IrCutFilterAutoAdjustment> ircurfilterautoadjustment_;
  boost::optional<ToneCompensation> tonecompensation_;
  boost::optional<Defogging> defogging_;
  boost::optional<NoiseReduction> noisereduction_;

};

class ExposureOptions20
{
 public:

  ExposureOptions20();
  ExposureOptions20(const std::vector<EXPOSUREMODE>& mode, const std::vector<EXPOSUREPRIORITY>& priority, const boost::optional<FloatRange>& minexposuretime, const boost::optional<FloatRange>& maxexposuretime, const boost::optional<FloatRange>& mingain, const boost::optional<FloatRange>& maxgain, const boost::optional<FloatRange>& miniris, const boost::optional<FloatRange>& maxiris, const boost::optional<FloatRange>& exposuretime, const boost::optional<FloatRange>& gain, const boost::optional<FloatRange>& iris);
  ExposureOptions20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ExposureOptions20& rhs) const;

  std::vector<EXPOSUREMODE> mode_;
  std::vector<EXPOSUREPRIORITY> priority_;
  boost::optional<FloatRange> minexposuretime_;
  boost::optional<FloatRange> maxexposuretime_;
  boost::optional<FloatRange> mingain_;
  boost::optional<FloatRange> maxgain_;
  boost::optional<FloatRange> miniris_;
  boost::optional<FloatRange> maxiris_;
  boost::optional<FloatRange> exposuretime_;
  boost::optional<FloatRange> gain_;
  boost::optional<FloatRange> iris_;

};

class Exposure
{
 public:

  Exposure();
  Exposure(const boost::optional<EXPOSUREMODE>& mode, const boost::optional<EXPOSUREPRIORITY>& priority, const boost::optional<Rectangle>& window, const boost::optional<float>& minexposuretime, const boost::optional<float>& maxexposuretime, const boost::optional<float>& mingain, const boost::optional<float>& maxgain, const boost::optional<float>& miniris, const boost::optional<float>& maxiris, const boost::optional<float>& exposuretime, const boost::optional<float>& gain, const boost::optional<float>& iris);
  Exposure(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Exposure& rhs) const;

  boost::optional<EXPOSUREMODE> mode_;
  boost::optional<EXPOSUREPRIORITY> priority_;
  boost::optional<Rectangle> window_;
  boost::optional<float> minexposuretime_;
  boost::optional<float> maxexposuretime_;
  boost::optional<float> mingain_;
  boost::optional<float> maxgain_;
  boost::optional<float> miniris_;
  boost::optional<float> maxiris_;
  boost::optional<float> exposuretime_;
  boost::optional<float> gain_;
  boost::optional<float> iris_;

};

class FocusConfiguration
{
 public:

  FocusConfiguration();
  FocusConfiguration(const boost::optional<AUTOFOCUSMODE>& autofocusmode, const boost::optional<float>& defaultspeed, const boost::optional<float>& nearlimit, const boost::optional<float>& farlimit);
  FocusConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const FocusConfiguration& rhs) const;

  boost::optional<AUTOFOCUSMODE> autofocusmode_;
  boost::optional<float> defaultspeed_;
  boost::optional<float> nearlimit_;
  boost::optional<float> farlimit_;

};

class WideDynamicRangeOptions20
{
 public:

  WideDynamicRangeOptions20();
  WideDynamicRangeOptions20(const std::vector<WIDEDYNAMICMODE>& mode, const boost::optional<FloatRange>& level);
  WideDynamicRangeOptions20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const WideDynamicRangeOptions20& rhs) const;

  std::vector<WIDEDYNAMICMODE> mode_;
  boost::optional<FloatRange> level_;

};

class WideDynamicRange
{
 public:

  WideDynamicRange();
  WideDynamicRange(const boost::optional<WIDEDYNAMICMODE>& mode, const boost::optional<float>& level);
  WideDynamicRange(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const WideDynamicRange& rhs) const;

  boost::optional<WIDEDYNAMICMODE> mode_;
  boost::optional<float> level_;

};

class ImagingSettings
{
 public:

  ImagingSettings();
  ImagingSettings(const boost::optional<BacklightCompensation>& backlightcompensation, const boost::optional<float>& brightness, const boost::optional<float>& colorsaturation, const boost::optional<float>& contrast, const boost::optional<Exposure>& exposure, const boost::optional<FocusConfiguration>& focus, const boost::optional<IRCUTFILTERMODE>& ircutfilter, const boost::optional<float>& sharpness, const boost::optional<WideDynamicRange>& widedynamicrange, const boost::optional<WhiteBalance>& whitebalance);
  ImagingSettings(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ImagingSettings& rhs) const;

  boost::optional<BacklightCompensation> backlightcompensation_;
  boost::optional<float> brightness_;
  boost::optional<float> colorsaturation_;
  boost::optional<float> contrast_;
  boost::optional<Exposure> exposure_;
  boost::optional<FocusConfiguration> focus_;
  boost::optional<IRCUTFILTERMODE> ircutfilter_;
  boost::optional<float> sharpness_;
  boost::optional<WideDynamicRange> widedynamicrange_;
  boost::optional<WhiteBalance> whitebalance_;

};

class BacklightCompensationOptions20
{
 public:

  BacklightCompensationOptions20();
  BacklightCompensationOptions20(const std::vector<BACKLIGHTCOMPENSATIONMODE>& mode, const boost::optional<FloatRange>& level);
  BacklightCompensationOptions20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const BacklightCompensationOptions20& rhs) const;

  std::vector<BACKLIGHTCOMPENSATIONMODE> mode_;
  boost::optional<FloatRange> level_;

};

class ImagingOptions20
{
 public:

  ImagingOptions20();
  ImagingOptions20(const boost::optional<BacklightCompensationOptions20>& backlightcompensation, const boost::optional<FloatRange>& brightness, const boost::optional<FloatRange>& colorsaturation, const boost::optional<FloatRange>& contrast, const boost::optional<ExposureOptions20>& exposure, const boost::optional<FocusOptions20>& focus, const std::vector<IRCUTFILTERMODE>& ircutfiltermodes, const boost::optional<FloatRange>& sharpness, const boost::optional<WideDynamicRangeOptions20>& widedynamicrange, const boost::optional<WhiteBalanceOptions20>& whitebalance, const boost::optional<ImageStabilizationOptions>& imagestabilization, const boost::optional<IrCutFilterAutoAdjustmentOptions>& ifcurfilterautoadjustment, const boost::optional<ToneCompensationOptions>& tonecompensationoptions, const boost::optional<DefoggingOptions>& defoggingoptions, const boost::optional<NoiseReductionOptions>& noisereductionoptions);
  ImagingOptions20(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ImagingOptions20& rhs) const;

  boost::optional<BacklightCompensationOptions20> backlightcompensation_;
  boost::optional<FloatRange> brightness_;
  boost::optional<FloatRange> colorsaturation_;
  boost::optional<FloatRange> contrast_;
  boost::optional<ExposureOptions20> exposure_;
  boost::optional<FocusOptions20> focus_;
  std::vector<IRCUTFILTERMODE> ircutfiltermodes_;
  boost::optional<FloatRange> sharpness_;
  boost::optional<WideDynamicRangeOptions20> widedynamicrange_;
  boost::optional<WhiteBalanceOptions20> whitebalance_;
  boost::optional<ImageStabilizationOptions> imagestabilization_;
  boost::optional<IrCutFilterAutoAdjustmentOptions> ircurfilterautoadjustment_;
  boost::optional<ToneCompensationOptions> tonecompensationoptions_;
  boost::optional<DefoggingOptions> defoggingoptions_;
  boost::optional<NoiseReductionOptions> noisereductionoptions_;

};

class ImagingCapabilities
{
 public:

  ImagingCapabilities();
  ImagingCapabilities(const boost::optional<std::string>& xaddr);
  ImagingCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ImagingCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;

};

class HostnameInformation
{
 public:

  HostnameInformation();
  HostnameInformation(const boost::optional<bool>& fromdhcp, const boost::optional<std::string>& name);
  HostnameInformation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const HostnameInformation& rhs) const;

  boost::optional<bool> fromdhcp_;
  boost::optional<std::string> name_;

};

class FocusMove
{
 public:

  FocusMove();
  FocusMove(const boost::optional<AbsoluteFocus>& absolute, const boost::optional<RelativeFocus>& relative, const boost::optional<ContinuousFocus>& continuous);
  FocusMove(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const FocusMove& rhs) const;

  boost::optional<AbsoluteFocus> absolute_;
  boost::optional<RelativeFocus> relative_;
  boost::optional<ContinuousFocus> continuous_;

};

class EventCapabilities
{
 public:

  EventCapabilities();
  EventCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& wssubscriptionpolicysupport, const boost::optional<bool>& wspullpointsupprt, const boost::optional<bool>& wspausablesubscriptionmanagerinterfacesupport);
  EventCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const EventCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;
  boost::optional<bool> wssubscriptionpolicysupport_;
  boost::optional<bool> wspullpointsupprt_;
  boost::optional<bool> wspausablesubscriptionmanagerinterfacesupport_;

};

class EAPMethodConfiguration
{
 public:

  EAPMethodConfiguration();
  EAPMethodConfiguration(const boost::optional<TLSConfiguration>& tlsconfiguration, const boost::optional<std::string>& password);
  EAPMethodConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const EAPMethodConfiguration& rhs) const;

  boost::optional<TLSConfiguration> tlsconfiguration_;
  boost::optional<std::string> password_;

};

class DynamicDNSInformation
{
 public:

  DynamicDNSInformation();
  DynamicDNSInformation(const boost::optional<DYNAMICDNSTYPE>& type, const boost::optional<std::string>& name, const boost::optional<Duration>& ttl);
  DynamicDNSInformation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const DynamicDNSInformation& rhs) const;

  boost::optional<DYNAMICDNSTYPE> type_;
  boost::optional<std::string> name_;
  boost::optional<Duration> ttl_;

};

class Dot1XConfiguration
{
 public:

  Dot1XConfiguration();
  Dot1XConfiguration(const boost::optional<std::string> dot1xconfigurationtoken, const boost::optional<std::string>& identity, const boost::optional<std::string>& anonymousid, const boost::optional<int>& eapmethod, const boost::optional<std::string>& certificateid, const boost::optional<EAPMethodConfiguration>& eapmethodconfiguration);
  Dot1XConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Dot1XConfiguration& rhs) const;

  boost::optional<std::string> dot1xconfigurationtoken_;
  boost::optional<std::string> identity_;
  boost::optional<std::string> anonymousid_;
  boost::optional<int> eapmethod_;
  boost::optional<std::string> certificateid_;
  boost::optional<EAPMethodConfiguration> eapmethodconfiguration_;

};

class DNSInformation
{
 public:

  DNSInformation();
  DNSInformation(const boost::optional<bool>& fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<IPAddress>& dnsfromdhcp, const std::vector<IPAddress>& dnsmanual);
  DNSInformation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const DNSInformation& rhs) const;

  boost::optional<bool> fromdhcp_;
  std::vector<std::string> searchdomain_;
  std::vector<IPAddress> dnsfromdhcp_;
  std::vector<IPAddress> dnsmanual_;

};

class DisplayCapabilities
{
 public:

  DisplayCapabilities();
  DisplayCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& fixedlayout);
  DisplayCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const DisplayCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;
  boost::optional<bool> fixedlayout_;

};

class DigitalInput
{
 public:

  DigitalInput();
  DigitalInput(const boost::optional<std::string>& token);
  DigitalInput(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const DigitalInput& rhs) const;

  boost::optional<std::string> token_;

};

class DeviceIOCapabilities
{
 public:

  DeviceIOCapabilities();
  DeviceIOCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<int>& videosources, const boost::optional<int>& videooutputs, const boost::optional<int>& audiosources, const boost::optional<int>& audiooutputs, const boost::optional<int>& relayoutputs);
  DeviceIOCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const DeviceIOCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;
  boost::optional<int> videosources_;
  boost::optional<int> videooutputs_;
  boost::optional<int> audiosources_;
  boost::optional<int> audiooutputs_;
  boost::optional<int> relayoutputs_;

};

class DeviceCapabilities
{
 public:

  DeviceCapabilities();
  DeviceCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<NetworkCapabilities>& network, const boost::optional<SystemCapabilities>& system, const boost::optional<IOCapabilities>& io, const boost::optional<SecurityCapabilities>& security);
  DeviceCapabilities(const pugi::xml_node& node);

  inline const std::string GetXAddr() const { return (xaddr_.is_initialized() ? *xaddr_ : std::string()); }

  // Network
  inline bool SupportsIPFilter() const { return (network_.is_initialized() && network_->SupportsIPFilter()); }
  inline bool SupportsZeroConfiguration() const { return (network_.is_initialized() && network_->SupportsZeroConfiguration()); }
  inline bool SupportsIPv6() const { return (network_.is_initialized() && network_->SupportsIPFilter()); }
  inline bool SupportsDynamicDNS() const { return (network_.is_initialized() && network_->SupportsDynamicDNS()); }
  inline bool SupportsDot11Configuration() const { return (network_.is_initialized() && network_->SupportsDot11Configuration()); }

  // System
  inline bool SupportsDiscoveryResolve() const { return (system_.is_initialized() && system_->SupportsDiscoveryResolve()); }
  inline bool SupportsDiscoveryBye() const { return (system_.is_initialized() && system_->SupportsDiscoveryBye()); }
  inline bool SupportsRemoteDiscovery() const { return (system_.is_initialized() && system_->SupportsRemoteDiscovery()); }
  inline bool SupportsSystemBackup() const { return (system_.is_initialized() && system_->SupportsSystemBackup()); }
  inline bool SupportsSystemLogging() const { return (system_.is_initialized() && system_->SupportsSystemLogging()); }
  inline bool SupportsFirmwareUpgrade() const { return (system_.is_initialized() && system_->SupportsFirmwareUpgrade()); }
  inline std::vector<OnvifVersion> SupportedVersions() const { return (system_.is_initialized() ? system_->SupportedVersions() : std::vector<OnvifVersion>()); }
  inline bool SupportsHTTPFirmwareUpgrade() const { return (system_.is_initialized() && system_->SupportsHTTPFirmwareUpgrade()); }
  inline bool SupportsHTTPSystemBackup() const { return (system_.is_initialized() && system_->SupportsHTTPSystemBackup()); }
  inline bool SupportsHTTPSystemLogging() const { return (system_.is_initialized() && system_->SupportsHTTPSystemLogging()); }
  inline bool SupportsHTTPSupportInformation() const { return (system_.is_initialized() && system_->SupportsHTTPSupportInformation()); }

  // Security
  inline bool SupportsTLS() const { return (security_.is_initialized() && security_->SupportsTLS()); }
  inline bool SupportsTLS11() const { return (security_.is_initialized() && security_->SupportsTLS11()); }
  inline bool SupportsTLS12() const { return (security_.is_initialized() && security_->SupportsTLS12()); }
  inline bool SupportsOnboardKeyGeneration() const { return (security_.is_initialized() && security_->SupportsOnboardKeyGeneration()); }
  inline bool SupportsAccessPolicyConfig() const { return (security_.is_initialized() && security_->SupportsAccessPolicyConfig()); }
  inline bool SupportsDefaultAccessPolicy() const { return (security_.is_initialized() && security_->SupportsDefaultAccessPolicy()); }
  inline bool SupportsX509Token() const { return (security_.is_initialized() && security_->SupportsX509Token()); }
  inline bool SupportsSAMLToken() const { return (security_.is_initialized() && security_->SupportsSAMLToken()); }
  inline bool SupportsKerberosToken() const { return (security_.is_initialized() && security_->SupportsKerberosToken()); }
  inline bool SupportsRELToken() const { return (security_.is_initialized() && security_->SupportsRELToken()); }
  inline bool SupportsTLS10() const { return (security_.is_initialized() && security_->SupportsTLS10()); }
  inline bool SupportsDot1X() const { return (security_.is_initialized() && security_->SupportsDot1X()); }
  inline std::vector<int> SupportedEAPMethods() const { return (security_.is_initialized() ? security_->SupportedEAPMethods() : std::vector<int>()); }
  inline bool SupportsRemoteUserHandling() const { return (security_.is_initialized() && security_->SupportsRemoteUserHandling()); }

  std::string ToXml(const std::string& name) const;

  bool operator==(const DeviceCapabilities& rhs) const;

  boost::optional<std::string> xaddr_;
  boost::optional<NetworkCapabilities> network_;
  boost::optional<SystemCapabilities> system_;
  boost::optional<IOCapabilities> io_;
  boost::optional<SecurityCapabilities> security_;

};

class DateTimeRange
{
 public:

  DateTimeRange();
  DateTimeRange(const boost::optional<ws::DateTime>& date, const boost::optional<ws::DateTime>& time);
  DateTimeRange(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const DateTimeRange& rhs) const;

  boost::optional<ws::DateTime> from_;
  boost::optional<ws::DateTime> until_;

};

class CertificateWithPrivateKey
{
 public:

  CertificateWithPrivateKey();
  CertificateWithPrivateKey(const boost::optional<std::string> certificateid, const boost::optional<BinaryData>& certificate, const boost::optional<BinaryData>& privatekey);
  CertificateWithPrivateKey(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const CertificateWithPrivateKey& rhs) const;

  boost::optional<std::string> certificateid_;
  boost::optional<BinaryData> certificate_;
  boost::optional<BinaryData> privatekey_;

};

class CertificateUsage
{
 public:

  CertificateUsage();
  CertificateUsage(const boost::optional<std::string>& usage, const boost::optional<bool>& critical);
  CertificateUsage(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const CertificateUsage& rhs) const;

  boost::optional<std::string> usage_;
  boost::optional<bool> critical_;

};

class CertificateStatus
{
 public:

  CertificateStatus();
  CertificateStatus(const boost::optional<std::string>& certificateid, const boost::optional<bool>& status);
  CertificateStatus(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const CertificateStatus& rhs) const;

  boost::optional<std::string> certificateid_;
  boost::optional<bool> status_;

};

class CertificateInformation
{
 public:

  CertificateInformation();
  CertificateInformation(const boost::optional<std::string> certificateid, const boost::optional<std::string>& issuerdn, const boost::optional<std::string>& subjectdn, const boost::optional<CertificateUsage>& keyusage, const boost::optional<CertificateUsage>& extendedkeyusage, const boost::optional<int>& keylength, const boost::optional<std::string>& version, const boost::optional<std::string>& serialnum, const boost::optional<std::string>& signaturealgorithm, const boost::optional<DateTimeRange>& validity);
  CertificateInformation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const CertificateInformation& rhs) const;

  boost::optional<std::string> certificateid_;
  boost::optional<std::string> issuerdn_;
  boost::optional<std::string> subjectdn_;
  boost::optional<CertificateUsage> keyusage_;
  boost::optional<CertificateUsage> extendedkeyusage_;
  boost::optional<int> keylength_;
  boost::optional<std::string> version_;
  boost::optional<std::string> serialnum_;
  boost::optional<std::string> signaturealgorithm_;
  boost::optional<DateTimeRange> validity_;

};

class Certificate
{
 public:

  Certificate();
  Certificate(const boost::optional<std::string>& certificateid, const boost::optional<BinaryData>& certificate);
  Certificate(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Certificate& rhs) const;

  boost::optional<std::string> certificateid_;
  boost::optional<BinaryData> certificate_;

};

class Capabilities
{
 public:

  Capabilities();
  Capabilities(const boost::optional<AnalyticsCapabilities>& analytics, const boost::optional<DeviceCapabilities>& device, const boost::optional<EventCapabilities>& events, const boost::optional<ImagingCapabilities>& imaging, const boost::optional<MediaCapabilities>& media, const boost::optional<PTZCapabilities>& ptz, const boost::optional<DeviceIOCapabilities>& deviceio, const boost::optional<DisplayCapabilities>& display, const boost::optional<RecordingCapabilities>& recording, const boost::optional<SearchCapabilities>& search, const boost::optional<ReplayCapabilities>& replay, const boost::optional<ReceiverCapabilities>& receiver, const boost::optional<AnalyticsDeviceCapabilities>& analyticsdevice);
  Capabilities(const pugi::xml_node& node);

  // Device
  inline const std::string GetDeviceXAddr() const { return (device_.is_initialized() ? device_->GetXAddr() : std::string()); }

  // Network
  inline bool SupportsIPFilter() const { return (device_.is_initialized() && device_->SupportsIPFilter()); }
  inline bool SupportsZeroConfiguration() const { return (device_.is_initialized() && device_->SupportsZeroConfiguration()); }
  inline bool SupportsIPv6() const { return (device_.is_initialized() && device_->SupportsIPFilter()); }
  inline bool SupportsDynamicDNS() const { return (device_.is_initialized() && device_->SupportsDynamicDNS()); }
  inline bool SupportsDot11Configuration() const { return (device_.is_initialized() && device_->SupportsDot11Configuration()); }

  // System
  utility::Version GetMaxSupportedVersion() const;
  inline bool SupportsDiscoveryResolve() const { return (device_.is_initialized() && device_->SupportsDiscoveryResolve()); }
  inline bool SupportsDiscoveryBye() const { return (device_.is_initialized() && device_->SupportsDiscoveryBye()); }
  inline bool SupportsRemoteDiscovery() const { return (device_.is_initialized() && device_->SupportsRemoteDiscovery()); }
  inline bool SupportsSystemBackup() const { return (device_.is_initialized() && device_->SupportsSystemBackup()); }
  inline bool SupportsSystemLogging() const { return (device_.is_initialized() && device_->SupportsSystemLogging()); }
  inline bool SupportsFirmwareUpgrade() const { return (device_.is_initialized() && device_->SupportsFirmwareUpgrade()); }
  inline std::vector<OnvifVersion> SupportedVersions() const { return (device_.is_initialized() ? device_->SupportedVersions() : std::vector<OnvifVersion>()); }
  inline bool SupportsHTTPFirmwareUpgrade() const { return (device_.is_initialized() && device_->SupportsHTTPFirmwareUpgrade()); }
  inline bool SupportsHTTPSystemBackup() const { return (device_.is_initialized() && device_->SupportsHTTPSystemBackup()); }
  inline bool SupportsHTTPSystemLogging() const { return (device_.is_initialized() && device_->SupportsHTTPSystemLogging()); }
  inline bool SupportsHTTPSupportInformation() const { return (device_.is_initialized() && device_->SupportsHTTPSupportInformation()); }

  // Security
  inline bool SupportsTLS() const { return (device_.is_initialized() && device_->SupportsTLS()); }
  inline bool SupportsTLS11() const { return (device_.is_initialized() && device_->SupportsTLS11()); }
  inline bool SupportsTLS12() const { return (device_.is_initialized() && device_->SupportsTLS12()); }
  inline bool SupportsOnboardKeyGeneration() const { return (device_.is_initialized() && device_->SupportsOnboardKeyGeneration()); }
  inline bool SupportsAccessPolicyConfig() const { return (device_.is_initialized() && device_->SupportsAccessPolicyConfig()); }
  inline bool SupportsDefaultAccessPolicy() const { return (device_.is_initialized() && device_->SupportsDefaultAccessPolicy()); }
  inline bool SupportsX509Token() const { return (device_.is_initialized() && device_->SupportsX509Token()); }
  inline bool SupportsSAMLToken() const { return (device_.is_initialized() && device_->SupportsSAMLToken()); }
  inline bool SupportsKerberosToken() const { return (device_.is_initialized() && device_->SupportsKerberosToken()); }
  inline bool SupportsRELToken() const { return (device_.is_initialized() && device_->SupportsRELToken()); }
  inline bool SupportsTLS10() const { return (device_.is_initialized() && device_->SupportsTLS10()); }
  inline bool SupportsDot1X() const { return (device_.is_initialized() && device_->SupportsDot1X()); }
  inline std::vector<int> SupportedEAPMethods() const { return (device_.is_initialized() ? device_->SupportedEAPMethods() : std::vector<int>()); }
  inline bool SupportsRemoteUserHandling() const { return (device_.is_initialized() && device_->SupportsRemoteUserHandling()); }

  // Media
  inline const std::string GetMediaXAddr() const { return (media_.is_initialized() ? media_->GetXAddr() : std::string()); }
  inline bool SupportsRTPMulticast() const { return (media_.is_initialized() && media_->SupportsRTPMulticast()); }
  inline bool SupportsRTPTCP() const { return (media_.is_initialized() && media_->SupportsRTPTCP()); }
  inline bool SupportsRTPRTSPTCP() const { return (media_.is_initialized() && media_->SupportsRTPRTSPTCP()); }
  inline int MaximumNumberOfProfiles() const { return (media_.is_initialized() ? media_->MaximumNumberOfProfiles() : 0); }

  // DeviceIO
  inline int NumVideoSources() const { return (deviceio_.is_initialized() && deviceio_->videosources_.is_initialized()) ? *deviceio_->videosources_ : 0; }
  inline int NumVideoOutputs() const { return (deviceio_.is_initialized() && deviceio_->videooutputs_.is_initialized()) ? *deviceio_->videooutputs_ : 0; }
  inline int NumAudioSources() const { return (deviceio_.is_initialized() && deviceio_->audiosources_.is_initialized()) ? *deviceio_->audiosources_ : 0; }
  inline int NumAudioOutputs() const { return (deviceio_.is_initialized() && deviceio_->audiooutputs_.is_initialized()) ? *deviceio_->audiooutputs_ : 0; }
  inline int NumRelayOutputs() const { return (deviceio_.is_initialized() && deviceio_->relayoutputs_.is_initialized()) ? *deviceio_->relayoutputs_ : 0; }

  std::string ToXml(const std::string& name) const;

  bool operator==(const Capabilities& rhs) const;

  boost::optional<AnalyticsCapabilities> analytics_;
  boost::optional<DeviceCapabilities> device_;
  boost::optional<EventCapabilities> events_;
  boost::optional<ImagingCapabilities> imaging_;
  boost::optional<MediaCapabilities> media_;
  boost::optional<PTZCapabilities> ptz_;
  boost::optional<DeviceIOCapabilities> deviceio_;
  boost::optional<DisplayCapabilities> display_;
  boost::optional<ReceiverCapabilities> receiver_;
  boost::optional<RecordingCapabilities> recording_;
  boost::optional<SearchCapabilities> search_;
  boost::optional<ReplayCapabilities> replay_;
  boost::optional<AnalyticsDeviceCapabilities> analyticsdevice_;

};

class AudioSource
{
 public:

  AudioSource();
  AudioSource(const boost::optional<std::string>& token, const boost::optional<int>& channels);
  AudioSource(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AudioSource& rhs) const;

  boost::optional<std::string> token_;
  boost::optional<int> channels_;

};

class AudioOutputConfigurationOptions
{
 public:

  AudioOutputConfigurationOptions();
  AudioOutputConfigurationOptions(const std::vector<std::string>& outputtokensavailable, const std::vector<std::string>& sendprimacyoptions, const boost::optional<IntRange>& outputlevelrange);
  AudioOutputConfigurationOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AudioOutputConfigurationOptions& rhs) const;

  std::vector<std::string> outputtokensavailable_;
  std::vector<std::string> sendprimacyoptions_;
  boost::optional<IntRange> outputlevelrange_;

};

class AudioEncoderConfigurationOption
{
 public:

  AudioEncoderConfigurationOption();
  AudioEncoderConfigurationOption(const boost::optional<AUDIOENCODING>& encoding, const boost::optional<IntList>& bitratelist, const boost::optional<IntList>& sampleratelist);
  AudioEncoderConfigurationOption(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AudioEncoderConfigurationOption& rhs) const;

  boost::optional<AUDIOENCODING> encoding_;
  boost::optional<IntList> bitratelist_;
  boost::optional<IntList> sampleratelist_;

};

class AudioEncoderConfigurationOptions
{
 public:

  AudioEncoderConfigurationOptions();
  AudioEncoderConfigurationOptions(const std::vector<AudioEncoderConfigurationOption>& options);
  AudioEncoderConfigurationOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AudioEncoderConfigurationOptions& rhs) const;

  std::vector<AudioEncoderConfigurationOption> options_;

};

class VideoSource
{
 public:

  VideoSource();
  VideoSource(const boost::optional<std::string>& token, const boost::optional<float>& framerate, const boost::optional<VideoResolution>& resolution, const boost::optional<ImagingSettings>& imaging, const boost::optional<ImagingSettings20>& extensionimaging);
  VideoSource(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const VideoSource& rhs) const;

  boost::optional<std::string> token_;
  boost::optional<float> framerate_;
  boost::optional<VideoResolution> resolution_;
  boost::optional<ImagingSettings> imaging_;
  boost::optional<ImagingSettings20> extensionimaging_;

};

class VideoOutputConfigurationOptions
{
 public:

  VideoOutputConfigurationOptions();
  VideoOutputConfigurationOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const VideoOutputConfigurationOptions& rhs) const;

};

class VideoOutputConfiguration
{
 public:

  VideoOutputConfiguration();
  VideoOutputConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<std::string>& outputtoken);
  VideoOutputConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const VideoOutputConfiguration& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<int> usecount_;
  boost::optional<std::string> token_;
  boost::optional<std::string> outputtoken_;

};

class VideoOutput
{
 public:

  VideoOutput();
  VideoOutput(const boost::optional<std::string>& token, const boost::optional<Layout>& layout, const boost::optional<VideoResolution>& resolution, const boost::optional<float>& refreshrate, const boost::optional<float>& aspectratio);
  VideoOutput(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const VideoOutput& rhs) const;

  boost::optional<std::string> token_;
  boost::optional<Layout> layout_;
  boost::optional<VideoResolution> resolution_;
  boost::optional<float> refreshrate_;
  boost::optional<float> aspectratio_;

};

class UserCredential
{
 public:

  UserCredential();
  UserCredential(const boost::optional<std::string>& username, const boost::optional<std::string>& password);
  UserCredential(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const UserCredential& rhs) const;

  boost::optional<std::string> username_;
  boost::optional<std::string> password_;

};

class User
{
 public:

  User();
  User(const boost::optional<std::string>& username, const boost::optional<std::string>& password, const boost::optional<USERLEVEL>& userlevel);
  User(const pugi::xml_node& node);

  void Clear();

  std::string ToXml(const std::string& name) const;

  bool operator==(const User& rhs) const;

  boost::optional<std::string> username_;
  boost::optional<std::string> password_;
  boost::optional<USERLEVEL> userlevel_;

};

class RecordingSummary
{
 public:

  RecordingSummary();
  RecordingSummary(const boost::optional<ws::DateTime>& datafrom, const boost::optional<ws::DateTime>& datauntil, const boost::optional<int>& numberrecordings);
  RecordingSummary(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RecordingSummary& rhs) const;

  boost::optional<ws::DateTime> datafrom_;
  boost::optional<ws::DateTime> datauntil_;
  boost::optional<int> numberrecordings_;

};

class TrackInformation
{
 public:

  TrackInformation();
  TrackInformation(const boost::optional<std::string>& tracktoken, const boost::optional<TRACKTYPE>& tracktype, const boost::optional<std::string>& description, const boost::optional<ws::DateTime>& datafrom, const boost::optional<ws::DateTime>& datato);
  TrackInformation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;
  std::string ToXml(const std::string& name, const bool namespaces) const;

  bool operator==(const TrackInformation& rhs) const;

  boost::optional<std::string> tracktoken_;
  boost::optional<TRACKTYPE> tracktype_;
  boost::optional<std::string> description_;
  boost::optional<ws::DateTime> datafrom_;
  boost::optional<ws::DateTime> datato_;

};

class TrackConfiguration
{
 public:

  TrackConfiguration();
  TrackConfiguration(const boost::optional<TRACKTYPE>& tracktype, const boost::optional<std::string>& description);
  TrackConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const TrackConfiguration& rhs) const;

  boost::optional<TRACKTYPE> tracktype_;
  boost::optional<std::string> description_;

};

class RecordingSourceInformation
{
 public:

  RecordingSourceInformation();
  RecordingSourceInformation(const boost::optional<std::string>& sourceid, const boost::optional<std::string>& name, const boost::optional<std::string>& location, const boost::optional<std::string>& description, const boost::optional<std::string>& address);
  RecordingSourceInformation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;
  std::string ToXml(const std::string& name, const bool namespaces) const;

  bool operator==(const RecordingSourceInformation& rhs) const;

  boost::optional<std::string> sourceid_;
  boost::optional<std::string> name_;
  boost::optional<std::string> location_;
  boost::optional<std::string> description_;
  boost::optional<std::string> address_;

};

class RecordingInformation
{
public:

  RecordingInformation();
  RecordingInformation(const boost::optional<std::string>& recordingtoken, const boost::optional<RecordingSourceInformation>& source, const boost::optional<ws::DateTime>& earliestrecording, const boost::optional<ws::DateTime>& latestrecording, const boost::optional<std::string>& content, const std::vector<TrackInformation>& track, const boost::optional<RECORDINGSTATUS>& recordingstatus);
  RecordingInformation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;
  std::string ToXml(const std::string& name, const bool namespaces) const;

  bool operator==(const RecordingInformation& rhs) const;

  boost::optional<std::string> recordingtoken_;
  boost::optional<RecordingSourceInformation> source_;
  boost::optional<ws::DateTime> earliestrecording_;
  boost::optional<ws::DateTime> latestrecording_;
  boost::optional<std::string> content_;
  std::vector<TrackInformation> track_;
  boost::optional<RECORDINGSTATUS> recordingstatus_;

};

class StorageReferencePath
{
 public:

  StorageReferencePath();
  StorageReferencePath(const boost::optional<std::string>& storagetoken, const boost::optional<std::string>& relativepath);
  StorageReferencePath(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const StorageReferencePath& rhs) const;

  boost::optional<std::string> storagetoken_;
  boost::optional<std::string> relativepath_;

};

class SourceReference
{
 public:

  SourceReference();
  SourceReference(const boost::optional<std::string>& type, const boost::optional<std::string>& token);
  SourceReference(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SourceReference& rhs) const;

  boost::optional<std::string> type_;
  boost::optional<std::string> token_;

};

class SearchScope
{
 public:

  SearchScope();
  SearchScope(const std::vector<SourceReference>& includedsources, const std::vector<std::string>& includedrecordings, const boost::optional<std::string>& recordinginformationfilter);
  SearchScope(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SearchScope& rhs) const;

  std::vector<SourceReference> includedsources_;
  std::vector<std::string> includedrecordings_;
  boost::optional<std::string> recordinginformationfilter_;

};

class FileProgress
{
 public:

  FileProgress();
  FileProgress(const boost::optional<std::string>& filename, const boost::optional<float>& progress);
  FileProgress(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const FileProgress& rhs) const;

  boost::optional<std::string> filename_;
  boost::optional<float> progress_;

};

class FindEventResult
{
public:

  FindEventResult();
  FindEventResult(const boost::optional<std::string>& recordingtoken, const boost::optional<std::string>& tracktoken, const boost::optional<ws::DateTime>& time, const boost::optional<Element>& event, const boost::optional<bool>& startstateevent);
  FindEventResult(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const FindEventResult& rhs) const;

  boost::optional<std::string> recordingtoken_;
  boost::optional<std::string> tracktoken_;
  boost::optional<ws::DateTime> time_;
  boost::optional<Element> event_;
  boost::optional<bool> startstateevent_;

};

class FindRecordingResultList
{
 public:

  FindRecordingResultList();
  FindRecordingResultList(const boost::optional<SEARCHSTATE>& searchstate_, const std::vector<RecordingInformation>& recordinginformation);
  FindRecordingResultList(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const FindRecordingResultList& rhs) const;

  boost::optional<SEARCHSTATE> searchstate_;
  std::vector<RecordingInformation> recordinginformation_;

};

class FindEventResultList
{
public:

  FindEventResultList();
  FindEventResultList(const boost::optional<SEARCHSTATE>& searchstate, const std::vector<FindEventResult>& result);
  FindEventResultList(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const FindEventResultList& rhs) const;

  boost::optional<SEARCHSTATE> searchstate_;
  std::vector<FindEventResult> result_;

};

class ArrayOfFileProgress
{
 public:

  ArrayOfFileProgress();
  ArrayOfFileProgress(const std::vector<FileProgress>& fileprogress);
  ArrayOfFileProgress(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ArrayOfFileProgress& rhs) const;

  std::vector<FileProgress> fileprogress_;

};

namespace search
{

class Capabilities
{
public:

  Capabilities();
  Capabilities(const boost::optional<bool>& metadatasearch, const boost::optional<bool>& generalstartevents);
  Capabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;
  Element ToElement(const std::string& name) const;

  bool operator==(const Capabilities& rhs) const;

  boost::optional<bool> metadatasearch_;
  boost::optional<bool> generalstartevents_;

};

}

namespace event
{

class Capabilities
{
 public:

  Capabilities();
  Capabilities(const boost::optional<bool>& wssubscriptionpolicysupport, const boost::optional<bool>& wspullpointsupport, const boost::optional<bool>& wspausablesubscriptionmanagerinterfacesupport, const boost::optional<int>& maxnotificationproducers, const boost::optional<int>& maxpullpoints, const boost::optional<bool>& persistentnotificationstorage);
  Capabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;
  Element ToElement(const std::string& name) const;

  bool operator==(const Capabilities& rhs) const;

  boost::optional<bool> wssubscriptionpolicysupport_;
  boost::optional<bool> wspullpointsupport_;
  boost::optional<bool> wspausablesubscriptionmanagerinterfacesupport_;
  boost::optional<int> maxnotificationproducers_;
  boost::optional<int> maxpullpoints_;
  boost::optional<bool> persistentnotificationstorage_;

};

}

namespace media
{

class VideoSourceMode
{
 public:

  VideoSourceMode();
  VideoSourceMode(const boost::optional<float>& maxframerate, const boost::optional<VideoResolution>& maxresolution, const std::vector<VIDEOENCODING>& encodings, const boost::optional<bool>& reboot, const boost::optional<std::string>& description, const boost::optional<std::string>& token, const boost::optional<bool>& enabled);
  VideoSourceMode(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const VideoSourceMode& rhs) const;

  boost::optional<float> maxframerate_;
  boost::optional<VideoResolution> maxresolution_;
  std::vector<VIDEOENCODING> encodings_;
  boost::optional<bool> reboot_;
  boost::optional<std::string> description_;
  boost::optional<std::string> token_;
  boost::optional<bool> enabled_;

};

class EncodingTypes
{
 public:

  EncodingTypes();
  EncodingTypes(const std::vector<VIDEOENCODING>& videoencodings);
  EncodingTypes(const std::string& encoding);

  std::string ToString() const;

  bool operator==(const EncodingTypes& rhs) const;

  std::vector<VIDEOENCODING> videoencodings_;

};

class Capabilities
{
 public:

  Capabilities();
  Capabilities(const boost::optional<ProfileCapabilities>& profilecapabilities, const boost::optional<StreamingCapabilities>& streamingcapabilities, const boost::optional<bool>& snapshoturi, const boost::optional<bool>& rotation, const boost::optional<bool>& videosourcemode, const boost::optional<bool>& osd, const boost::optional<bool>& exicompression);
  Capabilities(const pugi::xml_node& node);

  inline int MaximumNumberOfProfiles() const { return (profilecapabilities_.is_initialized() ? profilecapabilities_->MaximumNumberOfProfiles() : 0); }
  inline bool SupportsRTPMulticast() const { return streamingcapabilities_.is_initialized() && streamingcapabilities_->SupportsRTPMulticast(); }
  inline bool SupportsRTPTCP() const { return streamingcapabilities_.is_initialized() && streamingcapabilities_->SupportsRTPTCP(); }
  inline bool SupportsRTPRTSPTCP() const { return streamingcapabilities_.is_initialized() && streamingcapabilities_->SupportsRTPRTSPTCP(); }
  inline bool SupportsNonAggregateControl() const { return streamingcapabilities_.is_initialized() && streamingcapabilities_->SupportsNonAggregateControl(); }
  inline bool NoRTSPStreaming() const { return streamingcapabilities_.is_initialized() && streamingcapabilities_->NoRTSPStreaming(); }
  inline bool SupportsSnapshotUri() const { return (snapshoturi_.is_initialized() && (*snapshoturi_ == true)); }
  inline bool SupportsRotation() const { return (rotation_.is_initialized() && (*rotation_ == true)); }
  inline bool SupportsVideoSourceMode() const { return (videosourcemode_.is_initialized() && (*videosourcemode_ == true)); }
  inline bool SupportsOSD() const { return (osd_.is_initialized() && (*osd_ == true)); }
  inline bool SupportsEXICompression() const { return (exicompression_.is_initialized() && (*exicompression_ == true)); }

  std::string ToXml(const std::string& name) const;

  bool operator==(const Capabilities& rhs) const;

  boost::optional<ProfileCapabilities> profilecapabilities_;
  boost::optional<StreamingCapabilities> streamingcapabilities_;
  boost::optional<bool> snapshoturi_;
  boost::optional<bool> rotation_;
  boost::optional<bool> videosourcemode_;
  boost::optional<bool> osd_;
  boost::optional<bool> exicompression_;

};

}

namespace receiver
{

class Capabilities
{
 public:

  Capabilities();
  Capabilities(const boost::optional<bool>& rtpmulticast, const boost::optional<bool>& rtptcp, const boost::optional<bool>& rtprtsptcp, const boost::optional<int>& supportedreceivers, const boost::optional<int>& maximumrtspurilength);
  Capabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;
  Element ToElement(const std::string& name) const;

  bool operator==(const Capabilities& rhs) const;

  boost::optional<bool> rtpmulticast_;
  boost::optional<bool> rtptcp_;
  boost::optional<bool> rtprtsptcp_;
  boost::optional<int> supportedreceivers_;
  boost::optional<int> maximumrtspurilength_;

};

class ReceiverConfiguration
{
 public:

  ReceiverConfiguration();
  ReceiverConfiguration(const boost::optional<RECEIVERMODE>& mode, const boost::optional<std::string>& mediauri, const boost::optional<StreamSetup>& streamsetup);
  ReceiverConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ReceiverConfiguration& rhs) const;

  boost::optional<RECEIVERMODE> mode_;
  boost::optional<std::string> mediauri_;
  boost::optional<StreamSetup> streamsetup_;

};

class Receiver
{
 public:

  Receiver();
  Receiver(const boost::optional<std::string>& token, const boost::optional<ReceiverConfiguration>& configuration);
  Receiver(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Receiver& rhs) const;

  boost::optional<std::string> token_;
  boost::optional<ReceiverConfiguration> configuration_;

};

class ReceiverStateInformation
{
 public:

  ReceiverStateInformation();
  ReceiverStateInformation(const boost::optional<RECEIVERSTATE>& state, const boost::optional<bool>& autocreated);
  ReceiverStateInformation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ReceiverStateInformation& rhs) const;

  boost::optional<RECEIVERSTATE> state_;
  boost::optional<bool> autocreated_;

};

}

namespace recording
{

class EncodingTypes
{
 public:

  EncodingTypes();
  EncodingTypes(const std::vector<std::string>& encoding);
  EncodingTypes(const std::string& encoding);

  std::string ToString() const;

  bool operator==(const EncodingTypes& rhs) const;

  std::vector<std::string> encoding_;

};

class Capabilities
{
 public:

  Capabilities();
  Capabilities(const boost::optional<bool>& dynamicrecordings, const boost::optional<bool>& dynamictracks, const boost::optional<EncodingTypes>& encoding, const boost::optional<float>& maxrate, const boost::optional<float>& maxtotalrate, const boost::optional<int>& maxrecordings, const boost::optional<int>& maxrecordingjobs, const boost::optional<bool>& options, const boost::optional<bool>& metadatarecording, const boost::optional<StringAttrList>& supportedexportfileformats);
  Capabilities(const pugi::xml_node& node);
  
  inline bool GetDynamicRecordings() const { return (dynamicrecordings_.is_initialized() ? *dynamicrecordings_ : false); }
  inline bool GetDynamicTracks() const { return (dynamictracks_.is_initialized() ? *dynamicrecordings_ : false); }
  inline std::vector<std::string> GetEncodings() const { return (encoding_.is_initialized() ? encoding_->encoding_ : std::vector<std::string>()); }
  inline float GetMaxRate() const { return (maxrate_.is_initialized() ? *maxrate_ : 0.0f); }
  inline float GetMaxTotalRate() const { return (maxtotalrate_.is_initialized() ? *maxtotalrate_ : 0.0f); }
  inline int GetMaxRecordings() const { return (maxrecordings_.is_initialized() ? *maxrecordings_ : 0); }
  inline int GetMaxRecordingJobs() const { return (maxrecordingjobs_.is_initialized() ? *maxrecordingjobs_ : 0); }
  inline bool GetOptions() const { return (options_.is_initialized() ? *options_ : false); }
  inline bool GetMetadataRecording() const { return (metadatarecording_.is_initialized() ? *metadatarecording_ : false); }
  inline std::vector<std::string> GetSupportedExportFileFormats() const { return (supportedexportfileformats_.is_initialized() ? supportedexportfileformats_->items_ : std::vector<std::string>()); }

  std::string ToXml(const std::string& name) const;
  Element ToElement(const std::string& name) const;

  bool operator==(const Capabilities& rhs) const;

  boost::optional<bool> dynamicrecordings_;
  boost::optional<bool> dynamictracks_;
  boost::optional<EncodingTypes> encoding_;
  boost::optional<float> maxrate_;
  boost::optional<float> maxtotalrate_;
  boost::optional<int> maxrecordings_;
  boost::optional<int> maxrecordingjobs_;
  boost::optional<bool> options_;
  boost::optional<bool> metadatarecording_;
  boost::optional<StringAttrList> supportedexportfileformats_;

};

class RecordingConfiguration
{
 public:

  RecordingConfiguration();
  RecordingConfiguration(const boost::optional<RecordingSourceInformation>& source, const boost::optional<std::string>& content, const boost::optional<Duration>& maximumretentiontime);
  RecordingConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RecordingConfiguration& rhs) const;

  boost::optional<RecordingSourceInformation> source_;
  boost::optional<std::string> content_;
  boost::optional<Duration> maximumretentiontime_;

};

class GetTracksResponseItem
{
 public:

  GetTracksResponseItem();
  GetTracksResponseItem(const boost::optional<std::string>& tracktoken, const boost::optional<TrackConfiguration>& configuration);
  GetTracksResponseItem(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const GetTracksResponseItem& rhs) const;
  
  boost::optional<std::string> tracktoken_;
  boost::optional<TrackConfiguration> configuration_;

};

class GetTracksResponseList
{
 public:

  GetTracksResponseList();
  GetTracksResponseList(const std::vector<GetTracksResponseItem>& track);
  GetTracksResponseList(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const GetTracksResponseList& rhs) const;
  
  std::vector<GetTracksResponseItem> track_;

};

class GetRecordingsResponseItem
{
 public:

  GetRecordingsResponseItem();
  GetRecordingsResponseItem(const boost::optional<std::string>& recordingtoken, const boost::optional<RecordingConfiguration>& configuration, const boost::optional<GetTracksResponseList>& tracks);
  GetRecordingsResponseItem(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const GetRecordingsResponseItem& rhs) const;
  
  boost::optional<std::string> recordingtoken_;
  boost::optional<RecordingConfiguration> configuration_;
  boost::optional<GetTracksResponseList> tracks_;

};

class RecordingJobTrack
{
 public:

  RecordingJobTrack();
  RecordingJobTrack(const boost::optional<std::string>& sourcetag, const boost::optional<std::string>& destination);
  RecordingJobTrack(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RecordingJobTrack& rhs) const;

  boost::optional<std::string> sourcetag_;
  boost::optional<std::string> destination_;

};

class RecordingJobSource
{
 public:

  RecordingJobSource();
  RecordingJobSource(const boost::optional<SourceReference>& sourcetoken, const boost::optional<bool>& autocreatereceiver, const std::vector<RecordingJobTrack>& tracks);
  RecordingJobSource(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RecordingJobSource& rhs) const;

  boost::optional<SourceReference> sourcetoken_;
  boost::optional<bool> autocreatereceiver_;
  std::vector<RecordingJobTrack> tracks_;

};

class RecordingJobConfiguration
{
 public:

  RecordingJobConfiguration();
  RecordingJobConfiguration(const boost::optional<std::string>& recordingtoken, const boost::optional<RECORDINGJOBMODE>& mode, const boost::optional<uint64_t>& priority, const std::vector<RecordingJobSource>& source);
  RecordingJobConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RecordingJobConfiguration& rhs) const;

  boost::optional<std::string> recordingtoken_;
  boost::optional<RECORDINGJOBMODE> mode_;
  boost::optional<uint64_t> priority_;
  std::vector<RecordingJobSource> source_;

};

class GetRecordingJobsResponseItem
{
 public:

  GetRecordingJobsResponseItem();
  GetRecordingJobsResponseItem(const boost::optional<std::string>& jobtoken, const boost::optional<RecordingJobConfiguration>& jobconfiguration);
  GetRecordingJobsResponseItem(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const GetRecordingJobsResponseItem& rhs) const;
  
  boost::optional<std::string> jobtoken_;
  boost::optional<RecordingJobConfiguration> jobconfiguration_;

};

class RecordingJobStateTrack
{
 public:

  RecordingJobStateTrack();
  RecordingJobStateTrack(const boost::optional<std::string>& sourcetag, const boost::optional<std::string>& destination, const boost::optional<std::string>& error, const boost::optional<RECORDINGJOBSTATE>& state);
  RecordingJobStateTrack(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RecordingJobStateTrack& rhs) const;
  
  boost::optional<std::string> sourcetag_;
  boost::optional<std::string> destination_;
  boost::optional<std::string> error_;
  boost::optional<RECORDINGJOBSTATE> state_;

};

class RecordingJobStateTracks
{
 public:

  RecordingJobStateTracks();
  RecordingJobStateTracks(const std::vector<RecordingJobStateTrack>& track);
  RecordingJobStateTracks(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RecordingJobStateTracks& rhs) const;
  
  std::vector<RecordingJobStateTrack> track_;

};

class RecordingJobStateSource
{
 public:

  RecordingJobStateSource();
  RecordingJobStateSource(const boost::optional<SourceReference>& sourcetoken, const boost::optional<RECORDINGJOBSTATE>& state, const boost::optional<RecordingJobStateTracks>& tracks);
  RecordingJobStateSource(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RecordingJobStateSource& rhs) const;
  
  boost::optional<SourceReference> sourcetoken_;
  boost::optional<RECORDINGJOBSTATE> state_;
  boost::optional<RecordingJobStateTracks> tracks_;

};

class RecordingJobStateInformation
{
 public:

  RecordingJobStateInformation();
  RecordingJobStateInformation(const boost::optional<std::string>& recordingtoken, const boost::optional<RECORDINGJOBSTATE>& state, const std::vector<RecordingJobStateSource>& sources);
  RecordingJobStateInformation(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RecordingJobStateInformation& rhs) const;
  
  boost::optional<std::string> recordingtoken_;
  boost::optional<RECORDINGJOBSTATE> state_;
  std::vector<RecordingJobStateSource> sources_;

};

class TrackOptions
{
 public:

  TrackOptions();
  TrackOptions(const boost::optional<int>& sparetotal, const boost::optional<int>& sparevideo, const boost::optional<int>& spareaudio, const boost::optional<int>& sparemetadata);
  TrackOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const TrackOptions& rhs) const;
  
  boost::optional<int> sparetotal_;
  boost::optional<int> sparevideo_;
  boost::optional<int> spareaudio_;
  boost::optional<int> sparemetadata_;

};

class JobOptions
{
 public:

  JobOptions();
  JobOptions(const boost::optional<int>& spare, const boost::optional<StringAttrList>& compatiblesources);
  JobOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const JobOptions& rhs) const;
  
  boost::optional<int> spare_;
  boost::optional<StringAttrList> compatiblesources_;

};

class RecordingOptions
{
 public:

  RecordingOptions();
  RecordingOptions(const boost::optional<JobOptions>& job, const boost::optional<TrackOptions>& track);
  RecordingOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RecordingOptions& rhs) const;
  
  boost::optional<JobOptions> job_;
  boost::optional<TrackOptions> track_;

};

}

namespace replay
{

class Capabilities
{
 public:

  Capabilities();
  Capabilities(const boost::optional<bool>& reverseplayback, const boost::optional<FloatAttrList>& sessiontimeoutrange, const boost::optional<bool>& rtprtsptcp);
  Capabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;
  Element ToElement(const std::string& name) const;

  bool operator==(const Capabilities& rhs) const;

  boost::optional<bool> reverseplayback_;
  boost::optional<FloatAttrList> sessiontimeoutrange_;
  boost::optional<bool> rtprtsptcp_;

};

class ReplayConfiguration
{
 public:

  ReplayConfiguration();
  ReplayConfiguration(const boost::optional<Duration> sessiontimeout);
  ReplayConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ReplayConfiguration& rhs) const;

  boost::optional<Duration> sessiontimeout_;

};

}

namespace device
{

class MiscCapabilities
{
 public:

  MiscCapabilities();
  MiscCapabilities(const boost::optional<StringAttrList>& auxiliarycommands);
  MiscCapabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;
  Element ToElement(const std::string& name) const;

  bool operator==(const MiscCapabilities& rhs) const;

  boost::optional<StringAttrList> auxiliarycommands_;

};

class NetworkCapabilities
{
 public:

  NetworkCapabilities();
  NetworkCapabilities(const boost::optional<bool>& ipfilter, const boost::optional<bool>& zeroconfiguration, const boost::optional<bool>& ipversion6, const boost::optional<bool>& dynamicdns, const boost::optional<bool>& dot11configuration, const boost::optional<int>& dot1xconfigurations, const boost::optional<bool>& hostnamefromdhcp, const boost::optional<int>& ntp, const boost::optional<bool>& dhcpv6);
  NetworkCapabilities(const pugi::xml_node& node);

  inline bool SupportsIPFilter() const { return (ipfilter_.is_initialized() && (*ipfilter_ == true)); }
  inline bool SupportsZeroConfiguration() const { return (zeroconfiguration_.is_initialized() && (*zeroconfiguration_ == true)); }
  inline bool SupportsIPv6() const { return (ipversion6_.is_initialized() && (*ipversion6_ == true)); }
  inline bool SupportsDynamicDNS() const { return (dynamicdns_.is_initialized() && (*dynamicdns_ == true)); }
  inline bool SupportsDot11Configuration() const { return (dot11configuration_.is_initialized() && (*dot11configuration_ == true)); }
  inline int MaxDot1XConfigurations() const { return ((dot1xconfigurations_.is_initialized()) ? *dot1xconfigurations_ : std::numeric_limits<int>::max()); }
  inline bool SupportsHostnameFromDHCP() const { return (hostnamefromdhcp_.is_initialized() && (*hostnamefromdhcp_ == true)); }
  inline int MaxNTP() const { return ((ntp_.is_initialized()) ? *ntp_ : std::numeric_limits<int>::max()); }
  inline bool SupportsDHCPv6() const { return (hostnamefromdhcp_.is_initialized() && (*hostnamefromdhcp_ == true)); }

  std::string ToXml(const std::string& name) const;
  Element ToElement(const std::string& name) const;

  bool operator==(const NetworkCapabilities& rhs) const;

  boost::optional<bool> ipfilter_;
  boost::optional<bool> zeroconfiguration_;
  boost::optional<bool> ipversion6_;
  boost::optional<bool> dynamicdns_;
  boost::optional<bool> dot11configuration_;
  boost::optional<int> dot1xconfigurations_;
  boost::optional<bool> hostnamefromdhcp_;
  boost::optional<int> ntp_;
  boost::optional<bool> dhcpv6_;

};

class SecurityCapabilities
{
 public:

  SecurityCapabilities();
  SecurityCapabilities(const boost::optional<bool>& tls10, const boost::optional<bool>& tls11, const boost::optional<bool>& tls12, const boost::optional<bool>& onboardkeygeneration, const boost::optional<bool>& accesspolicyconfig, const boost::optional<bool>& defaultaccesspolicy, const boost::optional<bool>& dot1x, const boost::optional<bool>& remoteuserhandling, const boost::optional<bool>& x509token, const boost::optional<bool>& samltoken, const boost::optional<bool>& kerberostoken, const boost::optional<bool>& usernametoken, const boost::optional<bool>& httpdigest, const boost::optional<bool>& reltoken, const std::vector<int>& supportedeapmethods, const boost::optional<int>& maxusers, const boost::optional<int>& maxusernamelength, const boost::optional<int>& maxpasswordlength);
  SecurityCapabilities(const pugi::xml_node& node);

  inline bool SupportsTLS() const { return (SupportsTLS10() || SupportsTLS11() || SupportsTLS12()); }
  inline bool SupportsTLS10() const { return (tls10_.is_initialized() && (*tls10_ == true)); }
  inline bool SupportsTLS11() const { return (tls11_.is_initialized() && (*tls11_ == true)); }
  inline bool SupportsTLS12() const { return (tls12_.is_initialized() && (*tls12_ == true)); }
  inline bool SupportsOnboardKeyGeneration() const { return (onboardkeygeneration_.is_initialized() && (*onboardkeygeneration_ == true)); }
  inline bool SupportsAccessPolicyConfig() const { return (accesspolicyconfig_.is_initialized() && (*accesspolicyconfig_ == true)); }
  inline bool SupportsDefaultAccessPolicy() const { return (defaultaccesspolicy_.is_initialized() && (*defaultaccesspolicy_ == true)); }
  inline bool SupportsDot1X() const { return (dot1x_.is_initialized() && (*dot1x_ == true)); }
  inline bool SupportsRemoteUserHandling() const { return (remoteuserhandling_.is_initialized() && (*remoteuserhandling_ == true)); }
  inline bool SupportsX509Token() const { return (x509token_.is_initialized() && (*x509token_ == true)); }
  inline bool SupportsSAMLToken() const { return (samltoken_.is_initialized() && (*samltoken_ == true)); }
  inline bool SupportsKerberosToken() const { return (kerberostoken_.is_initialized() && (*kerberostoken_ == true)); }
  inline bool SupportsUsernameToken() const { return (usernametoken_.is_initialized() && (*usernametoken_ == true)); }
  inline bool SupportsHTTPDigest() const { return (httpdigest_.is_initialized() && (*httpdigest_ == true)); }
  inline bool SupportsRELToken() const { return (reltoken_.is_initialized() && (*reltoken_ == true)); }
  inline std::vector<int> SupportedEAPMethods() const { return supportedeapmethods_; }
  inline int MaxUsers() const { return (maxusers_.is_initialized() ? *maxusers_ : std::numeric_limits<int>::max()); }
  inline int MaxUserNameLength() const { return (maxusernamelength_.is_initialized() ? *maxusernamelength_ : std::numeric_limits<int>::max()); }
  inline int MaxPasswordLength() const { return (maxpasswordlength_.is_initialized() ? *maxpasswordlength_ : std::numeric_limits<int>::max()); }

  std::string ToXml(const std::string& name) const;
  Element ToElement(const std::string& name) const;

  bool operator==(const SecurityCapabilities& rhs) const;

  boost::optional<bool> tls10_;
  boost::optional<bool> tls11_;
  boost::optional<bool> tls12_;
  boost::optional<bool> onboardkeygeneration_;
  boost::optional<bool> accesspolicyconfig_;
  boost::optional<bool> defaultaccesspolicy_;
  boost::optional<bool> dot1x_;
  boost::optional<bool> remoteuserhandling_;
  boost::optional<bool> x509token_;
  boost::optional<bool> samltoken_;
  boost::optional<bool> kerberostoken_;
  boost::optional<bool> usernametoken_;
  boost::optional<bool> httpdigest_;
  boost::optional<bool> reltoken_;
  std::vector<int> supportedeapmethods_;
  boost::optional<int> maxusers_;
  boost::optional<int> maxusernamelength_;
  boost::optional<int> maxpasswordlength_;

};

class Service
{
 public:

  Service();
  Service(const std::string& Namespace, const std::string& xaddr, const boost::optional<Element>& capabilities, const OnvifVersion& version);
  Service(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const Service& rhs) const;

  boost::optional<std::string> Namespace_;
  boost::optional<std::string> xaddr_;
  boost::optional<Element> capabilities_;
  boost::optional<OnvifVersion> version_;

};

class StorageConfigurationData
{
 public:

  StorageConfigurationData();
  StorageConfigurationData(const boost::optional<std::string> localpath, const boost::optional<std::string>& storageuri, const boost::optional<UserCredential>& user, const boost::optional<std::string>& type);
  StorageConfigurationData(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const StorageConfigurationData& rhs) const;

  boost::optional<std::string> localpath_;
  boost::optional<std::string> storageuri_;
  boost::optional<UserCredential> user_;
  boost::optional<std::string> type_;

};

class StorageConfiguration
{
 public:

  StorageConfiguration();
  StorageConfiguration(const boost::optional<std::string> token, const boost::optional<StorageConfigurationData>& data);
  StorageConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const StorageConfiguration& rhs) const;

  boost::optional<std::string> token_;
  boost::optional<StorageConfigurationData> data_;

};

class SystemCapabilities
{
 public:

  SystemCapabilities();
  SystemCapabilities(const boost::optional<bool>& discoveryresolve, const boost::optional<bool>& discoverybye, const boost::optional<bool>& remotediscovery, const boost::optional<bool>& systembackup, const boost::optional<bool>& systemlogging, const boost::optional<bool>& firmwareupgrade, const boost::optional<bool>& httpfirmwareupgrade, const boost::optional<bool>& httpsystembackup, const boost::optional<bool>& httpsystemlogging, const boost::optional<bool>& httpsupportinformation, const boost::optional<bool>& storageconfiguration);
  SystemCapabilities(const pugi::xml_node& node);

  inline bool SupportsDiscoveryResolve() const { return (discoveryresolve_.is_initialized() && (*discoveryresolve_ == true)); }
  inline bool SupportsDiscoveryBye() const { return (discoverybye_.is_initialized() && (*discoverybye_ == true)); }
  inline bool SupportsRemoteDiscovery() const { return (remotediscovery_.is_initialized() && (*remotediscovery_ == true)); }
  inline bool SupportsSystemBackup() const { return (systembackup_.is_initialized() && (*systembackup_ == true)); }
  inline bool SupportsSystemLogging() const { return (systemlogging_.is_initialized() && (*systemlogging_ == true)); }
  inline bool SupportsFirmwareUpgrade() const { return (firmwareupgrade_.is_initialized() && (*firmwareupgrade_ == true)); }
  inline bool SupportsHTTPFirmwareUpgrade() const { return (httpfirmwareupgrade_.is_initialized() && (*httpfirmwareupgrade_ == true)); }
  inline bool SupportsHTTPSystemBackup() const { return (httpsystembackup_.is_initialized() && (*httpsystembackup_ == true)); }
  inline bool SupportsHTTPSystemLogging() const { return (httpsystemlogging_.is_initialized() && (*httpsystemlogging_ == true)); }
  inline bool SupportsHTTPSupportInformation() const { return (httpsupportinformation_.is_initialized() && (*httpsupportinformation_ == true)); }
  inline bool SupportsStorageConfiguration() const { return (storageconfiguration_.is_initialized() && (*storageconfiguration_ == true)); }

  std::string ToXml(const std::string& name) const;
  Element ToElement(const std::string& name) const;

  bool operator==(const SystemCapabilities& rhs) const;

  boost::optional<bool> discoveryresolve_;
  boost::optional<bool> discoverybye_;
  boost::optional<bool> remotediscovery_;
  boost::optional<bool> systembackup_;
  boost::optional<bool> systemlogging_;
  boost::optional<bool> firmwareupgrade_;
  boost::optional<bool> httpfirmwareupgrade_;
  boost::optional<bool> httpsystembackup_;
  boost::optional<bool> httpsystemlogging_;
  boost::optional<bool> httpsupportinformation_;
  boost::optional<bool> storageconfiguration_;

};

class DeviceServiceCapabilities
{
 public:

  DeviceServiceCapabilities();
  DeviceServiceCapabilities(const boost::optional<NetworkCapabilities>& network, const boost::optional<SecurityCapabilities>& security, const boost::optional<SystemCapabilities>& system, const boost::optional<MiscCapabilities>& misc);
  DeviceServiceCapabilities(const pugi::xml_node& node);

  // Network
  inline bool SupportsIPFilter() const { return (network_.is_initialized() && network_->SupportsIPFilter()); }
  inline bool SupportsZeroConfiguration() const { return (network_.is_initialized() && network_->SupportsZeroConfiguration()); }
  inline bool SupportsIPv6() const { return (network_.is_initialized() && network_->SupportsIPFilter()); }
  inline bool SupportsDynamicDNS() const { return (network_.is_initialized() && network_->SupportsDynamicDNS()); }
  inline bool SupportsDot11Configuration() const { return (network_.is_initialized() && network_->SupportsDot11Configuration()); }
  inline int MaxDot1XConfigurations() const { return ((network_.is_initialized()) ? network_->MaxDot1XConfigurations() : std::numeric_limits<int>::max()); }
  inline bool SupportsHostnameFromDHCP() const { return (network_.is_initialized() && network_->SupportsHostnameFromDHCP()); }
  inline int MaxNTP() const { return ((network_.is_initialized()) ? network_->MaxNTP() : std::numeric_limits<int>::max()); }
  inline bool SupportsDHCPv6() const { return (network_.is_initialized() && network_->SupportsDHCPv6()); }

  // Security
  inline bool SupportsTLS() const { return (security_.is_initialized() && security_->SupportsTLS()); }
  inline bool SupportsTLS10() const { return (security_.is_initialized() && security_->SupportsTLS10()); }
  inline bool SupportsTLS11() const { return (security_.is_initialized() && security_->SupportsTLS11()); }
  inline bool SupportsTLS12() const { return (security_.is_initialized() && security_->SupportsTLS12()); }
  inline bool SupportsOnboardKeyGeneration() const { return (security_.is_initialized() && security_->SupportsOnboardKeyGeneration()); }
  inline bool SupportsAccessPolicyConfig() const { return (security_.is_initialized() && security_->SupportsAccessPolicyConfig()); }
  inline bool SupportsDefaultAccessPolicy() const { return (security_.is_initialized() && security_->SupportsDefaultAccessPolicy()); }
  inline bool SupportsDot1X() const { return (security_.is_initialized() && security_->SupportsDot1X()); }
  inline bool SupportsRemoteUserHandling() const { return (security_.is_initialized() && security_->SupportsRemoteUserHandling()); }
  inline bool SupportsX509Token() const { return (security_.is_initialized() && security_->SupportsX509Token()); }
  inline bool SupportsSAMLToken() const { return (security_.is_initialized() && security_->SupportsSAMLToken()); }
  inline bool SupportsKerberosToken() const { return (security_.is_initialized() && security_->SupportsKerberosToken()); }
  inline bool SupportsUsernameToken() const { return (security_.is_initialized() && security_->SupportsUsernameToken()); }
  inline bool SupportsHTTPDigest() const { return (security_.is_initialized() && security_->SupportsHTTPDigest()); }
  inline bool SupportsRELToken() const { return (security_.is_initialized() && security_->SupportsRELToken()); }
  inline std::vector<int> SupportedEAPMethods() const { return (security_.is_initialized() ? security_->SupportedEAPMethods() : std::vector<int>()); }
  inline int MaxUsers() const { return (security_.is_initialized() ? security_->MaxUsers() : std::numeric_limits<int>::max()); }
  inline int MaxUserNameLength() const { return (security_.is_initialized() ? security_->MaxUserNameLength() : std::numeric_limits<int>::max()); }
  inline int MaxPasswordLength() const { return (security_.is_initialized() ? security_->MaxPasswordLength() : std::numeric_limits<int>::max()); }

  // System
  inline bool SupportsDiscoveryResolve() const { return (system_.is_initialized() && system_->SupportsDiscoveryResolve()); }
  inline bool SupportsDiscoveryBye() const { return (system_.is_initialized() && system_->SupportsDiscoveryBye()); }
  inline bool SupportsRemoteDiscovery() const { return (system_.is_initialized() && system_->SupportsRemoteDiscovery()); }
  inline bool SupportsSystemBackup() const { return (system_.is_initialized() && system_->SupportsSystemBackup()); }
  inline bool SupportsSystemLogging() const { return (system_.is_initialized() && system_->SupportsSystemLogging()); }
  inline bool SupportsFirmwareUpgrade() const { return (system_.is_initialized() && system_->SupportsFirmwareUpgrade()); }
  inline bool SupportsHTTPFirmwareUpgrade() const { return (system_.is_initialized() && system_->SupportsHTTPFirmwareUpgrade()); }
  inline bool SupportsHTTPSystemBackup() const { return (system_.is_initialized() && system_->SupportsHTTPSystemBackup()); }
  inline bool SupportsHTTPSystemLogging() const { return (system_.is_initialized() && system_->SupportsHTTPSystemLogging()); }
  inline bool SupportsHTTPSupportInformation() const { return (system_.is_initialized() && system_->SupportsHTTPSupportInformation()); }
  inline bool SupportsStorageConfiguration() const { return (system_.is_initialized() && system_->SupportsStorageConfiguration()); }

  std::string ToXml(const std::string& name) const;
  Element ToElement(const std::string& name) const;

  bool operator==(const DeviceServiceCapabilities& rhs) const;

  boost::optional<NetworkCapabilities> network_;
  boost::optional<SecurityCapabilities> security_;
  boost::optional<SystemCapabilities> system_;
  boost::optional<MiscCapabilities> misc_;

};

}

namespace deviceio
{

class AudioOutputConfiguration
{
 public:

  AudioOutputConfiguration();
  AudioOutputConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<std::string>& outputtoken, const boost::optional<std::string>& sendprimacy, const boost::optional<int>& outputlevel);
  AudioOutputConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const AudioOutputConfiguration& rhs) const;

  boost::optional<std::string> name_;
  boost::optional<int> usecount_;
  boost::optional<std::string> token_;
  boost::optional<std::string> outputtoken_;
  boost::optional<std::string> sendprimacy_;
  boost::optional<int> outputlevel_;

};

class Capabilities
{
 public:

  Capabilities();
  Capabilities(const boost::optional<int>& videosources, const boost::optional<int>& videooutputs, const boost::optional<int>& audiosources, const boost::optional<int>& audiooutputs, const boost::optional<int>& relayoutputs, const boost::optional<int>& serialports, const boost::optional<int>& digitalinputs, const boost::optional<bool>& digitalinputoptions);
  Capabilities(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;
  Element ToElement(const std::string& name) const;

  bool operator==(const Capabilities& rhs) const;

  boost::optional<int> videosources_;
  boost::optional<int> videooutputs_;
  boost::optional<int> audiosources_;
  boost::optional<int> audiooutputs_;
  boost::optional<int> relayoutputs_;
  boost::optional<int> serialports_;
  boost::optional<int> digitalinputs_;
  boost::optional<bool> digitalinputoptions_;

};

class DigitalInputConfigurationInputOptions
{
 public:

  DigitalInputConfigurationInputOptions();
  DigitalInputConfigurationInputOptions(const std::vector<DIGITALIDLESTATE>& idlestate);
  DigitalInputConfigurationInputOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const DigitalInputConfigurationInputOptions& rhs) const;

  std::vector<DIGITALIDLESTATE> idlestate_;

};

class ParityBitList
{
 public:

  ParityBitList();
  ParityBitList(const std::vector<PARITYBIT>& items);
  ParityBitList(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const ParityBitList& rhs) const;

  std::vector<PARITYBIT> items_;

};

class RelayOutputOptions
{
 public:

  RelayOutputOptions();
  RelayOutputOptions(const boost::optional<std::string>& token, const std::vector<RELAYMODE>& mode, const std::vector<int>& delaytimes, const boost::optional<bool>& discrete);
  RelayOutputOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const RelayOutputOptions& rhs) const;

  boost::optional<std::string> token_;
  std::vector<RELAYMODE> mode_;
  std::vector<int> delaytimes_;
  boost::optional<bool> discrete_;

};

class SerialData
{
 public:

  SerialData();
  SerialData(const boost::optional<std::string>& binary, const boost::optional<std::string>& string);
  SerialData(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SerialData& rhs) const;

  boost::optional<std::string> binary_;
  boost::optional<std::string> string_;

};

class SerialPort
{
 public:

  SerialPort();
  SerialPort(const boost::optional<std::string>& token);
  SerialPort(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SerialPort& rhs) const;

  boost::optional<std::string> token_;

};

class SerialPortConfiguration
{
 public:

  SerialPortConfiguration();
  SerialPortConfiguration(const boost::optional<std::string>& token, const boost::optional<SERIALPORTTYPE>& type, const boost::optional<int>& baudrate, const boost::optional<PARITYBIT>& paritybit, const boost::optional<int>& characterlength, const boost::optional<float>& stopbit);
  SerialPortConfiguration(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SerialPortConfiguration& rhs) const;

  boost::optional<std::string> token_;
  boost::optional<SERIALPORTTYPE> type_;
  boost::optional<int> baudrate_;
  boost::optional<PARITYBIT> paritybit_;
  boost::optional<int> characterlength_;
  boost::optional<float> stopbit_;

};

class SerialPortConfigurationOptions
{
 public:

  SerialPortConfigurationOptions();
  SerialPortConfigurationOptions(const boost::optional<IntList>& baudratelist, const boost::optional<ParityBitList>& paritybitlist, const boost::optional<IntList>& characterlengthlist, const boost::optional<FloatList>& stopbitlist, const boost::optional<std::string>& token);
  SerialPortConfigurationOptions(const pugi::xml_node& node);

  std::string ToXml(const std::string& name) const;

  bool operator==(const SerialPortConfigurationOptions& rhs) const;

  boost::optional<IntList> baudratelist_;
  boost::optional<ParityBitList> paritybitlist_;
  boost::optional<IntList> characterlengthlist_;
  boost::optional<FloatList> stopbitlist_;
  boost::optional<std::string> token_;

};

}

///// Prototypes /////

namespace deviceio
{

boost::optional<SERIALPORTTYPE> GetSerialPortType(const std::string& serialporttype);
std::string ToString(SERIALPORTTYPE serialporttype);

boost::optional<PARITYBIT> GetParityBit(const std::string& paritybit);
std::string ToString(PARITYBIT paritybit);

}

namespace receiver
{

boost::optional<RECEIVERMODE> GetReceiverMode(const std::string& receivermode);
std::string ToString(RECEIVERMODE receivermode);

boost::optional<RECEIVERSTATE> GetReceiverState(const std::string& receiverstate);
std::string ToString(RECEIVERSTATE receiverstate);

}

namespace recording
{
  
boost::optional<RECORDINGJOBMODE> GetRecordingJobMode(const std::string& recordingjobmode);
std::string ToString(RECORDINGJOBMODE recordingjobmode);

boost::optional<RECORDINGJOBSTATE> GetRecordingJobState(const std::string& recordingjobstate);
std::string ToString(RECORDINGJOBSTATE recordingjobstate);

}

boost::optional<TRACKTYPE> GetTrackType(const std::string& tracktype);
std::string ToString(const TRACKTYPE tracktype);

boost::optional<Duration> GetDuration(const std::string duration);

boost::optional<RECORDINGSTATUS> GetRecordingStatus(const std::string& recordingstatus);
std::string ToString(RECORDINGSTATUS recordingstatus);

boost::optional<TONECOMPENSATIONMODE> GetToneCompensationMode(const std::string& tonecompensationmode);
std::string ToString(TONECOMPENSATIONMODE tonecompensationmode);

boost::optional<SYSTEMLOGTYPE> GetSystemLogType(const std::string& systemlogtype);
std::string ToString(SYSTEMLOGTYPE systemlogtype);

boost::optional<STREAMTYPE> GetStreamType(const std::string& streamtype);
std::string ToString(STREAMTYPE streamtype);

boost::optional<SEARCHSTATE> GetSearchState(const std::string& searchstate);
std::string ToString(SEARCHSTATE searchstate);

boost::optional<SCOPEDEFINITION> GetScopeDefinitionType(const std::string& scopedefinition);
std::string ToString(SCOPEDEFINITION scopedefinition);

boost::optional<ROTATEMODE> GetRotateMode(const std::string& rotatemode);
std::string ToString(ROTATEMODE rotatemode);

boost::optional<REVERSEMODE> GetReverseMode(const std::string& reversemode);
std::string ToString(REVERSEMODE reversemode);

boost::optional<RELAYMODE> GetRelayMode(const std::string& relaymode);
std::string ToString(RELAYMODE relaymode);

boost::optional<RELAYLOGICALSTATE> GetRelayLogicalState(const std::string& relaylogicalstate);
std::string ToString(RELAYLOGICALSTATE relaylogicalstate);

boost::optional<RELAYIDLESTATE> GetRelayIdleState(const std::string& relayidlemode);
std::string ToString(RELAYIDLESTATE relayidlemode);

boost::optional<PTZPRESETTOUROPERATION> GetPTZPresetTourOperation(const std::string& ptzpresettouroperation);
std::string ToString(PTZPRESETTOUROPERATION ptzpresettouroperation);

boost::optional<NETWORKPROTOCOLTYPE> GetNetworkProtocolType(const std::string& mpeg4profile);
std::string ToString(NETWORKPROTOCOLTYPE mpeg4profile);

boost::optional<NETWORKHOSTTYPE> GetNetworkHostType(const std::string& networkhosttype);
std::string ToString(NETWORKHOSTTYPE networkhosttype);

boost::optional<MPEG4PROFILE> GetMpeg4Profile(const std::string& mpeg4profile);
std::string ToString(MPEG4PROFILE mpeg4profile);

boost::optional<MOVESTATUS> GetMoveStatus(const std::string& movestatus);
std::string ToString(MOVESTATUS movestatus);

boost::optional<METADATACOMPRESSIONTYPE> GetMetadataCompressionType(const std::string& metadatacompressiontype);
std::string ToString(METADATACOMPRESSIONTYPE metadatacompressiontype);

boost::optional<IRCUTFILTERMODE> GetIrCutFilterMode(const std::string& ircutfiltermode);
std::string ToString(IRCUTFILTERMODE ircutfiltermode);

boost::optional<IRCUTFILTERAUTOBOUNDARYTYPE> GetIrCutFilterAutoBoundaryType(const std::string& ircutfilterautoboundarytype);
std::string ToString(IRCUTFILTERAUTOBOUNDARYTYPE ircutfilterautoboundarytype);

boost::optional<IPV6DHCPCONFIGURATION> GetIPv6DHCPConfiguration(const std::string& ipv6dhcpconfiguration);
std::string ToString(IPV6DHCPCONFIGURATION ipv6dhcpconfiguration);

boost::optional<IPTYPE> GetIpType(const std::string& iptype);
std::string ToString(IPTYPE iptype);

boost::optional<IPADDRESSFILTERTYPE> GetIPAddressFilterType(const std::string& mpeg4profile);
std::string ToString(IPADDRESSFILTERTYPE mpeg4profile);

boost::optional<IMAGESTABILIZATIONMODE> GetImageStabilizationMode(const std::string& imagestabilizationmode);
std::string ToString(IMAGESTABILIZATIONMODE imagestabilizationmode);

boost::optional<H264PROFILE> GetH264Profile(const std::string& h264profile);
std::string ToString(H264PROFILE h264profile);

boost::optional<FACTORYDEFAULTTYPE> GetFactoryDefaultType(const std::string& factorydefaulttype);
std::string ToString(FACTORYDEFAULTTYPE factorydefaulttype);

boost::optional<EXPOSUREPRIORITY> GetExposurePriority(const std::string& exposurepriority);
std::string ToString(EXPOSUREPRIORITY exposurepriority);

boost::optional<EXPOSUREMODE> GetExposureMode(const std::string& exposuremode);
std::string ToString(EXPOSUREMODE exposuremode);

boost::optional<EFLIPMODE> GetEFlipMode(const std::string& eflipmode);
std::string ToString(EFLIPMODE eflipmode);

boost::optional<DYNAMICDNSTYPE> GetDynamicDNSType(const std::string& mpeg4profile);
std::string ToString(DYNAMICDNSTYPE mpeg4profile);

boost::optional<DUPLEX> GetDuplex(const std::string& mpeg4profile);
std::string ToString(DUPLEX mpeg4profile);

boost::optional<DOT11STATIONMODE> GetDot11StationMode(const std::string& dot11stationmode);
std::string ToString(DOT11STATIONMODE dot11stationmode);

boost::optional<DOT11SIGNALSTRENGTH> GetDot11SignalStrength(const std::string& dot11signalstrength);
std::string ToString(DOT11SIGNALSTRENGTH dot11signalstrength);

boost::optional<DOT11SECURITYMODE> GetDot11SecurityMode(const std::string& dot11securitymode);
std::string ToString(DOT11SECURITYMODE dot11securitymode);

boost::optional<DOT11CIPHER> GetDot11Cipher(const std::string& dot11cipher);
std::string ToString(DOT11CIPHER dot11cipher);

boost::optional<DOT11AUTHANDMANAGEMENTSUITE> GetDot11AuthAndManagementSuite(const std::string& dot11authandmanagementsuite);
std::string ToString(DOT11AUTHANDMANAGEMENTSUITE dot11authandmanagementsuite);

boost::optional<DISCOVERYMODE> GetDiscoveryMode(const std::string& discoverymode);
std::string ToString(DISCOVERYMODE discoverymode);

boost::optional<DIGITALIDLESTATE> GetDigitalIdleState(const std::string& digitalidlestate);
std::string ToString(DIGITALIDLESTATE digitalidlestate);

boost::optional<DEFOGGINGMODE> GetDefoggingMode(const std::string& defoggingmode);
std::string ToString(DEFOGGINGMODE defoggingmode);

boost::optional<DATETIMETYPE> GetDateTimeType(const std::string& datetimetype);
std::string ToString(DATETIMETYPE datetimetype);

boost::optional<CAPABILITYCATEGORY> GetCapabilityCategory(const std::string& capability);
std::string ToString(CAPABILITYCATEGORY capability);

boost::optional<BACKLIGHTCOMPENSATIONMODE> GetBacklightCompensationMode(const std::string& backlightcompensation);
std::string ToString(BACKLIGHTCOMPENSATIONMODE backlightcompensation);

boost::optional<AUTOFOCUSMODE> GetAutoFocusMode(const std::string& autofocusmode);
std::string ToString(AUTOFOCUSMODE autofocusmode);

boost::optional<AUDIOENCODING> GetAudioEncoding(const std::string& audioencoding);
std::string ToString(AUDIOENCODING audioencoding);

boost::optional<WIDEDYNAMICMODE> GetWideDynamicMode(const std::string& widedynamicrange);
std::string ToString(WIDEDYNAMICMODE widedynamicrange);

boost::optional<WHITEBALANCEMODE> GetWhiteBalanceMode(const std::string& whitebalancemode);
std::string ToString(WHITEBALANCEMODE whitebalancemode);

boost::optional<VIDEOENCODING> GetVideoEncoding(const std::string& text);
std::string ToString(VIDEOENCODING videoencoding);

boost::optional<USERLEVEL> GetUserLevel(const std::string& userlevel);
std::string ToString(USERLEVEL userlevel);

boost::optional<TRANSPORTPROTOCOL> GetTransportProtocol(const std::string& transportprotocol);
std::string ToString(TRANSPORTPROTOCOL transportprotocol);

///// Functions /////

std::string UrlDecode(const std::string& input);
std::string TrimNamespace(const std::string& text);
std::string ToString(const bool value);

template<class T>
inline std::string ToXmlAttributeEnum(const std::string& name, const boost::optional<T>& value)
{
  if (value.is_initialized())
    return (" " + name + "=\"" + ToString(*value) + "\"");
  else
    return std::string();
}

template<class T>
inline std::string ToXmlAttribute(const std::string& name, const boost::optional<T>& value)
{
  if (value.is_initialized())
    return (" " + name + "=\"" + std::to_string(*value) + "\"");
  else
    return std::string();
}

template<>
inline std::string ToXmlAttribute<std::string>(const std::string& name, const boost::optional<std::string>& value)
{
  if (value.is_initialized())
    return (" " + name + "=\"" + *value + "\"");
  else
    return std::string();
}

template<>
inline std::string ToXmlAttribute<bool>(const std::string& name, const boost::optional<bool>& value)
{
  if (value.is_initialized())
    return (" " + name + "=\"" + (*value ? "true\"" : "false\""));
  else
    return std::string();
}

template<class T>
inline std::string ToXmlAttributeList(const std::string& name, const std::vector<T>& vector)
{
  std::vector<std::string> result;
  for (const auto element : vector)
  {
    result.push_back(std::to_string(element));

  }
  return ToXmlAttribute(name, boost::optional<std::string>(boost::algorithm::join(result, std::string(" "))));
}

template<>
inline std::string ToXmlAttributeList<std::string>(const std::string& name, const std::vector<std::string>& vector)
{
  return ToXmlAttribute(name, boost::optional<std::string>(boost::algorithm::join(vector, std::string(" "))));
}

template<class T>
inline std::string ToXmlClass(const std::string& name, const boost::optional<T>& value, const bool namespaces)
{
  if (value.is_initialized())
    return value->ToXml(name, namespaces);
  else
    return std::string();
}

template<class T>
inline std::string ToXmlClass(const std::string& name, const boost::optional<T>& value)
{
  if (value.is_initialized())
    return value->ToXml(name);
  else
    return std::string();
}

template<class T>
inline std::string ToXmlEnum(const std::string& name, T value)
{
  return ("<" + name + ">" + ToString(value) + "</" + name + ">");
}

template<class T>
inline std::string ToXmlEnum(const std::string& name, const boost::optional<T>& value)
{
  if (value.is_initialized())
    return ToXmlEnum(name, *value);
  else
    return std::string();
}

template<class T>
inline std::string ToXml(const std::string& name, const boost::optional<T>& value)
{
  if (value.is_initialized())
    return ("<" + name + ">" + std::to_string(*value) + "</" + name + ">");
  else
    return std::string();
}

template<>
inline std::string ToXml<std::string>(const std::string& name, const boost::optional<std::string>& text)
{
  if (text.is_initialized())
    return ("<" + name + ">" + *text + "</" + name + ">");
  else
    return std::string();
}

template<>
inline std::string ToXml<Duration>(const std::string& name, const boost::optional<Duration>& duration)
{
  if (duration.is_initialized())
    return ("<" + name + ">" + duration->ToString() + "</" + name + ">");
  else
    return std::string();
}

template<>
inline std::string ToXml<bool>(const std::string& name, const boost::optional<bool>& value)
{
  if (value.is_initialized())
    return ("<" + name + ">" + (*value ? "true</" : "false</") + name + ">");
  else
    return std::string();
}

template<class T>
inline std::string ToXml(const std::string& name, const std::vector<T>& vector)
{
  std::string result;
  std::for_each(vector.begin(), vector.end(), [&name, &result](const T& element) { result += element.ToXml(name); });
  return result;
}

template<>
inline std::string ToXml<std::string>(const std::string& name, const std::vector<std::string>& vector)
{
  std::string result;
  std::for_each(vector.begin(), vector.end(), [&name, &result](const std::string& element) { result += ToXml(name, boost::optional<std::string>(element)); });
  return result;
}

template<class T>
inline std::string ToXmlList(const std::string& name, const std::vector<T>& vector)
{
  std::vector<std::string> result;
  for (const auto element : vector)
  {
    result.push_back(std::to_string(element));

  }
  return ToXml(name, boost::optional<std::string>(boost::algorithm::join(result, std::string(" "))));
}

template<>
inline std::string ToXmlList<std::string>(const std::string& name, const std::vector<std::string>& vector)
{
  return ToXml(name, boost::optional<std::string>(boost::algorithm::join(vector, std::string(" "))));
}

}

#endif
