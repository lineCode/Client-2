// browsefileswindow.h
//

#ifndef ID2VVSDFJDSIFJDSKXJCKCXJXCX9WKDN9G
#define ID2VVSDFJDSIFJDSKXJCKCXJXCX9WKDN9G

///// Includes /////

#include <QDialog>

#include "ui_browsefileswindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Classes /////

class BrowseFilesWindow : public QDialog
{
 Q_OBJECT

 public:

  BrowseFilesWindow(QWidget* parent);
  virtual ~BrowseFilesWindow();

 private:

  Ui::BrowseFilesWindow ui_;

};

}

QT_END_NAMESPACE

#endif
