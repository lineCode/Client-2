// newcameraquestionwindow.h
//

#ifndef ID2VVC49OZCDJZJQZZXCZKXCZJXCZXCZXZ
#define ID2VVC49OZCDJZJQZZXCZKXCZJXCZXCZXZ

///// Includes /////

#include <QDialog>
#include <QString>
#include <string>
#include <vector>

#include "ui_newcameraquestionwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Classes /////

class NewCameraQuestionWindow : public QDialog
{
 Q_OBJECT

 public:

  NewCameraQuestionWindow(QWidget* parent, const QString& address, const std::vector<std::string>& scopes);
  virtual ~NewCameraQuestionWindow();

  bool donotaskdevice_;
  bool donotask_;

 private:

  Ui::NewCameraQuestionWindow ui_;

 private slots:

  void on_buttonyes_clicked();
  void on_buttonno_clicked();

};

}

QT_END_NAMESPACE

#endif
