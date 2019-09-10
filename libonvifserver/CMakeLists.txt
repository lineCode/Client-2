cmake_minimum_required(VERSION 3.0)

########## SERVER ##########

add_library(libonvifserver STATIC
include/onvifserver/deviceioserviceinterface.hpp
include/onvifserver/deviceserviceinterface.hpp
include/onvifserver/eventserviceinterface.hpp
include/onvifserver/header.hpp
include/onvifserver/httpserver.hpp
include/onvifserver/nonce.hpp
include/onvifserver/onvifserver.hpp
include/onvifserver/onvifservice.hpp
include/onvifserver/service.hpp
include/onvifserver/receiverserviceinterface.hpp
include/onvifserver/recordingserviceinterface.hpp
include/onvifserver/replayserviceinterface.hpp
include/onvifserver/searchserviceinterface.hpp
include/onvifserver/service.hpp
include/onvifserver/serverresponse.hpp
include/onvifserver/soapfault.hpp
include/onvifserver/user.hpp
src/deviceioserviceinterface.cpp
src/deviceserviceinterface.cpp
src/eventserviceinterface.cpp
src/header.cpp
src/httpserver.cpp
src/onvifservice.cpp
src/nonce.cpp
src/onvifserver.cpp
src/onvifservice.cpp
src/service.cpp
src/receiverserviceinterface.cpp
src/recordingserviceinterface.cpp
src/replayserviceinterface.cpp
src/searchserviceinterface.cpp
src/service.cpp
src/serverresponse.cpp
src/soapfault.cpp
src/user.cpp)

add_sanitizers(libonvifserver)

set_property(TARGET libonvifserver PROPERTY CXX_STANDARD 17)

##### INCLUDES #####

include_directories(libonvifserver ${Boost_INCLUDE_DIRS})
include_directories(libonvifserver ${OPENSSL_INCLUDE_DIR})
include_directories(libonvifserver ${PROJECT_BINARY_DIR}/Libevent/include)
include_directories(libonvifserver ${PROJECT_BINARY_DIR}/zlib)
include_directories(libonvifserver ../curl/include)
include_directories(libonvifserver ../Libevent/include)
include_directories(libonvifserver ../libonviftypes/include)
include_directories(libonvifserver ../librtsp/include)
include_directories(libonvifserver ../libsocket/include)
include_directories(libonvifserver ../libutility/include)
include_directories(libonvifserver ../pugixml/src)
include_directories(libonvifserver ../uri/include)
include_directories(libonvifserver ../zlib)
include_directories(libonvifserver include)

if(WIN32)
  include_directories(libonvifserver ${PROJECT_BINARY_DIR}/Libevent/WIN32-Code/nmake)
endif()

##### LIBRARIES #####

target_link_libraries(libonvifserver ${OPENSSL_LIBRARIES})
target_link_libraries(libonvifserver event_extra)
target_link_libraries(libonvifserver libonviftypes)
target_link_libraries(libonvifserver librtsp)
target_link_libraries(libonvifserver libsocket)
target_link_libraries(libonvifserver libutility)
target_link_libraries(libonvifserver network-uri)
target_link_libraries(libonvifserver pugixml)
target_link_libraries(libonvifserver zlibstatic)
