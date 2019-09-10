// infoformatwindow.h
//

#ifndef IDSFFATNSDFJ49584954324DSFO5O5OPBW
#define IDSFFATNSDFJ49584954324DSFO5O5OPBW

///// Includes /////

#include <QDateTime>
#include <QDialog>
#include <string>

#include "ui_infoformatwindow.h"

///// Namespaces /////

namespace client
{

///// Classes /////

class InfoFormatWindow : public QDialog
{
 Q_OBJECT

 public:

  InfoFormatWindow(QWidget* parent, const QString& infotextformat);
  ~InfoFormatWindow();
  
  inline const std::string& GetInfoTextFormat() const { return infotextformat_; }

 protected:



 private:

  const QDateTime currentdatetime_;

  Ui::InfoFormatWindow ui_;

  std::string infotextformat_;

 private slots:

  void on_editinfotextformat_textChanged(const QString& text);
  void on_comboinfotextformat_currentTextChanged(const QString& text);
  void on_buttonok_clicked();

};

}

#endif
