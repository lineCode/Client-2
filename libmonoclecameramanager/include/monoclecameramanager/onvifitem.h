// onvifitem.h
// 

#ifndef IDFBJDBRQZDFZXDFREWAEREEEWWWWZISRJ
#define IDFBJDBRQZDFZXDFREWAEREEEWWWWZISRJ

///// Includes /////

#include <QString>
#include <QTreeWidgetItem>

///// Namespaces /////

namespace cm
{

///// Classes /////

class ONVIFItem : public QTreeWidgetItem
{
 public:

  ONVIFItem(QTreeWidget* parent, const QString& uri, const QString& username, const QString& password);
  ~ONVIFItem();

  void Expanded();
  void Collapsed();
  void DoubleClicked();

};

}

#endif
