// manageonvifmediametadataconfigurationwindow.h
//

#ifndef ID75N3SBS1X2V767777777S2OHNSNIKPL7
#define ID75N3SBS1X2V767777777S2OHNSNIKPL7

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <QDialog>
#include <string>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <vector>

#include "ui_manageonvifmediametadataconfigurationwindow.h"

///// Declarations /////

namespace onvif
{
namespace event
{
class EventClient;
}
namespace media
{
class MediaClient;
}
}

///// Namespaces /////

namespace client
{

///// Classes /////

class ManageONVIFMediaMetadataConfigurationWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaMetadataConfigurationWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::event::EventClient>& onvifevent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const std::string& token);
  virtual ~ManageONVIFMediaMetadataConfigurationWindow();

  const QString& GetName() const { return name_; }

 protected:



 private:

  void InitDialog(const onvif::MetadataConfiguration& metadataconfiguration, const onvif::MetadataConfigurationOptions& metadataconfigurationoptions, const std::vector<std::string>& topicnamespacelocation, const bool fixedtopicset, const onvif::ws::TopicSet& topicset, const std::vector<std::string>& producerpropertiesfilterdialect, const std::vector<std::string>& messagecontentschemalocation);
  void AddContentFilter(const onvif::ws::MessageContent& messagecontent);
  void AddTopicFilter(const onvif::ws::TopicExpression& topicexpression);
  bool SupportsMessageContentFiltering() const;

  const std::string token_;

  Ui::ManageONVIFMediaMetadataConfigurationWindow ui_;

  const onvif::Capabilities devicecapabilities_;
  const boost::shared_ptr<onvif::event::EventClient> onvifevent_;
  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection getmetadataconfigurationconnection_;
  onvif::ScopedConnection getmetadataconfigurationoptionsconnection_;
  onvif::ScopedConnection geteventpropertiesconnection_;
  onvif::ScopedConnection setmetadataconfigurationconnection_;

  std::vector<std::string> topicexpressiondialect_;
  std::vector<std::string> messagecontentfilterdialect_;

  QString name_;

 private slots:

  void on_checkevents_toggled(bool toggle);
  void on_buttoncontentaddfilter_clicked();
  void on_buttoncontentdeletefilter_clicked();
  void on_buttontopicaddfilter_clicked();
  void on_buttontopicdeletefilter_clicked();
  void on_buttonok_clicked();

};

}

#endif
