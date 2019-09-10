// manageonvifuserswindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifuserswindow.h"

#include <QMessageBox>

#include "monocleclient/device.h"
#include "monocleclient/manageonvifuserwindow.h"
#include "monocleclient/onvifuser.h"
#include "monocleclient/options.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFUsersWindow::ManageONVIFUsersWindow(QWidget* parent, boost::shared_ptr<Device>& device) :
  QDialog(parent),
  device_(device)
{
  ui_.setupUi(this);

  if (!Options::Instance().GetShowTokens())
  {
    ui_.tableusers->hideColumn(0);

  }

  connect(device.get(), QOverload<QSharedPointer<ONVIFUser>&>::of(&Device::SignalONVIFUserAdded), this, &ManageONVIFUsersWindow::ONVIFUserAdded);
  connect(device.get(), QOverload<QSharedPointer<ONVIFUser>&>::of(&Device::SignalONVIFUserChanged), this, &ManageONVIFUsersWindow::ONVIFUserChanged);
  connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalONVIFUserRemoved), this, &ManageONVIFUsersWindow::ONVIFUserRemoved);
  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);

  for (const QSharedPointer<ONVIFUser>& onvifuser : device_->GetONVIFUsers())
  {
    AddONVIFUser(onvifuser);

  }
}

ManageONVIFUsersWindow::~ManageONVIFUsersWindow()
{
  removeonvifuserconnection_.Close();

}

void ManageONVIFUsersWindow::AddONVIFUser(const QSharedPointer<ONVIFUser>& onvifuser)
{
  const int row = ui_.tableusers->rowCount();
  ui_.tableusers->insertRow(row);
  QTableWidgetItem* token = new QTableWidgetItem(QString::number(onvifuser->GetToken()));
  token->setData(Qt::UserRole, static_cast<qulonglong>(onvifuser->GetToken()));
  ui_.tableusers->setItem(row, 0, token);
  ui_.tableusers->setItem(row, 1, new QTableWidgetItem(onvifuser->GetUsername()));
  ui_.tableusers->setItem(row, 2, new QTableWidgetItem(monocle::EnumNameONVIFUserlevel(onvifuser->GetUserlevel())));
}

void ManageONVIFUsersWindow::ONVIFUserAdded(QSharedPointer<ONVIFUser>& onvifuser)
{
  AddONVIFUser(onvifuser);

}

void ManageONVIFUsersWindow::ONVIFUserChanged(QSharedPointer<ONVIFUser>& onvifuser)
{
  for (int i = (ui_.tableusers->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tableusers->item(i, 0)->data(Qt::UserRole).toULongLong() == onvifuser->GetToken())
    {
      ui_.tableusers->item(i, 1)->setText(onvifuser->GetUsername());
      ui_.tableusers->item(i, 2)->setText(monocle::EnumNameONVIFUserlevel(onvifuser->GetUserlevel()));
    }
  }
}

void ManageONVIFUsersWindow::ONVIFUserRemoved(const uint64_t token)
{
  for (int i = (ui_.tableusers->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tableusers->item(i, 0)->data(Qt::UserRole) == static_cast<qulonglong>(token))
    {
      ui_.tableusers->removeRow(i);

    }
  }
}

void ManageONVIFUsersWindow::on_buttonadd_clicked()
{
  ManageONVIFUserWindow(this, device_, boost::none).exec();

}

void ManageONVIFUsersWindow::on_buttonedit_clicked()
{
  const QModelIndexList selectedrows = ui_.tableusers->selectionModel()->selectedRows();
  if (selectedrows.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No ONVIF user selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  ManageONVIFUserWindow(this,
                        device_,
                        ui_.tableusers->item(selectedrows.at(0).row(), 0)->data(Qt::UserRole).toULongLong()).exec();
}

void ManageONVIFUsersWindow::on_buttonremove_clicked()
{
  const int row = ui_.tableusers->currentRow();
  if (row == -1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No user selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (QMessageBox::question(this, tr("Remove User"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
  {

    return;
  }

  const qulonglong token = ui_.tableusers->item(row, 0)->text().toULongLong();
  removeonvifuserconnection_ = device_->RemoveONVIFUser(token, [this](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVEONVIFUSERRESPONSE& removeonvifuserresponse)
  {
    if (removeonvifuserresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removeonvifuserresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  });
}

}

QT_END_NAMESPACE
