
if (DUTILCPP_COMPILE_RUN_PROCESS)
  target_sources(dCppUtilsLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/process_runner.cc)
  if (WIN32)
    target_sources(dCppUtilsLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/win/windows_process_runner.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/win/windows_error_format.cc)
  else ()
    find_package(PkgConfig)
    
    pkg_check_modules(GLIB2 REQUIRED IMPORTED_TARGET
        gio-2.0
        # glib-2.0
        )
    # pkg_check_modules(GTK2 gtk+-2.0)
    
    # target_include_directories(dCppUtilsLib PRIVATE ${GLIB2_INCLUDE_DIRS} ${GTK2_INCLUDE_DIRS})
    target_sources(dCppUtilsLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/linux.cc)
    target_link_libraries(dCppUtilsLib PUBLIC PkgConfig::GLIB2)
  endif ()

endif ()