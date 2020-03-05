// main.cpp
//

///// Includes /////

#include "main.h"

#include <boost/algorithm/string.hpp>
#include <boost/predef.h>
#include <boost/program_options.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <stdint.h>
#include <monocleclient/log.h>
#include <monocleclient/mainwindow.h>
#include <monocleclient/options.h>
#include <monocleclient/runguard.h>
#include <monocleclient/recording.h>
#include <monocleclient/recordingjob.h>
#include <monocleclient/recordingjobsource.h>
#include <monocleclient/recordingjobsourcetrack.h>
#include <monocleclient/recordingtrack.h>
#include <monocleclient/streamingprotocol.h>
#include <monocleprotocol/streamingprotocol_generated.h>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <vector>

extern "C"
{
  #include <libavformat/avformat.h>
}

///// Qt /////

Q_DECLARE_METATYPE(int64_t)
Q_DECLARE_METATYPE(boost::optional<std::string>)
Q_DECLARE_METATYPE(boost::optional<QString>)
Q_DECLARE_METATYPE(boost::optional<uint64_t>)
Q_DECLARE_METATYPE(boost::system::error_code)
Q_DECLARE_METATYPE(client::LOGLEVEL)
Q_DECLARE_METATYPE(const char*)
Q_DECLARE_METATYPE(monocle::FileState)
Q_DECLARE_METATYPE(monocle::FileMonitorState)
Q_DECLARE_METATYPE(QVector<int>)
Q_DECLARE_METATYPE(QSharedPointer<client::Device>)
Q_DECLARE_METATYPE(QSharedPointer<client::Recording>)
Q_DECLARE_METATYPE(QSharedPointer<client::RecordingJob>)
Q_DECLARE_METATYPE(QSharedPointer<client::RecordingJobSource>)
Q_DECLARE_METATYPE(QSharedPointer<client::RecordingJobSourceTrack>)
Q_DECLARE_METATYPE(QSharedPointer<client::RecordingTrack>)
Q_DECLARE_METATYPE(monocle::Codec)
Q_DECLARE_METATYPE(monocle::ReceiverMode)
Q_DECLARE_METATYPE(monocle::ReceiverState)
Q_DECLARE_METATYPE(monocle::Severity)
Q_DECLARE_METATYPE(std::chrono::steady_clock::duration)
Q_DECLARE_METATYPE(boost::shared_ptr<client::Device>)
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(std::vector<monocle::CODECINDEX>)
Q_DECLARE_METATYPE(std::vector<monocle::DISKSTAT>)
Q_DECLARE_METATYPE(std::vector<monocle::INDEX>)
Q_DECLARE_METATYPE(std::vector<monocle::RECORDINGJOBSOURCE>)
Q_DECLARE_METATYPE(std::vector<QString>)
Q_DECLARE_METATYPE(std::vector<std::string>)
Q_DECLARE_METATYPE(std::vector<uint64_t>)
Q_DECLARE_METATYPE(monocle::RecordingJobMode)
Q_DECLARE_METATYPE(monocle::RecordingJobState)
Q_DECLARE_METATYPE(monocle::StreamingProtocol)
Q_DECLARE_METATYPE(monocle::TrackType)
Q_DECLARE_METATYPE(client::STREAMINGPROTOCOL)
Q_DECLARE_METATYPE(uint32_t)
Q_DECLARE_METATYPE(uint64_t)

///// Functions /////

// We use WinMain for release builds to avoid bringing up the console entirely
#if BOOST_OS_WINDOWS && defined(NDEBUG)
int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previnstance, LPSTR cmdline, int show)
#else
int main(int argc, char** argv)
#endif
{
  av_register_all();
  av_log_set_level(AV_LOG_FATAL);
  
  client::RunGuard runguard(QString("3fa94d34-4753-4dbb-98c6-48b3acb65bf8"));
  if (!runguard.Run())
  {

    return 0;
  }

  srand(time(NULL));

  // Init
  qRegisterMetaType<int64_t>("int64_t");
  qRegisterMetaType< boost::optional<std::string> >();
  qRegisterMetaType< boost::optional<QString> >();
  qRegisterMetaType< boost::optional<uint64_t> >();
  qRegisterMetaType< boost::system::error_code >();
  qRegisterMetaType<client::LOGLEVEL>("monocle::LOGLEVEL");
  qRegisterMetaType<const char*>();
  qRegisterMetaType<monocle::FileState>();
  qRegisterMetaType<monocle::FileMonitorState>();
  qRegisterMetaType< QVector<int> >();
  qRegisterMetaType< const QSharedPointer<client::Device>& >();
  qRegisterMetaType< const QSharedPointer<client::Recording>& >();
  qRegisterMetaType< const QSharedPointer<client::RecordingJob>& >();
  qRegisterMetaType< const QSharedPointer<client::RecordingJobSource>& >();
  qRegisterMetaType< const QSharedPointer<client::RecordingJobSourceTrack>& >();
  qRegisterMetaType< QSharedPointer<client::RecordingJobSourceTrack> >();
  qRegisterMetaType< const QSharedPointer<client::RecordingTrack>& >();
  qRegisterMetaType<monocle::Codec>("monocle::Codec");
  qRegisterMetaType<monocle::ReceiverMode>("monocle::ReceiverMode");
  qRegisterMetaType<monocle::ReceiverState>("monocle::ReceiverState");
  qRegisterMetaType<std::chrono::steady_clock::duration>();
  qRegisterMetaType< boost::shared_ptr<client::Device> >();
  qRegisterMetaType<std::string>();
  qRegisterMetaType< std::vector<monocle::CODECINDEX> >();
  qRegisterMetaType< std::vector<monocle::DISKSTAT> >();
  qRegisterMetaType< std::vector<monocle::INDEX> >();
  qRegisterMetaType< std::vector<monocle::RECORDINGJOBSOURCE> >();
  qRegisterMetaType< std::vector<QString> >();
  qRegisterMetaType< std::vector<std::string> >();
  qRegisterMetaType< std::vector<uint64_t> >();
  qRegisterMetaType<uint64_t>();
  qRegisterMetaType<monocle::StreamingProtocol>("monocle::StreamingProtocol");
  qRegisterMetaType<client::STREAMINGPROTOCOL>("client::STREAMINGPROTOCOL");
  qRegisterMetaType<monocle::RecordingJobMode>("monocle::RecordingJobMode");
  qRegisterMetaType<monocle::RecordingJobState>("monocle::RecordingJobState");
  qRegisterMetaType<monocle::Severity>("monocle::Severity");
  qRegisterMetaType<monocle::TrackType>("monocle::TrackType");
  qRegisterMetaType<uint32_t>("uint32_t");
  qRegisterMetaType<uint64_t>("uint64_t");

  // Program options
  uint32_t numioservices = std::min(16u, static_cast<uint32_t>(std::thread::hardware_concurrency()));
  if (numioservices == 0)
  {
    std::cout << "Warning: Unable to determine number of processors" << std::endl;
    numioservices = 4;
  }
  uint32_t numioservicethreads = 16;

  boost::program_options::variables_map vm;
  boost::program_options::options_description description("Allowed options");
  description.add_options()
    ("numioservices", boost::program_options::value<uint32_t>(&numioservices), "Number of io_services")
    ("numioservicethreads", boost::program_options::value<uint32_t>(&numioservicethreads), "Number of io_service threads")
    ("version,v", boost::program_options::value<bool>()->implicit_value(false), "Version");

  // If using WinMain, convert cmdline to argv for boost program options. We don't use boost::split_winmain because it crashes in release mode
#if BOOST_OS_WINDOWS && defined(NDEBUG)
  std::vector<std::string> args;
  boost::split(args, std::string(cmdline), boost::is_any_of(" "), boost::algorithm::token_compress_on);
  int argc = static_cast<int>(args.size());
  char** argv = new char*[argc];
  for (int i = 0; i < argc; ++i)
  {
    argv[i] = new char[args.at(i).size() + 1];
    std::memcpy(argv[i], args.at(i).c_str(), args.at(i).size() + 1);
  }
#endif

  try
  {
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), vm);

    if (vm.count("version"))
    {
      std::cout << std::string("Monocle Security Client ") + std::to_string(MONOCLE_CLIENT_VERSION_MAJOR) + std::string(".") + std::to_string(MONOCLE_CLIENT_VERSION_MINOR) + std::string(".") + std::to_string(MONOCLE_CLIENT_VERSION_BUILD);
      return 0;
    }

    boost::program_options::notify(vm);
  }
  catch (const boost::program_options::error&)
  {
    // Do nothing I suppose?

  }
  catch (...)
  {
    // Do nothing I suppose?

  }

  // Init
  QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
  QApplication app(argc, argv);
  app.setOrganizationName(ORGANISATIONNAME);
  app.setApplicationName(APPLICATIONNAME);

  // Stylesheet
  {
    QFile f("styles.qss");
    if (f.open(QFile::ReadOnly | QFile::Text))
    {
      QTextStream in(&f);
      app.setStyleSheet(in.readAll());
    }
  }

  client::MainWindow::Create(numioservices, numioservicethreads);
  client::MainWindow::Instance()->show();
  const int ret = app.exec();
  client::MainWindow::Destroy();

#if BOOST_OS_WINDOWS && defined(NDEBUG)
  for (int i = 0; i < argc; ++i)
  {
    delete[] argv[i];

  }
  delete[] argv;
#endif
  return ret;
}
