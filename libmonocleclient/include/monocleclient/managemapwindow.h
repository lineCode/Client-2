// managemapwindow.h
//

#ifndef ID2VVCASAS5656565656666LDKSAWKDN9G
#define ID2VVCASAS5656565656666LDKSAWKDN9G

///// Includes /////

#include <boost/optional.hpp>
#include <monocleprotocol/client/connection.hpp>
#include <QDialog>
#include <QSharedPointer>

#include "ui_managemapwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Map;

///// Classes /////

class ManageMapWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageMapWindow(QWidget* parent, boost::shared_ptr<Device>& device, const boost::optional<uint64_t>& token);
  ~ManageMapWindow();

 private:

  void SetEnabled(const bool enabled);

  Ui::ManageMapWindow ui_;

  boost::shared_ptr<Device> device_;

  const boost::optional<uint64_t> token_;

  monocle::client::Connection mapconnection_;

 private slots:

  void MapChanged(const QSharedPointer<client::Map>& map);
  void MapRemoved(const uint64_t token);
  void on_buttonbrowsefile_clicked();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
