// aboutwindow.h
//

#ifndef ID2VVC49OZCDJZJQMU7OB4QAES19WKDN9G
#define ID2VVC49OZCDJZJQMU7OB4QAES19WKDN9G

///// Includes /////

#include <QDialog>
#include <utility/version.hpp>

#include "ui_aboutwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Classes /////

class AboutWindow : public QDialog
{
 Q_OBJECT

 public:

  AboutWindow(QWidget* parent, const utility::Version& version);
  virtual ~AboutWindow();

 private:

  Ui::AboutWindow ui_;

};

}

QT_END_NAMESPACE

#endif
