cmake_minimum_required(VERSION 3.0)

########## INTEGRATION ##########

add_executable(libonvifclientstress
main.hpp
main.cpp
testclientstress.hpp
testclientstress.cpp)

add_sanitizers(libonvifclientstress)

if(WIN32)
  target_compile_definitions(libonvifclientstress PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libonvifclientstress PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libonvifclientstress APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libonvifclientstress PROPERTY CXX_STANDARD 17)

##### INCLUDES #####

include_directories(libonvifclientstress ${Boost_INCLUDE_DIRS})
include_directories(libonvifclientstress ${gtest_SOURCE_DIR}/include)
include_directories(libonvifclientstress ${OPENSSL_INCLUDE_DIR})
include_directories(libonvifclientstress ${PROJECT_BINARY_DIR}/curl/include/curl)
include_directories(libonvifclientstress ${PROJECT_BINARY_DIR}/zlib)
include_directories(libonvifclientstress ../../../curl/include)
include_directories(libonvifclientstress ../../../pugixml/src)
include_directories(libonvifclientstress ../../../libonvifclient/include)
include_directories(libonvifclientstress ../../../libonviftypes/include)
include_directories(libonvifclientstress ../../../libsocket/include)
include_directories(libonvifclientstress ../../../libutility/include)
include_directories(libonvifclientstress ../../../zlib)

##### LIBRARIES #####

if(WIN32)
  target_link_libraries(libonvifclientstress Iphlpapi.lib)
  target_link_libraries(libonvifclientstress netapi32.lib)
  target_link_libraries(libonvifclientstress wbemuuid.lib)
  target_link_libraries(libonvifclientstress Wsdapi)
  target_link_libraries(libonvifclientstress Ws2_32.lib)
endif(WIN32)

target_link_libraries(libonvifclientstress debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE})
target_link_libraries(libonvifclientstress debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libonvifclientstress debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(libonvifclientstress debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libonvifclientstress debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libonvifclientstress debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE})
target_link_libraries(libonvifclientstress ${OPENSSL_LIBRARIES})
target_link_libraries(libonvifclientstress gtest)
target_link_libraries(libonvifclientstress libcurl)
target_link_libraries(libonvifclientstress pugixml)
target_link_libraries(libonvifclientstress libonvifclient)
target_link_libraries(libonvifclientstress libonvifserver)
target_link_libraries(libonvifclientstress libonviftypes)
target_link_libraries(libonvifclientstress libsocket)
target_link_libraries(libonvifclientstress libutility)
target_link_libraries(libonvifclientstress zlibstatic)

if(WIN32)
  target_link_libraries(libonvifclientstress Ws2_32.lib)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(libonvifclientstress pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)
