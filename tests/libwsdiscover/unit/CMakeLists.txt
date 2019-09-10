cmake_minimum_required(VERSION 3.0)

########## INTEGRATION ##########

add_test(libwsdiscoverunit libwsdiscoverunit --gtest_output=xml:results.xml)

add_executable(libwsdiscoverunit
main.hpp
main.cpp
testwsdiscoverserver.hpp
testwsdiscoverserver.cpp)

add_sanitizers(libwsdiscoverunit)

if(WIN32)
  target_compile_definitions(libwsdiscoverunit PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libwsdiscoverunit PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libwsdiscoverunit APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libwsdiscoverunit PROPERTY CXX_STANDARD 17)

##### INCLUDES #####

include_directories(libwsdiscoverunit ${OPENSSL_INCLUDE_DIR})
include_directories(libwsdiscoverunit ${Boost_INCLUDE_DIRS})
include_directories(libwsdiscoverunit ${gtest_SOURCE_DIR}/include)
include_directories(libwsdiscoverunit ${PROJECT_BINARY_DIR}/zlib)
include_directories(libwsdiscoverunit ../../curl/include)
include_directories(libwsdiscoverunit ../../../libutility/include)
include_directories(libwsdiscoverunit ../../../libwsdiscover/include)
include_directories(libwsdiscoverunit ../../../uri/include)
include_directories(libwsdiscoverunit ../../../zlib)

##### LIBRARIES #####

if(WIN32)
  target_link_libraries(libwsdiscoverunit Iphlpapi.lib)
  target_link_libraries(libwsdiscoverunit netapi32.lib)
  target_link_libraries(libwsdiscoverunit wbemuuid.lib)
  target_link_libraries(libwsdiscoverunit Wsdapi)
  target_link_libraries(libwsdiscoverunit Ws2_32.lib)
endif(WIN32)

target_link_libraries(libwsdiscoverunit ${OPENSSL_LIBRARIES})
target_link_libraries(libwsdiscoverunit debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libwsdiscoverunit debug ${Boost_FILESYSTEM_LIBRARY_DEBUG} optimized ${Boost_FILESYSTEM_LIBRARY_RELEASE})
target_link_libraries(libwsdiscoverunit debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libwsdiscoverunit debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libwsdiscoverunit gtest)
target_link_libraries(libwsdiscoverunit libcurl)
target_link_libraries(libwsdiscoverunit libwsdiscover)
target_link_libraries(libwsdiscoverunit network-uri)
target_link_libraries(libwsdiscoverunit pugixml)
target_link_libraries(libwsdiscoverunit zlibstatic)

if (WIN32)
  target_link_libraries(libwsdiscoverunit Wsdapi)
endif(WIN32)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(libwsdiscoverunit pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

