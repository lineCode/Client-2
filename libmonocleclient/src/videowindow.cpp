// videowindow.cpp
//

///// Includes /////

#include "monocleclient/videowindow.h"

#include <QCheckBox>
#include <QMessageBox>

#include "monocleclient/mainwindow.h"
#include "monocleclient/options.h"

///// Namespaces /////

namespace client
{

///// Methods /////

VideoWindow::VideoWindow(const unsigned int index, const QResource* arial, const QIcon& showfullscreen, const unsigned int videowidgetwidth, const unsigned int videowidgetheight, const bool showtoolbar) :
  QDialog(nullptr),
  actionfocus_(new QAction(QString("Video Window ") + QString::number(index), this))
{
  connect(actionfocus_, &QAction::triggered, this, &VideoWindow::Focus);

  ui_.setupUi(this);

  setWindowTitle(actionfocus_->text());

  auto flags = windowFlags();
  flags |= Qt::WindowMaximizeButtonHint;
  flags |= Qt::WindowMinimizeButtonHint;
  setWindowFlags(flags);

  setAttribute(Qt::WA_DeleteOnClose);

  ui_.videowidget->Init(arial, showfullscreen, videowidgetwidth, videowidgetheight, showtoolbar);

  MainWindow::Instance()->GetMenuWindow()->addAction(actionfocus_);
  MainWindow::Instance()->GetShortcutMgr().AddShortcut(Shortcut(Qt::NoModifier, Qt::Key_F10), std::make_unique<ShowFullscreenAction>(this));
}

VideoWindow::~VideoWindow()
{
  MainWindow::Instance()->GetShortcutMgr().RemoveShortcut(Shortcut(Qt::NoModifier, Qt::Key_F10), ShowFullscreenAction(this));
  MainWindow::Instance()->GetMenuWindow()->removeAction(actionfocus_);
}

void VideoWindow::closeEvent(QCloseEvent* event)
{
  if (Options::Instance().GetHideVideoWindowCloseDialog() == false)
  {
    QCheckBox* checkbox = new QCheckBox("Do not show this again");
    QMessageBox messagebox;
    messagebox.setText("Are you sure?");
    messagebox.setIcon(QMessageBox::Icon::Question);
    messagebox.addButton(QMessageBox::Yes);
    messagebox.addButton(QMessageBox::No);
    messagebox.setDefaultButton(QMessageBox::Yes);
    messagebox.setCheckBox(checkbox);
    bool donotshowagain = false;
    QObject::connect(checkbox, &QCheckBox::stateChanged, [&donotshowagain](int state)
    {
      if (static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked)
      {
        donotshowagain = true;

      }
      else
      {
        donotshowagain = false;

      }
    });
    if (messagebox.exec() != QMessageBox::Yes)
    {
      event->ignore();
      return;
    }

    if (donotshowagain)
    {
      Options::Instance().SetHideVideoWindowCloseDialog(true);

    }
  }
}

void VideoWindow::Focus(bool)
{
  activateWindow();

}

}
