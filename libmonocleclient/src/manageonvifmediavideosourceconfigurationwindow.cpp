// managevideosourceconfigurationwindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediavideosourceconfigurationwindow.h"

#include <boost/asio.hpp>
#include <onvifclient/mediaclient.hpp>
#include <QMessageBox>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Methods /////

ManageONVIFMediaVideoSourceConfigurationWindow::ManageONVIFMediaVideoSourceConfigurationWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const std::string& token) :
  QDialog(parent),
  token_(token),
  onvifmedia_(onvifmedia)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  ui_.edittoken->setText(QString::fromStdString(token));
  
  std::shared_ptr<unsigned int> count = std::make_shared<unsigned int>(2);
  std::shared_ptr<onvif::VideoSourceConfiguration> videosourceconfiguration = std::make_shared<onvif::VideoSourceConfiguration>();
  std::shared_ptr<onvif::VideoSourceConfigurationOptions> videosourceconfigurationoptions = std::make_shared<onvif::VideoSourceConfigurationOptions>();
  getvideosourceconfigurationconnection_ = onvifmedia_->GetVideoSourceConfigurationCallback(token, [this, count, videosourceconfiguration, videosourceconfigurationoptions](const onvif::media::GetVideoSourceConfigurationResponse& getvideosourceconfigurationresponse)
  {
    if (getvideosourceconfigurationresponse.Error() || !getvideosourceconfigurationresponse.videosourceconfiguration_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getvideosourceconfigurationresponse.Message()), QMessageBox::Ok);
      return;
    }

    *videosourceconfiguration = *getvideosourceconfigurationresponse.videosourceconfiguration_;

    if ((--(*count)) == 0)
    {
      InitDialog(*videosourceconfiguration);

    }
  });
  
  getvideosourceconfigurationoptionsconnection_ = onvifmedia_->GetVideoSourceConfigurationOptionsCallback(token_, boost::none, [this, count, videosourceconfiguration, videosourceconfigurationoptions](const onvif::media::GetVideoSourceConfigurationOptionsResponse& getvideosourceconfigurationoptionsresponse)
  {
    if (getvideosourceconfigurationoptionsresponse.Error() || !getvideosourceconfigurationoptionsresponse.videosourceconfigurationoptions_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getvideosourceconfigurationoptionsresponse.Message()), QMessageBox::Ok);
      return;
    }

    videosourceconfigurationoptions_ = *getvideosourceconfigurationoptionsresponse.videosourceconfigurationoptions_;

    if ((--(*count)) == 0)
    {
      InitDialog(*videosourceconfiguration);

    }
  });
}

ManageONVIFMediaVideoSourceConfigurationWindow::~ManageONVIFMediaVideoSourceConfigurationWindow()
{
  if (getvideosourceconfigurationconnection_.IsConnected() || getvideosourceconfigurationoptionsconnection_.IsConnected())
  {
    QApplication::restoreOverrideCursor();

  }
}

void ManageONVIFMediaVideoSourceConfigurationWindow::InitDialog(const onvif::VideoSourceConfiguration& videosourceconfiguration)
{
  // Configuration
  if (videosourceconfiguration.name_.is_initialized())
  {
    ui_.editname->setText(QString::fromStdString(*videosourceconfiguration.name_));

  }
  ui_.editname->setEnabled(true);

  if (videosourceconfiguration.usecount_.is_initialized())
  {
    ui_.editusecount->setText(QString::number(*videosourceconfiguration.usecount_));

  }

  // Source token
  for (const auto& videosourcetokenavailable : videosourceconfigurationoptions_.videosourcetokensavailable_)
  {
    ui_.combosourcetoken->addItem(QString::fromStdString(videosourcetokenavailable));

  }

  if (videosourceconfiguration.sourcetoken_.is_initialized())
  {
    // Select the current token(if it exists)
    const int item = ui_.combosourcetoken->findText(QString::fromStdString(*videosourceconfiguration.sourcetoken_));
    if (item == -1)
    {
      LOG_GUI_WARNING(QString("Source token not found: ") + QString::fromStdString(*videosourceconfiguration.sourcetoken_));

    }
    else
    {
      ui_.combosourcetoken->setCurrentIndex(item);

    }
  }

  // Bounds
  if (videosourceconfiguration.bounds_.is_initialized())
  {
    if (videosourceconfiguration.bounds_->x_.is_initialized())
    {
      ui_.editx->setText(QString::number(*videosourceconfiguration.bounds_->x_));

    }

    if (videosourceconfiguration.bounds_->y_.is_initialized())
    {
      ui_.edity->setText(QString::number(*videosourceconfiguration.bounds_->y_));

    }

    if (videosourceconfiguration.bounds_->width_.is_initialized())
    {
      ui_.editwidth->setText(QString::number(*videosourceconfiguration.bounds_->width_));

    }

    if (videosourceconfiguration.bounds_->height_.is_initialized())
    {
      ui_.editheight->setText(QString::number(*videosourceconfiguration.bounds_->height_));

    }
  }

  if (videosourceconfigurationoptions_.boundsrange_.is_initialized())
  {
    if (videosourceconfigurationoptions_.boundsrange_->xrange_.is_initialized() && videosourceconfigurationoptions_.boundsrange_->xrange_->min_.is_initialized() && videosourceconfigurationoptions_.boundsrange_->xrange_->max_.is_initialized())
    {
      ui_.editx->setValidator(new QIntValidator(*videosourceconfigurationoptions_.boundsrange_->xrange_->min_, *videosourceconfigurationoptions_.boundsrange_->xrange_->max_, this));
      ui_.labelx->setText(QString("x(") + QString::number(*videosourceconfigurationoptions_.boundsrange_->xrange_->min_) + QString("-") + QString::number(*videosourceconfigurationoptions_.boundsrange_->xrange_->max_) + QString("):"));

      if ((videosourceconfigurationoptions_.boundsrange_->xrange_->min_ != videosourceconfigurationoptions_.boundsrange_->xrange_->max_) || (videosourceconfigurationoptions_.boundsrange_->xrange_->min_ != videosourceconfiguration.bounds_->x_))
      {
        ui_.editx->setEnabled(true);

      }
    }

    if (videosourceconfigurationoptions_.boundsrange_->yrange_.is_initialized() && videosourceconfigurationoptions_.boundsrange_->yrange_->min_.is_initialized() && videosourceconfigurationoptions_.boundsrange_->yrange_->max_.is_initialized())
    {
      ui_.edity->setValidator(new QIntValidator(*videosourceconfigurationoptions_.boundsrange_->yrange_->min_, *videosourceconfigurationoptions_.boundsrange_->yrange_->max_, this));
      ui_.labely->setText(QString("y(") + QString::number(*videosourceconfigurationoptions_.boundsrange_->yrange_->min_) + QString("-") + QString::number(*videosourceconfigurationoptions_.boundsrange_->yrange_->max_) + QString("):"));

      if ((videosourceconfigurationoptions_.boundsrange_->yrange_->min_ != videosourceconfigurationoptions_.boundsrange_->yrange_->max_) || (videosourceconfigurationoptions_.boundsrange_->yrange_->min_ != videosourceconfiguration.bounds_->y_))
      {
        ui_.edity->setEnabled(true);

      }
    }

    if (videosourceconfigurationoptions_.boundsrange_->widthrange_.is_initialized() && videosourceconfigurationoptions_.boundsrange_->widthrange_->min_.is_initialized() && videosourceconfigurationoptions_.boundsrange_->widthrange_->max_.is_initialized())
    {
      ui_.editwidth->setValidator(new QIntValidator(*videosourceconfigurationoptions_.boundsrange_->widthrange_->min_, *videosourceconfigurationoptions_.boundsrange_->widthrange_->max_, this));
      ui_.labelwidth->setText(QString("width(") + QString::number(*videosourceconfigurationoptions_.boundsrange_->widthrange_->min_) + QString("-") + QString::number(*videosourceconfigurationoptions_.boundsrange_->widthrange_->max_) + QString("):"));

      if ((videosourceconfigurationoptions_.boundsrange_->widthrange_->min_ != videosourceconfigurationoptions_.boundsrange_->widthrange_->max_) || (videosourceconfigurationoptions_.boundsrange_->widthrange_->min_ != videosourceconfiguration.bounds_->width_))
      {
        ui_.editwidth->setEnabled(true);

      }
    }

    if (videosourceconfigurationoptions_.boundsrange_->heightrange_.is_initialized() && videosourceconfigurationoptions_.boundsrange_->heightrange_->min_.is_initialized() && videosourceconfigurationoptions_.boundsrange_->heightrange_->max_.is_initialized())
    {
      ui_.editheight->setValidator(new QIntValidator(*videosourceconfigurationoptions_.boundsrange_->heightrange_->min_, *videosourceconfigurationoptions_.boundsrange_->heightrange_->max_, this));
      ui_.labelheight->setText(QString("width(") + QString::number(*videosourceconfigurationoptions_.boundsrange_->heightrange_->min_) + QString("-") + QString::number(*videosourceconfigurationoptions_.boundsrange_->heightrange_->max_) + QString("):"));

      if ((videosourceconfigurationoptions_.boundsrange_->heightrange_->min_ != videosourceconfigurationoptions_.boundsrange_->heightrange_->max_) || (videosourceconfigurationoptions_.boundsrange_->heightrange_->min_ != videosourceconfiguration.bounds_->height_))
      {
        ui_.editheight->setEnabled(true);

      }
    }
  }

  // Rotation
  if (videosourceconfigurationoptions_.rotate_.is_initialized())
  {
    for (const auto mode : videosourceconfigurationoptions_.rotate_->mode_)
    {
      ui_.comborotationmode->addItem(QString::fromStdString(onvif::ToString(mode)));

    }

    if (videosourceconfigurationoptions_.rotate_->mode_.size())
    {
      ui_.comborotationmode->setEnabled(true);

    }

    if (videosourceconfigurationoptions_.rotate_->degreelist_.is_initialized())
    {
      for (const auto degree : videosourceconfigurationoptions_.rotate_->degreelist_->items_)
      {
        ui_.comborotationdegree->addItem(QString::number(degree));

      }

      if (videosourceconfigurationoptions_.rotate_->degreelist_->items_.size())
      {
        ui_.comborotationdegree->setEnabled(true);

      }
    }
  }

  if (videosourceconfiguration.rotate_.is_initialized())
  {
    if (videosourceconfiguration.rotate_->rotatemode_.is_initialized())
    {
      const int index = ui_.comborotationmode->findText(QString::fromStdString(onvif::ToString(*videosourceconfiguration.rotate_->rotatemode_)));
      if (index == -1)
      {
        LOG_GUI_WARNING(QString("Rotate mode not found: ") + QString::fromStdString(onvif::ToString(*videosourceconfiguration.rotate_->rotatemode_)));

      }
      else
      {
        ui_.comborotationmode->setCurrentIndex(index);

      }
    }

    if (videosourceconfiguration.rotate_->degree_.is_initialized())
    {
      const int index = ui_.comborotationdegree->findText(QString::number(*videosourceconfiguration.rotate_->degree_));
      if (index == -1)
      {
        LOG_GUI_WARNING(QString("Degree not found: ") + QString::number(*videosourceconfiguration.rotate_->degree_));

      }
      else
      {
        ui_.comborotationdegree->setCurrentIndex(index);

      }
    }
  }

  ui_.buttonok->setEnabled(true);
}

void ManageONVIFMediaVideoSourceConfigurationWindow::on_buttonok_clicked()
{
  // There must be a token
  if (ui_.combosourcetoken->currentText().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("Must be a video source token"), QMessageBox::Ok).exec();
    return;
  }

  // There must be values for bounds
  bool ok = false;
  const int x = ui_.editx->text().toInt(&ok);
  if (ui_.editx->text().isEmpty() || !ok)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("Invalid bounds range x component"), QMessageBox::Ok).exec();
    return;
  }

  const int y = ui_.edity->text().toInt(&ok);
  if (ui_.edity->text().isEmpty() || !ok)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("Invalid bounds range y component"), QMessageBox::Ok).exec();
    return;
  }

  const int width = ui_.editwidth->text().toInt(&ok);
  if (ui_.editwidth->text().isEmpty() || !ok)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("Invalid bounds range width component"), QMessageBox::Ok).exec();
    return;
  }

  const int height = ui_.editheight->text().toInt(&ok);
  if (ui_.editheight->text().isEmpty() || !ok)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("Invalid bounds range height component"), QMessageBox::Ok).exec();
    return;
  }

  // Rotation
  boost::optional<onvif::Rotate> rotate;
  if (videosourceconfigurationoptions_.rotate_.is_initialized())
  {
    rotate = onvif::Rotate();
    if (videosourceconfigurationoptions_.rotate_->mode_.size())
    {
      rotate->rotatemode_ = onvif::GetRotateMode(ui_.comborotationmode->currentText().toStdString());
      if (!rotate->rotatemode_.is_initialized())
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), QString("Invalid rotate mode"), QMessageBox::Ok).exec();
        return;
      }
    }

    if (videosourceconfigurationoptions_.rotate_->degreelist_.is_initialized() && videosourceconfigurationoptions_.rotate_->degreelist_->items_.size())
    {
      rotate->degree_ = ui_.comborotationdegree->currentText().toInt();

    }
  }

  // Set this video encoder configuration
  const onvif::VideoSourceConfiguration videosourceconfiguration(ui_.editname->text().toStdString(), ui_.editusecount->text().toInt(), ui_.edittoken->text().toStdString(), ui_.combosourcetoken->currentText().toStdString(), onvif::IntRectangle(x, y, width, height), rotate);

  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui_.buttonok->setEnabled(false);

  setvideosourceconfigurationconnection_ = onvifmedia_->SetVideoSourceConfigurationCallback(videosourceconfiguration, [this](const onvif::media::SetVideoSourceConfigurationResponse& setvideosourceconfigurationresponse)
  {
    QApplication::restoreOverrideCursor();
    ui_.buttonok->setEnabled(true);

    if (setvideosourceconfigurationresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(setvideosourceconfigurationresponse.Message()), QMessageBox::Ok);
      return;
    }

    name_ = ui_.editname->text();

    accept();
  });
}

}
