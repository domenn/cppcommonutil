cmake_minimum_required(VERSION 3.14)
project(dCppUtilsLib)
set(CMAKE_CXX_STANDARD 17)

option(DUTILCPP_SHARED "Build as shared library (dll on Windows) instead of static lib." OFF)
option(DUTILCPP_ADD_SPDLOG "Use spdlog library. Implies FMT." OFF)
option(DUTILCPP_ADD_FMT "Use fmt library." OFF)
option(DUTILCPP_ADD_CLI11 "Use CLI11 library." ON)
option(DUTILCPP_ADD_PLATFORM_FOLDER "Use PLATFORM_FOLDER library." ON)
option(DUTILCPP_ADD_PROTOBUF "Use protocol buffers library." OFF)
option(DUTILCPP_ADD_SQLITE "SQLite3 lib." OFF)
option(DUTILCPP_ADD_SQLITECPP "SQLite3cpp wrapper lib. Implies DUTILCPP_ADD_SQLITE." OFF)

option(DUTILCPP_COMPILE_RUN_PROCESS "Enable process runner for Windows, Linux. Requires gtk and glib on linux (system install)." ON)


file(GLOB_RECURSE dCppUtilsLib_GLOB_SRC LIST_DIRECTORIES false "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    )

set(D_UTIL_LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/str_util.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/contrib/errnoname.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stopwatches/log_info_on_destruct_stopwatch.cc
    src/tracer_exception.cc
    )

if (DUTILCPP_SHARED)
  set(PRIVATE_SHARED_PREFIX SHARED)
else ()
  unset(PRIVATE_SHARED_PREFIX)
endif ()

add_library(dCppUtilsLib ${PRIVATE_SHARED_PREFIX}
    ${D_UTIL_LIB_SRC}
    )


target_compile_definitions(dCppUtilsLib PUBLIC -DUNICODE -D_UNICODE $<$<CONFIG:Release>:NDEBUG>)

if (MSVC)
  add_definitions(/MP)
  get_directory_property(OUT_CO_D_LIB COMPILE_OPTIONS)
  if (NOT "/utf-8" IN_LIST OUT_CO_D_LIB)
    add_compile_options(/utf-8)
  endif ()
endif ()
add_custom_target(
    build_external
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/util/cmake_common_defines.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/fmt.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/spdlog.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cli11.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/platform_folders.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/protobuf.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/sqlite.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/sqlitecpp.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/run_process.cmake)

target_include_directories(dCppUtilsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#if(${DUTILCPP_AS_SUBDIR})
#  add_library(dCppUtilsLib_sub INTERFACE)
#
#  target_include_directories(dCppUtilsLib_sub INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
#  target_link_libraries(dCppUtilsLib_sub INTERFACE dCppUtilsLib)
#
##  set_target_properties(dCppUtilsLib_sub PROPERTIES
##      INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
##      INTERFACE_LINK_LIBRARIES dCppUtilsLib
##      )
#endif()

