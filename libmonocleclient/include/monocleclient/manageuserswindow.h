// manageuserswindow.h
//

#ifndef ID2VV1211212121212121212121212DN9G
#define ID2VV1211212121212121212121212DN9G

///// Includes /////

#include "ui_manageuserswindow.h"

#include <monocleprotocol/client/connection.hpp>
#include <QSharedPointer>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Group;
class User;

///// Classes /////

class ManageUsersWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageUsersWindow(QWidget* parent, boost::shared_ptr<Device>& device);
  ~ManageUsersWindow();

 private:
 
  void AddUser(const QSharedPointer<User>& user);

  Ui::ManageUsersWindow ui_;

  boost::shared_ptr<Device> device_;

  monocle::client::Connection removeuserconnection_;

 private slots:

  void GroupChanged(const QSharedPointer<Group>& group);
  void UserAdded(QSharedPointer<User>& user);
  void UserChanged(QSharedPointer<User>& user);
  void UserRemoved(const uint64_t token);
  void on_buttonadd_clicked();
  void on_buttonedit_clicked();
  void on_buttonremove_clicked();

};

}

QT_END_NAMESPACE

#endif
