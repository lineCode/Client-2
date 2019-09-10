cmake_minimum_required(VERSION 3.0)

########## UNIT ##########

add_test(libonvifserverunit libonvifserverunit --gtest_output=xml:results.xml)

add_executable(libonvifserverunit
dummydeviceservice.hpp
dummydeviceservice.cpp
dummyservice.hpp
dummyservice.cpp
main.hpp
main.cpp
testdeviceservice.hpp
testdeviceservice.cpp
testhttpserver.hpp
testhttpserver.cpp)

add_sanitizers(libonvifserverunit)

if(WIN32)
  target_compile_definitions(libonvifserverunit PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libonvifserverunit PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libonvifserverunit APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libonvifserverunit PROPERTY CXX_STANDARD 17)

##### INCLUDES #####

include_directories(libonvifserverunit ${Boost_INCLUDE_DIRS})
include_directories(libonvifserverunit ${gtest_SOURCE_DIR}/include)
include_directories(libonvifserverunit ${OPENSSL_INCLUDE_DIR})
include_directories(libonvifserverunit ${PROJECT_BINARY_DIR}/curl/include/curl)
include_directories(libonvifserverunit ${PROJECT_BINARY_DIR}/Libevent/include)
include_directories(libonvifserverunit ${PROJECT_BINARY_DIR}/zlib)
include_directories(libonvifserverunit ../../../curl/include)
include_directories(libonvifserverunit ../../../Libevent/include)
include_directories(libonvifserverunit ../../../libonvifclient/include)
include_directories(libonvifserverunit ../../../libonvifserver/include)
include_directories(libonvifserverunit ../../../libonviftypes/include)
include_directories(libonvifserverunit ../../../librtsp/include)
include_directories(libonvifserverunit ../../../libsocket/include)
include_directories(libonvifserverunit ../../../libutility/include)
include_directories(libonvifserverunit ../../../pugixml/src)
include_directories(libonvifserverunit ../../../zlib)

##### LIBRARIES #####

if(WIN32)
  target_link_libraries(libonvifserverunit Iphlpapi.lib)
  target_link_libraries(libonvifserverunit netapi32.lib)
  target_link_libraries(libonvifserverunit wbemuuid.lib)
  target_link_libraries(libonvifserverunit Wsdapi)
  target_link_libraries(libonvifserverunit Ws2_32.lib)
endif(WIN32)

target_link_libraries(libonvifserverunit ${OPENSSL_LIBRARIES})
target_link_libraries(libonvifserverunit debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libonvifserverunit debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(libonvifserverunit debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libonvifserverunit debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libonvifserverunit gtest)
target_link_libraries(libonvifserverunit libcurl)
target_link_libraries(libonvifserverunit libonvifclient)
target_link_libraries(libonvifserverunit libonvifserver)
target_link_libraries(libonvifserverunit librtsp)
target_link_libraries(libonvifserverunit libsocket)
target_link_libraries(libonvifserverunit libutility)
target_link_libraries(libonvifserverunit zlibstatic)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(libonvifserverunit pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)
