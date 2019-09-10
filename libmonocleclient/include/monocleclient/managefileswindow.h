// managefileswindow.h
//

#ifndef ID2VVCASDJLNRSDL343ASDSLAJD9WKDN9G
#define ID2VVCASDJLNRSDL343ASDSLAJD9WKDN9G

///// Includes /////

#include <monocleprotocol/client/connection.hpp>
#include <QDialog>
#include <QSharedPointer>

#include "ui_managefileswindow.h"

///// Declarations /////

namespace monocle
{

enum class FileMonitorState : int8_t;
enum class FileState : int8_t;

}

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class File;

///// Classes /////

class ManageFilesWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageFilesWindow(QWidget* parent, boost::shared_ptr<Device>& device);
  virtual ~ManageFilesWindow();

 private:

  void AddFile(const QSharedPointer<client::File>& file);
  void UpdateTotalCapacity();
  void SetEnabled(const bool enabled);

  Ui::ManageFilesWindow ui_;

  boost::shared_ptr<Device> device_;

  monocle::client::Connection removefileconnection_;
  monocle::client::Connection mountfileconnection_;
  monocle::client::Connection unmountfileconnection_;

 private slots:

  void FileAdded(QSharedPointer<client::File>& file);
  void FileRemoved(const uint64_t token);
  void FileMonitorStateChanged(QSharedPointer<client::File>& file, const monocle::FileMonitorState monitorstate);
  void FileStateChanged(QSharedPointer<client::File>& file, const monocle::FileState state);
  void CurrentRowChanged(const QModelIndex& current, const QModelIndex&);
  void on_buttonmount_clicked();
  void on_buttonunmount_clicked();
  void on_buttonadd_clicked();
  void on_buttonremove_clicked();


};

}

QT_END_NAMESPACE

#endif
