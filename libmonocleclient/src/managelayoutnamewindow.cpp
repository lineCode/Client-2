// managelayoutnamewindow.cpp
//

///// Includes /////

#include "monocleclient/managelayoutnamewindow.h"

#include <QMessageBox>
#include <QScreen>
#include <QTimer>
#include <random>
#include <utility/utility.hpp>

#include "monocleclient/layout.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/map.h"
#include "monocleclient/mapview.h"
#include "monocleclient/recording.h"
#include "monocleclient/videowidgetsmgr.h"
#include "monocleclient/videoview.h"
#include "monocleclient/view.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageLayoutNameWindow::ManageLayoutNameWindow(QWidget* parent, const uint64_t token) :
  QDialog(parent),
  token_(token)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  const std::vector< QSharedPointer<Layout> > layouts = MainWindow::Instance()->GetDeviceMgr().GetLayouts(token_);
  if (layouts.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find layout: ") + QString::number(token_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    QTimer::singleShot(std::chrono::milliseconds(1), [this]() { reject(); });
    return;
  }

  ui_.editname->setText(layouts.front()->GetName());
}

ManageLayoutNameWindow::~ManageLayoutNameWindow()
{
  std::for_each(connections_.begin(), connections_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
  connections_.clear();
}

void ManageLayoutNameWindow::on_buttonok_clicked()
{
  if (ui_.editname->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Please fill in a name for the layout"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  std::shared_ptr<unsigned int> count = std::make_shared<unsigned int>(0);
  std::shared_ptr< std::vector<monocle::Error> > errors = std::make_shared< std::vector<monocle::Error> >();

  std::for_each(connections_.begin(), connections_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
  connections_.clear();

  for (const boost::shared_ptr<Device>& device : MainWindow::Instance()->GetDeviceMgr().GetDevices())
  {
    if (!device->GetLayout(token_)) // Only change the name where this token exists
    {

      continue;
    }

    ++(*count);
    QTimer::singleShot(std::chrono::milliseconds(1), [this, count, errors, device]()
    {
      connections_.push_back(device->ChangeLayoutName(token_, ui_.editname->text().toStdString(), [this, count, errors](const std::chrono::steady_clock::duration latency, const monocle::client::CHANGELAYOUTNAMERESPONSE& changelayoutnameresponse)
      {
        errors->push_back(changelayoutnameresponse.error_);
        if ((--(*count)) == 0)
        {
          if (std::all_of(errors->cbegin(), errors->cend(), [](const monocle::Error& error) { return (error.code_ == monocle::ErrorCode::Success); }))
          {
            accept();
            return;
          }
          else
          {
            QStringList errorlist;
            for (const monocle::Error& error : *errors)
            {
              errorlist.push_back(QString::fromStdString(error.text_));

            }

            QMessageBox(QMessageBox::Warning, tr("Error"), tr("Layout failed:\n") + errorlist.join("\n"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
            return;
          }
        }
      }));
    });
  }

  if ((*count) == 0)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to find layout on devices"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }
}

}

QT_END_NAMESPACE
