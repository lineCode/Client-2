cmake_minimum_required(VERSION 3.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

########## CLIENT ##########

if (WIN32)
  set(RESOURCES resources/resources.rc)
endif(WIN32)

add_executable(monoclecameramanager
main.h
main.cpp
resources/resources.qrc
${RESOURCES})

add_sanitizers(monoclecameramanager)

if(WIN32)
  target_compile_definitions(monoclecameramanager PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(monoclecameramanager PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_target_properties(monoclecameramanager PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /LTCG")
endif()

set_property(TARGET monoclecameramanager PROPERTY CXX_STANDARD 17)

# Create a translations directory next to the exe and place the qm files in there after every build
add_custom_command(TARGET monoclecameramanager PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:monoclecameramanager>/translations)
foreach(TRANSLATIONQM ${TRANSLATIONSQM})
  add_custom_command(TARGET monoclecameramanager POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TRANSLATIONQM} $<TARGET_FILE_DIR:monoclecameramanager>/translations)
endforeach(TRANSLATIONQM)

set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM true)

##### INCLUDES #####

include_directories(monoclecameramanager ${Boost_INCLUDE_DIRS})
include_directories(monoclecameramanager ${CMAKE_CURRENT_BINARY_DIR})
include_directories(monoclecameramanager ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(monoclecameramanager ${CUDA_INCLUDE_DIRS})
include_directories(monoclecameramanager ${FFMPEG_INCLUDE_DIR})
include_directories(monoclecameramanager ${OPENSSL_INCLUDE_DIR})
include_directories(monoclecameramanager ${PROJECT_BINARY_DIR}/curl/include/curl)
include_directories(monoclecameramanager ${PROJECT_BINARY_DIR}/libmonoclecameramanager)
include_directories(monoclecameramanager ${PROJECT_BINARY_DIR}/zlib)
include_directories(monoclecameramanager ${Qt5Core_INCLUDE_DIRS})
include_directories(monoclecameramanager ${Qt5Network_INCLUDE_DIRS})
include_directories(monoclecameramanager ${Qt5OpenGL_INCLUDE_DIRS})
include_directories(monoclecameramanager ${Qt5Widgets_INCLUDE_DIRS})
include_directories(monoclecameramanager ../curl/include)
include_directories(monoclecameramanager ../freetype2/include)
include_directories(monoclecameramanager ../libmonoclecameramanager/include)
include_directories(monoclecameramanager ../libonvifclient/include)
include_directories(monoclecameramanager ../libonviftypes/include)
include_directories(monoclecameramanager ../libsocket/include)
include_directories(monoclecameramanager ../libutility/include)
include_directories(monoclecameramanager ../libwsdiscover/include)
include_directories(monoclecameramanager ../pugixml/src)
include_directories(monoclecameramanager ../uri/include)
include_directories(monoclecameramanager ../zlib)

if(CMAKE_COMPILER_IS_GNUCXX)
  include_directories(monoclecameramanager pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

##### LIBRARIES #####

target_link_libraries(monoclecameramanager ${OPENGL_LIBRARIES})
target_link_libraries(monoclecameramanager ${OPENSSL_LIBRARIES})
target_link_libraries(monoclecameramanager ${Qt5Gui_EGL_LIBRARIES})
target_link_libraries(monoclecameramanager ${Qt5Gui_LIBRARIES})
target_link_libraries(monoclecameramanager debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE})
target_link_libraries(monoclecameramanager debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(monoclecameramanager debug ${Boost_FILESYSTEM_LIBRARY_DEBUG} optimized ${Boost_FILESYSTEM_LIBRARY_RELEASE})
target_link_libraries(monoclecameramanager debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(monoclecameramanager debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(monoclecameramanager debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(monoclecameramanager debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE})
target_link_libraries(monoclecameramanager Qt5::Core)
target_link_libraries(monoclecameramanager Qt5::Gui)
target_link_libraries(monoclecameramanager Qt5::Network)
target_link_libraries(monoclecameramanager Qt5::OpenGL)
target_link_libraries(monoclecameramanager Qt5::Widgets)
target_link_libraries(monoclecameramanager freetype)
target_link_libraries(monoclecameramanager libcurl)
target_link_libraries(monoclecameramanager libmonoclecameramanager)
target_link_libraries(monoclecameramanager libonvifclient)
target_link_libraries(monoclecameramanager libonviftypes)
target_link_libraries(monoclecameramanager network-uri)
target_link_libraries(monoclecameramanager pugixml)
target_link_libraries(monoclecameramanager libsocket)
target_link_libraries(monoclecameramanager libwsdiscover)
target_link_libraries(monoclecameramanager libutility)
target_link_libraries(monoclecameramanager zlibstatic)

if(WIN32)
  target_link_libraries(monoclecameramanager ${FFMPEG_LIBRARIES})
  target_link_libraries(monoclecameramanager Bcrypt.lib)
  target_link_libraries(monoclecameramanager Iphlpapi.lib)
  target_link_libraries(monoclecameramanager netapi32.lib)
  target_link_libraries(monoclecameramanager Qt5::Gui_GLESv2)
  target_link_libraries(monoclecameramanager Secur32.lib)
  target_link_libraries(monoclecameramanager wbemuuid.lib)
  target_link_libraries(monoclecameramanager Ws2_32.lib)
  target_link_libraries(monoclecameramanager Wsdapi)
else()
  target_link_libraries(monoclecameramanager ${X11_LIBRARIES})
  target_link_libraries(monoclecameramanager avcodec.so)
  target_link_libraries(monoclecameramanager avformat.so)
  target_link_libraries(monoclecameramanager pthread)
  target_link_libraries(monoclecameramanager swscale.so)
  target_link_libraries(monoclecameramanager avutil.so)
endif()
