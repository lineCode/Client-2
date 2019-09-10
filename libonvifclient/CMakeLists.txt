cmake_minimum_required(VERSION 3.0)

########## CLIENT ##########

add_library(libonvifclient STATIC
include/onvifclient/analyticsclient.hpp
include/onvifclient/client.hpp
include/onvifclient/connection.hpp
include/onvifclient/connectionblock.hpp
include/onvifclient/deviceclient.hpp
include/onvifclient/deviceioclient.hpp
include/onvifclient/eventclient.hpp
include/onvifclient/imagingclient.hpp
include/onvifclient/mediaclient.hpp
include/onvifclient/onvifclient.hpp
include/onvifclient/ptzclient.hpp
include/onvifclient/receiverclient.hpp
include/onvifclient/recordingclient.hpp
include/onvifclient/replayclient.hpp
include/onvifclient/response.hpp
include/onvifclient/scopedconnection.hpp
include/onvifclient/searchclient.hpp
include/onvifclient/signal.hpp
src/analytics/getservicecapabilitiesresponse.cpp
src/analyticsclient.cpp
src/connection.cpp
src/connectionblock.cpp
src/device/addipaddressfilterresponse.cpp
src/device/addscopesresponse.cpp
src/device/createcertificateresponse.cpp
src/device/createstorageconfigurationresponse.cpp
src/device/createusersresponse.cpp
src/device/deletecertificatesresponse.cpp
src/device/deletedot1xconfigurationresponse.cpp
src/device/deletestorageconfigurationresponse.cpp
src/device/deleteusersresponse.cpp
src/device/getaccesspolicyresponse.cpp
src/device/getcacertificatesresponse.cpp
src/device/getcapabilitiesresponse.cpp
src/device/getcertificateinformationresponse.cpp
src/device/getcertificatesresponse.cpp
src/device/getcertificatesstatusresponse.cpp
src/device/getclientcertificatemoderesponse.cpp
src/device/getdeviceinformationresponse.cpp
src/device/getdiscoverymoderesponse.cpp
src/device/getdnsresponse.cpp
src/device/getdot11capabilitiesresponse.cpp
src/device/getdot11statusresponse.cpp
src/device/getdot1xconfigurationresponse.cpp
src/device/getdot1xconfigurationsresponse.cpp
src/device/getdpaddressesresponse.cpp
src/device/getdynamicdnsresponse.cpp
src/device/getendpointreferenceresponse.cpp
src/device/gethostnameresponse.cpp
src/device/getipaddressfilterresponse.cpp
src/device/getnetworkdefaultgatewayresponse.cpp
src/device/getnetworkinterfacesresponse.cpp
src/device/getnetworkprotocolsresponse.cpp
src/device/getntpresponse.cpp
src/device/getpkcs10requestresponse.cpp
src/device/getrelayoutputsresponse.cpp
src/device/getremotediscoverymoderesponse.cpp
src/device/getremoteuserresponse.cpp
src/device/restoresystemresponse.cpp
src/device/getscopesresponse.cpp
src/device/getservicecapabilitiesresponse.cpp
src/device/getservicesresponse.cpp
src/device/getstorageconfigurationresponse.cpp
src/device/getstorageconfigurationsresponse.cpp
src/device/getsystembackupresponse.cpp
src/device/getsystemdateandtimeresponse.cpp
src/device/getsystemlogresponse.cpp
src/device/getsystemsupportinformationresponse.cpp
src/device/getsystemurisresponse.cpp
src/device/getusersresponse.cpp
src/device/getwsdlurlresponse.cpp
src/device/loadcertificatesresponse.cpp
src/device/loadcacertificatesresponse.cpp
src/device/loadcertificatewithprivatekeyresponse.cpp
src/device/getzeroconfigurationresponse.cpp
src/device/removeipaddressfilterresponse.cpp
src/device/removescopesresponse.cpp
src/device/scanavailabledot11networksresponse.cpp
src/device/sendauxiliarycommandresponse.cpp
src/device/setaccesspolicyresponse.cpp
src/device/setcertificatesstatusresponse.cpp
src/device/setclientcertificatemoderesponse.cpp
src/device/setdiscoverymoderesponse.cpp
src/device/setdnsresponse.cpp
src/device/setdot1xconfigurationresponse.cpp
src/device/setdpaddressesresponse.cpp
src/device/setdynamicdnsresponse.cpp
src/device/sethostnameresponse.cpp
src/device/sethostnamefromdhcpresponse.cpp
src/device/setipaddressfilterresponse.cpp
src/device/setnetworkdefaultgatewayresponse.cpp
src/device/setnetworkinterfacesresponse.cpp
src/device/setnetworkprotocolsresponse.cpp
src/device/setntpresponse.cpp
src/device/setrelayoutputsettingsresponse.cpp
src/device/setremotediscoverymoderesponse.cpp
src/device/setsystemdateandtimeresponse.cpp
src/device/setsystemfactorydefaultresponse.cpp
src/device/setuserresponse.cpp
src/device/setzeroconfigurationresponse.cpp
src/device/systemrebootresponse.cpp
src/deviceclient.cpp
src/deviceio/getaudiooutputconfigurationoptionsresponse.cpp
src/deviceio/getaudiooutputconfigurationresponse.cpp
src/deviceio/getaudiooutputsresponse.cpp
src/deviceio/getaudiosourceconfigurationoptionsresponse.cpp
src/deviceio/getaudiosourceconfigurationresponse.cpp
src/deviceio/getaudiosourcesresponse.cpp
src/deviceio/getdigitalinputconfigurationoptions.cpp
src/deviceio/getdigitalinputsresponse.cpp
src/deviceio/getrelayoutputoptionsresponse.cpp
src/deviceio/getrelayoutputsresponse.cpp
src/deviceio/getserialportconfigurationoptionsresponse.cpp
src/deviceio/getserialportconfigurationoptionsresponse.cpp
src/deviceio/getserialportsresponse.cpp
src/deviceio/getserialportconfigurationresponse.cpp
src/deviceio/getservicecapabilitiesresponse.cpp
src/deviceio/setaudiooutputconfigurationresponse.cpp
src/deviceio/setaudiosourceconfigurationresponse.cpp
src/deviceio/setdigitalinputconfigurationsresponse.cpp
src/deviceio/setrelayoutputsettingsresponse.cpp
src/deviceio/setrelayoutputstateresponse.cpp
src/deviceio/setserialportconfigurationresponse.cpp
src/deviceioclient.cpp
src/event/createpullpointsubscriptionresponse.cpp
src/event/geteventpropertiesresponse.cpp
src/event/getservicecapabilitiesresponse.cpp
src/event/pullmessagesresponse.cpp
src/eventclient.cpp
src/imaging/getimagingsettingsresponse.cpp
src/imaging/getoptionsresponse.cpp
src/imaging/getservicecapabilitiesresponse.cpp
src/imaging/setimagingsettingsresponse.cpp
src/imagingclient.cpp
src/media/addaudiodecoderconfigurationresponse.cpp
src/media/addaudioencoderconfigurationresponse.cpp
src/media/addaudiooutputconfigurationresponse.cpp
src/media/addaudiosourceconfigurationresponse.cpp
src/media/addmetadataconfigurationresponse.cpp
src/media/addptzconfigurationresponse.cpp
src/media/addvideoanalyticsconfigurationresponse.cpp
src/media/addvideoencoderconfigurationresponse.cpp
src/media/addvideosourceconfigurationresponse.cpp
src/media/createprofileresponse.cpp
src/media/deleteprofileresponse.cpp
src/media/getaudiodecoderconfigurationresponse.cpp
src/media/getaudiodecoderconfigurationoptionsresponse.cpp
src/media/getaudiodecoderconfigurationsresponse.cpp
src/media/getaudioencoderconfigurationresponse.cpp
src/media/getaudioencoderconfigurationoptionsresponse.cpp
src/media/getaudioencoderconfigurationsresponse.cpp
src/media/getaudiooutputconfigurationresponse.cpp
src/media/getaudiooutputconfigurationoptionsresponse.cpp
src/media/getaudiooutputconfigurationsresponse.cpp
src/media/getaudiosourcesresponse.cpp
src/media/getaudiosourceconfigurationresponse.cpp
src/media/getaudiosourceconfigurationoptionsresponse.cpp
src/media/getaudiosourceconfigurationsresponse.cpp
src/media/getcompatibleaudiodecoderconfigurationsresponse.cpp
src/media/getcompatibleaudioencoderconfigurationsresponse.cpp
src/media/getcompatibleaudiooutputconfigurationsresponse.cpp
src/media/getcompatibleaudiosourceconfigurationsresponse.cpp
src/media/getcompatiblemetadataconfigurationsresponse.cpp
src/media/getcompatiblevideoanalyticsconfigurationsresponse.cpp
src/media/getcompatiblevideoencoderconfigurationsresponse.cpp
src/media/getcompatiblevideosourceconfigurationsresponse.cpp
src/media/getmetadataconfigurationresponse.cpp
src/media/getmetadataconfigurationoptionsresponse.cpp
src/media/getmetadataconfigurationsresponse.cpp
src/media/getprofileresponse.cpp
src/media/getprofilesresponse.cpp
src/media/getservicecapabilitiesresponse.cpp
src/media/getsnapshoturiresponse.cpp
src/media/getstreamuriresponse.cpp
src/media/getvideoanalyticsconfigurationsresponse.cpp
src/media/getvideoencoderconfigurationoptionsresponse.cpp
src/media/getvideoencoderconfigurationresponse.cpp
src/media/getvideoencoderconfigurationsresponse.cpp
src/media/getvideosourceconfigurationoptionsresponse.cpp
src/media/getvideosourceconfigurationresponse.cpp
src/media/getvideosourceconfigurationsresponse.cpp
src/media/getvideosourcemodesresponse.cpp
src/media/getvideosourcesresponse.cpp
src/media/removeaudiodecoderconfigurationresponse.cpp
src/media/removeaudioencoderconfigurationresponse.cpp
src/media/removeaudiooutputconfigurationresponse.cpp
src/media/removeaudiosourceconfigurationresponse.cpp
src/media/removemetadataconfigurationresponse.cpp
src/media/removeptzconfigurationresponse.cpp
src/media/removevideoanalyticsconfigurationresponse.cpp
src/media/removevideoencoderconfigurationresponse.cpp
src/media/removevideosourceconfigurationresponse.cpp
src/media/setaudiodecoderconfigurationresponse.cpp
src/media/setaudioencoderconfigurationresponse.cpp
src/media/setaudiooutputconfigurationresponse.cpp
src/media/setaudiosourceconfigurationresponse.cpp
src/media/setmetadataconfigurationresponse.cpp
src/media/setsynchronizationpointresponse.cpp
src/media/setvideoencoderconfigurationresponse.cpp
src/media/setvideosourceconfigurationresponse.cpp
src/media/startmulticaststreamingresponse.cpp
src/media/stopmulticaststreamingresponse.cpp
src/mediaclient.cpp
src/onvifclient.cpp
src/ptz/continuousmoveresponse.cpp
src/ptz/getcompatibleconfigurationsresponse.cpp
src/ptz/getconfigurationresponse.cpp
src/ptz/getconfigurationoptionsresponse.cpp
src/ptz/getconfigurationsresponse.cpp
src/ptz/getnodesresponse.cpp
src/ptz/getservicecapabilitiesresponse.cpp
src/ptz/setconfigurationresponse.cpp
src/ptz/stopresponse.cpp
src/ptzclient.cpp
src/receiver/configurereceiverresponse.cpp
src/receiver/createreceiverresponse.cpp
src/receiver/deletereceiverresponse.cpp
src/receiver/getreceiverresponse.cpp
src/receiver/getreceiversresponse.cpp
src/receiver/getreceiverstateresponse.cpp
src/receiver/getservicecapabilitiesresponse.cpp
src/receiver/setreceivermoderesponse.cpp
src/receiverclient.cpp
src/recording/createrecordingresponse.cpp
src/recording/createrecordingjobresponse.cpp
src/recording/createtrackresponse.cpp
src/recording/deleterecordingjobresponse.cpp
src/recording/deleterecordingresponse.cpp
src/recording/deletetrackresponse.cpp
src/recording/exportrecordeddataresponse.cpp
src/recording/getexportrecordeddatastateresponse.cpp
src/recording/getrecordingconfigurationresponse.cpp
src/recording/getrecordingjobconfigurationresponse.cpp
src/recording/getrecordingjobsresponse.cpp
src/recording/getrecordingjobstateresponse.cpp
src/recording/getrecordingoptionsresponse.cpp
src/recording/getrecordingsresponse.cpp
src/recording/getservicecapabilitiesresponse.cpp
src/recording/gettrackconfigurationresponse.cpp
src/recording/setrecordingconfigurationresponse.cpp
src/recording/setrecordingjobconfigurationresponse.cpp
src/recording/setrecordingjobmoderesponse.cpp
src/recording/settrackconfigurationresponse.cpp
src/recording/stopexportrecordeddataresponse.cpp
src/recordingclient.cpp
src/replay/getreplayuriresponse.cpp
src/replay/getservicecapabilitiesresponse.cpp
src/replayclient.cpp
src/scopedconnection.cpp
src/search/getservicecapabilitiesresponse.cpp
src/searchclient.cpp)

add_sanitizers(libonvifclient)

if(WIN32)
  target_compile_definitions(libonvifclient PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libonvifclient PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libonvifclient APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libonvifclient PROPERTY CXX_STANDARD 17)

set_target_properties(libonvifclient PROPERTIES PREFIX "")
set_target_properties(libonvifclient PROPERTIES DEBUG_POSTFIX "")
set_target_properties(libonvifclient PROPERTIES RELEASE_POSTFIX "")

##### INCLUDES #####

include_directories(libonvifclient ${Boost_INCLUDE_DIRS})
include_directories(libonvifclient ${OPENSSL_INCLUDE_DIR})
include_directories(libonvifclient ${PROJECT_BINARY_DIR}/curl/include/curl)
include_directories(libonvifclient ${PROJECT_BINARY_DIR}/zlib)
include_directories(libonvifclient ../curl/include)
include_directories(libonvifclient ../libonviftypes/include)
include_directories(libonvifclient ../libsocket/include)
include_directories(libonvifclient ../libutility/include)
include_directories(libonvifclient ../pugixml/src)
include_directories(libonvifclient include)
include_directories(libonvifclient ../zlib)

##### LIBRARIES #####

target_link_libraries(libonvifclient ${OPENSSL_LIBRARIES})
target_link_libraries(libonvifclient debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE})
target_link_libraries(libonvifclient debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libonvifclient debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libonvifclient debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libonvifclient debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE})
target_link_libraries(libonvifclient libcurl)
target_link_libraries(libonvifclient libonviftypes)
target_link_libraries(libonvifclient libutility)
target_link_libraries(libonvifclient pugixml)
target_link_libraries(libonvifclient zlibstatic)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(libonvifclient pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(WIN32)
  target_link_libraries(libonvifclient Ws2_32.lib)
endif()


