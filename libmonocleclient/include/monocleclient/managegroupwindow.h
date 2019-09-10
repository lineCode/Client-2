// managegroupwindow.h
//

#ifndef ID2VV6776767676767676767676767DN9G
#define ID2VV6776767676767676767676767DN9G

///// Includes /////

#include "ui_managegroupwindow.h"

#include <boost/optional.hpp>
#include <monocleprotocol/client/connection.hpp>
#include <QSharedPointer>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Group;
class Recording;
class User;

///// Classes /////

class ManageGroupWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageGroupWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const boost::optional<uint64_t>& token);
  ~ManageGroupWindow();

 private:
 
  void SetEnabled(const bool enabled);

  Ui::ManageGroupWindow ui_;

  boost::shared_ptr<Device> device_;

  monocle::client::Connection groupconnection_;

  boost::optional<uint64_t> token_;

 private slots:

  void RecordingAdded(const QSharedPointer<client::Recording>& recording);
  void RecordingChanged(const QSharedPointer<client::Recording>& recording);
  void RecordingRemoved(const uint64_t token);
  void UserAdded(QSharedPointer<User>& user);
  void UserChanged(QSharedPointer<User>& user);
  void UserRemoved(const uint64_t token);
  void GroupChanged(const QSharedPointer<Group>& group);
  void GroupRemoved(const uint64_t token);
  void on_checkallrecordings_stateChanged(int state);
  void on_buttonright_clicked();
  void on_buttonleft_clicked();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
