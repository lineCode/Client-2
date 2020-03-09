// externalwindow.cpp
//

///// Includes /////

#include "utility/externalwindow.hpp"

#include <boost/filesystem/path.hpp>

#ifdef _WIN32
  #include <psapi.h>
#endif

///// Namespaces /////

namespace utility
{

///// Structures /////

struct ENUM_WINDOWS_PARAM
{
  ENUM_WINDOWS_PARAM(const std::string& executablename, const std::string& classname) :
    executablename_(executablename),
    classname_(classname)
  {

  }

  std::string executablename_;
  std::string classname_;
  std::vector<ExternalWindow> hwnds_;
};

///// Functions /////

std::vector<ExternalWindow> GetExternalWindows(const std::string& executablename, const std::string& classname)
{
#ifdef _WIN32
  ENUM_WINDOWS_PARAM param(executablename, classname);
  if (EnumWindows([](HWND hwnd, LPARAM param) -> BOOL
  {
    // We ignore errors in this function so that we keep on searching
    ENUM_WINDOWS_PARAM* hwnds = reinterpret_cast<ENUM_WINDOWS_PARAM*>(param);
    DWORD processid = 0;
    const DWORD threadid = GetWindowThreadProcessId(hwnd, &processid);
    if ((threadid == 0) || (processid == 0))
    {

      return 1;
    }

    const HANDLE hprocess = OpenProcess(PROCESS_QUERY_INFORMATION, TRUE, processid);
    if (hprocess == nullptr)
    {

      return 1;
    }

    if (GetCurrentProcessId() == processid) // Ignore our current process just in case
    {
      CloseHandle(hprocess);
      return 1;
    }

    char path[MAX_PATH];
    if (GetModuleFileNameEx(hprocess, nullptr, path, sizeof(path)) == 0)
    {
      CloseHandle(hprocess);
      return 1;
    }
    CloseHandle(hprocess);

    const boost::filesystem::path tmp(path);
    if (tmp.filename() != hwnds->executablename_)
    {

      return 1;
    }

    char classname[MAX_PATH];
    if (GetClassName(hwnd, classname, sizeof(classname)) == 0)
    {

      return 1;
    }

    if (hwnds->classname_ != classname)
    {

      return 1;
    }

    hwnds->hwnds_.push_back(ExternalWindow(hwnd));
    return 1;
  }, reinterpret_cast<LPARAM>(&param)) == 0)
  {

    return std::vector<ExternalWindow>();
  }
  return param.hwnds_;
#else
  return std::vector<ExternalWindow>();
#endif
}

///// Methods /////

#ifdef _WIN32
ExternalWindow::ExternalWindow(const HWND hwnd) :
#else
ExternalWindow::ExternalWindow(Window* hwnd) :
#endif
  hwnd_(hwnd)
{
	
}

void ExternalWindow::Show()
{
#ifdef _WIN32
  WINDOWPLACEMENT placement;
  placement.length = sizeof(placement);
  if (!GetWindowPlacement(hwnd_, &placement))
  {

    return;
  }

  if (placement.showCmd & SW_SHOWMINIMIZED) // For whatever reason we aren't able to maximimise another processes window from the tray successfully, so don't bother trying
  {

    return;
  }

  ShowWindow(hwnd_, SW_SHOW);
  SetForegroundWindow(hwnd_);
#else

#endif
}

}
