
if (DUTILCPP_ADD_PLATFORM_FOLDER)
  
  set(THE_FILE_LIB_NAME e_platform_folders)
  
  ExternalProject_Add(
      ${THE_FILE_LIB_NAME}
      PREFIX ${PROJECT_CMK_EXT_PROJS_PRE}
      CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_CMK_EXT_PROJS_PRE} -DBUILD_TESTING:BOOL=0
      GIT_REPOSITORY https://github.com/sago007/PlatformFolders.git
      EXCLUDE_FROM_ALL TRUE
  )
  
  add_dependencies(build_external ${THE_FILE_LIB_NAME})
  ### print_external_itm_many(${THE_FILE_LIB_NAME})
  
  ExternalProject_Get_Property(${THE_FILE_LIB_NAME} INSTALL_DIR)
  
  set(LibDLedCheckVar ${INSTALL_DIR}/include/sago)
  
  if (IS_DIRECTORY ${LibDLedCheckVar})
    find_package(platform_folders REQUIRED
        #        PATHS
        #        ${PROJECT_CMK_EXT_PROJS_PRE}/cmake
        HINTS
        ${PROJECT_CMK_EXT_PROJS_PRE}
        NO_DEFAULT_PATH)
    target_link_libraries(dCppUtilsLib PUBLIC sago::platform_folders)
    target_sources(dCppUtilsLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/application_files_platform_folders.cc)
  else ()
    message(WARNING "${THE_FILE_LIB_NAME} not build! Run build_external cmake target!\n  Testing dir: ${LibDLedCheckVar}")
  endif ()
endif ()