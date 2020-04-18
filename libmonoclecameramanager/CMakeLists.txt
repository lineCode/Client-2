cmake_minimum_required(VERSION 3.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

########## WIDGET ##########

SET(LIBMONOCLECAMERAMANAGER_HEADERS
include/monoclecameramanager/devicetree.h
include/monoclecameramanager/devicetreeonvifitem.h
include/monoclecameramanager/mainwindow.h
include/monoclecameramanager/networkmapper.h)

SET(LIBMONOCLECAMERAMANAGER_SOURCES
src/devicetree.cpp
src/devicetreeitem.cpp
src/devicetreeonvifitem.cpp
src/mainwindow.cpp
src/networkmapper.cpp)

SET(LIBMONOCLECAMERAMANAGER_FORMS
mainwindow.ui)

##### TRANSLATIONS #####

set(TRANSLATEFILES ${LIBMONOCLECAMERAMANAGER_HEADERS} ${LIBMONOCLECAMERAMANAGER_SOURCES} ${LIBMONOCLECAMERAMANAGER_FORMS})
set(LANGUAGES translations/monocle_en_US.ts translations/monocle_en_GB.ts)
message("Language files: ${LANGUAGES}")

option(UPDATETRANSLATIONS "Update source translations")
if(UPDATETRANSLATIONS)
  qt5_create_translation(TRANSLATIONFILES ${TRANSLATEFILES} ${LANGUAGES})
  message("Creating translation files: ${TRANSLATIONFILES}")
else()
  qt5_add_translation(TRANSLATIONFILES ${LANGUAGES})
  message("Updating translation files ${TRANSLATIONFILES}")
endif()

set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM true)

##### LIBRARY ######

QT5_WRAP_UI(LIBMONOCLECAMERAMANAGER_FORMS_HEADERS ${LIBMONOCLECAMERAMANAGER_FORMS})
QT5_WRAP_CPP(LIBMONOCLECAMERAMANAGER_HEADERS_MOC ${LIBMONOCLECAMERAMANAGER_HEADERS})

add_library(libmonoclecameramanager STATIC
${LIBMONOCLECAMERAMANAGER_SOURCES}
${LIBMONOCLECAMERAMANAGER_HEADERS}
${LIBMONOCLECAMERAMANAGER_HEADERS_MOC}
${LIBMONOCLECAMERAMANAGER_FORMS}
${LIBMONOCLECAMERAMANAGER_FORMS_HEADERS}
${TRANSLATIONFILES})

add_sanitizers(libmonoclecameramanager)

if(WIN32)
  target_compile_definitions(libmonoclecameramanager PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libmonoclecameramanager PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libmonoclecameramanager APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libmonoclecameramanager PROPERTY CXX_STANDARD 17)

set(TRANSLATIONSQM ${TRANSLATIONFILES} CACHE STRING "Translation files")

set_target_properties(libmonoclecameramanager PROPERTIES PREFIX "")
set_target_properties(libmonoclecameramanager PROPERTIES DEBUG_POSTFIX "")
set_target_properties(libmonoclecameramanager PROPERTIES RELEASE_POSTFIX "")

##### INCLUDES #####

include_directories(libmonoclecameramanager ${Boost_INCLUDE_DIRS})
include_directories(libmonoclecameramanager ${CMAKE_CURRENT_BINARY_DIR})
include_directories(libmonoclecameramanager ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(libmonoclecameramanager ${CUDA_INCLUDE_DIRS})
include_directories(libmonoclecameramanager ${FFMPEG_INCLUDE_DIR})
include_directories(libmonoclecameramanager ${OPENSSL_INCLUDE_DIR})
include_directories(libmonoclecameramanager ${PROJECT_BINARY_DIR}/curl/include/curl)
include_directories(libmonoclecameramanager ${PROJECT_BINARY_DIR}/zlib)
include_directories(libmonoclecameramanager ${Qt5Core_INCLUDE_DIRS})
include_directories(libmonoclecameramanager ${Qt5Network_INCLUDE_DIRS})
include_directories(libmonoclecameramanager ${Qt5OpenGL_INCLUDE_DIRS})
include_directories(libmonoclecameramanager ${Qt5Widgets_INCLUDE_DIRS})
include_directories(libmonoclecameramanager ../curl/include)
include_directories(libmonoclecameramanager ../freetype2/include)
include_directories(libmonoclecameramanager ../libonvifclient/include)
include_directories(libmonoclecameramanager ../libonviftypes/include)
include_directories(libmonoclecameramanager ../librtsp/include)
include_directories(libmonoclecameramanager ../libsocket/include)
include_directories(libmonoclecameramanager ../libutility/include)
include_directories(libmonoclecameramanager ../libwsdiscover/include)
include_directories(libmonoclecameramanager ../pugixml/include)
include_directories(libmonoclecameramanager ../uri/include)
include_directories(libmonoclecameramanager ../zlib)
include_directories(libmonoclecameramanager include)

if(CMAKE_COMPILER_IS_GNUCXX)
  include_directories(libmonoclecameramanager pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

##### LIBRARIES #####

target_link_libraries(libmonoclecameramanager ${OPENGL_LIBRARIES})
target_link_libraries(libmonoclecameramanager ${OPENSSL_LIBRARIES})
target_link_libraries(libmonoclecameramanager ${Qt5Gui_EGL_LIBRARIES})
target_link_libraries(libmonoclecameramanager ${Qt5Gui_LIBRARIES})
target_link_libraries(libmonoclecameramanager debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE})
target_link_libraries(libmonoclecameramanager debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libmonoclecameramanager debug ${Boost_FILESYSTEM_LIBRARY_DEBUG} optimized ${Boost_FILESYSTEM_LIBRARY_RELEASE})
target_link_libraries(libmonoclecameramanager debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libmonoclecameramanager debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libmonoclecameramanager debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE})
target_link_libraries(libmonoclecameramanager Qt5::Charts)
target_link_libraries(libmonoclecameramanager Qt5::Core)
target_link_libraries(libmonoclecameramanager Qt5::Gui)
target_link_libraries(libmonoclecameramanager Qt5::Network)
target_link_libraries(libmonoclecameramanager Qt5::OpenGL)
target_link_libraries(libmonoclecameramanager Qt5::Widgets)
target_link_libraries(libmonoclecameramanager freetype)
target_link_libraries(libmonoclecameramanager libcurl)
target_link_libraries(libmonoclecameramanager libonvifclient)
target_link_libraries(libmonoclecameramanager libonviftypes)
target_link_libraries(libmonoclecameramanager librtsp)
target_link_libraries(libmonoclecameramanager libsocket)
target_link_libraries(libmonoclecameramanager libwsdiscover)
target_link_libraries(libmonoclecameramanager libutility)
target_link_libraries(libmonoclecameramanager network-uri)
target_link_libraries(libmonoclecameramanager zlibstatic)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(libmonoclecameramanager pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(WIN32)
  target_link_libraries(libmonoclecameramanager ${FFMPEG_LIBRARIES})
  target_link_libraries(libmonoclecameramanager Iphlpapi.lib)
  target_link_libraries(libmonoclecameramanager netapi32.lib)
  target_link_libraries(libmonoclecameramanager Qt5::Gui_GLESv2)
  target_link_libraries(libmonoclecameramanager wbemuuid.lib)
  target_link_libraries(libmonoclecameramanager Ws2_32.lib)
else()
  target_link_libraries(libmonoclecameramanager ${X11_LIBRARIES})
  target_link_libraries(libmonoclecameramanager avcodec.so)
  target_link_libraries(libmonoclecameramanager avformat.so)
  target_link_libraries(libmonoclecameramanager pthread)
  target_link_libraries(libmonoclecameramanager swscale.so)
endif()
