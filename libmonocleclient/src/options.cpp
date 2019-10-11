// options.cpp
//

///// Includes /////

#include "monocleclient/options.h"

#include <algorithm>
#include <QCoreApplication>
#include <QSettings>
#include <QStandardPaths>

#include "monocleclient/videowidget.h"

///// Namespaces /////

namespace client
{

///// Globals /////

static const QString DEFAULTVIDEOWINDOWWIDTH("defaultvideowindowwidth");
static const QString DEFAULTVIDEOWINDOWHEIGHT("defaultvideowindowheight");
static const QString UPDATEFREQUENCY("updatefrequency");
static const QString DEFAULTSHOWTOOLBAR("showtoolbar");
static const QString CHECKFORUPDATEONSTARTUP("checkforupdateonstartup");
static const QString CHECKSHOWTOKENS("checkshowtokens");
static const QString CHECKHIDEMEDIACLOSEDIALOG("checkhidemediaclosedialog");
static const QString CHECKHIDEVIDEOWINDOWCLOSEDIALOG("checkhidevideowindowclosedialog");
static const QString CHECKHIDEMAINWINDOWCLOSEDIALOG("checkhidemainwindowclosedialog");

static const QString INFOTEXTFORMAT("infotextformat");
static const QString SHOWINFO("showinfo");
static const QString STRETCHVIDEO("stretchvideo");
static const QString MAXCUDADECODERSPERDEVICE("maxcudadecodersperdevice");

static const QString MINIMISETOTRAY("minimisetotray");
static const QString ALWAYSSHOWTRAY("alwaysshowtray");

static const QString BACKUPDIRECTORY("backupdirectory");

static const QString EXPORTDIRECTORY("exportdirectory");

///// Methods /////

Options::Options() :
  defaultvideowindowwidth_(2),
  defaultvideowindowheight_(2),
  updatefrequency_(33),
  defaultshowtoolbar_(true),
  checkforupdateonstartup_(true),
  showtokens_(false),
  hidemediaclosedialog_(false),
  hidevideowindowclosedialog_(false),
  hidemainwindowclosedialog_(false),
  infotextformat_("%a %b %d %T.%f %Y %k %q"),
  showinfo_(true),
  stretchvideo_(true),
  maxcudadecodersperdevice_(0),
  minimisetotray_(true),
  alwaysshowtray_(false),
  backupdirectory_(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/MonocleSecurity")),
  exportdirectory_(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation))
{
  
}

Options::~Options()
{

}

void Options::Load()
{
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

  // UI
  defaultvideowindowwidth_ = std::max(1u, std::min(MAXVIDEOWINDOWWIDTH, settings.value(DEFAULTVIDEOWINDOWWIDTH, defaultvideowindowwidth_).toUInt()));
  defaultvideowindowheight_ = std::max(1u, std::min(MAXVIDEOWINDOWHEIGHT, settings.value(DEFAULTVIDEOWINDOWHEIGHT, defaultvideowindowheight_).toUInt()));
  updatefrequency_ = std::max(0u, std::min(MAXUPDATEFREQUENCY, settings.value(UPDATEFREQUENCY, updatefrequency_).toUInt()));
  defaultshowtoolbar_ = settings.value(DEFAULTSHOWTOOLBAR, defaultshowtoolbar_).toBool();
  checkforupdateonstartup_ = settings.value(CHECKFORUPDATEONSTARTUP, checkforupdateonstartup_).toBool();
  showtokens_ = settings.value(CHECKSHOWTOKENS, showtokens_).toBool();
  hidemediaclosedialog_ = settings.value(CHECKHIDEMEDIACLOSEDIALOG, hidemediaclosedialog_).toBool();
  hidevideowindowclosedialog_ = settings.value(CHECKHIDEVIDEOWINDOWCLOSEDIALOG, hidevideowindowclosedialog_).toBool();
  hidemainwindowclosedialog_ = settings.value(CHECKHIDEMAINWINDOWCLOSEDIALOG, hidemainwindowclosedialog_).toBool();

  // Streaming
  infotextformat_ = settings.value(INFOTEXTFORMAT, QString::fromStdString(infotextformat_)).toString().toStdString();
  showinfo_ = settings.value(SHOWINFO, showinfo_).toBool();
  stretchvideo_ = settings.value(STRETCHVIDEO, stretchvideo_).toBool();
  maxcudadecodersperdevice_ = std::max(0u, std::min(100u, settings.value(MAXCUDADECODERSPERDEVICE, maxcudadecodersperdevice_).toUInt()));

  // Tray
  minimisetotray_ = settings.value(MINIMISETOTRAY, minimisetotray_).toBool();
  alwaysshowtray_ = settings.value(ALWAYSSHOWTRAY, alwaysshowtray_).toBool();

  // Directories
  backupdirectory_ = settings.value(BACKUPDIRECTORY, backupdirectory_).toString();
  exportdirectory_ = settings.value(EXPORTDIRECTORY, exportdirectory_).toString();
}

void Options::Save()
{
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

  // UI
  settings.setValue(DEFAULTVIDEOWINDOWWIDTH, defaultvideowindowwidth_);
  settings.setValue(DEFAULTVIDEOWINDOWHEIGHT, defaultvideowindowheight_);
  settings.setValue(UPDATEFREQUENCY, updatefrequency_);
  settings.setValue(DEFAULTSHOWTOOLBAR, defaultshowtoolbar_);
  settings.setValue(CHECKFORUPDATEONSTARTUP, checkforupdateonstartup_);
  settings.setValue(CHECKSHOWTOKENS, showtokens_);
  settings.setValue(CHECKHIDEMEDIACLOSEDIALOG, hidemediaclosedialog_);
  settings.setValue(CHECKHIDEVIDEOWINDOWCLOSEDIALOG, hidevideowindowclosedialog_);
  settings.setValue(CHECKHIDEMAINWINDOWCLOSEDIALOG, hidemainwindowclosedialog_);

  // Streaming
  settings.setValue(INFOTEXTFORMAT, QString::fromStdString(infotextformat_));
  settings.setValue(SHOWINFO, showinfo_);
  settings.setValue(STRETCHVIDEO, stretchvideo_);
  settings.setValue(MAXCUDADECODERSPERDEVICE, maxcudadecodersperdevice_);

  // Tray
  settings.setValue(MINIMISETOTRAY, minimisetotray_);
  settings.setValue(ALWAYSSHOWTRAY, alwaysshowtray_);

  // Directories
  settings.setValue(BACKUPDIRECTORY, backupdirectory_);
  settings.setValue(EXPORTDIRECTORY, exportdirectory_);
}

}
