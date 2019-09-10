// actions.h
// 

#ifndef IDFBJDBRQUSPD5WPRRZBMIT5TZ6376ISRJ
#define IDFBJDBRQUSPD5WPRRZBMIT5TZ6376ISRJ

///// Includes /////

#include <onvifclient/connection.hpp>
#include <QObject>
#include <QSharedPointer>

#include "videoview.h"

///// Namespaces /////

namespace client
{

///// Classes /////

class Action : public QObject
{
 Q_OBJECT

 public:

  Action();
  virtual ~Action();

  virtual bool KeyPress(const bool autorepeat) = 0;
  virtual bool KeyRelease(const bool autorepeat) = 0;

  virtual bool operator==(const Action& action) = 0;

 protected:

  bool IsVideoWidgetWindowActive() const;

};

class CopyViewAction : public Action
{
  Q_OBJECT

 public:

  CopyViewAction();
  ~CopyViewAction();

  bool KeyPress(const bool autorepeat) override;
  bool KeyRelease(const bool autorepeat) override;

  virtual bool operator==(const Action& action) override;

 private:

};

class ShowFullscreenAction : public Action
{
  Q_OBJECT

 public:

  ShowFullscreenAction(QWidget* window);
  ~ShowFullscreenAction();

  bool KeyPress(const bool autorepeat) override;
  bool KeyRelease(const bool autorepeat) override;

  virtual bool operator==(const Action& action) override;

 private:

  QWidget* window_;

};

class ExitFullscreenAction : public Action
{
  Q_OBJECT

 public:

  ExitFullscreenAction();
  ~ExitFullscreenAction();

  bool KeyPress(const bool autorepeat) override;
  bool KeyRelease(const bool autorepeat) override;

  virtual bool operator==(const Action& action) override;

};

class ShowHideDocksAction : public Action
{
  Q_OBJECT

public:

  ShowHideDocksAction();
  ~ShowHideDocksAction();

  bool KeyPress(const bool autorepeat) override;
  bool KeyRelease(const bool autorepeat) override;

  virtual bool operator==(const Action& action) override;

private:

};

class PanTiltLastCameraAction : public Action
{
  Q_OBJECT

 public:

  PanTiltLastCameraAction(const float x, const float y); // x and y are in a range of -1.0f to 1.0f
  ~PanTiltLastCameraAction();

  bool KeyPress(const bool autorepeat) override;
  bool KeyRelease(const bool autorepeat) override;

  virtual bool operator==(const Action& action) override;

 private:

  bool Stop();

  const float x_;
  const float y_;

  QWeakPointer<VideoView> videoview_;
  boost::optional<std::string> profiletoken_;

  onvif::Connection continuousmoveconnection_;
  onvif::Connection stopconnection_;

  private slots:

  void FocusChanged(QWidget*, QWidget* now);

};

class SelectAllAction : public Action
{
  Q_OBJECT

 public:

  SelectAllAction();
  ~SelectAllAction();

  bool KeyPress(const bool autorepeat) override;
  bool KeyRelease(const bool autorepeat) override;

  virtual bool operator==(const Action& action) override;

};

class SetMenuVisibleAction : public Action
{
  Q_OBJECT

public:

  SetMenuVisibleAction();
  ~SetMenuVisibleAction();

  bool KeyPress(const bool autorepeat) override;
  bool KeyRelease(const bool autorepeat) override;

  virtual bool operator==(const Action& action) override;

};

class ZoomLastCameraAction : public Action
{
 Q_OBJECT

 public:

  ZoomLastCameraAction(float x); // x is in a range of -1.0f to 1.0f
  ~ZoomLastCameraAction();

  bool KeyPress(const bool autorepeat) override;
  bool KeyRelease(const bool autorepeat) override;

  virtual bool operator==(const Action& action) override;

 private:

  bool Stop();

  const float x_;

  QWeakPointer<VideoView> videoview_;
  boost::optional<std::string> profiletoken_;

  onvif::Connection continuousmoveconnection_;
  onvif::Connection stopconnection_;

 private slots:

  void FocusChanged(QWidget*, QWidget* now);

};

}

#endif
