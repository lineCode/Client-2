// filterlineedit.cpp
//

///// Includes /////

#include "monocleclient/filterlineedit.h"

///// Namespaces /////

namespace client
{

///// Globals /////

const QString FILTER_PLACEHOLDER("Filter...");
const QString FILTER_PLACEHOLDER_STYLESHEET("color: #D3D3D3");

///// Methods /////

FilterLineEdit::FilterLineEdit(QWidget* parent) :
  QLineEdit(parent)
{
  setStyleSheet(FILTER_PLACEHOLDER_STYLESHEET);
  setText(FILTER_PLACEHOLDER);
}

FilterLineEdit::~FilterLineEdit()
{
  
}

QString FilterLineEdit::Text() const
{
  if (text() == FILTER_PLACEHOLDER)
  {

    return QString();
  }
  return text();
}

void FilterLineEdit::focusInEvent(QFocusEvent* event)
{
  setStyleSheet(QString());
  if (text() == FILTER_PLACEHOLDER)
  {
    setStyleSheet(QString());
    setText(QString());
  }
  QLineEdit::focusInEvent(event);
}

void FilterLineEdit::focusOutEvent(QFocusEvent* event)
{
  if (text().isEmpty())
  {
    setStyleSheet(FILTER_PLACEHOLDER_STYLESHEET);
    setText(FILTER_PLACEHOLDER);
  }
  QLineEdit::focusOutEvent(event);
}

}
