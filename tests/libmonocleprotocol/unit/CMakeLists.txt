cmake_minimum_required(VERSION 3.0)

########## UNIT ##########

add_test(libmonocleprotocolunit libmonocleprotocolunit --gtest_output=xml:results.xml)

add_executable(libmonocleprotocolunit
main.hpp
main.cpp
testclient.hpp
testclient.cpp
testconnection.hpp
testconnection.cpp
testprotocol.hpp
testprotocol.cpp
testserver.hpp
testserver.cpp)

add_sanitizers(libmonocleprotocolunit)

if(WIN32)
  target_compile_definitions(libmonocleprotocolunit PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libmonocleprotocolunit PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libmonocleprotocolunit APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libmonocleprotocolunit PROPERTY CXX_STANDARD 17)

##### INCLUDES #####

include_directories(libmonocleprotocolunit ${Boost_INCLUDE_DIRS})
include_directories(libmonocleprotocolunit ${gtest_SOURCE_DIR}/include)
include_directories(libmonocleprotocolunit ${OPENSSL_INCLUDE_DIR})
include_directories(libmonocleprotocolunit ${PROJECT_BINARY_DIR}/zlib)
include_directories(libmonocleprotocolunit ../../../flatbuffers/include)
include_directories(libmonocleprotocolunit ../../../libmonocleprotocol/include)
include_directories(libmonocleprotocolunit ../../../libsocket/include)
include_directories(libmonocleprotocolunit ../../../libutility/include)
include_directories(libmonocleprotocolunit ../../../zlib)

##### LIBRARIES #####

if(WIN32)
  target_link_libraries(libmonocleprotocolunit Iphlpapi.lib)
  target_link_libraries(libmonocleprotocolunit netapi32.lib)
  target_link_libraries(libmonocleprotocolunit wbemuuid.lib)
  target_link_libraries(libmonocleprotocolunit Wsdapi)
  target_link_libraries(libmonocleprotocolunit Ws2_32.lib)
endif(WIN32)

target_link_libraries(libmonocleprotocolunit ${OPENSSL_LIBRARIES})
target_link_libraries(libmonocleprotocolunit debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE})
target_link_libraries(libmonocleprotocolunit debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libmonocleprotocolunit debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(libmonocleprotocolunit debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libmonocleprotocolunit debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libmonocleprotocolunit debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE})
target_link_libraries(libmonocleprotocolunit gtest)
target_link_libraries(libmonocleprotocolunit libmonocleprotocol)
target_link_libraries(libmonocleprotocolunit libsocket)
target_link_libraries(libmonocleprotocolunit libutility)
target_link_libraries(libmonocleprotocolunit zlibstatic)

if(SANITIZE_THREAD)
  target_link_libraries(libmonocleprotocolunit tsan)
endif(SANITIZE_THREAD)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(libmonocleprotocolunit pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(WIN32)
  target_link_libraries(libmonocleprotocolunit Ws2_32.lib)
endif()
