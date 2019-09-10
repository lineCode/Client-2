cmake_minimum_required(VERSION 3.0)

########## UNIT ##########

add_test(librtspunit librtspunit --gtest_output=xml:results.xml)

add_executable(librtspunit
main.hpp
main.cpp
streamtest.cpp
streamtest.hpp
headers/testauthenticationtype.cpp
headers/testcontenttype.cpp
headers/testinterleaved.cpp
headers/testmode.cpp
headers/testparameter.cpp
headers/testparameters.cpp
headers/testprotocoltype.cpp
headers/testrange.cpp
headers/testrequesttype.cpp
headers/testroutingtype.cpp
headers/testrtpinfo.cpp
headers/testssrc.cpp
headers/testtransport.cpp
headers/testtransportdata.cpp
headers/testwwwauthenticate.cpp
rtcp/testchunk.cpp
rtcp/testreportblock.cpp
rtcp/testrtcppacket.cpp
rtcp/testsourcedescriptionitem.cpp
rtp/testrtponvifjpegpacket.cpp
rtp/testrtponvifpacket.cpp
rtp/testrtppacket.cpp
rtspconnectiontest.hpp
rtspconnectiontest.cpp
rtspservertest.hpp
rtspservertest.cpp
sdp/testaddresstype.cpp
sdp/testbandwidth.cpp
sdp/testbandwidthtype.cpp
sdp/testconnection.cpp
sdp/testfmtp.cpp
sdp/testmedia.cpp
sdp/testmediadescription.cpp
sdp/testmediaprotocol.cpp
sdp/testmediatype.cpp
sdp/testnettype.cpp
sdp/testorigin.cpp
sdp/testrange.cpp
sdp/testrtpmap.cpp
sdp/testsdp.cpp
sdp/testtiming.cpp
testresponse.cpp
testrequest.cpp
testrtsp.hpp
testserver.hpp
testserver.cpp
testsession.hpp
testsession.cpp)

add_sanitizers(librtspunit)

if(WIN32)
  target_compile_definitions(librtspunit PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(librtspunit PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET librtspunit APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif(WIN32)

set_property(TARGET librtspunit PROPERTY CXX_STANDARD 17)

if(UNIX)
  add_custom_command(TARGET librtspunit POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tsan-suppression.txt $<TARGET_FILE_DIR:librtspunit>/tsan-suppression.txt)
  add_custom_command(TARGET librtspunit POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tsan.sh $<TARGET_FILE_DIR:librtspunit>/tsan.sh)
endif(UNIX)

##### INCLUDES #####

include_directories(librtspunit ${Boost_INCLUDE_DIRS})
include_directories(librtspunit ${gtest_SOURCE_DIR}/include)
include_directories(librtspunit ${OPENSSL_INCLUDE_DIR})
include_directories(librtspunit ${PROJECT_BINARY_DIR}/curl/include/curl)
include_directories(librtspunit ${PROJECT_BINARY_DIR}/zlib)
include_directories(librtspunit ../../../curl/include)
include_directories(librtspunit ../../../librtsp/include)
include_directories(librtspunit ../../../libsocket/include)
include_directories(librtspunit ../../../libutility/include)
include_directories(librtspunit ../../../uri/include)
include_directories(librtspunit ../../../zlib)

##### LIBRARIES #####

if(WIN32)
  target_link_libraries(librtspunit Iphlpapi.lib)
  target_link_libraries(librtspunit netapi32.lib)
  target_link_libraries(librtspunit wbemuuid.lib)
  target_link_libraries(librtspunit Wsdapi)
  target_link_libraries(librtspunit Ws2_32.lib)
endif(WIN32)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(librtspunit pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(librtspunit debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE})
target_link_libraries(librtspunit debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(librtspunit debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(librtspunit debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(librtspunit debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(librtspunit debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE})
target_link_libraries(librtspunit ${OPENSSL_LIBRARIES})
target_link_libraries(librtspunit gtest)
target_link_libraries(librtspunit libcurl)
target_link_libraries(librtspunit librtsp)
target_link_libraries(librtspunit libsocket)
target_link_libraries(librtspunit libutility)
target_link_libraries(librtspunit zlibstatic)
target_link_libraries(librtspunit network-uri)

if(SANITIZE_THREAD)
  target_link_libraries(librtspunit tsan)
endif(SANITIZE_THREAD)

if(WIN32)
  target_link_libraries(librtspunit Ws2_32.lib)
endif()
