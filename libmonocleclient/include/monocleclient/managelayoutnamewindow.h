// managelayoutnamewindow.h
//

#ifndef ID2VVCASDASDASDADADZZZZXXS19WKDN9G
#define ID2VVCASDASDASDADADZZZZXXS19WKDN9G

///// Includes /////

#include <monocleprotocol/monocleprotocol.hpp>
#include <monocleprotocol/client/connection.hpp>
#include <QDialog>
#include <vector>

#include "ui_managelayoutnamewindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Classes /////

class ManageLayoutNameWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageLayoutNameWindow(QWidget* parent, const uint64_t token);
  virtual ~ManageLayoutNameWindow();

 private:

  const uint64_t token_;

  Ui::ManageLayoutNameWindow ui_;

  std::vector<monocle::client::Connection> connections_;

 private slots:

  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
