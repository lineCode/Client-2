cmake_minimum_required(VERSION 3.0)

########## SOCKET ##########

add_library(libsocket STATIC
include/socket/connection.hpp
include/socket/connectionblock.hpp
include/socket/proxyparams.hpp
include/socket/proxytype.hpp
include/socket/scopedconnection.hpp
include/socket/socket.hpp
include/socket/tcpsocket.hpp
src/connection.cpp
src/connectionblock.cpp
src/proxyparams.cpp
src/proxytype.cpp
src/scopedconnection.cpp
src/socket.cpp
src/tcpsocket.cpp)

add_sanitizers(libsocket)

if(WIN32)
  target_compile_definitions(libsocket PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libsocket PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libsocket APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libsocket PROPERTY CXX_STANDARD 17)

set_target_properties(libsocket PROPERTIES PREFIX "")
set_target_properties(libsocket PROPERTIES DEBUG_POSTFIX "")
set_target_properties(libsocket PROPERTIES RELEASE_POSTFIX "")

##### INCLUDES #####

include_directories(libsocket ${Boost_INCLUDE_DIRS})
include_directories(libsocket ${OPENSSL_INCLUDE_DIR})
include_directories(libsocket ${PROJECT_BINARY_DIR}/zlib)
include_directories(libsocket ../libutility/include)
include_directories(libsocket ../zlib)
include_directories(libsocket include)

##### LIBRARIES #####

target_link_libraries(libsocket ${OPENSSL_LIBRARIES})
target_link_libraries(libsocket debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libsocket debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(libsocket debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libsocket debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libsocket gtest)
target_link_libraries(libsocket libutility)
target_link_libraries(libsocket zlibstatic)
