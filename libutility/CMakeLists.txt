cmake_minimum_required(VERSION 3.0)

########## UTILITY ##########

add_library(libutility STATIC
include/utility/cpuid.hpp
include/utility/ioservicepool.hpp
include/utility/pipeclient.hpp
include/utility/pipeserver.hpp
include/utility/privatekey.hpp
include/utility/publickey.hpp
include/utility/registry.hpp
include/utility/service.hpp
include/utility/servicemgr.hpp
include/utility/utility.hpp
include/utility/version.hpp
src/cpuid.cpp
src/ioservicepool.cpp
src/pipeclient.cpp
src/pipeserver.cpp
src/privatekey.cpp
src/publickey.cpp
src/registry.cpp
src/service.cpp
src/servicemgr.cpp
src/utility.cpp
src/version.cpp)

add_sanitizers(libutility)

if(WIN32)
  target_compile_definitions(libutility PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libsocket PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libsocket APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libutility PROPERTY CXX_STANDARD 17)

set_target_properties(libutility PROPERTIES PREFIX "")
set_target_properties(libutility PROPERTIES DEBUG_POSTFIX "")
set_target_properties(libutility PROPERTIES RELEASE_POSTFIX "")

##### INCLUDES #####

include_directories(libutility ${Boost_INCLUDE_DIRS})
include_directories(libutility ${OPENSSL_INCLUDE_DIR})
include_directories(libutility ${PROJECT_BINARY_DIR}/zlib) 
include_directories(libutility include)
include_directories(libutility ../zlib)

##### LIBRARIES #####

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(libutility pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(libutility debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libutility debug ${Boost_FILESYSTEM_LIBRARY_DEBUG} optimized ${Boost_FILESYSTEM_LIBRARY_RELEASE})
target_link_libraries(libutility debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(libutility debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libutility debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libutility ${OPENSSL_LIBRARIES})
target_link_libraries(libutility zlibstatic)
