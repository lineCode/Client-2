// shortcut.h
// Represents a set of pressed keys

#ifndef ID9J05NQ4UFC3KXBRCXYRWQ4AJUZXZKCHL
#define ID9J05NQ4UFC3KXBRCXYRWQ4AJUZXZKCHL

///// Includes /////

#include <Qt>

///// Namespaces /////

namespace client
{

///// Classes /////

class Shortcut
{
 public:

  Shortcut(Qt::KeyboardModifiers modifiers, Qt::Key key);
  ~Shortcut();

  bool operator==(const Shortcut& rhs) const;
  bool operator<(const Shortcut& rhs) const;

 private:

  Qt::KeyboardModifiers modifiers_;
  Qt::Key key_;

};

}

#endif
