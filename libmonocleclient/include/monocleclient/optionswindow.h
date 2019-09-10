// optionswindow.h
//

#ifndef IDSFFATN00JSYND19BKK0IOF0VO5O5OPBW
#define IDSFFATN00JSYND19BKK0IOF0VO5O5OPBW

///// Includes /////

#include <QDialog>
#include <vector>

#include "ui_optionswindow.h"

///// Namespaces /////

namespace client
{

///// Classes /////

class OptionsWindow : public QDialog
{
 Q_OBJECT

 public:

  OptionsWindow(QWidget* parent);
  ~OptionsWindow();
  

 protected:



 private:

  Ui::OptionsWindow ui_;

 private slots:

  void on_buttoninfotexteditor_clicked();
  void on_editbackupdirectorybrowse_clicked();
  void on_buttonok_clicked();

};

}

#endif
