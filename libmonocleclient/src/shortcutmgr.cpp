// shortcutmgr.cpp
//

///// Includes /////

#include "monocleclient/shortcutmgr.h"

#include <algorithm>
#include <QShortcut>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Methods /////

ShortcutMgr::ShortcutMgr()
{

}

ShortcutMgr::~ShortcutMgr()
{

}

void ShortcutMgr::Init()
{
  qApp->installEventFilter(this);

  shortcuts_.push_back(std::make_pair(Shortcut(Qt::ControlModifier, Qt::Key_C), std::make_unique<CopyViewAction>()));

  shortcuts_.push_back(std::make_pair(Shortcut(Qt::KeypadModifier, Qt::Key_1), std::make_unique<PanTiltLastCameraAction>(-1.0f, -1.0f)));
  shortcuts_.push_back(std::make_pair(Shortcut(Qt::KeypadModifier, Qt::Key_2), std::make_unique<PanTiltLastCameraAction>(0.0f, -1.0f)));
  shortcuts_.push_back(std::make_pair(Shortcut(Qt::KeypadModifier, Qt::Key_3), std::make_unique<PanTiltLastCameraAction>(1.0f, -1.0f)));
  shortcuts_.push_back(std::make_pair(Shortcut(Qt::KeypadModifier, Qt::Key_4), std::make_unique<PanTiltLastCameraAction>(-1.0f, 0.0f)));
  shortcuts_.push_back(std::make_pair(Shortcut(Qt::KeypadModifier, Qt::Key_6), std::make_unique<PanTiltLastCameraAction>(1.0f, 0.0f)));
  shortcuts_.push_back(std::make_pair(Shortcut(Qt::KeypadModifier, Qt::Key_7), std::make_unique<PanTiltLastCameraAction>(-1.0f, 1.0f)));
  shortcuts_.push_back(std::make_pair(Shortcut(Qt::KeypadModifier, Qt::Key_8), std::make_unique<PanTiltLastCameraAction>(0.0f, 1.0f)));
  shortcuts_.push_back(std::make_pair(Shortcut(Qt::KeypadModifier, Qt::Key_9), std::make_unique<PanTiltLastCameraAction>(1.0f, 1.0f)));

  shortcuts_.push_back(std::make_pair(Shortcut(Qt::ControlModifier, Qt::Key_A), std::make_unique<SelectAllAction>()));

  shortcuts_.push_back(std::make_pair(Shortcut(Qt::NoModifier, Qt::Key_F9), std::make_unique<ShowHideDocksAction>()));

  shortcuts_.push_back(std::make_pair(Shortcut(Qt::NoModifier, Qt::Key_F11), std::make_unique<SetMenuVisibleAction>()));

  shortcuts_.push_back(std::make_pair(Shortcut(Qt::NoModifier, Qt::Key_Escape), std::make_unique<ExitFullscreenAction>()));

  shortcuts_.push_back(std::make_pair(Shortcut(Qt::KeypadModifier, Qt::Key_Plus), std::make_unique<ZoomLastCameraAction>(1.0f)));
  shortcuts_.push_back(std::make_pair(Shortcut(Qt::KeypadModifier, Qt::Key_Minus), std::make_unique<ZoomLastCameraAction>(-1.0f)));
}

void ShortcutMgr::Destroy()
{
  shortcuts_.clear();

}

void ShortcutMgr::AddShortcut(const Shortcut& shortcut, std::unique_ptr<Action>&& action)
{
  shortcuts_.push_back(std::make_pair(shortcut, std::move(action)));

}

void ShortcutMgr::RemoveShortcut(const Shortcut& shortcut, const Action& action)
{
  std::vector< std::pair<Shortcut, std::unique_ptr<Action> > >::const_iterator i = std::find_if(shortcuts_.cbegin(), shortcuts_.cend(), [&shortcut, &action](const std::pair<Shortcut, std::unique_ptr<Action> >& sc) { return ((sc.first == shortcut) && (*sc.second == action)); });
  if (i == shortcuts_.cend())
  {

    return;
  }
  shortcuts_.erase(i);
}

bool ShortcutMgr::eventFilter(QObject* obj, QEvent* event)
{
  switch (event->type())
  {
    case QEvent::KeyPress:
    {
      QKeyEvent* keyevent = static_cast<QKeyEvent*>(event);
      bool ret = false;
      for (const std::pair<Shortcut, std::unique_ptr<Action> >& shortcut : shortcuts_)
      {
        if ((shortcut.first == Shortcut(keyevent->modifiers(), static_cast<Qt::Key>(keyevent->key()))))
        {
          ret |= shortcut.second->KeyPress(keyevent->isAutoRepeat());

        }
      }
      return ret;
    }
    case QEvent::KeyRelease:
    {
      QKeyEvent* keyevent = static_cast<QKeyEvent*>(event);
      bool ret = false;
      for (const std::pair<Shortcut, std::unique_ptr<Action> >& shortcut : shortcuts_)
      {
        if ((shortcut.first == Shortcut(keyevent->modifiers(), static_cast<Qt::Key>(keyevent->key()))))
        {
          ret |= shortcut.second->KeyRelease(keyevent->isAutoRepeat());

        }
      }
      return ret;
    }
  }

  return QObject::eventFilter(obj, event);
}

}
