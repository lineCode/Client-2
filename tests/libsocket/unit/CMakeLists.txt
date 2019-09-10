cmake_minimum_required(VERSION 3.0)

########## UNIT ##########

add_test(libsocketunit libsocketunit --gtest_output=xml:results.xml)

add_executable(libsocketunit
main.hpp
main.cpp
testproxyparams.cpp)

add_sanitizers(libsocketunit)

if(WIN32)
  target_compile_definitions(libsocketunit PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libsocketunit PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libsocketunit APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libsocketunit PROPERTY CXX_STANDARD 17)

##### INCLUDES #####

include_directories(libsocketunit ${Boost_INCLUDE_DIRS})
include_directories(libsocketunit ${gtest_SOURCE_DIR}/include)
include_directories(libsocketunit ${PROJECT_BINARY_DIR}/zlib)
include_directories(libsocketunit ../../../libsocket/include)
include_directories(libsocketunit ../../../zlib)

##### LIBRARIES #####

if(WIN32)
  target_link_libraries(libsocketunit Iphlpapi.lib)
  target_link_libraries(libsocketunit netapi32.lib)
  target_link_libraries(libsocketunit wbemuuid.lib)
  target_link_libraries(libsocketunit Wsdapi)
  target_link_libraries(libsocketunit Ws2_32.lib)
endif(WIN32)

target_link_libraries(libsocketunit ${OPENSSL_LIBRARIES})
target_link_libraries(libsocketunit debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libsocketunit debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(libsocketunit debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libsocketunit debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libsocketunit gtest)
target_link_libraries(libsocketunit libsocket)
target_link_libraries(libsocketunit zlibstatic)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(libsocketunit pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(WIN32)
  target_link_libraries(libsocketunit Ws2_32.lib)
endif()
