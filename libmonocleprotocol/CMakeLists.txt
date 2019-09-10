cmake_minimum_required(VERSION 3.0)

########## LIBMONOCLEPROTOCOL ##########

add_library(libmonocleprotocol STATIC
include/monocleprotocol/client/client.hpp
include/monocleprotocol/client/connection.hpp
include/monocleprotocol/client/connectionblock.hpp
include/monocleprotocol/client/response.hpp
include/monocleprotocol/client/signal.hpp
include/monocleprotocol/monocleprotocol.hpp
include/monocleprotocol/server/connection.hpp
include/monocleprotocol/server/server.hpp
src/client/client.cpp
src/client/connection.cpp
src/client/connectionblock.cpp
src/client/response.cpp
src/monocleprotocol.cpp
src/server/connection.cpp
src/server/server.cpp)

add_sanitizers(libmonocleprotocol)

if(WIN32)
  target_compile_definitions(libmonocleprotocol PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libmonocleprotocol PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libmonocleprotocol APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libmonocleprotocol PROPERTY CXX_STANDARD 17)

set_target_properties(libmonocleprotocol PROPERTIES PREFIX "")
set_target_properties(libmonocleprotocol PROPERTIES DEBUG_POSTFIX "")
set_target_properties(libmonocleprotocol PROPERTIES RELEASE_POSTFIX "")

##### INCLUDES #####

include_directories(libmonocleprotocol ${Boost_INCLUDE_DIRS})
include_directories(libmonocleprotocol ${OPENSSL_INCLUDE_DIR})
include_directories(libmonocleprotocol ${PROJECT_BINARY_DIR}/zlib)
include_directories(libmonocleprotocol include)
include_directories(libmonocleprotocol ../flatbuffers/include)
include_directories(libmonocleprotocol ../libsocket/include)
include_directories(libmonocleprotocol ../libutility/include)
include_directories(libmonocleprotocol ../uri/include)
include_directories(libmonocleprotocol ../zlib)

##### LIBRARIES #####

target_link_libraries(libmonocleprotocol ${OPENSSL_LIBRARIES})
target_link_libraries(libmonocleprotocol debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE})
target_link_libraries(libmonocleprotocol debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libmonocleprotocol debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libmonocleprotocol debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libmonocleprotocol debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE})
target_link_libraries(libmonocleprotocol libsocket)
target_link_libraries(libmonocleprotocol libutility)
target_link_libraries(libmonocleprotocol network-uri)
target_link_libraries(libmonocleprotocol zlibstatic)
