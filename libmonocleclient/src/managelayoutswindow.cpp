// managelayoutswindow.cpp
//

///// Includes /////

#include "monocleclient/managelayoutswindow.h"

#include <QMessageBox>
#include <QStringList>

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/managelayoutnamewindow.h"
#include "monocleclient/layout.h"
#include "monocleclient/options.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageLayoutsWindow::ManageLayoutsWindow(QWidget* parent) :
  QDialog(parent)
{
  ui_.setupUi(this);

  if (!Options::Instance().GetShowTokens())
  {
    ui_.tablelayouts->hideColumn(0);
  
  }

  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);
  connect(&MainWindow::Instance()->GetDeviceMgr(), &DeviceMgr::LayoutAdded, this, &ManageLayoutsWindow::LayoutAdded);
  connect(&MainWindow::Instance()->GetDeviceMgr(), &DeviceMgr::LayoutChanged, this, &ManageLayoutsWindow::LayoutChanged);
  connect(&MainWindow::Instance()->GetDeviceMgr(), &DeviceMgr::LayoutRemoved, this, &ManageLayoutsWindow::LayoutRemoved);
  
  for (const QSharedPointer<Layout>& layout : MainWindow::Instance()->GetDeviceMgr().GetLayouts())
  {
    AddLayout(layout);
  
  }

  on_tablelayouts_itemSelectionChanged();
}

ManageLayoutsWindow::~ManageLayoutsWindow()
{
  std::for_each(removelayoutconnections_.begin(), removelayoutconnections_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
  removelayoutconnections_.clear();
}

void ManageLayoutsWindow::AddLayout(const QSharedPointer<client::Layout>& layout)
{
  const int row = ui_.tablelayouts->rowCount();
  ui_.tablelayouts->insertRow(row);
  ui_.tablelayouts->setItem(row, 0, new QTableWidgetItem(QString::number(layout->GetToken())));
  ui_.tablelayouts->item(row, 0)->setData(Qt::UserRole, static_cast<qulonglong>(layout->GetToken()));
  ui_.tablelayouts->setItem(row, 1, new QTableWidgetItem(layout->GetName()));
}

void ManageLayoutsWindow::LayoutAdded(const QSharedPointer<client::Layout>& layout)
{
  AddLayout(layout);

}

void ManageLayoutsWindow::LayoutChanged(const QSharedPointer<client::Layout>& layout)
{
  for (int row = 0; row < ui_.tablelayouts->rowCount(); ++row)
  {
    if (ui_.tablelayouts->item(row, 0)->data(Qt::UserRole).toULongLong() == layout->GetToken())
    {
      ui_.tablelayouts->setItem(row, 1, new QTableWidgetItem(layout->GetName()));
      return;
    }
  }
}

void ManageLayoutsWindow::LayoutRemoved(const uint64_t token)
{
  for (int i = (ui_.tablelayouts->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tablelayouts->item(i, 0)->data(Qt::UserRole).toULongLong() == token)
    {
      ui_.tablelayouts->removeRow(i);
  
    }
  }
}

void ManageLayoutsWindow::on_tablelayouts_itemSelectionChanged()
{
  const QModelIndexList selectedrows = ui_.tablelayouts->selectionModel()->selectedRows();
  if (selectedrows.empty())
  {
    ui_.buttonedit->setEnabled(false);
    ui_.buttonremove->setEnabled(false);
  }
  else
  {
    ui_.buttonedit->setEnabled(true);
    ui_.buttonremove->setEnabled(true);
  }
}

void ManageLayoutsWindow::on_buttonedit_clicked()
{
  const QModelIndexList selectedrows = ui_.tablelayouts->selectionModel()->selectedRows();
  if (selectedrows.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No layout selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }
  
  ManageLayoutNameWindow(this, ui_.tablelayouts->item(selectedrows.at(0).row(), 0)->data(Qt::UserRole).toULongLong()).exec();
}

void ManageLayoutsWindow::on_buttonremove_clicked()
{
  const int row = ui_.tablelayouts->currentRow();
  if (row == -1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No layout selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (QMessageBox::question(this, tr("Remove Layout"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
  {

    return;
  }

  const qulonglong token = ui_.tablelayouts->item(row, 0)->text().toULongLong();

  std::shared_ptr<unsigned int> count = std::make_shared<unsigned int>(0);
  std::shared_ptr< std::vector<monocle::Error> > errors = std::make_shared< std::vector<monocle::Error> >();

  std::for_each(removelayoutconnections_.begin(), removelayoutconnections_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
  removelayoutconnections_.clear();
  for (const boost::shared_ptr<Device>& device : MainWindow::Instance()->GetDeviceMgr().GetDevices())
  {
    if (!device->GetLayout(token))
    {

      continue;
    }

    ++(*count);
    removelayoutconnections_.push_back(device->RemoveLayout(token, [this, count, errors](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVELAYOUTRESPONSE& removelayoutresponse)
    {
      errors->push_back(removelayoutresponse.error_);

      if ((--(*count)) == 0)
      {
        if (std::all_of(errors->cbegin(), errors->cend(), [](const monocle::Error& error) { return (error.code_ == monocle::ErrorCode::Success); }))
        {

          return;
        }
        else
        {
          QStringList errorlist;
          for (const monocle::Error& error : *errors)
          {
            errorlist.push_back(QString::fromStdString(error.text_));

          }

          QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove layout failed:\n") + errorlist.join("\n"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
          return;
        }
      }
    }));
  }
}

}

QT_END_NAMESPACE
