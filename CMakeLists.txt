cmake_minimum_required(VERSION 3.0)

project(Client C CXX CUDA)

enable_testing()

##### Globals #####

if (UNIX)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif (UNIX)

add_definitions(/DMONOCLE_CLIENT_VERSION_MAJOR=1)
add_definitions(/DMONOCLE_CLIENT_VERSION_MINOR=11)
add_definitions(/DMONOCLE_CLIENT_VERSION_BUILD=0)

add_definitions(-DSQLITE_OMIT_DEPRECATED)
add_definitions(-DSQLITE_OMIT_PROGRESS_CALLBACK)
add_definitions(-DSQLITE_OMIT_SHARED_CACHE)
add_definitions(-DSQLITE_THREADSAFE=0)
add_definitions(-DSQLITE_USE_ALLOCA)

if(MSVC)
  add_definitions(-DNOMINMAX)
  add_definitions(-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
  add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake ${CMAKE_CURRENT_SOURCE_DIR}/sanitizers-cmake/cmake)

##### Packages #####

# Sanitizers
find_package(Sanitizers REQUIRED)

# Boost
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS atomic chrono date_time filesystem program_options regex system thread)

# Curl
add_definitions(-DCURL_STATICLIB)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "BUILD_CURL_TESTS")
set(BUILD_CURL_EXE OFF CACHE BOOL "BUILD_CURL_TESTS")
set(ENABLE_MANUAL OFF CACHE BOOL "ENABLE_MANUAL")
set(ENABLE_THREADED_RESOLVER ON CACHE BOOL "ENABLE_THREADED_RESOLVER")
set(BUILD_CURL_TESTS OFF CACHE BOOL "BUILD_CURL_TESTS")
set(CURL_STATICLIB ON CACHE BOOL "CURL_STATICLIB")
set(HTTP_ONLY ON CACHE BOOL "HTTP_ONLY")
set(CURL_ZLIB OFF CACHE BOOL "CURL_ZLIB")
add_subdirectory(curl EXCLUDE_FROM_ALL)

# FFMPEG
find_package(FFMPEG REQUIRED)

# freetype2
add_subdirectory(freetype2 EXCLUDE_FROM_ALL)

# googletest
set(BUILD_GTEST ON CACHE BOOL "BUILD_GTEST")
set(gtest_force_shared_crt ON CACHE BOOL "gtest_force_shared_crt")
add_subdirectory(googletest EXCLUDE_FROM_ALL)

# Libevent
set(EVENT__DISABLE_BENCHMARK ON CACHE BOOL "EVENT__DISABLE_BENCHMARK")
set(EVENT__DISABLE_DEBUG_MODE ON CACHE BOOL "EVENT__DISABLE_DEBUG_MODE")
set(EVENT__DISABLE_REGRESS ON CACHE BOOL "EVENT__DISABLE_REGRESS")
set(EVENT__DISABLE_SAMPLES ON CACHE BOOL "EVENT__DISABLE_SAMPLES")
set(EVENT__DISABLE_TESTS ON CACHE BOOL "EVENT__DISABLE_TESTS")
set(ENABLE_MANUAL OFF CACHE BOOL "ENABLE_MANUAL")
add_subdirectory(Libevent EXCLUDE_FROM_ALL)

# OpenCV
find_package(OpenCV REQUIRED)

# OpenGL
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

# Openssl
find_package(OpenSSL REQUIRED)

# Pugixml
add_subdirectory(pugixml EXCLUDE_FROM_ALL)

# Qt
set(CMAKE_PREFIX_PATH $ENV{QT_DIR})
find_package(Qt5Charts REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)

# SQLite3
if(WIN32)
  add_subdirectory(sqlite3)
endif(WIN32)

# Uri
set(Uri_BUILD_DOCS OFF CACHE BOOL "Uri_BUILD_DOCS")
set(Uri_BUILD_TESTS OFF CACHE BOOL "Uri_BUILD_TESTS")
set(Uri_USE_STATIC_CRT OFF CACHE BOOL "Uri_USE_STATIC_CRT")
set(Uri_WARNINGS_AS_ERRORS OFF CACHE BOOL "Uri_WARNINGS_AS_ERRORS")
add_subdirectory(uri)

# X11
if (UNIX)
  find_package(X11 REQUIRED)
endif(UNIX)

# ZLib
add_subdirectory(zlib EXCLUDE_FROM_ALL)

# Compiler Flags
if(MSVC)
  set(CMAKE_C_FLAGS "/W0 /MP /DWIN32 /D_WINSOCK_DEPRECATED_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS")
  set(CMAKE_C_FLAGS_DEBUG "/MDd /Od /Zi")
  set(CMAKE_C_FLAGS_RELEASE "/MD /O2 /DNDEBUG")
  set(CMAKE_CXX_FLAGS "/bigobj /EHsc /W3 /wd4351 /MP /DWIN32 /D_WINSOCK_DEPRECATED_NO_WARNINGS /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS -wd4005")
  set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Od /Zi")
  set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /DNDEBUG")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS "-w -O3")
  set(CMAKE_CXX_FLAGS "-std=c++17 -O3")
endif()

##### Subdirectories #####

add_subdirectory(clientcli)
add_subdirectory(libfile)
add_subdirectory(liblicense)
add_subdirectory(libmonocleclient)
add_subdirectory(libmonocleprotocol)
add_subdirectory(libonvifclient)
add_subdirectory(libonvifserver)
add_subdirectory(libonviftypes)
add_subdirectory(librtsp)
add_subdirectory(libsocket)
add_subdirectory(libutility)
add_subdirectory(libwsdiscover)
add_subdirectory(monocleclient)
add_subdirectory(tests)
