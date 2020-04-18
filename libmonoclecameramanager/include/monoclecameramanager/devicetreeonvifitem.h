// onvifitem.h
// 

#ifndef IDFBJDBRQZDFZXDFREWAEREEEWWWWZISRJ
#define IDFBJDBRQZDFZXDFREWAEREEEWWWWZISRJ

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <onvifclient/connection.hpp>
#include <QObject>
#include <QString>
#include <QTreeWidgetItem>

#include "monoclecameramanager/devicetreeitem.h"

///// Declarations /////

class QAction;
class QContextMenuEvent;
namespace onvif { namespace device { class DeviceClient; } }

///// Namespaces /////

namespace cm
{

///// Classes /////

class DeviceTreeONVIFItem : public DeviceTreeItem
{
 Q_OBJECT;

 public:

  DeviceTreeONVIFItem(QTreeWidget* parent, const QString& uri, const QString& username, const QString& password);
  ~DeviceTreeONVIFItem();

  virtual void Expanded() override;
  virtual void Collapsed() override;
  virtual void DoubleClicked() override;
  virtual void ContextMenuEvent(QContextMenuEvent* event) override;
  virtual void Update() override;

 private:

  QString uri_;
  QString username_;
  QString password_;

  QAction* hack_;

  boost::shared_ptr<onvif::device::DeviceClient> hackdevice_;
  onvif::Connection hackconnection_;


 private slots:

  void Hack(bool);

};

}

#endif
