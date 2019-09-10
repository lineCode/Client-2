cmake_minimum_required(VERSION 3.0)

########## CLIENT ##########

add_library(libwsdiscover STATIC
include/wsdiscover/wsdiscoverclient.hpp
include/wsdiscover/wsdiscoverserver.hpp
src/wsdiscoverserver.cpp
src/wsdiscoverclient.cpp)

add_sanitizers(libwsdiscover)

if (WIN32)
  target_compile_definitions(libwsdiscover PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libwsdiscover PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libwsdiscover APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif (WIN32)

set_property(TARGET libwsdiscover PROPERTY CXX_STANDARD 17)

set_target_properties(libwsdiscover PROPERTIES PREFIX "")
set_target_properties(libwsdiscover PROPERTIES DEBUG_POSTFIX "")
set_target_properties(libwsdiscover PROPERTIES RELEASE_POSTFIX "")

##### INCLUDES #####

include_directories(libwsdiscover ${Boost_INCLUDE_DIRS})
include_directories(libwsdiscover ${OPENSSL_INCLUDE_DIR})
include_directories(libwsdiscover ${PROJECT_BINARY_DIR}/zlib)
include_directories(libwsdiscover ../../../zlib)
include_directories(libwsdiscover include)
include_directories(libwsdiscover ../libutility/include)
include_directories(libwsdiscover ../pugixml/src)
include_directories(libwsdiscover ../uri/include)
include_directories(libwsdiscover ../utility)
include_directories(libwsdiscover ../zlib)

##### LIBRARIES #####

target_link_libraries(libwsdiscover pugixml)
target_link_libraries(libwsdiscover libutility)
target_link_libraries(libwsdiscover zlibstatic)
target_link_libraries(libwsdiscover network-uri)

