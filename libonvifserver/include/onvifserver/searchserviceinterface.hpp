// searchserviceinterface.hpp
//

#ifndef IDP2PZQ93HUADMK60FAQJ8Q9QK414UZGUF
#define IDP2PZQ93HUADMK60FAQJ8Q9QK414UZGUF

///// Includes /////

#include <memory>
#include <onviftypes/onviftypes.hpp>
#include <string>

#include "onvifservice.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace search
{

///// Globals /////

extern const std::string SEARCHNAMESPACEURI;

extern const std::string ENDSEARCH;
extern const std::string FINDEVENTS;
extern const std::string FINDRECORDINGS;
extern const std::string GETEVENTSEARCHRESULTS;
extern const std::string GETRECORDINGINFORMATION;
extern const std::string GETRECORDINGSEARCHRESULTS;
extern const std::string GETRECORDINGSUMMARY;
extern const std::string GETSERVICECAPABILITIES;

///// Classes ////

class SearchServiceInterface : public OnvifService
{
public:

  SearchServiceInterface(const int onvifauthenticationtimediff);
  virtual ~SearchServiceInterface();

  int Init();
  void Destroy();

  virtual onvif::SearchCapabilities Capabilities(const std::string& localaddress, const uint16_t port) const = 0;
  virtual onvif::search::Capabilities ServiceCapabilities() const = 0;

  inline virtual const std::string& GetXAddr() const override { return XADDR_; }

protected:

  virtual boost::shared_future<ServerResponse> OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata) override;

  virtual ServerResponse EndSearch(const uint64_t searchtoken) = 0;
  virtual ServerResponse FindEvents(const onvif::ws::DateTime& startpoint, const boost::optional<onvif::ws::DateTime>& endpoint, const onvif::SearchScope& scope, const std::string& eventfilter, const bool includestartstate, const boost::optional<int>& maxmatches, const onvif::Duration& keepalivetime) = 0;
  virtual ServerResponse FindRecordings(const onvif::SearchScope& scope, const boost::optional<int>& maxmatches, const onvif::Duration& keepalivetime) = 0;
  virtual boost::shared_future<ServerResponse> GetEventSearchResults(const uint64_t searchtoken, const boost::optional<int>& minresults, const boost::optional<int>& maxresults, const boost::optional<onvif::Duration>& waittime) = 0;
  virtual ServerResponse GetRecordingInformation(const uint64_t recordingtoken) = 0;
  virtual ServerResponse GetRecordingSearchResults(const uint64_t searchtoken, const boost::optional<int>& minresults, const boost::optional<int>& maxresults, const boost::optional<onvif::Duration>& waittime) = 0;
  virtual ServerResponse GetRecordingSummary() = 0;
  virtual ServerResponse GetServiceCapabilities() = 0;

  static const std::string XADDR_;

private:


};

}

}

}

#endif
