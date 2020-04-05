cmake_minimum_required(VERSION 3.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

########## WIDGET ##########

SET(LIBMONOCLE_HEADERS
include/monocleclient/aboutwindow.h
include/monocleclient/actions.h
include/monocleclient/browsefilestree.h
include/monocleclient/browsefilestreeitem.h
include/monocleclient/browsefileswindow.h
include/monocleclient/checkforupdate.h
include/monocleclient/connection.h
include/monocleclient/decoder.h
include/monocleclient/device.h
include/monocleclient/devicelogwindow.h
include/monocleclient/devicemgr.h
include/monocleclient/devicepropertieswindow.h
include/monocleclient/devicetree.h
include/monocleclient/devicetreedeviceitem.h
include/monocleclient/devicetreeitem.h
include/monocleclient/devicetreemapitem.h
include/monocleclient/devicetreemediaitem.h
include/monocleclient/devicetreemediarecordingitem.h
include/monocleclient/devicetreerecordingitem.h
include/monocleclient/devicetreerecordingtrackitem.h
include/monocleclient/discoverytree.h
include/monocleclient/editdevicewindow.h
include/monocleclient/exportprogresswindow.h
include/monocleclient/exportwindow.h
include/monocleclient/file.h
include/monocleclient/filterlineedit.h
include/monocleclient/findmotionplaybackwidget.h
include/monocleclient/findmotionvideowidget.h
include/monocleclient/findmotionwindow.h
include/monocleclient/findobjectplaybackwidget.h
include/monocleclient/findobjectvideowidget.h
include/monocleclient/findobjectwindow.h
include/monocleclient/group.h
include/monocleclient/h265decoder.h
include/monocleclient/h264decoder.h
include/monocleclient/imagebuffer.h
include/monocleclient/imagecache.h
include/monocleclient/infoformatwindow.h
include/monocleclient/layout.h
include/monocleclient/locationtree.h
include/monocleclient/locationtreemapitem.h
include/monocleclient/locationtreerecordingitem.h
include/monocleclient/locationtreeitem.h
include/monocleclient/log.h
include/monocleclient/logtable.h
include/monocleclient/mainwindow.h
include/monocleclient/managedevicewindow.h
include/monocleclient/managefilewindow.h
include/monocleclient/managefileswindow.h
include/monocleclient/managegroupwindow.h
include/monocleclient/managegroupswindow.h
include/monocleclient/managelayoutnamewindow.h
include/monocleclient/managelayoutswindow.h
include/monocleclient/managelayoutwindow.h
include/monocleclient/managemapswindow.h
include/monocleclient/managemapwindow.h
include/monocleclient/manageonvifdeviceipv4networkinterfaceconfigurationwindow.h
include/monocleclient/manageonvifdeviceipv4networkinterfaceconfigurationswindow.h
include/monocleclient/manageonvifmediaaudioencoderconfigurationwindow.h
include/monocleclient/manageonvifmediaaudioencoderconfigurationswindow.h
include/monocleclient/manageonvifmediaaudiooutputconfigurationwindow.h
include/monocleclient/manageonvifmediaaudiooutputconfigurationswindow.h
include/monocleclient/manageonvifmediaaudiosourceconfigurationwindow.h
include/monocleclient/manageonvifmediaaudiosourceconfigurationswindow.h
include/monocleclient/manageonvifmediacreateprofilewindow.h
include/monocleclient/manageonvifmediametadataconfigurationswindow.h
include/monocleclient/manageonvifmediametadataconfigurationwindow.h
include/monocleclient/manageonvifmediaprofileswindow.h
include/monocleclient/manageonvifmediaprofilewindow.h
include/monocleclient/manageonvifmediavideosourceconfigurationwindow.h
include/monocleclient/manageonvifmediavideosourceconfigurationswindow.h
include/monocleclient/manageonvifmediavideoencoderconfigurationwindow.h
include/monocleclient/manageonvifmediavideoencoderconfigurationswindow.h
include/monocleclient/manageonvifuserswindow.h
include/monocleclient/manageonvifuserwindow.h
include/monocleclient/manageonvifwindow.h
include/monocleclient/managerecordingswindow.h
include/monocleclient/managerecordingwindow.h
include/monocleclient/managetrackobjectdetectorwindow.h
include/monocleclient/managetrackfileswindow.h
include/monocleclient/managetrackfindonvifdevicediscoverytree.h
include/monocleclient/managetrackfindonvifdevicediscoverytreeitem.h
include/monocleclient/managetrackfindonvifdevicewindow.h
include/monocleclient/managetrackwindow.h
include/monocleclient/manageuserswindow.h
include/monocleclient/manageuserwindow.h
include/monocleclient/map.h
include/monocleclient/mapview.h
include/monocleclient/media.h
include/monocleclient/mediamgr.h
include/monocleclient/mediapropertiestree.h
include/monocleclient/mediapropertieswindow.h
include/monocleclient/mediaview.h
include/monocleclient/mjpegdecoder.h
include/monocleclient/mpeg4decoder.h
include/monocleclient/newcameraquestionwindow.h
include/monocleclient/objects.h
include/monocleclient/onvifuser.h
include/monocleclient/options.h
include/monocleclient/optionswindow.h
include/monocleclient/pipeclient.h
include/monocleclient/playbackwidget.h
include/monocleclient/receiver.h
include/monocleclient/recording.h
include/monocleclient/recordingblock.h
include/monocleclient/recordingblocks.h
include/monocleclient/recordingjob.h
include/monocleclient/recordingjobsource.h
include/monocleclient/recordingjobsourcetrack.h
include/monocleclient/recordinglogwindow.h
include/monocleclient/recordingtrack.h
include/monocleclient/runguard.h
include/monocleclient/savevideo.h
include/monocleclient/shaders.h
include/monocleclient/shortcut.h
include/monocleclient/shortcutmgr.h
include/monocleclient/singleton.h
include/monocleclient/stream.h
include/monocleclient/streamingprotocol.h
include/monocleclient/updatewindow.h
include/monocleclient/user.h
include/monocleclient/videochartview.h
include/monocleclient/videoview.h
include/monocleclient/videoviewpropertieswindow.h
include/monocleclient/videowidget.h
include/monocleclient/videowidgettoolbar.h
include/monocleclient/videowidgetgrid.h
include/monocleclient/videowidgetsmgr.h
include/monocleclient/videowindow.h
include/monocleclient/videowindowmgr.h
include/monocleclient/view.h)

SET(LIBMONOCLE_SOURCES
src/aboutwindow.cpp
src/actions.cpp
src/browsefilestree.cpp
src/browsefilestreeitem.cpp
src/browsefileswindow.cpp
src/checkforupdate.cpp
src/connection.cpp
src/decoder.cpp
src/device.cpp
src/devicelogwindow.cpp
src/devicemgr.cpp
src/devicepropertieswindow.cpp
src/devicetree.cpp
src/devicetreedeviceitem.cpp
src/devicetreeitem.cpp
src/devicetreemapitem.cpp
src/devicetreemediaitem.cpp
src/devicetreemediarecordingitem.cpp
src/devicetreerecordingitem.cpp
src/devicetreerecordingtrackitem.cpp
src/discoverytree.cpp
src/editdevicewindow.cpp
src/exportprogresswindow.cpp
src/exportwindow.cpp
src/file.cpp
src/filterlineedit.cpp
src/findmotionplaybackwidget.cpp
src/findmotionvideowidget.cpp
src/findmotionwindow.cpp
src/findobjectplaybackwidget.cpp
src/findobjectvideowidget.cpp
src/findobjectwindow.cpp
src/group.cpp
src/h265decoder.cpp
src/h264decoder.cpp
src/imagebuffer.cpp
src/imagecache.cpp
src/infoformatwindow.cpp
src/layout.cpp
src/locationtree.cpp
src/locationtreemapitem.cpp
src/locationtreerecordingitem.cpp
src/locationtreeitem.cpp
src/log.cpp
src/logtable.cpp
src/mainwindow.cpp
src/managedevicewindow.cpp
src/managefilewindow.cpp
src/managefileswindow.cpp
src/managegroupwindow.cpp
src/managegroupswindow.cpp
src/managelayoutnamewindow.cpp
src/managelayoutswindow.cpp
src/managelayoutwindow.cpp
src/managemapswindow.cpp
src/managemapwindow.cpp
src/manageonvifdeviceipv4networkinterfaceconfigurationwindow.cpp
src/manageonvifdeviceipv4networkinterfaceconfigurationswindow.cpp
src/manageonvifmediaaudioencoderconfigurationswindow.cpp
src/manageonvifmediaaudioencoderconfigurationwindow.cpp
src/manageonvifmediaaudiooutputconfigurationswindow.cpp
src/manageonvifmediaaudiooutputconfigurationwindow.cpp
src/manageonvifmediaaudiosourceconfigurationswindow.cpp
src/manageonvifmediaaudiosourceconfigurationwindow.cpp
src/manageonvifmediacreateprofilewindow.cpp
src/manageonvifmediametadataconfigurationswindow.cpp
src/manageonvifmediametadataconfigurationwindow.cpp
src/manageonvifmediavideosourceconfigurationswindow.cpp
src/manageonvifmediavideosourceconfigurationwindow.cpp
src/manageonvifmediavideoencoderconfigurationswindow.cpp
src/manageonvifmediavideoencoderconfigurationwindow.cpp
src/manageonvifmediaprofileswindow.cpp
src/manageonvifmediaprofilewindow.cpp
src/manageonvifuserswindow.cpp
src/manageonvifuserwindow.cpp
src/manageonvifwindow.cpp
src/managerecordingswindow.cpp
src/managerecordingwindow.cpp
src/managetrackobjectdetectorwindow.cpp
src/managetrackfileswindow.cpp
src/managetrackfindonvifdevicediscoverytree.cpp
src/managetrackfindonvifdevicediscoverytreeitem.cpp
src/managetrackfindonvifdevicewindow.cpp
src/managetrackwindow.cpp
src/manageuserswindow.cpp
src/manageuserwindow.cpp
src/map.cpp
src/mapview.cpp
src/media.cpp
src/mediamgr.cpp
src/mediapropertiestree.cpp
src/mediapropertieswindow.cpp
src/mediaview.cpp
src/mjpegdecoder.cpp
src/mpeg4decoder.cpp
src/newcameraquestionwindow.cpp
src/objects.cpp
src/onvifuser.cpp
src/options.cpp
src/optionswindow.cpp
src/pipeclient.cpp
src/playbackwidget.cpp
src/receiver.cpp
src/recording.cpp
src/recordingjob.cpp
src/recordingblock.cpp
src/recordingblocks.cpp
src/recordingjobsource.cpp
src/recordingjobsourcetrack.cpp
src/recordinglogwindow.cpp
src/recordingtrack.cpp
src/runguard.cpp
src/savevideo.cpp
src/shaders.cpp
src/shortcut.cpp
src/shortcutmgr.cpp
src/stream.cpp
src/streamingprotocol.cpp
src/updatewindow.cpp
src/user.cpp
src/videochartview.cpp
src/videoview.cpp
src/videoviewpropertieswindow.cpp
src/videowidget.cpp
src/videowidgettoolbar.cpp
src/videowidgetgrid.cpp
src/videowidgetsmgr.cpp
src/videowindow.cpp
src/videowindowmgr.cpp
src/view.cpp)

SET(LIBMONOCLE_FORMS
aboutwindow.ui
browsefileswindow.ui
devicelogwindow.ui
devicepropertieswindow.ui
editdevicewindow.ui
exportprogresswindow.ui
exportwindow.ui
findmotionwindow.ui
findobjectwindow.ui
infoformatwindow.ui
mainwindow.ui
managedevicewindow.ui
managefilewindow.ui
managefileswindow.ui
managegroupwindow.ui
managegroupswindow.ui
managelayoutnamewindow.ui
managelayoutswindow.ui
managelayoutwindow.ui
managemapswindow.ui
managemapwindow.ui
manageonvifdeviceipv4networkinterfaceconfigurationwindow.ui
manageonvifdeviceipv4networkinterfaceconfigurationswindow.ui
manageonvifmediaaudioencoderconfigurationswindow.ui
manageonvifmediaaudioencoderconfigurationwindow.ui
manageonvifmediaaudiooutputconfigurationswindow.ui
manageonvifmediaaudiooutputconfigurationwindow.ui
manageonvifmediaaudiosourceconfigurationswindow.ui
manageonvifmediaaudiosourceconfigurationwindow.ui
manageonvifmediacreateprofilewindow.ui
manageonvifmediametadataconfigurationswindow.ui
manageonvifmediametadataconfigurationwindow.ui
manageonvifmediaprofileswindow.ui
manageonvifmediaprofilewindow.ui
manageonvifmediavideosourceconfigurationswindow.ui
manageonvifmediavideosourceconfigurationwindow.ui
manageonvifmediavideoencoderconfigurationswindow.ui
manageonvifmediavideoencoderconfigurationwindow.ui
manageonvifuserswindow.ui
manageonvifuserwindow.ui
manageonvifwindow.ui
managerecordingswindow.ui
managerecordingwindow.ui
managetrackobjectdetectorwindow.ui
managetrackfileswindow.ui
managetrackfindonvifdevicewindow.ui
managetrackwindow.ui
manageuserswindow.ui
manageuserwindow.ui
mediapropertieswindow.ui
newcameraquestionwindow.ui
recordinglogwindow.ui
optionswindow.ui
updatewindow.ui
videowindow.ui
videoviewpropertieswindow.ui)

##### TRANSLATIONS #####

set(TRANSLATEFILES ${LIBMONOCLE_HEADERS} ${LIBMONOCLE_SOURCES} ${LIBMONOCLE_FORMS})
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

QT5_WRAP_UI(LIBMONOCLE_FORMS_HEADERS ${LIBMONOCLE_FORMS})
QT5_WRAP_CPP(LIBMONOCLE_HEADERS_MOC ${LIBMONOCLE_HEADERS})

add_library(libmonocleclient STATIC
${LIBMONOCLE_SOURCES}
${LIBMONOCLE_HEADERS}
${LIBMONOCLE_HEADERS_MOC}
${LIBMONOCLE_FORMS}
${LIBMONOCLE_FORMS_HEADERS}
${TRANSLATIONFILES})

add_sanitizers(libmonocleclient)

if(WIN32)
  target_compile_definitions(libmonocleclient PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libmonocleclient PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libmonocleclient APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libmonocleclient PROPERTY CXX_STANDARD 17)

set(TRANSLATIONSQM ${TRANSLATIONFILES} CACHE STRING "Translation files")

set_target_properties(libmonocleclient PROPERTIES PREFIX "")
set_target_properties(libmonocleclient PROPERTIES DEBUG_POSTFIX "")
set_target_properties(libmonocleclient PROPERTIES RELEASE_POSTFIX "")

##### INCLUDES #####

include_directories(libmonocleclient ${Boost_INCLUDE_DIRS})
include_directories(libmonocleclient ${CMAKE_CURRENT_BINARY_DIR})
include_directories(libmonocleclient ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(libmonocleclient ${CUDA_INCLUDE_DIRS})
include_directories(libmonocleclient ${FFMPEG_INCLUDE_DIR})
include_directories(libmonocleclient ${OPENSSL_INCLUDE_DIR})
include_directories(libmonocleclient ${PROJECT_BINARY_DIR}/curl/include/curl)
include_directories(libmonocleclient ${PROJECT_BINARY_DIR}/zlib)
include_directories(libmonocleclient ${Qt5Charts_INCLUDE_DIRS})
include_directories(libmonocleclient ${Qt5Core_INCLUDE_DIRS})
include_directories(libmonocleclient ${Qt5Network_INCLUDE_DIRS})
include_directories(libmonocleclient ${Qt5OpenGL_INCLUDE_DIRS})
include_directories(libmonocleclient ${Qt5Widgets_INCLUDE_DIRS})
include_directories(libmonocleclient ../curl/include)
include_directories(libmonocleclient ../flatbuffers/include)
include_directories(libmonocleclient ../freetype2/include)
include_directories(libmonocleclient ../libfile/include)
include_directories(libmonocleclient ../libmonocleprotocol/include)
include_directories(libmonocleclient ../libonvifclient/include)
include_directories(libmonocleclient ../libonviftypes/include)
include_directories(libmonocleclient ../librtsp/include)
include_directories(libmonocleclient ../libsocket/include)
include_directories(libmonocleclient ../libutility/include)
include_directories(libmonocleclient ../libwsdiscover/include)
include_directories(libmonocleclient ../pugixml/include)
include_directories(libmonocleclient ../sqlite3)
include_directories(libmonocleclient ../uri/include)
include_directories(libmonocleclient ../zlib)
include_directories(libmonocleclient include)

if(CMAKE_COMPILER_IS_GNUCXX)
  include_directories(libmonocleclient pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

##### LIBRARIES #####

target_link_libraries(libmonocleclient ${CUDA_LIBRARIES})
target_link_libraries(libmonocleclient ${OPENGL_LIBRARIES})
target_link_libraries(libmonocleclient ${OPENSSL_LIBRARIES})
target_link_libraries(libmonocleclient ${Qt5Gui_EGL_LIBRARIES})
target_link_libraries(libmonocleclient ${Qt5Gui_LIBRARIES})
target_link_libraries(libmonocleclient debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE})
target_link_libraries(libmonocleclient debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libmonocleclient debug ${Boost_FILESYSTEM_LIBRARY_DEBUG} optimized ${Boost_FILESYSTEM_LIBRARY_RELEASE})
target_link_libraries(libmonocleclient debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libmonocleclient debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libmonocleclient debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE})
target_link_libraries(libmonocleclient Qt5::Charts)
target_link_libraries(libmonocleclient Qt5::Core)
target_link_libraries(libmonocleclient Qt5::Gui)
target_link_libraries(libmonocleclient Qt5::Network)
target_link_libraries(libmonocleclient Qt5::OpenGL)
target_link_libraries(libmonocleclient Qt5::Widgets)
target_link_libraries(libmonocleclient freetype)
target_link_libraries(libmonocleclient libcurl)
target_link_libraries(libmonocleclient libfile)
target_link_libraries(libmonocleclient libmonocleprotocol)
target_link_libraries(libmonocleclient libonvifclient)
target_link_libraries(libmonocleclient libonviftypes)
target_link_libraries(libmonocleclient librtsp)
target_link_libraries(libmonocleclient libsocket)
target_link_libraries(libmonocleclient libwsdiscover)
target_link_libraries(libmonocleclient libutility)
target_link_libraries(libmonocleclient network-uri)
target_link_libraries(libmonocleclient zlibstatic)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(libmonocleclient pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(WIN32)
  target_link_libraries(libmonocleclient ${CMAKE_CURRENT_SOURCE_DIR}/../libmfx.lib)
  target_link_libraries(libmonocleclient ${FFMPEG_LIBRARIES})
  target_link_libraries(libmonocleclient Iphlpapi.lib)
  target_link_libraries(libmonocleclient netapi32.lib)
  target_link_libraries(libmonocleclient Qt5::Gui_GLESv2)
  target_link_libraries(libmonocleclient wbemuuid.lib)
  target_link_libraries(libmonocleclient Ws2_32.lib)
else()
  target_link_libraries(libmonocleclient ${X11_LIBRARIES})
  target_link_libraries(libmonocleclient avcodec.so)
  target_link_libraries(libmonocleclient avformat.so)
  target_link_libraries(libmonocleclient pthread)
  target_link_libraries(libmonocleclient swscale.so)
endif()
