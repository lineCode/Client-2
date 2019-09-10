// videoviewpropertieswindow.h
//

#ifndef ID2VVC12333DJZ4DKFJDSFSDFSX9WKDN9G
#define ID2VVC12333DJZ4DKFJDSFSDFSX9WKDN9G

///// Includes /////

#include <QDialog>

#include "ui_videoviewpropertieswindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class VideoView;

///// Classes /////

class VideoViewPropertiesWindow : public QDialog
{
 Q_OBJECT

 public:

  VideoViewPropertiesWindow(QWidget* parent, const QSharedPointer<VideoView>& videoview);
  virtual ~VideoViewPropertiesWindow();

 protected:

  virtual void timerEvent(QTimerEvent* event) override;

 private:

  void Render();

  Ui::VideoViewPropertiesWindow ui_;

  const QSharedPointer<VideoView> videoview_;

 signals:

 private slots:

  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
