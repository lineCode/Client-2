cmake_minimum_required(VERSION 3.0)

enable_testing()

add_test(libutilityunit libutilityunit --gtest_output=xml:results.xml)

add_executable(libutilityunit
testcpuid.cpp
testpipe.cpp
testpipeclient.hpp
testpipeclient.cpp
testpipeserver.hpp
testpipeserver.cpp
testprivatekey.cpp
testservice.cpp
testutility.cpp
testversion.cpp)

add_sanitizers(libutilityunit)

if(WIN32)
  target_compile_definitions(libutilityunit PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libutilityunit PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libutilityunit APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libutilityunit PROPERTY CXX_STANDARD 17)

##### INCLUDES #####

include_directories(libutilityunit ${Boost_INCLUDE_DIRS})
include_directories(libutilityunit ${gtest_SOURCE_DIR}/include)
include_directories(libutilityunit ${OPENSSL_INCLUDE_DIR})
include_directories(libutilityunit ${PROJECT_BINARY_DIR}/zlib)
include_directories(libutilityunit ../../libutility/include)
include_directories(libutilityunit ../../zlib)

##### LIBRARIES #####

if(WIN32)
  target_link_libraries(libutilityunit Iphlpapi.lib)
  target_link_libraries(libutilityunit netapi32.lib)
  target_link_libraries(libutilityunit wbemuuid.lib)
  target_link_libraries(libutilityunit Wsdapi)
  target_link_libraries(libutilityunit Ws2_32.lib)
endif(WIN32)

target_link_libraries(libutilityunit ${OPENSSL_LIBRARIES})
target_link_libraries(libutilityunit debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libutilityunit debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libutilityunit debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libutilityunit gtest)
target_link_libraries(libutilityunit libutility)
target_link_libraries(libutilityunit zlibstatic)
