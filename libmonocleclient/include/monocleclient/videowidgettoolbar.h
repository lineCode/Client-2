// videowidgettoolbar.h
//

#ifndef IDWGTBKKR0MDWNT11QZAYMN2UT17R82VG8
#define IDWGTBKKR0MDWNT11QZAYMN2UT17R82VG8

///// Includes /////

#include <QAction>
#include <QContextMenuEvent>
#include <QToolButton>
#include <QWidget>

///// Namespaces /////

namespace client
{

///// Classes /////

class VideoWidgetToolbar : public QWidget
{
 Q_OBJECT

 public:

  VideoWidgetToolbar(QWidget* parent);
  ~VideoWidgetToolbar();

  QToolButton* GetToolButtonAddRow();
  QToolButton* GetToolButtonRemoveRow();
  QToolButton* GetToolButtonAddColumn();
  QToolButton* GetToolButtonRemoveColumn();
  QToolButton* GetToolButtonShowFullscreen();

  void Disable();

 protected:

  virtual void contextMenuEvent(QContextMenuEvent* event) override;

 private:

  QAction* hidetoolbar_;

 private slots:

  void HideToolbar(bool);

};

}

#endif
