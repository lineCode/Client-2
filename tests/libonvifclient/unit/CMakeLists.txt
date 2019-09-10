cmake_minimum_required(VERSION 3.0)

########## UNIT ##########

add_test(libonvifclientunit libonvifclientunit --gtest_output=xml:results.xml)

add_executable(libonvifclientunit
main.hpp
main.cpp
dummyanalyticsservice.hpp
dummyanalyticsservice.cpp
dummydeviceservice.hpp
dummydeviceservice.cpp
dummydeviceioservice.hpp
dummydeviceioservice.cpp
dummyeventservice.hpp
dummyeventservice.cpp
dummyimagingservice.hpp
dummyimagingservice.cpp
dummymediaservice.hpp
dummymediaservice.cpp
dummyptzservice.hpp
dummyptzservice.cpp
dummyreceiverservice.hpp
dummyreceiverservice.cpp
dummyrecordingservice.hpp
dummyrecordingservice.cpp
dummyreplayservice.hpp
dummyreplayservice.cpp
dummysearchservice.hpp
dummysearchservice.cpp
testclient.hpp
testanalyticsclient.hpp
testanalyticsclient.cpp
testdeviceclient.hpp
testdeviceclient.cpp
testdeviceioclient.hpp
testdeviceioclient.cpp
testeventclient.hpp
testeventclient.cpp
testimagingclient.hpp
testimagingclient.cpp
testmediaclient.hpp
testmediaclient.cpp
testptzclient.hpp
testptzclient.cpp
testreceiverclient.hpp
testreceiverclient.cpp
testrecordingclient.hpp
testrecordingclient.cpp
testreplayclient.hpp
testreplayclient.cpp
testsearchclient.hpp
testsearchclient.cpp)

add_sanitizers(libonvifclientunit)

if(WIN32)
  target_compile_definitions(libonvifclientunit PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libonvifclientunit PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libonvifclientunit APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libonvifclientunit PROPERTY CXX_STANDARD 17)

##### INCLUDES #####

include_directories(libonvifclientunit ${Boost_INCLUDE_DIRS})
include_directories(libonvifclientunit ${gtest_SOURCE_DIR}/include)
include_directories(libonvifclientunit ${OPENSSL_INCLUDE_DIR})
include_directories(libonvifclientunit ${PROJECT_BINARY_DIR}/curl/include/curl)
include_directories(libonvifclientunit ${PROJECT_BINARY_DIR}/Libevent/include)
include_directories(libonvifclientunit ${PROJECT_BINARY_DIR}/zlib)
include_directories(libonvifclientunit ../../../curl/include)
include_directories(libonvifclientunit ../../../Libevent/include)
include_directories(libonvifclientunit ../../../libonvifclient/include)
include_directories(libonvifclientunit ../../../libonvifserver/include)
include_directories(libonvifclientunit ../../../libonviftypes/include)
include_directories(libonvifclientunit ../../../librtsp/include)
include_directories(libonvifclientunit ../../../libsocket/include)
include_directories(libonvifclientunit ../../../libutility/include)
include_directories(libonvifclientunit ../../../pugixml/src)
include_directories(libonvifclientunit ../../../zlib)

##### LIBRARIES #####

if(WIN32)
  target_link_libraries(libonvifclientunit Iphlpapi.lib)
  target_link_libraries(libonvifclientunit netapi32.lib)
  target_link_libraries(libonvifclientunit wbemuuid.lib)
  target_link_libraries(libonvifclientunit Wsdapi)
  target_link_libraries(libonvifclientunit Ws2_32.lib)
endif(WIN32)

target_link_libraries(libonvifclientunit debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE})
target_link_libraries(libonvifclientunit debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libonvifclientunit debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(libonvifclientunit debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libonvifclientunit debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libonvifclientunit debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE})
target_link_libraries(libonvifclientunit ${OPENSSL_LIBRARIES})
target_link_libraries(libonvifclientunit gtest)
target_link_libraries(libonvifclientunit libcurl)
target_link_libraries(libonvifclientunit pugixml)
target_link_libraries(libonvifclientunit libonvifclient)
target_link_libraries(libonvifclientunit libonvifserver)
target_link_libraries(libonvifclientunit libonviftypes)
target_link_libraries(libonvifclientunit librtsp)
target_link_libraries(libonvifclientunit libsocket)
target_link_libraries(libonvifclientunit libutility)
target_link_libraries(libonvifclientunit zlibstatic)

if(WIN32)
  target_link_libraries(libonvifclientunit Ws2_32.lib)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(libonvifclientunit pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)
