// devicelogwindow.cpp
//

///// Includes /////

#include "monocleclient/devicelogwindow.h"

#include <QClipboard>
#include <QMessageBox>

#include "monocleclient/device.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceLogWindow::DeviceLogWindow(QWidget* parent, const std::vector< boost::shared_ptr<Device> >& devices) :
  QDialog(parent),
  devices_(devices)
{
  ui_.setupUi(this);

  Update();

  for (const boost::shared_ptr<Device>& device : devices_)
  {
    connect(device.get(), &Device::SignalServerLogMessage, this, QOverload<const uint64_t, const monocle::Severity, const QString&>::of(&DeviceLogWindow::ServerLogMessage), Qt::QueuedConnection);

  }
}

DeviceLogWindow::~DeviceLogWindow()
{

}

void DeviceLogWindow::Update()
{
  // Accumulate and sort all the messages
  std::vector<monocle::LOGMESSAGE> messages;
  for (const boost::shared_ptr<Device>& device : devices_)
  {
    messages.insert(messages.end(), device->GetLogMessages().begin(), device->GetLogMessages().end());

  }
  std::sort(messages.begin(), messages.end(), [](const monocle::LOGMESSAGE& lhs, const monocle::LOGMESSAGE& rhs) { return (lhs.time_ < rhs.time_); });
  
  // Create the text
  QString text;
  for (const monocle::LOGMESSAGE& logmessage : messages)
  {
    if (Filter(logmessage.severity_))
    {
      text.append(HTML(logmessage.severity_, QString::fromStdString(logmessage.message_)));

    }
  }
  ui_.textlog->setHtml(text);
}

void DeviceLogWindow::ServerLogMessage(const uint64_t time, const monocle::Severity severity, const QString& message)
{
  if (!Filter(severity))
  {

    return;
  }
  ui_.textlog->moveCursor(QTextCursor::End);
  ui_.textlog->insertHtml(HTML(severity, message));
}

bool DeviceLogWindow::Filter(const monocle::Severity severity) const
{
  switch (severity)
  {
    case monocle::Severity::Trace:
    {

      return ui_.checktrace->isChecked();
    }
    case monocle::Severity::Debug:
    {

      return ui_.checkdebug->isChecked();
    }
    case monocle::Severity::Info:
    {

      return ui_.checkinfo->isChecked();
    }
    case monocle::Severity::Warn:
    {

      return ui_.checkwarning->isChecked();
    }
    case monocle::Severity::Err:
    {

      return ui_.checkerror->isChecked();
    }
    case monocle::Severity::Critical:
    {

      return ui_.checkcritical->isChecked();
    }
    default:
    {

      return false;
    }
  }
}

QString DeviceLogWindow::HTML(const monocle::Severity severity, const QString& message) const
{
  QString html = "<font color=\"";
  switch (severity)
  {
    case monocle::Severity::Trace:
    case monocle::Severity::Debug:
    case monocle::Severity::Info:
    {
      html += "Green\">";
      break;
    }
    case monocle::Severity::Warn:
    {
      html += "Orange\">";
      break;
    }
    case monocle::Severity::Err:
    case monocle::Severity::Critical:
    {
      html += "Red\">";
      break;
    }
    default:
    {

      return QString();
    }
  }
  html += message + "</font><br>";
  return html;
}

void DeviceLogWindow::on_checktrace_clicked()
{
  Update();

}

void DeviceLogWindow::on_checkdebug_clicked()
{
  Update();

}

void DeviceLogWindow::on_checkinfo_clicked()
{
  Update();

}

void DeviceLogWindow::on_checkwarning_clicked()
{
  Update();

}

void DeviceLogWindow::on_checkerror_clicked()
{
  Update();

}

void DeviceLogWindow::on_checkcritical_clicked()
{
  Update();

}

void DeviceLogWindow::on_buttoncopy_clicked()
{
  QApplication::clipboard()->setText(ui_.textlog->toPlainText());

}

void DeviceLogWindow::on_buttonok_clicked()
{
  accept();
  
}

}
