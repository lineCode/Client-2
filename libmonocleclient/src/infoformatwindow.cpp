// infoformatwindow.cpp
//

///// Includes /////

#include "monocleclient/infoformatwindow.h"

#include <QSharedPointer>

#include "monocleclient/videowidget.h"

///// Namespaces /////

namespace client
{

///// Methods /////

InfoFormatWindow::InfoFormatWindow(QWidget* parent, const QString& infotextformat) :
  QDialog(parent),
  currentdatetime_(QDateTime::currentDateTime())
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);
  
  ui_.editinfotextformat->setText(infotextformat);

  // Select the combo box if we have it there
  const int i = ui_.comboinfotextformat->findText(infotextformat);
  if (i != -1)
  {
    ui_.comboinfotextformat->setCurrentIndex(i);

  }
}

InfoFormatWindow::~InfoFormatWindow()
{

}

void InfoFormatWindow::on_editinfotextformat_textChanged(const QString& text)
{
  boost::circular_buffer< std::pair<std::chrono::steady_clock::time_point, size_t> > bandwidthsizes(10);
  bandwidthsizes.push_back(std::make_pair(std::chrono::steady_clock::now(), 1024 * 8));
  bandwidthsizes.push_back(std::make_pair(std::chrono::steady_clock::now() - std::chrono::milliseconds(80), 1024 * 8));
  bandwidthsizes.push_back(std::make_pair(std::chrono::steady_clock::now() - std::chrono::milliseconds(160), 1024 * 8));
  bandwidthsizes.push_back(std::make_pair(std::chrono::steady_clock::now() - std::chrono::milliseconds(240), 1024 * 8));
  bandwidthsizes.push_back(std::make_pair(std::chrono::steady_clock::now() - std::chrono::milliseconds(320), 1024 * 8));
  bandwidthsizes.push_back(std::make_pair(std::chrono::steady_clock::now() - std::chrono::milliseconds(400), 1024 * 8));
  bandwidthsizes.push_back(std::make_pair(std::chrono::steady_clock::now() - std::chrono::milliseconds(480), 1024 * 8));
  bandwidthsizes.push_back(std::make_pair(std::chrono::steady_clock::now() - std::chrono::milliseconds(540), 1024 * 8));
  bandwidthsizes.push_back(std::make_pair(std::chrono::steady_clock::now() - std::chrono::milliseconds(600), 1024 * 8));
  bandwidthsizes.push_back(std::make_pair(std::chrono::steady_clock::now() - std::chrono::milliseconds(660), 1024 * 8));

  std::vector<char> buffer;
  ToInfoText(currentdatetime_, ui_.editinfotextformat->text().toStdString(), monocle::Codec::MJPEG, bandwidthsizes, std::make_pair(std::string("Location"), QString()), std::make_pair(std::string("Recording name"), QString()), 1024, 768, buffer);
  ui_.editinfotextformatexampleoutput->setText(QString::fromLatin1(buffer.data(), static_cast<int>(buffer.size())));
}

void InfoFormatWindow::on_comboinfotextformat_currentTextChanged(const QString& text)
{
  ui_.editinfotextformat->setText(text);

}

void InfoFormatWindow::on_buttonok_clicked()
{
  infotextformat_ = ui_.editinfotextformat->text().toStdString();
  accept();
}

}
