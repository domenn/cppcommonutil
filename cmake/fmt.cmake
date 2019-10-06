
# set(CMK_SCRIPT_VERBOSE "on")
set(CMK_SCRIPT_VERBOSE "")

if (${CMK_SCRIPT_VERBOSE})
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/util/get_target_props.cmake)
endif ()

# Temporarily disable usage of seperate FMT. Problem with SPDLOG.

# if (DUTILCPP_ADD_SPDLOG OR DUTILCPP_ADD_FMT)
if("0" STREQUAL "1")
  ExternalProject_Add(
      e_fmt
      PREFIX ${PROJECT_CMK_EXT_PROJS_PRE}
      CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_CMK_EXT_PROJS_PRE} -DFMT_DOC:BOOL=0 -DFMT_TEST:BOOL=0 -DBUILD_SHARED_LIBS:BOOL=0
      GIT_REPOSITORY https://github.com/fmtlib/fmt.git
      GIT_TAG 6.0.0
      EXCLUDE_FROM_ALL TRUE
      BUILD_ALWAYS TRUE
  )
  
  add_dependencies(build_external e_fmt)
  if (${CMK_SCRIPT_VERBOSE})
    print_external_itm_many(e_fmt)
  endif ()
  
  ExternalProject_Get_Property(e_fmt INSTALL_DIR)
  
  set(FmtTestingVar ${INSTALL_DIR}/include/fmt)
  ###### print_any_var(FmtTestingVar)
  
  if (IS_DIRECTORY ${FmtTestingVar})
    find_package(fmt REQUIRED CONFIG
        # PATHS
        # ${PROJECT_CMK_EXT_PROJS_PRE}/lib64/cmake/fmt
        HINTS
        ${PROJECT_CMK_EXT_PROJS_PRE}
        # PATH_SUFFIXES
        NO_DEFAULT_PATH)
    if (${CMK_SCRIPT_VERBOSE})
      message(STATUS "fmt at ${fmt_DIR}")
      print_target_properties(fmt::fmt)
    endif ()
    # message(WARNING "Fmt Link")
    target_link_libraries(dCppUtilsLib PUBLIC fmt::fmt)
  else ()
    message(WARNING "fmt not build! Run build_external cmake target!\n  Testing dir: ${FmtTestingVar}")
  endif ()
endif ()