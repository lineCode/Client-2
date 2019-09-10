cmake_minimum_required(VERSION 3.0)

########## SERVER ##########

add_library(librtsp STATIC
include/rtsp/client/client.hpp
include/rtsp/client/clientrequest.hpp
include/rtsp/client/connection.hpp
include/rtsp/client/connectionblock.hpp
include/rtsp/client/interleavedrtpclient.hpp
include/rtsp/client/scopedconnection.hpp
include/rtsp/client/rtpclient.hpp
include/rtsp/client/session.hpp
include/rtsp/client/signal.hpp
include/rtsp/client/udpmulticastrtpclient.hpp
include/rtsp/client/udpunicastrtpclient.hpp
include/rtsp/describeresponse.hpp
include/rtsp/getparameterresponse.hpp
include/rtsp/headers/authenticationtype.hpp
include/rtsp/headers/contenttype.hpp
include/rtsp/headers/interleaved.hpp
include/rtsp/headers/mode.hpp
include/rtsp/headers/parameter.hpp
include/rtsp/headers/parameters.hpp
include/rtsp/headers/protocoltype.hpp
include/rtsp/headers/range.hpp
include/rtsp/headers/requesttype.hpp
include/rtsp/headers/routingtype.hpp
include/rtsp/headers/ssrc.hpp
include/rtsp/headers/rtpinfo.hpp
include/rtsp/headers/transport.hpp
include/rtsp/headers/transportdata.hpp
include/rtsp/headers/wwwauthenticate.hpp
include/rtsp/optionsresponse.hpp
include/rtsp/pauseresponse.hpp
include/rtsp/playresponse.hpp
include/rtsp/response.hpp
include/rtsp/rtcp/chunk.hpp
include/rtsp/rtcp/reportblock.hpp
include/rtsp/rtcp/packet.hpp
include/rtsp/rtcp/packets.hpp
include/rtsp/rtcp/sourcedescriptionitem.hpp
include/rtsp/rtp/onvifjpegpacket.hpp
include/rtsp/rtp/onvifpacket.hpp
include/rtsp/rtp/packet.hpp
include/rtsp/rtsp.hpp
include/rtsp/rtsprequest.hpp
include/rtsp/rtspresponse.hpp
include/rtsp/sdp/addresstype.hpp
include/rtsp/sdp/bandwidth.hpp
include/rtsp/sdp/bandwidthtype.hpp
include/rtsp/sdp/connection.hpp
include/rtsp/sdp/fmtp.hpp
include/rtsp/sdp/media.hpp
include/rtsp/sdp/mediatype.hpp
include/rtsp/sdp/mediadescription.hpp
include/rtsp/sdp/mediaprotocol.hpp
include/rtsp/sdp/nettype.hpp
include/rtsp/sdp/origin.hpp
include/rtsp/sdp/range.hpp
include/rtsp/sdp/rtpmap.hpp
include/rtsp/sdp/sdp.hpp
include/rtsp/sdp/timing.hpp
include/rtsp/server/connection.hpp
include/rtsp/server/httpconnection.hpp
include/rtsp/server/interleaved.hpp
include/rtsp/server/rtspconnection.hpp
include/rtsp/server/server.hpp
include/rtsp/server/session.hpp
include/rtsp/server/stream.hpp
include/rtsp/setparameterresponse.hpp
include/rtsp/setupresponse.hpp
include/rtsp/teardownresponse.hpp
src/client/clientrequest.cpp
src/client/connection.cpp
src/client/connectionblock.cpp
src/client/scopedconnection.cpp
src/describeresponse.cpp
src/getparameterresponse.cpp
src/headers/authenticationtype.cpp
src/headers/contenttype.cpp
src/headers/interleaved.cpp
src/headers/mode.cpp
src/headers/parameter.cpp
src/headers/parameters.cpp
src/headers/protocoltype.cpp
src/headers/range.cpp
src/headers/requesttype.cpp
src/headers/routingtype.cpp
src/headers/ssrc.cpp
src/headers/rtpinfo.cpp
src/headers/transport.cpp
src/headers/transportdata.cpp
src/headers/wwwauthenticate.cpp
src/optionsresponse.cpp
src/pauseresponse.cpp
src/playresponse.cpp
src/rtcp/chunk.cpp
src/rtcp/reportblock.cpp
src/rtcp/packet.cpp
src/rtcp/packets.cpp
src/rtcp/sourcedescriptionitem.cpp
src/rtp/onvifjpegpacket.cpp
src/rtp/onvifpacket.cpp
src/rtp/packet.cpp
src/rtsp.cpp
src/rtsprequest.cpp
src/rtspresponse.cpp
src/setparameterresponse.cpp
src/setupresponse.cpp
src/teardownresponse.cpp
src/sdp/bandwidth.cpp
src/sdp/addresstype.cpp
src/sdp/bandwidthtype.cpp
src/sdp/connection.cpp
src/sdp/fmtp.cpp
src/sdp/media.cpp
src/sdp/mediadescription.cpp
src/sdp/mediatype.cpp
src/sdp/mediaprotocol.cpp
src/sdp/nettype.cpp
src/sdp/origin.cpp
src/sdp/range.cpp
src/sdp/rtpmap.cpp
src/sdp/sdp.cpp
src/sdp/timing.cpp
src/server/connection.cpp
src/server/httpconnection.cpp
src/server/interleaved.cpp
src/server/rtspconnection.cpp
src/server/session.cpp
src/server/stream.cpp
src/server/server.cpp)

add_sanitizers(librtsp)

if(WIN32)
  target_compile_definitions(librtsp PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(librtsp PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET librtsp APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET librtsp PROPERTY CXX_STANDARD 17)

set_target_properties(librtsp PROPERTIES PREFIX "")
set_target_properties(librtsp PROPERTIES DEBUG_POSTFIX "")
set_target_properties(librtsp PROPERTIES RELEASE_POSTFIX "")

##### INCLUDES #####

include_directories(librtsp ${Boost_INCLUDE_DIRS})
include_directories(librtsp ${OPENSSL_INCLUDE_DIR})
include_directories(librtsp ${PROJECT_BINARY_DIR}/zlib)
include_directories(librtsp .)
include_directories(librtsp ../libsocket/include)
include_directories(librtsp ../libutility/include)
include_directories(librtsp ../uri/include)
include_directories(librtsp ../zlib)
include_directories(librtsp include)

##### LIBRARIES #####

target_link_libraries(librtsp debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE})
target_link_libraries(librtsp debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(librtsp debug ${Boost_FILESYSTEM_LIBRARY_DEBUG} optimized ${Boost_FILESYSTEM_LIBRARY_RELEASE})
target_link_libraries(librtsp debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(librtsp debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(librtsp debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(librtsp debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE})
target_link_libraries(librtsp zlibstatic)
