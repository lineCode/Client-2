// managegroupswindow.h
//

#ifndef ID2VV6575643453453535435435333DN9G
#define ID2VV6575643453453535435435333DN9G

///// Includes /////

#include "ui_managegroupswindow.h"

#include <monocleprotocol/client/connection.hpp>
#include <QSharedPointer>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Group;

///// Classes /////

class ManageGroupsWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageGroupsWindow(QWidget* parent, boost::shared_ptr<Device>& device);
  ~ManageGroupsWindow();

 private:
 
  void AddGroup(const QSharedPointer<Group>& group);

  Ui::ManageGroupsWindow ui_;

  boost::shared_ptr<Device> device_;

  monocle::client::Connection removegroupconnection_;

 private slots:

  void GroupAdded(const QSharedPointer<Group>& group);
  void GroupChanged(const QSharedPointer<Group>& group);
  void GroupRemoved(const uint64_t token);
  void on_buttonadd_clicked();
  void on_buttonedit_clicked();
  void on_buttonremove_clicked();

};

}

QT_END_NAMESPACE

#endif
