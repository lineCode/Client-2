// browsefileswindow.h
//

#ifndef ID2VVSDFJDSIFJDSKXJCKCXJXCX9WKDN9G
#define ID2VVSDFJDSIFJDSKXJCKCXJXCX9WKDN9G

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <monocleprotocol/client/connection.hpp>
#include <QDialog>
#include <QString>

#include "ui_browsefileswindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;

///// Classes /////

class BrowseFilesWindow : public QDialog
{
 Q_OBJECT

 public:

  BrowseFilesWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QString& location);
  virtual ~BrowseFilesWindow();

  const QString& GetLocation() const { return location_; }

 private:

  Ui::BrowseFilesWindow ui_;

  boost::shared_ptr<Device> device_;

  QString location_;

  monocle::client::Connection getchildfoldersconnection_;

 private slots:

  void ItemClicked(QTreeWidgetItem *item, int column);
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
