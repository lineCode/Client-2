cmake_minimum_required(VERSION 3.0)

########## CLIENT ##########

add_library(sqlite3
sqlite3.c
sqlite3.h)

if(WIN32)
  target_compile_definitions(sqlite3 PUBLIC WINVER=0x0601 _WIN32_WINNT=0x0601)
  set_target_properties(sqlite3 PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG:FASTLINK")
  set_property(TARGET sqlite3 APPEND PROPERTY LINK_FLAGS_RELEASE /LTCG)
endif()

set_property(TARGET sqlite3 PROPERTY CXX_STANDARD 17)

##### INCLUDES #####



##### LIBRARIES #####


