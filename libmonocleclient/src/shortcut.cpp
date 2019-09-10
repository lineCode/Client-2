// shortcut.cpp
//

///// Includes /////

#include "monocleclient/shortcut.h"

///// Namespaces /////

namespace client
{

///// Methods /////

Shortcut::Shortcut(Qt::KeyboardModifiers modifiers, Qt::Key key) :
  modifiers_(modifiers),
  key_(key)
{

}

Shortcut::~Shortcut()
{

}

bool Shortcut::operator==(const Shortcut& rhs) const
{
  return ((modifiers_ == rhs.modifiers_) && (key_ == rhs.key_));
}

bool Shortcut::operator<(const Shortcut& rhs) const
{
  return (modifiers_ < rhs.modifiers_ || (!(rhs.modifiers_ < modifiers_) && key_ < rhs.key_));
}

}
