// videowindow.h
//

#ifndef ID6MNPLBCJXMASBJFMYUW9MD2NL2EGG22S
#define ID6MNPLBCJXMASBJFMYUW9MD2NL2EGG22S

///// Includes /////

#include <QAction>
#include <QDialog>

#include "ui_videowindow.h"
#include "monocleclient/videowidget.h"

///// Namespaces /////

namespace client
{

///// Classes /////

class VideoWindow : public QDialog
{
 Q_OBJECT

 public:

  VideoWindow(const unsigned int index, const QResource* arial, const QIcon& showfullscreen, const unsigned int videowidgetwidth, const unsigned int videowidgetheight, const bool showtoolbar);
  ~VideoWindow();

  VideoWidget* GetVideoWidget() { return ui_.videowidget; }


 protected:

  void closeEvent(QCloseEvent* event);


 private:

  Ui::VideoWindow ui_;

  QAction* actionfocus_;

 private slots:

  void Focus(bool);

};

}

#endif
