// managefilewindow.h
//

#ifndef ID2VVCASDJASDSNDLADASKSLAJD9WKDN9G
#define ID2VVCASDJASDSNDLADASKSLAJD9WKDN9G

///// Includes /////

#include <monocleprotocol/client/connection.hpp>
#include <QCompleter>
#include <QDialog>
#include <QSharedPointer>
#include <QStringListModel>
#include <string>
#include <vector>

#include "ui_managefilewindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;

///// Classes /////

class ManageFileWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageFileWindow(QWidget* parent, boost::shared_ptr<Device>& device);
  ~ManageFileWindow();

 protected:

 private:

  void SetEnabled(const bool enabled);

  Ui::ManageFileWindow ui_;

  boost::shared_ptr<Device> device_;

  monocle::client::Connection addfileconnection_;

  QStringListModel* mountmodel_;
  QStringListModel* pathmodel_;
  QCompleter* mountcompleter_;
  QCompleter* pathcompleter_;

  std::vector<std::string> requestpaths_;

  std::vector<monocle::client::Connection> getchildfoldersconnections_;

 private slots:

  void SlotMountPointAdded(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const QString& path, const QString& type, const QString& source);
  void on_editpath_textChanged(const QString& text);
  void on_checkfilldisk_toggled(const bool);
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
