cmake_minimum_required(VERSION 3.0)

########## file ##########

add_library(libfile
include/file/file_generated.h
include/file/file.hpp
include/file/fileread.hpp
include/file/filewrite.hpp
src/file.cpp
src/fileread.cpp
src/filewrite.cpp)

add_sanitizers(libfile)

if(WIN32)
  target_compile_definitions(libfile PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libfile PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libfile APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libfile PROPERTY CXX_STANDARD 17)

set_target_properties(libfile PROPERTIES PREFIX "")
set_target_properties(libfile PROPERTIES DEBUG_POSTFIX "")
set_target_properties(libfile PROPERTIES RELEASE_POSTFIX "")

##### INCLUDES #####

include_directories(libfile ${Boost_INCLUDE_DIRS})
include_directories(libfile ${OPENSSL_INCLUDE_DIR})
include_directories(libfile ${PROJECT_BINARY_DIR}/zlib)
include_directories(libfile include)
include_directories(libfile ../flatbuffers/include)
include_directories(libfile ../libutility/include)
include_directories(libfile ../zlib)

##### LIBRARIES #####

target_link_libraries(libfile ${OPENSSL_LIBRARIES})
target_link_libraries(libfile debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libfile debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(libfile debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libfile debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libfile libutility)
target_link_libraries(libfile zlibstatic)
