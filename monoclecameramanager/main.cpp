// main.cpp
//

///// Includes /////

#include "main.h"

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/predef.h>
#include <boost/program_options.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <stdint.h>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <utility/externalwindow.hpp>
#include <utility>
#include <vector>

#include <monoclecameramanager/mainwindow.h>

///// Qt /////

Q_DECLARE_METATYPE(int64_t)
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(uint64_t)

///// Functions /////

// We use WinMain for release builds to avoid bringing up the console entirely
#if BOOST_OS_WINDOWS && defined(NDEBUG)
int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previnstance, LPSTR cmdline, int show)
#else
int main(int argc, char** argv)
#endif
{
  srand(time(nullptr));

  // Init
  qRegisterMetaType<int64_t>("int64_t");
  qRegisterMetaType<std::string>("std::string");
  qRegisterMetaType<uint64_t>("uint64_t");

  // Program options
  boost::program_options::variables_map vm;
  boost::program_options::options_description description("Allowed options");
  description.add_options()
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
      std::cout << std::string("Monocle Camera Manager") + std::to_string(MONOCLE_CLIENT_VERSION_MAJOR) + std::string(".") + std::to_string(MONOCLE_CLIENT_VERSION_MINOR) + std::string(".") + std::to_string(MONOCLE_CLIENT_VERSION_BUILD);
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

  cm::MainWindow mainwindow;
  mainwindow.show();
  const int ret = app.exec();

#if BOOST_OS_WINDOWS && defined(NDEBUG)
  for (int i = 0; i < argc; ++i)
  {
    delete[] argv[i];

  }
  delete[] argv;
#endif
  return ret;
}
