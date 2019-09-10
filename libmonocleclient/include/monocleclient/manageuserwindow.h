// manageuserwindow.h
//

#ifndef ID2VV1211212324343434342121212DN9G
#define ID2VV1211212324343434342121212DN9G

///// Includes /////

#include "ui_manageuserwindow.h"

#include <boost/optional.hpp>
#include <QSharedPointer>
#include <monocleprotocol/client/connection.hpp>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Group;
class User;

///// Classes /////

class ManageUserWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageUserWindow(QWidget* parent, boost::shared_ptr<Device>& device, const boost::optional<uint64_t>& token);
  ~ManageUserWindow();

 private:

  void SetEnabled(const bool enabled);
 
  Ui::ManageUserWindow ui_;

  boost::shared_ptr<Device> device_;

  boost::optional<uint64_t> token_;

  bool passworddirty_;

  monocle::client::Connection userconnection_;

 private slots:

  void GroupAdded(const QSharedPointer<Group>& group);
  void GroupChanged(const QSharedPointer<Group>& group);
  void GroupRemoved(const uint64_t token);
  void UserChanged(QSharedPointer<User>& user);
  void UserRemoved(const uint64_t token);
  void on_editpassword_textChanged();
  void on_editconfirmpassword_textChanged();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
