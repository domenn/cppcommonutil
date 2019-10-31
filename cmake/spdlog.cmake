
if (DUTILCPP_ADD_SPDLOG)
  ExternalProject_Add(
      e_spdlog
      PREFIX ${PROJECT_CMK_EXT_PROJS_PRE}
      CMAKE_CACHE_ARGS -DSPDLOG_FMT_EXTERNAL:BOOL=0 -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_CMK_EXT_PROJS_PRE} -DSPDLOG_BUILD_EXAMPLES:BOOL=0 -DSPDLOG_BUILD_EXAMPLE:BOOL=0 -DSPDLOG_BUILD_SHARED:BOOL=0 -DSPDLOG_INSTALL:BOOL=0 -DSPDLOG_SANITIZE_ADDRESS:BOOL=0 -DSPDLOG_BUILD_EXAMPLE_HO:BOOL=0 -DSPDLOG_BUILD_BENCH:BOOL=0 -DSPDLOG_BUILD_TESTS:BOOL=0 -DSPDLOG_NO_EXCEPTIONS:BOOL=0 -DSPDLOG_BUILD_TESTS_HO:BOOL=0
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      GIT_REPOSITORY https://github.com/gabime/spdlog.git
      GIT_TAG v1.4.2
      EXCLUDE_FROM_ALL TRUE
  )

  add_dependencies(build_external e_spdlog)
  
  ExternalProject_Get_Property(e_spdlog SOURCE_DIR)

  #  message(STATUS spdlsrc ${SOURCE_DIR})
  
  ## No need to install this .. Disable find and install step.
  #  if(IS_DIRECTORY ${CMAKE_BINARY_DIR}/CmkExtProj/lib)
  #    find_package(spdlog CONFIG REQUIRED)
  #    print_target_properties(spdlog::spdlog)
  #    target_link_libraries(dCppUtilsLib PUBLIC spdlog::spdlog)
  #  endif()
  
  ## Alternate way: just use include dirs.
  target_include_directories(dCppUtilsLib PUBLIC ${SOURCE_DIR}/include)
  
  ## Add my util src to the main library
  target_sources(dCppUtilsLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/d_spdlog/spd_logging.cc
      ${CMAKE_CURRENT_SOURCE_DIR}/src/d_spdlog/colorful_sink_factory_singleton.cc
      )
  target_compile_definitions(dCppUtilsLib PUBLIC SPDLOG_ACTIVE_LEVEL=1 D_USING_SPDLOG)

endif ()