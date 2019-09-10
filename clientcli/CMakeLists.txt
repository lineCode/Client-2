cmake_minimum_required(VERSION 3.0)

########## CLIENT ##########

add_executable(clientcli
main.h
main.cpp)

add_sanitizers(clientcli)

if(WIN32)
  target_compile_definitions(clientcli PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(clientcli PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET clientcli APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET clientcli PROPERTY CXX_STANDARD 17)

##### INCLUDES #####

include_directories(clientcli ${Boost_INCLUDE_DIRS})
include_directories(clientcli ${CMAKE_CURRENT_BINARY_DIR})
include_directories(clientcli ${OPENSSL_INCLUDE_DIR})
include_directories(clientcli ${PROJECT_BINARY_DIR}/curl/include/curl)
include_directories(clientcli ${PROJECT_BINARY_DIR}/zlib)
include_directories(clientcli ../curl/include)
include_directories(clientcli ../libonvifclient/include)
include_directories(clientcli ../libonviftypes/include)
include_directories(clientcli ../libsocket/include)
include_directories(clientcli ../libutility/include)
include_directories(clientcli ../pugixml/src)
include_directories(clientcli ../wsdiscover)
include_directories(clientcli ../zlib)

##### LIBRARIES #####

if(WIN32)
  target_link_libraries(clientcli Iphlpapi.lib)
  target_link_libraries(clientcli netapi32.lib)
  target_link_libraries(clientcli wbemuuid.lib)
  target_link_libraries(clientcli Wsdapi)
  target_link_libraries(clientcli Ws2_32.lib)
endif(WIN32)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(clientcli pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(clientcli debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE})
target_link_libraries(clientcli debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(clientcli debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(clientcli debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(clientcli debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(clientcli debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE})
target_link_libraries(clientcli pugixml)
target_link_libraries(clientcli libcurl)
target_link_libraries(clientcli libonvifclient)
target_link_libraries(clientcli libsocket)
target_link_libraries(clientcli libonviftypes)
target_link_libraries(clientcli libutility)
target_link_libraries(clientcli libwsdiscover)
target_link_libraries(clientcli zlibstatic)

target_link_libraries(clientcli ${OPENSSL_LIBRARIES})

if(WIN32)
  target_link_libraries(clientcli Ws2_32.lib)
endif()
