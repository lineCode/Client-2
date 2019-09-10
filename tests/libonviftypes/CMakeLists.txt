cmake_minimum_required(VERSION 3.0)

########## UNIT ##########

add_test(libonviftypesunit libonviftypesunit --gtest_output=xml:results.xml)

add_executable(libonviftypesunit
main.hpp
main.cpp
analytics/testcapabilities.cpp
device/testdeviceservicecapabilities.cpp
device/testmisccapabilities.cpp
device/testnetworkcapabilities.cpp
device/testsecuritycapabilities.cpp
device/testservice.cpp
device/teststorageconfiguration.cpp
device/teststorageconfigurationdata.cpp
device/testsystemcapabilities.cpp
deviceio/testaudiooutputconfiguration.cpp
deviceio/testcapabilities.cpp
deviceio/testdigitalinputconfigurationinputoptions.cpp
deviceio/testparitybit.cpp
deviceio/testparitybitlist.cpp
deviceio/testrelayoutputoptions.cpp
deviceio/testserialdata.cpp
deviceio/testserialport.cpp
deviceio/testserialportconfiguration.cpp
deviceio/testserialportconfigurationoptions.cpp
deviceio/testserialporttype.cpp
event/testcapabilities.cpp
imaging/testcapabilities.cpp
media/testcapabilities.cpp
media/testencodingtypes.cpp
media/testvideosourcemode.cpp
ptz/testcapabilities.cpp
receiver/testcapabilities.cpp
receiver/testreceiver.cpp
receiver/testreceiverconfiguration.cpp
receiver/testreceivermode.cpp
receiver/testreceiverstate.cpp
receiver/testreceiverstateinformation.cpp
recording/testcapabilities.cpp
recording/testencodingtypes.cpp
recording/testgetrecordingsresponseitem.cpp
recording/testgettracksresponseitem.cpp
recording/testgettracksresponselist.cpp
recording/testjoboptions.cpp
recording/testrecordingconfiguration.cpp
recording/testrecordingjobconfiguration.cpp
recording/testrecordingjobmode.cpp
recording/testrecordingjobsource.cpp
recording/testrecordingjobstate.cpp
recording/testrecordingjobstateinformation.cpp
recording/testrecordingjobstatesource.cpp
recording/testrecordingjobstatetrack.cpp
recording/testrecordingjobstatetracks.cpp
recording/testrecordingjobtrack.cpp
recording/testrecordingoptions.cpp
recording/testtrackoptions.cpp
replay/testcapabilities.cpp
replay/testreplayconfiguration.cpp
search/testcapabilities.cpp
testaacdecoptions.cpp
testabsolutefocus.cpp
testabsolutefocusoptions.cpp
testanalyticscapabilities.cpp
testanalyticsdevicecapabilities.cpp
testanalyticsengineconfiguration.cpp
testattachmentdata.cpp
testaudiodecoderconfiguration.cpp
testaudioencoderconfiguration.cpp
testaudiodecoderconfigurationoptions.cpp
testaudioencoderconfigurationoption.cpp
testaudioencoderconfigurationoptions.cpp
testaudioencoding.cpp
testaudiooutputconfiguration.cpp
testaudiooutputconfigurationoptions.cpp
testaudiosource.cpp
testaudiosourceconfiguration.cpp
testaudiosourceconfigurationoptions.cpp
testbacklightcompensation.cpp
testbacklightcompensation20.cpp
testbacklightcompensationmode.cpp
testbacklightcompensationoptions20.cpp
testbackupfile.cpp
testbinarydata.cpp
testcapabilities.cpp
testcapabilitycategory.cpp
testcertificate.cpp
testcertificateinformation.cpp
testcertificatestatus.cpp
testcertificateusage.cpp
testcertificatewithprivatekey.cpp
testconfig.cpp
testcontinuousfocusoptions.cpp
testdate.cpp
testdatetime.cpp
testdatetimerange.cpp
testdatetimetype.cpp
testdefogging.cpp
testdefoggingmode.cpp
testdefaultptzspeed.cpp
testdevicecapabilities.cpp
testdeviceiocapabilities.cpp
testdiscoverymode.cpp
testdigitalidlestate.cpp
testdigitalinput.cpp
testdisplaycapabilities.cpp
testdnsinformation.cpp
testdot11authandmanagementsuite.cpp
testdot11availablenetworks.cpp
testdot11capabilities.cpp
testdot11cipher.cpp
testdot11configuration.cpp
testdot11pskset.cpp
testdot11securityconfiguration.cpp
testdot11securitymode.cpp
testdot11signalstrength.cpp
testdot11stationmode.cpp
testdot11status.cpp
testdot1xconfiguration.cpp
testdynamicdnsinformation.cpp
testdynamicdnstype.cpp
testduplex.cpp
testduration.cpp
testdurationrange.cpp
testeapmethodconfiguration.cpp
testeflipmode.cpp
testeventcapabilities.cpp
testelement.cpp
testelementitem.cpp
testelementitemdescription.cpp
testeventsubscription.cpp
testexposure.cpp
testexposure20.cpp
testexposuremode.cpp
testexposureoptions20.cpp
testexposurepriority.cpp
testfactorydefaulttype.cpp
testarrayoffileprogress.cpp
testfileprogress.cpp
testfindeventresult.cpp
testfindeventresultlist.cpp
testfindrecordingresultlist.cpp
testfloatattrlist.cpp
testfloatlist.cpp
testfloatrange.cpp
testfocusconfiguration.cpp
testfocusconfiguration20.cpp
testfocusmove.cpp
testfocusoptions20.cpp
testfocusstatus20.cpp
testg711decoptions.cpp
testg726decoptions.cpp
testh264configuration.cpp
testh264options.cpp
testh264options2.cpp
testh264profile.cpp
testhostnameinformation.cpp
testimagestabilization.cpp
testimagestabilizationmode.cpp
testimagestabilizationoptions.cpp
testimagingcapabilities.cpp
testimagingoptions20.cpp
testimagingsettings.cpp
testimagingsettings20.cpp
testimagingstatus20.cpp
testinclude.cpp
testintattrlist.cpp
testintlist.cpp
testintrange.cpp
testintrectangle.cpp
testintrectanglerange.cpp
testiocapabilities.cpp
testipaddress.cpp
testipaddressfilter.cpp
testipaddressfiltertype.cpp
testiptype.cpp
testipv4configuration.cpp
testipv4networkinterface.cpp
testipv4networkinterfacesetconfiguration.cpp
testipv6configuration.cpp
testipv6dhcpconfiguration.cpp
testipv6networkinterface.cpp
testipv6networkinterfacesetconfiguration.cpp
testitemlist.cpp
testircutfilterautoadjustment.cpp
testircutfilterautoadjustmentoptions.cpp
testircutfilterautoboundarytype.cpp
testircutfiltermode.cpp
testitemlistdescription.cpp
testjpegoptions.cpp
testjpegoptions2.cpp
testlayout.cpp
testmediacapabilities.cpp
testmediauri.cpp
testmessagedescription.cpp
testmetadatacompressiontype.cpp
testmetadataconfiguration.cpp
testmetadataconfigurationoptions.cpp
testmoveoptions20.cpp
testmovestatus.cpp
testmpeg4configuration.cpp
testmpeg4options.cpp
testmpeg4options2.cpp
testmpeg4profile.cpp
testmulticastconfiguration.cpp
testnetworkcapabilities.cpp
testnetworkgateway.cpp
testnetworkhost.cpp
testnetworkhosttype.cpp
testnetworkinterfaceconnectionsetting.cpp
testnetworkinterfacesetconfiguration.cpp
testnetworkinterfaceinfo.cpp
testnetworkinterfacelink.cpp
testnetworkprotocol.cpp
testnetworkprotocoltype.cpp
testnetworkzeroconfiguration.cpp
testnoisereduction.cpp
testnoisereductionoptions.cpp
testntpinformation.cpp
testonvifversion.cpp
testpanelayout.cpp
testpantiltlimits.cpp
testprefixedipv4address.cpp
testprefixedipv6address.cpp
testprofile.cpp
testprofilecapabilities.cpp
testptzcapabilities.cpp
testptzconfiguration.cpp
testptzconfigurationoptions.cpp
testptzcontroldirectionoptions.cpp
testptzfilter.cpp
testptznode.cpp
testptzpresettouroperation.cpp
testptzpresettoursupported.cpp
testptzspaces.cpp
testptzspeed.cpp
testptzstatusfilteroptions.cpp
testrealtimestreamingcapabilities.cpp
testreceivercapabilities.cpp
testrecordingcapabilities.cpp
testrecordinginformation.cpp
testrecordingsourceinformation.cpp
testrecordingsummary.cpp
testrectangle.cpp
testrelativefocus.cpp
testrelativefocusoptions20.cpp
testrelayidlestate.cpp
testrelaylogicalstate.cpp
testrelaymode.cpp
testrelayoutput.cpp
testrelayoutputsettings.cpp
testremoteuser.cpp
testreplaycapabilities.cpp
testreversemode.cpp
testrotate.cpp
testrotatemode.cpp
testrotateoptions.cpp
testruleengineconfiguration.cpp
testscope.cpp
testscopedefinition.cpp
testsearchcapabilities.cpp
testsearchscope.cpp
testsearchstate.cpp
testsecuritycapabilities.cpp
testsimpleitem.cpp
testsourcereference.cpp
testspace1ddescription.cpp
testspace2ddescription.cpp
teststreamingcapabilities.cpp
teststreamsetup.cpp
teststreamtype.cpp
teststringattrlist.cpp
testsupportinformation.cpp
testsystemcapabilities.cpp
testsystemdatetime.cpp
testsystemlogtype.cpp
testsystemlog.cpp
testsystemloguri.cpp
testsystemlogurilist.cpp
testtime.cpp
testtimezone.cpp
testtlsconfiguration.cpp
testtonecompensation.cpp
testtonecompensationmode.cpp
testtonecompensationoptions.cpp
testtrackconfiguration.cpp
testtrackinformation.cpp
testtracktype.cpp
testtransport.cpp
testtransportprotocol.cpp
testtypes.hpp
testuser.cpp
testusercredential.cpp
testuserlevel.cpp
testvector1d.cpp
testvector2d.cpp
testvideosource.cpp
testvideoanalyticsconfiguration.cpp
testvideoencoding.cpp
testvideoencoderconfiguration.cpp
testvideoencoderconfigurationoptions.cpp
testvideooutput.cpp
testvideooutputconfiguration.cpp
testvideooutputconfigurationoptions.cpp
testvideoratecontrol.cpp
testvideoresolution.cpp
testvideosourceconfigurationoptions.cpp
testvideosourceconfiguration.cpp
testwhitebalance.cpp
testwhitebalance20.cpp
testwhitebalancemode.cpp
testwhitebalanceoptions20.cpp
testwidedynamicmode.cpp
testwidedynamicrange.cpp
testwidedynamicrange20.cpp
testwidedynamicrangeoptions20.cpp
testzoomlimits.cpp
ws/testdatetime.cpp
ws/testdevice.cpp
ws/testdigitalinput.cpp
ws/testendpointreferencetype.cpp
ws/testfiltertype.cpp
ws/testmessagecontent.cpp
ws/testtopicexpression.cpp
ws/testtopicset.cpp
ws/testtrigger.cpp)

add_sanitizers(libonviftypesunit)

if(WIN32)
  target_compile_definitions(libonviftypesunit PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(libonviftypesunit PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET libonviftypesunit APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET libonviftypesunit PROPERTY CXX_STANDARD 17)

##### INCLUDES #####

include_directories(libonviftypesunit ${Boost_INCLUDE_DIRS})
include_directories(libonviftypesunit ${gtest_SOURCE_DIR}/include)
include_directories(libonviftypesunit ${OPENSSL_INCLUDE_DIR})
include_directories(libonviftypesunit ${PROJECT_BINARY_DIR}/zlib)
include_directories(libonviftypesunit ../../client)
include_directories(libonviftypesunit ../../libonviftypes/include)
include_directories(libonviftypesunit ../../libutility/include)
include_directories(libonviftypesunit ../../pugixml/src)
include_directories(libonviftypesunit ../../zlib)

##### LIBRARIES #####

if(WIN32)
  target_link_libraries(libonviftypesunit Iphlpapi.lib)
  target_link_libraries(libonviftypesunit netapi32.lib)
  target_link_libraries(libonviftypesunit wbemuuid.lib)
  target_link_libraries(libonviftypesunit Wsdapi)
  target_link_libraries(libonviftypesunit Ws2_32.lib)
endif(WIN32)

target_link_libraries(libonviftypesunit debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(libonviftypesunit debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(libonviftypesunit debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(libonviftypesunit ${OPENSSL_LIBRARIES})
target_link_libraries(libonviftypesunit gtest)
target_link_libraries(libonviftypesunit libcurl)
target_link_libraries(libonviftypesunit libonviftypes)
target_link_libraries(libonviftypesunit libutility)
target_link_libraries(libonviftypesunit pugixml)
target_link_libraries(libonviftypesunit zlibstatic)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(libonviftypesunit pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)
