// updatewindow.h
//

#ifndef IDF0SC8OOXXMT2XXCI4ELO1TIVAUJJFGA5
#define IDF0SC8OOXXMT2XXCI4ELO1TIVAUJJFGA5

///// Includes /////

#include <QDialog>
#include <QUrl>

#include "checkforupdate.h"
#include "ui_updatewindow.h"

///// Namespaces /////

namespace client
{

///// Classes /////

class UpdateWindow : public QDialog
{
 Q_OBJECT

 public:

  UpdateWindow(QWidget* parent, const utility::Version& currentversion);
  virtual ~UpdateWindow();

 private:

  QString InstallerPath() const;

  const utility::Version currentversion_;

  Ui::UpdateWindow ui_;

  QNetworkAccessManager* manager_;

  CheckForUpdate checkforupdate_;

  UpdateVersion latestversion_;
  
 private slots:
  
  void on_buttoncancel_clicked();
  void on_buttondownload_clicked();
  
  void ProgressDownload(qint64 received, qint64 total);
  void FinishedDownload(QNetworkReply* networkreply);
  void UpdateAvailable(bool updateavailable, const std::vector<UpdateVersion>& versions, const UpdateVersion& latestversion);

};

}

#endif
