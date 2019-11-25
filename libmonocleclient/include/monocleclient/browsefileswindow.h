// browsefileswindow.h
//

#ifndef ID2VVSDFJDSIFJDSKXJCKCXJXCX9WKDN9G
#define ID2VVSDFJDSIFJDSKXJCKCXJXCX9WKDN9G

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <QDialog>

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

  BrowseFilesWindow(QWidget* parent, const boost::shared_ptr<Device>& device);
  virtual ~BrowseFilesWindow();

 private:

  Ui::BrowseFilesWindow ui_;

  boost::shared_ptr<Device> device_;

};

}

QT_END_NAMESPACE

#endif
