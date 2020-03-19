// managelayoutwindow.cpp
//

///// Includes /////

#include "monocleclient/managelayoutwindow.h"

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

ManageLayoutWindow::ManageLayoutWindow(QWidget* parent) :
  QDialog(parent),
  token_(0)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);
}

ManageLayoutWindow::~ManageLayoutWindow()
{
  std::for_each(connections_.begin(), connections_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
  connections_.clear();
}

void ManageLayoutWindow::on_buttonok_clicked()
{
  if (ui_.editname->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Please fill in a name for the layout"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  const uint64_t token = MainWindow::Instance()->GetDeviceMgr().GetUniqueLayoutToken();
  const std::vector< std::pair< boost::shared_ptr<Device>, monocle::LAYOUT> > layouts = MainWindow::Instance()->GetLayout(token, ui_.editname->text().toStdString());
  if (layouts.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No layout windows found:\n"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  std::shared_ptr<unsigned int> count = std::make_shared<unsigned int>(0);
  std::shared_ptr< std::vector<monocle::Error> > errors = std::make_shared< std::vector<monocle::Error> >();
  std::for_each(connections_.begin(), connections_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
  connections_.clear();
  for (const std::pair< boost::shared_ptr<Device>, monocle::LAYOUT>& layout : layouts)
  {
    ++(*count);
    connections_.push_back(layout.first->AddLayout(layout.second, [this, token, count, errors](const std::chrono::steady_clock::duration latency, const monocle::client::ADDLAYOUTRESPONSE& addlayoutresponse)
    {
      errors->push_back(addlayoutresponse.error_);
      if ((--(*count)) == 0)
      {
        if (std::all_of(errors->cbegin(), errors->cend(), [](const monocle::Error& error) { return (error.code_ == monocle::ErrorCode::Success); }))
        {
          token_ = token;
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

          QMessageBox(QMessageBox::Warning, tr("Error"), tr("Add layout failed:\n") + errorlist.join("\n"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
          return;
        }
      }
    }));
  }
}

}

QT_END_NAMESPACE
