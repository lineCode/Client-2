// logtable.cpp
//

///// Includes /////

#include "monocleclient/logtable.h"

#include <assert.h>
#include <functional>
#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QHeaderView>
#include <QMenu>
#include <QScrollBar>
#include <QSettings>
#include <QTimer>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Globals /////

const QString LOGTABLEHEADERGEOMETORY("logtableheadergeometory");
const QString LOGTABLEHEADERSTATE("logtableheaderstate");
const QString FILTEREVENTS("filterevents");
const QString FILTERMESSAGE("filtermessage");
const QString FILTERWARNINGS("fitlerwarnings");
const QString FILTERFATAL("filterfatal");
const QString MAXROWS("maxrows");
const QString AUTOSCROLLTYPE("autoscrolltype");

///// Functions /////

AUTOSCROLL FromInt(int autoscroll, AUTOSCROLL defaultvalue)
{
  switch (autoscroll)
  {
    case AUTOSCROLL_NONE:
    {

      return AUTOSCROLL_NONE;
    }
    case AUTOSCROLL_FOLLOW:
    {

      return AUTOSCROLL_FOLLOW;
    }
    case AUTOSCROLL_SMART:
    {

      return AUTOSCROLL_SMART;
    }
    default:
    {

      return defaultvalue;
    }
  }
}

///// Methods /////

LogTable::LogTable(QWidget* parent) :
  QTableWidget(parent),
  showindexheader_(new QAction(tr("Index"), this)),
  showtypeheader_(new QAction(tr("Type"), this)),
  showtimeheader_(new QAction(tr("Time"), this)),
  showsourceheader_(new QAction(tr("Source"), this)),
  filterevents_(new QAction(tr("Events"), this)),
  filtermessage_(new QAction(tr("Debug"), this)),
  filterwarnings_(new QAction(tr("Warnings"), this)),
  filterfatal_(new QAction(tr("Fatal"), this)),
  maxrows50_(new QAction(tr("50"), this)),
  maxrows100_(new QAction(tr("100"), this)),
  maxrows200_(new QAction(tr("200"), this)),
  maxrows400_(new QAction(tr("400"), this)),
  maxrows1000_(new QAction(tr("1000"), this)),
  autoscrollnone_(new QAction(tr("None"), this)),
  autoscrollfollow_(new QAction(tr("Follow"), this)),
  autoscrollsmart_(new QAction(tr("Smart"), this)),
  filter_({{ false, false, false, false }}),
  copyrowtoclipboard_(new QAction(tr("Copy Row to Clipboard"), this)),
  deleterow_(new QAction(tr("Delete Row"), this)),
  deleteallrows_(new QAction(tr("Delete All Rows"), this)),
  maxrows_(50),
  autoscrolltype_(AUTOSCROLL_SMART),
  currentindex_(0)
{
  connect(showindexheader_, &QAction::triggered, this, &LogTable::ShowIndexHeader);
  connect(showtypeheader_, &QAction::triggered, this, &LogTable::ShowTypeHeader);
  connect(showtimeheader_, &QAction::triggered, this, &LogTable::ShowTimeHeader);
  connect(showsourceheader_, &QAction::triggered, this, &LogTable::ShowSourceHeader);
  connect(filterevents_, &QAction::triggered, this, &LogTable::FilterEvents);
  connect(filtermessage_, &QAction::triggered, this, &LogTable::FilterMessage);
  connect(filterwarnings_, &QAction::triggered, this, &LogTable::FilterWarnings);
  connect(filterfatal_, &QAction::triggered, this, &LogTable::FilterFatal);
  connect(deleteallrows_, &QAction::triggered, this, &LogTable::DeleteAllRows);
  connect(maxrows50_, &QAction::triggered, this, std::bind(&LogTable::MaxRows, this, std::placeholders::_1, 50));
  connect(maxrows100_, &QAction::triggered, this, std::bind(&LogTable::MaxRows, this, std::placeholders::_1, 100));
  connect(maxrows200_, &QAction::triggered, this, std::bind(&LogTable::MaxRows, this, std::placeholders::_1, 200));
  connect(maxrows400_, &QAction::triggered, this, std::bind(&LogTable::MaxRows, this, std::placeholders::_1, 400));
  connect(maxrows1000_, &QAction::triggered, this, std::bind(&LogTable::MaxRows, this, std::placeholders::_1, 1000));
  connect(autoscrollnone_, &QAction::triggered, this, std::bind((void(LogTable::*)(bool, AUTOSCROLL))&LogTable::AutoScroll, this, std::placeholders::_1, AUTOSCROLL_NONE));
  connect(autoscrollfollow_, &QAction::triggered, this, std::bind((void(LogTable::*)(bool, AUTOSCROLL))&LogTable::AutoScroll, this, std::placeholders::_1, AUTOSCROLL_FOLLOW));
  connect(autoscrollsmart_, &QAction::triggered, this, std::bind((void(LogTable::*)(bool, AUTOSCROLL))&LogTable::AutoScroll, this, std::placeholders::_1, AUTOSCROLL_SMART));
  connect(horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &LogTable::customContextMenuRequested);
  horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

  QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
  filter_.at(LOGLEVEL_EVENT) = settings.value(FILTEREVENTS, false).toBool();
  filter_.at(LOGLEVEL_MESSAGE) = settings.value(FILTERMESSAGE, false).toBool();
  filter_.at(LOGLEVEL_WARNING) = settings.value(FILTERWARNINGS, false).toBool();
  filter_.at(LOGLEVEL_FATAL) = settings.value(FILTERFATAL, false).toBool();

  maxrows_ = settings.value(MAXROWS, maxrows_).toInt();

  autoscrolltype_ = FromInt(settings.value(AUTOSCROLLTYPE, autoscrolltype_).toInt(), autoscrolltype_);
  
  // This is singleshot because the gui is not ready to be altered yet
  QByteArray geometory = settings.value(LOGTABLEHEADERGEOMETORY).toByteArray();
  QByteArray state = settings.value(LOGTABLEHEADERSTATE).toByteArray();
  QTimer::singleShot(0, this, [this, geometory, state]()
  {
    horizontalHeader()->restoreGeometry(geometory);
    horizontalHeader()->restoreState(state);
  });
}

LogTable::~LogTable()
{
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
  settings.setValue(LOGTABLEHEADERGEOMETORY, horizontalHeader()->saveGeometry());
  settings.setValue(LOGTABLEHEADERSTATE, horizontalHeader()->saveState());
  settings.setValue(FILTEREVENTS, filter_.at(LOGLEVEL_EVENT));
  settings.setValue(FILTERMESSAGE, filter_.at(LOGLEVEL_MESSAGE));
  settings.setValue(FILTERWARNINGS, filter_.at(LOGLEVEL_WARNING));
  settings.setValue(FILTERFATAL, filter_.at(LOGLEVEL_FATAL));
  settings.setValue(MAXROWS, maxrows_);
  settings.setValue(AUTOSCROLLTYPE, autoscrolltype_);
}

void LogTable::Log(client::LOGLEVEL loglevel, const boost::shared_ptr<client::Device> source, const QString& message)
{
  // Filter it
  if (filter_.at(loglevel))
  {
    
    return;
  }

  // Get the current visible item at the top of the list
  QTableWidgetItem* currentitem = nullptr;
  int currentrow = rowAt(rect().top() - horizontalHeader()->rect().top());
  if (currentrow != -1)
  {
    currentitem = item(currentrow, 0);

  }

  // Is the scroll bar at the bottom
  bool atbottom = false;
  if (verticalScrollBar()->sliderPosition() == verticalScrollBar()->maximum())
  {
    atbottom = true;

  }

  // Don't exceed maxrows_ by removing the oldest item
  if (rowCount() >= maxrows_)
  {
    if (currentitem && (currentitem->row() == 0))
    {
      currentitem = nullptr;

    }

    removeRow(0);
  }

  // Add the item
  insertRow(rowCount());
  setItem(rowCount() - 1, 0, new QTableWidgetItem(QString::number(++currentindex_)));
  setItem(rowCount() - 1, 1, new QTableWidgetItem(ToString(loglevel)));
  setItem(rowCount() - 1, 2, new QTableWidgetItem(QDateTime::currentDateTime().toString(Qt::DefaultLocaleShortDate)));
  setItem(rowCount() - 1, 3, new QTableWidgetItem(source ? source->GetAddress() : QString()));
  setItem(rowCount() - 1, 4, new QTableWidgetItem(message));
  
  AutoScroll(currentitem, atbottom);
}

void LogTable::ShowIndexHeader(bool checked)
{
  if (!checked)
  {
    hideColumn(0);

  }
  else
  {
    showColumn(0);

  }
}

void LogTable::ShowTypeHeader(bool checked)
{
  if (!checked)
  {
    hideColumn(1);

  }
  else
  {
    showColumn(1);

  }
}

void LogTable::ShowTimeHeader(bool checked)
{
  if (!checked)
  {
    hideColumn(2);

  }
  else
  {
    showColumn(2);

  }
}

void LogTable::ShowSourceHeader(bool checked)
{
  if (!checked)
  {
    hideColumn(3);

  }
  else
  {
    showColumn(3);

  }
}

void LogTable::FilterEvents(bool checked)
{
  filter_.at(LOGLEVEL_EVENT) = !checked;

}

void LogTable::FilterMessage(bool checked)
{
  filter_.at(LOGLEVEL_MESSAGE) = !checked;

}

void LogTable::FilterWarnings(bool checked)
{
  filter_.at(LOGLEVEL_WARNING) = !checked;

}

void LogTable::FilterFatal(bool checked)
{
  filter_.at(LOGLEVEL_FATAL) = !checked;

}

void LogTable::MaxRows(bool, int maxrows)
{
  maxrows_ = maxrows;

  int rows = rowCount();
  if (maxrows > rows)
  {

    return;
  }

  // Get the current visible item at the top of the list
  QTableWidgetItem* currentitem = nullptr;
  int currentrow = rowAt(rect().top() - horizontalHeader()->rect().top());
  if (currentrow != -1)
  {
    currentitem = item(currentrow, 0);

  }

  // Is the scroll bar at the bottom
  bool atbottom = false;
  if (verticalScrollBar()->sliderPosition() == verticalScrollBar()->maximum())
  {
    atbottom = true;

  }

  // Remove rows from the 'top'
  for (int i = (rows - maxrows - 1); i >= 0; --i)
  {
    if (currentitem && currentitem->row() == i)
    {
      currentitem = nullptr;

    }

    removeRow(i);
  }
  
  AutoScroll(currentitem, atbottom);
}

void LogTable::AutoScroll(bool, AUTOSCROLL autoscrolltype)
{
  autoscrolltype_ = autoscrolltype;
}

void LogTable::CopyRow(bool, QTableWidgetItem* i, int column)
{
  // Check the item still exists
  if (!ItemExists(i, column))
  {
    LOG_GUI_WARNING(tr("Could not copy row to clipboard"));
    return;
  }
  QApplication::clipboard()->setText(item(i->row(), 1)->text() + QString(" ") + item(i->row(), 2)->text() + QString(" ") + item(i->row(), 3)->text());
}

void LogTable::DeleteRow(bool, QTableWidgetItem* item, int column)
{
  // Check the item still exists
  if (!ItemExists(item, column))
  {
    LOG_GUI_WARNING(tr("Could not delete row"));
    return;
  }
  removeRow(item->row());
}

void LogTable::DeleteAllRows(bool)
{
  for (int i = (rowCount()-1); i >= 0; --i)
  {
    removeRow(i);

  }
}

void LogTable::contextMenuEvent(QContextMenuEvent* event)
{
  QMenu* menu = new QMenu(this);
  FiltersMenu(menu->addMenu(tr("Filters")));
  MaxRowsMenu(menu->addMenu(tr("Max Rows")));
  AutoScrollMenu(menu->addMenu(tr("Auto Scroll")));
  ColumnsMenu(menu->addMenu(tr("Columns")));
  if (rowCount() > 0)
  {
    menu->addAction(deleteallrows_);

  }
  QTableWidgetItem* item = itemAt(event->pos());
  if (item)
  {
    copyrowtoclipboard_->disconnect(this);
    deleterow_->disconnect(this);
    connect(copyrowtoclipboard_, &QAction::triggered, this, std::bind(&LogTable::CopyRow, this, std::placeholders::_1, item, item->column()));
    connect(deleterow_, &QAction::triggered, this, std::bind(&LogTable::DeleteRow, this, std::placeholders::_1, item, item->column()));
    menu->addAction(copyrowtoclipboard_);
    menu->addAction(deleterow_);
  }
  menu->exec(event->globalPos());
}

void LogTable::FiltersMenu(QMenu* menu)
{
  menu->addAction(filterevents_);
  filterevents_->setCheckable(true);
  filterevents_->setChecked(!filter_.at(LOGLEVEL_EVENT));
  menu->addAction(filtermessage_);
  filtermessage_->setCheckable(true);
  filtermessage_->setChecked(!filter_.at(LOGLEVEL_MESSAGE));
  menu->addAction(filterwarnings_);
  filterwarnings_->setCheckable(true);
  filterwarnings_->setChecked(!filter_.at(LOGLEVEL_WARNING));
  menu->addAction(filterfatal_);
  filterfatal_->setCheckable(true);
  filterfatal_->setChecked(!filter_.at(LOGLEVEL_FATAL));
}

void LogTable::MaxRowsMenu(QMenu* menu)
{
  std::function<void(QAction*, int)> add = [this, menu](QAction* action, int rows)
  {
    menu->addAction(action);
    action->setCheckable(true);
    action->setChecked(maxrows_ == rows);
    action->setEnabled(maxrows_ != rows);
  };

  add(maxrows50_, 50);
  add(maxrows100_, 100);
  add(maxrows200_, 200);
  add(maxrows400_, 400);
  add(maxrows1000_, 1000);
}

void LogTable::AutoScrollMenu(QMenu* menu)
{
  std::function<void(QAction*, AUTOSCROLL)> add = [this, menu](QAction* action, AUTOSCROLL autoscrolltype)
  {
    menu->addAction(action);
    action->setCheckable(true);
    action->setChecked(autoscrolltype_ == autoscrolltype);
    action->setEnabled(autoscrolltype_ != autoscrolltype);
  };

  add(autoscrollnone_, AUTOSCROLL_NONE);
  add(autoscrollfollow_, AUTOSCROLL_FOLLOW);
  add(autoscrollsmart_, AUTOSCROLL_SMART);
}

void LogTable::ColumnsMenu(QMenu* menu)
{
  menu->addAction(showindexheader_);
  showindexheader_->setCheckable(true);
  showindexheader_->setChecked(!isColumnHidden(0));
  menu->addAction(showtypeheader_);
  showtypeheader_->setCheckable(true);
  showtypeheader_->setChecked(!isColumnHidden(1));
  menu->addAction(showtimeheader_);
  showtimeheader_->setCheckable(true);
  showtimeheader_->setChecked(!isColumnHidden(2));
  menu->addAction(showsourceheader_);
  showsourceheader_->setCheckable(true);
  showsourceheader_->setChecked(!isColumnHidden(3));
}

bool LogTable::ItemExists(const QTableWidgetItem* i, int column)
{
  for (int j = 0; j < rowCount(); ++j)
  {
    if (i == item(j, column))
    {
      
      return true;
    }
  }

  return false;
}

void LogTable::AutoScroll(const QTableWidgetItem* viewitem, bool atbottom)
{
  switch (autoscrolltype_)
  {
    case AUTOSCROLL_NONE:
    {
      if (viewitem == nullptr) // If there wasn't a row at currentrow, then we must have been at the top beforehand
      {
        scrollToTop();
        break;
      }
      
      scrollToItem(viewitem, QAbstractItemView::PositionAtTop);
      break;
    }
    case AUTOSCROLL_FOLLOW:
    {
      scrollToBottom();
      break;
    }
    case AUTOSCROLL_SMART:
    {
      if (atbottom)
      {
        scrollToBottom();

      }
      else
      {
        if (viewitem == nullptr) // If there wasn't a row at currentrow, then we must have been at the top beforehand
        {
          scrollToTop();
          break;
        }
      
        scrollToItem(viewitem, QAbstractItemView::PositionAtTop);
      }
      break;
    }
    default:
    {
      assert(false);
      return;
    }
  }
}

void LogTable::customContextMenuRequested(const QPoint& pos)
{
  QMenu* menu = new QMenu(this);
  FiltersMenu(menu->addMenu(tr("Filters")));
  MaxRowsMenu(menu->addMenu(tr("Max Rows")));
  AutoScrollMenu(menu->addMenu(tr("Auto Scroll")));
  ColumnsMenu(menu->addMenu(tr("Columns")));
  if (rowCount() > 0)
  {
    menu->addAction(deleteallrows_);

  }
  menu->exec(mapToGlobal(pos));
}

}
