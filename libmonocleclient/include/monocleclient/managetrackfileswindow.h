// managetrackfileswindow.h
//

#ifndef ID2VVC49OZZDFZFDZU7OB4QAES19WKDN9G
#define ID2VVC49OZZDFZFDZU7OB4QAES19WKDN9G

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <monocleprotocol/filemonitorstate_generated.h>
#include <monocleprotocol/filestate_generated.h>
#include <QDialog>
#include <QSharedPointer>
#include <string>
#include <vector>

#include "ui_managetrackfileswindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class File;

///// Classes /////

class ManageTrackFilesWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageTrackFilesWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const std::vector< QSharedPointer<File> >& files);
  virtual ~ManageTrackFilesWindow();

  std::vector< QSharedPointer<File> > files_;

 private slots:

  void FileAdded(const uint64_t filetoken, const std::string& path, const std::string& mountpoint, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate);
  void FileRemoved(const uint64_t token);
  void on_buttonok_clicked();


 private:

  Ui::ManageTrackFilesWindow ui_;

  boost::shared_ptr<Device> device_;

};

}

QT_END_NAMESPACE

#endif
