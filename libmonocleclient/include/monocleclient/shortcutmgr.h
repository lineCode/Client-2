// shortcutmgr.h
// 

#ifndef IDWDL7T6IXL9WNIS887983SIN1QPE1U3SI
#define IDWDL7T6IXL9WNIS887983SIN1QPE1U3SI

///// Includes /////

#include <memory>
#include <QHash>
#include <QKeyEvent>
#include <QObject>
#include <utility>
#include <vector>

#include "actions.h"
#include "shortcut.h"

///// Namespaces /////

namespace client
{

///// Classes /////

class ShortcutMgr : public QObject
{
 Q_OBJECT

 public:

  ShortcutMgr();
  ~ShortcutMgr();

  void Init();
  void Destroy();

  void AddShortcut(const Shortcut& shortcut, std::unique_ptr<Action>&& action);
  void RemoveShortcut(const Shortcut& shortcut, const Action& action);

 protected:

  bool eventFilter(QObject* obj, QEvent* event);


 private:

   std::vector< std::pair<Shortcut, std::unique_ptr<Action> > > shortcuts_;

};

}

#endif
