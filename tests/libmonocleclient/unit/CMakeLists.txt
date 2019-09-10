cmake_minimum_required(VERSION 3.0)

########## UNIT ##########

add_test(libmonocleclientunit libmonocleclientunit --gtest_output=xml:results.xml)

add_executable(libmonocleclientunit
main.hpp
main.cpp)

add_sanitizers(libmonocleclientunit)

if(WIN32)
  target_compile_definitions(libmonocleclientunit PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libmonocleclientunit PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libmonocleclientunit APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libmonocleclientunit PROPERTY CXX_STANDARD 17)

##### INCLUDES #####

include_directories(libmonocleclientunit ${Boost_INCLUDE_DIRS})
include_directories(libmonocleclientunit ${gtest_SOURCE_DIR}/include)
include_directories(libmonocleclientunit ${PROJECT_BINARY_DIR}/Libevent/include)
include_directories(libmonocleclientunit ${PROJECT_BINARY_DIR}/zlib)
include_directories(libmonocleclientunit ../../../libmonocleclient/include)
include_directories(libmonocleclientunit ../../../libonviftypes/include)
include_directories(libmonocleclientunit ../../../libsocket/include)
include_directories(libmonocleclientunit ../../../Libevent/include)
include_directories(libmonocleclientunit ../../../pugixml/src)
include_directories(libmonocleclientunit ../../../sqlite3)
include_directories(libmonocleclientunit ../../../zlib)

##### LIBRARIES #####

target_link_libraries(libmonocleclientunit ${OPENSSL_LIBRARIES})
target_link_libraries(libmonocleclientunit debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libmonocleclientunit debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(libmonocleclientunit debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libmonocleclientunit debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libmonocleclientunit gtest)
target_link_libraries(libmonocleclientunit libmonocleclient)
target_link_libraries(libmonocleclientunit libonvifclient)
target_link_libraries(libmonocleclientunit libonviftypes)
target_link_libraries(libmonocleclientunit libsocket)
target_link_libraries(libmonocleclientunit pugixml)
target_link_libraries(libmonocleclientunit sqlite3)
target_link_libraries(libmonocleclientunit zlibstatic)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(libmonocleclientunit pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(WIN32)
  target_link_libraries(libmonocleclientunit ${CMAKE_CURRENT_SOURCE_DIR}/../../../libmfx.lib)
  target_link_libraries(libmonocleclientunit Ws2_32.lib)
else()
  target_link_libraries(libmonocleclientunit ${X11_LIBRARIES})
  target_link_libraries(libmonocleclientunit avcodec.so)
  target_link_libraries(libmonocleclientunit avformat.so)
  target_link_libraries(libmonocleclientunit pthread)
  target_link_libraries(libmonocleclientunit swscale.so)
  target_link_libraries(libmonocleclientunit avutil.so)
endif()
