// manageonvifuserwindow.h
//

#ifndef ID2VV12ASMNDADASMDANMDNAMSNDN2DN9G
#define ID2VV12ASMNDADASMDANMDNAMSNDN2DN9G

///// Includes /////

#include "ui_manageonvifuserwindow.h"

#include <boost/optional.hpp>
#include <QSharedPointer>
#include <monocleprotocol/client/connection.hpp>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class ONVIFUser;

///// Classes /////

class ManageONVIFUserWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFUserWindow(QWidget* parent, boost::shared_ptr<Device>& device, const boost::optional<uint64_t>& token);
  ~ManageONVIFUserWindow();

 private:

  void SetEnabled(const bool enabled);
 
  Ui::ManageONVIFUserWindow ui_;

  boost::shared_ptr<Device> device_;

  boost::optional<uint64_t> token_;

  bool passworddirty_;

  monocle::client::Connection userconnection_;

 private slots:

  void ONVIFUserChanged(QSharedPointer<ONVIFUser>& onvifuser);
  void ONVIFUserRemoved(const uint64_t token);
  void on_editpassword_textChanged();
  void on_editconfirmpassword_textChanged();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
