// externalwindow.hpp
//

#ifndef IDEDSKFJDSFKSDJFSKFSJFSSSSSSSM77DZ
#define IDEDSKFJDSFKSDJFSKFSJFSSSSSSSM77DZ

///// Includes /////

#ifdef _WIN32
  #include <windows.h>
#else
  #include <X11/Xlib.h>
#endif

#include <string>
#include <vector>

///// Namespaces /////

namespace utility
{

///// Classes /////

class ExternalWindow
{
 public:

#ifdef _WIN32
  ExternalWindow(const HWND hwnd);
#else
  ExternalWindow(Window* hwnd);
#endif

  void Show();
  
 private:
 
#ifdef _WIN32
  HWND hwnd_;
#else
  Window* hwnd_; 
#endif
  
};

///// Prototypes /////

std::vector<ExternalWindow> GetExternalWindows(const std::string& executablename, const std::string& classname);

}

#endif
