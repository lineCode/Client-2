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
#include <QScreen>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>
#include <QToolBar>
#include <random>
#include <utility/ioservicepool.hpp>

#include "monocleclient/aboutwindow.h"
#include "monocleclient/devicemgr.h"
#include "monocleclient/editdevicewindow.h"
#include "monocleclient/layout.h"
#include "monocleclient/managelayoutswindow.h"
#include "monocleclient/managelayoutwindow.h"
#include "monocleclient/managetrackwindow.h"
#include "monocleclient/newcameraquestionwindow.h"
#include "monocleclient/options.h"
#include "monocleclient/optionswindow.h"
#include "monocleclient/receiver.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
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
static const QString NEWDEVICEIDENTIFIERS("newdeviceidentifiers");
static const QString NEWCAMERAS("newcameras");

static const std::chrono::seconds DISCOVERY_DELAY(30);

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
  cameraicon_(":/camera.png"),
  gen_(rd_()),
  hcolordist_(0.0, 1.0),
  scolordist_(0.4, 1.0),
  vcolordist_(0.6, 1.0),
  ioservicepool_(numioservices, numioservicethreads, [](){}, [](){}),
  guiioservice_(0),
  tray_(new QSystemTrayIcon(QIcon(":/icon.ico"), this)),
  monocletranslator_(new QTranslator(this)),
  qttranslator_(new QTranslator(this)),
  videowidgetsmgr_(arial_, showfullscreen_),
  checkforupdate_(version_),
  colourpickercolour_(2.0f, 2.0f, 2.0f),
  discoverytimer_(-1),
  iotimer_(-1)
{
  instance_ = this;

  ui_.setupUi(this);
  
  connect(ui_.dockdevices, &QDockWidget::visibilityChanged, [this](bool visible) { ui_.actiondevices->setChecked(visible); });
  connect(ui_.docklog, &QDockWidget::visibilityChanged, [this](bool visible) { ui_.actionlog->setChecked(visible); });
  connect(ui_.dockplayback, &QDockWidget::visibilityChanged, [this](bool visible) { ui_.actionplayback->setChecked(visible); });
  connect(ui_.toolbar, &QToolBar::visibilityChanged, [this](bool visible) { ui_.actiontoolbar->setChecked(visible); ui_.actionfindmotion->setChecked(false); ui_.actioncolourpicker->setChecked(false); ui_.actionfindobject->setChecked(false); });

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

  ui_.editdevicetreefilter->addAction(QIcon(":/magnifyingglass.png"), QLineEdit::LeadingPosition);
  ui_.editdevicetreefilter->setPlaceholderText("Filter...");

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

  connect(&devicemgr_, &DeviceMgr::LayoutAdded, this, &MainWindow::LayoutAdded);
  connect(&devicemgr_, &DeviceMgr::LayoutChanged, this, &MainWindow::LayoutChanged);
  connect(&devicemgr_, &DeviceMgr::LayoutRemoved, this, &MainWindow::LayoutRemoved);
  connect(&videowidgetsmgr_, &VideoWidgetsMgr::ViewDestroyed, this, &MainWindow::ViewDestroyed);
  connect(&videowidgetsmgr_, &VideoWidgetsMgr::VideoViewCreated, this, &MainWindow::VideoViewCreated);
  connect(&videowidgetsmgr_, &VideoWidgetsMgr::ViewDestroyed, this, &MainWindow::ViewDestroyed);
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

  // Devices to not the warn the user about
  for (const QString& newdeviceidentifier : settings.value(NEWDEVICEIDENTIFIERS).toStringList())
  {
    try
    {
      newdeviceidentifiers_.push_back(boost::lexical_cast<uint64_t>(newdeviceidentifier.toStdString()));

    }
    catch (...)
    {

    }
  }

  for (const QString& newcamera : settings.value(NEWCAMERAS).toStringList())
  {
    newcameras_.push_back(std::make_pair(true, newcamera));

  }

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

  discover_ = boost::make_shared<onvif::wsdiscover::WsDiscoverClient>(MainWindow::Instance()->GetGUIIOService());
  discover_->hello_.connect([this](const std::vector<std::string>& addresses, const std::vector<std::string>& scopes) { DiscoverCallback(addresses, scopes); });
  if (discover_->Init())
  {
    LOG_GUI_WARNING(QString("WsDiscoverClient::Init failed"));

  }

  if (Options::Instance().GetDiscoveryHelper())
  {
    discoverytimer_ = startTimer(DISCOVERY_DELAY);

  }
  iotimer_ = startTimer(100);
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

void MainWindow::SetDiscoveryHelper(const bool discoveryhelper)
{
  if (discoveryhelper)
  {
    if (discoverytimer_ == -1)
    {
      discoverytimer_ = startTimer(DISCOVERY_DELAY);

    }
  }
  else
  {
    if (discoverytimer_ != -1)
    {
      killTimer(discoverytimer_);
      discoverytimer_ = -1;
    }
  }
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
  return QColor::fromHsvF(hcolordist_(gen_), scolordist_(gen_), vcolordist_(gen_), 1.0f);
}

QVector4D MainWindow::GetRandomHSVColour4D()
{
  const QColor colour = QColor::fromHsvF(hcolordist_(gen_), scolordist_(gen_), vcolordist_(gen_), 1.0f);
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
  ui_.actionfindobject->setChecked(false);
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
  else if (ui_.actionfindobject->isChecked())
  {

    return MOUSESTATE_FINDOBJECT;
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
    const int ret = messagebox.exec();
    if (donotshowagain)
    {
      Options::Instance().SetHideMainWindowCloseDialog(true);

    }
    if (ret != QMessageBox::Yes)
    {
      event->ignore();
      return;
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
  if (event->timerId() == discoverytimer_)
  {
    discover_->Broadcast();

  }
  else if (event->timerId() == iotimer_)
  {
    guiioservice_.reset();
    guiioservice_.poll();
  }
}

uint32_t MainWindow::GridWidth(const std::vector< QSharedPointer<LayoutWindow> >& layouts)
{
  return std::max(1u, std::min(MAXVIDEOWINDOWWIDTH, (*std::max_element(layouts.cbegin(), layouts.cend(), [](const QSharedPointer<LayoutWindow>& lhs, const QSharedPointer<LayoutWindow>& rhs) { return (lhs->gridwidth_ < rhs->gridwidth_); }))->gridwidth_));
}

uint32_t MainWindow::GridHeight(const std::vector< QSharedPointer<LayoutWindow> >& layouts)
{
  return std::max(1u, std::min(MAXVIDEOWINDOWHEIGHT, (*std::max_element(layouts.cbegin(), layouts.cend(), [](const QSharedPointer<LayoutWindow>& lhs, const QSharedPointer<LayoutWindow>& rhs) { return (lhs->gridheight_ < rhs->gridheight_); }))->gridheight_));
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
    if (ui_.actionfindmotion->isChecked() || ui_.actioncolourpicker->isChecked() || ui_.actionfindobject->isChecked())
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

void MainWindow::DiscoverCallback(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes)
{
  if (utility::Contains(scopes, "onvif://www.onvif.org/manufacturer/Monocle"))
  {
    std::vector<std::string>::const_iterator identifierscope = std::find_if(scopes.cbegin(), scopes.cend(), [](const std::string& scope) { return boost::starts_with(scope, "onvif://www.onvif.org/monocle/identifier/"); });
    if (identifierscope == scopes.cend())
    {

      return;
    }
    uint64_t identifier = 0;
    try
    {
      identifier = boost::lexical_cast<uint64_t>(identifierscope->substr(41));

    }
    catch (...)
    {

      return;
    }

    if (MainWindow::Instance()->GetDeviceMgr().GetDevice(identifier))
    {
      // We already have this device, ignore this
      return;
    }

    std::vector< std::pair<boost::asio::ip::address, uint16_t> > localaddresses;
    localaddresses.reserve(addresses.size());
    for (const std::string& address : addresses)
    {
      network::uri uri(address);
      try
      {
        uri = network::uri(address);

      }
      catch (...)
      {

        continue;
      }

      if (!uri.has_path())
      {

        continue;
      }

      if (uri.path().compare("/monocle_service"))
      {

        continue;
      }

      if (!uri.has_host())
      {

        continue;
      }

      boost::system::error_code err;
      const std::string host = uri.host().to_string();
      const boost::asio::ip::address a = boost::asio::ip::address::from_string(host, err);
      if (err)
      {

        continue;
      }

      uint16_t port = 9854;
      if (uri.has_port())
      {
        try
        {
          port = boost::lexical_cast<uint16_t>(uri.port());

        }
        catch (...)
        {

          continue;
        }
      }

      if (utility::Contains(localaddresses, std::make_pair(a, port))) // Don't add duplicates
      {

        continue;
      }

      if (a.is_v4())
      {
        if (boost::starts_with(host, "192.168.") || boost::starts_with(host, "172.") || boost::starts_with(host, "10."))
        {
          localaddresses.push_back(std::make_pair(a, port));

        }
      }
      else if (a.is_v6())
      {
        if (a.to_v6().is_site_local())
        {
          localaddresses.push_back(std::make_pair(a, port));

        }
      }
    }

    if (QApplication::activeWindow() != MainWindow::Instance()) // Only bother the user if they haven't got another window open...
    {

      return;
    }

    if (localaddresses.empty())
    {

      return;
    }

    if (MainWindow::Instance()->GetDeviceMgr().GetDevices(localaddresses, 0).size()) // If we have a device that has never connected before, but shares an address, we probably don't want to add this
    {

      return;
    }

    QMetaObject::invokeMethod(this, [this, identifier, localaddresses]()
    {
      if (utility::Contains(newdeviceidentifiers_, identifier)) // Ignore any devices that we have already queried the user with
      {

        return;
      }
      newdeviceidentifiers_.push_back(identifier);

      QStringList localaddressestext;
      for (const std::pair<boost::asio::ip::address, uint16_t>& localaddress : localaddresses)
      {
        localaddressestext.push_back(QString::fromStdString(localaddress.first.to_string()));

      }

      // If there are multiple devices to add, this will spam, but that's kind of ok I guess...
      QCheckBox* checkbox = new QCheckBox("Do not show this again");
      QMessageBox messagebox;
      messagebox.setWindowTitle("New Device Discovery: " + localaddressestext.join(" "));
      messagebox.setText("Would you like to add this device?");
      messagebox.setIcon(QMessageBox::Icon::Question);
      messagebox.addButton(QMessageBox::Yes);
      messagebox.addButton(QMessageBox::No);
      messagebox.setDefaultButton(QMessageBox::No);
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
      const int ret = messagebox.exec();
      if (donotshowagain)
      {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
        QStringList newdeviceidentifiers;
        newdeviceidentifiers.reserve(static_cast<int>(newdeviceidentifiers_.size()));
        for (const uint64_t newdeviceidenfitier : newdeviceidentifiers_)
        {
          newdeviceidentifiers.push_back(QString::number(newdeviceidenfitier));

        }
        settings.setValue(NEWDEVICEIDENTIFIERS, newdeviceidentifiers);
      }
      if (ret != QMessageBox::Yes)
      {

        return;
      }

      // Kick off a bunch of Connections, which automatically adds the device if it can authenticate, otherwise brings up the window to add it
      boost::shared_ptr< std::vector< boost::shared_ptr<sock::Connection> > > connections = boost::make_shared< std::vector< boost::shared_ptr<sock::Connection> > >();
      boost::shared_ptr<bool> connecting = boost::make_shared<bool>(false);
      const boost::shared_ptr<size_t> count = boost::make_shared<size_t>(localaddresses.size());
      for (const std::pair<boost::asio::ip::address, uint16_t>& localaddress : localaddresses)
      {
        boost::shared_ptr<Connection> connection = boost::make_shared<Connection>(MainWindow::Instance()->GetGUIIOService(), sock::ProxyParams(), QString::fromStdString(localaddress.first.to_string()), localaddress.second);
        connections->push_back(boost::make_shared<sock::Connection>(connection->Connect([identifier, count, connection, connections, connecting](const boost::system::error_code& err) mutable
        {
          if (*connecting)
          {
            // Another connection has stolen the show already, ignore this one
            return;
          }

          if (err)
          {
            --(*count);
            if (*count == 0) // If all connections failed, open the window
            {
              EditDeviceWindow(MainWindow::Instance(), connection->GetAddress(), connection->GetPort(), "admin", "password").exec();
              return;
            }
          }
          else
          {
            *connecting = true;

            boost::shared_ptr<monocle::client::Connection> c = boost::make_shared<monocle::client::Connection>();
            *c = connection->GetAuthenticationNonce([identifier, connection, c](const std::chrono::steady_clock::duration latency, const monocle::client::GETAUTHENTICATIONNONCERESPONSE& getauthenticationnonceresponse)
            {
              if (getauthenticationnonceresponse.GetErrorCode() != monocle::ErrorCode::Success)
              {
                EditDeviceWindow(MainWindow::Instance(), connection->GetAddress(), connection->GetPort(), "admin", "password").exec();
                return;
              }

              const std::string clientnonce = utility::GenerateRandomString(32);
              *c = connection->Authenticate("admin", clientnonce, monocle::AuthenticateDigest("admin", "password", getauthenticationnonceresponse.authenticatenonce_, clientnonce), [identifier, connection, c](const std::chrono::steady_clock::duration latency, const monocle::client::AUTHENTICATERESPONSE& authenticateresponse)
              {
                if (authenticateresponse.GetErrorCode() != monocle::ErrorCode::Success)
                {
                  EditDeviceWindow(MainWindow::Instance(), connection->GetAddress(), connection->GetPort(), "admin", "password").exec();
                  return;
                }

                *c = connection->GetState([identifier, connection, c](const std::chrono::steady_clock::duration latency, const monocle::client::GETSTATERESPONSE& getstateresponse)
                {
                  if (getstateresponse.GetErrorCode() != monocle::ErrorCode::Success)
                  {
                    EditDeviceWindow(MainWindow::Instance(), connection->GetAddress(), connection->GetPort(), "admin", "password").exec();
                    return;
                  }

                  if (identifier == getstateresponse.identifier_) // Make sure it is the device we discovered
                  {
                    MainWindow::Instance()->GetDeviceMgr().AddDevice(sock::ProxyParams(), connection->GetAddress(), connection->GetPort(), "admin", "password", 0, true);

                  }
                  else
                  {
                    EditDeviceWindow(MainWindow::Instance(), connection->GetAddress(), connection->GetPort(), "admin", "password").exec();

                  }
                });
              });
            });
          }
        })));
      }
    }, Qt::QueuedConnection);
  }
  else if (utility::Contains(scopes, "onvif://www.onvif.org/Profile/Streaming")) // Found an ONVIF Profile S device
  {
    QMetaObject::invokeMethod(this, [this, addresses, scopes]() 
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<uint64_t> dist(0, 20 * 1000);
      QTimer::singleShot(dist(gen), [this, addresses, scopes]() // We randomize the time we call this, because otherwise we potentially spam the user with windows
      {
        if (!Options::Instance().GetDiscoveryHelper()) // Possible this got disabled in the delay
        {

          return;
        }

        if (QApplication::activeWindow() != MainWindow::Instance()) // Only bother the user if they haven't got another window open...
        {

          return;
        }

        for (const std::pair<bool, QString>& newcamera : newcameras_) // If this camera contains an address on the ignore list, just leave
        {
          if (utility::Contains(addresses, newcamera.second.toStdString()))
          {

            return;
          }
        }

        // Discover hostnames
        std::vector<std::string> discoveryhostnames;
        discoveryhostnames.reserve(addresses.size());
        for (const std::string& address : addresses)
        {
          try
          {
            const network::uri uri(address);
            if (uri.has_host())
            {
              discoveryhostnames.push_back(uri.host().to_string());

            }
          }
          catch (...)
          {

          }
        }

        std::vector<std::string> devicehostnames;
        for (const boost::shared_ptr<Device>& device : devicemgr_.GetDevices())
        {
          if (device->GetState() != DEVICESTATE::SUBSCRIBED) // Check all devices are subscribed, this makes it easier to determine what discovered devices are currently in use or not
          {

            return;
          }

          if (device->GetFiles().empty()) // The user must setup files on each device before helping them to setup cameras
          {

            return;
          }

          // Device hostnames
          for (const QSharedPointer<Recording>& recording : device->GetRecordings())
          {
            for (const QSharedPointer<RecordingJob>& job : recording->GetJobs())
            {
              for (const QSharedPointer<RecordingJobSource>& source : job->GetSources())
              {
                const QSharedPointer<Receiver> receiver = device->GetReceiver(source->GetReceiverToken());
                if (!receiver)
                {

                  continue;
                }

                try
                {
                  const network::uri uri(receiver->GetMediaUri().toStdString());
                  if (uri.has_host())
                  {
                    devicehostnames.push_back(uri.host().to_string());

                  }
                }
                catch (...)
                {

                }
              }
            }
          }
        }

        if (!utility::Intersects(discoveryhostnames, devicehostnames)) // If no device has setup this discovered device before
        {
          std::vector<std::string>::const_iterator ipv4address = std::find_if(addresses.cbegin(), addresses.cend(), [](const std::string& address)
          {
            try
            {
              const network::uri uri(address);
              if (!uri.has_host())
              {

                return false;
              }

              boost::system::error_code err;
              const std::string host = uri.host().to_string();
              boost::asio::ip::address_v4::from_string(host, err);
              if (err)
              {

                return false;
              }

              if (boost::starts_with(host, "10.") || boost::starts_with(host, "172.16") || boost::starts_with(host, "192.168."))
              {

                return true;
              }
              else
              {

                return false;
              }
            }
            catch (...)
            {

              return false;
            }
          });
          if (ipv4address == addresses.cend())
          {

            return;
          }

          std::vector< boost::shared_ptr<Device> > devices; // This will contain a set of devices which
          devices.reserve(devicemgr_.GetDevices().size());
          for (const boost::shared_ptr<Device>& device : devicemgr_.GetDevices())
          {
            if (device->GetRecordings().size() < device->GetMaxRecordings()) // Do we have space for another recording on this device?
            {
              devices.push_back(device);

            }
          }

          if (devices.empty()) // No devices to place this recording on
          {

            return;
          }

          NewCameraQuestionWindow newcameraquestionwindow(this, QString::fromStdString(*ipv4address), scopes);
          if (newcameraquestionwindow.exec() == QDialog::Accepted)
          {
            if (newcameraquestionwindow.donotaskdevice_)
            {
              // Don't bother the user ever again with this camera
              for (const std::string& address : addresses)
              {
                newcameras_.push_back(std::make_pair(true, QString::fromStdString(address)));

              }
              SaveNewCameras();
            }

            if (newcameraquestionwindow.donotask_) // Disable the discovery helper
            {
              Options::Instance().SetDiscoveryHelper(false);

            }

            if (devices.size() == 1)
            {
              ManageTrackWindow(this, devices.front(), nullptr, nullptr, nullptr, nullptr, nullptr, QString::fromStdString(*ipv4address)).exec();

            }
            else
            {
              ManageTrackWindow(this, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QString::fromStdString(*ipv4address)).exec();

            }
          }
          else
          {
            // Don't bother the user again with these addresses this session
            for (const std::string& address : addresses)
            {
              newcameras_.push_back(std::make_pair(false, QString::fromStdString(address)));

            }
            SaveNewCameras();
          }
        }
      });
    }, Qt::QueuedConnection);
  }
}

void MainWindow::SaveNewCameras() const
{
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
  QStringList newcameras;
  newcameras.reserve(static_cast<int>(newcameras_.size()));
  for (const std::pair<bool, QString>& newcamera : newcameras_)
  {
    if (newcamera.first)
    {
      newcameras.push_back(newcamera.second);

    }
  }
  settings.setValue(NEWCAMERAS, newcameras);
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

void MainWindow::LayoutAdded(const QSharedPointer<Layout>& layout)
{
  const uint64_t token = layout->GetToken();
  for (const QAction* action : ui_.menulayouts->actions())
  {
    if (action->data().toULongLong() == token) // If we already have a menu item representing this layout, ignore this message
    {

      return;
    }
  }

  QAction* action = ui_.menulayouts->addAction(layout->GetName(), [this, token]()
  {
    // Gather up the windows from the layouts from all the devices for the selected token
    const std::vector< QSharedPointer<Layout> > layouts = MainWindow::Instance()->GetDeviceMgr().GetLayouts(token);
    if (layouts.empty())
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find layout: ") + QString::number(token), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    std::map< uint64_t, std::vector< QSharedPointer<LayoutWindow> > > windows;
    for (const QSharedPointer<Layout>& layout : layouts)
    {
    for (const QSharedPointer<LayoutWindow>& layoutwindow : layout->GetWindows())
    {
      auto window = windows.find(layout->GetToken());
      if (window == windows.end())
      {
        windows[layoutwindow->token_] = std::vector< QSharedPointer<LayoutWindow> >({ layoutwindow });

      }
      else
      {
        window->second.push_back(layoutwindow);

      }
    }
    }

    // Find whether we can match the windows we want to exist, with ones that are currently there already
    std::vector<VideoWidget*> videowidgets;
    videowidgets.reserve(videowindowmgr_.GetVideoWindows().size());
    for (VideoWindow* videowindow : videowindowmgr_.GetVideoWindows())
    {
      videowidgets.push_back(videowindow->GetVideoWidget());

    }

    // There should only be one MainWindow
    std::vector< std::pair< VideoWidget*, std::vector< QSharedPointer<LayoutWindow> > > > fittedwindows;
    fittedwindows.reserve(videowidgets.size() + 1);
    auto mainwindow = std::find_if(windows.cbegin(), windows.cend(), [this](const std::pair<uint64_t, std::vector< QSharedPointer<LayoutWindow> > >& window) { return (window.second.front()->mainwindow_); });
    if (mainwindow != windows.cend())
    {
      fittedwindows.push_back(std::make_pair(ui_.videowidget, mainwindow->second));
      windows.erase(mainwindow);
    }

    // Find any exact matches for windows
    for (auto window = windows.cbegin(); window != windows.cend();)
    {
      // If there is currently an exact match for a window, use it
      auto videowidget = std::find_if(videowidgets.cbegin(), videowidgets.cend(), [window](const VideoWidget* videowidget) { return ((videowidget->GetVideoWindowX() == window->second.front()->screenx_) && (videowidget->GetVideoWindowY() == window->second.front()->screeny_) && (videowidget->GetVideoWindowWidth() == window->second.front()->screenwidth_) && (videowidget->GetVideoWindowHeight() == window->second.front()->screenheight_)); });
      if (videowidget != videowidgets.cend())
      {
        fittedwindows.push_back(std::make_pair(*videowidget, window->second));
        videowidgets.erase(videowidget);
        window = windows.erase(window);
      }
      else
      {
        ++window;

      }
    }

    // Now find any spare window, or create one
    for (const std::pair<uint64_t, std::vector< QSharedPointer<LayoutWindow> > >& window : windows)
    {
      if (videowidgets.empty())
      {
        const uint32_t gridwidth = GridWidth(window.second);
        const uint32_t gridheight = GridHeight(window.second);
        VideoWindow* videowindow = videowindowmgr_.CreateVideoWindow(QPoint(window.second.front()->x_, window.second.front()->y_), arial_, showfullscreen_, gridwidth, gridheight, Options::Instance().GetDefaultShowToolbar());
        fittedwindows.push_back(std::make_pair(videowindow->GetVideoWidget(), window.second));
      }
      else
      {
        fittedwindows.push_back(std::make_pair(videowidgets.front(), window.second));
        videowidgets.erase(videowidgets.begin());
      }
    }
    windows.clear();

    // Fill out all the windows
    for (const std::pair< VideoWidget*, std::vector< QSharedPointer<LayoutWindow> > >& fittedwindow : fittedwindows)
    {
      int32_t x = 0;
      int32_t y = 0;
      if (QApplication::screenAt(QPoint(fittedwindow.second.front()->x_, fittedwindow.second.front()->y_)) == nullptr) // If the point in the layout is not accessible in the current setup, place the window on the primary screen
      {
        x = fittedwindow.second.front()->x_;
        y = fittedwindow.second.front()->y_;
      }
      else
      {
        QScreen* primaryscreen = QApplication::primaryScreen();
        if (primaryscreen) // Just in case
        {
          const QRect geometry = primaryscreen->geometry();
          x = geometry.x();
          y = geometry.y();
        }
      }

      if (!fittedwindow.second.front()->maximised_)
      {
        fittedwindow.first->SetWindowState(Qt::WindowNoState);

      }
      fittedwindow.first->VideoWindowMove(fittedwindow.second.front()->x_, fittedwindow.second.front()->y_);
      fittedwindow.first->VideoWindowResize(fittedwindow.second.front()->width_, fittedwindow.second.front()->height_);
      if (fittedwindow.second.front()->maximised_)
      {
        fittedwindow.first->SetWindowState(Qt::WindowMaximized);

      }
      fittedwindow.first->update(); // Not sure this is required, but can't hurt

      const uint32_t gridwidth = GridWidth(fittedwindow.second);
      const uint32_t gridheight = GridHeight(fittedwindow.second);
      fittedwindow.first->SetGridSize(gridwidth, gridheight);

      for (const QSharedPointer<LayoutWindow>& layout : fittedwindow.second)
      {
        for (const QSharedPointer<LayoutView>& mapview : layout->maps_)
        {
          const QSharedPointer<Map> map = layout->device_->GetMap(mapview->GetToken());
          if (!map)
          {
            LOG_GUI_MESSAGE(QString("Unable to find map: ") + QString::number(mapview->GetToken()));
            continue;
          }

          for (const QSharedPointer<View> view : fittedwindow.first->GetViews(mapview->GetX(), mapview->GetY(), mapview->GetWidth(), mapview->GetHeight())) // Remove any views that might get in the way
          {
            fittedwindow.first->RemoveView(view);

          }

          QSharedPointer<MapView> view = fittedwindow.first->CreateMapView(mapview->GetX(), mapview->GetY(), mapview->GetWidth(), mapview->GetHeight(), Options::Instance().GetStretchVideo(), layout->device_, map);
          if (!view)
          {
            LOG_GUI_MESSAGE(QString("Failed to create map view: ") + QString::number(mapview->GetToken()));

          }
          else
          {
            emit videowidgetsmgr_.MapViewCreated(view);

          }
        }

        for (const QSharedPointer<LayoutView>& recordingview : layout->recordings_)
        {
          const QSharedPointer<Recording> recording = layout->device_->GetRecording(recordingview->GetToken());
          if (!recording)
          {
            LOG_GUI_MESSAGE(QString("Unable to find recording: ") + QString::number(recordingview->GetToken()));
            continue;
          }

          const std::vector< QSharedPointer<RecordingTrack> > videotracks = recording->GetVideoTracks();
          if (videotracks.empty())
          {
            LOG_GUI_MESSAGE(QString("Unable to find recording track: ") + QString::number(recordingview->GetToken()));
            continue;
          }

          for (const QSharedPointer<View> view : fittedwindow.first->GetViews(recordingview->GetX(), recordingview->GetY(), recordingview->GetWidth(), recordingview->GetHeight())) // Remove any views that might get in the way
          {
            fittedwindow.first->RemoveView(view);

          }

          QSharedPointer<VideoView> videoview = fittedwindow.first->CreateVideoView(recordingview->GetX(), recordingview->GetY(), recordingview->GetWidth(), recordingview->GetHeight(), Options::Instance().GetStretchVideo(), layout->device_, recording, videotracks.front());
          if (!videoview)
          {
            LOG_GUI_MESSAGE(QString("Failed to create recording view: ") + QString::number(recordingview->GetToken()));

          }
          else
          {
            emit videowidgetsmgr_.VideoViewCreated(videoview);

          }
        }
      }
    }

    currentlayout_ = token;
    ui_.actionsavecurrentlayout->setEnabled(true);
  });
  action->setData(layout->GetToken());
}

void MainWindow::LayoutChanged(const QSharedPointer<Layout>& layout)
{
  for (QAction* action : ui_.menulayouts->actions())
  {
    if (action->data().toULongLong() == layout->GetToken())
    {
      action->setText(layout->GetName());

    }
  }
}

void MainWindow::LayoutRemoved(const uint64_t token)
{
  std::vector<QAction*> removeactions;
  for (QAction* action : ui_.menulayouts->actions())
  {
    if (action->data().toULongLong() == token)
    {
      removeactions.push_back(action);

    }
  }

  for (QAction* action : removeactions)
  {
    ui_.menulayouts->removeAction(action);

  }

  if (currentlayout_.is_initialized() && (*currentlayout_ == token))
  {
    currentlayout_.reset();
    ui_.actionsavecurrentlayout->setEnabled(false);
  }
}

void MainWindow::MapViewCreated(const QSharedPointer<MapView>& mapview)
{
  ui_.actionsavecurrentlayoutas->setEnabled(true);

}

void MainWindow::VideoViewCreated(const QSharedPointer<VideoView>& videoview)
{
  ui_.actionsavecurrentlayoutas->setEnabled(true);

}

void MainWindow::ViewDestroyed(const QSharedPointer<View>& view)
{
  if (videowidgetsmgr_.GetNumViews({ VIEWTYPE::VIEWTYPE_MONOCLE, VIEWTYPE::VIEWTYPE_MAP }) == 0)
  {
    ui_.actionsavecurrentlayoutas->setEnabled(false);

  }
  else
  {
    ui_.actionsavecurrentlayoutas->setEnabled(true); // Shouldn't be necessary, but lets do it anyway

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

void MainWindow::on_actionmanagelayouts_triggered()
{
  ManageLayoutsWindow(this).exec();

}

void MainWindow::on_actionsavecurrentlayout_triggered()
{
  if (!currentlayout_.is_initialized()) // This shouldn't happen, but just in case...
  {
    on_actionsavecurrentlayoutas_triggered();
    return;
  }

  //TODO copy much of the AddLayout stuff imo...

  //TODO we will need to use ChangeLayout and AddLayout AND RemoveLayout as ManageLayoutWindow does
    //TODO should look very similar to ManageLayoutWindow when it saves...

  //TODO look at currentlayout_

}

void MainWindow::on_actionsavecurrentlayoutas_triggered()
{
  ManageLayoutWindow managelayoutwindow(this);
  if (managelayoutwindow.exec())
  {
    currentlayout_ = managelayoutwindow.token_;
    ui_.actionsavecurrentlayout->setEnabled(true);
  }
}

void MainWindow::on_actionfindmotion_toggled()
{
  ToolbarUpdated();

  if (ui_.actionfindmotion->isChecked()) // Disable other tools
  {
    ui_.actioncolourpicker->setChecked(false);
    ui_.actionfindobject->setChecked(false);
  }
}

void MainWindow::on_actioncolourpicker_toggled()
{
  ToolbarUpdated();

  if (ui_.actioncolourpicker->isChecked()) // Disable other tools
  {
    ui_.actionfindmotion->setChecked(false);
    ui_.actionfindobject->setChecked(false);
  }
}

void MainWindow::on_actionfindobject_toggled()
{
  ToolbarUpdated();

  if (ui_.actionfindobject->isChecked()) // Disable other tools
  {
    ui_.actioncolourpicker->setChecked(false);
    ui_.actionfindmotion->setChecked(false);
  }
}

void MainWindow::on_editdevicetreefilter_textChanged(const QString&)
{
  ui_.devicetree->SetFilter(ui_.editdevicetreefilter->text());

}

void MainWindow::on_editlocationtreefilter_textChanged(const QString&)
{
  ui_.locationtree->SetFilter(ui_.editlocationtreefilter->text());

}

}
