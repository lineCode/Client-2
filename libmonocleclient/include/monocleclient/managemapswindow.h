// managemapswindow.h
//

#ifndef ID2VVC45454354354353455LAJD9WKDN9G
#define ID2VVC45454354354353455LAJD9WKDN9G

///// Includes /////

#include "ui_managemapswindow.h"

#include <boost/shared_ptr.hpp>
#include <monocleprotocol/client/connection.hpp>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Map;

///// Classes /////

class ManageMapsWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageMapsWindow(QWidget* parent, boost::shared_ptr<Device>& device);
  ~ManageMapsWindow();

 private:

  void AddMap(const QSharedPointer<client::Map>& map);

  Ui::ManageMapsWindow ui_;

  boost::shared_ptr<Device> device_;

  monocle::client::Connection removemapconnection_;

 private slots:

  void MapAdded(const QSharedPointer<client::Map>& map);
  void MapChanged(const QSharedPointer<client::Map>& map);
  void MapRemoved(const uint64_t token);
  void on_tablemaps_itemSelectionChanged();
  void on_buttonadd_clicked();
  void on_buttonedit_clicked();
  void on_buttonremove_clicked();


};

}

QT_END_NAMESPACE

#endif
