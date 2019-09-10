// logtable.h
//

#ifndef IDSYTO6NXA4Y2GUGF78SYDTA5VF07BIQHW
#define IDSYTO6NXA4Y2GUGF78SYDTA5VF07BIQHW

///// Includes /////

#include <array>
#include <QAction>
#include <QMouseEvent>
#include <QString>
#include <QTableWidget>
#include <stdint.h>

#include "log.h"
#include "device.h"

///// Namespaces /////

namespace client
{

///// Enumerations /////

enum AUTOSCROLL
{
  AUTOSCROLL_NONE,
  AUTOSCROLL_FOLLOW,
  AUTOSCROLL_SMART
};

///// Prototypes /////

AUTOSCROLL FromInt(int autoscroll, AUTOSCROLL defaultvalue);

///// Classes /////

class LogTable : public QTableWidget
{
 Q_OBJECT

 public:

  LogTable(QWidget* parent);
  virtual ~LogTable();

  Q_INVOKABLE void Log(client::LOGLEVEL loglevel, const boost::shared_ptr<client::Device> source, const QString& message); // This requires the fully qualified client::LEVEL for Qt to recognise it as Q_INVOKABLE

  void ShowIndexHeader(bool checked);
  void ShowTypeHeader(bool checked);
  void ShowTimeHeader(bool checked);
  void ShowSourceHeader(bool checked);

  void FilterEvents(bool checked);
  void FilterMessage(bool checked);
  void FilterWarnings(bool checked);
  void FilterFatal(bool checked);

  void MaxRows(bool, int maxrows);

  void AutoScroll(bool, AUTOSCROLL autoscrolltype);

  void CopyRow(bool, QTableWidgetItem* item, int column);
  void DeleteRow(bool, QTableWidgetItem* item, int column);
  void DeleteAllRows(bool);


 protected:

  virtual void contextMenuEvent(QContextMenuEvent* event) override;



 private:

  void FiltersMenu(QMenu* menu);
  void MaxRowsMenu(QMenu* menu);
  void AutoScrollMenu(QMenu* menu);
  void ColumnsMenu(QMenu* menu);
  bool ItemExists(const QTableWidgetItem* i, int column);
  void AutoScroll(const QTableWidgetItem* viewitem, bool atbottom);

  QAction* showindexheader_;
  QAction* showtypeheader_;
  QAction* showtimeheader_;
  QAction* showsourceheader_;

  QAction* filterevents_;
  QAction* filtermessage_;
  QAction* filterwarnings_;
  QAction* filterfatal_;

  QAction* maxrows50_;
  QAction* maxrows100_;
  QAction* maxrows200_;
  QAction* maxrows400_;
  QAction* maxrows1000_;

  QAction* autoscrollnone_;
  QAction* autoscrollfollow_;
  QAction* autoscrollsmart_;

  QAction* copyrowtoclipboard_;
  QAction* deleterow_;
  QAction* deleteallrows_;

  std::array<bool, 4> filter_;
  int maxrows_;
  AUTOSCROLL autoscrolltype_;
  uint64_t currentindex_;

 private slots:
 
  void customContextMenuRequested(const QPoint& pos);

};

}

#endif
