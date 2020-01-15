cmake_minimum_required(VERSION 3.0)

########## LICENSE ##########

add_library(liblicense STATIC
include/license/database.hpp
include/license/license.hpp
include/license/licensekey.hpp
include/license/liblicense.hpp
include/license/productkey.hpp
include/license/user.hpp
src/database.cpp
src/license.cpp
src/licensekey.cpp
src/liblicense.cpp
src/productkey.cpp
src/user.cpp)

add_sanitizers(liblicense)

if(WIN32)
  target_compile_definitions(liblicense PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(liblicense PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET liblicense APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET liblicense PROPERTY CXX_STANDARD 17)

set_target_properties(liblicense PROPERTIES PREFIX "")
set_target_properties(liblicense PROPERTIES DEBUG_POSTFIX "")
set_target_properties(liblicense PROPERTIES RELEASE_POSTFIX "")

##### INCLUDES #####

include_directories(liblicense ${Boost_INCLUDE_DIRS})
include_directories(liblicense ${OPENSSL_INCLUDE_DIR})
include_directories(liblicense ${PROJECT_BINARY_DIR}/curl/include/curl)
include_directories(liblicense ${PROJECT_BINARY_DIR}/zlib)
include_directories(liblicense include)
include_directories(liblicense ../curl/include)
include_directories(liblicense ../pugixml/src)
include_directories(liblicense ../libutility/include)
include_directories(liblicense ../zlib)

##### LIBRARIES #####

target_link_libraries(liblicense ${OPENSSL_LIBRARIES})
target_link_libraries(liblicense debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE})
target_link_libraries(liblicense debug ${Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG} optimized ${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE})
target_link_libraries(liblicense debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE})
target_link_libraries(liblicense debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE})
target_link_libraries(liblicense libutility)
target_link_libraries(liblicense zlibstatic)
