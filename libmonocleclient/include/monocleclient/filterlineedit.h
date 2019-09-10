// filterlineedit.h
//

#ifndef IDLIY0KUADFASKDJSAJDKADZZRUCV1EPVI
#define IDLIY0KUADFASKDJSAJDKADZZRUCV1EPVI

///// Includes /////

#include <QLineEdit>

///// Namespaces /////

namespace client
{

///// Classes /////

class FilterLineEdit : public QLineEdit
{
 Q_OBJECT

 public:

  FilterLineEdit(QWidget* parent);
  virtual ~FilterLineEdit();

  QString Text() const;

 protected:

  virtual void focusInEvent(QFocusEvent* event) override;
  virtual void focusOutEvent(QFocusEvent* event) override;

 private:

 
};

}

#endif
