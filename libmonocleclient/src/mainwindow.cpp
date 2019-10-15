// mainwindow.cpp
//

///// Includes /////

#include "monocleclient/mainwindow.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <network/uri.hpp>
#include <QCloseEvent>
#include <QDirIterator>
#include <QFileDialog>
#include <QFontDatabase>
#include <QLocale>
#include <QMessageBox>
#include <QMimeData>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>
#include <QToolBar>
#include <utility/ioservicepool.hpp>

#include "monocleclient/aboutwindow.h"
#include "monocleclient/devicemgr.h"
#include "monocleclient/editdevicewindow.h"
#include "monocleclient/options.h"
#include "monocleclient/optionswindow.h"
#include "monocleclient/updatewindow.h"

///// Defines /////

#if !defined(MONOCLE_CLIENT_VERSION_MAJOR) || !defined(MONOCLE_CLIENT_VERSION_MINOR) || !defined(MONOCLE_CLIENT_VERSION_BUILD)
  #error "MONOCLE_CLIENT_VERSION_XXX not defined"
#endif

///// Namespaces /////

namespace client
{

///// Declarations /////

MainWindow* MainWindow::instance_ = nullptr;

///// Globals /////

static const QString MAINWINDOWGEOMETORY("mainwindowgeometory");
static const QString MAINWINDOWSTATE("mainwindowstate");
static const QString CURRENTLANGUAGE("currentlanguage");
static const QString MAINVIDEOWINDOWWIDTH("mainvideowindowwidth");
static const QString MAINVIDEOWINDOWHEIGHT("videowindowheight");
static const QString MAINVIDEOWINDOWSHOWTOOLBAR("videowindowshowtoolbar");
static const QString MAINSHOWTOOLBAR("showtoolbar");

///// Methods /////

CUDADEVICE::CUDADEVICE(const CUdevice device, const CUcontext context) :
  device_(device),
  context_(context)
{

}

void MainWindow::Create(const uint32_t numioservices, const uint32_t numioservicethreads)
{
  assert(!instance_);
  instance_ = new MainWindow(numioservices, numioservicethreads);
}

void MainWindow::Destroy()
{
  delete instance_;
  instance_ = nullptr;
}

MainWindow::MainWindow(const uint32_t numioservices, const uint32_t numioservicethreads) :
  version_(MONOCLE_CLIENT_VERSION_MAJOR, MONOCLE_CLIENT_VERSION_MINOR, MONOCLE_CLIENT_VERSION_BUILD),
  translationsdirectory_((QCoreApplication::applicationDirPath() + QString("/translations"))),
  arial_(new QResource(":/arial.ttf")),
  showfullscreen_(":/showfullscreen.png"),
  gen_(rd_()),
  hsvcolordist_(0.0, 1.0),
  ioservicepool_(numioservices, numioservicethreads, [](){}, [](){}),
  guiioservice_(0),
  tray_(new QSystemTrayIcon(QIcon(":/icon.ico"), this)),
  monocletranslator_(new QTranslator(this)),
  qttranslator_(new QTranslator(this)),
  videowidgetsmgr_(arial_, showfullscreen_),
  checkforupdate_(version_),
  colourpickercolour_(2.0f, 2.0f, 2.0f)
{
  instance_ = this;

  ui_.setupUi(this);
  
  connect(ui_.dockdevices, &QDockWidget::visibilityChanged, [this](bool visible) { ui_.actiondevices->setChecked(visible); });
  connect(ui_.docklog, &QDockWidget::visibilityChanged, [this](bool visible) { ui_.actionlog->setChecked(visible); });
  connect(ui_.dockplayback, &QDockWidget::visibilityChanged, [this](bool visible) { ui_.actionplayback->setChecked(visible); });
  connect(ui_.toolbar, &QToolBar::visibilityChanged, [this](bool visible) { ui_.actiontoolbar->setChecked(visible); ui_.actionfindmotion->setChecked(false); ui_.actioncolourpicker->setChecked(false); });

  const int ibmplexmonobold = QFontDatabase::addApplicationFont(":/IBMPlexMono-Bold.ttf");
  if (ibmplexmonobold == -1)
  {
    LOG_GUI_WARNING(tr("Failed to load font: IBMPlexMono-Bold.ttf"));

  }

  const int ibmplexmonoregular = QFontDatabase::addApplicationFont(":/IBMPlexMono-Regular.ttf");
  if (ibmplexmonoregular == -1)
  {
    LOG_GUI_WARNING(tr("Failed to load font: IBMPlexMono-Regular.ttf"));

  }

  const int ibmplexsansitalic = QFontDatabase::addApplicationFont(":/IBMPlexSans-Italic.ttf");
  if (ibmplexsansitalic == -1)
  {
    LOG_GUI_WARNING(tr("Failed to load font: IBMPlexSans-Italic.ttf"));

  }

  const int ibmplexsansregular = QFontDatabase::addApplicationFont(":/IBMPlexSans-Regular.ttf");
  if (ibmplexsansregular == -1)
  {
    LOG_GUI_WARNING(tr("Failed to load font: IBMPlexSans-Regular.ttf"));

  }

  // Collect all short month names for later
  std::array<char, 256> buffer;
  struct tm tm;
  for (int i = 0; i <= 11; ++i)
  {
    std::memset(&tm, 0, sizeof(tm));
    tm.tm_mon = i;
    const size_t len = std::strftime(buffer.data(), buffer.size(), "%b", &tm);
    if (len)
    {
      shortmonthnames_[static_cast<size_t>(i)] = std::string(buffer.data(), static_cast<int>(len));

    }
  }

  // Collect all short week day names for later
  for (int i = 0; i <= 6; ++i)
  {
    std::memset(&tm, 0, sizeof(tm));
    tm.tm_wday = i; // Week starts on monday damnit
    const size_t len = std::strftime(buffer.data(), buffer.size(), "%a", &tm);
    if (len)
    {
      shortweekdaynames_[static_cast<size_t>((i + 6) % 7)] = std::string(buffer.data(), static_cast<int>(len));
      
    }
  }

  // Get the CUDA devices and create a context on each of them
#ifdef _WIN32
  if (cuInit(0) == CUDA_SUCCESS)
  {
    int tmp = 0;
    if (cuDeviceGetCount(&tmp) == CUDA_SUCCESS)
    {
      cudadevices_.reserve(tmp);
      for (int i = 0; i < tmp; ++i)
      {
        CUcontext context = nullptr;
        if (cuDevicePrimaryCtxRetain(&context, i) != CUDA_SUCCESS)
        {

          continue;
        }
        cudadevices_.push_back(CUDADEVICE(i, context));
      }
    }
  }
#else

#endif

  if (!QDir().mkpath(QString(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))))
  {
    LOG_GUI_WARNING(tr("Could not locate or create: ") + QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

  }

  if (log_.Init("log.txt"))
  {
    LOG_GUI_WARNING(tr("Could not open log.txt"));

  }

  if (!arial_->isValid())
  {
    LOG_GUI_WARNING(tr("Error retrieving Arial resource"));

  }

  const QString publickey(":/licensepub.pem");
  const QResource file(publickey);
  if (!file.isValid())
  {
    LOG_GUI_WARNING(tr("Unable to load public key: ") + publickey);
  
  }

  if (publickey_.Init(std::vector<char>(file.data(), file.data() + file.size())))
  {
    LOG_GUI_WARNING(tr("Unable to initialise public key: ") + publickey);
  
  }
  
  Options::Instance().Load();

  connect(&checkforupdate_, &CheckForUpdate::UpdateAvailable, this, &MainWindow::UpdateAvailable);
  connect(ui_.actionexit, &QAction::triggered, this, &QMainWindow::close);
  connect(ui_.buttonframestepbackwards, &QPushButton::clicked, ui_.playbackwidget, &PlaybackWidget::on_buttonframestepbackwards_clicked);
  connect(ui_.buttonplay, &QPushButton::clicked, ui_.playbackwidget, &PlaybackWidget::on_buttonplay_clicked);
  connect(ui_.buttonpause, &QPushButton::clicked, ui_.playbackwidget, &PlaybackWidget::on_buttonpause_clicked);
  connect(ui_.buttonstop, &QPushButton::clicked, ui_.playbackwidget, &PlaybackWidget::on_buttonstop_clicked);
  connect(ui_.buttonframestepforwards, &QPushButton::clicked, ui_.playbackwidget, &PlaybackWidget::on_buttonframestepforwards_clicked);
  connect(ui_.buttonzoomin, &QPushButton::clicked, ui_.playbackwidget, &PlaybackWidget::on_buttonzoomin_clicked);
  connect(ui_.buttonzoomout, &QPushButton::clicked, ui_.playbackwidget, &PlaybackWidget::on_buttonzoomout_clicked);
  connect(ui_.buttonexport, &QPushButton::clicked, ui_.playbackwidget, &PlaybackWidget::on_buttonexport_clicked);

  QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

  // Geometory
  if (!settings.value(MAINWINDOWGEOMETORY).isNull() && !settings.value(MAINWINDOWSTATE).isNull())
  {
    restoreGeometry(settings.value(MAINWINDOWGEOMETORY).toByteArray());
    restoreState(settings.value(MAINWINDOWSTATE).toByteArray());
    restoreDockWidget(ui_.dockdevices);
    restoreDockWidget(ui_.dockplayback);
    restoreDockWidget(ui_.docklog);
  }
  else
  {
    ui_.docklog->hide(); // First load should hide the dock log

  }

  ui_.actiondevices->setChecked(ui_.dockdevices->isVisible());
  ui_.actionlog->setChecked(ui_.docklog->isVisible());
  ui_.actionplayback->setChecked(ui_.dockplayback->isVisible());
  ui_.actiontoolbar->setChecked(ui_.toolbar->isVisible());

  // Translations
  QActionGroup* langgroup = new QActionGroup(ui_.menulanguage);
  langgroup->setExclusive(true);

  connect(langgroup, &QActionGroup::triggered, this, &MainWindow::LanguageChanged);

  QStringList filenames = translationsdirectory_.entryList(QStringList(QString("monocle_*.qm")));
  if (filenames.empty())
  {
    LOG_GUI_WARNING(tr("Could not find any translations"));

  }
  else
  {
    for (int i = 0; i < filenames.size(); ++i)
    {
      static const QRegularExpression regex("monocle_([a-z]+(?:_[a-zA-Z]+)?).qm");
      const QString filename = filenames[i];
      const QRegularExpressionMatch match = regex.match(filename);
      if (!match.hasMatch())
      {
        LOG_GUI_WARNING(tr("Language did not match regular expression: ") + filename);
        continue;
      }

      QLocale locale(match.captured(1));
      if (locale.language() == QLocale::C)
      {
        LOG_GUI_WARNING(tr("Could not match locale: ") + match.captured(1));
        continue;
      }

      QAction* action = new QAction(locale.nativeLanguageName(), this);
      action->setCheckable(true);
      action->setData(match.captured(1));

      ui_.menulanguage->addAction(action);
      langgroup->addAction(action);
    }
  }

  // Select a language
  currentlanguage_ = settings.value(CURRENTLANGUAGE).toString();
  if (currentlanguage_.isEmpty())
  {
    LoadDefaultLanguage();

  }
  else
  {
    if (LoadLanguage(currentlanguage_))
    {
      LOG_GUI_WARNING(tr("Error loading translation: ") + currentlanguage_);
      LoadDefaultLanguage();
    }
  }

  // Tray
  connect(tray_, &QSystemTrayIcon::activated, this, &MainWindow::TrayActivated);

  QAction* restoreaction = new QAction(tr("&Restore"), this);
  connect(restoreaction, &QAction::triggered, this, &MainWindow::TrayRestore);

  QAction* maximiseaction = new QAction(tr("&Maximize"), this);
  connect(maximiseaction, &QAction::triggered, this, &MainWindow::TrayMaximise);

  QAction* quitaction = new QAction(tr("&Quit"), this);
  connect(quitaction, &QAction::triggered, this, &MainWindow::close);

  QMenu* traymenu = new QMenu(this);
  traymenu->addAction(restoreaction);
  traymenu->addAction(maximiseaction);
  traymenu->addSeparator();
  traymenu->addAction(quitaction);
  tray_->setContextMenu(traymenu);
  tray_->setToolTip(tr("Monocle Client"));

  if (Options::Instance().GetAlwaysShowTray())
  {
    tray_->show();

  }

  if (!QDir(Options::Instance().GetBackupDirectory()).exists())
  {
    if (!QDir().mkpath(Options::Instance().GetBackupDirectory()))
    {
      LOG_GUI_WARNING(QString("Unable to create backup directory: ") + Options::Instance().GetBackupDirectory());

    }
  }

  ui_.videowidget->Init(arial_, showfullscreen_, std::max(std::min(settings.value(MAINVIDEOWINDOWWIDTH, 2).toUInt(), MAXVIDEOWINDOWWIDTH), 1u), std::max(std::min(settings.value(MAINVIDEOWINDOWHEIGHT, 2).toUInt(), MAXVIDEOWINDOWHEIGHT), 1u), settings.value(MAINVIDEOWINDOWSHOWTOOLBAR, Options::Instance().GetDefaultShowToolbar()).toBool()); // The main window widget initialisation
  shortcutmgr_.Init();
  devicemgr_.Init();
  mediamgr_.Init();
  videowidgetsmgr_.Init();

  ui_.toolbar->setVisible(settings.value(MAINSHOWTOOLBAR, true).toBool());

  shortcutmgr_.AddShortcut(Shortcut(Qt::NoModifier, Qt::Key_F10), std::make_unique<ShowFullscreenAction>(this));

  if (Options::Instance().GetCheckForUpdateOnStartup())
  {
    checkforupdate_.Check();

  }

  startTimer(100);
}

MainWindow::~MainWindow()
{
  shortcutmgr_.Destroy();
  mediamgr_.Destroy();
  devicemgr_.Destroy();

  if (arial_)
  {
    delete arial_;
    arial_ = nullptr;
  }

  QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
  settings.setValue(MAINVIDEOWINDOWWIDTH, ui_.videowidget->GetWidth());
  settings.setValue(MAINVIDEOWINDOWHEIGHT, ui_.videowidget->GetHeight());
  settings.setValue(MAINVIDEOWINDOWSHOWTOOLBAR, !ui_.videowidget->GetToolbar()->isHidden());
  settings.setValue(MAINSHOWTOOLBAR, !ui_.toolbar->isHidden());

  videowindowmgr_.Destroy();

  log_.Destroy();
  ui_.videowidget->Destroy();
  ui_.playbackwidget->Destroy();

  ioservicepool_.Destroy();
  guiioservice_.stop();

  for (CUDADEVICE& cudadevice : cudadevices_)
  {
    if (cuDevicePrimaryCtxRelease(cudadevice.device_) != CUDA_SUCCESS)
    {
      LOG_GUI_WARNING(tr("Failed to destroy CUDA context: ") + QString::number(cudadevice.device_));

    }
  }
  cudadevices_.clear();
}

void MainWindow::ShortMonthName(const int mon, std::vector<char>& buffer) const
{
  if (mon > 11)
  {

    return;
  }

  for (size_t i = 0; i < shortmonthnames_[mon].size(); ++i)
  {
    buffer.push_back(shortmonthnames_[mon][i]);

  }
}

void MainWindow::ShortWeekDayName(const int weekday, std::vector<char>& buffer) const
{
  if (weekday > 6)
  {

    return;
  }

  for (size_t i = 0; i < shortweekdaynames_[weekday].size(); ++i)
  {
    buffer.push_back(shortweekdaynames_[weekday][i]);

  }
}

CUcontext MainWindow::GetNextCUDAContext()
{
  if (cudadevices_.empty())
  {

    return nullptr;
  }
  std::rotate(cudadevices_.begin(), cudadevices_.begin() + 1, cudadevices_.end());
  return cudadevices_.front().context_;
}

void MainWindow::UpdateTray()
{
  if (Options::Instance().GetAlwaysShowTray())
  {
    tray_->show();

  }
  else
  {
    tray_->hide();

  }
}

void MainWindow::ShowHideDocks()
{
  if (ui_.dockdevices->isVisible() && ui_.dockplayback->isVisible() && ui_.docklog->isVisible())
  {
    ui_.dockdevices->hide();
    ui_.dockplayback->hide();
    ui_.docklog->hide();
  }
  else
  {
    ui_.dockdevices->show();
    ui_.dockplayback->show();
    ui_.docklog->show();
  }
}

QColor MainWindow::GetRandomHSVColour()
{
  return QColor::fromHsvF(hsvcolordist_(gen_), hsvcolordist_(gen_), hsvcolordist_(gen_), 1.0f);
}

QVector4D MainWindow::GetRandomHSVColour4D()
{
  const QColor colour = QColor::fromHsvF(hsvcolordist_(gen_), hsvcolordist_(gen_), hsvcolordist_(gen_), 1.0f).darker(200);
  return QVector4D(colour.redF(), colour.greenF(), colour.blueF(), 1.0f);
}

void MainWindow::changeEvent(QEvent* event)
{
  switch (event->type())
  {
    case QEvent::WindowStateChange:
    {
      if ((windowState() & Qt::WindowMinimized) && Options::Instance().GetMinimiseToTray())
      {
        QTimer::singleShot(0, this, &MainWindow::hide);
        tray_->show();
      }

      break;
    }
  }

  QMainWindow::changeEvent(event);
}

bool MainWindow::IsDeviceTreeFilterHidden()
{
  return ui_.editdevicetreefilter->isHidden();
}

void MainWindow::ShowDeviceTreeFilter(const bool show)
{
  ui_.editdevicetreefilter->setVisible(show);

}

bool MainWindow::IsLocationTreeFilterHidden()
{
  return ui_.editlocationtreefilter->isHidden();
}

void MainWindow::ShowLocationTreeFilter(const bool show)
{
  ui_.editlocationtreefilter->setVisible(show);

}

void MainWindow::ResetMouseState()
{
  ui_.actionfindmotion->setChecked(false);

}

MOUSESTATE MainWindow::GetMouseState() const
{
  if (ui_.actionfindmotion->isChecked())
  {

    return MOUSESTATE_FINDMOTION;
  }
  else if (ui_.actioncolourpicker->isChecked())
  {

    return MOUSESTATE_COLOURPICKER;
  }
  else
  {

    return MOUSESTATE_SELECT;
  }
}

void MainWindow::SetColourPickerColour(const QVector3D& colour)
{
  colourpickercolour_ = colour;

}

QVector3D MainWindow::GetColourPickerColour() const
{
  if (GetMouseState() == MOUSESTATE_COLOURPICKER)
  {

    return colourpickercolour_;
  }
  else
  {

    return QVector3D(2.0f, 2.0f, 2.0f); // Hack to disable the colour picking stuff in the shader >1.0f
  }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  if (Options::Instance().GetHideMainWindowCloseDialog() == false)
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
      Options::Instance().SetHideMainWindowCloseDialog(true);

    }
  }

  videowindowmgr_.Destroy();

  // Save settings
  Options::Instance().Save();

  QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
  settings.setValue(MAINWINDOWGEOMETORY, saveGeometry());
  settings.setValue(MAINWINDOWSTATE, saveState());
  if (!currentlanguage_.isEmpty())
  {
    settings.setValue(CURRENTLANGUAGE, currentlanguage_);

  }

  event->accept();
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
  AcceptDrop(event);

}

void MainWindow::dragMoveEvent(QDragMoveEvent* event)
{
  AcceptDrop(event);

}

void MainWindow::dragLeaveEvent(QDragLeaveEvent* event)
{
  event->accept();

}

void MainWindow::dropEvent(QDropEvent* event)
{
  const QMimeData* mimedata = event->mimeData();
  if (mimedata == nullptr)
  {

    return;
  }

  for (const QUrl& format : mimedata->urls())
  {
    if (format.scheme() != "file")
    {

      continue;
    }

    const QString path = format.toLocalFile();
    if (!path.endsWith(".mon"))
    {

      continue;
    }

    MainWindow::Instance()->GetMediaMgr().AddMedia(path);
  }
}

void MainWindow::timerEvent(QTimerEvent* event)
{
  guiioservice_.reset();
  guiioservice_.poll();
}

void MainWindow::AcceptDrop(QDragMoveEvent* event)
{
  const QMimeData* mimedata = event->mimeData();
  if (mimedata == nullptr)
  {

    return;
  }

  bool hasmonoclefile = false;
  for (const QUrl& format : mimedata->urls())
  {
    if (format.scheme() != "file")
    {

      continue;
    }

    const QString path = format.path();
    if (!path.endsWith(".mon"))
    {

      continue;
    }
    hasmonoclefile = true;
  }

  if (hasmonoclefile)
  {
    event->acceptProposedAction();

  }
}

int MainWindow::LoadLanguage(const QString& language)
{
  qApp->removeTranslator(monocletranslator_);
  qApp->removeTranslator(qttranslator_);

  // Load the monocle translation unit
  if (!monocletranslator_->load(QString("monocle_") + language + QString(".qm"), translationsdirectory_.path()))
  {

    return 1;
  }

  if (monocletranslator_->isEmpty())
  {

    return 1;
  }

  qApp->installTranslator(monocletranslator_);

  // Attempt to load qt translator
  qttranslator_->load(QString("qt_") + language + QString(".qm"), translationsdirectory_.path());
  if (!qttranslator_->isEmpty())
  {
    qApp->installTranslator(qttranslator_);

  }

  if (!SelectLanguage(language))
  {
    LOG_GUI_WARNING(tr("MainWindow::SelectLanguage Failed to select language: ") + language);
    UnselectAllLanguages();
  }

  currentlanguage_ = language;

  return 0;
}

void MainWindow::LoadDefaultLanguage()
{
  // Attempt to load the system locale translation
  if (!LoadLanguage(QLocale::system().name()))
  {

    return;
  }

  // Attempt to load en_GB which should have no translations anyway
  if (!LoadLanguage(QString("en_GB")))
  {

    return;
  }

  // Fall back on having no translators
  qApp->removeTranslator(monocletranslator_);
  qApp->removeTranslator(qttranslator_);
  UnselectAllLanguages();
}

bool MainWindow::SelectLanguage(const QString& language)
{
  for (QAction* action : ui_.menulanguage->actions())
  {
    if (language == action->data().toString())
    {
      action->setChecked(true);
      return true;
    }
  }

  return false;
}

void MainWindow::UnselectAllLanguages()
{
  for (QAction* action : ui_.menulanguage->actions())
  {
    action->setChecked(false);

  }
}

void MainWindow::ToolbarUpdated()
{
  for (VideoWidget* videowidget : videowidgetsmgr_.GetVideoWidgets())
  {
    if (ui_.actionfindmotion->isChecked() || ui_.actioncolourpicker->isChecked())
    {
      videowidget->parentWidget()->setCursor(Qt::CrossCursor);

    }
    else
    {
      videowidget->parentWidget()->unsetCursor();

    }
  }

  if (!ui_.actioncolourpicker->isChecked())
  {
    colourpickercolour_ = QVector3D(2.0f, 2.0f, 2.0f); // Reset

  }
}

void MainWindow::LanguageChanged(QAction* action)
{
  if (action == nullptr)
  {
    LOG_GUI_WARNING(tr("MainWindow::LanguageChanged Failed"));
    return;
  }

  if (LoadLanguage(action->data().toString()))
  {
    LOG_GUI_WARNING(tr("Error loading language: ") + action->data().toString());

    // Select the previous language in the menu
    for (QAction* action : ui_.menulanguage->actions())
    {
      if (action->data() == currentlanguage_)
      {
        action->setChecked(true);
        return;
      }
    }
    return;
  }
}

void MainWindow::TrayActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason)
  {
    case QSystemTrayIcon::Trigger:
    {
      setWindowState(Qt::WindowActive);
      QTimer::singleShot(0, this, &MainWindow::TrayRestore);
      break;
    }
  }
}

void MainWindow::TrayRestore()
{
  showNormal();

  if (!Options::Instance().GetAlwaysShowTray())
  {
    tray_->hide();

  }
}

void MainWindow::TrayMaximise()
{
  showMaximized();

  if (!Options::Instance().GetAlwaysShowTray())
  {
    tray_->hide();

  }
}

void MainWindow::UpdateAvailable(bool updateavailable, const std::vector<UpdateVersion>& versions, const UpdateVersion& latestversion)
{
  if (!updateavailable)
  {

    return;
  }

  LOG_GUI_MESSAGE(QString("Version ") + QString::fromStdString(latestversion.version_.ToString()) + QString(" is available"));
}

void MainWindow::on_actionnewvideowindow_triggered()
{
  videowindowmgr_.CreateVideoWindow(boost::none, arial_, showfullscreen_, Options::Instance().GetDefaultVideoWindowWidth(), Options::Instance().GetDefaultVideoWindowHeight(), Options::Instance().GetDefaultShowToolbar());

}

void MainWindow::on_actionadddevice_triggered()
{
  EditDeviceWindow(this, nullptr).exec();

}

void MainWindow::on_actionimportfile_triggered()
{
  const QString filename = QFileDialog::getOpenFileName(this, tr("Import File"), QString(), tr("Monocle File (*.mon);;"));
  if (filename.isEmpty())
  {

    return;
  }
  mediamgr_.AddMedia(filename);
}

void MainWindow::on_actionoptions_triggered()
{
  OptionsWindow(this).exec();

}

void MainWindow::on_actioncheckforupdate_triggered()
{
  UpdateWindow(this, version_).exec();

}

void MainWindow::on_actionabout_triggered()
{
  AboutWindow(this, version_).exec();

}

void MainWindow::on_actiondevices_triggered()
{
  ui_.dockdevices->setVisible(!ui_.dockdevices->isVisible());

}

void MainWindow::on_actionlog_triggered()
{
  ui_.docklog->setVisible(!ui_.docklog->isVisible());

}

void MainWindow::on_actionplayback_triggered()
{
  ui_.dockplayback->setVisible(!ui_.dockplayback->isVisible());

}

void MainWindow::on_actiontoolbar_triggered()
{
  ui_.toolbar->setVisible(!ui_.toolbar->isVisible());

}

void MainWindow::on_actionfindmotion_toggled()
{
  ToolbarUpdated();

  if (ui_.actionfindmotion->isChecked()) // Disable other tools
  {
    ui_.actioncolourpicker->setChecked(false);

  }
}

void MainWindow::on_actioncolourpicker_toggled()
{
  ToolbarUpdated();

  if (ui_.actioncolourpicker->isChecked()) // Disable other tools
  {
    ui_.actionfindmotion->setChecked(false);

  }
}

void MainWindow::on_editdevicetreefilter_textChanged(const QString&)
{
  ui_.devicetree->SetFilter(ui_.editdevicetreefilter->Text());

}

void MainWindow::on_editlocationtreefilter_textChanged(const QString&)
{
  ui_.locationtree->SetFilter(ui_.editlocationtreefilter->Text());

}

}
