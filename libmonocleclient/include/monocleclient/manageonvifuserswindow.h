// manageonvifuserswindow.h
//

#ifndef ID2VV657ASDNSADADADADADNN35333DN9G
#define ID2VV657ASDNSADADADADADNN35333DN9G

///// Includes /////

#include "ui_manageonvifuserswindow.h"

#include <monocleprotocol/client/connection.hpp>
#include <QSharedPointer>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class ONVIFUser;

///// Classes /////

class ManageONVIFUsersWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFUsersWindow(QWidget* parent, boost::shared_ptr<Device>& device);
  ~ManageONVIFUsersWindow();

 private:

  void AddONVIFUser(const QSharedPointer<ONVIFUser>& onvifuser);

  Ui::ManageONVIFUsersWindow ui_;

  boost::shared_ptr<Device> device_;

  monocle::client::Connection removeonvifuserconnection_;
  
 private slots:

  void ONVIFUserAdded(QSharedPointer<ONVIFUser>& onvifuser);
  void ONVIFUserChanged(QSharedPointer<ONVIFUser>& onvifuser);
  void ONVIFUserRemoved(const uint64_t token);
  void on_buttonadd_clicked();
  void on_buttonedit_clicked();
  void on_buttonremove_clicked();
 
};

}

QT_END_NAMESPACE

#endif
