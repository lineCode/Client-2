cmake_minimum_required(VERSION 3.0)

########## fileunit ##########

add_test(libfileunit libfileunit --gtest_output=xml:results.xml)

add_executable(libfileunit
main.hpp
main.cpp)

add_sanitizers(libfileunit)

if(WIN32)
  target_compile_definitions(libfileunit PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libfileunit PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libfileunit APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

##### INCLUDES #####

include_directories(libfileunit ${Boost_INCLUDE_DIRS})
include_directories(libfileunit ${gtest_SOURCE_DIR}/include)
include_directories(libfileunit ${OPENSSL_INCLUDE_DIR})
include_directories(libfileunit ${PROJECT_BINARY_DIR}/zlib)
include_directories(libfileunit ../../libfile/include)
include_directories(libfileunit ../../flatbuffers/include)
include_directories(libfileunit ../../libutility/include)
include_directories(libfileunit ../../zlib)

##### LIBRARIES #####


target_link_libraries(libfileunit debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libfileunit debug ${Boost_FILESYSTEM_LIBRARY_DEBUG} optimized ${Boost_FILESYSTEM_LIBRARY_RELEASE})
target_link_libraries(libfileunit debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(libfileunit debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libfileunit debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libfileunit ${OPENSSL_LIBRARIES})
target_link_libraries(libfileunit gtest)
target_link_libraries(libfileunit libfile)
target_link_libraries(libfileunit libutility)
target_link_libraries(libfileunit zlibstatic)
