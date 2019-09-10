// manageonvifmediametadataconfigurationwindow.cpp
//

///// Defines /////

#define NOMINMAX

///// Includes /////

#include "monocleclient/manageonvifmediametadataconfigurationwindow.h"

#include <boost/asio.hpp>
#include <onvifclient/eventclient.hpp>
#include <onvifclient/mediaclient.hpp>
#include <QMessageBox>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Methods /////

ManageONVIFMediaMetadataConfigurationWindow::ManageONVIFMediaMetadataConfigurationWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::event::EventClient>& onvifevent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const std::string& token) :
  QDialog(parent),
  devicecapabilities_(devicecapabilities),
  onvifevent_(onvifevent),
  onvifmedia_(onvifmedia),
  token_(token)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  ui_.editport->setValidator(new QIntValidator(1, 65535, this));
  ui_.edittoken->setText(QString::fromStdString(token));

  if (!onvifevent_->IsInitialised())
  {
    QMessageBox::warning(this, tr("Error"), QString("Event service not initialised"), QMessageBox::Ok);
    return;
  }
  
  std::shared_ptr<unsigned int> count = std::make_shared<unsigned int>(3);
  std::shared_ptr<onvif::MetadataConfiguration> metadataconfiguration = std::make_shared<onvif::MetadataConfiguration>();
  std::shared_ptr<onvif::MetadataConfigurationOptions> metadataconfigurationoptions = std::make_shared<onvif::MetadataConfigurationOptions>();
  std::shared_ptr< std::vector<std::string> > topicnamespacelocation = std::make_shared< std::vector<std::string> >();
  std::shared_ptr< bool > fixedtopicset = std::make_shared< bool >();
  std::shared_ptr< onvif::ws::TopicSet > topicset = std::make_shared< onvif::ws::TopicSet >();
  std::shared_ptr< std::vector<std::string> > producerpropertiesfilterdialect = std::make_shared< std::vector<std::string> >();
  std::shared_ptr< std::vector<std::string> > messagecontentschemalocation = std::make_shared< std::vector<std::string> >();
  getmetadataconfigurationconnection_ = onvifmedia_->GetMetadataConfigurationCallback(token, [this, count, metadataconfiguration, metadataconfigurationoptions, topicnamespacelocation, fixedtopicset, topicset, producerpropertiesfilterdialect, messagecontentschemalocation](const onvif::media::GetMetadataConfigurationResponse& getmetadataconfigurationresponse)
  {
    if (getmetadataconfigurationresponse.Error() || !getmetadataconfigurationresponse.configuration_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getmetadataconfigurationresponse.Message()), QMessageBox::Ok);
      return;
    }

    *metadataconfiguration = *getmetadataconfigurationresponse.configuration_;

    if ((--(*count)) == 0)
    {
      InitDialog(*metadataconfiguration, *metadataconfigurationoptions, *topicnamespacelocation, *fixedtopicset, *topicset, *producerpropertiesfilterdialect, *messagecontentschemalocation);

    }
  });
  
  getmetadataconfigurationoptionsconnection_ = onvifmedia_->GetMetadataConfigurationOptionsCallback(token_, boost::none, [this, count, metadataconfiguration, metadataconfigurationoptions, topicnamespacelocation, fixedtopicset, topicset, producerpropertiesfilterdialect, messagecontentschemalocation](const onvif::media::GetMetadataConfigurationOptionsResponse& getmetadataconfigurationoptionsresponse)
  {
    if (getmetadataconfigurationoptionsresponse.Error() || !getmetadataconfigurationoptionsresponse.options_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getmetadataconfigurationoptionsresponse.Message()), QMessageBox::Ok);
      return;
    }

    *metadataconfigurationoptions = *getmetadataconfigurationoptionsresponse.options_;

    if ((--(*count)) == 0)
    {
      InitDialog(*metadataconfiguration, *metadataconfigurationoptions, *topicnamespacelocation, *fixedtopicset, *topicset, *producerpropertiesfilterdialect, *messagecontentschemalocation);

    }
  });

  geteventpropertiesconnection_ = onvifevent_->GetEventPropertiesCallback([this, count, metadataconfiguration, metadataconfigurationoptions, topicnamespacelocation, fixedtopicset, topicset, producerpropertiesfilterdialect, messagecontentschemalocation](const onvif::event::GetEventPropertiesResponse& geteventpropertiesresponse)
  {
    if (geteventpropertiesresponse.Error() || !geteventpropertiesresponse.fixedtopicset_.is_initialized() || !geteventpropertiesresponse.topicset_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(geteventpropertiesresponse.Message()), QMessageBox::Ok);
      return;
    }

    *topicnamespacelocation = geteventpropertiesresponse.topicnamespacelocation_;
    *fixedtopicset = *geteventpropertiesresponse.fixedtopicset_;
    *topicset = *geteventpropertiesresponse.topicset_;
    topicexpressiondialect_ = geteventpropertiesresponse.topicexpressiondialect_;
    messagecontentfilterdialect_ = geteventpropertiesresponse.messagecontentfilterdialect_;
    *producerpropertiesfilterdialect = geteventpropertiesresponse.producerpropertiesfilterdialect_;
    *messagecontentschemalocation = geteventpropertiesresponse.messagecontentschemalocation_;

    if ((--(*count)) == 0)
    {
      InitDialog(*metadataconfiguration, *metadataconfigurationoptions, *topicnamespacelocation, *fixedtopicset, *topicset, *producerpropertiesfilterdialect, *messagecontentschemalocation);

    }
  });
}

ManageONVIFMediaMetadataConfigurationWindow::~ManageONVIFMediaMetadataConfigurationWindow()
{
  if (setmetadataconfigurationconnection_.IsConnected())
  {
    QApplication::restoreOverrideCursor();

  }
}

void ManageONVIFMediaMetadataConfigurationWindow::InitDialog(const onvif::MetadataConfiguration& metadataconfiguration, const onvif::MetadataConfigurationOptions& metadataconfigurationoptions, const std::vector<std::string>& topicnamespacelocation, const bool fixedtopicset, const onvif::ws::TopicSet& topicset, const std::vector<std::string>& producerpropertiesfilterdialect, const std::vector<std::string>& messagecontentschemalocation)
{
  // Details
  ui_.editname->setEnabled(true);
  if (metadataconfiguration.name_.is_initialized())
  {
    ui_.editname->setText(QString::fromStdString(*metadataconfiguration.name_));
    
  }

  if (metadataconfiguration.token_.is_initialized())
  {
    ui_.edittoken->setText(QString::fromStdString(*metadataconfiguration.token_));

  }

  if (metadataconfiguration.usecount_.is_initialized())
  {
    ui_.editusecount->setText(QString::number(*metadataconfiguration.usecount_));

  }

  // Multicast
  if (devicecapabilities_.SupportsRTPMulticast())
  {
    // Fill in the dialog as best as we can
    if (metadataconfiguration.multicast_.is_initialized())
    {
      if (metadataconfiguration.multicast_->address_.is_initialized() && metadataconfiguration.multicast_->address_->type_.is_initialized())
      {
        if ((*metadataconfiguration.multicast_->address_->type_ == onvif::IPTYPE_IPV4) && metadataconfiguration.multicast_->address_->ipv4address_.is_initialized())
        {
          ui_.editaddress->setText(QString::fromStdString(*metadataconfiguration.multicast_->address_->ipv4address_));

        }
        else if ((*metadataconfiguration.multicast_->address_->type_ == onvif::IPTYPE_IPV6) && metadataconfiguration.multicast_->address_->ipv6address_.is_initialized())
        {
          ui_.editaddress->setText(QString::fromStdString(*metadataconfiguration.multicast_->address_->ipv6address_));

        }
      }

      if (metadataconfiguration.multicast_->port_.is_initialized())
      {
        ui_.editport->setText(QString::number(*metadataconfiguration.multicast_->port_));

      }

      if (metadataconfiguration.multicast_->ttl_.is_initialized())
      {
        ui_.spinttl->setValue(*metadataconfiguration.multicast_->ttl_);


      }

      if (metadataconfiguration.multicast_->autostart_.is_initialized())
      {
        ui_.checkautostart->setChecked(*metadataconfiguration.multicast_->autostart_);

      }
    }

    // Enable all multicast controls
    ui_.comboaddresstype->setEnabled(true);
    ui_.editaddress->setEnabled(true);
    ui_.editport->setEnabled(true);
    ui_.spinttl->setEnabled(true);
    ui_.checkautostart->setEnabled(true);
  }

  // Configuration and content
  if (metadataconfigurationoptions.compressiontype_.size())
  {
    ui_.combocompression->setEnabled(true);

    for (auto compressiontype : metadataconfigurationoptions.compressiontype_)
    {
      ui_.combocompression->addItem(QString::fromStdString(ToString(compressiontype)));

    }

    if (metadataconfiguration.compressiontype_.is_initialized())
    {
      ui_.combocompression->setCurrentText(QString::fromStdString(ToString(*metadataconfiguration.compressiontype_)));

    }
  }

  if (metadataconfiguration.ptzstatus_.is_initialized() && metadataconfiguration.ptzstatus_->status_.is_initialized() && metadataconfiguration.ptzstatus_->position_.is_initialized())
  {
    ui_.checkptzstatus->setEnabled(true);
    ui_.checkptzstatus->setChecked(*metadataconfiguration.ptzstatus_->status_);
    ui_.checkptzposition->setEnabled(true);
    ui_.checkptzposition->setChecked(*metadataconfiguration.ptzstatus_->position_);
  }

  if (metadataconfiguration.analytics_.is_initialized())
  {
    ui_.checkanalytics->setEnabled(true);
    ui_.checkanalytics->setChecked(*metadataconfiguration.analytics_);
  }
  
  if (metadataconfiguration.events_.is_initialized() && metadataconfiguration.events_->filter_.is_initialized())
  {
    ui_.checkevents->setChecked(true);

    for (const auto& messagecontent : metadataconfiguration.events_->filter_->messagecontent_)
    {
      AddContentFilter(messagecontent);

    }

    ui_.tablecontentfilters->resizeColumnsToContents();

    if (!SupportsMessageContentFiltering()) // ONVIF rules for not supporting message content filtering
    {
      ui_.tablecontentfilters->setEnabled(false);
      ui_.buttoncontentaddfilter->setEnabled(false);
      ui_.buttoncontentdeletefilter->setEnabled(false);
    }
    else
    {
      for (const auto& topicexpression : metadataconfiguration.events_->filter_->topicexpression_)
      {
        AddTopicFilter(topicexpression);

      }

      ui_.tabletopicfilters->resizeColumnsToContents();
    }
  }
  else
  {
    ui_.checkevents->setChecked(false);
    on_checkevents_toggled(false);
  }

  ui_.buttonok->setEnabled(true);
}

void ManageONVIFMediaMetadataConfigurationWindow::AddContentFilter(const onvif::ws::MessageContent& messagecontent)
{
  const int row = ui_.tablecontentfilters->rowCount();
  ui_.tablecontentfilters->insertRow(row);

  // Dialect
  QComboBox* dialectcombo = new QComboBox();
  for (const auto& dialect : messagecontentfilterdialect_)
  {
    dialectcombo->insertItem(dialectcombo->count(), QString::fromStdString(dialect));

  }

  if (messagecontent.dialect_.is_initialized())
  {
    dialectcombo->setCurrentText(QString::fromStdString(*messagecontent.dialect_));

  }

  ui_.tablecontentfilters->setCellWidget(row, 0, dialectcombo);

  // Filter
  ui_.tablecontentfilters->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(messagecontent.expression_)));
}

void ManageONVIFMediaMetadataConfigurationWindow::AddTopicFilter(const onvif::ws::TopicExpression& topicexpression)
{
  const int row = ui_.tabletopicfilters->rowCount();
  ui_.tabletopicfilters->insertRow(row);

  // Dialect
  QComboBox* dialectcombo = new QComboBox();
  for (const auto& dialect : topicexpressiondialect_)
  {
    dialectcombo->insertItem(dialectcombo->count(), QString::fromStdString(dialect));

  }

  if (topicexpression.dialect_.is_initialized())
  {
    dialectcombo->setCurrentText(QString::fromStdString(*topicexpression.dialect_));

  }

  ui_.tabletopicfilters->setCellWidget(row, 0, dialectcombo);

  // Filter
  ui_.tabletopicfilters->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(topicexpression.expression_)));

}

bool ManageONVIFMediaMetadataConfigurationWindow::SupportsMessageContentFiltering() const
{
  return !(messagecontentfilterdialect_.empty() || ((messagecontentfilterdialect_.size() == 1) && messagecontentfilterdialect_.front().empty()));
}

void ManageONVIFMediaMetadataConfigurationWindow::on_checkevents_toggled(bool toggle)
{
  if (SupportsMessageContentFiltering())
  {
    ui_.tablecontentfilters->setEnabled(toggle);
    ui_.buttoncontentaddfilter->setEnabled(toggle);
    ui_.buttoncontentdeletefilter->setEnabled(toggle);
  }

  ui_.tabletopicfilters->setEnabled(toggle);
  ui_.buttontopicaddfilter->setEnabled(toggle);
  ui_.buttontopicdeletefilter->setEnabled(toggle);
}

void ManageONVIFMediaMetadataConfigurationWindow::on_buttoncontentaddfilter_clicked()
{
  AddContentFilter(onvif::ws::MessageContent(boost::none, std::string()));
  
}

void ManageONVIFMediaMetadataConfigurationWindow::on_buttoncontentdeletefilter_clicked()
{
  int row = ui_.tablecontentfilters->currentRow();
  if (row == -1)
  {
    QMessageBox::warning(this, QString("Error"), QString("No row selected"), QMessageBox::Ok);
    return;
  }

  ui_.tablecontentfilters->removeRow(row);
}

void ManageONVIFMediaMetadataConfigurationWindow::on_buttontopicaddfilter_clicked()
{
  AddTopicFilter(onvif::ws::TopicExpression(boost::none, std::string()));

}

void ManageONVIFMediaMetadataConfigurationWindow::on_buttontopicdeletefilter_clicked()
{
  int row = ui_.tabletopicfilters->currentRow();
  if (row == -1)
  {
    QMessageBox::warning(this, QString("Error"), QString("No row selected"), QMessageBox::Ok);
    return;
  }

  ui_.tabletopicfilters->removeRow(row);
}

void ManageONVIFMediaMetadataConfigurationWindow::on_buttonok_clicked()
{
  // Retrieve the settings from the dialog making sure they are valid
  if (ui_.editname->text().size() <= 0)
  {
    QMessageBox::warning(this, tr("Error"), tr("Invalid name"), QMessageBox::Ok);
    return;
  }

  // Multicast
  onvif::MulticastConfiguration multicastconfiguration;
  onvif::IPTYPE iptype = *onvif::GetIpType(ui_.comboaddresstype->currentText().toStdString());
  if (iptype == onvif::IPTYPE_IPV4)
  {
    if (!boost::asio::ip::address_v4::from_string(ui_.editaddress->text().toStdString()).is_multicast() && (ui_.editaddress->text() != QString("0.0.0.0")))
    {
      QMessageBox::warning(this, tr("Error"), tr("Please enter a multicast address"), QMessageBox::Ok);
      return;
    }

    multicastconfiguration = onvif::MulticastConfiguration(onvif::IPAddress(iptype, ui_.editaddress->text().toStdString(), boost::none), ui_.editport->text().toInt(), ui_.spinttl->value(), ui_.checkautostart->isChecked());
  }
  else if (iptype == onvif::IPTYPE_IPV4)
  {
    if (!boost::asio::ip::address_v6::from_string(ui_.editaddress->text().toStdString()).is_multicast() && (ui_.editaddress->text() != QString("::/0")))
    {
      QMessageBox::warning(this, tr("Error"), tr("Please enter a multicast address"), QMessageBox::Ok);
      return;
    }

    multicastconfiguration = onvif::MulticastConfiguration(onvif::IPAddress(iptype, boost::none, ui_.editaddress->text().toStdString()), ui_.editport->text().toInt(), ui_.spinttl->value(), ui_.checkautostart->isChecked());
  }
  else
  {
    QMessageBox::warning(this, tr("Error"), tr("Invalid type"), QMessageBox::Ok);
    return;
  }

  // Configuration
  boost::optional<onvif::METADATACOMPRESSIONTYPE> compressiontype;
  if (ui_.combocompression->isEnabled())
  {
    compressiontype = onvif::GetMetadataCompressionType(ui_.combocompression->currentText().toStdString());

  }

  boost::optional<onvif::PTZFilter> ptzstatus;
  if (ui_.checkptzstatus->isEnabled() && ui_.checkptzposition->isEnabled())
  {
    ptzstatus = onvif::PTZFilter(ui_.checkptzstatus->isChecked(), ui_.checkptzposition->isChecked());

  }

  boost::optional<bool> analytics;
  if (ui_.checkanalytics->isEnabled())
  {
    analytics = ui_.checkanalytics->isChecked();

  }

  // Filters
  boost::optional<onvif::EventSubscription> events;
  if (ui_.checkevents->isChecked())
  {
    events = onvif::EventSubscription();
    if ((ui_.tablecontentfilters->rowCount()) || (ui_.tabletopicfilters->rowCount()))
    {
      events->filter_ = onvif::ws::FilterType();
      for (int i = 0; i < ui_.tablecontentfilters->rowCount(); ++i)
      {
        events->filter_->messagecontent_.push_back(onvif::ws::MessageContent((static_cast<QComboBox*>(ui_.tablecontentfilters->cellWidget(i, 0)))->currentText().toStdString(), ui_.tablecontentfilters->item(i, 1)->text().toStdString()));


      }

      for (int i = 0; i < ui_.tabletopicfilters->rowCount(); ++i)
      {
        events->filter_->topicexpression_.push_back(onvif::ws::TopicExpression((static_cast<QComboBox*>(ui_.tabletopicfilters->cellWidget(i, 0)))->currentText().toStdString(), ui_.tabletopicfilters->item(i, 1)->text().toStdString()));

      }
    }
  }

  // Set this metadata configuration
  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui_.buttonok->setEnabled(false);
  
  const onvif::MetadataConfiguration metadataconfiguration(ui_.edittoken->text().toStdString(),
                                                           compressiontype,
                                                           ui_.editname->text().toStdString(),
                                                           ui_.editusecount->text().toInt(),
                                                           ptzstatus,
                                                           events,
                                                           analytics,
                                                           multicastconfiguration,
                                                           onvif::Duration(0, 0, 0, 0, 0, 60),
                                                           boost::none);

  setmetadataconfigurationconnection_ = onvifmedia_->SetMetadataConfigurationCallback(metadataconfiguration, [this](const onvif::media::SetMetadataConfigurationResponse& setmetadataconfigurationresponse)
  {
    QApplication::restoreOverrideCursor();
    ui_.buttonok->setEnabled(true);

    if (setmetadataconfigurationresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(setmetadataconfigurationresponse.Message()), QMessageBox::Ok);
      return;
    }

    name_ = ui_.editname->text();

    accept();
  });
}

}
